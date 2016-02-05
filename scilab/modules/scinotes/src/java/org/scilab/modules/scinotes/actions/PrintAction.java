/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.scinotes.actions;

import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.CodeExporter;

/**
 * Class Print action for SciNotes
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
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
        ScilabEditorPane pane = getEditor().getTextPane();
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
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new PrintAction(tooltip, editor));
    }
}
