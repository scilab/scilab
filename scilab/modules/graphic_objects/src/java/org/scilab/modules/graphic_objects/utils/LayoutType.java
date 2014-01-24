/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graphic_objects.utils;


public enum LayoutType {
    NONE, GRID, BORDER;

    public static LayoutType intToEnum(Integer intValue) {
        switch (intValue) {
            default:
            case 0:
                return LayoutType.NONE;
            case 1:
                return LayoutType.GRID;
            case 2:
                return LayoutType.BORDER;
        }
    }
}
