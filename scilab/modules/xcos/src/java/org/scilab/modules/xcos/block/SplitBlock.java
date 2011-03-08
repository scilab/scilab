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
package org.scilab.modules.xcos.block;

import java.util.Arrays;
import java.util.List;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;

/**
 * A SplitBlock is used on a junction between links.
 */
public final class SplitBlock extends BasicBlock {

	/** The default size */
	public static final int DEFAULT_SIZE = 7;
	/** The default color value */
	public static final int DEFAULT_COLOR = 7;

	private static final long serialVersionUID = 5817243367840540106L;

	/**
	 * Constructor
	 */
	public SplitBlock() {
		super();
	}

	/**
	 * Add connection port depending on the type of the source.
	 * @param source the type of the split
	 */
	public void addConnection(BasicPort source) {
		if (source.getType() == Type.EXPLICIT) {
			addPort(new ExplicitInputPort());
			addPort(new ExplicitOutputPort());
			addPort(new ExplicitOutputPort());
		} else if (source.getType() == Type.IMPLICIT) {
			addPort(new ImplicitInputPort());
			addPort(new ImplicitOutputPort());
			addPort(new ImplicitOutputPort());
		} else {
			addPort(new ControlPort());
			addPort(new CommandPort());
			addPort(new CommandPort());
		}

		getChildAt(0).setVisible(false);
		getChildAt(1).setVisible(false);
		getChildAt(2).setVisible(false);
	}

	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setInterfaceFunctionName("SPLIT_f");
		setStyle(getInterfaceFunctionName());
		setSimulationFunctionName("lsplit");
		setRealParameters(new ScilabDouble());
		setIntegerParameters(new ScilabDouble());
		setObjectsParameters(new ScilabList());
		setExprs(new ScilabDouble());
	}

	/**
	 * Add a port on the block.
	 * 
	 * @param child the port to add
	 * @param index index where to put the child
	 */
	@Override
	public mxICell insert(mxICell child, int index) {
		child.setVisible(false);
		return super.insert(child, index);
	}
	
	/**
	 * @return input port
	 */
	@SuppressWarnings("unchecked")
	public BasicPort getIn() {
		return getChild(0, Arrays.asList(ExplicitInputPort.class,ImplicitInputPort.class, ControlPort.class), 1);
	}

	/**
	 * @return first output port
	 */
	@SuppressWarnings("unchecked")
	public BasicPort getOut1() {
		return getChild(1, Arrays.asList(ExplicitOutputPort.class, ImplicitOutputPort.class, CommandPort.class), 1);
	}

	/**
	 * @return second output port
	 */
	@SuppressWarnings("unchecked")
	public BasicPort getOut2() {
		return getChild(2, Arrays.asList(ExplicitOutputPort.class, ImplicitOutputPort.class, CommandPort.class), 2);
	}

	/**
	 * Get the child of the kind class from the start to a count.
	 * @param startIndex the start index (default position)
	 * @param kind the kind of the port
	 * @param ordering the ordering of the port
	 * @return the found port or null.
	 */
	private BasicPort getChild(int startIndex, List<Class<? extends BasicPort>> kind, int ordering) {
		final int size = children.size();

		int loopCount = size;
		for (int i = startIndex; loopCount > 0; i = (i + 1) % size, loopCount--) {
			Object child = children.get(i);
			for (Class<? extends BasicPort> klass : kind) {
				if (klass.isInstance(child)) {
					BasicPort port = klass.cast(child);

					if (port.getOrdering() == ordering) {
						// end of the loop
						return klass.cast(child);
					}
				}
			}
		}
		LogFactory.getLog(SplitBlock.class).error("Unable to find a child.");
		return null;
	}

	/**
	 * delete split block child before delete
	 */
	public void unlinkAndClean() {
		XcosDiagram graph = getParentDiagram();
		if (graph == null) {
			setParentDiagram(Xcos.findParent(this));
			graph = getParentDiagram();
			LogFactory.getLog(getClass()).error("Parent diagram was null");
		}
		
		Object[] objs = graph.getAllEdges(
				new Object[] {getChildAt(0), getChildAt(1), getChildAt(2)});
		getParentDiagram().getModel().beginUpdate();
		for (Object obj : objs) {
			if (obj instanceof BasicLink) {
				BasicLink link = (BasicLink) obj;
				graph.getModel().remove(link);
			}
		}
		graph.getModel().endUpdate();
	}

	/**
	 * Set the geometry of the block
	 *
	 * @param geometry
	 *            change split block geometry
	 */
	@Override
	public void setGeometry(mxGeometry geometry) {
		if (geometry != null) {
			geometry.setWidth(DEFAULT_SIZE);
			geometry.setHeight(DEFAULT_SIZE);

			/*
			 * Align the geometry on the grid
			 */
			if (getParentDiagram() != null && getParentDiagram().isGridEnabled()) {
				final double cx = getParentDiagram().snap(geometry.getCenterX());
				final double cy = getParentDiagram().snap(geometry.getCenterY());
				
				geometry.setX(cx - (DEFAULT_SIZE / 2));
				geometry.setY(cy - (DEFAULT_SIZE / 2));
			}
		}

		super.setGeometry(geometry);
	}

	/**
	 * @return true if the split is connectable, false otherwise
	 * @see org.scilab.modules.xcos.block.BasicBlock#isConnectable()
	 */
	@Override
	public boolean isConnectable() {
		if (getChildCount() != 0) {
			boolean hasNoEdges = true;
			for (int i = 0; i < getChildCount() && hasNoEdges; i++) {
				final mxICell cell = getChildAt(i);
				hasNoEdges = cell.getEdgeCount() == 0;
			}

			return hasNoEdges;
		} else {
			return false;
		}
	}

	/**
	 * Insert edges as children port edges
	 *
	 * @param edge the current edge
	 * @param isOutgoing if it is an input port or output one
	 * @return the inserted edges
	 * @see com.mxgraph.model.mxCell#insertEdge(com.mxgraph.model.mxICell, boolean)
	 */
	@Override
	public mxICell insertEdge(mxICell edge, boolean isOutgoing) {
		final mxICell ret;

		if (isOutgoing) {
			if (getOut1().getEdgeCount() == 0) {
				ret = getOut1().insertEdge(edge, isOutgoing);
			} else if (getOut2().getEdgeCount() == 0) {
				ret = getOut2().insertEdge(edge, isOutgoing);
			} else {
				ret = null;
			}
		} else {
			if (getIn().getEdgeCount() == 0) {
				ret = getIn().insertEdge(edge, isOutgoing);
			} else {
				ret = null;
			}
		}

		if (ret == null) {
			LogFactory.getLog(SplitBlock.class).error(
					"Unable to connect : " + edge);
			return super.insertEdge(edge, isOutgoing);
		} else {
			return ret;
		}
	}
}
