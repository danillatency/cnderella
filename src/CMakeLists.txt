set(CPP_PARTS
    "lexical_analyzer/Tokens.cpp"
    "lexical_analyzer/LexicalAnalyzer.cpp"

    "syntactical_analyzer/TreeGround.cpp"
    "syntactical_analyzer/TreeVertices.cpp")

add_executable(cnderella Main.cpp  ${CPP_PARTS})

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolongPrograms" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	