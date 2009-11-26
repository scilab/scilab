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

package org.scilab.modules.graph;

import java.util.List;

import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.localization.Messages;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.swing.handler.mxKeyboardHandler;
import com.mxgraph.swing.handler.mxRubberband;
import com.mxgraph.swing.util.mxGraphActions;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUndoManager;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.view.mxGraph;

public class ScilabGraph extends mxGraph {

    protected mxUndoManager undoManager = new mxUndoManager();
    protected mxGraphOutline graphOutline = null;
    protected mxKeyboardHandler keyboardHandler = null;
    protected mxGraphComponent component = null;

    private String title = Messages.gettext("Untitled");
    private String savedFile = null;
    private boolean modified = false;
    private Tab parentTab;
    private boolean opened = false;
    private boolean redoInAction = false;
    private int undoCounter = 0;

    /**
     * 
     */
    protected mxIEventListener changeTracker = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {
	    setModified(true);
	}
    };

    protected mxIEventListener undoHandler = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {

	    if (!redoInAction) {
		undoManager.undoableEditHappened((mxUndoableEdit) evt
			.getArgAt(0));
		incrementUndoCounter();
	    }
	}
    };

    mxIEventListener selectionHandler = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {
	    List changes = ((mxUndoableEdit) evt.getArgAt(0)).getChanges();
	    setSelectionCells(getSelectionCellsForChanges(changes));
	}
    };

    public ScilabGraph() {
	super();
	
	/*
	 * Disabling the default connected action and event listeners.
	 */
	mxGraphActions.getSelectNextAction().setEnabled(false);
	mxGraphActions.getSelectPreviousAction().setEnabled(false);
	mxGraphActions.getSelectChildAction().setEnabled(false);
	mxGraphActions.getSelectParentAction().setEnabled(false);
	
	// Undo / Redo capabilities
	getModel().addListener(mxEvent.UNDO, undoHandler);
	getView().addListener(mxEvent.UNDO, undoHandler);

	// Keeps the selection in sync with the command history

	undoManager.addListener(mxEvent.UNDO, selectionHandler);
	undoManager.addListener(mxEvent.REDO, selectionHandler);

	component = new mxGraphComponent(this);

	// Adds rubberband selection
	new mxRubberband(component);

	// Modified property change
	getModel().addListener(mxEvent.CHANGE, changeTracker);

	// addKeyListener(new XcosShortCut());
	// setMarqueeHandler(new XcosPortAction());
	// getGraphLayoutCache().setFactory(new DiagrammFactory());
	// setPortsScaled(true);
	// setVisible(true);
	// Control-drag should clone selection
	// this.setCloneable(true);
	// this.setPortsVisible(true);

	// Enable edit without final RETURN keystroke
	// this.setInvokesStopCellEditing(true);
    }

    public String getSavedFile() {
	return savedFile;
    }

    public void setSavedFile(String savedFile) {
	this.savedFile = savedFile;
    }

    public boolean isModified() {
	return modified;
    }

    public void setModified(boolean modified) {
	boolean oldValue = this.modified;
	this.modified = modified;

	getAsComponent().firePropertyChange("modified", oldValue, modified);
    }

    public void setTitle(String title) {
	this.title = title;
    }

    public String getTitle() {
	return title;
    }

    public mxGraphComponent getAsComponent() {
	return component;
    }

    public void undo() {
	decrementUndoCounter();
	redoInAction = true;
	undoManager.undo();
	redoInAction = false;
    }

    public void redo() {
	incrementUndoCounter();
	redoInAction = true;
	undoManager.redo();
	redoInAction = false;
    }

    private void incrementUndoCounter() {
	if (undoCounter < Integer.MAX_VALUE) {
	    undoCounter++;
	}
    }

    private void decrementUndoCounter() {
	if (undoCounter > Integer.MIN_VALUE) {
	    undoCounter--;
	}
    }

    protected boolean isZeroUndoCounter() {
	return (undoCounter == 0);
    }

    protected void resetUndoCounter() {
	undoCounter = 0;
    }

    public void zoom() {
	// this.setScale(2 * this.getScale());
    }

    public void unzoom() {
	// this.setScale(this.getScale() / 2);
    }

    public void delete() {
	// if (!isSelectionEmpty()) {
	// getModel().remove(getDescendants(getSelectionCells()));
	// }
    }

    public Tab getParentTab() {
	return parentTab;
    }

    public void setParentTab(Tab parentTab) {
	this.parentTab = parentTab;
    }

    public void setOpened(boolean opened) {
	this.opened = opened;
    }

    public boolean isOpened() {
	return opened;
    }

    public void setVisible(boolean visible) {
	if (parentTab != null) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper
		    .getCorrespondingUIElement(parentTab.getParentWindowId());
	    xcosWindow.setVisible(visible);
	}
    }

    public boolean isVisible() {
	if (parentTab != null) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper
		    .getCorrespondingUIElement(parentTab.getParentWindowId());
	    return xcosWindow.isVisible();
	}

	return false;
    }
}
