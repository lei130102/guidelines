#主要第三方库
#
set(BOOST_ROOT "$ENV{VCPKG_ROOT}")
find_package(Boost COMPONENTS system locale context filesystem REQUIRED)

message(STATUS "Boost_INCLUDE_DIRS                           =${Boost_INCLUDE_DIRS}")
message(STATUS "Boost_LIBRARIES                              =${Boost_LIBRARIES}")
message(STATUS "Boost_CONTEXT_LIBRARY                        =${Boost_CONTEXT_LIBRARY}")
message(STATUS "Boost_LOCALE_LIBRARY                         =${Boost_LOCALE_LIBRARY}")

message(STATUS "Boost_THREAD_LIBRARY                         =${Boost_THREAD_LIBRARY}")
message(STATUS "Boost_LOG_SETUP_LIBRARY                      =${Boost_LOG_SETUP_LIBRARY}")
message(STATUS "Boost_LOG_LIBRARY                            =${Boost_LOG_LIBRARY}")
message(STATUS "Boost_PROGRAM_OPTIONS_LIBRARY                =${Boost_PROGRAM_OPTIONS_LIBRARY}")
message(STATUS "Boost_FILESYSTEM_LIBRARY                     =${Boost_FILESYSTEM_LIBRARY}")

#
find_package(Qt5 COMPONENTS Core Gui Widgets PrintSupport Xml OpenGL Concurrent Sql REQUIRED)

##其他第三方库
##CGAL
##CGAL_Qt5 is needed for the drawing and CGAL_Core is needed for this special Kernel.
##注意要将
##vcpkg/installed/x64-windows/include/CGAL/Qt/resources/
##中的文件拷贝到
##vcpkg/installed/x64-windows/include/CGAL/Qt/
##或者使用下面代码，否则找不到需要使用的.ui文件
#set(CMAKE_AUTOUIC_SEARCH_PATHS "${AUTOUIC_SEARCH_PATHS};$ENV{CGAL_QT_RESOURCES}")
#find_package(CGAL CONFIG REQUIRED COMPONENTS Qt5 Core)
#
#find_package(RapidJSON CONFIG REQUIRED)
#
#find_package(imgui CONFIG REQUIRED)
#
#find_package(sqlite3 REQUIRED)
#
#message(STATUS "CGAL_ROOT                         =${CGAL_ROOT}")
#message(STATUS "CGAL_CONFIG_DIR                   =${CGAL_CONFIG_DIR}")
#message(STATUS "CGAL_INCLUDE_DIRS                 =${CGAL_INCLUDE_DIRS}")
#message(STATUS "CGAL_GRAPHICSVIEW_PACKAGE_DIR     =${CGAL_GRAPHICSVIEW_PACKAGE_DIR}")
##无法输出AUTOUIC_SEARCH_PATHS，原因不明 message(STATUS "AUTOUIC_SEARCH_PATHS              =${AUTOUIC_SEARCH_PATHS}")

function(add_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} Qt5::Core Qt5::Widgets Qt5::Sql ${Boost_CONTEXT_LIBRARY} ${Boost_LOCALE_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

    if(MSVC)
        target_compile_options(${exe_name} PUBLIC /source-charset:utf-8)
    else()
        target_compile_options(${exe_name} PUBLIC -municode)
         target_link_options(${test_name} PUBLIC -municode)
    endif()

    set_target_properties(${exe_name} PROPERTIES OUTPUT_NAME "${exe_name}$<$<CONFIG:Debug>:_d>")

    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")

endfunction()

#function(add_cgal_exe exe_name)
#    add_executable(${exe_name} ${ARGN})
#
#    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})
#
#    target_compile_definitions(${exe_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS)
#    #注意CGAL依赖这些Qt5模块
#    target_link_libraries(${exe_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 
#
#    if(MSVC)
#        target_compile_options(${exe_name} PUBLIC /source-charset:utf-8)
#    else()
#        target_compile_options(${exe_name} PUBLIC -municode)
#         target_link_options(${test_name} PUBLIC -municode)
#    endif()
#
#    set_target_properties(${exe_name} PROPERTIES OUTPUT_NAME "${exe_name}$<$<CONFIG:Debug>:_d>")
#
#    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#
#endfunction()

function(add_rapidjson_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})

    target_link_libraries(${exe_name} PRIVATE Qt5::Core Qt5::Widgets ${Boost_LIBRARIES}) 

    if(MSVC)
        target_compile_options(${exe_name} PUBLIC /source-charset:utf-8)
    else()
        target_compile_options(${exe_name} PUBLIC -municode)
         target_link_options(${test_name} PUBLIC -municode)
    endif()

    set_target_properties(${exe_name} PROPERTIES OUTPUT_NAME "${exe_name}$<$<CONFIG:Debug>:_d>")

    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")

