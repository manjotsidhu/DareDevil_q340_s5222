/* 
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/kobject.h>
#include <linux/earlysuspend.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>

#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>
#include <mach/irqs.h>
#include <mach/eint.h>

#include <asm/io.h>
#include <cust_eint.h>
#include <linux/proc_fs.h>
#include "bu5203.h"
/******************************************************************************
 * configuration
*******************************************************************************/
/*----------------------------------------------------------------------------*/
#define APS_TAG                  "[BU5203] "
#define APS_FUN(f)               printk(KERN_INFO APS_TAG"%s\n", __FUNCTION__)
#define APS_ERR(fmt, args...)    printk(KERN_ERR  APS_TAG"%s %d : "fmt, __FUNCTION__, __LINE__, ##args)
#define APS_LOG(fmt, args...)    printk(KERN_ERR APS_TAG fmt, ##args)
#define APS_DBG(fmt, args...)    printk(KERN_INFO APS_TAG fmt, ##args)         


static	struct work_struct	eint_work;
static struct input_dev *bu5203_key_dev;
#if 1
static struct proc_dir_entry *bu5203_config_state = NULL;
#define BU5203_CONFIG_PROC_FILE     "bu5203_config"
#define HALL_CONFIG_MAX_LENGTH       256
char hall_buf[16]={"-1"};
static char config[HALL_CONFIG_MAX_LENGTH] = {0};
atomic_t hall_write_flag=ATOMIC_INIT(1);
int  bEnHall = 0;
#endif
extern int gBackLightLevel;
//#define KEY_SENSOR 238
//#define KEY_SENSOR 116  //power key
#define KEY_SENSOR 251 // define keycode for hall
static int enable_input_key = 1;
static s32 value_hall1_rev = -1;
//static s32 value_hall2_rev = 1;
static s32 hall_state = 1; //open status

static int bu5203_probe(struct platform_device *pdev);
static int bu5203_remove(struct platform_device *pdev);
/******************************************************************************
 * extern functions
*******************************************************************************/
extern void mt_eint_mask(unsigned int eint_num);
extern void mt_eint_unmask(unsigned int eint_num);
extern void mt_eint_set_hw_debounce(unsigned int eint_num, unsigned int ms);
extern void mt_eint_set_polarity(unsigned int eint_num, unsigned int pol);
extern unsigned int mt_eint_set_sens(unsigned int eint_num, unsigned int sens);
extern void mt_eint_registration(unsigned int eint_num, unsigned int flow, void (EINT_FUNC_PTR)(void), unsigned int is_auto_umask);
extern void mt_eint_print_status(void);


/*-------------------------------attribute file for debugging----------------------------------*/

/******************************************************************************
 * Sysfs attributes
*******************************************************************************/
/*----------------------------------------------------------------------------*/
static ssize_t bu5203_show_state(struct device_driver *ddri, char *buf)
{
	APS_LOG("hall_state = %d\n", hall_state);
	return snprintf(buf, PAGE_SIZE, "%d\n", hall_state);
}
/*----------------------------------------------------------------------------*/
static ssize_t bu5203_show_key(struct device_driver *ddri, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", enable_input_key);
}
/*----------------------------------------------------------------------------*/
static ssize_t bu5203_store_key(struct device_driver *ddri, const char *buf, size_t count)
{
	int enable;

	if(1 == sscanf(buf, "%d", &enable))
	{
		enable_input_key = enable;
	}
	else 
	{
		APS_ERR("invalid enable content: '%s', length = %d\n", buf, count);
	}
	return count;    
}
/*---------------------------------------------------------------------------------------*/
static DRIVER_ATTR(hall_state,     S_IWUSR | S_IRUGO, bu5203_show_state, NULL);
static DRIVER_ATTR(input_key_enable,    0666, bu5203_show_key, bu5203_store_key);
/*----------------------------------------------------------------------------*/
static struct driver_attribute *bu5203_attr_list[] = {
    &driver_attr_hall_state,
    &driver_attr_input_key_enable,
};

