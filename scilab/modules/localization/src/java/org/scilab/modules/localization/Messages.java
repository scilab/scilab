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

    private static final String systemLocale = "LC_MESSAGES"; 
    private static final String defaultLocale = "en_US";
    private static final String UTF8 = "UTF-8"; 
	private static final String pathToTheClass = "org.scilab.modules.localization.Messages";
    private static ResourceBundle resourceBundle;
    private static boolean failedToLoadBundle;
    /**
     * Private method to load the bundle file
     *
     */
	private static void loadBundle() {
		try {
			String locale = System.getenv(systemLocale);
			if (locale != null && !locale.equals("")) { /* If we haven't been able to get the language from the env */
				String[] localeLanguageCountry = convertLocale(locale);
				resourceBundle = ResourceBundle.getBundle(pathToTheClass, 
						new Locale(localeLanguageCountry[0], localeLanguageCountry[1]));
			} else {
				failedToLoadBundle = true;
			}
		} catch (java.util.MissingResourceException e) {
			System.err.println("Could not file localization file for " + systemLocale);
			System.err.println("Switch back to the default language " + defaultLocale);
			try {
				String[] localeLanguageCountry = convertLocale(defaultLocale);
				resourceBundle = ResourceBundle.getBundle(pathToTheClass, 
						new Locale(localeLanguageCountry[0], localeLanguageCountry[1]));
			} catch (java.util.MissingResourceException e2) {
				failedToLoadBundle = true;
			}
		}
	}
	
	/**
     * converts locale string Language_Country.CodePage to 
	 * returns language, country, code page
     * @param localeEnv Language_Country.CodePage
     * @return String[] language, country, code page
	 */
	private static String[] convertLocale(String localeEnv) {
		final int SIZELOCALESPLIT = 3; 
		final int CASETHREE = 3;
		final int CASETWO = 2;
		String [] localeSplit = new String[SIZELOCALESPLIT];
		
		String [] splitString = localeEnv.split("[_.]");
		switch (splitString.length) {
			case CASETHREE : localeSplit = splitString;	break;
			case CASETWO : {
			localeSplit[0] = splitString[0]; 
			localeSplit[1] = splitString[1];
			localeSplit[2] = new String(UTF8);
		}
		break;
		
		default : {
			/* error in format localeEnv then we use default language */
			localeSplit[0] = new String("en");
			localeSplit[1] = new String("US");
			localeSplit[2] = new String(UTF8);
		}
		break;
		
		}
		return localeSplit;
	}
	

    /**
     * Returns the translation of a message
	 * Returns the same string if not found
     *
     * @param key the string to translate
     * @return The translated string (or the same if the translation is not avaiable)
	 */
    public static String gettext(String key) {
        /* If the bundle failed to load, just return the key */
        if (failedToLoadBundle) {
            return key;
        }
        /* Load the bundle the first call to this (static) method */
        if (resourceBundle == null) {
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

}
