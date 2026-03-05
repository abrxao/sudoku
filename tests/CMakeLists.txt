find_package(Qt6 REQUIRED COMPONENTS Test Core)

add_executable(TestSudokuModel 
    model_tests/tst_SudokuModel.cpp 
    ../src/model/SudokuModel.cpp
)

target_link_libraries(TestSudokuModel PRIVATE Qt6::Test Qt6::Core)

add_test(NAME TestSudokuModel COMMAND TestSudokuModel)