################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CamLib/mjpeg_encoder.cpp \
../src/CamLib/omx_encoder.cpp \
../src/CamLib/ttimer.cpp \
../src/CamLib/v4lcam.cpp \
../src/CamLib/video_encoder.cpp \
../src/CamLib/video_io.cpp \
../src/CamLib/videocam.cpp \
../src/CamLib/videodecoder.cpp 

OBJS += \
./src/CamLib/mjpeg_encoder.o \
./src/CamLib/omx_encoder.o \
./src/CamLib/ttimer.o \
./src/CamLib/v4lcam.o \
./src/CamLib/video_encoder.o \
./src/CamLib/video_io.o \
./src/CamLib/videocam.o \
./src/CamLib/videodecoder.o 

CPP_DEPS += \
./src/CamLib/mjpeg_encoder.d \
./src/CamLib/omx_encoder.d \
./src/CamLib/ttimer.d \
./src/CamLib/v4lcam.d \
./src/CamLib/video_encoder.d \
./src/CamLib/video_io.d \
./src/CamLib/videocam.d \
./src/CamLib/videodecoder.d 


# Each subdirectory must supply rules for building sources it contributes
src/CamLib/%.o: ../src/CamLib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I../include -O3 -g3 -Wall -c -fmessage-length=0 -mcpu=cortex-a7 -mfpu=neon-vfpv4 -ftree-vectorize -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


