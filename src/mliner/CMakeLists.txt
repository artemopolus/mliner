cmake_minimum_required(VERSION 3.5)

project (ExMlinerLib VERSION 0.0.1)

list(APPEND CMAKE_PREFIX_PATH "J:\\WorkspaceFast\\ThirdParty\\zeromq\\intall\\share\\cmake\\cppzmq")
list(APPEND CMAKE_PREFIX_PATH "J:\\WorkspaceFast\\ThirdParty\\zeromq\\install_zmq\\CMake")
add_definitions(-DEXT_COMPILE)
add_compile_definitions(EXACTO_BUFFER_UINT8_SZ=512)
add_compile_definitions(EXACTO_EXTENDED_BUFFER_UINT8_SZ=1024)


find_package(cppzmq)

set(ZMQ_PATH ../zmq)

set(ZMQ_SOURCES
	${ZMQ_PATH}/exmliner.cpp
	${ZMQ_PATH}/exmliner_zmq.cpp
)
set(ZMQ_HEADERS
	${ZMQ_PATH}/exmliner_zmq.h
	${ZMQ_PATH}/exmliner.h
)

set(
	SOURCES
	mliner.c
	mliner_sec.c
	# mliner_dev_main.c
	mliner_secdev_win.c
)
set(
	HEADERS
	mliner.h
	mliner_sec.h
	mliner_secdev.h
	mliner_secdev_impl.h
	mliner_headers.h
	mliner_basedev.h
)
add_library(exmlinerseclib 
	STATIC 
	${SOURCES} ${HEADERS} 
	${ZMQ_SOURCES} ${ZMQ_HEADERS}
	${ZMQ_PATH}/mliner_secdev_impl_zmq.cpp
	# ${ZMQ_PATH}/zmq_impl_maindev.cpp
)

target_link_libraries(exmlinerseclib 
	exactolinklib
	exactobufferlib
	cppzmq
)

set_target_properties(
	exmlinerseclib PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)
set_target_properties(exmlinerseclib PROPERTIES PUBLIC_HEADER  "${HEADERS}")


target_include_directories(exmlinerseclib	
	PRIVATE	
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/..
		# ${CMAKE_CURRENT_SOURCE_DIR}/${ZMQ_PATH}
)

install(
	TARGETS exmlinerseclib 
	EXPORT exmlinerlibTargets
	LIBRARY DESTINATION $<CONFIG>
	ARCHIVE DESTINATION $<CONFIG>
	PUBLIC_HEADER DESTINATION incl/mliner
)
set_target_properties(exmlinerseclib PROPERTIES PUBLIC_HEADER  "${ZMQ_HEADERS}")
install(
	TARGETS exmlinerseclib 
	PUBLIC_HEADER DESTINATION incl/zmq
)


set(Name exmlinerseclib)
install(FILES "$<TARGET_FILE_DIR:${Name}>/${Name}.pdb" DESTINATION "$<CONFIG>" OPTIONAL)



set(
	SOURCES2
	mliner.c
	mliner_main.c
	# mliner_maindev_win.c
	mliner_dev_main.c
)
set(
	HEADERS2
	mliner.h
	mliner_headers.h
	mliner_basedev.h
	mliner_main.h
	mliner_maindev.h
	mliner_maindev_impl.h
)


add_library(exmlinermainlib 
	STATIC 
	${SOURCES2} ${HEADERS2} 	
	${ZMQ_SOURCES} ${ZMQ_HEADERS}
	${ZMQ_PATH}/zmq_impl_maindev.cpp
)

target_link_libraries(exmlinermainlib 
	exactolinklib
	exactobufferlib
	cppzmq
)

set_target_properties(
	exmlinermainlib PROPERTIES
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
)
set_target_properties(exmlinermainlib PROPERTIES PUBLIC_HEADER  "${HEADERS2}")


target_include_directories(exmlinermainlib	
	PRIVATE	
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/..
)

get_target_property(pdb_output_dir exmlinermainlib PDB_OUTPUT_DIRECTORY)
get_target_property(pdb_name exmlinermainlib PDB_NAME)


install(
	TARGETS exmlinermainlib 
	EXPORT exmlinerlibTargets
	RUNTIME DESTINATION $<CONFIG>
	LIBRARY DESTINATION $<CONFIG>
	ARCHIVE DESTINATION $<CONFIG>
	PUBLIC_HEADER DESTINATION incl/mliner
)
set(Name exmlinermainlib)
install(FILES "$<TARGET_FILE_DIR:${Name}>/${Name}.pdb" DESTINATION "$<CONFIG>" OPTIONAL)
# install(FILES ${HEADERS} DESTINATION incl)

# install(
# 	EXPORT exmlinerlibTargets
# 	FILE exmlinerlibConfig.cmake
# 	NAMESPACE ExMlinerLib::
# 	DESTINATION cmake/ExMlinerLib
# )


