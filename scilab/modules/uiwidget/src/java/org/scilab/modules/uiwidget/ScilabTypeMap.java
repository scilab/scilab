/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.uiwidget;

import java.util.HashMap;

import org.scilab.modules.types.ScilabType;

public class ScilabTypeMap extends HashMap<String, ScilabType> implements ConvertableMap<ScilabType> {

    public ScilabTypeMap() {
        super();
    }

    public <T> T get(Class<T> clazz, String key, T defaultValue) {
        return ScilabTypeConverters.getObjectFromValue(clazz, get(key), defaultValue);
    }

    public <T> T get(Class<T> clazz, String key) {
        return ScilabTypeConverters.getObjectFromValue(clazz, get(key));
    }
}
