#ifdef IAP_PORTION
const u8 huaruichuan_fw[]=
{
#include "tinno_MTK_L5220VC406.i"
};

struct vendor_map
{
	int vendor_id;
	char vendor_name[30];
	uint8_t* fw_array;
};
const struct vendor_map g_vendor_map[]=
{
	{0x2ae1,"hrc",huaruichuan_fw}
};

#endif/*IAP_PORTION*/
