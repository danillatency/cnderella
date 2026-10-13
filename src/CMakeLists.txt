set(CPP_PARTS
    "classes/Tokens.cpp"
    "classes/LexicalAnalyzer.cpp"

    "classes/TreeGround.cpp")

add_executable(cnderella Main.cpp  ${CPP_PARTS})

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolongPrograms" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	