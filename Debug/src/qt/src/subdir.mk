################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/qt/src/Collection.cpp \
../src/qt/src/LoadUI.cpp \
../src/qt/src/common.cpp 

OBJS += \
./src/qt/src/Collection.o \
./src/qt/src/LoadUI.o \
./src/qt/src/common.o 

CPP_DEPS += \
./src/qt/src/Collection.d \
./src/qt/src/LoadUI.d \
./src/qt/src/common.d 


# Each subdirectory must supply rules for building sources it contributes
src/qt/src/%.o: ../src/qt/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


