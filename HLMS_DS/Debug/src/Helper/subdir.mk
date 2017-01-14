################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Helper/DSShaderGenerator.cpp \
../src/Helper/HLMSDSWorkspaceListener.cpp 

OBJS += \
./src/Helper/DSShaderGenerator.o \
./src/Helper/HLMSDSWorkspaceListener.o 

CPP_DEPS += \
./src/Helper/DSShaderGenerator.d \
./src/Helper/HLMSDSWorkspaceListener.d 


# Each subdirectory must supply rules for building sources it contributes
src/Helper/%.o: ../src/Helper/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


