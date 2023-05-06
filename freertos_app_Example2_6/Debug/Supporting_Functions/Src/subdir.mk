################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Supporting_Functions/Src/supporting_Functions.c 

OBJS += \
./Supporting_Functions/Src/supporting_Functions.o 

C_DEPS += \
./Supporting_Functions/Src/supporting_Functions.d 


# Each subdirectory must supply rules for building sources it contributes
Supporting_Functions/Src/%.o Supporting_Functions/Src/%.su: ../Supporting_Functions/Src/%.c Supporting_Functions/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I"C:/JMCruz/ST/STM32CubeIDE_1.10.1/SOTR-I_workspace/freertos_app_Example2_6/Supporting_Functions/Inc" -I"C:/JMCruz/ST/STM32CubeIDE_1.10.1/SOTR-I_workspace/freertos_app_Example2_6/App/Inc" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Supporting_Functions-2f-Src

clean-Supporting_Functions-2f-Src:
	-$(RM) ./Supporting_Functions/Src/supporting_Functions.d ./Supporting_Functions/Src/supporting_Functions.o ./Supporting_Functions/Src/supporting_Functions.su

.PHONY: clean-Supporting_Functions-2f-Src

