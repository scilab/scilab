/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.xpad.Xpad;

public class ShowToolBarAction extends DefaultCheckAction {

    private ShowToolBarAction(Xpad editor) {
	super("Show/Hide ToolBar", editor);
    }
    
    public static CheckBoxMenuItem createCheckBoxMenu(Xpad editor) {
	return createCheckBoxMenu("Show/Hide ToolBar", null, new ShowToolBarAction(editor), null);
    }
    
}
