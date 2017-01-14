################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Datablocks/Data/DSBlocks/MappingBlocks/DSDefaultMBFactory.cpp \
../src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBFactory.cpp \
../src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBModulator.cpp \
../src/Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.cpp 

OBJS += \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSDefaultMBFactory.o \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBFactory.o \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBModulator.o \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.o 

CPP_DEPS += \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSDefaultMBFactory.d \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBFactory.d \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMBModulator.d \
./src/Datablocks/Data/DSBlocks/MappingBlocks/DSMappingBlock.d 


# Each subdirectory must supply rules for building sources it contributes
src/Datablocks/Data/DSBlocks/MappingBlocks/%.o: ../src/Datablocks/Data/DSBlocks/MappingBlocks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


