################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ENCODER_trial.c \
../src/ENC_api.c 

SRC_OBJS += \
./src/ENCODER_trial.doj \
./src/ENC_api.doj 

C_DEPS += \
./src/ENCODER_trial.d \
./src/ENC_api.d 


# Each subdirectory must supply rules for building sources it contributes
src/ENCODER_trial.doj: ../src/ENCODER_trial.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ENCODER_trial" -proc ADSP-21369 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-8c53e7d0acbaae87bf8dbd28e37d724a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -gnu-style-dependencies -MD -Mo "src/ENCODER_trial.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/ENC_api.doj: ../src/ENC_api.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="ENCODER_trial" -proc ADSP-21369 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -g -DCORE0 -D_DEBUG @includes-8c53e7d0acbaae87bf8dbd28e37d724a.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -gnu-style-dependencies -MD -Mo "src/ENC_api.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


