################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Datablocks/DSDatablock.cpp \
../src/Datablocks/DSDatablockHelper.cpp \
../src/Datablocks/DSLightDatablock.cpp \
../src/Datablocks/DSMaterialDatablock.cpp 

OBJS += \
./src/Datablocks/DSDatablock.o \
./src/Datablocks/DSDatablockHelper.o \
./src/Datablocks/DSLightDatablock.o \
./src/Datablocks/DSMaterialDatablock.o 

CPP_DEPS += \
./src/Datablocks/DSDatablock.d \
./src/Datablocks/DSDatablockHelper.d \
./src/Datablocks/DSLightDatablock.d \
./src/Datablocks/DSMaterialDatablock.d 


# Each subdirectory must supply rules for building sources it contributes
src/Datablocks/%.o: ../src/Datablocks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

