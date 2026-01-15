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
	if(EASYREDIR_SHARED)
		set(EASYREDIR_PREFIX SHARED)
		set(EASYREDIR_SHARED 1)
	else()
		set(EASYREDIR_PREFIX STATIC)
		set(EASYREDIR_SHARED 0)
	endif()

	configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/lib/easyredir-link.h.in
		${CMAKE_CURRENT_SOURCE_DIR}/lib/easyredir-link.h
		)

	ae2f_CoreLibTentConfigCustom(
		libeasyredir ${EASYREDIR_PREFIX} lib ae3f cfg/lib.cmake.in
		lib/posix.c lib/win.c
		)

	add_executable(easyredir lib/win.c app/main.c lib/posix.c)
	add_executable(ae3f::easyredir ALIAS	easyredir)

	ae2f_CoreLibFetchX(ae2f Core main)

	install(
		TARGETS easyredir
		EXPORT	easyredirTargets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)

	ae2f_CoreMkComponent(easyredir ae3f cfg/app.cmake.in)
	ae2f_UltraErrWERROR(easyredir PRIVATE)

	target_link_libraries(easyredir PRIVATE ae2f::Core)
	target_link_libraries(ae3f-libeasyredir PUBLIC ae2f::Core)
	target_include_directories(easyredir PRIVATE lib)
endif()
