add_executable(cnderella Main.cpp classes/Tokens.cpp)

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolongPrograms" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	