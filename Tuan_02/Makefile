CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size


# APP = src/Bai1.c
APP = src/Bai2.c
# APP = src/Bai3.c
# APP = src/Bai4.c
# APP = src/main.c

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -Iinc
LDFLAGS = -T STM32F103XB_FLASH.ld -nostartfiles


SRCS = $(APP) src/startup_stm32f103xb.s
TARGET = build/counter_program

all: $(TARGET).bin

$(TARGET).elf: $(SRCS)
	mkdir -p build
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

flash: $(TARGET).bin
	sudo st-flash write $(TARGET).bin 0x08000000

clean:
	rm -rf build/*
