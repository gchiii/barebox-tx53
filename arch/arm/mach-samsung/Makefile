obj-y += s3c-timer.o generic.o
obj-$(CONFIG_RESET_SOURCE) += reset_source.o
obj-lowlevel-$(CONFIG_ARCH_S3C24xx) += lowlevel-s3c24x0.o
obj-lowlevel-$(CONFIG_ARCH_S5PCxx) += lowlevel-s5pcxx.o
obj-$(CONFIG_ARCH_S3C24xx) += gpio-s3c24x0.o clocks-s3c24xx.o mem-s3c24x0.o
obj-$(CONFIG_ARCH_S3C64xx) += gpio-s3c64xx.o clocks-s3c64xx.o mem-s3c64xx.o
obj-$(CONFIG_ARCH_S5PCxx) += gpio-s5pcxx.o clocks-s5pcxx.o mem-s5pcxx.o
obj-$(CONFIG_S3C_LOWLEVEL_INIT) += $(obj-lowlevel-y)
