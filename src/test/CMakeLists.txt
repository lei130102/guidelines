#
set(BOOST_ROOT "$ENV{VCPKG_ROOT}")
find_package(Boost COMPONENTS locale REQUIRED)

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

function(add_dbghelp_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} Qt5::Core Qt5::Widgets ${Boost_LIBRARIES} dbghelp)

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

add_exe(test_qdialog_derived_class "test_qdialog_derived_class.cpp" "dlg_qdialog_derived_class.h" "dlg_qdialog_derived_class.cpp" "dlg_qdialog_derived_class.ui")

add_exe(test_qdialog
    "test_qdialog.cpp"
    "dlg_qdialog.h"
    "dlg_qdialog.cpp"
    "dlg_qdialog.ui"
    "dlg_value.h"
    "dlg_value.cpp"
    "dlg_value.ui"
    )

add_exe(test_qlineedit_qtextedit_qplaintextedit
    "test_qlineedit_qtextedit_qplaintextedit.cpp"
    "dlg_qlineedit_qtextedit_qplaintextedit.h"
    "dlg_qlineedit_qtextedit_qplaintextedit.cpp"
    "dlg_qlineedit_qtextedit_qplaintextedit.ui"
    "focusinselectlineedit.h"
    "focusinselectlineedit.cpp"
    )

add_exe(test_iterator
    "test_iterator.cpp"
    "dlg_iterator.h"
    "dlg_iterator.cpp"
    "dlg_iterator.ui"
    "numeric_range.h"
    )





add_dbghelp_exe(test_dbghelp "test_dbghelp.cpp" "dlg_dbghelp.h" "dlg_dbghelp.cpp" "dlg_dbghelp.ui")
