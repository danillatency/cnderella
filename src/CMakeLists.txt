set(CPP_PARTS
    "lexical_analyzer/Tokens.cpp"
    "lexical_analyzer/LexicalAnalyzer.cpp"

    "syntactical_analyzer/SyntacticalAnalyzer.cpp"
    "syntactical_analyzer/ASTreeVerticesProductions.cpp"
    "syntactical_analyzer/ASTreeVerticesConstructors.cpp"
    "syntactical_analyzer/ASTreeVerticesVisualization.cpp"

    "semantical_analyzer/SemanticalAnalyzerMap.cpp"
    "semantical_analyzer/SemanticalAnalyzerRest.cpp"
    "semantical_analyzer/AASTreeVerticesCreation.cpp"
    "semantical_analyzer/AASTreeVerticesVisualization.cpp"
    "semantical_analyzer/AASTreeVerticesScoping.cpp"
    "semantical_analyzer/AASTreeVerticesValidation.cpp"

    "errors/TerminalErrors.cpp"
    "errors/NonTerminalErrors.cpp"
    "errors/PrintRowColumn.cpp"
)

add_executable(cnderella Main.cpp  ${CPP_PARTS})

install(TARGETS cnderella RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/oolong_programs" DESTINATION "${CMAKE_INSTALL_PREFIX}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/builtin_libraries" DESTINATION "${CMAKE_INSTALL_PREFIX}")
	