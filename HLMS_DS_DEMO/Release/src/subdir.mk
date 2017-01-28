################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BaseSystem.cpp \
../src/CameraController.cpp \
../src/GameEntityManager.cpp \
../src/GraphicsSystem.cpp \
../src/LogicSystem.cpp \
../src/PbsMaterials.cpp \
../src/PbsMaterialsGameState.cpp \
../src/SdlInputHandler.cpp \
../src/TutorialGameState.cpp 

OBJS += \
./src/BaseSystem.o \
./src/CameraController.o \
./src/GameEntityManager.o \
./src/GraphicsSystem.o \
./src/LogicSystem.o \
./src/PbsMaterials.o \
./src/PbsMaterialsGameState.o \
./src/SdlInputHandler.o \
./src/TutorialGameState.o 

CPP_DEPS += \
./src/BaseSystem.d \
./src/CameraController.d \
./src/GameEntityManager.d \
./src/GraphicsSystem.d \
./src/LogicSystem.d \
./src/PbsMaterials.d \
./src/PbsMaterialsGameState.d \
./src/SdlInputHandler.d \
./src/TutorialGameState.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

