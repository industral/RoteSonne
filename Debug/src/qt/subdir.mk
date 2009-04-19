################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/qt/Collection.o \
../src/qt/Collection_UI.o \
../src/qt/LoadUI.o \
../src/qt/MainWindow_UI.o \
../src/qt/common.o \
../src/qt/moc_Collection_UI.o \
../src/qt/moc_MainWindow_UI.o \
../src/qt/qrc_rotesonne.o 

CPP_SRCS += \
../src/qt/moc_Collection.cpp \
../src/qt/moc_Collection_UI.cpp \
../src/qt/moc_MainWindow.cpp \
../src/qt/moc_MainWindow_UI.cpp \
../src/qt/qrc_rotesonne.cpp 

OBJS += \
./src/qt/moc_Collection.o \
./src/qt/moc_Collection_UI.o \
./src/qt/moc_MainWindow.o \
./src/qt/moc_MainWindow_UI.o \
./src/qt/qrc_rotesonne.o 

CPP_DEPS += \
./src/qt/moc_Collection.d \
./src/qt/moc_Collection_UI.d \
./src/qt/moc_MainWindow.d \
./src/qt/moc_MainWindow_UI.d \
./src/qt/qrc_rotesonne.d 


# Each subdirectory must supply rules for building sources it contributes
src/qt/%.o: ../src/qt/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


