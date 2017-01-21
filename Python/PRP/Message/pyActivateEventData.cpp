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

#include "pyEventData.h"

#include <PRP/Message/proEventData.h>

PY_PLASMA_NEW(ActivateEventData, proActivateEventData)

PY_PROPERTY(bool, ActivateEventData, active, isActive, setActive)
PY_PROPERTY(bool, ActivateEventData, activate, isActivate, setActivate)

static PyGetSetDef pyActivateEventData_GetSet[] = {
    pyActivateEventData_active_getset,
    pyActivateEventData_activate_getset,
    PY_GETSET_TERMINATOR
};

PY_PLASMA_TYPE(ActivateEventData, proActivateEventData, "proActivateEventData wrapper")

PY_PLASMA_TYPE_INIT(ActivateEventData) {
    pyActivateEventData_Type.tp_new = pyActivateEventData_new;
    pyActivateEventData_Type.tp_getset = pyActivateEventData_GetSet;
    pyActivateEventData_Type.tp_base = &pyEventData_Type;
    if (PyType_CheckAndReady(&pyActivateEventData_Type) < 0)
        return NULL;

    Py_INCREF(&pyActivateEventData_Type);
    return (PyObject*)&pyActivateEventData_Type;
}

PY_PLASMA_IFC_METHODS(ActivateEventData, proActivateEventData)
