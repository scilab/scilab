/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.helptools;

import org.xml.sax.SAXException;

@SuppressWarnings(value = { "serial" })
public class UnhandledDocbookTagException extends SAXException {

    public UnhandledDocbookTagException(String tag) {
        super("The tag " + tag + " is not handled.");
    }
}