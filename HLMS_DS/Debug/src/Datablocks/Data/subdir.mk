################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Datablocks/Data/DSAutoParam.cpp \
../src/Datablocks/Data/DSMaterialParam.cpp \
../src/Datablocks/Data/DSMaterialParamType.cpp \
../src/Datablocks/Data/DSPieceParam.cpp \
../src/Datablocks/Data/DSPieceParamType.cpp \
../src/Datablocks/Data/DSPropertyParam.cpp \
../src/Datablocks/Data/DSPropertyParamType.cpp \
../src/Datablocks/Data/DSShadowParam.cpp \
../src/Datablocks/Data/DSShadowTexture.cpp \
../src/Datablocks/Data/DSTextureParam.cpp \
../src/Datablocks/Data/DSTextureParamType.cpp \
../src/Datablocks/Data/DSVec4Param.cpp 

OBJS += \
./src/Datablocks/Data/DSAutoParam.o \
./src/Datablocks/Data/DSMaterialParam.o \
./src/Datablocks/Data/DSMaterialParamType.o \
./src/Datablocks/Data/DSPieceParam.o \
./src/Datablocks/Data/DSPieceParamType.o \
./src/Datablocks/Data/DSPropertyParam.o \
./src/Datablocks/Data/DSPropertyParamType.o \
./src/Datablocks/Data/DSShadowParam.o \
./src/Datablocks/Data/DSShadowTexture.o \
./src/Datablocks/Data/DSTextureParam.o \
./src/Datablocks/Data/DSTextureParamType.o \
./src/Datablocks/Data/DSVec4Param.o 

CPP_DEPS += \
./src/Datablocks/Data/DSAutoParam.d \
./src/Datablocks/Data/DSMaterialParam.d \
./src/Datablocks/Data/DSMaterialParamType.d \
./src/Datablocks/Data/DSPieceParam.d \
./src/Datablocks/Data/DSPieceParamType.d \
./src/Datablocks/Data/DSPropertyParam.d \
./src/Datablocks/Data/DSPropertyParamType.d \
./src/Datablocks/Data/DSShadowParam.d \
./src/Datablocks/Data/DSShadowTexture.d \
./src/Datablocks/Data/DSTextureParam.d \
./src/Datablocks/Data/DSTextureParamType.d \
./src/Datablocks/Data/DSVec4Param.d 


# Each subdirectory must supply rules for building sources it contributes
src/Datablocks/Data/%.o: ../src/Datablocks/Data/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DDEBUG -I/home/marvin/lib/ogre/sdk_d/include/OGRE -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms/Pbs -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Vao -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Hlms -I/home/marvin/lib/ogre/sdk_d/include/OGRE/Overlay -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