endfunction()

##用于T-S-D点聚图检验评估指数计算
#function(add_tsd_lib lib_name)
#    add_library(${lib_name} SHARED ${ARGN})
#
#    target_include_directories(${lib_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})
#
#    target_compile_definitions(${lib_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS TSDPointClusterTestEvaluationIndexCalculationExport)
#    #注意CGAL依赖这些Qt5模块
#    target_link_libraries(${lib_name} PRIVATE Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 
#
#    if(MSVC)
#        target_compile_options(${lib_name} PUBLIC /source-charset:utf-8)
#    else()
#        target_compile_options(${lib_name} PUBLIC -municode)
#         target_link_options(${test_name} PUBLIC -municode)
#    endif()
#
#    set_target_properties(${lib_name} PROPERTIES OUTPUT_NAME "${lib_name}$<$<CONFIG:Debug>:_d>")
#
#    set_target_properties(${lib_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${lib_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${lib_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${lib_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#    set_target_properties(${lib_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#endfunction()
#
#function(add_tsd_exe exe_name)
#    add_executable(${exe_name} ${ARGN})
#
#    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})
#
#    target_compile_definitions(${exe_name} PRIVATE CGAL_USE_BASIC_VIEWER QT_NO_KEYWORDS)
#    #注意CGAL依赖这些Qt5模块
#    target_link_libraries(${exe_name} PRIVATE TSDPointClusterTestEvaluationIndexCalculation Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Sql Qt5::OpenGL CGAL_Qt5_moc_and_resources CGAL::Qt5_moc_and_resources CGAL::CGAL CGAL::CGAL_Qt5 CGAL::CGAL_Core ${Boost_LIBRARIES}) 
#
#    if(MSVC)
#        target_compile_options(${exe_name} PUBLIC /source-charset:utf-8)
#    else()
#        target_compile_options(${exe_name} PUBLIC -municode)
#         target_link_options(${test_name} PUBLIC -municode)
#    endif()
#
#    set_target_properties(${exe_name} PROPERTIES OUTPUT_NAME "${exe_name}$<$<CONFIG:Debug>:_d>")
#
#    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${exe_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_RUNTIME_DIR}")
#    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${exe_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_LIBRARY_DIR}")
#    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#    set_target_properties(${exe_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/${BUILD_ARCHIVE_DIR}")
#endfunction()


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

function(add_imgui_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} ${Boost_LIBRARIES} imgui::imgui)

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

