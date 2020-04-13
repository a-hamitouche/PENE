cmake_minimum_required (VERSION 3.14)

project(counters_module)

add_executable(test_counters_executable "test_counters_executable.cpp")
set_property(TARGET test_counters_executable PROPERTY CXX_STANDARD 17)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_counters.py
  COMMAND ${CMAKE_COMMAND} 
          -D INFILE=${CMAKE_CURRENT_SOURCE_DIR}/test_counters.py.in
          -D OUTFILE=${CMAKE_CURRENT_BINARY_DIR}/test_counters.py
          -D COUNTERS_TESTS_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
          -D COUNTERS_TESTS_ECEXUTABLE=$<TARGET_FILE:test_counters_executable>
          -D COUNTERS_TESTS_PINTOOL=$<TARGET_FILE:pene>
          -D PIN_EXECUTABLE=$<TARGET_FILE:pin>
          -P ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
  DEPENDS
    ${CMAKE_GENERICS_PATH}/GenericConfigureFile.cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/test_counters.py.in
  COMMENT "Configuring test_counters.py"
  VERBATIM
)

set(TEST_SCRIPTS test_counters.py)
#set(REFERENCE_FILES test_counters_reference_inactive.txt test_counters_reference_active.txt)
set(TEST_LIST test_counters.flag_tests.test_active test_counters.flag_tests.test_inactive)



function(copy_file FILENAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
                ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME}
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}
        COMMENT "Copying ${FILENAME}"
        VERBATIM
    )
endfunction(copy_file)

foreach(reference_file ${REFERENCE_FILES})
    copy_file(${reference_file})
endforeach()

list(TRANSFORM REFERENCE_FILES PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
list(TRANSFORM TEST_SCRIPTS PREPEND ${CMAKE_CURRENT_BINARY_DIR}/)
add_custom_target(${PROJECT_NAME} ALL DEPENDS 
    ${TEST_SCRIPTS}
    ${REFERENCE_FILES}
)

find_package(Python REQUIRED Interpreter)
foreach(unittest ${TEST_LIST})
	add_test(NAME ${unittest}
			COMMAND ${Python_EXECUTABLE} -m unittest ${unittest})
endforeach()

	