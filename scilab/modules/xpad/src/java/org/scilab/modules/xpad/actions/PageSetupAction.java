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

import java.awt.print.PageFormat;
import java.awt.print.PrinterJob;

import javax.print.attribute.HashPrintRequestAttributeSet;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xpad.Xpad;
import org.scilab.modules.xpad.utils.XpadMessages;

/**
 * Class Page setup action for Xpad
 * @author Sylvestre Koumar
 *
 */
public class PageSetupAction extends DefaultAction {

	private static PageFormat pageFormat;

	/**
	 * Default constructor
	 * @param editor Editor
	 */
	private PageSetupAction(Xpad editor) {
		super(XpadMessages.PAGE_SETUP, editor);
	}

	/**
	 * Function Run
	 */
	public void doAction() {
		PrinterJob printTask = PrinterJob.getPrinterJob();
		if (pageFormat == null) {
			pageFormat = printTask.pageDialog(new HashPrintRequestAttributeSet());
		} else {
			pageFormat = printTask.pageDialog(pageFormat);
		}
	}

	/**
	 * Create the MenuItem for page setup action
	 * @param editor Editor
	 * @return a MenuItem
	 */
	public static MenuItem createMenu(Xpad editor) {
		return createMenu(XpadMessages.PAGE_SETUP, null, new PageSetupAction(editor), null);
	}

	/**
	 * Get the page format
	 * @return PageFormat
	 */
	public static PageFormat getPageFormat() {
		return pageFormat;
	}
}
