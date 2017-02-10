################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Museum/Museum.cpp 

OBJS += \
./src/Museum/Museum.o 

CPP_DEPS += \
./src/Museum/Museum.d 


# Each subdirectory must supply rules for building sources it contributes
src/Museum/%.o: ../src/Museum/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DNDEBUG -I/home/marvin/lib/ogre/sdk_r/include/OGRE -ISDL2 -I/usr/include/SDL2 -I/home/marvin/workspace/HLMS_DS/src -I/home/marvin/lib/ogre/sdk_r/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_r/include/OGRE/Hlms/Unlit -I/home/marvin/lib/ogre/sdk_r/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_r/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_r/include/OGRE/Overlay -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


