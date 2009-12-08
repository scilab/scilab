/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.io.Serializable;

import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

public class SuperBlockDiagram extends XcosDiagram implements Serializable {

    private static final long serialVersionUID = -402918614723713301L;
    private SuperBlock container = null;

    public SuperBlockDiagram() {
	super();
    }

    public SuperBlockDiagram(SuperBlock superBlock) {
	super();
	this.container = superBlock;
    }

    public SuperBlock getContainer() {
	return container;
    }

    public void setContainer(SuperBlock container) {
	this.container = container;
    }

    public String[] buildEntireContext() {
	
	String[] parentContext = getContainer().getParentDiagram().buildEntireContext();
	String []entireContext = new String[getContext().length + parentContext.length];
	
	for (int i = 0 ; i < parentContext.length ; ++i) {
	    entireContext[i] = parentContext[i];
	}

	for (int i = 0 ; i < getContext().length ; ++i) {
	    entireContext[i + parentContext.length] = getContext()[i];
	}
	
	return entireContext;
    }
    
    public void closeDiagram() {
	getContainer().closeBlockSettings();
    }

    public class GenericSuperBlockListener implements mxIEventListener {
	public void invoke(Object arg0, mxEventObject arg1) {
	    getContainer().updateAllBlocksColor();
	    getContainer().updateExportedPort();	    
	}	
    }
    
    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, new GenericSuperBlockListener());

	addListener(XcosEvent.CELLS_REMOVED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, new GenericSuperBlockListener());

	addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, new GenericSuperBlockListener());
    }

    public void setModified(boolean modified) {
        super.setModified(modified);
        /* FIXME: The modified state must not be passed to the parent when :
         * 	- We are closing the diagram and thus saving it (child is not modified but parent is modified)
         * 	- When opening a superblock and the parent is modified, then the parent is modified and child not
         *      - When opening a superblock and the parent is not modified, then the parent neither child are not modified
         *      
         * But in general, when editing, the modified state is shared (thus when modifying the child the parent is set to modified)   
         */
        if (getContainer() != null &&
        	getContainer().getParentDiagram() != null) {
            getContainer().getParentDiagram().setModified(modified);
        }
    }
    
}
