/**
 * Copyright (C) 2012 Christian Kapeller, <christian.kapeller@cmotion.eu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/byteorder.h>
#include <mach/imx-flash-header.h>
#include <asm/barebox-arm-head.h>
#include <mach/imx53-regs.h>
#include <mach/clock-imx51_53.h>

#if 0
struct fcb_entry
{
  uint32_t addr;
  uint32_t value;
};
static struct fcb_entry fcb_entries[] =
{
  { 0x00, 0x00000000 }, // res.
  { 0x04, 0x46434220 }, // fingerprint #2 (ascii FCB)
  { 0x08, 0x01000000 }, // fingerprint #3 (version)
  { 0x68, 0x02000000 }, // primary image start page (page 2)
  { 0x6C, 0x02000000 }, // secondary image start page (page 2)
  { 0x78, 0x00000000 }, // Start page address of DBBT Search Area (0 means no bad blocks)
  { 0x7C, 0x00000000 }, // Bad Block Marker Offset in page data buffer
  { 0xAC, 0x00000000 }, // Bad Block Marker Swapping Enable
  { 0xB0, 0x00000000 }, // Bad Block Marker Offset to Spare Byte
};

#endif
//void __naked __flash_header_start go(void)
void __naked __section(.flash_header_0x0100) go(void)
{
	barebox_arm_head();
}

#define MXC_DCD_ITEM(x,y) { .addr = cpu_to_be32(x), .val = cpu_to_be32(y), },

struct imx_dcd_v2_entry __dcd_entry_section dcd_entry[] = {
		/* disable all irrelevant clocks */
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR0, 0xffcc0fff)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR1, 0x000fffc3)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR2, 0x033c0000)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR3, 0x00000000)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR4, 0x00000000)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR5, 0x00fff033)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX5_CCM_CCGR6, 0x0f00030f)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX53_CCM_CCGR7, 0xfff00000)
	MXC_DCD_ITEM(MX53_CCM_BASE_ADDR + MX51_CCM_CMEOR, 0x00000000)

	{ .addr = cpu_to_be32(0x53fa8554), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8558), .val = cpu_to_be32(0x00f00000), },
	{ .addr = cpu_to_be32(0x53fa8560), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8564), .val = cpu_to_be32(0x003000c0), },
	{ .addr = cpu_to_be32(0x53fa8568), .val = cpu_to_be32(0x00f00000), },
	{ .addr = cpu_to_be32(0x53fa8570), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8574), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8578), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa857c), .val = cpu_to_be32(0x00f000c0), },
	{ .addr = cpu_to_be32(0x53fa8580), .val = cpu_to_be32(0x003000c0), },
	{ .addr = cpu_to_be32(0x53fa8584), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8588), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8590), .val = cpu_to_be32(0x00f00000), },
	{ .addr = cpu_to_be32(0x53fa8594), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa86f0), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa86f4), .val = cpu_to_be32(0x00000000), },
	{ .addr = cpu_to_be32(0x53fa86fc), .val = cpu_to_be32(0x00000000), },
	{ .addr = cpu_to_be32(0x53fa8714), .val = cpu_to_be32(0x00000000), },
	{ .addr = cpu_to_be32(0x53fa8718), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa871c), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8720), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa8724), .val = cpu_to_be32(0x00000000), },
	{ .addr = cpu_to_be32(0x53fa8728), .val = cpu_to_be32(0x00300000), },
	{ .addr = cpu_to_be32(0x53fa872c), .val = cpu_to_be32(0x00300000), },
	
	{ .addr = cpu_to_be32(0x63fd9088), .val = cpu_to_be32(0x35343535), },
	{ .addr = cpu_to_be32(0x63fd9090), .val = cpu_to_be32(0x4d444c44), },
	{ .addr = cpu_to_be32(0x63fd907c), .val = cpu_to_be32(0x01370138), },
	{ .addr = cpu_to_be32(0x63fd9080), .val = cpu_to_be32(0x013b013c), },
	
	{ .addr = cpu_to_be32(0x63fd9018), .val = cpu_to_be32(0x00011740), },
	{ .addr = cpu_to_be32(0x63fd9000), .val = cpu_to_be32(0xc3190000), },
	{ .addr = cpu_to_be32(0x63fd900c), .val = cpu_to_be32(0x9f5152e3), },
	{ .addr = cpu_to_be32(0x63fd9010), .val = cpu_to_be32(0xb68e8a63), },
	{ .addr = cpu_to_be32(0x63fd9014), .val = cpu_to_be32(0x01ff00db), },
	
	{ .addr = cpu_to_be32(0x63fd902c), .val = cpu_to_be32(0x000026d2), },
	{ .addr = cpu_to_be32(0x63fd9030), .val = cpu_to_be32(0x009f0e21), },
	{ .addr = cpu_to_be32(0x63fd9008), .val = cpu_to_be32(0x12273030), },
	{ .addr = cpu_to_be32(0x63fd9004), .val = cpu_to_be32(0x0002002d), },
	
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x00008032), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x00008033), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x00028031), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x092080b0), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x04008040), },

	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x0000803a), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x0000803b), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x00028039), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x09208138), },
	
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x04008048), },
	
	{ .addr = cpu_to_be32(0x63fd9020), .val = cpu_to_be32(0x00005800), },
	{ .addr = cpu_to_be32(0x63fd9040), .val = cpu_to_be32(0x04b80003), },
	{ .addr = cpu_to_be32(0x63fd9058), .val = cpu_to_be32(0x00011117), },
	{ .addr = cpu_to_be32(0x63fd901c), .val = cpu_to_be32(0x00000000), },

	{ .addr = cpu_to_be32(0x53fa8004), .val = cpu_to_be32(0x00194005), }, // set LDO to 1.3V

	/* setup NFC pads */
	/* MUX_SEL */
	{ .addr = cpu_to_be32(0x53fa819c), .val = cpu_to_be32(0x00000000), }, // EIM_DA0
	{ .addr = cpu_to_be32(0x53fa81a0), .val = cpu_to_be32(0x00000000), }, // EIM_DA1
	{ .addr = cpu_to_be32(0x53fa81a4), .val = cpu_to_be32(0x00000000), }, // EIM_DA2
	{ .addr = cpu_to_be32(0x53fa81a8), .val = cpu_to_be32(0x00000000), }, // EIM_DA3
	{ .addr = cpu_to_be32(0x53fa81ac), .val = cpu_to_be32(0x00000000), }, // EIM_DA4
	{ .addr = cpu_to_be32(0x53fa81b0), .val = cpu_to_be32(0x00000000), }, // EIM_DA5
	{ .addr = cpu_to_be32(0x53fa81b4), .val = cpu_to_be32(0x00000000), }, // EIM_DA6
	{ .addr = cpu_to_be32(0x53fa81b8), .val = cpu_to_be32(0x00000000), }, // EIM_DA7
	{ .addr = cpu_to_be32(0x53fa81dc), .val = cpu_to_be32(0x00000000), }, // WE_B
	{ .addr = cpu_to_be32(0x53fa81e0), .val = cpu_to_be32(0x00000000), }, // RE_B
	{ .addr = cpu_to_be32(0x53fa8228), .val = cpu_to_be32(0x00000000), }, // CLE
	{ .addr = cpu_to_be32(0x53fa822c), .val = cpu_to_be32(0x00000000), }, // ALE
	{ .addr = cpu_to_be32(0x53fa8230), .val = cpu_to_be32(0x00000000), }, // WP_B
	{ .addr = cpu_to_be32(0x53fa8234), .val = cpu_to_be32(0x00000000), }, // RB0
	{ .addr = cpu_to_be32(0x53fa8238), .val = cpu_to_be32(0x00000000), }, // CS0
	/* PAD_CTL */
	{ .addr = cpu_to_be32(0x53fa84ec), .val = cpu_to_be32(0x000000e4), }, // EIM_DA0
	{ .addr = cpu_to_be32(0x53fa84f0), .val = cpu_to_be32(0x000000e4), }, // EIM_DA1
	{ .addr = cpu_to_be32(0x53fa84f4), .val = cpu_to_be32(0x000000e4), }, // EIM_DA2
	{ .addr = cpu_to_be32(0x53fa84f8), .val = cpu_to_be32(0x000000e4), }, // EIM_DA3
	{ .addr = cpu_to_be32(0x53fa84fc), .val = cpu_to_be32(0x000000e4), }, // EIM_DA4
	{ .addr = cpu_to_be32(0x53fa8500), .val = cpu_to_be32(0x000000e4), }, // EIM_DA5
	{ .addr = cpu_to_be32(0x53fa8504), .val = cpu_to_be32(0x000000e4), }, // EIM_DA6
	{ .addr = cpu_to_be32(0x53fa8508), .val = cpu_to_be32(0x000000e4), }, // EIM_DA7
	{ .addr = cpu_to_be32(0x53fa852c), .val = cpu_to_be32(0x00000004), }, // NANDF_WE_B
	{ .addr = cpu_to_be32(0x53fa8530), .val = cpu_to_be32(0x00000004), }, // NANDF_RE_B
	{ .addr = cpu_to_be32(0x53fa85a0), .val = cpu_to_be32(0x00000004), }, // NANDF_CLE_B
	{ .addr = cpu_to_be32(0x53fa85a4), .val = cpu_to_be32(0x00000004), }, // NANDF_ALE_B
	{ .addr = cpu_to_be32(0x53fa85a8), .val = cpu_to_be32(0x000000e4), }, // NANDF_WE_B
	{ .addr = cpu_to_be32(0x53fa85ac), .val = cpu_to_be32(0x000000e4), }, // NANDF_RB0
	{ .addr = cpu_to_be32(0x53fa85b0), .val = cpu_to_be32(0x00000004), }, // NANDF_CS0

};

