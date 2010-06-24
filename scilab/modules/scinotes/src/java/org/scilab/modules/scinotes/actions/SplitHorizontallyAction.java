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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Split horizontally
 * @author Calixte DENIZET
 */
public final class SplitHorizontallyAction extends DefaultAction {

        /**
         * Constructor
         * @param editor associated editor
         */
        public SplitHorizontallyAction(SciNotes editor) {
                super(SciNotesMessages.SPLIT_HORIZONTALLY, editor);
        }

        /**
         * Create the MenuItem
         * @param editor Editor
         * @param key KeyStroke
         * @return a MenuItem
         */
        public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
            return createMenu(SciNotesMessages.SPLIT_HORIZONTALLY, null, new SplitHorizontallyAction(editor), key);
        }

        /**
         * Action !!
         * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
         */
        public void doAction() {
            getEditor().splitTab(false);
        }
}
