find_package(KF5SyntaxHighlighting ${KF5_DEP_VERSION} QUIET)
set_package_properties(KF5SyntaxHighlighting PROPERTIES PURPOSE "Required to build the dapclient addon")

if(NOT KF5SyntaxHighlighting_FOUND)
  return()
endif()

kate_add_plugin(dapclientplugin)
target_compile_definitions(dapclientplugin PRIVATE TRANSLATION_DOMAIN="dapclient")

target_link_libraries(
  dapclientplugin
  PRIVATE
    KF5::I18n
    KF5::TextEditor
    KF5::SyntaxHighlighting
)

set_property(
  TARGET
    dapclientplugin
  PROPERTY CXX_STANDARD 20
)

# ki18n_wrap_ui(dapclientplugin dapconfigwidget.ui)

target_sources(
  dapclientplugin
  PRIVATE
    dapclientplugin.cpp
)

target_include_directories(
    dapclientplugin
    PUBLIC
    ${CMAKE_SOURCE_DIR}/shared
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
