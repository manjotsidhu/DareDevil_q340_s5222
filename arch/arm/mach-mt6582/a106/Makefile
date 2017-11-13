
include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Module project related part

ifeq ($(CONFIG_MTK_SENSOR_SUPPORT),y)
ifeq ($(CONFIG_MTK_AP3220),y)
obj-y	+=  alsps/ap3220/
endif

ifeq ($(CONFIG_MTK_BMA222E_AUTO),y)
obj-y	+=  accelerometer/bma222E_auto/
endif

ifeq ($(CONFIG_MTK_KXTJ2_1009_AUTO),y)
obj-y	+=  accelerometer/KXTJ2_1009_auto/
endif
endif

ifneq ($(CONFIG_ARCH_MTK_PROJECT),)
ARCH_MTK_PROJECT := $(shell echo $(CONFIG_ARCH_MTK_PROJECT))
obj-y	+= core/
obj-y	+= dct/dct/
endif

obj-y   +=  lcm/
#obj-y	+=  hdmi/
obj-y	+=  camera/camera/
obj-y	+=  imgsensor/ov5648_mipi_raw_darling/
obj-y	+=  imgsensor/ov5648_mipi_raw_truly/
obj-y	+=  imgsensor/ov5648_mipi_raw_sunwin/
obj-y	+=  imgsensor/ov5648_mipi_raw_darling_zl/
obj-y	+=  imgsensor/hi258_mipi_yuv_cmk/
obj-y	+=  imgsensor/hi258_mipi_yuv_sw/
obj-y   +=  flashlight/constant_flashlight/
obj-y	+=  vibrator/
obj-y	+=  leds/
obj-y	+=  keypad/
obj-y	+=  sound/
obj-y	+=  accdet/
obj-y	+=  partition/

