cmake_minimum_required(VERSION 3.16)

project(PipeAnalyzer
    VERSION 0.1
    LANGUAGES CXX
    DESCRIPTION "Pipe Analysis Tool"
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts Network PrintSupport LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts Network PrintSupport LinguistTools)

# 设置 CoolProp 路径（根据您的实际路径修改）
set(COOLPROP_DIR "${CMAKE_SOURCE_DIR}/thirdparty/coolprop" CACHE PATH "CoolProp 安装目录")

# 查找 CoolProp 头文件和库文件
find_path(COOLPROP_INCLUDE_DIR
    NAMES CoolPropLib.h
    PATHS
        ${COOLPROP_DIR}/include
    NO_DEFAULT_PATH              # 不搜索系统默认路径，只按指定路径查找（避免干扰）
    REQUIRED
)

# 在 Windows 上查找 .lib 文件，在其他平台查找 .a 或 .so 文件
if(WIN32)
    find_library(COOLPROP_LIBRARY
        NAMES CoolProp
        PATHS
            ${COOLPROP_DIR}/lib
        NO_DEFAULT_PATH
        REQUIRED
    )
else()
    find_library(COOLPROP_LIBRARY
        NAMES coolprop
        PATHS
            ${COOLPROP_DIR}/lib
            ${COOLPROP_DIR}
        REQUIRED
    )
endif()

if(COOLPROP_INCLUDE_DIR AND COOLPROP_LIBRARY)
    set(COOLPROP_FOUND TRUE)
    message(STATUS "Found CoolProp include dir: ${COOLPROP_INCLUDE_DIR}")
    message(STATUS "Found CoolProp library: ${COOLPROP_LIBRARY}")
else()
    set(COOLPROP_FOUND FALSE)
    message(FATAL_ERROR "CoolProp not found. Please check the path: ${COOLPROP_DIR}")
endif()

set(TS_FILES PipeAnalyzer_zh_CN.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PipeAnalyzer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/MaterialManager.h
        src/FluidAnalyzer.h
        src/ActivationManager.h
        src/MaterialManager.cpp
        src/FluidAnalyzer.cpp
        src/ActivationManager.cpp
        src/ReportGenerator.h
        src/ReportGenerator.cpp
        dialog/MaterialDialog.h
        dialog/MaterialDialog.cpp
        dialog/ActivationDialog.h
        dialog/ActivationDialog.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PipeAnalyzer APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(PipeAnalyzer SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PipeAnalyzer
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 关键修改：添加包含目录
target_include_directories(PipeAnalyzer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog
)

# 添加 CoolProp 包含目录
if(COOLPROP_FOUND)
    target_include_directories(PipeAnalyzer PRIVATE ${COOLPROP_INCLUDE_DIR})
    message(STATUS "Added CoolProp include directory: ${COOLPROP_INCLUDE_DIR}")
endif()

target_link_libraries(PipeAnalyzer PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

# 添加 CoolProp 库链接
if(COOLPROP_FOUND)
    target_link_libraries(PipeAnalyzer PRIVATE ${COOLPROP_LIBRARY})
    message(STATUS "Linked CoolProp library: ${COOLPROP_LIBRARY}")

    # 复制 DLL 到输出目录（Windows）
    if(WIN32)
        # 查找 DLL 文件
        find_file(COOLPROP_DLL
            NAMES CoolProp.dll
            PATHS
                ${COOLPROP_DIR}/bin
                ${COOLPROP_DIR}/lib
                ${COOLPROP_DIR}
            REQUIRED
        )

        if(COOLPROP_DLL)
            add_custom_command(TARGET PipeAnalyzer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${COOLPROP_DLL}"
                $<TARGET_FILE_DIR:PipeAnalyzer>
                COMMENT "Copying CoolProp.dll to output directory"
            )
            message(STATUS "Found CoolProp DLL: ${COOLPROP_DLL}")
        else()
            message(WARNING "CoolProp.dll not found, please manually copy it to output directory")
        endif()
    endif()
endif()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PipeAnalyzer)
endif()
set_target_properties(PipeAnalyzer PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PipeAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PipeAnalyzer)
endif()
