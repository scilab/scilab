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

    public static final String SCILABVAR = Messages.gettext("Hide Scilab variables");

    public static final String DOUBLE = Messages.gettext("Double");
    public static final String POLYNOMIAL = Messages.gettext("Polynomial");
    public static final String BOOLEAN = Messages.gettext("Boolean");
    public static final String SPARSE = Messages.gettext("Sparse");
    public static final String SPARSE_BOOLEAN = Messages.gettext("Sparse boolean");
    public static final String INTEGER = Messages.gettext("Integer");
    public static final String GRAPHIC_HANDLES = Messages.gettext("Graphic handles");
    public static final String STRING = Messages.gettext("String");
    public static final String UNCOMPILED_FUNC = Messages.gettext("Un-compiled function");
    public static final String COMPILED_FUNC = Messages.gettext("Compiled function");
    public static final String FUNCTIONLIB = Messages.gettext("Function library");
    public static final String LIST = Messages.gettext("List");
    public static final String TLIST = Messages.gettext("Tlist");
    public static final String MLIST = Messages.gettext("Mlist");
    public static final String POINTER = Messages.gettext("Pointer");
    public static final String INTRINSIC_FUNCTION = Messages.gettext("Intrinsic function");
    public static final String MATLAB_SPARSE = Messages.gettext("Matlab sparse");
    public static final String IMPLICIT_POLYNOMIAL = Messages.gettext("Implicit polynomial");

    public static final String VARIABLE_EDITOR = Messages.gettext("Variable Editor");
    public static final String VARIABLE_BROWSER = Messages.gettext("Variable Browser");

    public static final String GLOBAL_NOT_EDITABLE = Messages.gettext("Global variables can not be edited.");

    public static final String REFRESH = Messages.gettext("Refresh the variable ");
    public static final String COPY = Messages.gettext("Copy");
    public static final String PASTE = Messages.gettext("Paste");
    public static final String CUT = Messages.gettext("Cut");
    public static final String CLEAR = Messages.gettext("Clear");
    public static final String UNDO = Messages.gettext("Undo");
    public static final String REDO = Messages.gettext("Redo");

    public static final String FILE_BROWSER = Messages.gettext("File Browser");
    public static final String NAME_COLUMN = Messages.gettext("Name");
    public static final String SIZE_COLUMN = Messages.gettext("Size");
    public static final String TYPE_COLUMN = Messages.gettext("Type");
    public static final String LASTMODIF_COLUMN = Messages.gettext("Last modified");
    public static final String DIRECTORY = Messages.gettext("Directory");
    public static final String FILETYPE = Messages.gettext("%s File");
    public static final String FILEFILTER = Messages.gettext("Enter a file filter");
    public static final String CWD = Messages.gettext("Enter your working folder");
    public static final String CASESENSITIVE = Messages.gettext("Case sensitive");
    public static final String REGEXP = Messages.gettext("Regular expression");
    public static final String SELECTADIR = Messages.gettext("Select a directory");
    public static final String USERHOME = Messages.gettext("User home");
    public static final String SCIFOLDER = Messages.gettext("Scilab");

    public static final String CANNOTREAD = Messages.gettext("Cannot edit the file %s");
    public static final String CANNOTOPEN = Messages.gettext("Cannot open the file %s");

    public static final String OPENINSCINOTES = Messages.gettext("Open with SciNotes");
    public static final String EXECINCONSOLE = Messages.gettext("Execute in Scilab");
    public static final String OPENINXCOS = Messages.gettext("Open with Xcos");
    public static final String LOADINSCILAB = Messages.gettext("Load in Scilab");
    public static final String EDITWITHDEFAULT = Messages.gettext("Edit with default application");
    public static final String OPENWITHDEFAULT = Messages.gettext("Open with default application");

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
