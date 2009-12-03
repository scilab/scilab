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
package org.scilab.modules.xpad.style;

import javax.swing.event.DocumentEvent;
import javax.swing.event.UndoableEditEvent;
import javax.swing.text.AbstractDocument;
import javax.swing.undo.CompoundEdit;
import javax.swing.undo.UndoManager;

public class CompoundUndoManager extends UndoManager {

	CompoundEdit compoundEdit=null;
	
	void startCompoundEdit(){
		compoundEdit = new CompoundEdit();
		addEdit(compoundEdit);
	}
	void endCompoundEdit(){
		compoundEdit.end();
		compoundEdit = null;
	}
	
	public void undoableEditHappened(UndoableEditEvent e) {
		
		AbstractDocument.DefaultDocumentEvent event =
			(AbstractDocument.DefaultDocumentEvent)e.getEdit();

		if  (event.getType().equals(DocumentEvent.EventType.CHANGE))
		{
			// do not store change events
			return;
		}
		addEdit( e.getEdit() );
	}
}
		