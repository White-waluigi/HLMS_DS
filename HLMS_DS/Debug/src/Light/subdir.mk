################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Light/DSLight.cpp \
../src/Light/DSLightManager.cpp \
../src/Light/GeomUtils.cpp \
../src/Light/StringValueUtils.cpp 

OBJS += \
./src/Light/DSLight.o \
./src/Light/DSLightManager.o \
./src/Light/GeomUtils.o \
./src/Light/StringValueUtils.o 

CPP_DEPS += \
./src/Light/DSLight.d \
./src/Light/DSLightManager.d \
./src/Light/GeomUtils.d \
./src/Light/StringValueUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/Light/%.o: ../src/Light/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


