/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Split vertically
 * @author Calixte DENIZET
 */
public final class SplitVerticallyAction extends DefaultAction {

        /**
         * Constructor
         * @param editor associated editor
         */
        public SplitVerticallyAction(Xpad editor) {
                super(XpadMessages.SPLIT_VERTICALLY, editor);
        }

        /**
         * Create the MenuItem
         * @param editor Editor
         * @param key KeyStroke
         * @return a MenuItem
         */
        public static MenuItem createMenu(Xpad editor, KeyStroke key) {
            return createMenu(XpadMessages.SPLIT_VERTICALLY, null, new SplitVerticallyAction(editor), key);
        }

        /**
         * Action !!
         * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
         */
        public void doAction() {
            getEditor().splitTab(true);
        }
}
