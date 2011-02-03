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
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;

/**
 * @author Antoine ELIAS
 *
 */
public final class SuperBlockDiagram extends XcosDiagram implements Serializable, Cloneable {

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
    	final SuperBlock block = getContainer();
		XcosDiagram graph = block.getParentDiagram();
		if (graph == null) {
			block.setParentDiagram(Xcos.findParent(block));
			graph = block.getParentDiagram();
			LogFactory.getLog(getClass()).error("Parent diagram was null");
		}
		
    	final String[] parent = graph.getContext();
    	final String[] current = super.getContext();
    	
    	String[] full = new String[current.length + parent.length];
    	System.arraycopy(parent, 0, full, 0, parent.length);
    	System.arraycopy(current, 0, full, parent.length, current.length);
    	return full;
    }
    
    /**
     * Listener for SuperBlock diagram events.
     */
	private static final class GenericSuperBlockListener implements
			mxIEventListener, Serializable {
		private static GenericSuperBlockListener instance;

		/**
		 * Reduce constructor visibility
		 */
		private GenericSuperBlockListener() {
			super();
		}

		/**
		 * Mono-threaded singleton implementation getter
		 * 
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
		 * 
		 * @param arg0
		 *            the source
		 * @param arg1
		 *            the event data
		 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
		 *      com.mxgraph.util.mxEventObject)
		 */
		@Override
		public void invoke(Object arg0, mxEventObject arg1) {
			final SuperBlock block = ((SuperBlockDiagram) arg0).getContainer();
			if (block != null) {
				block.updateAllBlocksColor();
				block.updateExportedPort();
			}
		}
	}
    
    private static final class LabelBlockListener implements mxIEventListener, Serializable {
    	private static final LabelBlockListener instance = new LabelBlockListener();
    	
    	/**
		 * Default Constructor
		 */
		private LabelBlockListener() {
			super();
		}
		
		/**
		 * @return the instance
		 */
		public static LabelBlockListener getInstance() {
			return instance;
		}

		/** {@inheritDoc} */
		@Override
		public void invoke(Object sender, mxEventObject evt) {
			final String value = (String) evt.getProperty("value");
			final Object parent = evt.getProperty("parent");
			if (parent instanceof ContextUpdate) {
				final ContextUpdate block = (ContextUpdate) parent;
				final ScilabDouble data = (ScilabDouble) block.getIntegerParameters();
				final int index = (int) data.getRealPart()[0][0];
				
				final SuperBlock container = ((SuperBlockDiagram) sender).getContainer();
				if (container == null) {
					return;
				}
				
				List<mxICell> tmp = IOBlocks.getPorts(container, block.getClass());
				BasicPort[] ports = new BasicPort[tmp.size()];
				Arrays.sort(tmp.toArray(ports), new Comparator<BasicPort>() {
					@Override
					public int compare(BasicPort o1, BasicPort o2) {
						return o1.getOrdering() - o2.getOrdering();
					}});
				
				XcosDiagram graph = container.getParentDiagram();
				if (graph == null) {
					container.setParentDiagram(Xcos.findParent(container));
					graph = container.getParentDiagram();
					LogFactory.getLog(getClass()).error("Parent diagram was null");
				}
				container.getParentDiagram().cellLabelChanged(ports[index - 1], value, false);
			}
		}
    }
    
    /**
     * Install the specific listeners for {@link SuperBlockDiagram}.
     */
    public void installSuperBlockListeners() {
	addListener(XcosEvent.CELLS_ADDED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.CELLS_REMOVED, GenericSuperBlockListener.getInstance());

	addListener(XcosEvent.IO_PORT_VALUE_UPDATED, GenericSuperBlockListener.getInstance());
	
	addListener(mxEvent.LABEL_CHANGED, LabelBlockListener.getInstance());
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
    
    /** {@inheritDoc}} */
    @Override
    public Object clone() throws CloneNotSupportedException {
    	final SuperBlockDiagram clone = new SuperBlockDiagram();    	
    	clone.installListeners();
    	clone.installSuperBlockListeners();
    	
    	clone.setScicosParameters((ScicosParameters) getScicosParameters().clone());
    	clone.addCells(cloneCells(getChildCells(getDefaultParent())), clone.getDefaultParent());
    	
    	return clone;
    }
}
