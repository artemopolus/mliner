cmake_minimum_required(VERSION 3.5)

project (ExactoLink VERSION 0.0.1)

set(
	SOURCES
	exlnk_dtAVPack.c
	exlnk_dtPHPack.c
	exlnk_dtTempPack.c
	exlnk_getHeader.c
	exlnk_setHeader.c
	exlnk_Cmd.c
)

set(
	HEADERS
	exlnk_Cmd.h
	exactolink.h
	exlnk_dtAVPack.h
	exlnk_dtPHPack.h
	exlnk_dtTempPack.h
	exlnk_getHeader.h
	exlnk_setHeader.h
	exlnk_options.h
)

add_library(exactolinklib STATIC ${SOURCES} ${HEADERS})
set_target_properties(exactolinklib PROPERTIES PUBLIC_HEADER  "${HEADERS}")

target_include_directories(
	exactolinklib	
	PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)
install(
	TARGETS exactolinklib 
	EXPORT exmlinerlibTargets
	LIBRARY DESTINATION $<CONFIG>
	ARCHIVE DESTINATION $<CONFIG>
	PUBLIC_HEADER DESTINATION incl/exactolink
)
set(Name exactolinklib)
install(FILES "$<TARGET_FILE_DIR:${Name}>/${Name}.pdb" DESTINATION "$<CONFIG>" OPTIONAL)

