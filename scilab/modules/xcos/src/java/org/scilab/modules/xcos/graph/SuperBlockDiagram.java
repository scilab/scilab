/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
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

/**
 * @author Antoine ELIAS
 *
 */
public final class SuperBlockDiagram extends XcosDiagram implements Serializable {

    private static final long serialVersionUID = -402918614723713301L;
    private SuperBlock container;

    /**
     * Constructor
     */
    public SuperBlockDiagram() {
	super();
    }

    /**
     * @param superBlock parent super block
     */
    public SuperBlockDiagram(SuperBlock superBlock) {
	super();
	this.container = superBlock;
    }

    /**
     * @return parent super block
     */
    public SuperBlock getContainer() {
	return container;
    }

    /**
     * @param container parent super block
     */
    public void setContainer(SuperBlock container) {
	this.container = container;
    }

    /**
     * Concatenate the context with the parent one
     * @return the context
     * @see org.scilab.modules.xcos.graph.XcosDiagram#getContext()
     */
    @Override
    public String[] getContext() {
    	final String[] parent = getContainer().getParentDiagram().getContext();
    	final String[] current = super.getContext();
    	
    	String[] full = new String[current.length + parent.length];
    	System.arraycopy(parent, 0, full, 0, parent.length);
    	System.arraycopy(current, 0, full, parent.length, current.length);
    	return full;
    }
    
    /**
     * Listener for SuperBlock diagram events.
     */
    private static final class GenericSuperBlockListener implements mxIEventListener {
	private static GenericSuperBlockListener instance;
	
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
	
	/**
	 * Update the IOPorts colors and values.
	 * @param arg0 the source
	 * @param arg1 the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object arg0, mxEventObject arg1) {
	    ((SuperBlockDiagram) arg0).getContainer().updateAllBlocksColor();
	    ((SuperBlockDiagram) arg0).getContainer().updateExportedPort();
	}
    }
    
    /**
     * Install the specific listeners for {@link SuperBlockDiagram}.
     */
    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.CELLS_REMOVED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.IO_PORT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());
    }

    /**
     * This function set the SuperBlock diagram and all its parents in a 
     * modified state or not.
     * @param modified status
     */
	@Override
    public void setModified(boolean modified) {
        super.setModified(modified);

        if (getContainer() != null
        		&& getContainer().getParentDiagram() != null) {
            getContainer().getParentDiagram().setModified(modified);
        }
    }
    
    /**
     * This function set the SuperBlock diagram in a modified state or not.
     * 
     * It doesn't perform recursively on the parent diagrams. If you want such
     * a behavior use setModified instead.
     * @param modified status
     * @see setModified
     */
    public void setModifiedNonRecursively(boolean modified) {
	super.setModified(modified);
    }
}
