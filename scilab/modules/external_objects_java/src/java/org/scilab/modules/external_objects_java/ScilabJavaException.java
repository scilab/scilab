/*
 * JIMS ( http://forge.scilab.org/index.php/p/JIMS/ ) - This file is a part of JIMS
 * Copyright (C) 2010 - 2011 - Calixte DENIZET <calixte@contrib.scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

/**
 * ScilabJavaException class
 *
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public class ScilabJavaException extends Exception {

    /**
     * Default constructor
     * @param msg the exception message
     */
    public ScilabJavaException(String msg) {
        super(msg);
    }
}
