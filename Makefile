CROSS ?=  /opt/gcc-arm-none-eabi-4_9-2015q3/bin/arm-none-eabi
CC := $(CROSS)-gcc

CFLAGS := -O0 -g -Wall -mlittle-endian -mthumb -mcpu=cortex-m3 -mthumb-interwork
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -std=gnu99
LDFLAGS := -g -lgcc -Wl,-TSTM32F103RG_FLASH.ld
LDFLAGS += -Xlinker --gc-sections

CFLAGS += \
	-Iinc/ \
	-ICMSIS/Include \
	-ICMSIS/Device/ST/STM32F1xx/Include

CFLAGS += \
	-DSTM32F10X_XL \
	-DUSE_STDPERIPH_DRIVER \
	-DHSE_VALUE=16000000

OBJS := \
	src/startup_stm32f103xg.o \
	src/system_stm32f10x.o \
	src/syscalls.o \
	src/uart.o \
	src/log.o \
	src/adc.o \
	src/dac.o \
	src/buttons.o \
	src/cli/cli.o \
	src/cli/cmd_free.o \
	src/cli/cmd_info.o \
	src/cli/cmd_ps.o \
	src/cli/cmd_uname.o \
	src/main.o

# USB
CFLAGS += \
	-Ilibs/STM32_USB-FS-Device_Driver/inc

OBJS += \
	src/usb_glue.o \
	src/usb_desc.o \
	src/usb_endp.o \
	src/usb_prop.o \
	src/usb_pwr.o \
	libs/STM32_USB-FS-Device_Driver/src/usb_init.o \
	libs/STM32_USB-FS-Device_Driver/src/usb_core.o \
	libs/STM32_USB-FS-Device_Driver/src/usb_regs.o \
	libs/STM32_USB-FS-Device_Driver/src/usb_sil.o

include Makefile.stdperiph
include Makefile.mw

ALL_DEPS := $(patsubst %.o,%.d,$(OBJS))

all: dcload.bin

clean:
	rm -f dcload.bin dcload.elf $(OBJS) $(ALL_DEPS)

flash: dcload.bin
	~/bin/stlink/st-flash write dcload.bin 0x08000000

%.bin:%.elf
	@$(CROSS)-objcopy -O binary $< $@
	@echo "Firmware size: "
	@du -hs $@

dcload.elf: $(OBJS)
	@echo "LD $@"
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o:%.c
	@echo "CC $<"
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(CFLAGS) $<
	@$(CC) $(CFLAGS) -c -o $@ $<

%.o:%.s
	@echo "CC $<"
	@$(CC) -MM -MF $(subst .o,.d,$@) -MP -MT "$@ $(subst .o,.d,$@)" $(CFLAGS) $<
	@$(CC) $(CFLAGS) -c -o $@ $<	

ifneq ("$(MAKECMDGOALS)","clean")
-include $(ALL_DEPS)
endif
