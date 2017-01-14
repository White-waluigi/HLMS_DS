################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Datablocks/AutoParams/DSAutoParamParser.cpp \
../src/Datablocks/AutoParams/DSAutoParamTime.cpp \
../src/Datablocks/AutoParams/DSAutoTime.cpp 

OBJS += \
./src/Datablocks/AutoParams/DSAutoParamParser.o \
./src/Datablocks/AutoParams/DSAutoParamTime.o \
./src/Datablocks/AutoParams/DSAutoTime.o 

CPP_DEPS += \
./src/Datablocks/AutoParams/DSAutoParamParser.d \
./src/Datablocks/AutoParams/DSAutoParamTime.d \
./src/Datablocks/AutoParams/DSAutoTime.d 


# Each subdirectory must supply rules for building sources it contributes
src/Datablocks/AutoParams/%.o: ../src/Datablocks/AutoParams/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


