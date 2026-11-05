productions = open("../src/syntactical_analyzer/ASTreeVerticesProductions.cpp", mode="r+", encoding="utf-8")
production_lines = productions.readlines()
new_error_names = list()
for i, line in enumerate(production_lines):
    if "throw domain_error" in line:
        error_name = line[line.index('\"') + 1:line.index("\")")]
        new_error_name = ''.join(map(str.capitalize, error_name.split()))
        production_lines[i] = line.replace(f"domain_error(\"{error_name}\")", f"parser::{new_error_name}(errorSpan)")
        new_error_names.append(new_error_name)
productions.seek(0)
productions.write(''.join(production_lines))

errors = open("../src/errors/NonTerminalErrors.h", mode="r+", encoding="utf-8")
error_lines = errors.readlines()
for elem in new_error_names:
    lines = f"    class {elem} : public domain_error {{\n"
    lines += "    public:\n"
    lines += f"        {elem}(Span span);\n"
    lines += "    };\n\n\n"
    error_lines.insert(error_lines.index("}\n"), lines)
errors.seek(0)
errors.write(''.join(error_lines))
