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

package org.scilab.modules.xcos.graph;

import java.io.Serializable;

import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.util.mxEventObject;

public final class SuperBlockDiagram extends XcosDiagram implements Serializable {

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

    private static class GenericSuperBlockListener implements mxIEventListener {
	static GenericSuperBlockListener instance=null;
	
	/**
	 * Reduce constructor visibility
	 */
	private GenericSuperBlockListener() {
	    super();
	}
	
	/**
	 * Mono-threaded singleton implementation getter
	 * @return The unique instance
	 */
	public static GenericSuperBlockListener getInstance() {
	    if (instance == null) {
		instance = new GenericSuperBlockListener();
	    }
	    return instance;
	}
	
	public void invoke(Object arg0, mxEventObject arg1) {
	    ((SuperBlockDiagram) arg0).getContainer().updateAllBlocksColor();
	    ((SuperBlockDiagram) arg0).getContainer().updateExportedPort();	    
	}
    }
    
    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.CELLS_REMOVED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());
    }

    /**
     * This function set the SuperBlock diagram and all its parents in a 
     * modified state or not.
     */
    public void setModified(boolean modified) {
        super.setModified(modified);

        if (getContainer() != null &&
        	getContainer().getParentDiagram() != null) {
            getContainer().getParentDiagram().setModified(modified);
        }
    }
    
    /**
     * This function set the SuperBlock diagram in a modified state or not.
     * 
     * It doesn't perform recursively on the parent diagrams. If you want such
     * a behavior use setModified instead.
     * 
     * @see setModified
     */
    public void setModifiedNonRecursively(boolean modified) {
	super.setModified(modified);
    }
    
    @Override
    public void undo() {
	super.undo();
	getContainer().updateAllBlocksColor();
	getContainer().updateExportedPort();
    }

    @Override
    public void redo() {
	super.redo();
	getContainer().updateAllBlocksColor();
	getContainer().updateExportedPort();
    }
}
