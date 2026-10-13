cmake_minimum_required (VERSION 3.20)
project (CruiseControl__CruiseControl_FMU)

set(CREATE_FMU_TARGET CruiseControl__CruiseControl_FMU)

# Select the right binary directory for the output
if (UNIX AND NOT WIN32)
 if (CMAKE_SIZEOF_VOID_P MATCHES "8")
   set(FMU_BINARY_DIRECTORY binaries/linux64)
 else ()
   set(FMU_BINARY_DIRECTORY binaries/linux32)
 endif ()
else ()
 if (CMAKE_SIZEOF_VOID_P MATCHES "8")
   set(FMU_BINARY_DIRECTORY binaries/win64)
 else ()
   set(FMU_BINARY_DIRECTORY binaries/win32)
 endif ()
endif ()

# build the source
include_directories("../../../../specifications/FMI_for_ModelExchange_v1.0/")
add_library(CruiseControl__CruiseControl_FMU SHARED
	sources/CruiseControl_CruiseControl.c
	sources/CruiseControl_CruiseControl_dll.c
	sources/CruiseControl__CruiseControl_FMU.c
	sources/CruiseRegulation_CruiseControl.c
	sources/CruiseSpeedMgt_CruiseControl.c
	sources/kcg_consts.c
	sources/kcg_types.c
	sources/SaturateThrottle_CruiseControl.c
)
set_target_properties(CruiseControl__CruiseControl_FMU PROPERTIES PREFIX "")

# copy the binaries  
add_custom_command(TARGET ${CREATE_FMU_TARGET}
  POST_BUILD
  COMMAND
  ${CMAKE_COMMAND} -E "copy"
      "$<TARGET_FILE:${CREATE_FMU_TARGET}>"
      "${CMAKE_CURRENT_SOURCE_DIR}/../${FMU_BINARY_DIRECTORY}/${CREATE_FMU_TARGET}${CMAKE_SHARED_LIBRARY_SUFFIX}"
)

# Build the zipped fmu file on the source parent directory
add_custom_command(TARGET ${CREATE_FMU_TARGET}
  POST_BUILD
  COMMAND
      ${CMAKE_COMMAND} -E "tar" "cvf" "${CMAKE_CURRENT_SOURCE_DIR}/../${CREATE_FMU_TARGET}.fmu"
      "--format=zip"
      "modelDescription.xml"
      "sources/"
      "model.png"
      "binaries/"
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  COMMENT "${CMAKE_CURRENT_SOURCE_DIR}/../${CREATE_FMU_TARGET}.fmu content"
)

