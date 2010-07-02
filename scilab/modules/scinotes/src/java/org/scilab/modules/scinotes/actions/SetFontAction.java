/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
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

import java.awt.Font;
import java.util.List;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

public class SetFontAction extends DefaultAction {

        private SetFontAction(SciNotes editor) {
                super(SciNotesMessages.SET_FONT, editor);
        }

        public void doAction() {
                SwingScilabFontChooser _fontChooser = new SwingScilabFontChooser(ConfigSciNotesManager.getFont());
                _fontChooser.displayAndWait();

                Font newFont = _fontChooser.getSelectedFont();

                if (newFont != null) {

                        List<String> listStylesName = ConfigSciNotesManager.getAllStyleName();

                        getEditor().getTextPane().setFont(newFont);

                        /*we need to loop on every style , if not after the second change, styles will not change anymore
                  except default*/
                        int numberOfTab = getEditor().getTabPane().getComponentCount();
                        for (int j = 0; j < numberOfTab; j++) {
                            ScilabEditorPane textPane = getEditor().getTextPane(j);
                            textPane.resetFont(newFont);
                            if (textPane.getOtherPaneInSplit() != null) {
                                textPane.getOtherPaneInSplit().resetFont(newFont);
                            }
                        }
                        getEditor().getTextPane().setFocusable(true);
                        ConfigSciNotesManager.saveFont(newFont);
                }

        }

        public static MenuItem createMenu(SciNotes editor) {
                return createMenu(SciNotesMessages.SET_FONT, null, new SetFontAction(editor), null);
        }

}
