#include <linux/linkage.h>
#include <init.h>
#include <asm-generic/barebox.lds.h>

#define CONFIG_NR_DRAM_BANKS	2
#define PHYS_SDRAM_1		CSD0_BASE_ADDR
#define PHYS_SDRAM_1_SIZE	(512 * 1024 * 1024)
#define PHYS_SDRAM_2		CSD1_BASE_ADDR
#define PHYS_SDRAM_2_SIZE	(512 * 1024 * 1024)
#define CONFIG_TX53_SDRAM_TYPE  SDRAM_TYPE_DDR3


/*
#ifdef	CONFIG_FLASH_HEADER
#ifndef CONFIG_FLASH_HEADER_OFFSET
# error "FLASH_HEADER_OFFSET not defined."
#endif
*/

#if !defined(PHYS_SDRAM_1_SIZE) || !defined(PHYS_SDRAM_1)
#error "PHYS_SDRAM_1_SIZE or PHYS_SDRAM_1 not defined."
#endif

#if defined(PHYS_SDRAM_2) && !defined(PHYS_SDRAM_2_SIZE)
#error "PHYS_SDRAM_2_SIZE not defined."
#endif


#define SDRAM_SIZE     (PHYS_SDRAM_1_SIZE + PHYS_SDRAM_2_SIZE)
#define CONFIG_SYS_PLL2_FREQ    400

#define SDRAM_CLK CONFIG_SYS_PLL2_FREQ


#define NS_TO_CK(ns)	(((ns) * SDRAM_CLK + 999) / 1000)

	.macro		CK_VAL,	name, clks, offs, max
	.iflt		\clks - \offs
	.set		\name, 0
	.else
	.ifle		\clks - \offs - \max
	.set		\name, \clks - \offs
	.endif
	.endif
	.endm

	.macro		NS_VAL,	name, ns, offs, max
	.iflt		\ns - \offs
	.set		\name, 0
	.else
	CK_VAL		\name, NS_TO_CK(\ns), \offs, \max
	.endif
	.endm

	.macro		CK_MAX, name, ck1, ck2, offs, max
	.ifgt		\ck1 - \ck2
	CK_VAL		\name, \ck1, \offs, \max
	.else
	CK_VAL		\name, \ck2, \offs, \max
	.endif
	.endm

#define SDRAM_TYPE_DDR2 2
#define SDRAM_TYPE_DDR3 3

#define ESDMISC_DDR_TYPE_DDR3		0
#define ESDMISC_DDR_TYPE_LPDDR2		1
#define ESDMISC_DDR_TYPE_DDR2		2

#define ESDOR_CLK_PERIOD		15625	/* base clock for ESDOR values 15.625uS */

#if CONFIG_TX53_SDRAM_TYPE == SDRAM_TYPE_DDR2
# error "SDRAM_TYPE_DDR2 not supported"
#elif CONFIG_TX53_SDRAM_TYPE == SDRAM_TYPE_DDR3
/* DDR3 SDRAM */
#if SDRAM_SIZE > SZ_512M
#define BANK_ADDR_BITS			2
#else
#define BANK_ADDR_BITS			1
#endif
#define SDRAM_BURST_LENGTH		8
#define RALAT				5
#define WALAT				1
#define ADDR_MIRROR			0
#define DDR_TYPE			ESDMISC_DDR_TYPE_DDR3

/* 512/1024MiB SDRAM: NT5CB128M16P-CG */
/* ESDCFG0 0x0c */
NS_VAL	tRFC,	160, 1, 255		/* clks - 1 (0..255) */
CK_MAX	tXS,	tRFC + 1 + NS_TO_CK(10), 5, 1, 255	/* clks - 1 (0..255) tRFC + 10 */
CK_MAX	tXP,	3, NS_TO_CK(6), 1, 7	/* clks - 1 (0..7) */ /* max(6ns, 3*CK) */
CK_MAX	tXPDLL, NS_TO_CK(24), 2, 1, 15		/* clks - 1 (0..15) */
NS_VAL	tFAW,	45, 1, 31		/* clks - 1 (0..31) */
CK_VAL	tCL,	9, 3, 8			/* clks - 3 (0..8) CAS Latency */

/* ESDCFG1 0x10 */
NS_VAL	tRCD,	14, 1, 7		/* clks - 1 (0..7) */
NS_VAL	tRP,	14, 1, 7		/* clks - 1 (0..7) */
NS_VAL	tRC,	50, 1, 31		/* clks - 1 (0..31) */
NS_VAL	tRAS,	36, 1, 31		/* clks - 1 (0..31) */
CK_VAL	tRPA,	0, 0, 1			/* clks     (0..1) */
NS_VAL	tWR,	15, 1, 15		/* clks - 1 (0..15) */
CK_VAL	tMRD,	4, 1, 15		/* clks - 1 (0..15) */
CK_VAL	tCWL,	7, 2, 6			/* clks - 2 (0..6) */

/* ESDCFG2 0x14 */
CK_VAL	tDLLK,	512, 1, 511		/* clks - 1 (0..511) */
CK_MAX	tRTP,	4, NS_TO_CK(8), 1, 7	/* clks - 1 (0..7) */
CK_MAX	tWTR,	4, NS_TO_CK(8), 1, 7	/* clks - 1 (0..7) */
CK_MAX	tRRD,	4, NS_TO_CK(8), 1, 7	/* clks - 1 (0..7) */

/* ESDOR 0x30 */
CK_MAX	tXPR,	NS_TO_CK(160 + 10), 5, 1, 255 /* max(tRFC + 10, 5CK) */

