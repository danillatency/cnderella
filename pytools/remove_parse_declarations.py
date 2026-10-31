source = open("../src/semantical_analyzer/AASTreeVertices.h", "r+", encoding="utf-8")
source_lines = source.readlines()
remove_from_source = list()
for i, line in enumerate(source_lines):
    if "* parse" in line:
        remove_from_source.extend([i, i - 1])
source.seek(0)
source.write(''.join(map(lambda el: el[1],
                         filter(lambda elem: elem[0] not in remove_from_source,
                                enumerate(source_lines)))))
source.truncate()
