/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.ScilabDocument;
import org.scilab.modules.xpad.IndentManager;
import org.scilab.modules.xpad.utils.XpadMessages;


/**
 * IndentAction Class
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 *
 */
@SuppressWarnings("serial")
public final class IndentAction extends DefaultAction {

        /**
         * Constructor
         * @param editor Xpad
         */
        private IndentAction(Xpad editor) {
                super(XpadMessages.INDENT, editor);
        }

        /**
         * doAction
         */
        public void doAction() {
            ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
            int start = sep.getSelectionStart();
            int end = sep.getSelectionEnd();
            IndentManager indent = sep.getIndentManager();
            ScilabDocument doc = (ScilabDocument) sep.getDocument();
            int[] ret = new int[2];

            doc.mergeEditsBegin();
            if (start == end) {
                ret = indent.indentDoc(start, end);
                if (ret != null) {
                    sep.setCaretPosition(ret[0]);
                }
            } else {
                ret = indent.indentDoc(start, end - 1);
                if (ret != null) {
                    sep.setSelectionStart(ret[0]);
                    sep.setSelectionEnd(ret[1]);
                }
            }
            doc.mergeEditsEnd();
        }

        /**
         * createMenu
         * @param editor Xpad
         * @param key KeyStroke
         * @return MenuItem
         */
        public static MenuItem createMenu(Xpad editor, KeyStroke key) {
                return createMenu(XpadMessages.INDENT, null, new IndentAction(editor), key);
        }
}
