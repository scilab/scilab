/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.graph.utils;

import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.Map.Entry;

/**
 * Perform useful conversions between a style string and key/value based map.
 */
@SuppressWarnings(value = { "serial" })
public final class StyleMap extends LinkedHashMap<String, String> {

    /**
     * Create a Map from a style string
     * @param style The string which contains key=value list
     */
    public StyleMap(String style) {
        super();
        putAll(style);
    }

    /**
     * Put all the style attributes to the current map
     * @param style The string which contains key=value list
     * @return the current map
     */
    public StyleMap putAll(String style) {
        if (style != null && style.length() > 0) {
            final String[] pairs = style.split(";");

            for (String keyValue : pairs) {
                final int sep = keyValue.indexOf('=');

                if (sep >= 0) {
                    put(keyValue.substring(0, sep), keyValue.substring(sep + 1));
                } else {
                    put(keyValue, null);
                }
            }
        }
        return this;
    }

    /**
     * Export to a key=value; string
     * @return formatted string
     */
    @Override
    public String toString() {
        StringBuilder str = new StringBuilder();
        String valueRef = null;

        for (Iterator<Entry<String, String>> iterator = entrySet().iterator();
                iterator.hasNext();) {
            Entry<String, String> entry = iterator.next();

            str.append(entry.getKey());

            valueRef = entry.getValue();
            if (valueRef != null && valueRef.length() > 0) {
                str.append("=");
                str.append(valueRef);
            }

            if (iterator.hasNext()) {
                str.append(";");
            }

        }

        return str.toString();
    }
}
