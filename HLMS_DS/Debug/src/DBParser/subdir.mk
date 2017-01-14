################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DBParser/DBParser.cpp \
../src/DBParser/JsonParser.cpp \
../src/DBParser/ParamVecParser.cpp \
../src/DBParser/TextureParser.cpp 

OBJS += \
./src/DBParser/DBParser.o \
./src/DBParser/JsonParser.o \
./src/DBParser/ParamVecParser.o \
./src/DBParser/TextureParser.o 

CPP_DEPS += \
./src/DBParser/DBParser.d \
./src/DBParser/JsonParser.d \
./src/DBParser/ParamVecParser.d \
./src/DBParser/TextureParser.d 


# Each subdirectory must supply rules for building sources it contributes
src/DBParser/%.o: ../src/DBParser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


