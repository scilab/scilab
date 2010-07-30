/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block.io;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxEventObject;

/**
 * Common class for the SuperBlock I/O blocks (represent ports)
 */
public abstract class ContextUpdate extends BasicBlock {

	private static final long serialVersionUID = 6076826729067963560L;

	/**
	 * Implement a listener which update the value and refresh the view when the
	 * index of the port change.
	 */
	private static final class IndexChangeAdapter implements PropertyChangeListener, Serializable {
		private static IndexChangeAdapter instance;
		
		/**
		 * Default constructor.
		 */
		private IndexChangeAdapter() { }
		
		/**
		 * @return the instance
		 */
		public static synchronized IndexChangeAdapter getInstance() {
			if (instance == null) {
				instance = new IndexChangeAdapter();
			}
			return instance;
		}
		
		/**
		 * Update the value and refresh the graph view.
		 * 
		 * @param evt the event
		 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
		 */
		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			ScilabType data = (ScilabType) evt.getNewValue();
			ContextUpdate ioBlock = (ContextUpdate) evt.getSource();
			
			if (!data.isEmpty()) {
				int newIndex = (int) ((ScilabDouble) data).getRealPart()[0][0];
				
				int oldIndex;
				if (evt.getOldValue() instanceof ScilabDouble
						&& !((ScilabDouble) evt.getOldValue()).isEmpty()) {
					oldIndex = (int) ((ScilabDouble) evt.getOldValue()).getRealPart()[0][0];
				} else {
					oldIndex = -1;
				}
				
				ioBlock.setValue(newIndex);
				
				if (ioBlock.getParentDiagram() != null) {
					ioBlock.getParentDiagram().fireEvent(
							new mxEventObject(XcosEvent.IO_PORT_VALUE_UPDATED,
									"block", ioBlock, "oldIndex", oldIndex,
									"newIndex", newIndex));
				}
			}
		}
		
	}
	
	/**
	 * This enum represent all the subclasses of ContextUpdate .
	 * 
	 * It is used to easily loop over a BasicBlock I/O blocks
	 */
	public enum IOBlocks {
		/** Map a control port to an event input block */
		EventInBlock(EventInBlock.class, ControlPort.class),
		/** Map a command port to an event output block */
		EventOutBlock(EventOutBlock.class, CommandPort.class),
		/** Map an explicit input port to an explicit input block */
		ExplicitInBlock(ExplicitInBlock.class, ExplicitInputPort.class),
		/** Map an explicit output port to an explicit output block */
		ExplicitOutBlock(ExplicitOutBlock.class, ExplicitOutputPort.class),
		/** Map an implicit input port to an implicit input block */
		ImplicitInBlock(ImplicitInBlock.class, ImplicitInputPort.class),
		/** Map an implicit output port to an implicit output block */
		ImplicitOutBlock(ImplicitOutBlock.class, ImplicitOutputPort.class);

		private final Class< ? extends ContextUpdate> ioBlock;
		private final Class< ? extends BasicPort> port;

		/**
		 * @param ioBlock
		 *            input/output block
		 * @param port
		 *            the associated port class
		 */
		private IOBlocks(Class< ? extends ContextUpdate> ioBlock,
				Class< ? extends BasicPort> port) {
			this.ioBlock = ioBlock;
			this.port = port;
		}
		
		/**
		 * Get all the port of the SuperBlock parent.
		 * 
		 * @param parent
		 *            the parent
		 * @return the port list mapped by port type
		 */
		public static Map<IOBlocks, List<mxICell>> getAllPorts(SuperBlock parent) {
			final EnumMap<IOBlocks, List<mxICell>> ret = new EnumMap<IOBlocks, List<mxICell>>(
					IOBlocks.class);

			/* Allocation */
			for (IOBlocks b : IOBlocks.values()) {
				ret.put(b, new ArrayList<mxICell>());
			}

			/* Loop all over the children */
			final int childCount = parent.getChildCount();

			for (int i = 0; i < childCount; i++) {
				final mxICell child = parent.getChildAt(i);

				/* if compatible add it to the list */
				for (IOBlocks b : IOBlocks.values()) {
					if (child.getClass().equals(b.getReferencedPortClass())) {
						ret.get(b).add(child);
					}
				}
			}

			return ret;
		}

		/**
		 * Get all the I/O blocks of the SuperBlock parent.
		 * 
		 * @param parent
		 *            the parent
		 * @return the port list mapped by port type
		 */
		public static Map<IOBlocks, List<mxICell>> getAllBlocks(SuperBlock parent) {
			final EnumMap<IOBlocks, List<mxICell>> ret = new EnumMap<IOBlocks, List<mxICell>>(
					IOBlocks.class);

			final SuperBlockDiagram graph = parent.getChild();
			if (graph == null) {
				parent.createChildDiagram();
			}
			
			/* Allocation */
			for (IOBlocks b : IOBlocks.values()) {
				ret.put(b, new ArrayList<mxICell>());
			}

			/* Loop all over the children */
			final Object defaultParent = graph.getDefaultParent();
			final mxIGraphModel defaultModel = graph.getModel();
			final Object[] children = mxGraphModel.getChildCells(
					defaultModel, defaultParent, true, false);
			
			for (int i = 0; i < children.length; i++) {
				final mxICell child = (mxICell) children[i];

				/* if compatible add it to the list */
				for (IOBlocks b : IOBlocks.values()) {
					if (child.getClass().equals(b.getReferencedClass())) {
						ret.get(b).add(child);
					}
				}
			}

			return ret;
		}

		/**
		 * @return referenced class
		 */
		public Class< ? extends ContextUpdate> getReferencedClass() {
			return ioBlock;
		}

		/**
		 * @return the port referenced class
		 */
		public Class< ? extends BasicPort> getReferencedPortClass() {
			return port;
		}
	}
	
	/**
	 * Constructor.
	 */
	public ContextUpdate() {
		super();
		
		getParametersPCS().addPropertyChangeListener("integerParameters",
				IndexChangeAdapter.getInstance());
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setNbZerosCrossing(new ScilabDouble(0));
		setNmode(new ScilabDouble(0));
		setODState(new ScilabList());
		setValue(1);
	}

	/**
	 * @param context
	 *            new context
	 */
	public void onContextChange(String[] context) {
		// prevent to open twice
		if (isLocked()) {
			return;
		}

		final File tempOutput;
		final File tempInput;
		final File tempContext;
		try {
			tempInput = FileUtils.createTempFile();
			tempInput.deleteOnExit();

			// Write scs_m
			tempOutput = exportBlockStruct();
			// Write context
			tempContext = exportContext(context);

			String cmd = ScilabInterpreterManagement.buildCall("xcosBlockEval",
					tempOutput.getAbsolutePath(), tempInput.getAbsolutePath(),
					getInterfaceFunctionName().toCharArray(), tempContext.getAbsolutePath());

			try {
				ScilabInterpreterManagement.synchronousScilabExec(cmd);
			} catch (InterpreterException e) {
				e.printStackTrace();
			}
			BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
			updateBlockSettings(modifiedBlock);

		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
