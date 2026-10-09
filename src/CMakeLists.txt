add_executable(cnderella Main.cpp classes/Tokens.cpp)

set_target_properties(cnderella PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)