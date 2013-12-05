/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.ged;

import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.EditorManager;

/**
 * Manages the action of the buttons on the toolbar and menubar.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class StartGED {

    public static boolean enableNewGed = false;

    /**
    * Initializes the lightGED.
    */
    public static void quickGED(Integer uid) {
        if (enableNewGed) {
            Editor instanceEditor = EditorManager.getFromUid(uid);
            instanceEditor.onClickGED();
        }
    }
}
