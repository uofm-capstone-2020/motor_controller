################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/utils/subdir.mk
-include Src/sensors/subdir.mk
-include Src/modules/subdir.mk
-include Src/control/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
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
controller_v4.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
controller_v4.list \

OBJCOPY_BIN += \
controller_v4.bin \


# All Target
all: controller_v4.elf secondary-outputs

# Tool invocations
controller_v4.elf: $(OBJS) $(USER_OBJS) C:\Users\jolen\OneDrive\Documents\Capstone\embedded\controller_v4\STM32F767ZGTX_FLASH.ld
	arm-none-eabi-gcc -o "controller_v4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\jolen\OneDrive\Documents\Capstone\embedded\controller_v4\STM32F767ZGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="controller_v4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

controller_v4.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "controller_v4.list"
	@echo 'Finished building: $@'
	@echo ' '

controller_v4.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "controller_v4.bin"
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
