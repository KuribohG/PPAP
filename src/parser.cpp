#include "Python.h"
#include "node.h"

int main() {
    const char *fn = "source.py";
    FILE *fp = fopen(fn, "r");
    //_node *root = PyParser_SimpleParseFile(fp, fn, Py_file_input);
    printf("%d\n", Py_file_input);
    _node *root = PyParser_SimpleParseString("print(1 + 2)", Py_file_input);
    printf("%d\n", LINENO(root));
    return 0;
}
