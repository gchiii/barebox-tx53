/*
 * Copyright (C) 2007 Sascha Hauer, Pengutronix
 * Copyright (C) 2012 Christian Kapeller, <christian.kapeller@cmotion.eu>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <common.h>
#include <environment.h>
#include <fcntl.h>
#include <fec.h>
#include <fs.h>
#include <init.h>
#include <nand.h>
#include <net.h>
#include <partition.h>
#include <sizes.h>

#include <generated/mach-types.h>

#include <mach/imx-regs.h>
#include <mach/iomux-mx53.h>
#include <mach/devices-imx53.h>
#include <mach/generic.h>
#include <mach/gpio.h>
#include <mach/imx-nand.h>
#include <mach/iim.h>
#include <mach/imx5.h>
#include <mach/iomux-v3.h>

#include <i2c/i2c.h>

#include <asm/armlinux.h>
#include <io.h>
#include <asm/mmu.h>

static struct fec_platform_data fec_info = {
	.xcv_type = RMII,
};

#define MX53_GPIO_PAD_CTRL	(PAD_CTL_PKE | PAD_CTL_PUE |		\
				PAD_CTL_DSE_HIGH | PAD_CTL_PUS_22K_UP)

#define TX53_SDHC_PAD_CTRL      (PAD_CTL_HYS | PAD_CTL_DSE_HIGH |	\
				PAD_CTL_SRE_FAST | PAD_CTL_PUS_100K_DOWN)


struct imx_nand_platform_data nand_info = {
	.width	= 1,
	.hw_ecc	= 1,
	.flash_bbt = 1,
};


static iomux_v3_cfg_t tx53_pads[] = {
	/* UART pads */
	MX53_PAD_PATA_DIOW__UART1_TXD_MUX,
	MX53_PAD_PATA_DMACK__UART1_RXD_MUX,
	MX53_PAD_PATA_IORDY__UART1_RTS,
	MX53_PAD_PATA_RESET_B__UART1_CTS,

	MX53_PAD_PATA_BUFFER_EN__UART2_RXD_MUX,
	MX53_PAD_PATA_DMARQ__UART2_TXD_MUX,
	MX53_PAD_PATA_DIOR__UART2_RTS,
	MX53_PAD_PATA_INTRQ__UART2_CTS,

	MX53_PAD_PATA_CS_0__UART3_TXD_MUX,
	MX53_PAD_PATA_CS_1__UART3_RXD_MUX,
	MX53_PAD_PATA_DA_2__UART3_RTS,
	MX53_PAD_PATA_DA_1__UART3_CTS,

	/* I2C */
	NEW_PAD_CTRL(MX53_PAD_EIM_D28__I2C1_SDA, MX53_GPIO_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_EIM_D21__I2C1_SCL, MX53_GPIO_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_GPIO_6__I2C3_SDA, MX53_GPIO_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_GPIO_3__I2C3_SCL, MX53_GPIO_PAD_CTRL),

	/* (e)CSPI pads */
	MX53_PAD_EIM_D16__ECSPI1_SCLK,
	MX53_PAD_EIM_D17__ECSPI1_MISO,
	MX53_PAD_EIM_D18__ECSPI1_MOSI,

	/* eCSPI chip select lines */
	NEW_PAD_CTRL(MX53_PAD_EIM_EB2__GPIO2_30, MX53_GPIO_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_EIM_D19__GPIO3_19, MX53_GPIO_PAD_CTRL),

	/* SW controlled LED on STK5 baseboard */
	MX53_PAD_EIM_A18__GPIO2_20,

	/* eSDHC 1 */
	NEW_PAD_CTRL(MX53_PAD_SD1_CMD__ESDHC1_CMD, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD1_CLK__ESDHC1_CLK, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD1_DATA0__ESDHC1_DAT0, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD1_DATA1__ESDHC1_DAT1, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD1_DATA2__ESDHC1_DAT2, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD1_DATA3__ESDHC1_DAT3, TX53_SDHC_PAD_CTRL),
	/* SD1 CD */
	NEW_PAD_CTRL(MX53_PAD_EIM_D24__GPIO3_24, MX53_GPIO_PAD_CTRL),

	/* eSDHC 2 */
	NEW_PAD_CTRL(MX53_PAD_SD2_CMD__ESDHC2_CMD, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD2_CLK__ESDHC2_CLK, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD2_DATA0__ESDHC2_DAT0, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD2_DATA1__ESDHC2_DAT1, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD2_DATA2__ESDHC2_DAT2, TX53_SDHC_PAD_CTRL),
	NEW_PAD_CTRL(MX53_PAD_SD2_DATA3__ESDHC2_DAT3, TX53_SDHC_PAD_CTRL),
	/* SD2 CD */
	NEW_PAD_CTRL(MX53_PAD_EIM_D25__GPIO3_25, MX53_GPIO_PAD_CTRL),

	/* FEC */
	MX53_PAD_EIM_D20__GPIO3_20,
	MX53_PAD_PATA_DA_0__GPIO7_6,
	MX53_PAD_PATA_DATA4__GPIO2_4,
	MX53_PAD_FEC_MDC__FEC_MDC,
	MX53_PAD_FEC_MDIO__FEC_MDIO,
	MX53_PAD_FEC_REF_CLK__FEC_TX_CLK,
	MX53_PAD_FEC_RX_ER__FEC_RX_ER,
	MX53_PAD_FEC_CRS_DV__FEC_RX_DV,
	MX53_PAD_FEC_RXD1__FEC_RDATA_1,
	MX53_PAD_FEC_RXD0__FEC_RDATA_0,
	MX53_PAD_FEC_TX_EN__FEC_TX_EN,
	MX53_PAD_FEC_TXD1__FEC_TDATA_1,
	MX53_PAD_FEC_TXD0__FEC_TDATA_0,

	/* TSC200x PEN IRQ */
	NEW_PAD_CTRL(MX53_PAD_EIM_D26__GPIO3_26, MX53_GPIO_PAD_CTRL),

	/* EDT-FT5x06 Polytouch panel */
	NEW_PAD_CTRL(MX53_PAD_NANDF_CS2__GPIO6_15, MX53_GPIO_PAD_CTRL), /* IRQ */
	NEW_PAD_CTRL(MX53_PAD_EIM_A16__GPIO2_22, MX53_GPIO_PAD_CTRL), /* RESET */
	NEW_PAD_CTRL(MX53_PAD_EIM_A17__GPIO2_21, MX53_GPIO_PAD_CTRL), /* WAKE */

	/* LCD RESET */
	NEW_PAD_CTRL(MX53_PAD_EIM_D29__GPIO3_29, MX53_GPIO_PAD_CTRL),
	/* LCD POWER_ENABLE */
	NEW_PAD_CTRL(MX53_PAD_EIM_EB3__GPIO2_31, MX53_GPIO_PAD_CTRL),
	/* LCD Backlight (PWM) */
	NEW_PAD_CTRL(MX53_PAD_GPIO_1__GPIO1_1, MX53_GPIO_PAD_CTRL),
	/* DISPLAY */
	MX53_PAD_DI0_DISP_CLK__IPU_DI0_DISP_CLK,
	MX53_PAD_DI0_PIN15__IPU_DI0_PIN15,
	MX53_PAD_DI0_PIN2__IPU_DI0_PIN2,
	MX53_PAD_DI0_PIN3__IPU_DI0_PIN3,
	MX53_PAD_DISP0_DAT0__IPU_DISP0_DAT_0,
	MX53_PAD_DISP0_DAT1__IPU_DISP0_DAT_1,
	MX53_PAD_DISP0_DAT2__IPU_DISP0_DAT_2,
	MX53_PAD_DISP0_DAT3__IPU_DISP0_DAT_3,
	MX53_PAD_DISP0_DAT4__IPU_DISP0_DAT_4,
	MX53_PAD_DISP0_DAT5__IPU_DISP0_DAT_5,
	MX53_PAD_DISP0_DAT6__IPU_DISP0_DAT_6,
	MX53_PAD_DISP0_DAT7__IPU_DISP0_DAT_7,
	MX53_PAD_DISP0_DAT8__IPU_DISP0_DAT_8,
	MX53_PAD_DISP0_DAT9__IPU_DISP0_DAT_9,
	MX53_PAD_DISP0_DAT10__IPU_DISP0_DAT_10,
	MX53_PAD_DISP0_DAT11__IPU_DISP0_DAT_11,
	MX53_PAD_DISP0_DAT12__IPU_DISP0_DAT_12,
	MX53_PAD_DISP0_DAT13__IPU_DISP0_DAT_13,
	MX53_PAD_DISP0_DAT14__IPU_DISP0_DAT_14,
	MX53_PAD_DISP0_DAT15__IPU_DISP0_DAT_15,
	MX53_PAD_DISP0_DAT16__IPU_DISP0_DAT_16,
	MX53_PAD_DISP0_DAT17__IPU_DISP0_DAT_17,
	MX53_PAD_DISP0_DAT18__IPU_DISP0_DAT_18,
	MX53_PAD_DISP0_DAT19__IPU_DISP0_DAT_19,
	MX53_PAD_DISP0_DAT20__IPU_DISP0_DAT_20,
	MX53_PAD_DISP0_DAT21__IPU_DISP0_DAT_21,
	MX53_PAD_DISP0_DAT22__IPU_DISP0_DAT_22,
	MX53_PAD_DISP0_DAT23__IPU_DISP0_DAT_23,

	/* LVDS option */
	MX53_PAD_LVDS1_TX3_P__LDB_LVDS1_TX3,
	MX53_PAD_LVDS1_TX2_P__LDB_LVDS1_TX2,
	MX53_PAD_LVDS1_CLK_P__LDB_LVDS1_CLK,
	MX53_PAD_LVDS1_TX1_P__LDB_LVDS1_TX1,
	MX53_PAD_LVDS1_TX0_P__LDB_LVDS1_TX0,
	MX53_PAD_LVDS0_TX3_P__LDB_LVDS0_TX3,
	MX53_PAD_LVDS0_CLK_P__LDB_LVDS0_CLK,
	MX53_PAD_LVDS0_TX2_P__LDB_LVDS0_TX2,
	MX53_PAD_LVDS0_TX1_P__LDB_LVDS0_TX1,
	MX53_PAD_LVDS0_TX0_P__LDB_LVDS0_TX0,

	/* USB Host */
	NEW_PAD_CTRL(MX53_PAD_EIM_D31__GPIO3_31, MX53_GPIO_PAD_CTRL), /* VBUSEN */
	NEW_PAD_CTRL(MX53_PAD_EIM_D30__GPIO3_30, MX53_GPIO_PAD_CTRL), /* OC */

	MX53_PAD_GPIO_7__GPIO1_7, /* VBUSEN */
	MX53_PAD_GPIO_8__GPIO1_8, /* OC */

	/* Flexcan */
	MX53_PAD_KEY_COL4__CAN2_TXCAN,
	MX53_PAD_KEY_ROW4__CAN2_RXCAN,

	/* Audio */
	MX53_PAD_KEY_COL0__AUDMUX_AUD5_TXC,
	MX53_PAD_KEY_ROW0__AUDMUX_AUD5_TXD,
	MX53_PAD_KEY_COL1__AUDMUX_AUD5_TXFS,
	MX53_PAD_KEY_ROW1__AUDMUX_AUD5_RXD,

	/* DS1339 Interrupt */
	NEW_PAD_CTRL(MX53_PAD_DI0_PIN4__GPIO4_20, MX53_GPIO_PAD_CTRL),

};

