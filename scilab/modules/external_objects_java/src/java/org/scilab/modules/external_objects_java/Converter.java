/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

public interface Converter {

    /**
     * Convert an Object to another where its class is to
     * @param original the object to convert
     * @param to the destination class
     * @return the converted object
     */
    Object convert(Object original, Class<?> to);

    /**
     * Check if the converter is able to convert an object with class equal to 'from' to an object with class 'to'
     * @param from the base class
     * @param to the destination class
     * @return true if the converter is able to convert
     */
    boolean canConvert(Class<?> from, Class<?> to);
}
