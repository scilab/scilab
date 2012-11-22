/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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

public class StringMap extends HashMap<String, String> implements ConvertableMap<String> {

    public StringMap() {
        super();
    }

    public StringMap(int len) {
        super(len);
    }

    public <T> T get(Class<T> clazz, String key, T defaultValue) {
        return StringConverters.getObjectFromValue(clazz, get(key), defaultValue);
    }

    public <T> T get(Class<T> clazz, String key) {
        return StringConverters.getObjectFromValue(clazz, get(key));
    }
}
