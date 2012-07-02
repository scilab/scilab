/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

/**
 * Set true or false for datatip toggle button
 * @author Gustavo Barbosa Libotte
 */
public class DatatipActivateCallBack {

    public static boolean isDatatipActivated = true;

    /**
    * Get the axes handler of the figure which mouse click belongs
    *
    * @return true if datatip button is activated or false otherwise
    */
    public static boolean toggleDatatip() {
        if (isDatatipActivated == false) {
            isDatatipActivated = true;
            return isDatatipActivated;
        } else {
            isDatatipActivated = false;
            return isDatatipActivated;
        }
    }
}
