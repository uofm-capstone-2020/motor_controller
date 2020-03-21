################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/sensors/HCPL7510.c \
../Src/sensors/HO250S.c \
../Src/sensors/PGA411.c \
../Src/sensors/amc1106.c \
../Src/sensors/battery.c \
../Src/sensors/thermistor.c 

OBJS += \
./Src/sensors/HCPL7510.o \
./Src/sensors/HO250S.o \
./Src/sensors/PGA411.o \
./Src/sensors/amc1106.o \
./Src/sensors/battery.o \
./Src/sensors/thermistor.o 

C_DEPS += \
./Src/sensors/HCPL7510.d \
./Src/sensors/HO250S.d \
./Src/sensors/PGA411.d \
./Src/sensors/amc1106.d \
./Src/sensors/battery.d \
./Src/sensors/thermistor.d 


# Each subdirectory must supply rules for building sources it contributes
Src/sensors/HCPL7510.o: ../Src/sensors/HCPL7510.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/HCPL7510.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sensors/HO250S.o: ../Src/sensors/HO250S.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/HO250S.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sensors/PGA411.o: ../Src/sensors/PGA411.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/PGA411.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sensors/amc1106.o: ../Src/sensors/amc1106.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/amc1106.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sensors/battery.o: ../Src/sensors/battery.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/battery.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/sensors/thermistor.o: ../Src/sensors/thermistor.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/sensors/thermistor.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

