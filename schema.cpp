#include "Python.h"
#include "rapidjson/include/rapidjson/error/en.h"
#include "rapidjson/include/rapidjson/schema.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"


typedef struct {
    PyObject_HEAD
    rapidjson::SchemaDocument *document;
} SchemaObject;

static PyObject *
schema_validate(SchemaObject* self, PyObject *args)
{
    char* json;
    int accept;
    rapidjson::Document d;

	if (!PyArg_ParseTuple(args, "s", &json)) return NULL;

    Py_BEGIN_ALLOW_THREADS;
    accept = !d.Parse(json).HasParseError();
    Py_END_ALLOW_THREADS;

    if (!accept) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }

    rapidjson::SchemaValidator validator(*self->document);

    Py_BEGIN_ALLOW_THREADS;
    accept = d.Accept(validator);
    Py_END_ALLOW_THREADS;

    if (!accept) {
        rapidjson::StringBuffer sb;
        rapidjson::StringBuffer sb2;
        Py_BEGIN_ALLOW_THREADS;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb2);
        Py_END_ALLOW_THREADS;
        PyErr_SetObject(PyExc_ValueError,
                Py_BuildValue("sss", 
                    validator.GetInvalidSchemaKeyword(),
                    sb.GetString(),
                    sb2.GetString()));
        sb.Clear();
        return NULL;
    }

    return Py_BuildValue("");
}

static PyMethodDef Schema_methods[] = {
    {"validate", (PyCFunction)schema_validate, METH_VARARGS,
     "Validate a document"},
    {NULL}
};

static PyTypeObject SchemaType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "rapidjson_schema.Schema",     /* tp_name */
    sizeof(SchemaObject),          /* tp_basicsize */
    0,                             /* tp_itemsize */
    0,                             /* tp_dealloc */
    0,                             /* tp_print */
    0,                             /* tp_getattr */
    0,                             /* tp_setattr */
    0,                             /* tp_reserved */
    0,                             /* tp_repr */
    0,                             /* tp_as_number */
    0,                             /* tp_as_sequence */
    0,                             /* tp_as_mapping */
    0,                             /* tp_hash  */
    0,                             /* tp_call */
    0,                             /* tp_str */
    0,                             /* tp_getattro */
    0,                             /* tp_setattro */
    0,                             /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,            /* tp_flags */
    "Schema objects",              /* tp_doc */
    0,                             /* tp_traverse */
    0,                             /* tp_clear */
    0,                             /* tp_richcompare */
    0,                             /* tp_weaklistoffset */
    0,                             /* tp_iter */
    0,                             /* tp_iternext */
    Schema_methods,                /* tp_methods */
};

static PyObject*
schema_loads(PyObject *self, PyObject *args)
{
    char* json;
    rapidjson::Document d;

	if (!PyArg_ParseTuple(args, "s", &json)) return NULL;

    if (d.Parse(json).HasParseError()) {
        PyErr_SetString(PyExc_ValueError, "Invalid JSON");
        return NULL;
    }
    
    SchemaObject *obj = PyObject_New(SchemaObject, &SchemaType); 
    obj->document = new rapidjson::SchemaDocument(d);
    return (PyObject*) obj;
}

static PyMethodDef
schema_functions[] = {
    {"loads", (PyCFunction) schema_loads, METH_VARARGS, "Load a schema"},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef schemamodule = {
    PyModuleDef_HEAD_INIT,
    "rapidjson_schema",
    "rapidjson schema validation",
    -1,
    schema_functions
};

PyMODINIT_FUNC
PyInit_rapidjson_schema(void)
{
    PyObject* m;

    if (PyType_Ready(&SchemaType) < 0)
        return NULL;

    m = PyModule_Create(&schemamodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&SchemaType);
    return m;
}

