/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes;

import javax.swing.event.DocumentEvent;
import javax.swing.event.UndoableEditEvent;
import javax.swing.text.AbstractDocument;
import javax.swing.text.Segment;
import javax.swing.text.BadLocationException;
import javax.swing.undo.CompoundEdit;
import javax.swing.undo.UndoManager;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.CannotRedoException;

/**
 * Class CompoundUndoManager
 * @author Bernard Hugueney
 * @author Calixte DENIZET
 *
 */
public class CompoundUndoManager extends UndoManager {

    private static final long serialVersionUID = 2400488911410627080L;

    private CompoundEdit compoundEdit;
    private ScilabDocument sdoc;
    private Segment seg = new Segment();
    private boolean remove;
    private int nbEdits;
    private char[] breaks;
    private int prevLine;
    private boolean oneShot;

    /**
     * Constructor
     * @param sdoc the ScilabDocument where we want to make undo/redo
     */
    public CompoundUndoManager(ScilabDocument sdoc) {
        super();
        this.sdoc = sdoc;
        setLimit(-1);
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
     * getBreakingChars returns the breaking chars
     * @return the breaking chars
     */
    public String getBreakingChars() {
        return new String(breaks);
    }

    /**
     * startCompoundEdit
     */
    public void startCompoundEdit() {
        if (compoundEdit == null && sdoc.getEditorPane() != null) {
            compoundEdit = new CompoundEdit();
            addEdit(compoundEdit);
            ++nbEdits;
            if (sdoc.getEditorPane().getEditor() != null) {
                sdoc.getEditorPane().getEditor().enableUndoButton(true);
            }
        }
    }

    /**
     * endCompoundEdit
     */
    public void endCompoundEdit() {
        if (compoundEdit != null && sdoc.getEditorPane() != null) {
            compoundEdit.end();
            compoundEdit = null;
        }
    }

    /**
     * Enable one shot, i.e. if several modifs on one char occurred then they
     * can be added in the same CompoundEdit.
     * @param b true if one shot must be enabled
     */
    public void enableOneShot(boolean b) {
        this.oneShot = b;
    }

    /**
     * undo
     */
    public void undo() {
        endCompoundEdit();
        try {
            super.undo();
            if (sdoc.getEditorPane().getEditor() != null) {
                sdoc.getEditorPane().getEditor().enableRedoButton(true);
            }
            --nbEdits;
            if (nbEdits == 0) {
                sdoc.setContentModified(false);
            }
            if (!canUndo() && sdoc.getEditorPane().getEditor() != null) {
                sdoc.getEditorPane().getEditor().enableUndoButton(false);
            }
        } catch (CannotUndoException e) {
            return;
        }
    }

    /**
     * redo
     */
    public void redo() {
        endCompoundEdit();
        try {
            super.redo();
            if (sdoc.getEditorPane().getEditor() != null) {
                sdoc.getEditorPane().getEditor().enableUndoButton(true);
            }
            ++nbEdits;
            if (nbEdits == 0) {
                sdoc.setContentModified(false);
            }
            if (!canRedo() && sdoc.getEditorPane().getEditor() != null) {
                sdoc.getEditorPane().getEditor().enableRedoButton(false);
            }
        } catch (CannotRedoException e) {
            return;
        }
    }

    /**
     * Enable or not the Undo and Redo buttons
     */
    public void enableUndoRedoButtons() {
        endCompoundEdit();
        if (sdoc.getEditorPane().getEditor() != null) {
            sdoc.getEditorPane().getEditor().enableRedoButton(canRedo());
            sdoc.getEditorPane().getEditor().enableUndoButton(canUndo());
        }
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
        if (sdoc.getEditorPane() != null) {
            if (!oneShot && event.getLength() == 1) {
                if (!remove && event.getType() == DocumentEvent.EventType.REMOVE) {
                    endCompoundEdit();
                    remove = true;
                }

                if (remove && event.getType() == DocumentEvent.EventType.INSERT) {
                    endCompoundEdit();
                    remove = false;
                }

                try {
                    sdoc.getText(event.getOffset(), 1, seg);
                    boolean br = false;
                    for (int i = 0; i < breaks.length && !br; i++) {
                        br = seg.array[seg.offset] == breaks[i];
                    }
                    if (!remove && br) {
                        // there is a problem when the window is splitted
                        // two compoundEdits are created !
                        endCompoundEdit();
                        startCompoundEdit();
                        compoundEdit.addEdit(e.getEdit());
                        endCompoundEdit();
                        return;
                    } else {
                        if (sdoc.getDefaultRootElement().getElementIndex(event.getOffset()) != prevLine) {
                            prevLine = sdoc.getDefaultRootElement().getElementIndex(event.getOffset());
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
}

