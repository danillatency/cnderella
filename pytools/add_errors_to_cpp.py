prepared_lines = set()
for elem in open("../src/errors/NonTerminalErrors.h", mode='r', encoding="utf-8"):
    if "class" in elem:
        class_name = elem.split()[1]
        prepared_lines.add(f"parser::{class_name}::{class_name}(Span span) : domain_error(\"\"s + rowColumn(span)) {{}}\n\n\n")
open("../src/errors/NonTerminalErrors.cpp", mode='a', encoding="utf-8").write(''.join(prepared_lines))
