################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/utils/flash.c 

OBJS += \
./Src/utils/flash.o 

C_DEPS += \
./Src/utils/flash.d 


# Each subdirectory must supply rules for building sources it contributes
Src/utils/flash.o: ../Src/utils/flash.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/utils/flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

