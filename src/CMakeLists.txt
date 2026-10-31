set(CPP_PARTS
    "lexical_analyzer/Tokens.cpp"
    "lexical_analyzer/LexicalAnalyzer.cpp"

    "syntactical_analyzer/ASTreeGround.cpp"
    "syntactical_analyzer/ASTreeVerticesProductions.cpp"
    "syntactical_analyzer/ASTreeVerticesConstructors.cpp"
    "syntactical_analyzer/ASTreeVerticesVisualization.cpp")

add_executable(cnderella Main.cpp  ${CPP_PARTS})

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolongPrograms" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	