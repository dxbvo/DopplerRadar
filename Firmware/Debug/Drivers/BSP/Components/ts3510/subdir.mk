################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/ts3510/ts3510.c 

OBJS += \
./Drivers/BSP/Components/ts3510/ts3510.o 

C_DEPS += \
./Drivers/BSP/Components/ts3510/ts3510.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ts3510/%.o Drivers/BSP/Components/ts3510/%.su Drivers/BSP/Components/ts3510/%.cyclo: ../Drivers/BSP/Components/ts3510/%.c Drivers/BSP/Components/ts3510/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -DARM_MATH_CM4 -c -I../Core/Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/DSP/Include -I../Utilities/Fonts -I../Drivers/BSP/STM32F429I-Discovery -I../Drivers/BSP/Components/stmpe811 -I../Drivers/BSP/Components/ili9341 -I../Drivers/BSP/Components/Common -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-ts3510

clean-Drivers-2f-BSP-2f-Components-2f-ts3510:
	-$(RM) ./Drivers/BSP/Components/ts3510/ts3510.cyclo ./Drivers/BSP/Components/ts3510/ts3510.d ./Drivers/BSP/Components/ts3510/ts3510.o ./Drivers/BSP/Components/ts3510/ts3510.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-ts3510

