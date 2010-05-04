/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * Copyright (C) 2010 - Calixte DENIZET
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
import javax.swing.text.Document;
import javax.swing.text.Segment;
import javax.swing.text.BadLocationException;
import javax.swing.undo.CompoundEdit;
import javax.swing.undo.UndoManager;
import javax.swing.undo.CannotUndoException;

/**
 * Class CompoundUndoManager
 * @author Bernard Hugueney
 * @author Calixte DENIZET
 *
 */
public class CompoundUndoManager extends UndoManager {

    private CompoundEdit compoundEdit;
    private Segment seg = new Segment();
    private boolean remove;
    private int nbEdits;
    private char[] breaks;
    private int prevLine;

    private final int INITIALCAPACITY = 256;
    
    /**
     * constructor
     */
    public CompoundUndoManager() {
	super();
	setLimit(INITIALCAPACITY);
	setBreakingChars(" \n");
    }

    /**
     * setBreakingChars defines the breaking chars which break a compound when they are encountered.
     * @param breaks the breaking chars
     */
    public void setBreakingChars(String breaks) {
	this.breaks = breaks.toCharArray();
    }
    
    /**
     * startCompoundEdit
     */
    public void startCompoundEdit() {
	if (compoundEdit == null) {
	    compoundEdit = new CompoundEdit();
	    addEdit(compoundEdit);
	    ++nbEdits;
	}
    }
    
    /**
     * endCompoundEdit
     */
    public void endCompoundEdit() {
	if (compoundEdit != null) {
	    compoundEdit.end();
	    compoundEdit = null;
	}
    }
    
    /**
     * undo
     */
    public void undo() {
	endCompoundEdit();
	try {
	    super.undo();
	    --nbEdits;
	} catch (CannotUndoException e) {
	    return;
	}
    }
    
    /**
     * redo
     */
    public void redo() {
	++nbEdits;
	super.redo();
    }

    /**
     * isAtReference
     * @return boolean
     */
    public boolean isAtReference() {
	return nbEdits == 0;
    }
    
    /**
     * setReference
     */
    public void setReference() {
	nbEdits = 0;
    }
            
    /**
     * discardAllEdits
     */
    public void discardAllEdits() {
	endCompoundEdit();
	super.discardAllEdits();
    }
    
    /**
     * undoableEditHappened handles an undo-strategy :
     * - single chars are grouped in a same Edit but if this char is in breaks (e.g. ' ' or '\n')
     * - removed chars are grouped too
     * - if several modifs are made in different lines, then these are considered like several groups
     * - if a modif contains more than one char, then it is considered like one group.
     * @param e UndoableEditEvent
     */
    public void undoableEditHappened(UndoableEditEvent e) {
	DocumentEvent event = (AbstractDocument.DefaultDocumentEvent) e.getEdit();     
	Document doc = event.getDocument();
	
	if (event.getLength() == 1) {
	    if (!remove && event.getType() == DocumentEvent.EventType.REMOVE) {
		endCompoundEdit();
		remove = true;
	    }

	    if (remove && event.getType() == DocumentEvent.EventType.INSERT) {
		endCompoundEdit();
		remove = false;
	    }

	    try {
		doc.getText(event.getOffset(), 1, seg);
		boolean br = false;
		for (int i = 0; i < breaks.length && !br; i++) {
		    br = seg.array[seg.offset] == breaks[i];
		}
		if (!remove && br) {
		    startCompoundEdit();
		    compoundEdit.addEdit(e.getEdit());
		    endCompoundEdit();
		    return;
		} else {
		    if (doc.getDefaultRootElement().getElementIndex(event.getOffset()) != prevLine) {
			prevLine = doc.getDefaultRootElement().getElementIndex(event.getOffset());
			endCompoundEdit();
		    }
		    startCompoundEdit();
		    compoundEdit.addEdit(e.getEdit());
		    return;
		}
	    } catch (BadLocationException ex) {
		ex.printStackTrace();
	    }
	} else {
	    startCompoundEdit();
	    compoundEdit.addEdit(e.getEdit());
	}
    }
}
