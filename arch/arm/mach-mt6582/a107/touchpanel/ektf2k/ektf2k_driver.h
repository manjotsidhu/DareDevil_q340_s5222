#ifndef _LINUX_ELAN_KTF2K_H
#define _LINUX_ELAN_KTF2K_H

#define ELAN_X_MAX      744 
#define ELAN_Y_MAX      1152

#define LCM_X_MAX      480 //simple_strtoul(LCM_WIDTH, NULL, 0)
#define LCM_Y_MAX       800//simple_strtoul(LCM_HEIGHT, NULL, 0)

//Elan Key's define
#define ELAN_KEY_BACK       0x10 
#define ELAN_KEY_HOME       0x08
#define ELAN_KEY_MENU       0x04
#define ELAN_KEY_SEARCH 		0x11


#define ELAN_KTF2K_NAME "elan-ktf2k"

#define TPD_POWER_SOURCE_CUSTOM 	MT6323_POWER_LDO_VGP1

struct elan_ktf2k_i2c_platform_data {
         uint16_t version;
         int abs_x_min;
         int abs_x_max;
         int abs_y_min;
         int abs_y_max;
         int intr_gpio;
         int (*power)(int on);
};

struct osd_offset{
        int left_x;
        int right_x;
        unsigned int key_event;
};

static struct osd_offset OSD_mapping[] = {
  {35, 99, KEY_MENU},     	//menu_left_x, menu_right_x, KEY_MENU
  {203, 267, KEY_HOMEPAGE}, //home_left_x, home_right_x, KEY_HOME
 	{373, 437, KEY_BACK},  		//back_left_x, back_right_x, KEY_BACK
  {541, 605, KEY_SEARCH},   //search_left_x, search_right_x, KEY_SEARCH
};

static int key_pressed = -1;

#endif /* _LINUX_ELAN_KTF2K_H */
