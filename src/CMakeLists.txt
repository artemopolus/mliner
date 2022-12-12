cmake_minimum_required(VERSION 3.5)



SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

FOREACH(subdir ${SUBDIRS})
  ADD_SUBDIRECTORY(${subdir})
ENDFOREACH()

install(
	EXPORT exmlinerlibTargets 
	FILE exmlinerlibConfig.cmake
	NAMESPACE ExMlinerLib::
	DESTINATION cmake/ExMlinerLib
)

