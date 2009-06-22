#include <PyPlasma.h>
#include <PRP/Message/proEventData.h>
#include "pyEventData.h"
#include "PRP/KeyedObject/pyKey.h"

extern "C" {

static PyObject* pySpawnedEventData_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pySpawnedEventData* self = (pySpawnedEventData*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new proSpawnedEventData();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pySpawnedEventData_Convert(PyObject*, PyObject* args) {
    pyEventData* evt;
    if (!PyArg_ParseTuple(args, "O", &evt)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a proEventData");
        return NULL;
    }
    if (!pyEventData_Check((PyObject*)evt)) {
        PyErr_SetString(PyExc_TypeError, "Convert expects a proEventData");
        return NULL;
    }
    return pySpawnedEventData_FromSpawnedEventData(proSpawnedEventData::Convert(evt->fThis));
}

static PyObject* pySpawnedEventData_getSpawner(pySpawnedEventData* self, void*) {
    return pyKey_FromKey(self->fThis->getSpawner());
}

static PyObject* pySpawnedEventData_getSpawnee(pySpawnedEventData* self, void*) {
    return pyKey_FromKey(self->fThis->getSpawnee());
}

static int pySpawnedEventData_setSpawner(pySpawnedEventData* self, PyObject* value, void*) {
    if (value == NULL || value == Py_None) {
        self->fThis->setSpawner(plKey());
        return 0;
    } else if (pyKey_Check(value)) {
        self->fThis->setSpawner(*((pyKey*)value)->fThis);
        return 0;
    } else {
        PyErr_SetString(PyExc_TypeError, "spawner should be a plKey");
        return -1;
    }
}

static int pySpawnedEventData_setSpawnee(pySpawnedEventData* self, PyObject* value, void*) {
    if (value == NULL || value == Py_None) {
        self->fThis->setSpawnee(plKey());
        return 0;
    } else if (pyKey_Check(value)) {
        self->fThis->setSpawnee(*((pyKey*)value)->fThis);
        return 0;
    } else {
        PyErr_SetString(PyExc_TypeError, "spawnee should be a plKey");
        return -1;
    }
}

static PyMethodDef pySpawnedEventData_Methods[] = {
    { "Convert", (PyCFunction)pySpawnedEventData_Convert, METH_VARARGS | METH_STATIC,
      "Convert a proEventData to a proSpawnedEventData" },
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pySpawnedEventData_GetSet[] = {
    { "spawner", (getter)pySpawnedEventData_getSpawner, (setter)pySpawnedEventData_setSpawner, NULL, NULL },
    { "spawnee", (getter)pySpawnedEventData_getSpawnee, (setter)pySpawnedEventData_setSpawnee, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pySpawnedEventData_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.proSpawnedEventData",     /* tp_name */
    sizeof(pySpawnedEventData),         /* tp_basicsize */
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
    "proSpawnedEventData wrapper",      /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pySpawnedEventData_Methods,         /* tp_methods */
    NULL,                               /* tp_members */
    pySpawnedEventData_GetSet,          /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pySpawnedEventData_new,             /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */
};

PyObject* Init_pySpawnedEventData_Type() {
    pySpawnedEventData_Type.tp_base = &pyEventData_Type;
    if (PyType_Ready(&pySpawnedEventData_Type) < 0)
        return NULL;

    Py_INCREF(&pySpawnedEventData_Type);
    return (PyObject*)&pySpawnedEventData_Type;
}

int pySpawnedEventData_Check(PyObject* obj) {
    if (obj->ob_type == &pySpawnedEventData_Type
        || PyType_IsSubtype(obj->ob_type, &pySpawnedEventData_Type))
        return 1;
    return 0;
}

PyObject* pySpawnedEventData_FromSpawnedEventData(proSpawnedEventData* evt) {
    if (evt == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pySpawnedEventData* pyobj = PyObject_New(pySpawnedEventData, &pySpawnedEventData_Type);
    pyobj->fThis = evt;
    pyobj->fPyOwned = false;
    return (PyObject*)pyobj;
}

}