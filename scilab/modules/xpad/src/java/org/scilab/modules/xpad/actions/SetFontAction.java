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

package org.scilab.modules.xpad.actions;

import java.awt.Font;
import java.util.List;

import org.scilab.modules.gui.bridge.fontchooser.SwingScilabFontChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.ScilabEditorPane;
import org.scilab.modules.xpad.utils.ConfigXpadManager;
import org.scilab.modules.xpad.utils.XpadMessages;

public class SetFontAction extends DefaultAction {

        private SetFontAction(Xpad editor) {
                super(XpadMessages.SET_FONT, editor);
        }

        public void doAction() {
                SwingScilabFontChooser _fontChooser = new SwingScilabFontChooser(ConfigXpadManager.getFont());
                _fontChooser.displayAndWait();

                Font newFont = _fontChooser.getSelectedFont();

                if (newFont != null) {

                        List<String> listStylesName = ConfigXpadManager.getAllStyleName();

                        getEditor().getTextPane().setFont(newFont);

                        /*we need to loop on every style , if not after the second change, styles will not change anymore
                  except default*/
                        int numberOfTab = getEditor().getTabPane().getComponentCount();
                        for (int j = 0; j < numberOfTab; j++) {
                            ScilabEditorPane textPane = getEditor().getTextPane(j);
                            textPane.resetFont(newFont);
                            if (textPane.getRightTextPane() != null) {
                                textPane.getRightTextPane().resetFont(newFont);
                            }
                        }
                        getEditor().getTextPane().setFocusable(true);
                        ConfigXpadManager.saveFont(newFont);
                }

        }

        public static MenuItem createMenu(Xpad editor) {
                return createMenu(XpadMessages.SET_FONT, null, new SetFontAction(editor), null);
        }

}
