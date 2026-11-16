source = open("../src/semantical_analyzer/AASTreeVerticesValidation.cpp", mode="r+", encoding="utf-8")
lines = source.readlines()
removed_indexes = set()
blocklist = ["this->mark", "this->isTypeChecked"]
for i, line in enumerate(lines):
    if any(elem for elem in blocklist if elem in line):
        removed_indexes.add(i)
source.seek(0)
source.write(''.join(map(lambda el: el[1], filter(lambda elem: elem[0] not in removed_indexes,
                                                  enumerate(lines)))))
source.truncate()
