/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.localization;


import java.util.MissingResourceException;
import java.util.ResourceBundle;
import java.util.Locale;

public class Messages {

    /**
     * Returns the translation of a message
	 * Returns the same string if not found
     *
     * @param key the string to translate
     * @return The translated string (or the same if the translation is not avaiable)
	 */
    public static String gettext(String key) {
		return Localization.getTextWarp(key);
    }

}
