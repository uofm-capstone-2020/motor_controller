################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/modules/_data.c \
../Src/modules/adc.c \
../Src/modules/can.c \
../Src/modules/log.c \
../Src/modules/serial.c 

OBJS += \
./Src/modules/_data.o \
./Src/modules/adc.o \
./Src/modules/can.o \
./Src/modules/log.o \
./Src/modules/serial.o 

C_DEPS += \
./Src/modules/_data.d \
./Src/modules/adc.d \
./Src/modules/can.d \
./Src/modules/log.d \
./Src/modules/serial.d 


# Each subdirectory must supply rules for building sources it contributes
Src/modules/_data.o: ../Src/modules/_data.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/modules/_data.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/modules/adc.o: ../Src/modules/adc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/modules/adc.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/modules/can.o: ../Src/modules/can.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/modules/can.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/modules/log.o: ../Src/modules/log.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/modules/log.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/modules/serial.o: ../Src/modules/serial.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/modules/serial.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

