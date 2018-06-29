#
# Makefile for misc devices that really don't fit anywhere else.
#
include $(srctree)/drivers/misc/mediatek/Makefile.custom

obj-y				+= kd_sensorlist.o

# guomingyi20150122 add.
# START OF CUST_PRJ_IMGSENSOR
CUST_PRJ_IMGSENSOR := $(PWD)/../vendor/tinno/$(ARCH_MTK_PROJECT)/$(PROJECT_NAME)/kernel/imgsensor
ifeq ($(wildcard $(CUST_PRJ_IMGSENSOR)/Makefile),)

ifeq ($(wildcard $(srctree)/arch/arm/mach-$(MTK_PLATFORM)/$(ARCH_MTK_PROJECT)/imgsensor),)

custom_kernel_imgsensor := $(addsuffix /, $(shell echo $(CONFIG_CUSTOM_KERNEL_IMGSENSOR)))

obj-y += $(custom_kernel_imgsensor)

endif

endif
# END OF CUST_PRJ_IMGSENSOR
