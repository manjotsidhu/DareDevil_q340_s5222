
include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Module project related part

ifeq ($(CONFIG_MTK_SENSOR_SUPPORT),y)
ifeq ($(CONFIG_MTK_CM36283),y)
obj-y	+=  alsps/cm36283/
endif

ifeq ($(CONFIG_MTK_AP3220),y)
obj-y	+=  alsps/ap3220/
endif

ifeq ($(CONFIG_MTK_STK3308),y)
obj-y	+=  alsps/stk3308/
endif

ifeq ($(CONFIG_MTK_BMA050_NEW),y)
obj-y   +=  accelerometer/bma050-new/
endif

ifeq ($(CONFIG_MTK_BMA222E_NEW),y)
obj-y   +=  accelerometer/bma222E-new/
endif

ifeq ($(CONFIG_MTK_MPU3000),y)
obj-y   +=  gyroscope/mpu3000/
endif

ifeq ($(CONFIG_MTK_MPU3000_NEW),y)
obj-y   +=  gyroscope/mpu3000-new/
endif

ifeq ($(CONFIG_MTK_BMM050_NEW),y)
obj-y	+=  magnetometer/bmm050-new/
endif

ifeq ($(CONFIG_MTK_AKM09911_NEW),y)
obj-y	+=  magnetometer/akm09911-new/
endif

endif

ifneq ($(CONFIG_ARCH_MTK_PROJECT),)
ARCH_MTK_PROJECT := $(shell echo $(CONFIG_ARCH_MTK_PROJECT))
obj-y	+= core/
obj-y	+= dct/dct/
endif

obj-y   +=  lcm/
obj-y   +=  flashlight/
obj-y	+=  camera/camera/
obj-y	+=  vibrator/
obj-y	+=  leds/
obj-y	+=  keypad/
obj-y	+=  sound/
obj-y	+=  accdet/
obj-y	+=  partition/
