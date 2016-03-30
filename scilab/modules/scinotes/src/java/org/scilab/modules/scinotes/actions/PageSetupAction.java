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
@SuppressWarnings(value = { "serial" })
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
