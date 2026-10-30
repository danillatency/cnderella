source = open("../src/syntactical_analyzer/TreeVertices.cpp", "r+", encoding="utf-8")
source_lines = source.readlines()
remove_from_source = list()
with open("../src/syntactical_analyzer/TreeVerticesVisualization.cpp", 'a', encoding="utf-8") as destination:
    definition = str()
    further = False
    for i, elem in enumerate(source_lines):
        if further or "::selfPrint" in elem:
            definition += elem
            remove_from_source.append(i)
            further = True
        if elem[0] == '}' and definition:
            destination.write(definition + "\n\n")
            # print(definition)
            definition = str()
            further = False
source_lines = filter(lambda el: el[0] not in remove_from_source, enumerate(source_lines))
source_lines = map(lambda el: el[1], source_lines)
source.seek(0)
source.write(''.join(source_lines))
# print(''.join(source_lines))
source.truncate()
