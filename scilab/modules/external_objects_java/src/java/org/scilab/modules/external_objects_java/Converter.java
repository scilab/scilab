/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
