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

import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;

import com.mxgraph.model.mxGeometry;

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
	 * Connect the splitblock to a source and 2 targets.
	 * 
	 * @param source
	 *            source to be connected with
	 * @param target1
	 *            first target to be connected with
	 * @param target2
	 *            second target to be connected with
	 */
	public void setConnection(BasicPort source, BasicPort target1,
			BasicPort target2) {

		// source
		if (source instanceof ExplicitOutputPort) {
			addPort(new ExplicitInputPort());
		} else if (source instanceof ImplicitOutputPort
				|| source instanceof ImplicitInputPort) {
			addPort(new ImplicitInputPort());
		} else if (source instanceof CommandPort) {
			addPort(new ControlPort());
		}

		// target1 -> add 3 output ports
		if (target1 instanceof ExplicitInputPort) {
			addPort(new ExplicitOutputPort());
			addPort(new ExplicitOutputPort());
		} else if (target1 instanceof ImplicitOutputPort
				|| target1 instanceof ImplicitInputPort) {
			addPort(new ImplicitOutputPort());
			addPort(new ImplicitOutputPort());
		} else if (target1 instanceof ControlPort) {
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
	 * @param port
	 *            The port to be added to the block
	 * @see org.scilab.modules.xcos.block.BasicBlock#addPort(org.scilab.modules.xcos.port.BasicPort)
	 */
	@Override
	public void addPort(BasicPort port) {
		super.addPort(port);
		port.setVisible(false);
	}

	/**
	 * @return input port
	 */
	public BasicPort getIn() {
		return (BasicPort) getChildAt(0);
	}

	/**
	 * @return first output port
	 */
	public BasicPort getOut1() {
		return (BasicPort) getChildAt(1);
	}

	/**
	 * @return second ouput port
	 */
	public BasicPort getOut2() {
		return (BasicPort) getChildAt(2);
	}

	/**
	 * delete split block child before delete
	 */
	public void unlinkAndClean() {

		Object[] objs = getParentDiagram().getAllEdges(
				new Object[] {getChildAt(0), getChildAt(1), getChildAt(2)});
		getParentDiagram().getModel().beginUpdate();
		for (int i = 0; i < objs.length; i++) {
			if (objs[i] instanceof BasicLink) {
				BasicLink link = (BasicLink) objs[i];
				getParentDiagram().getModel().remove(link);
			}
		}
		getParentDiagram().getModel().endUpdate();
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
			double gridSize;
			if (getParentDiagram() != null) {
				gridSize = getParentDiagram().getGridSize();
			} else {
				gridSize = BlockPositioning.DEFAULT_GRIDSIZE;
			}
			BlockPositioning.alignPoint(geometry, gridSize,
					(geometry.getWidth() / 2.0));
		}
		
		super.setGeometry(geometry);
	}

}
