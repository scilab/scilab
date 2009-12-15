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

import java.awt.Color;
import java.util.List;

import org.scilab.modules.graph.utils.ScilabGraphMessages;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.utils.XcosComponent;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.swing.handler.mxKeyboardHandler;
import com.mxgraph.swing.handler.mxRubberband;
import com.mxgraph.swing.util.mxGraphActions;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUndoManager;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.view.mxGraph;

/**
 * Represent the base diagram of Xcos.
 *
 * It performs generic operations like undo/redo management, action clean-up,
 * modification state management, Tab association, etc...
 */
public class ScilabGraph extends mxGraph {

    protected mxUndoManager undoManager = new mxUndoManager();
    protected mxGraphOutline graphOutline = null;
    protected mxKeyboardHandler keyboardHandler = null;
    protected XcosComponent component = null;
    //    protected mxGraphComponent component = null;

    private String title = ScilabGraphMessages.UNTITLED;
    private String savedFile = null;
    private boolean modified = false;
    private Tab parentTab;
    private boolean opened = false;
    private boolean redoInAction = false;
    private int undoCounter = 0;
    private boolean readOnly = false;
    private Color originalColor = null;
    private transient mxRubberband rubberBand;

    /**
     * Manage the modification state on change
     */
    protected mxIEventListener changeTracker = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {
	    setModified(true);
	}
    };

    /**
     * Manage the undo/redo on change
     */
    protected mxIEventListener undoHandler = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {

	    if (!redoInAction) {
		undoManager.undoableEditHappened((mxUndoableEdit) evt.getArgAt(0));
		incrementUndoCounter();
	    }
	}
    };

    /**
     * Manage the selection on change
     */
    mxIEventListener selectionHandler = new mxIEventListener() {
	public void invoke(Object source, mxEventObject evt) {
	    List<mxUndoableChange> changes = ((mxUndoableEdit) evt.getArgAt(0)).getChanges();
	    setSelectionCells(getSelectionCellsForChanges(changes));
	}
    };

    /**
     * Default constructor:
     *     - disable unused actions
     *     - install listeners
     *     - Replace JGraphX components by specialized components if needed.
     */
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

	component = new XcosComponent(this);
	//	component = new mxGraphComponent(this);

	// Adds rubberband selection
	rubberBand = new mxRubberband(component);

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

    /**
     * @return The previously saved file or null.
     */
    public String getSavedFile() {
	return savedFile;
    }

    /**
     * @param savedFile The new saved file
     */
    public void setSavedFile(String savedFile) {
	this.savedFile = savedFile;
    }

    /**
     * @return true, if the graph has been modified ; false otherwise.  
     */
    public boolean isModified() {
	return modified;
    }

    /**
     * Modify the state of the diagram.
     * @param modified The new modified state. 
     * @category UseEvent
     */
    public void setModified(boolean modified) {
	boolean oldValue = this.modified;
	this.modified = modified;

	getAsComponent().firePropertyChange("modified", oldValue, modified);
    }

    /**
     * @param title The new title of the tab
     */
    public void setTitle(String title) {
	this.title = title;
    }

    /**
     * @return The current Tab title
     */
    public String getTitle() {
	return title;
    }

    /**
     * @return The component associated with the current graph.
     */
    public mxGraphComponent getAsComponent() {
	return component;
    }

    /**
     * Undo the last action
     * @see com.mxgraph.util.mxUndoManager
     */
    public void undo() {
	decrementUndoCounter();
	redoInAction = true;
	undoManager.undo();
	redoInAction = false;
    }

    /**
     * Redo the last action
     * com.mxgraph.util.mxUndoManager
     */
    public void redo() {
	incrementUndoCounter();
	redoInAction = true;
	undoManager.redo();
	redoInAction = false;
    }

    /**
     * Used internally to manage the modified state on undo/redo
     */
    private void incrementUndoCounter() {
	if (undoCounter < Integer.MAX_VALUE) {
	    undoCounter++;
	}
    }

    /**
     * Used internally to manage the modified state on undo/redo
     */
    private void decrementUndoCounter() {
	if (undoCounter > Integer.MIN_VALUE) {
	    undoCounter--;
	}
    }

    /**
     * Used internally to manage the modified state on undo/redo
     */
    protected boolean isZeroUndoCounter() {
	return (undoCounter == 0);
    }

    /**
     * Used internally to manage the modified state on undo/redo
     */
    protected void resetUndoCounter() {
	undoCounter = 0;
    }

    @Deprecated
    public void zoom() {
	// this.setScale(2 * this.getScale());
    }

    @Deprecated
    public void unzoom() {
	// this.setScale(this.getScale() / 2);
    }

    @Deprecated
    public void delete() {
	// if (!isSelectionEmpty()) {
	// getModel().remove(getDescendants(getSelectionCells()));
	// }
    }

    /**
     * @return The associated Tab
     */
    public Tab getParentTab() {
	return parentTab;
    }

    /**
     * @param parentTab The new associated Tab
     */
    public void setParentTab(Tab parentTab) {
	this.parentTab = parentTab;
    }

    /**
     * The instance can be not visible but used (when using SuperBlock). The
     * openned flag is true in this case and also when the Window/Tab is
     * visible.
     * @param opened Openned state 
     */
    public void setOpened(boolean opened) {
	this.opened = opened;
    }

    /**
     * @return Openned state
     */
    public boolean isOpened() {
	return opened;
    }

    /**
     * Set the associated Window/Tab visible or not.
     * @param visible State of visibility
     */
    public void setVisible(boolean visible) {
	if (parentTab != null) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(parentTab.getParentWindowId());
	    xcosWindow.setVisible(visible);
	}
    }

    /**
     * Check if the associated Window/Tab is visible 
     * @return State of visibility
     */
    public boolean isVisible() {
	if (parentTab != null) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(parentTab.getParentWindowId());
	    return xcosWindow.isVisible();
	}

	return false;
    }

    /**
     * A read-only state will disable all actions in the graph.
     * @param readOnly Read-only state
     */
    public void setReadOnly(boolean readOnly) {
	this.readOnly = readOnly;
	
	setCellsLocked(readOnly);
	if(isReadonly()) {
	    setOriginalColor(getAsComponent().getBackground());
	    getAsComponent().setBackground(new Color(240, 240, 240));
	} else {
	    getAsComponent().setBackground(getOriginalColor());
	}
    }

    /**
     * @return True if actions are not allowed, false otherwise.
     */
    public boolean isReadonly() {
	return readOnly;
    }

    /**
     * Useful function for the read-only property
     */
    private void setOriginalColor(Color originalColor) {
	this.originalColor = originalColor;
    }

    /**
     * Useful function for the read-only property
     */
    private Color getOriginalColor() {
	if(originalColor != null){
	    return originalColor;
	}
	return Color.WHITE;
    }

    /**
     * @return The associated RubberBand
     * @see com.mxgraph.swing.handler.mxRubberband
     */
    public mxRubberband getRubberBand() {
        return rubberBand;
    }
}
