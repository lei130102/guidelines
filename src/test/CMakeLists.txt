#主要第三方库
#
set(BOOST_ROOT "$ENV{VCPKG_ROOT}")
find_package(Boost COMPONENTS locale REQUIRED)

#
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Xml OpenGL Concurrent Sql REQUIRED)

#其他第三方库
#CGAL
#CGAL_Qt5 is needed for the drawing and CGAL_Core is needed for this special Kernel.
#注意要将
#vcpkg/installed/x64-windows/include/CGAL/Qt/resources/
#中的文件拷贝到
#vcpkg/installed/x64-windows/include/CGAL/Qt/
#或者使用下面代码，否则找不到需要使用的.ui文件
set(CMAKE_AUTOUIC_SEARCH_PATHS "${AUTOUIC_SEARCH_PATHS};$ENV{CGAL_QT_RESOURCES}")
find_package(CGAL CONFIG REQUIRED COMPONENTS Qt5 Core)

find_package(RapidJSON CONFIG REQUIRED)

message(STATUS "Boost_INCLUDE_DIRS                =${Boost_INCLUDE_DIRS}")
message(STATUS "CGAL_ROOT                         =${CGAL_ROOT}")
message(STATUS "CGAL_CONFIG_DIR                   =${CGAL_CONFIG_DIR}")
message(STATUS "CGAL_INCLUDE_DIRS                 =${CGAL_INCLUDE_DIRS}")
message(STATUS "CGAL_GRAPHICSVIEW_PACKAGE_DIR     =${CGAL_GRAPHICSVIEW_PACKAGE_DIR}")
#无法输出AUTOUIC_SEARCH_PATHS，原因不明 message(STATUS "AUTOUIC_SEARCH_PATHS              =${AUTOUIC_SEARCH_PATHS}")

function(add_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} Qt5::Core Qt5::Widgets Qt5::Sql ${Boost_LIBRARIES})

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

function(add_cgal_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_compile_definitions(${exe_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS)
    #注意CGAL依赖这些Qt5模块
    target_link_libraries(${exe_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 

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

function(add_rapidjson_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})

    target_link_libraries(${exe_name} PRIVATE Qt5::Core Qt5::Widgets ${Boost_LIBRARIES}) 

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

#用于T-S-D点聚图检验评估指数计算
function(add_tsd_lib lib_name)
    add_library(${lib_name} SHARED ${ARGN})

    target_include_directories(${lib_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})

    target_compile_definitions(${lib_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS TSDPointClusterTestEvaluationIndexCalculationExport)
    #注意CGAL依赖这些Qt5模块
    target_link_libraries(${lib_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 

    if(MSVC)
        target_compile_options(${lib_name} PUBLIC /source-charset:utf-8)
    else()
        target_compile_options(${lib_name} PUBLIC -municode)
    endif()

    set_target_properties(${lib_name} PROPERTIES OUTPUT_NAME "${lib_name}$<$<CONFIG:Debug>:_d>")

    set_target_properties(${lib_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${lib_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${lib_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
    set_target_properties(${lib_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
endfunction()

function(add_tsd_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})

    target_compile_definitions(${exe_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS)
    #注意CGAL依赖这些Qt5模块
    target_link_libraries(${exe_name} PRIVATE TSDPointClusterTestEvaluationIndexCalculation Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 

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

#临时
add_exe(test "test.cpp")

#算法
add_exe(test_idw "test_idw.cpp")
add_exe(test_gradient "test_gradient.cpp")


#qt
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

add_exe(test_qcombobox
    "test_qcombobox.cpp"
    "test_qcombobox.qrc"
    "dlg_qcombobox.h"
    "dlg_qcombobox.cpp"
    "dlg_qcombobox.ui"
    )

add_exe(test_qspinbox_qdoublespinbox
    "test_qspinbox_qdoublespinbox.cpp"
    "dlg_qspinbox_qdoublespinbox.h"
    "dlg_qspinbox_qdoublespinbox.cpp"
    "dlg_qspinbox_qdoublespinbox.ui"
    )

add_exe(test_qsqlquery
    "test_qsqlquery.cpp"
    "dlg_qsqlquery.cpp"
    "dlg_qsqlquery.h"
    "dlg_qsqlquery.ui"
    "sqlfield.h"
    "sqlvalue.h"
    "sqlvaluewrapper.h"
    "sqlqueryresult.h"
    "integer_sequence.hpp"
    "sqlquerycontainer.h"
    "sqlquerycontainerelement.h"
    )

add_exe(test_qgroupbox
    "test_qgroupbox.cpp"
    "dlg_qgroupbox.cpp"
    "dlg_qgroupbox.h"
    "dlg_qgroupbox.ui")

add_exe(test_hostwindowforwpf
    "test_hostwindowforwpf.cpp"
    "test_hostwindowforwpf.qrc"
    "hostwindow_manager.h"
    "hostwindow_manager.cpp"
    "widget_hostwindow.cpp"
    "widget_hostwindow.h"
    "widget_hostwindow.ui"
    )






#c++标准库
add_exe(test_iterator
    "test_iterator.cpp"
    "dlg_iterator.h"
    "dlg_iterator.cpp"
    "dlg_iterator.ui"
    "numeric_range.h"
    )



#windows测试工具相关
add_dbghelp_exe(test_dbghelp "test_dbghelp.cpp" "dlg_dbghelp.h" "dlg_dbghelp.cpp" "dlg_dbghelp.ui")


#cgal库
add_cgal_exe(test_cgal_test
    "test_cgal_test.cpp"
    )

add_cgal_exe(test_cgal_2dpolygons
    "test_cgal_2dpolygons.cpp"
    )

add_cgal_exe(test_cgal_convex_hull_indices_2
    "test_cgal_convex_hull_indices_2.cpp"
    )

#rapidjson
add_rapidjson_exe(test_rapidjson_test
    "test_rapidjson_test.cpp"
    )

#T-S-D点聚图检验评估指数计算
add_tsd_lib(TSDPointClusterTestEvaluationIndexCalculation
    "TSDPointClusterTestEvaluationIndexCalculation.cpp"
    "TSDPointClusterTestEvaluationIndexCalculation.h"
    "TSDPointClusterTestEvaluationIndexCalculationExport.h"
    )

add_tsd_exe(TSDPointClusterTestEvaluationIndexCalculationTest
    "TSDPointClusterTestEvaluationIndexCalculationTest.cpp"
    )
