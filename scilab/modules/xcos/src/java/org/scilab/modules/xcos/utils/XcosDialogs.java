/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.utils;

import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.graph.XcosDiagram;

/**
 * Xcos standard dialogs
 */
public final class XcosDialogs {

    /**
     * Constructor
     */
    private XcosDialogs() {

    }

    /**
     * Dialog displayed when no block is selected and one is needed for action
     *
     * @param diagram
     *            The associated diagram
     */
    public static void noBlockSelected(XcosDiagram diagram) {
        ScilabModalDialog.show(XcosTab.get(diagram),
                               XcosMessages.NO_BLOCK_SELECTED, XcosMessages.XCOS_ERROR,
                               IconType.ERROR_ICON);
    }

    /**
     * Dialog displayed when saving failed
     *
     * @param diagram
     *            The associated diagram
     */
    public static void couldNotSaveFile(XcosDiagram diagram) {
        ScilabModalDialog.show(XcosTab.get(diagram),
                               new String[] { XcosMessages.FAIL_SAVING_DIAGRAM },
                               XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
    }

    /**
     * Dialog displayed when loading failed
     *
     * @param diagram
     *            The associated diagram
     */
    public static void couldNotLoadFile(XcosDiagram diagram) {
        ScilabModalDialog.show(XcosTab.get(diagram),
                               new String[] { XcosMessages.FAIL_LOADING_DIAGRAM },
                               XcosMessages.XCOS_ERROR, IconType.ERROR_ICON);
    }
}
