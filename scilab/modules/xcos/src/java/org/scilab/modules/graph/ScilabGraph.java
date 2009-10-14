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

import org.scilab.modules.localization.Messages;

import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.swing.mxGraphOutline;
import com.mxgraph.swing.handler.mxKeyboardHandler;
import com.mxgraph.swing.handler.mxRubberband;
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
    private boolean modified = false;

    public ScilabGraph() {
	// Undo / Redo capabilities
	getModel().addListener(mxEvent.UNDO, undoHandler);
	getView().addListener(mxEvent.UNDO, undoHandler);

	component = new mxGraphComponent(this);

	// Adds rubberband selection
	new mxRubberband(component);

	// Modified property change
	getModel().addListener(mxEvent.CHANGE, changeTracker);

	//addKeyListener(new XcosShortCut());
	//setMarqueeHandler(new XcosPortAction());
	//getGraphLayoutCache().setFactory(new DiagrammFactory());
	//setPortsScaled(true);
	//setVisible(true);
	// Control-drag should clone selection
	//this.setCloneable(true);
	//this.setPortsVisible(true);

	// Enable edit without final RETURN keystroke
	//this.setInvokesStopCellEditing(true);
    }

    /**
     * 
     */
    protected mxIEventListener changeTracker = new mxIEventListener()
    {
	public void invoke(Object source, mxEventObject evt)
	{
	    setModified(true);
	}
    };   

    protected mxIEventListener undoHandler = new mxIEventListener()
    {
	public void invoke(Object source, mxEventObject evt)
	{
	    undoManager.undoableEditHappened((mxUndoableEdit) evt.getArgAt(0));
	}
    };

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
	undoManager.undo();
    }

    public void redo() {
	undoManager.redo();
    }

    public void zoom() {
	//	this.setScale(2 * this.getScale());
    }

    public void unzoom() {
	//	this.setScale(this.getScale() / 2);
    }

    public void delete() {
	//	if (!isSelectionEmpty()) {
	//	    getModel().remove(getDescendants(getSelectionCells()));
	//	}
    }

}

