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

package org.scilab.modules.graphic_objects.graphicObject;

/**
 * UIWidget Exception class
 */
public class GraphicObjectException extends Exception {

    /**
     *
     */
    private static final long serialVersionUID = 693332709505471397L;

    /**
     * Default constructor
     * @param msg the error message
     */
    public GraphicObjectException(String msg) {
        super(msg);
    }
}
