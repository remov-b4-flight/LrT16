################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/USBMIDI/Src/usbd_midi.c 

OBJS += \
./Middlewares/USBMIDI/Src/usbd_midi.o 

C_DEPS += \
./Middlewares/USBMIDI/Src/usbd_midi.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/USBMIDI/Src/%.o Middlewares/USBMIDI/Src/%.su Middlewares/USBMIDI/Src/%.cyclo: ../Middlewares/USBMIDI/Src/%.c Middlewares/USBMIDI/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Middlewares/USBMIDI/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Middlewares-2f-USBMIDI-2f-Src

clean-Middlewares-2f-USBMIDI-2f-Src:
	-$(RM) ./Middlewares/USBMIDI/Src/usbd_midi.cyclo ./Middlewares/USBMIDI/Src/usbd_midi.d ./Middlewares/USBMIDI/Src/usbd_midi.o ./Middlewares/USBMIDI/Src/usbd_midi.su

.PHONY: clean-Middlewares-2f-USBMIDI-2f-Src

