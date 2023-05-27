################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/arm_cfft_init_f32.c \
../Core/Src/main.c \
../Core/Src/measuring.c \
../Core/Src/menu.c \
../Core/Src/pushbutton.c \
../Core/Src/speed_model.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uart.c 

OBJS += \
./Core/Src/arm_cfft_init_f32.o \
./Core/Src/main.o \
./Core/Src/measuring.o \
./Core/Src/menu.o \
./Core/Src/pushbutton.o \
./Core/Src/speed_model.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uart.o 

C_DEPS += \
./Core/Src/arm_cfft_init_f32.d \
./Core/Src/main.d \
./Core/Src/measuring.d \
./Core/Src/menu.d \
./Core/Src/pushbutton.d \
./Core/Src/speed_model.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DHAL_UART_MODULE_ENABLED -DDEBUG -DSTM32F429xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/DSP/Include -I../Utilities/Fonts -I../Drivers/BSP/STM32F429I-Discovery -I../Drivers/BSP/Components/stmpe811 -I../Drivers/BSP/Components/ili9341 -I../Drivers/BSP/Components/Common -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/arm_cfft_init_f32.cyclo ./Core/Src/arm_cfft_init_f32.d ./Core/Src/arm_cfft_init_f32.o ./Core/Src/arm_cfft_init_f32.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/measuring.cyclo ./Core/Src/measuring.d ./Core/Src/measuring.o ./Core/Src/measuring.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/pushbutton.cyclo ./Core/Src/pushbutton.d ./Core/Src/pushbutton.o ./Core/Src/pushbutton.su ./Core/Src/speed_model.cyclo ./Core/Src/speed_model.d ./Core/Src/speed_model.o ./Core/Src/speed_model.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su

.PHONY: clean-Core-2f-Src

