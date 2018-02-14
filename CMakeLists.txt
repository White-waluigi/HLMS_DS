cmake_minimum_required (VERSION 3.4)
project (HLMS_DS)


set(PF_LIB_EXT "so")
set(PF_LIB_FOLDER "")
set(PF_LIB_OPT_FOLDER "OGRE/")
if (WIN32)
	set(PF_LIB_FOLDER "*/")
	set(PF_LIB_OPT_FOLDER "opt/")	
	set(PF_LIB_EXT "lib")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release
RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(OGRE_SDK_FOLDER "/home/user/lib/ogre/sdk_r" CACHE PATH "SDK Folder of OGRE")
set(OGRE_DEPS_FOLDER "/usr/include/" CACHE PATH "Ogre Dependencies")

IF(NOT WIN32) 
  option(USE_BOOST "Use the Boost Library" true)
ENDIF()
#Load default

include(CMakeConfig.txt)






if(${CMAKE_BUILD_TYPE}!="Debug")
endif()
add_definitions(-DNDEBUG)
message(STATUS "Copying Media and Library files from ${PROJECT_SOURCE_DIR}/data/ to ${PROJECT_BINARY_DIR}")

file(GLOB DEMO_LIB_FILES
  "${OGRE_SDK_FOLDER}/lib/${PF_LIB_FOLDER}*Ogre*.${PF_LIB_EXT}*"
)
file(COPY ${DEMO_LIB_FILES} DESTINATION "${PROJECT_BINARY_DIR}")
file(GLOB DEMO_RTLIB_FILES
  "${OGRE_SDK_FOLDER}/lib/${PF_LIB_FOLDER}${PF_LIB_OPT_FOLDER}/*.${PF_LIB_EXT}*"
)
file(COPY ${DEMO_RTLIB_FILES} DESTINATION "${PROJECT_BINARY_DIR}")

file(GLOB DEMO_CFG_FILES
  "${PROJECT_SOURCE_DIR}/data/*"
)

file(COPY ${DEMO_CFG_FILES} DESTINATION "${PROJECT_BINARY_DIR}")

file(GLOB DEMO_DLL_FILES
  "${OGRE_SDK_FOLDER}/bin/*/*.dll"
)
file(COPY ${DEMO_DLL_FILES} DESTINATION "${PROJECT_BINARY_DIR}")
message(STATUS "Copying done")




include_directories("${PROJECT_SOURCE_DIR}/HLMS_DS/src")
include_directories("${OGRE_SDK_FOLDER}/include/OGRE")
include_directories("${OGRE_SDK_FOLDER}/include/OGRE/Hlms/Pbs")
include_directories("${OGRE_SDK_FOLDER}/include/OGRE/Vao")
include_directories("${OGRE_SDK_FOLDER}/include/OGRE/Hlms")
include_directories("${OGRE_SDK_FOLDER}/include/OGRE/Overlay")

include_directories("${PROJECT_SOURCE_DIR}/src/ds")
include_directories("${PROJECT_SOURCE_DIR}/src/demo")

include (src/CMakeLists.txt)
set(DEMO_INCLUDE
    ${OGRE_SDK_FOLDER}/include/OGRE
    ${OGRE_DEPS_FOLDER}/include
    ${OGRE_SDK_FOLDER}/include/OGRE/Hlms/Pbs
    ${OGRE_SDK_FOLDER}/include/OGRE/Hlms/Unlit
    ${OGRE_SDK_FOLDER}/include/OGRE/Vao
    ${OGRE_SDK_FOLDER}/include/OGRE/Hlms
    ${OGRE_SDK_FOLDER}/include/OGRE/Overlay
)
include_directories(${DEMO_INCLUDE})
set(DEMO_LIBS 
    OgreHlmsUnlit
    OgreHlmsUnlitMobile
    OgreMain
    OgreMeshLodGenerator
    OgreOverlay
    SDL2
)
set(DS_LIBS 
    OgreHlmsUnlit
    OgreHlmsUnlitMobile
    OgreMain
)
IF(WIN32) # Check if we are on Windows
list(APPEND DEMO_LIBS 
	winmm
	imm32
	version
	msimg32	
)
ENDIF()
IF(USE_BOOST) # Check if we are on Windows
list(APPEND DS_LIBS 
    boost_system
    boost_thread
)
ENDIF()

link_directories("${PROJECT_SOURCE_DIR}/HLMS_DS_DEMO")
link_directories("${PROJECT_BINARY_DIR}")
link_directories("${OGRE_DEPS_FOLDER}/lib")
link_directories("${OGRE_DEPS_FOLDER}/lib/Release")





add_library(HLMS_DS SHARED ${SOURCES_DS})
target_link_libraries(HLMS_DS ${DS_LIBS})





add_executable(HLMS_DS_DEMO  ${SOURCES_DEMO})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT HLMS_DS_DEMO)
target_link_libraries(HLMS_DS_DEMO ${DEMO_LIBS})
target_link_libraries(HLMS_DS_DEMO HLMS_DS)

IF(WIN32) # Check if we are on Windows 
  if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(HLMS_DS_DEMO PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(HLMS_DS_DEMO PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")

  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  else()
    message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