/*----------------------------------------------------------------------------*/
static int bu5203_create_attr(struct device_driver *driver) 
{
	int idx, err = 0;
	int num = (int)(sizeof(bu5203_attr_list)/sizeof(bu5203_attr_list[0]));
	if (driver == NULL)
	{
		return -EINVAL;
	}

	for(idx = 0; idx < num; idx++)
	{
		if((err = driver_create_file(driver, bu5203_attr_list[idx])))
		{            
			APS_ERR("driver_create_file (%s) = %d\n", bu5203_attr_list[idx]->attr.name, err);
			break;
		}
	}    
	return err;
}
/*----------------------------------------------------------------------------*/
	static int bu5203_delete_attr(struct device_driver *driver)
	{
	int idx ,err = 0;
	int num = (int)(sizeof(bu5203_attr_list)/sizeof(bu5203_attr_list[0]));

	if (!driver)
	return -EINVAL;

	for (idx = 0; idx < num; idx++) 
	{
		driver_remove_file(driver, bu5203_attr_list[idx]);
	}
	
	return err;
}
/*----------------------------------------------------------------------------*/
static struct platform_driver bu5203_hall_driver = {
	.probe      = bu5203_probe,
	.remove     = bu5203_remove,    
	.driver     = {
		.name  = "hall",
	}
};
static ssize_t hall_config_read_proc(struct file *file, char __user *page, size_t size, loff_t *ppos)
{
        char *ptr = page;
        char temp_data[2] = {'a','\0'};
        int i;
        u8 ascii = 0;
        if (*ppos)  // CMD call again
        {
                return 0;
        }
        APS_ERR("HALL 0:%d\n",hall_state);
        if(hall_state == CLOSE)
        {
                ascii = 'i'; // close
        }
        else
        {
                ascii = 'h';//far
        }
        //ptr += sprintf(ptr, "0x%c,%s ", hall_state,hall_buf);
        APS_ERR("HALL 1:%d\n",ascii);
        ptr += sprintf(ptr, "0x%c,%s ", ascii,hall_buf);
        APS_ERR("HALL 2:%s\n",ptr);
        *ppos += ptr - page;
        return (ptr - page) ;
}
static ssize_t hall_config_write_proc(struct file *filp, const char __user *buffer, size_t count, loff_t *off)
{
        s32 ret = 0;

        APS_ERR("hall_config_write_proc = %d\n", hall_state);
        if (count > HALL_CONFIG_MAX_LENGTH  )
        {
        APS_ERR("size not match [%d:%d]\n", HALL_CONFIG_MAX_LENGTH, count);
        return -EFAULT;
        }

        if (copy_from_user(&config, buffer, count))
        {
                APS_ERR("copy from user fail\n");
                return -EFAULT;
        }
        //  tp__write_flag;

        if(atomic_read(&hall_write_flag))
        {
                atomic_set(&hall_write_flag,0);
                bEnHall=config[0]-48;
                APS_ERR("===== TGesture_config_write_proc%d=====",bEnHall);
                atomic_set(&hall_write_flag,1);
        }
        return count;
}

/*----------------------------------------------------------------------------*/
static const struct file_operations config_proc_ops = {
    .owner = THIS_MODULE,
    .read = hall_config_read_proc,
    .write = hall_config_write_proc,
};
/*----------------------------------------------------------------------------*/
static void bu5203_eint1_func(void)
{

	APS_ERR("debug bu5203_eint1_func!");
	schedule_work(&eint_work);
}


/*----------------------------------interrupt functions--------------------------------*/
static void bu5203_eint_work(struct work_struct *work)
{
	//int res = 0;
	s32 value_hall1 = -1;
	
	value_hall1 = mt_get_gpio_in(GPIO_MHALL_EINT_PIN);
	//APS_LOG("bu5203_eint_work GPIO_HALL_1_PIN=%d\n",value_hall1);	
	if(!value_hall1)
		mt_eint_registration(CUST_EINT_MHALL_NUM, EINTF_TRIGGER_HIGH, bu5203_eint1_func, 0);
	else
		mt_eint_registration(CUST_EINT_MHALL_NUM, EINTF_TRIGGER_LOW, bu5203_eint1_func, 0);
	
	if(0 == value_hall1)
		hall_state = 0;
	else
		hall_state = 1;		
		
	//APS_LOG("bu5203_eint_work gBackLightLevel=%d enable_input_key=%d\n",gBackLightLevel, enable_input_key);	
	//APS_LOG("bu5203_eint_work value_hall1_rev=%d value_hall1=%d\n",value_hall1_rev, value_hall1);	
	//APS_LOG("bu5203_eint_work value_hall2_rev=%d value_hall2=%d\n",value_hall2_rev, value_hall2);


	if(enable_input_key){
		//if((value_hall1_rev != value_hall1)&&((0!=gBackLightLevel) ||(value_hall1&&(0==gBackLightLevel)))
		if(value_hall1_rev != value_hall1
			){
/*	if((value_hall1_rev != value_hall1) || (value_hall2_rev != value_hall2)  ){*/
		//APS_LOG("bu5203_eint_work input_report_key \n");				
                        if(hall_state == 1){
                            input_report_key(bu5203_key_dev, KEY_SENSOR, 1);
                            input_report_key(bu5203_key_dev, KEY_SENSOR, 0);
                        } else {
                            input_report_key(bu5203_key_dev, KEY_SENSOR+1, 1);
                            input_report_key(bu5203_key_dev, KEY_SENSOR+1, 0);}
			input_sync(bu5203_key_dev);
		} /*else{
			input_report_key(bu5203_key_dev, KEY_SENSOR+1, 1);
			input_report_key(bu5203_key_dev, KEY_SENSOR+1, 0);
			input_sync(bu5203_key_dev);
		}*/

		value_hall1_rev = value_hall1;
	}

	mt_eint_unmask(CUST_EINT_MHALL_NUM); 
	return;
}

