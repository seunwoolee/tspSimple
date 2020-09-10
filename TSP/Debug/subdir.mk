################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TSP.cpp \
../TspAlgorithm.cpp \
../TspCity.cpp \
../TspView.cpp \
../stdafx.cpp 

OBJS += \
./TSP.o \
./TspAlgorithm.o \
./TspCity.o \
./TspView.o \
./stdafx.o 

CPP_DEPS += \
./TSP.d \
./TspAlgorithm.d \
./TspCity.d \
./TspView.d \
./stdafx.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/eclipse-workspaceTSPS/TSP/inc -I/root/eclipse-workspaceTSPS/TSP/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


