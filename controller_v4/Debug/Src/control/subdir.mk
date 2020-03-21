################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/control/PWM.c \
../Src/control/control.c \
../Src/control/inner.c \
../Src/control/lookupTables.c \
../Src/control/math_fcns.c \
../Src/control/outer.c \
../Src/control/torque.c 

OBJS += \
./Src/control/PWM.o \
./Src/control/control.o \
./Src/control/inner.o \
./Src/control/lookupTables.o \
./Src/control/math_fcns.o \
./Src/control/outer.o \
./Src/control/torque.o 

C_DEPS += \
./Src/control/PWM.d \
./Src/control/control.d \
./Src/control/inner.d \
./Src/control/lookupTables.d \
./Src/control/math_fcns.d \
./Src/control/outer.d \
./Src/control/torque.d 


# Each subdirectory must supply rules for building sources it contributes
Src/control/PWM.o: ../Src/control/PWM.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/PWM.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/control.o: ../Src/control/control.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/control.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/inner.o: ../Src/control/inner.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/inner.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/lookupTables.o: ../Src/control/lookupTables.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/lookupTables.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/math_fcns.o: ../Src/control/math_fcns.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/math_fcns.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/outer.o: ../Src/control/outer.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/outer.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/control/torque.o: ../Src/control/torque.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DDEBUG -c -I../Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v3/Drivers/STM32F7xx_HAL_Driver/Inc" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Inc" -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I"C:/Users/jolen/OneDrive/Documents/Capstone/embedded/controller_v4/Drivers/STM32F7xx_HAL_Driver/Inc" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/control/torque.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

