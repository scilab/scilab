/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
