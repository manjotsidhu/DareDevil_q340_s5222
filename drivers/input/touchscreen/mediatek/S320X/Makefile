include $(srctree)/drivers/misc/mediatek/Makefile.custom

# Linux driver folder
ccflags-y += -I$(srctree)/drivers/input/touchscreen/mediatek/
ccflags-y += -I$(srctree)/arch/arm/mach-$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/touchpanel/S320X

obj-y	+=  rmi_dev.o
obj-y	+=  rmi_fw_update.o
obj-y	+=  synaptics_rmi4_driver.o

