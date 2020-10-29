#
set(BOOST_ROOT "$ENV{VCPKG_ROOT}")
find_package(Boost REQUIRED)

#
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Xml OpenGL Concurrent REQUIRED)

message(STATUS "Boost_INCLUDE_DIRS                =${Boost_INCLUDE_DIRS}")

function(add_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} Qt5::Core Qt5::Widgets ${Boost_LIBRARIES})

    if(MSVC)
        target_compile_options(${exe_name} PUBLIC /source-charset:utf-8)
    else()
        target_compile_options(${exe_name} PUBLIC -municode)
    endif()

    set_target_properties(${exe_name} PROPERTIES OUTPUT_NAME "${exe_name}$<$<CONFIG:Debug>:_d>")

    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")

endfunction()

add_exe(test "test.cpp")

add_exe(test_idw "test_idw.cpp")

add_exe(test_qstring "test_qstring.cpp" "dlg_qstring.h" "dlg_qstring.cpp" "dlg_qstring.ui")
