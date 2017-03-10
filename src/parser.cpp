#include <math.h>
#include "Python.h"
#include "convert.hpp"
#include "cpython-ast.h"
#include "ast.hpp"

static int RunModule(wchar_t *modname, int set_argv0)
{
    PyObject *module, *runpy, *runmodule, *runargs, *result;
    runpy = PyImport_ImportModule("runpy");
    if (runpy == NULL) {
        fprintf(stderr, "Could not import runpy module\n");
        PyErr_Print();
        return -1;
    }
    runmodule = PyObject_GetAttrString(runpy, "_run_module_as_main");
    if (runmodule == NULL) {
        fprintf(stderr, "Could not access runpy._run_module_as_main\n");
        PyErr_Print();
        Py_DECREF(runpy);
        return -1;
    }
    module = PyUnicode_FromWideChar(modname, wcslen(modname));
    if (module == NULL) {
        fprintf(stderr, "Could not convert module name to unicode\n");
        PyErr_Print();
        Py_DECREF(runpy);
        Py_DECREF(runmodule);
        return -1;
    }
    runargs = Py_BuildValue("(Oi)", module, set_argv0);
    if (runargs == NULL) {
        fprintf(stderr,
                "Could not create arguments for runpy._run_module_as_main\n");
        PyErr_Print();
        Py_DECREF(runpy);
        Py_DECREF(runmodule);
        Py_DECREF(module);
        return -1;
    }
    result = PyObject_Call(runmodule, runargs, NULL);
    if (result == NULL) {
        PyErr_Print();
    }
    Py_DECREF(runpy);
    Py_DECREF(runmodule);
    Py_DECREF(module);
    Py_DECREF(runargs);
    if (result == NULL) {
        return -1;
    }
    Py_DECREF(result);
    return 0;
}

static int RunMainFromImporter(wchar_t *filename)
{
    PyObject *argv0 = NULL, *importer, *sys_path, *sys_path0;
    int sts;

    argv0 = PyUnicode_FromWideChar(filename, wcslen(filename));
    if (argv0 == NULL)
        goto error;

    importer = PyImport_GetImporter(argv0);
    if (importer == NULL)
        goto error;

    if (importer == Py_None) {
        Py_DECREF(argv0);
        Py_DECREF(importer);
        return -1;
    }
    Py_DECREF(importer);

    /* argv0 is usable as an import source, so put it in sys.path[0]
       and import __main__ */
    sys_path = PySys_GetObject("path");
    if (sys_path == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "unable to get sys.path");
        goto error;
    }
    sys_path0 = PyList_GetItem(sys_path, 0);
    sts = 0;
    if (!sys_path0) {
        PyErr_Clear();
        sts = PyList_Append(sys_path, argv0);
    } else if (PyObject_IsTrue(sys_path0)) {
        sts = PyList_Insert(sys_path, 0, argv0);
    } else {
        sts = PyList_SetItem(sys_path, 0, argv0);
    }
    if (sts) {
        argv0 = NULL;
        goto error;
    }
    Py_INCREF(argv0);

    sts = RunModule(L"__main__", 0);
    return sts != 0;

    error:
    Py_XDECREF(argv0);
    PyErr_Print();
    return 1;
}

mod_ty Parse(wchar_t *fn) {
    mod_ty mod;
    FILE *fp = _Py_wfopen(fn, L"r");
    PyArena *arena = NULL;
    arena = PyArena_New();
    PyCompilerFlags cf;
    cf.cf_flags = 0;

    Py_Initialize();
    RunMainFromImporter(fn);
    char *filename = PyBytes_AsString(PyUnicode_EncodeFSDefault(PyUnicode_FromWideChar(fn, wcslen(fn))));
    PyObject *filename_obj = PyUnicode_DecodeFSDefault(filename);
    mod = PyParser_ASTFromFileObject(fp, filename_obj, NULL,
        Py_file_input, 0, 0, &cf, NULL, arena);

    fclose(fp);
    return mod;
}

int main(int argc, char **argv) {
    const char* argv1 = "source.py";
    int n = strlen(argv1);
    wchar_t *ws = new wchar_t[n+1];
    swprintf(ws, n+1, L"%hs", argv1);
    PPAP::AST *root = PPAP::CpythonToPPAP(Parse(ws), argv1);
    delete [] ws;
    return 0;
}