static int tx53_mem_init(void)
{
	arm_add_mem_device("ram0", 0x70000000, SZ_512M);
	arm_add_mem_device("ram1", 0xb0000000, SZ_512M);

	return 0;
}
mem_initcall(tx53_mem_init);


#define TX53_FEC_PHY_RST	IMX_GPIO_NR(2, 14)
#define TX53_FEC_PHY_PWR	IMX_GPIO_NR(1, 3)
#define TX53_FEC_PHY_INT	IMX_GPIO_NR(3, 18)

static struct esdhc_platform_data tx53_sd1_data = {
	.cd_gpio = IMX_GPIO_NR(3,24),
	.cd_type = ESDHC_CD_GPIO,
	.wp_type = ESDHC_WP_NONE,
};

static struct esdhc_platform_data tx53_sd2_data = {
	.cd_gpio = IMX_GPIO_NR(3,25),
	.cd_type = ESDHC_CD_GPIO,
	.wp_type = ESDHC_WP_NONE,
};

#define TX53_USB1_PWREN		IMX_GPIO_NR(3, 31)
#define TX53_USB2_PWREN		IMX_GPIO_NR(1, 8)
static void tx53_ehci_init(void)
{
	/* USB PWR enable */
	gpio_direction_output(TX53_USB1_PWREN, 0);
	gpio_set_value(TX53_USB1_PWREN, 1);

	gpio_direction_output(TX53_USB2_PWREN, 0);
	gpio_set_value(TX53_USB2_PWREN, 1);

	writel(0, MX53_OTG_BASE_ADDR + 0x384); /* setup portsc */
	add_generic_usb_ehci_device(1, MX53_OTG_BASE_ADDR + 0x200, NULL);
}

