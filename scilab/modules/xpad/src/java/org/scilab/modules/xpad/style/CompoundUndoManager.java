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
	private int nbEdits = 0;
	
	void startCompoundEdit(){
		//System.err.println("starting compound edit");
		compoundEdit = new CompoundEdit();
		addEdit(compoundEdit);
	}
	void endCompoundEdit(){
		//System.err.println("ending compound edit");
		compoundEdit.end();
		compoundEdit = null;
	}
	
	public void undo(){
		--nbEdits;
		super.undo();
	}

	public void redo(){
		++nbEdits;
		super.redo();
	}

	public boolean isAtReference(){
		return nbEdits == 0;
	}
	
	public void setReference(){
		nbEdits = 0;
	}
	
	public void discardAllEdits() {
		super.discardAllEdits();
		setReference();
	}
	
	public void undoableEditHappened(UndoableEditEvent e) {
		
		AbstractDocument.DefaultDocumentEvent event =
			(AbstractDocument.DefaultDocumentEvent)e.getEdit();

		if  (event.getType().equals(DocumentEvent.EventType.CHANGE))
		{
			// do not store change events
			return;
		}
		//System.err.println("storing an edit");
		addEdit( e.getEdit() );
		++nbEdits;
	}
}
		