function(add_sqlite_exe exe_name)
    add_executable(${exe_name} ${ARGN})

    target_include_directories(${exe_name} PRIVATE ${Boost_INCLUDE_DIRS})

    target_link_libraries(${exe_name} Qt5::Core Qt5::Widgets Qt5::Sql SQLite3 ${Boost_CONTEXT_LIBRARY} ${Boost_LOCALE_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_LOG_SETUP_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

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
add_exe(test "test.h" "test.cpp")

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


add_exe(test_sql
    "test_qsql.cpp")

add_exe(test_template
    "test_template.h"
    "test_template.cpp")

add_exe(test_template_custom
    "test_template_custom.cpp")


#c++标准库
add_exe(test_iterator
    "test_iterator.cpp"
    "dlg_iterator.h"
    "dlg_iterator.cpp"
    "dlg_iterator.ui"
    "numeric_range.h"
    )


#boost
add_exe(test_intrusive_test
    "test_intrusive_test.cpp"
    "test_intrusive_test.h")



#windows测试工具相关
add_dbghelp_exe(test_dbghelp "test_dbghelp.cpp" "dlg_dbghelp.h" "dlg_dbghelp.cpp" "dlg_dbghelp.ui")


##cgal库
#add_cgal_exe(test_cgal_test
#    "test_cgal_test.cpp"
#    )
#
#add_cgal_exe(test_cgal_2dpolygons
#    "test_cgal_2dpolygons.cpp"
#    )
#
#add_cgal_exe(test_cgal_convex_hull_indices_2
#    "test_cgal_convex_hull_indices_2.cpp"
#    )

#rapidjson
add_rapidjson_exe(test_rapidjson_test
    "test_rapidjson_test.cpp"
    )

##T-S-D点聚图检验评估指数计算
#add_tsd_lib(TSDPointClusterTestEvaluationIndexCalculation
#    "TSDPointClusterTestEvaluationIndexCalculation.cpp"
#    "TSDPointClusterTestEvaluationIndexCalculation.h"
#    "TSDPointClusterTestEvaluationIndexCalculationExport.h"
#    )
#
#add_tsd_exe(TSDPointClusterTestEvaluationIndexCalculationTest
#    "TSDPointClusterTestEvaluationIndexCalculationTest.cpp"
#    )

add_exe(test_type_traits
    "test_type_traits.cpp"
    "test_type_traits.h"
    )

add_exe(test_encryption
    "test_encryption.cpp"
    "dlg_encryption.h"
    "dlg_encryption.cpp"
    "dlg_encryption.ui"
    )

add_exe(test_optional
    "test_optional.cpp"
    "test_optional.h"
    )

add_exe(test_propertytree
    "test_propertytree.cpp"
    "test_propertytree.h"
    )

add_exe(test_date_time
    "test_date_time.cpp"
    "test_date_time.h"
    )

#add_imgui_exe(test_imgui
#    "test_imgui.cpp"
#    "test_imgui.h")

add_exe(test_threadpool
    "test_threadpool.cpp"
    "test_threadpool.h"
    )

add_exe(test_process
    "test_process.cpp"
    "test_process.h")

add_exe(test_type_index
    "test_type_index.cpp"
    "test_type_index.h")

add_exe(test_beast_http
    "mime_type.h"
    "path_cat.h"
    "handle_request.h"
    "fail.h"
    "websocket_session.h"
    "http_session.h"
    "listener.h"
    "test_beast_http.cpp"
    "test_beast_http.h")

add_exe(test_cpu
    "test_cpu.h"
    "test_cpu.cpp")

add_sqlite_exe(test_sqlite
    "test_sqlite.h"
    "test_sqlite.cpp")

add_exe(test_string_wstring_u8string
    "test_string_wstring_u8string.h"
    "test_string_wstring_u8string.cpp")

add_exe(test_signal2
    "test_signals2.h"
    "test_signals2.cpp")

add_exe(test_reference_wrapper
    "test_reference_wrapper.h"
    "test_reference_wrapper.cpp")

add_exe(test_bind
    "test_bind.h"
    "test_bind.cpp")

add_exe(test_function
    "test_function.h"
    "test_function.cpp")

add_exe(test_name
    "test_name.h"
    "test_name.cpp")

add_exe(test_any
    "test_any.h"
    "test_any.cpp")

add_exe(test_leet_code_flatten_nested_list_iterator
    "test_lc_flatten_nested_list_iterator.h"
    "test_lc_flatten_nested_list_iterator.cpp")

add_exe(test_mp11
    "test_mp11.h"
    "test_mp11.cpp")

add_exe(test_design_pattern
#创建型模式(Creational Patterns)        用于构建对象，以便他们可以从实现系统中分离出来
#   单例模式(Singleton Pattern)
    #"design_pattern_singleton.cpp"
    #"design_pattern_singleton_thread_safe.cpp"
#   抽象工厂模式(Abstract Factory Pattern)
    #"design_pattern_abstract_factory.cpp"
#   建造者模式(Builder Pattern)
    #"design_pattern_builder.cpp"
#   工厂方法模式(Factory Method Pattern)
    #"design_pattern_factory_method.cpp"
#   原型模式(Prototype Pattern)
    #"design_pattern_prototype.cpp"
#结构型模式(Structural Patterns)        用于在许多不同的对象之间形成大型对象结构
#   适配器模式(Adapter Pattern)
    #"design_pattern_adapter.cpp"
    #"design_pattern_adapter_multiple_inheritance.cpp"
#   桥接模式(Bridge Pattern)
    #"design_pattern_bridge.cpp"
#   装饰者模式(Decorator Pattern)
    #"design_pattern_decorator.cpp"
#   组合模式(Composite Pattern)
    #"design_pattern_composite.cpp"
#   外观模式(Facade Pattern)
    #"design_pattern_facade.cpp"
#   享元模式(Flyweight Pattern)
    #比较复杂，以后补充
#   代理模式(Proxy Pattern)
    #"design_pattern_proxy.cpp"
#行为型模式(Behavioral Patterns)        用于管理对象之间的算法、关系和职责
#   模板方法模式(TemplateMethodPattern)
#   命令模式(Command Pattern)
#   迭代器模式(Iterator Pattern)
#   观察者模式(Observer Pattern)
#   中介者模式(Mediator Pattern)
#   备忘录模式(Memento Pattern)
#   解释器模式(Interpreter Pattern)
#   状态模式(State Pattern)
#   策略模式(Strategy Pattern)
#   职责链模式(Chain of Responsibility Pattern)
#   访问者模式(Visitor Pattern)
    )
