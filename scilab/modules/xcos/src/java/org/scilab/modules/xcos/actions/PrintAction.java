/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.actions;

import java.awt.Toolkit;
import java.awt.event.KeyEvent;
import java.awt.print.PageFormat;
import java.awt.print.Paper;
import java.awt.print.PrinterException;
import java.awt.print.PrinterJob;

import javax.swing.KeyStroke;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.swing.mxGraphComponent;

/**
 * Diagram printing management
 * @author Vincent COUVERT
 *
 */
public final class PrintAction extends DefaultAction {

	private static final long serialVersionUID = 1L;

	/** Constructor
	 * @param scilabGraph associated diagram
	 */
	private PrintAction(ScilabGraph scilabGraph) {
		super(XcosMessages.PRINT, scilabGraph);
	}

	/**
	 * Print menu creation 
	 * @param scilabGraph associated diagram
	 * @return the menu
	 */
	public static MenuItem createMenu(ScilabGraph scilabGraph) {
		return createMenu(XcosMessages.PRINT, null, new PrintAction(scilabGraph),
				KeyStroke.getKeyStroke(KeyEvent.VK_P, Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
	}

	/**
	 * Print button creation 
	 * @param scilabGraph associated diagram
	 * @return the button
	 */
	public static PushButton createButton(ScilabGraph scilabGraph) {
		return createButton(XcosMessages.PRINT, "document-print.png", new PrintAction(scilabGraph));
	}

	/**
	 * Action !
	 * @see org.scilab.modules.graph.actions.DefaultAction#doAction()
	 */
	public void doAction() {
		mxGraphComponent graphComponent = getGraph(null).getAsComponent();
		PrinterJob pj = PrinterJob.getPrinterJob();

		if (pj.printDialog()) {
			PageFormat pf = graphComponent.getPageFormat();
			Paper paper = new Paper();
			double margin = 36;
			paper.setImageableArea(margin, margin, paper.getWidth()	- margin * 2, paper.getHeight() - margin * 2);
			pf.setPaper(paper);
			pj.setPrintable(graphComponent, pf);

			try {
				pj.print();
			} catch (PrinterException e2) {
				System.out.println(e2);
			}
		}

	}
}
