################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TSP.cpp \
../src/TspAlgorithm.cpp \
../src/TspCity.cpp \
../src/TspView.cpp \
../src/stdafx.cpp 

OBJS += \
./src/TSP.o \
./src/TspAlgorithm.o \
./src/TspCity.o \
./src/TspView.o \
./src/stdafx.o 

CPP_DEPS += \
./src/TSP.d \
./src/TspAlgorithm.d \
./src/TspCity.d \
./src/TspView.d \
./src/stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/eclipse-workspace/TSP/inc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

