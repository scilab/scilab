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

import java.awt.print.PageFormat;
import java.awt.print.PrinterJob;
import javax.print.attribute.HashPrintRequestAttributeSet;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * Class Page setup action for SciNotes
 * @author Sylvestre Koumar
 *
 */
public class PageSetupAction extends DefaultAction {

    private static PageFormat pageFormat;

    static {
        pageFormat = new PageFormat();
        pageFormat.setPaper(ConfigSciNotesManager.getPaperFormat());
    }

    /**
     * Default constructor
     * @param name the name of the action
     * @param editor Editor
     */
    public PageSetupAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Function Run
     */
    public void doAction() {
        pageFormat = PrinterJob.getPrinterJob().pageDialog(pageFormat);
        ConfigSciNotesManager.savePaperFormat(pageFormat.getPaper());
    }

    /**
     * Create the MenuItem for page setup action
     * @param label label of the menu
     * @param editor Editor
     * @param key KeyStroke
     * @return a MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new PageSetupAction(label, editor), key);
    }

    /**
     * Get the page format
     * @return PageFormat
     */
    public static PageFormat getPageFormat() {
        return pageFormat;
    }
}
