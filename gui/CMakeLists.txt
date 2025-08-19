cmake_minimum_required(VERSION 3.16)
project(FileOrganizer_GUI LANGUAGES CXX)

# ---------- C++ & Qt ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)


set(HAVE_YAML_CONFIG OFF)
find_package(yaml-cpp CONFIG QUIET)
if(yaml-cpp_FOUND)
  set(HAVE_YAML_CONFIG ON)
else()
  find_package(yaml-cpp REQUIRED) 
endif()

set(APP_NAME organizer_gui)

# ---------- 目标 & 源 ----------
qt_add_executable(${APP_NAME} MACOSX_BUNDLE
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/organizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc
)

# ---------- 资源：config.yaml 打进 Resources ----------
set(APP_CONF ${CMAKE_CURRENT_SOURCE_DIR}/config.yaml)
set_source_files_properties(${APP_CONF} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
target_sources(${APP_NAME} PRIVATE ${APP_CONF})

# ---------- 图标 (.icns) ----------
set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/icons/icon.icns)
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
target_sources(${APP_NAME} PRIVATE ${APP_ICON})

# ---------- macOS bundle 元数据 ----------
set_target_properties(${APP_NAME} PROPERTIES
  MACOSX_BUNDLE_ICON_FILE            "icon.icns"
  MACOSX_BUNDLE_BUNDLE_NAME          "File Organizer Assistant"
  MACOSX_BUNDLE_GUI_IDENTIFIER       "com.cmt.fileorganizer"
  MACOSX_BUNDLE_INFO_STRING          "File Organizer Assistant"
  MACOSX_BUNDLE_BUNDLE_VERSION       "1.0.0"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
  INSTALL_RPATH                      "@executable_path/../Frameworks"
  BUILD_WITH_INSTALL_RPATH           TRUE
)

# ---------- 链接 ----------
if(HAVE_YAML_CONFIG)
  # CONFIG 模式：官方 imported target
  target_link_libraries(${APP_NAME}
    PRIVATE Qt6::Widgets yaml-cpp::yaml-cpp Threads::Threads
  )
else()
  target_link_libraries(${APP_NAME}
    PRIVATE Qt6::Widgets yaml-cpp Threads::Threads
  )
endif()


include(GNUInstallDirs)
install(TARGETS ${APP_NAME}
  BUNDLE  DESTINATION .
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})  # 非 macOS 备用


