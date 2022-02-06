
include(ExternalProject)

# Helpers
macro (set_external_project_args name path)
    set(${name}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${path})
    set(${name}_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${path})

    set(${name}_CMAKE_ARGS  "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}"
                            "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
                            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                            # These are only useful if you're cross-compiling.
                            # They, however, will not hurt regardless.
                            # These will cause crul cmake confige fail in first time
                            # "-DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}"
                            # "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
                            # "-DCMAKE_AR=${CMAKE_AR}"
                            # "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                            # "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                            # "-DCMAKE_RC_COMPILER=${CMAKE_RC_COMPILER}"
                            # "-DCMAKE_COMPILER_PREFIX=${CMAKE_COMPILER_PREFIX}"
                            # "-DCMAKE_FIND_ROOT_PATH=${CMAKE_FIND_ROOT_PATH}"
                            )
endmacro(set_external_project_args)

#
#   Add external project.
#
#   \param name             Name of external project
#   \param path             Path to source directory
#   \param external         Name of the external target
#
macro(add_external_project name path)
    set_external_project_args(${name} ${path})
    set (${name}_CMAKE_EXTRA_ARGS ${ARGN})
    list (LENGTH ${name}_CMAKE_EXTRA_ARGS ${name}_CMAKE_EXTRA_ARGS_LEN)

    if (${name}_CMAKE_EXTRA_ARGS_LEN GREATER 0)
        foreach (arg ${${name}_CMAKE_EXTRA_ARGS})
            set(${name}_CMAKE_ARGS ${${name}_CMAKE_ARGS} "-D${arg}")
        endforeach()
    endif()

    message ("EXTERNAL_PROJECT_NAME=${name} CMAKE_ARGS=${${name}_CMAKE_ARGS}")

    ExternalProject_Add(${name}
        SOURCE_DIR "${${name}_SOURCE_DIR}"
        BINARY_DIR "${${name}_BINARY_DIR}"
        CMAKE_ARGS "${${name}_CMAKE_ARGS}"
       INSTALL_COMMAND ""
    )

endmacro(add_external_project)

#
#   Add external target to external project.
#
#   \param name             Name of external project
#   \param includedir       Path to include directory
#   \param libpath          Path to library
#   \param build_type       Build type {STATIC, SHARED}
#   \param external         Name of the external target
#
macro(add_external_target_by_lib name includedir libpath build_type external)
    # Create external library
    add_library(${name} ${build_type} IMPORTED GLOBAL)
    set(${name}_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/${libpath}")

    # Find paths and set dependencies
    add_dependencies(${name} ${external})
    set(${name}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${includedir})

    # Set interface properties
    set_target_properties(${name} PROPERTIES IMPORTED_LOCATION ${${name}_LIBRARY})
    set_target_properties(${name} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${${name}_INCLUDE_DIR})
endmacro(add_external_target_by_lib)

#
#   Add external target to external project.
#
#   \param name             Name of external project
#   \param includedir       Path to include directory
#   \param libdir           Path to library directory
#   \param build_type       Build type {STATIC, SHARED}
#   \param external         Name of the external target
#
macro(add_external_target name includedir libdir build_type external)
    add_external_target_by_lib(${name} ${includedir} ${libdir}/${CMAKE_CFG_INTDIR}/${CMAKE_${build_type}_LIBRARY_PREFIX}${name}${CMAKE_${build_type}_LIBRARY_SUFFIX} ${build_type} ${external})
endmacro(add_external_target)


