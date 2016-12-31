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

#include "pyDynamicEnvMap.h"

#include <PRP/Surface/plDynamicEnvMap.h>
#include "pyRenderTarget.h"
#include "PRP/KeyedObject/pyKey.h"
#include "Math/pyGeometry3.h"
#include "Sys/pyColor.h"

extern "C" {

PY_PLASMA_NEW(DynamicCamMap, plDynamicCamMap)

static PyObject* pyDynamicCamMap_addMatLayer(pyDynamicCamMap* self, PyObject* args) {
    PyObject* key;
    if (!(PyArg_ParseTuple(args, "O", &key) && pyKey_Check(key))) {
        PyErr_SetString(PyExc_TypeError, "addMatLayer expects a plKey");
        return NULL;
    }
    self->fThis->addMatLayer(*((pyKey*)key)->fThis);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_clearMatLayers(pyDynamicCamMap* self) {
    self->fThis->clearMatLayers();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_delMatLayer(pyDynamicCamMap* self, PyObject* args) {
    Py_ssize_t idx;
    if (!PyArg_ParseTuple(args, "n", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delMatLayer expects an int");
        return NULL;
    }
    self->fThis->delMatLayer((size_t)idx);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_addTargetNode(pyDynamicCamMap* self, PyObject* args) {
    PyObject* key;
    if (!(PyArg_ParseTuple(args, "O", &key) && pyKey_Check(key))) {
        PyErr_SetString(PyExc_TypeError, "addTargetNode expects a plKey");
        return NULL;
    }
    self->fThis->addTargetNode(*((pyKey*)key)->fThis);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_clearTargetNodes(pyDynamicCamMap* self) {
    self->fThis->clearTargetNodes();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_delTargetNode(pyDynamicCamMap* self, PyObject* args) {
    Py_ssize_t idx;
    if (!PyArg_ParseTuple(args, "n", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delTargetNode expects an int");
        return NULL;
    }
    self->fThis->delTargetNode((size_t)idx);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_addVisRegion(pyDynamicCamMap* self, PyObject* args) {
    PyObject* key;
    if (!(PyArg_ParseTuple(args, "O", &key) && pyKey_Check(key))) {
        PyErr_SetString(PyExc_TypeError, "addVisRegion expects a plKey");
        return NULL;
    }
    self->fThis->addVisRegion(*((pyKey*)key)->fThis);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_clearVisRegions(pyDynamicCamMap* self) {
    self->fThis->clearVisRegions();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_delVisRegion(pyDynamicCamMap* self, PyObject* args) {
    Py_ssize_t idx;
    if (!PyArg_ParseTuple(args, "n", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delVisRegion expects an int");
        return NULL;
    }
    self->fThis->delVisRegion((size_t)idx);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_addVisRegionName(pyDynamicCamMap* self, PyObject* args) {
    PyObject* name;
    if (!(PyArg_ParseTuple(args, "O", &name) && PyAnyStr_Check(name))) {
        PyErr_SetString(PyExc_TypeError, "addVisRegionName expects a string");
        return NULL;
    }
    self->fThis->addVisRegionName(PyStr_To_PlStr(name));
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_clearVisRegionNames(pyDynamicCamMap* self) {
    self->fThis->clearVisRegionNames();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_delVisRegionName(pyDynamicCamMap* self, PyObject* args) {
    Py_ssize_t idx;
    if (!PyArg_ParseTuple(args, "n", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delVisRegionName expects an int");
        return NULL;
    }
    self->fThis->delVisRegionName((size_t)idx);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* pyDynamicCamMap_getVisRegions(pyDynamicCamMap* self, void*) {
    const std::vector<plKey>& keys = self->fThis->getVisRegions();
    PyObject* regionList = PyList_New(keys.size());
    for (size_t i=0; i<keys.size(); i++)
        PyList_SET_ITEM(regionList, i, pyKey_FromKey(keys[i]));
    return regionList;
}

static PyObject* pyDynamicCamMap_getTargetNodes(pyDynamicCamMap* self, void*) {
    const std::vector<plKey>& keys = self->fThis->getTargetNodes();
    PyObject* nodeList = PyList_New(keys.size());
    for (size_t i=0; i<keys.size(); i++)
        PyList_SET_ITEM(nodeList, i, pyKey_FromKey(keys[i]));
    return nodeList;
}

static PyObject* pyDynamicCamMap_getMatLayers(pyDynamicCamMap* self, void*) {
    const std::vector<plKey>& keys = self->fThis->getMatLayers();
    PyObject* layerList = PyList_New(keys.size());
    for (size_t i=0; i<keys.size(); i++)
        PyList_SET_ITEM(layerList, i, pyKey_FromKey(keys[i]));
    return layerList;
}

static PyObject* pyDynamicCamMap_getVisRegionNames(pyDynamicCamMap* self, void*) {
    const std::vector<plString>& names = self->fThis->getVisRegionNames();
    PyObject* regionNameList = PyList_New(names.size());
    for (size_t i=0; i<names.size(); i++)
        PyList_SET_ITEM(regionNameList, i, PlasmaString_To_PyString(names[i]));
    return regionNameList;
}

static int pyDynamicCamMap_setVisRegions(pyDynamicCamMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "visRegions should be a sequence of plKeys");
        return -1;
    }
    std::vector<plKey> regions;
    regions.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* region = PySequence_GetItem(value, i);
        if (pyKey_Check(region)){
            regions[i] = *(reinterpret_cast<pyKey *>(region)->fThis);
        } else {
            PyErr_SetString(PyExc_TypeError, "visRegions should be a sequence of plKeys");
            return -1;
        }
    }
    self->fThis->setVisRegions(regions);
    return 0;
}

static int pyDynamicCamMap_setTargetNodes(pyDynamicCamMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "targetNodes should be a sequence of plKeys");
        return -1;
    }
    std::vector<plKey> nodes;
    nodes.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* node = PySequence_GetItem(value, i);
        if (pyKey_Check(node)){
            nodes[i] = *(reinterpret_cast<pyKey *>(node)->fThis);
        } else {
            PyErr_SetString(PyExc_TypeError, "targetNodes should be a sequence of plKeys");
            return -1;
        }
    }
    self->fThis->setTargetNodes(nodes);
    return 0;
}

static int pyDynamicCamMap_setMatLayers(pyDynamicCamMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "matLayers should be a sequence of plKeys");
        return -1;
    }
    std::vector<plKey> layers;
    layers.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* layer = PySequence_GetItem(value, i);
        if (pyKey_Check(layer)){
            layers[i] = *(reinterpret_cast<pyKey *>(layer)->fThis);
        } else {
            PyErr_SetString(PyExc_TypeError, "matLayers should be a sequence of plKeys");
            return -1;
        }
    }
    self->fThis->setMatLayers(layers);
    return 0;
}

static int pyDynamicCamMap_setVisRegionNames(pyDynamicCamMap* self, PyObject* value, void*) {
    if (value == NULL || !PySequence_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "visRegionNames should be a sequence of strings");
        return -1;
    }
    std::vector<plString> names;
    names.resize(PySequence_Size(value));
    for (Py_ssize_t i=0; i<PySequence_Size(value); i++) {
        PyObject* name = PySequence_GetItem(value, i);
        if (PyAnyStr_Check(name)) {
            names[i] = PyString_To_PlasmaString(name);
        } else {
            PyErr_SetString(PyExc_TypeError, "visRegionNames should be a sequence of strings");
            return -1;
        }
    }
    self->fThis->setVisRegionNames(names);
    return 0;
}

static PyMethodDef pyDynamicCamMap_Methods[] = {
    { "addMatLayer", (PyCFunction)pyDynamicCamMap_addMatLayer, METH_VARARGS,
      "Params: key\n"
      "Adds a layer key" },
    { "clearMatLayers", (PyCFunction)pyDynamicCamMap_clearMatLayers, METH_NOARGS,
      "Clears the list of layer keys" },
    { "delMatLayer", (PyCFunction)pyDynamicCamMap_delMatLayer, METH_VARARGS,
      "Params: idx\n"
      "Removes a layer key" },
    { "addTargetNode", (PyCFunction)pyDynamicCamMap_addTargetNode, METH_VARARGS,
      "Params: key\n"
      "Adds a target SceneObject key" },
    { "clearTargetNodes", (PyCFunction)pyDynamicCamMap_clearTargetNodes, METH_NOARGS,
      "Clears the list of target SceneObject keys" },
    { "delTargetNode", (PyCFunction)pyDynamicCamMap_delTargetNode, METH_VARARGS,
      "Params: idx\n"
      "Removes a target SceneObject key" },
    { "addVisRegion", (PyCFunction)pyDynamicCamMap_addVisRegion, METH_VARARGS,
      "Params: key\n"
      "Adds a VisRegion key" },
    { "clearVisRegions", (PyCFunction)pyDynamicCamMap_clearVisRegions, METH_NOARGS,
      "Clears the list of VisRegion keys" },
    { "delVisRegion", (PyCFunction)pyDynamicCamMap_delVisRegion, METH_VARARGS,
      "Params: idx\n"
      "Removes a VisRegion key" },
    { "addVisRegionName", (PyCFunction)pyDynamicCamMap_addVisRegionName, METH_VARARGS,
      "Params: name\n"
      "Adds a VisRegion name" },
    { "clearVisRegionNames", (PyCFunction)pyDynamicCamMap_clearVisRegionNames, METH_NOARGS,
      "Clears the list of VisRegion names" },
    { "delVisRegionName", (PyCFunction)pyDynamicCamMap_delVisRegionName, METH_VARARGS,
      "Params: idx\n"
      "Removes a VisRegion name" },
    { NULL, NULL, 0, NULL }
};

PY_PROPERTY(float, DynamicCamMap, hither, getHither, setHither)
PY_PROPERTY(float, DynamicCamMap, yon, getYon, setYon)
PY_PROPERTY(float, DynamicCamMap, fogStart, getFogStart, setFogStart)
PY_PROPERTY(hsColorRGBA, DynamicCamMap, color, getColor, setColor)
PY_PROPERTY(float, DynamicCamMap, refreshRate, getRefreshRate, setRefreshRate)
PY_PROPERTY(bool, DynamicCamMap, incCharacters, getIncludeCharacters,
            setIncludeCharacters)
PY_PROPERTY(plKey, DynamicCamMap, camera, getCamera, setCamera)
PY_PROPERTY(plKey, DynamicCamMap, rootNode, getRootNode, setRootNode)
PY_PROPERTY(plKey, DynamicCamMap, disableTexture, getDisableTexture, setDisableTexture)

static PyGetSetDef pyDynamicCamMap_GetSet[] = {
    pyDynamicCamMap_hither_getset,
    pyDynamicCamMap_yon_getset,
    pyDynamicCamMap_fogStart_getset,
    pyDynamicCamMap_color_getset,
    pyDynamicCamMap_refreshRate_getset,
    { _pycs("visRegions"), (getter)pyDynamicCamMap_getVisRegions, (setter)pyDynamicCamMap_setVisRegions, NULL, NULL },
    { _pycs("targetNodes"), (getter)pyDynamicCamMap_getTargetNodes, (setter)pyDynamicCamMap_setTargetNodes, NULL, NULL },
    { _pycs("matLayers"), (getter)pyDynamicCamMap_getMatLayers, (setter)pyDynamicCamMap_setMatLayers, NULL, NULL },
    { _pycs("visRegionNames"), (getter)pyDynamicCamMap_getVisRegionNames, (setter)pyDynamicCamMap_setVisRegionNames, NULL, NULL },
    pyDynamicCamMap_incCharacters_getset,
    pyDynamicCamMap_camera_getset,
    pyDynamicCamMap_rootNode_getset,
    pyDynamicCamMap_disableTexture_getset,
    PY_GETSET_TERMINATOR
};

PyTypeObject pyDynamicCamMap_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyHSPlasma.plDynamicCamMap",       /* tp_name */
    sizeof(pyDynamicCamMap),            /* tp_basicsize */
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
    "plDynamicCamMap wrapper",          /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyDynamicCamMap_Methods,            /* tp_methods */
    NULL,                               /* tp_members */
    pyDynamicCamMap_GetSet,             /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyDynamicCamMap_new,                /* tp_new */
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

PyObject* Init_pyDynamicCamMap_Type() {
    pyDynamicCamMap_Type.tp_base = &pyRenderTarget_Type;
    if (PyType_Ready(&pyDynamicCamMap_Type) < 0)
        return NULL;

    Py_INCREF(&pyDynamicCamMap_Type);
    return (PyObject*)&pyDynamicCamMap_Type;
}

PY_PLASMA_IFC_METHODS(DynamicCamMap, plDynamicCamMap)

}
