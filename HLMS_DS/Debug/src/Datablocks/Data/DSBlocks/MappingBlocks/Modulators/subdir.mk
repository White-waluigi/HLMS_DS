################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBFrameAnimation.cpp \
../src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBRotate.cpp \
../src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBScroll.cpp 

OBJS += \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBFrameAnimation.o \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBRotate.o \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBScroll.o 

CPP_DEPS += \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBFrameAnimation.d \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBRotate.d \
./src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/DSMBScroll.d 


# Each subdirectory must supply rules for building sources it contributes
src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/%.o: ../src/Datablocks/Data/DSBlocks/MappingBlocks/Modulators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


