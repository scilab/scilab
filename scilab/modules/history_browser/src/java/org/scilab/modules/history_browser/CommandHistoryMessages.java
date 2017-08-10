/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.history_browser;

import org.scilab.modules.localization.Messages;

/**
 * All localized messages/menus labels used in History Browser
 */
public final class CommandHistoryMessages {

    public static final String TITLE = Messages.gettext("Command History");

    public static final String FILE = Messages.gettext("File");
    public static final String CLOSE = Messages.gettext("Close");

    public static final String EDIT = Messages.gettext("Edit");
    public static final String COPY = Messages.gettext("Copy");
    public static final String CUT = Messages.gettext("Cut");
    public static final String DELETE = Messages.gettext("Delete");
    public static final String CLEAR_HISTORY = Messages.gettext("Clear history");

    public static final String HELP = Messages.gettext("?");
    public static final String SCILAB_HELP = Messages.gettext("Scilab Help");

    public static final String EVALUATE_SELECTION = Messages.gettext("Evaluate Selection");

    public static final String EDIT_IN_SCINOTES = Messages.gettext("Edit in SciNotes");

    public static final String ERROR = Messages.gettext("Error");
    public static final String CANNOT_DELETE_CURRENT_SESSION_NODE = Messages.gettext("Cannot delete current session beginning line.");

    public static final String SCILAB_PREFS = Messages.gettext("History preferences");

    /**
     * This class is a static singleton
     * Thus it must not be instantiated
     */
    private CommandHistoryMessages() {

    }

}
