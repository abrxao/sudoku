cmake_minimum_required(VERSION 3.16)
project(SudokuAssistant VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets LinguistTools)

set(RESOURCES src/assets/assets.qrc)

set(SOURCES
    src/main.cpp
    src/model/SudokuModel.cpp
    src/view/MainWindow.cpp
    src/presenter/SudokuPresenter.cpp
    src/view/SudokuCellDelegate.cpp
)

set(TS_FILES src/translations/sudoku_fr.ts)

qt_add_translations(SudokuAssistant
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/i18n"
)

add_executable(SudokuAssistant ${SOURCES} ${RESOURCES})

target_link_libraries(SudokuAssistant PRIVATE Qt6::Widgets)

enable_testing()
add_subdirectory(tests)