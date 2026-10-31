set(CPP_PARTS
    "lexical_analyzer/Tokens.cpp"
    "lexical_analyzer/LexicalAnalyzer.cpp"

    "syntactical_analyzer/TreeGround.cpp"
    "syntactical_analyzer/TreeVerticesProductions.cpp"
    "syntactical_analyzer/TreeVerticesConstructors.cpp"
    "syntactical_analyzer/TreeVerticesVisualization.cpp")

add_executable(cnderella Main.cpp  ${CPP_PARTS})

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolongPrograms" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	