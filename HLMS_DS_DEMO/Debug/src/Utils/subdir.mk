################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Utils/HdrUtils.cpp 

OBJS += \
./src/Utils/HdrUtils.o 

CPP_DEPS += \
./src/Utils/HdrUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/Utils/%.o: ../src/Utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -ISDL2 -I/usr/include/SDL2 -I/home/marvin/workspace/HLMS_DS/src -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Unlit -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


