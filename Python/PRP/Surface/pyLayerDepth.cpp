#include <PyPlasma.h>
#include <PRP/Surface/plLayer.h>
#include "pyLayer.h"
#include "PRP/pyCreatable.h"

extern "C" {

static PyObject* pyLayerDepth_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyLayerDepth* self = (pyLayerDepth*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new plLayerDepth();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pyLayerDepth_Convert(PyObject*, PyObject* args) {
    pyCreatable* cre;
    if (!PyArg_ParseTuple(args, "O", &cre)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a plCreatable");
        return NULL;
    }
    if (!pyCreatable_Check((PyObject*)cre)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a plCreatable");
        return NULL;
    }
    return pyLayerDepth_FromLayerDepth(plLayerDepth::Convert(cre->fThis));
}

static PyMethodDef pyLayerDepth_Methods[] = {
    { "Convert", (PyCFunction)pyLayerDepth_Convert, METH_VARARGS | METH_STATIC,
      "Convert a Creatable to a plLayerDepth" },
    { NULL, NULL, 0, NULL }
};

PyTypeObject pyLayerDepth_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.plLayerDepth",            /* tp_name */
    sizeof(pyLayerDepth),               /* tp_basicsize */
    0,                                  /* tp_itemsize */

    NULL,                               /* tp_dealloc */
    NULL,                               /* tp_print */
    NULL,                               /* tp_getattr */
    NULL,                               /* tp_setattr */
    NULL,                               /* tp_compare */
    NULL,                               /* tp_repr */
    NULL,                               /* tp_as_number */
    NULL,                               /* tp_as_sequence */
    NULL,                               /* tp_as_mapping */
    NULL,                               /* tp_hash */
    NULL,                               /* tp_call */
    NULL,                               /* tp_str */
    NULL,                               /* tp_getattro */
    NULL,                               /* tp_setattro */
    NULL,                               /* tp_as_buffer */

    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "plLayerDepth wrapper",             /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyLayerDepth_Methods,               /* tp_methods */
    NULL,                               /* tp_members */
    NULL,                               /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyLayerDepth_new,                   /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */
};

PyObject* Init_pyLayerDepth_Type() {
    pyLayerDepth_Type.tp_base = &pyLayer_Type;
    if (PyType_Ready(&pyLayerDepth_Type) < 0)
        return NULL;

    Py_INCREF(&pyLayerDepth_Type);
    return (PyObject*)&pyLayerDepth_Type;
}

int pyLayerDepth_Check(PyObject* obj) {
    if (obj->ob_type == &pyLayerDepth_Type
        || PyType_IsSubtype(obj->ob_type, &pyLayerDepth_Type))
        return 1;
    return 0;
}

PyObject* pyLayerDepth_FromLayerDepth(class plLayerDepth* layer) {
    if (layer == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pyLayerDepth* pylay = PyObject_New(pyLayerDepth, &pyLayerDepth_Type);
    pylay->fThis = layer;
    pylay->fPyOwned = false;
    return (PyObject*)pylay;
}

}
