################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
galam\ card\ v1.04.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
galam\ card\ v1.04.list \

OBJCOPY_BIN += \
galam\ card\ v1.04.bin \


# All Target
all: galam\ card\ v1.04.elf secondary-outputs

# Tool invocations
galam\ card\ v1.04.elf: $(OBJS) $(USER_OBJS) C:\Users\arnau\Desktop\SMARTRONICS\GALAM_ROBOTICS\firmware\galam\ card\ v1.04b\STM32G070RBTX_FLASH.ld
	arm-none-eabi-gcc -o "galam card v1.04.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\arnau\Desktop\SMARTRONICS\GALAM_ROBOTICS\firmware\galam card v1.04b\STM32G070RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="galam card v1.04.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

galam\ card\ v1.04.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "galam card v1.04.list"
	@echo 'Finished building: $@'
	@echo ' '

galam\ card\ v1.04.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "galam card v1.04.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
