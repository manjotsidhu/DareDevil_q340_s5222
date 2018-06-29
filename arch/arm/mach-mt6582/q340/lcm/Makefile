include $(srctree)/drivers/misc/mediatek/Makefile.custom

obj-y   +=  mt65xx_lcm_list.o


ifeq ($(CONFIG_CUSTOM_KERNEL_LCM),"ili9806e_fwvga_dsi_vdo_txd")
obj-y   +=  nt35590_hd720_dsi_cmd_auo.o
endif

ifeq ($(CONFIG_CUSTOM_KERNEL_LCM),"otm8018b_fwvga_dsi_vdo_boe")
obj-y   +=  nt35590_hd720_dsi_cmd_auo_wvga.o
endif


