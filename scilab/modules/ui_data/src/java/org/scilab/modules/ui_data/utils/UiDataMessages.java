/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.ui_data.utils;

import java.awt.event.MouseEvent;

import javax.swing.SwingUtilities;

import org.scilab.modules.localization.Messages;


/**
 * All localized messages/menus labels used in Editvar and Browsevar
 */
// CSOFF: JavadocVariable
// CSOFF: LineLength
// CSOFF: MultipleStringLiterals

public final class UiDataMessages {

    /* File menu */
    public static final String FILE = Messages.gettext("File");
    public static final String CLOSE = Messages.gettext("Close");
    
    public static final String FILTER = Messages.gettext("Filter");
    
    public static final String DOUBLE = Messages.gettext("double");
    public static final String POLYNOMIAL = Messages.gettext("polynomial");
    public static final String BOOLEAN = Messages.gettext("boolean");
    public static final String SPARCE = Messages.gettext("sparce");
    public static final String SPARCE_BOOLEAN = Messages.gettext("sparce boolean");
    public static final String INTEGER = Messages.gettext("integer");
    public static final String GRAPHIC_HANDLES = Messages.gettext("graphic handles");
    public static final String STRING = Messages.gettext("string");
    public static final String UNCOMPILED_FUNC = Messages.gettext("un-compiled function");
    public static final String COMPILED_FUNC = Messages.gettext("compiled function");
    public static final String FUNCTIONLIB = Messages.gettext("function library");
    public static final String LIST = Messages.gettext("list");
    public static final String TLIST = Messages.gettext("tlist");
    public static final String MLIST = Messages.gettext("mlist");
    public static final String POINTER = Messages.gettext("pointer");
    /** This class is a static singleton, thus it must not be instantiated */
    private UiDataMessages() { }
    
    
    
 // CSON: JavadocVariable
 // CSON: LineLength
 // CSON: MultipleStringLiterals
     
    /**
     * This function checks for the popup menu activation under MacOS with Java version 1.5
     * Related to Scilab bug #5190
     * @param e Click event
     * @return true if Java 1.5 and MacOS and mouse clic and ctrl activated
     */
	public static boolean isMacOsPopupTrigger(MouseEvent e) {
		return (SwingUtilities.isLeftMouseButton(e)
				&& e.isControlDown()
				&& (System.getProperty("os.name").toLowerCase().indexOf("mac") != -1) && (System
				.getProperty("java.specification.version").equals("1.5")));
	}
}
