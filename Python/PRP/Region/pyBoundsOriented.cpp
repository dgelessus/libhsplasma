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

#include "pyBounds.h"

#include <PRP/Region/hsBounds.h>
#include "Math/pyGeometry3.h"

extern "C" {

PY_PLASMA_VALUE_NEW(BoundsOriented, hsBoundsOriented)

static PyObject* pyBoundsOriented_getPlanes(pyBoundsOriented* self, void*) {
    PyObject* list = PyList_New(self->fThis->getNumPlanes());
    for (size_t i=0; i<self->fThis->getNumPlanes(); i++)
        PyList_SET_ITEM(list, i, pyPlane3_FromPlane3(self->fThis->getPlanes()[i]));
    return list;
}

static int pyBoundsOriented_setPlanes(pyBoundsOriented* self, PyObject* value, void*) {
    if (value == NULL) {
        self->fThis->setPlanes(0, NULL);
        return 0;
    } else if (PyList_Check(value)) {
        size_t numPlanes = PyList_Size(value);
        hsPlane3* planes = new hsPlane3[numPlanes];
        for (size_t i=0; i<numPlanes; i++) {
            if (!pyPlane3_Check(PyList_GetItem(value, i))) {
                PyErr_SetString(PyExc_TypeError, "planes should be a list of hsPlane3 objects");
                delete[] planes;
                return -1;
            }
            planes[i] = *((pyPlane3*)PyList_GetItem(value, i))->fThis;
        }
        self->fThis->setPlanes(numPlanes, planes);
        delete[] planes;
        return 0;
    } else {
        PyErr_SetString(PyExc_TypeError, "planes should be a list of hsPlane3 objects");
        return -1;
    }
}

PY_PROPERTY(hsVector3, BoundsOriented, center, getCenter, setCenter)
PY_PROPERTY(unsigned int, BoundsOriented, centerValid, getCenterValid, setCenterValid)

static PyGetSetDef pyBoundsOriented_GetSet[] = {
    pyBoundsOriented_center_getset,
    pyBoundsOriented_centerValid_getset,
    { _pycs("planes"), (getter)pyBoundsOriented_getPlanes,
        (setter)pyBoundsOriented_setPlanes, NULL, NULL },
    PY_GETSET_TERMINATOR
};

PyTypeObject pyBoundsOriented_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyHSPlasma.hsBoundsOriented",      /* tp_name */
    sizeof(pyBoundsOriented),           /* tp_basicsize */
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
    "hsBoundsOriented wrapper",         /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    NULL,                               /* tp_methods */
    NULL,                               /* tp_members */
    pyBoundsOriented_GetSet,            /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyBoundsOriented_new,               /* tp_new */
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

PyObject* Init_pyBoundsOriented_Type() {
    pyBoundsOriented_Type.tp_base = &pyBounds_Type;
    if (PyType_Ready(&pyBoundsOriented_Type) < 0)
        return NULL;

    Py_INCREF(&pyBoundsOriented_Type);
    return (PyObject*)&pyBoundsOriented_Type;
}

PY_PLASMA_VALUE_IFC_METHODS(BoundsOriented, hsBoundsOriented)

}
