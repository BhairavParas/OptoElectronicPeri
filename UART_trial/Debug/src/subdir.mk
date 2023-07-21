################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/UART_trial.c \
../src/uart_api.c 

SRC_OBJS += \
./src/UART_trial.doj \
./src/uart_api.doj 

C_DEPS += \
./src/UART_trial.d \
./src/uart_api.d 


# Each subdirectory must supply rules for building sources it contributes
src/UART_trial.doj: ../src/UART_trial.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="UART_trial" -proc ADSP-21369 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-1855dd4341d0f2a28a10ea597448c89e.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -gnu-style-dependencies -MD -Mo "src/UART_trial.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/uart_api.doj: ../src/uart_api.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="UART_trial" -proc ADSP-21369 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-1855dd4341d0f2a28a10ea597448c89e.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -gnu-style-dependencies -MD -Mo "src/uart_api.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