#define tSDE_RST			((200000 + (ESDOR_CLK_PERIOD - 1)) / ESDOR_CLK_PERIOD)
#define tRST_CKE			((500000 + (ESDOR_CLK_PERIOD - 1)) / ESDOR_CLK_PERIOD)

#define ROW_ADDR_BITS			14
#define COL_ADDR_BITS			10

	.iflt	tWR - 7
	.set	ESDSCR_MRS_VAL, (0x8000 | (3 << 4) | \
				(((tWR - 3) << 9) | \
				(((tCL + 3) - 4) << 4)) << 16)
	.else
	.set	ESDSCR_MRS_VAL, (0x8000 | (3 << 4) | \
				((((tWR + 1) / 2) << 9) | \
				(((tCL + 3) - 4) << 4)) << 16)
	.endif

#else
#error Unsupported SDRAM type selected
#endif

#define ESDCFG0_VAL	(			\
	((tRFC) << 24) |			\
	((tXS) << 16) |				\
	((tXP) << 13) |				\
	((tXPDLL) << 9) |			\
	((tFAW) << 4) |				\
	((tCL) << 0))				\

#define ESDCFG1_VAL	(			\
	((tRCD) << 29) |			\
	((tRP) << 26) |				\
	((tRC) << 21) |				\
	((tRAS) << 16) |			\
	((tRPA) << 15) |			\
	((tWR) << 9) |				\
	((tMRD) << 5) |				\
	((tCWL) << 0))				\

#define ESDCFG2_VAL	(			\
	((tDLLK) << 16) |			\
	((tRTP) << 6) |				\
	((tWTR) << 3) |				\
	((tRRD) << 0))

#define BL				(SDRAM_BURST_LENGTH / 8) /* 0: 4 byte 1: 8 byte */
#define ESDCTL_VAL			(((ROW_ADDR_BITS - 11) << 24) | \
					((COL_ADDR_BITS - 9) << 20) | \
					(BL << 19) | \
					(1 << 16) | /* SDRAM bus width */ \
					((-1) << (32 - BANK_ADDR_BITS)))

#define ESDMISC_VAL			((1 << 12) | \
					(0x3 << 9) | \
					(RALAT << 6) | \
					(WALAT << 16) | \
					(ADDR_MIRROR << 19) | \
					(DDR_TYPE << 3))

#define ESDOR_VAL		((tXPR << 16) | (tSDE_RST << 8) | (tRST_CKE << 0))


.section ".flash_header_start", "x"
	b	go
/* Informationen to the FCB can be found on p. 495 @ i.MX53 Multimedia Applications Processor Reference Manual, Rev. 2, 12/2011 */
/* on device power the boot ROM will start searching for the firmware configuration block (FCB) */
	.word	0x20424346	/* "FCB " marker */
	.word	0x01	/* FCB version number */
	.org	0x68
	.word	0x0	/* primary image starting page number */
	.word	0x0	/* secondary image starting page number */
	.word	0x6b
	.word	0x6b
	.word	0x0	/* DBBT start page (0 == NO DBBT) */
	.word	0	/* Bad block marker offset in main area (unused) */
	.org	0xac
	.word	0	/* BI Swap disabled */
	.word	0	/* Bad Block marker offset in spare area */
#if 1
	.word	0
tRFC_VAL:
	.byte	tRFC
tXS_VAL:
	.byte	tXS
tXP_VAL:
	.byte	tXP
tXPDLL_VAL:
	.byte	tXPDLL
tFAW_VAL:
	.byte	tFAW
tCL_VAL:
	.byte	tCL
tRCD_VAL:
	.byte	tRCD
tRP_VAL:
	.byte	tRP
tRC_VAL:
	.byte	tRC
tRAS_VAL:
	.byte	tRAS
tRPA_VAL:
	.byte	tRPA
tWR_VAL:
	.byte	tWR
tMRD_VAL:
	.byte	tMRD
tCWL_VAL:
	.byte	tCWL
tDLLK_VAL:
	.byte	tDLLK
tRTP_VAL:
	.byte	tRTP
tWTR_VAL:
	.byte	tWTR
tRRD_VAL:
	.byte	tRRD
tXPR_VAL:
	.byte	tXPR
tSDE_RST_VAL:
	.byte	tSDE_RST
tRST_CKE_VAL:
	.byte	tRST_CKE
	.align	2
W_ESDCTL_VAL:
	.word	ESDCTL_VAL
W_ESDMISC_VAL:
	.word	ESDMISC_VAL
W_ESDCFG0_VAL:
	.word	ESDCFG0_VAL
W_ESDCFG1_VAL:
	.word	ESDCFG1_VAL
W_ESDCFG2_VAL:
	.word	ESDCFG2_VAL
W_ESDOR_VAL:
	.word	ESDOR_VAL
W_ESDSCR_MRS_VAL:
	.word	ESDSCR_MRS_VAL
#endif
#if 0
#undef ESDCFG0_VAL
#undef ESDCFG1_VAL
#undef ESDMISC_VAL

#define ESDCFG0_VAL 0x9f5152e3
#define ESDCFG1_VAL 0xb68e8a63
#define ESDMISC_VAL 0x00011740
W_ESDMISC_VAL_FSL:
	.word	ESDMISC_VAL
W_ESDCFG0_VAL_FSL:
	.word	ESDCFG0_VAL
W_ESDCFG1_VAL_FSL:
	.word	ESDCFG1_VAL
#endif
