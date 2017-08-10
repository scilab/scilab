/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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
package org.scilab.modules.graph.utils;

import org.scilab.modules.localization.Messages;

/**
 * Internationalized messages for this graph
 */
public final class ScilabGraphMessages {

    public static final String COPY = Messages.gettext("Copy");
    public static final String CUT = Messages.gettext("Cut");
    public static final String GROUP = Messages.gettext("Group");
    public static final String UNGROUP = Messages.gettext("UnGroup");
    public static final String PASTE = Messages.gettext("Paste");
    public static final String REDO = Messages.gettext("Redo");
    public static final String UNDO = Messages.gettext("Undo");
    public static final String ZOOM_IN = Messages.gettext("Zoom In");
    public static final String ZOOM_OUT = Messages.gettext("Zoom Out");
    public static final String DELETE = Messages.gettext("Delete");
    public static final String SELECT_ALL = Messages.gettext("Select all");
    public static final String INVERT_SELECTION = Messages.gettext("Invert selection");
    public static final String UNTITLED = Messages.gettext("Untitled - %s");

    /** This class is a static singleton, thus it must not be instantiated */
    private ScilabGraphMessages() {
    }

}
