cmake_minimum_required(	VERSION 3.10)
project(easyredir	VERSION 0)

# Set this source directory as the tree root for project.
set(ae2f_ProjRoot ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Tree root for project")
set(ae2f_submod  .submod CACHE STRING "Relative path to submodule (subdirectory)")
set(ae2f_CoreScript_Wh ${ae2f_ProjRoot}/${ae2f_submod}/ae2f/CoreScript)

# Execute git to fetch CoreScript when repository does not exist
if(NOT EXISTS ${ae2f_CoreScript_Wh})
	execute_process(
		COMMAND git clone https://codeberg.org/ae2f/CoreScript
		${ae2f_CoreScript_Wh}
		)
endif()

# add subdirectory
if(NOT TARGET ae2f::CoreScript)
	add_subdirectory(${ae2f_CoreScript_Wh} ${ae2f_submod}/ae2f/CoreScript)
endif()

if(NOT TARGET ae3f::easyredir)
	add_executable(easyredir ./posix.c ./win.c ./main.c ./entry.h)
	add_executable(ae3f::easyredir ALIAS	easyredir)

	if(NOT EXISTS ${ae2f_submod}/ae2f/Core)
		execute_process(
			COMMAND git clone https://codeberg.org/ae2f/Core
			${ae2f_submod}/ae2f/Core
			)
	endif()

	install(
		TARGETS easyredir
		EXPORT	easyredirTargets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)

	ae2f_CoreMkComponent(easyredir ae3f cfg.cmake.in)
	ae2f_UltraErrWERROR(easyredir PRIVATE)
	target_include_directories(easyredir PRIVATE 
		${ae2f_submod}/ae2f/Core/inc
		${ae2f_submod}/ae2f/Sys/inc
		)
endif()
