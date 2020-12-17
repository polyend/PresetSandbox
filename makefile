# Target configuration

TARGET		:= preset-sandbox
TOOLCHAIN	:= arm-none-eabi

# Build environment setup

.DEFAULT_GOAL := all

BUILD_DIR	:= build
OBJ_DIR 	:= $(BUILD_DIR)/obj/
RM 			:= rm -rf

SUBDIRS := app drivers libs/Encoder libs/FastCRC libs/SPI libs/SodaqDataFlash libs/core_teensy libs/i2c_t3 

CPP_SRCS += $(shell find ./libs/ -name *.cpp)
CPP_SRCS += $(shell find ./app/ -name *.cpp)
CPP_SRCS += $(shell find ./drivers/ -name *.cpp)
C_SRCS 	 += $(shell find ./libs/ -name *.c)
ASM_SRCS += $(shell find ./libs/ -name *.S)

_CPP_OBJS	= $(patsubst ./%.cpp, %.o, $(CPP_SRCS))
_C_OBJS	= $(patsubst ./%.c, %.o, $(C_SRCS))
_ASM_OBJS	= $(patsubst ./%.S, %.o, $(ASM_SRCS))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(_CPP_OBJS))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(_C_OBJS))
OBJS	+= $(addprefix $(OBJ_DIR)/, $(_ASM_OBJS))

INCLUDES=-I./libs/core_teensy -I./libs/FastCRC -I./libs/i2c_t3 -I./libs/SodaqDataFlash -I./libs/SPI -I./drivers
DEFINES=-D__NEWLIB__ -DUSB_SERIAL -DDEBUG -D__CODE_RED -DARDUINO=10613 -DTEENSYDUINO=132 -D_FPU_PRESENT -DF_CPU=96000000 -DLAYOUT_US_ENGLISH -D__MK20DX256__ -DSDK_DEBUGCONSOLE=0 -D__NEWLIB__
CC_OPTIONS=-O1 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)"

# Build rules

$(OBJ_DIR)/%.o: %.cpp
	$(TOOLCHAIN)-c++ $(DEFINES) $(INCLUDES) $(CC_OPTIONS) -fno-rtti -fno-exceptions  -o "$@" "$<"

$(OBJ_DIR)/%.o: %.c
	$(TOOLCHAIN)-gcc $(DEFINES) $(INCLUDES) $(CC_OPTIONS) -o "$@" "$<"

$(OBJ_DIR)/%.o: %.S
	$(TOOLCHAIN)-gcc -c -x assembler-with-cpp $(DEFINES) $(INCLUDES) -g3 -mcpu=cortex-m4 -mthumb -o "$@" "$<"

$(TARGET).axf: $(OBJS) $(USER_OBJS)
	$(TOOLCHAIN)-c++ -L./libs/core_teensy -Xlinker -Map="$(BUILD_DIR)/$(TARGET).map" -Xlinker --defsym=__rtc_localtime=0 -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m4 -mthumb -T "mk20dx256.ld" -o "$(BUILD_DIR)/$(TARGET).axf" $(OBJS) $(USER_OBJS) $(LIBS)
	$(MAKE) --no-print-directory post-build
	
# Main rules

all:
	$(MAKE) --no-print-directory pre-build
	$(MAKE) --no-print-directory main-build

main-build: $(TARGET).axf

pre-build:
	$(foreach dir,$(SUBDIRS),mkdir -p $(OBJ_DIR)/$(dir);)

post-build:
	$(TOOLCHAIN)-objcopy -v -O ihex "$(BUILD_DIR)/$(TARGET).axf" "$(BUILD_DIR)/$(TARGET).hex" ; \
	$(TOOLCHAIN)-size "$(BUILD_DIR)/$(TARGET).axf" ; \
	$(TOOLCHAIN)-objcopy -v -O binary "$(BUILD_DIR)/$(TARGET).axf" "$(BUILD_DIR)/$(TARGET).bin"

clean:
	-$(RM) $(BUILD_DIR)
	
.PHONY: all clean main-build pre-build post-build
