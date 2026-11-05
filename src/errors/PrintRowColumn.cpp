#include "PrintRowColumn.h"


using namespace std;


string rowColumn(Span& span) {
    return "\nrow: " + to_string(span.row)
        + "\ncolumn: " + to_string(span.beginPosition) + ".." + to_string(span.endPosition);
}
