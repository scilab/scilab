/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
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

    private static final String bundleName = "Messages_scilab"; 
    private static final String systemLocale = "LC_MESSAGES"; 
    private static final String defaultLocale = "en_US"; 
	private static final String pathToTheClass = "org.scilab.modules.localization.Messages";
    private static ResourceBundle resourceBundle=null;
    private static boolean failedToLoadBundle=false;
    /**
     * Private method to load the bundle file
     *
     */
	private static void loadBundle(){
		try{
			String locale=System.getenv(systemLocale);
			resourceBundle = ResourceBundle.getBundle(pathToTheClass,new Locale(locale));
		}catch (java.util.MissingResourceException e){
			System.err.println("Could not file localization file for "+systemLocale);
			System.err.println("Switch back to the default language " + defaultLocale);
			try {
				resourceBundle = ResourceBundle.getBundle(pathToTheClass,new Locale(defaultLocale));
			} catch (java.util.MissingResourceException e2) {
				failedToLoadBundle=true;
			}
		}
	}

    /**
     * Returns the translation of a message
	 * Returns the same string if not found
     *
     * @param key   
     * @return <ReturnValue>
	 */
    public static String getText(String key) {
        /* If the bundle failed to load, just return the key */
        if (failedToLoadBundle) {
            return key;
        }
        /* Load the bundle the first call to this (static) method */
        if (resourceBundle==null) {
            loadBundle();
			/* If the bundle failed to load, just return the key */
			if (failedToLoadBundle) { /* Should be used only on the first start */
				return key;
			}
        }
        try {
            return resourceBundle.getString(key);
        } catch (MissingResourceException e) {
            /* Not found in the bundle, return the same message */
            return key;
        }
    }

	public static void main (String[] args) {
		Messages.getText("plop");

	}
}
