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

import java.util.Arrays;

public final class ScilabOperations {

    /**
     * Add a string and an other object
     * @param idA the id of the first object
     * @param idB the id of the second object
     * @return the id of the concatened Strings
     */
    public static final int add(final int idA, final int idB) throws ScilabJavaException {
        if (idA != 0 && idB != 0) {
            ScilabJavaObject A = ScilabJavaObject.arraySJO[idA];
            ScilabJavaObject B = ScilabJavaObject.arraySJO[idB];

            if (A.clazz == String.class) {
                return new ScilabJavaObject((String) A.object + toString(B), String.class).id;
            } else if (B.clazz == String.class) {
                return new ScilabJavaObject(toString(A) + (String) B.object, String.class).id;
            }
        }

        throw new ScilabJavaException("Invalid operation between objects");
    }

    /**
     * Convert an object to a string
     * @param A a ScilabJavaObject
     * @return the corresponding String
     */
    private static final String toString(ScilabJavaObject A) {
        String s;
        if (A.clazz.isArray()) {
            Class cl = A.clazz.getComponentType();
            if (cl.isPrimitive()) {
                s = Arrays.deepToString(new Object[] { A.object });
                s = s.substring(1, s.length() - 1);
            } else {
                s = Arrays.deepToString((Object[]) A.object);
            }
        } else {
            s = A.object.toString();
        }

        return s;
    }
}
