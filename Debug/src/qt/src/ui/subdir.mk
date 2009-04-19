################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/qt/src/ui/Collection_UI.cpp \
../src/qt/src/ui/MainWindow_UI.cpp 

OBJS += \
./src/qt/src/ui/Collection_UI.o \
./src/qt/src/ui/MainWindow_UI.o 

CPP_DEPS += \
./src/qt/src/ui/Collection_UI.d \
./src/qt/src/ui/MainWindow_UI.d 


# Each subdirectory must supply rules for building sources it contributes
src/qt/src/ui/%.o: ../src/qt/src/ui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