static int tx53_devices_init(void)
{
#ifdef CONFIG_MCI_IMX_ESDHC
	imx53_add_mmc0(&tx53_sd1_data);
	imx53_add_mmc1(&tx53_sd2_data);
#endif

	imx53_add_nand(&nand_info);


	imx53_iim_register_fec_ethaddr();
	imx53_add_fec(&fec_info);
	tx53_ehci_init();

	//Linux Parameters
	armlinux_set_bootparams((void *)MX53_CSD0_BASE_ADDR + 0x100);
	armlinux_set_architecture(MACH_TYPE_TX53);

	return 0;
}
device_initcall(tx53_devices_init);

static int tx53_part_init(void)
{
	devfs_add_partition("nand0", 0x00000, SZ_256K+SZ_128K, DEVFS_PARTITION_FIXED, "self_raw");
	dev_add_bb_dev("self_raw", "self0");
	devfs_add_partition("nand0", 0x00000+SZ_256K+SZ_128K, SZ_128K, DEVFS_PARTITION_FIXED, "env_raw");
	dev_add_bb_dev("env_raw", "env0");

	return 0;
}
late_initcall(tx53_part_init);

static int tx53_console_init(void)
{
	mxc_iomux_v3_setup_multiple_pads(tx53_pads, ARRAY_SIZE(tx53_pads));
	imx53_init_lowlevel(800);
	imx53_add_uart0();

	return 0;
}
console_initcall(tx53_console_init);
