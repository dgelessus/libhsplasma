/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pyKeyedObject.h"

#include <PRP/KeyedObject/hsKeyedObject.h>
#include "pyKey.h"
#include "PRP/pyCreatable.h"

extern "C" {

PY_PLASMA_INIT_DECL(KeyedObject) {
    const char* name = "";
    if (!PyArg_ParseTuple(args, "|s", &name)) {
        PyErr_SetString(PyExc_TypeError, "__init__ expects an optional string");
        return -1;
    }

    self->fThis->init(name);
    return 0;
}

PY_PLASMA_NEW_MSG(KeyedObject, "hsKeyedObject is abstract")

PY_GETSET_GETTER_DECL(KeyedObject, key) {
    if (self->fThis->getKey().Exists()) {
        return pyKey_FromKey(self->fThis->getKey());
    } else {
        Py_INCREF(Py_None);
        return Py_None;
    }
}

PY_PROPERTY_GETSET_RO_DECL(KeyedObject, key)

static PyGetSetDef pyKeyedObject_GetSet[] = {
    pyKeyedObject_key_getset,
    PY_GETSET_TERMINATOR
};

PyTypeObject pyKeyedObject_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyHSPlasma.hsKeyedObject",         /* tp_name */
    sizeof(pyKeyedObject),              /* tp_basicsize */
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
    "hsKeyedObject wrapper",            /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    NULL,                               /* tp_methods */
    NULL,                               /* tp_members */
    pyKeyedObject_GetSet,               /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    pyKeyedObject___init__,             /* tp_init */
    NULL,                               /* tp_alloc */
    pyKeyedObject_new,                  /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */

    NULL,                               /* tp_del */
    TP_VERSION_TAG_INIT                 /* tp_version_tag */
    TP_FINALIZE_INIT                    /* tp_finalize */
};

PyObject* Init_pyKeyedObject_Type() {
    pyKeyedObject_Type.tp_base = &pyCreatable_Type;
    if (PyType_Ready(&pyKeyedObject_Type) < 0)
        return NULL;

    Py_INCREF(&pyKeyedObject_Type);
    return (PyObject*)&pyKeyedObject_Type;
}

PY_PLASMA_IFC_METHODS(KeyedObject, hsKeyedObject)

}