static int bu5203_setup_eint(void)
{
	
	mt_set_gpio_dir(GPIO_MHALL_EINT_PIN, GPIO_DIR_IN);
	mt_set_gpio_mode(GPIO_MHALL_EINT_PIN, GPIO_MHALL_EINT_PIN_M_EINT);
	mt_set_gpio_pull_enable(GPIO_MHALL_EINT_PIN, TRUE);
	mt_set_gpio_pull_select(GPIO_MHALL_EINT_PIN, GPIO_PULL_UP);

	mt_eint_set_sens(CUST_EINT_MHALL_NUM, MT_LEVEL_SENSITIVE);
	mt_eint_set_hw_debounce(CUST_EINT_MHALL_NUM, CUST_EINT_MHALL_DEBOUNCE_CN);
	mt_eint_registration(CUST_EINT_MHALL_NUM, EINTF_TRIGGER_LOW, bu5203_eint1_func, 0);
	mt_eint_unmask(CUST_EINT_MHALL_NUM); 
    
  	return 0;
}

/*----------------------------------------------------------------------------*/

static int bu5203_probe(struct platform_device *pdev) 
{
	int err = 0;
	APS_FUN();  
	

	INIT_WORK(&eint_work, bu5203_eint_work);
	if((err = bu5203_create_attr(&bu5203_hall_driver.driver)))
	{
		APS_ERR("create attribute err = %d\n", err);
		goto exit_create_attr_failed;
	}

	//------------------------------------------------------------------
	// 							Create input device 
	//------------------------------------------------------------------
	bu5203_key_dev = input_allocate_device();
	if (!bu5203_key_dev) 
	{
		APS_LOG("[APS]bu5203_key_dev : fail!\n");
		return -ENOMEM;
	}

	//define multi-key keycode
	__set_bit(EV_KEY, bu5203_key_dev->evbit);
	__set_bit(KEY_SENSOR, bu5203_key_dev->keybit);
	__set_bit(KEY_SENSOR+1, bu5203_key_dev->keybit);

	
	bu5203_key_dev->id.bustype = BUS_HOST;
	bu5203_key_dev->name = "bu5203";
	if(input_register_device(bu5203_key_dev))
	{
		APS_LOG("[APS]bu5203_key_dev register : fail!\n");
	}else
	{
		APS_LOG("[APS]bu5203_key_dev register : success!!\n");
	} 
	//WIKOKK-320 tonsal
	bu5203_setup_eint();
        #if 1
        // Create proc file system
        bu5203_config_state = proc_create(BU5203_CONFIG_PROC_FILE, 0666, NULL, &config_proc_ops);
        if (bu5203_config_state == NULL)
        {
                APS_ERR("hall create proc entry %s failed\n", BU5203_CONFIG_PROC_FILE);
        }
        else
        {
                APS_ERR("hall create proc entry %s success", BU5203_CONFIG_PROC_FILE);
        }
        #endif
	return 0;
	
	exit_create_attr_failed:
	APS_ERR("%s: err = %d\n", __func__, err);
	return err;
}
/*----------------------------------------------------------------------------*/
static int bu5203_remove(struct platform_device *pdev)
{
	int err = 0;
	APS_FUN(); 
		
	input_unregister_device(bu5203_key_dev);
	if((err = bu5203_delete_attr(&bu5203_hall_driver.driver)))
	{
		APS_ERR("ap3220_delete_attr fail: %d\n", err);
	} 
		
	return 0;
}



/*----------------------------------------------------------------------------*/
static int __init bu5203_init(void)
{
	APS_FUN();
  if(platform_driver_register(&bu5203_hall_driver))
	{
		APS_ERR("failed to register driver");
		return -ENODEV;
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
static void __exit bu5203_exit(void)
{
	APS_FUN();
	platform_driver_unregister(&bu5203_hall_driver);
}
/*----------------------------------------------------------------------------*/
module_init(bu5203_init);
module_exit(bu5203_exit);
/*----------------------------------------------------------------------------*/
MODULE_AUTHOR("cyong");
MODULE_DESCRIPTION("bu5203 driver");
MODULE_LICENSE("GPL");

