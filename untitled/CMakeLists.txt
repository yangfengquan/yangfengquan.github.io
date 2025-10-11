cmake_minimum_required(VERSION 3.16)
project(CoolPropQtDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 启用Qt自动处理（保持默认）
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 2. 查找Qt6组件
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 3. CoolProp路径配置（与Visual Studio测试项目一致）
set(COOLPROP_INCLUDE_DIR "F:/Tools/untitled/thirdparty/coolprop/include")
set(COOLPROP_LIBRARY_DIR "F:/Tools/untitled/thirdparty/coolprop/lib")

# 4. 验证文件存在（确保路径正确）
if(NOT EXISTS "${COOLPROP_INCLUDE_DIR}/CoolPropLib.h")
    message(FATAL_ERROR "CoolProp头文件缺失: ${COOLPROP_INCLUDE_DIR}/CoolPropLib.h")
endif()
if(NOT EXISTS "${COOLPROP_LIBRARY_DIR}/CoolProp.lib")
    message(FATAL_ERROR "CoolProp导入库缺失: ${COOLPROP_LIBRARY_DIR}/CoolProp.lib")
endif()
if(NOT EXISTS "${COOLPROP_LIBRARY_DIR}/CoolProp.dll")
    message(FATAL_ERROR "CoolProp DLL缺失: ${COOLPROP_LIBRARY_DIR}/CoolProp.dll")
endif()

# 5. 源文件列表（无.ui文件，纯代码界面）
set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

# 6. 创建可执行文件（关键：指定Visual Studio运行时库，与CoolProp兼容）
add_executable(CoolPropQtDemo ${SOURCES})

# 7. 对齐Visual Studio运行时库（必须与CoolProp编译时一致！）
# 若CoolProp用MT编译，此处设MT；若用MD，设MD（参考Visual Studio测试项目的配置）
set_property(TARGET CoolPropQtDemo PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 8. 链接Qt库
target_link_libraries(CoolPropQtDemo PRIVATE Qt6::Widgets)

# 9. CoolProp配置（完全对齐Visual Studio测试代码的声明逻辑）
target_include_directories(CoolPropQtDemo PRIVATE ${COOLPROP_INCLUDE_DIR})
target_link_directories(CoolPropQtDemo PRIVATE ${COOLPROP_LIBRARY_DIR})
# 直接链接CoolProp.lib（与Visual Studio测试项目一致，不额外添加__declspec(dllimport)）
target_link_libraries(CoolPropQtDemo PRIVATE CoolProp.lib)

# 10. 复制DLL到输出目录（确保运行时能找到）
add_custom_command(TARGET CoolPropQtDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${COOLPROP_LIBRARY_DIR}/CoolProp.dll"
    $<TARGET_FILE_DIR:CoolPropQtDemo>
    COMMENT "复制CoolProp.dll到输出目录"
)
