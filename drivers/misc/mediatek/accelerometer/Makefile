include $(srctree)/drivers/misc/mediatek/Makefile.custom

# In case the platform does NOT support this type of sensors
obj-y   +=  accel.o accel_factory.o

# Accelerometer device related drivers
ifeq ($(CONFIG_MTK_BMA222E_AUTO),y)
obj-y   +=  bma222E_auto/
endif

ifeq ($(CONFIG_MTK_KXTJ2_1009_AUTO),y)
obj-y   +=  KXTJ2_1009_auto/
endif

