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

package org.scilab.modules.gui.uiwidget;

import java.util.HashMap;

/**
 * A Map where the value is convert into a Java object
 */
public class StringMap extends HashMap<String, String> implements ConvertableMap<String> {

    /**
     * Default constructor
     */
    public StringMap() {
        super();
    }

    /**
     * Constructor
     * @param len the map initial capacity
     */
    public StringMap(int len) {
        super(len);
    }

    /**
     * Get a value and convert it
     * @param clazz the class of the result after conversion
     * @param key the key
     * @param defaultValue the default value if the key is not in the map
     * @return the converted object
     */
    public <T> T get(Class<T> clazz, String key, T defaultValue) {
        return StringConverters.getObjectFromValue(clazz, get(key), defaultValue);
    }

    /**
     * Get a value and convert it
     * @param clazz the class of the result after conversion
     * @param key the key
     * @return the converted object
     */
    public <T> T get(Class<T> clazz, String key) {
        return StringConverters.getObjectFromValue(clazz, get(key));
    }
}
