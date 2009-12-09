/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xpad.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;

import javax.swing.JComponent;
import javax.swing.JTextPane;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;

import org.scilab.modules.xpad.ScilabEditorKit;
import org.scilab.modules.xpad.style.IndentManager;
import org.scilab.modules.xpad.style.ScilabStyleDocument;

/**
 * LineBeautifierAction Class
 * @author Bernard HUGUENEY
 *
 */
public final class LineBeautifierAction extends ScilabEditorKit.InsertBreakAction {
	

	/**
	 * serialVersionUID
	 */
	private static final long serialVersionUID = -8313095922543576108L;
	private IndentManager indentManager = new IndentManager();
	
	/**
	 * Constructor
	 */
	private LineBeautifierAction() {
		
	}
	
	/**
	 * actionPerformed
	 * @param ev ActionEvent
	 */
	public void actionPerformed(ActionEvent ev) {
		super.actionPerformed(ev);
		JTextPane textPane = (JTextPane) ev.getSource();
		ScilabStyleDocument doc =  (ScilabStyleDocument) textPane.getStyledDocument();
		javax.swing.text.Element root = doc.getDefaultRootElement();
		int line =  root.getElementIndex(textPane.getCaretPosition())-1;
		if (doc.isUpdater() && doc.getAutoIndent()) {
			boolean autoColorize = doc.getAutoColorize();
			doc.setAutoColorize(false);
			boolean mergeEdits = doc.getShouldMergeEdits();
			indentManager.beautifyLine(doc, line, true);
			doc.setAutoColorize(autoColorize);
			doc.setShouldMergeEdits(mergeEdits);
		}
	}
	
	/**
	 * putInInputMap
	 * @param textPane JComponent
	 */
	public static void putInInputMap(JComponent textPane) {
		textPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), new LineBeautifierAction());
	}
}