#define APP_DEST	0x77800000

struct imx_flash_header_v2 __flash_header_section flash_header = {
	.header.tag		= IVT_HEADER_TAG,
	.header.length		= cpu_to_be16(32),
	.header.version		= IVT_VERSION,

	.entry			= APP_DEST + 0x1000,
	.dcd_ptr		= APP_DEST + 0x400 + offsetof(struct imx_flash_header_v2, dcd),
	.boot_data_ptr		= APP_DEST + 0x400 + offsetof(struct imx_flash_header_v2, boot_data),
	.self			= APP_DEST + 0x400,

	.boot_data.start	= APP_DEST,
	.boot_data.size		= DCD_BAREBOX_SIZE,

	.dcd.header.tag		= DCD_HEADER_TAG,
	.dcd.header.length	= cpu_to_be16(sizeof(struct imx_dcd) + sizeof(dcd_entry)),
	.dcd.header.version	= DCD_VERSION,

	.dcd.command.tag	= DCD_COMMAND_WRITE_TAG,
	.dcd.command.length	= cpu_to_be16(sizeof(struct imx_dcd_command) + sizeof(dcd_entry)),
	.dcd.command.param	= DCD_COMMAND_WRITE_PARAM,
};

unsigned long __image_len_section barebox_len = DCD_BAREBOX_SIZE;
