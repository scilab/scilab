/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.utils;

import org.scilab.modules.localization.Messages;

public class XcosMessages {
	/** File menu */
    public static final String FILE = Messages.gettext("File");
    public static final String NEW = Messages.gettext("New") + "...";
    public static final String OPEN = Messages.gettext("Open") + "...";
    public static final String SAVE = Messages.gettext("Save") + "...";
    
    /** Edit Menu */
    public static final String EDIT = Messages.gettext("Edit") + "...";
    
    /** Simulation menu */
    public static final String SETUP = Messages.gettext("Setup");
    public static final String RUN = Messages.gettext("Run");
    public static final String STOP = Messages.gettext("Stop");
    
    
    public static final String DUMP = Messages.gettext("Dump");
    public static final String VIEW_IN_SCICOS = Messages.gettext("View in Scicos");
   
    
    
    public static final String SEARCH = Messages.gettext("Search");
    public static final String VIEW = Messages.gettext("View");
    public static final String DOCUMENT = Messages.gettext("Document");
    public static final String EXECUTE = Messages.gettext("Execute");
    public static final String SYNTAX_TYPE = Messages.gettext("Syntax Type");
    public static final String ENCODING_TYPE = Messages.gettext("Encoding Type");
    public static final String RECENT_FILES = Messages.gettext("Recent Files");
    
    public static final String FILE_DOESNT_EXIST = Messages.gettext( "This file doesn't exist\n Do you want to create it?");
    public static final String CLOSE_TAB_TIP = Messages.gettext("close this tab");
    public static final String FILE_MODIFIED = Messages.gettext("This file has been modified since last save\nsave it?");
    public static final String REPLACE_FILE_TITLE = Messages.gettext("This file has been modified since last save\nsave it?");
    public static final String FILE_ALREADY_EXIST = Messages.gettext("File already exists");
    public static final String UNTITLED = Messages.gettext("Untitled ");

}
