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

package org.scilab.modules.scinotes.actions;

import javax.swing.KeyStroke;

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.CodeExporter;

/**
 * Class Print action for SciNotes
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 */
public class PrintAction extends DefaultAction {

    private boolean codeConverterLoaded;

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor the editor
     */
    public PrintAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Function doAction
     */
    public void doAction() {
        if (!codeConverterLoaded) {
            LoadClassPath.loadOnUse("copyAsHTMLinScinotes");
            LoadClassPath.loadOnUse("pdf_ps_eps_graphic_export");
            codeConverterLoaded = true;
        }
        ScilabEditorPane pane = (ScilabEditorPane) getEditor().getTextPane();
        CodeExporter.convert(pane, null, CodeExporter.PRINT, PageSetupAction.getPageFormat());
    }

    /**
     * Create the MenuItem for print action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new PrintAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new PrintAction(tooltip, editor));
    }
}
