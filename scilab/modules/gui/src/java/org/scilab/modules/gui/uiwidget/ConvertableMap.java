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

import java.util.Map;

/**
 * Typically a ConvertableMap contains attributes values.
 * Value can be a string (if coming from XML or a ScilabType)
 * so we need to convert it into a valid Java object.
 */
public interface ConvertableMap<U> extends Map<String, U> {

    /**
     * Get the value associated to the key and convert it in an instance of the given clazz
     * @param clazz the base class
     * @param key the key value to get
     * @param defaultValue the default value to return when it is not possible to convert or if no value has been found
     * @return the converted value
     */
    public <T> T get(Class<T> clazz, String key, T defaultValue);

    /**
     * Get the value associated to the key and convert it in an instance of the given clazz
     * @param clazz the base class
     * @param key the key value to get
     * @return the converted value
     */
    public <T> T get(Class<T> clazz, String key);

}
