################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DBParser/Template/JSONMaterialCreator.cpp \
../src/DBParser/Template/MTMultiData.cpp \
../src/DBParser/Template/MaterialTemplate.cpp 

OBJS += \
./src/DBParser/Template/JSONMaterialCreator.o \
./src/DBParser/Template/MTMultiData.o \
./src/DBParser/Template/MaterialTemplate.o 

CPP_DEPS += \
./src/DBParser/Template/JSONMaterialCreator.d \
./src/DBParser/Template/MTMultiData.d \
./src/DBParser/Template/MaterialTemplate.d 


# Each subdirectory must supply rules for building sources it contributes
src/DBParser/Template/%.o: ../src/DBParser/Template/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


