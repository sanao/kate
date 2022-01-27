add_executable(dapjsontest
    dapjsontest.cpp
)
add_test(dapjsontest dapjsontest)

find_package(Qt5Test ${QT_MIN_VERSION} QUIET REQUIRED)

target_link_libraries(dapjsontest
  PRIVATE
    KF5::I18n
    KF5::TextEditor
    KF5::SyntaxHighlighting
    Qt5::Test
)

set_property(
  TARGET
    dapjsontest
  PROPERTY CXX_STANDARD 20
)

