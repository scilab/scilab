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

package org.scilab.modules.xcos.io;

import java.util.ArrayList;
import java.util.EnumMap;
import java.util.List;
import java.util.Map;

import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxICell;

/**
 * Convert BasicBlock pure objects to a mixed BasicBlock objects (update the
 * scicos informations)
 */
public final class BasicBlockInfo {
// As this is the mapping string according to the Scicos simulator, it is
// better to keep them as raw vectors instead of playing with reference
// name.
// CSOFF: MultipleStringLiterals
	private static final String[] SCICOS_OBJ_FIELDS = {"Block", "graphics",
			"model", "gui", "doc" };
	private static final String[] SCICOS_MODEL_FIELDS = {"model", "sim", "in",
			"in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
			"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype",
			"firing", "dep_ut", "label", "nzcross", "nmode", "equations" };
	private static final String[] SCICOS_GRAPHIC_FIELDS = {"graphics", "orig",
			"sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout",
			"gr_i", "id", "in_implicit", "out_implicit" };
// CSON: MultipleStringLiterals

	private static final int GRAPHICS_INSTRUCTION_SIZE = 8;
	
    /**
     * This class is a static singleton.
     */
    private BasicBlockInfo() { }

    /**
     * Get all the ids.
     * @param ports the ports
     * @return array of links id
     */
    public static ScilabDouble getAllLinkId(List< ? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0; i < ports.size(); ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getConnectedLinkId();
	}

	return new ScilabDouble(data);
    }


    /**
     * Get all the port data lines.
     * @param ports the ports
     * @return array of ports data lines
     */
    public static ScilabDouble getAllPortsDataLines(List< ? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0; i < ports.size(); ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataLines();
	}

	return new ScilabDouble(data);
    }

    /**
     * Get all the port data columns
     * @param ports the ports
     * @return array of ports data columns
     */
    public static ScilabDouble getAllPortsDataColumns(List< ? extends BasicPort> ports) {
	boolean allZeros = true;
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0; i < ports.size(); ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataColumns();
	    if (data[i][0] != 0) {
		allZeros = false;
	    }
	}

	if (allZeros) {
	    return new ScilabDouble();
	}

	return new ScilabDouble(data);
    }

    /**
     * Get all the port data type.
     * @param ports the ports
     * @return array of ports data type
     */
    public static ScilabType getAllPortsDataType(List< ? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0; i < ports.size(); ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataType().getAsDouble();
	}

	return new ScilabDouble(data);
    }

    /**
     * Get all the ports types
     * @param ports the ports
     * @return array of ports type
     */
    public static ScilabType getAllPortsType(List< ? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	String[][] data = new String[ports.size()][1];
	for (int i = 0; i < ports.size(); ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getType().getAsString();
	}

	return new ScilabString(data);
    }

    /**
     * Create a graphic property from block parameters.
     * @param block the current block.
     * @return graphic structure of given block
     */
    public static ScilabMList createScilabGraphicsProperties(BasicBlock block) {
	ScilabMList graphics = new ScilabMList(SCICOS_GRAPHIC_FIELDS);

	List<InputPort> inPorts = getAllTypedPorts(block, false, InputPort.class);
	List<OutputPort> outPorts = getAllTypedPorts(block, false, OutputPort.class);
	
	// Adjust block cause Scilab(0,0) is bottom left
	double y = block.getGeometry().getY() + block.getGeometry().getHeight();
	
	double[][] orig = {{block.getGeometry().getX(), -y}};
	graphics.add(new ScilabDouble(orig)); // orig

	double[][] sz = {{block.getGeometry().getWidth(), block.getGeometry().getHeight()}};
	graphics.add(new ScilabDouble(sz)); // sz

	//    	graphics.add(new ScilabBoolean(!flip)); // flip
	graphics.add(new ScilabBoolean(true)); // flip

	graphics.add(new ScilabDouble(0)); // theta

	graphics.add(block.getExprs()); // exprs

	graphics.add(getAllLinkId(inPorts)); // pin

	graphics.add(getAllLinkId(outPorts)); // pout

	graphics.add(getAllLinkId(getAllTypedPorts(block, false, ControlPort.class))); // pein

	graphics.add(getAllLinkId(getAllTypedPorts(block, false, CommandPort.class))); // peout

	ScilabList graphicsInstructionList = new ScilabList();
	ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\"" + block.getInterfaceFunctionName() + "\",sz(1),sz(2));");
	graphicsInstructionList.add(graphicsInstructions);
	graphicsInstructionList.add(new ScilabDouble(GRAPHICS_INSTRUCTION_SIZE));
	graphics.add(graphicsInstructionList); // gr_i

	graphics.add(new ScilabString("")); // id

	graphics.add(getAllPortsType(inPorts)); // in_implicit

	graphics.add(getAllPortsType(outPorts)); // out_implicit

	return graphics;
    }


    /**
     * Create a graphic property from block parameters.
     * @param block the current block
     * @return model structure of given block
     */
    public static ScilabMList createScilabModelProperties(BasicBlock block) {
	ScilabMList model = new ScilabMList(SCICOS_MODEL_FIELDS);
	
	List<InputPort> inPorts = getAllTypedPorts(block, false, InputPort.class);
	List<OutputPort> outPorts = getAllTypedPorts(block, false, OutputPort.class);
	List<ControlPort> ctrlPorts = getAllTypedPorts(block, false, ControlPort.class);
	List<CommandPort> commandPorts = getAllTypedPorts(block, false, CommandPort.class);

	model.add(block.getSimulationFunctionNameAndType()); // sim

	model.add(getAllPortsDataLines(inPorts)); // in

	model.add(getAllPortsDataColumns(inPorts)); // in2

	model.add(getAllPortsDataType(inPorts)); // intyp

	model.add(getAllPortsDataLines(outPorts)); // out

	model.add(getAllPortsDataColumns(outPorts)); // out2

	model.add(getAllPortsDataType(outPorts)); // outtyp

	model.add(getAllPortsDataLines(ctrlPorts)); // evtin

	model.add(getAllPortsDataLines(commandPorts)); // evtout

	model.add(block.getState()); // state

	model.add(block.getDState()); // dstate

	model.add(block.getODState()); // odstate

	model.add(block.getRealParameters()); // rpar

	model.add(block.getIntegerParameters()); // ipar

	model.add(block.getObjectsParameters()); // opar

	model.add(new ScilabString(block.getBlockType())); // blocktype

	model.add(block.getAllCommandPortsInitialStates()); // firing

	boolean[][] dependsOnUandT = {{block.isDependsOnU() , block.isDependsOnT()}};
	model.add(new ScilabBoolean(dependsOnUandT)); // dep_ut

	model.add(new ScilabString("")); // label

	model.add(block.getNbZerosCrossing()); // nzcross

	model.add(block.getNmode()); // nmode

	if (block.getEquations() == null) {
	    model.add(new ScilabList()); // equations
	} else {
	    model.add(block.getEquations()); // equations
	}

	return model;
    }

    /**
     * Create the Scilab gui properties from block parameters
     * @param block the current block
     * @return gui structure of given block
     */
    public static ScilabString createScilabGuiProperties(BasicBlock block) {
	return new ScilabString(block.getInterfaceFunctionName());
    }

    /**
     * Create the Scilab doc properties from block parameters
     * @param block the current block
     * @return doc structure of given block
     */
    public static ScilabList createScilabDocProperties(BasicBlock block) {
	ScilabList result = new ScilabList();
	// Store UID in doc so that Scilab will now it without being disturbed.
	result.add(new ScilabString(block.getId()));

	return result;
    }

    /**
     * Create the Scilab block representation
     * @param block the current block
     * @return Scilab structure of given block
     */
    public static ScilabMList getAsScilabObj(BasicBlock block) {
	ScilabMList obj = new ScilabMList(SCICOS_OBJ_FIELDS);

	obj.add(BasicBlockInfo.createScilabGraphicsProperties(block));
	obj.add(BasicBlockInfo.createScilabModelProperties(block));
	obj.add(BasicBlockInfo.createScilabGuiProperties(block));
	obj.add(BasicBlockInfo.createScilabDocProperties(block));

	return obj;
    }
    
	/**
	 * @param block
	 *            the block we are working on
	 * @param revert
	 *            True if the returned list have to be in a reversed order or
	 * @param type
	 *            the class instance to search for
	 * @param <T>
	 *            The type to search for.
	 * @return control ports of given block
	 */
	public static <T extends BasicPort> List<T> getAllTypedPorts(
			BasicBlock block, boolean revert, Class<T> type) {
		List<T> data = new ArrayList<T>();
		int childrenCount = block.getChildCount();

		for (int i = 0; i < childrenCount; ++i) {
			mxICell cell = block.getChildAt(i);
			if (type.isInstance(cell)) {
				// There we are sure that the cell is an instance of the type
				// class. Thus we can safely cast it and add it to the result
				// vector.
				if (revert) {
					data.add(0, (T) cell);
				} else {
					data.add((T) cell);
				}
			}
		}

		return data;
	}
    
    /**
     * Get the associated port ordered by orientation.
     * @param block The block we are working on
     * @return Lists of ports where key are BasicPort.Orientation
     */
    public static Map<Orientation, List<BasicPort>> getAllOrientedPorts(BasicBlock block) {
    	EnumMap<Orientation, List<BasicPort>> map = new EnumMap<Orientation, List<BasicPort>>(Orientation.class);
    	List<BasicPort> northPorts = new ArrayList<BasicPort>();
    	List<BasicPort> southPorts = new ArrayList<BasicPort>();
    	List<BasicPort> eastPorts = new ArrayList<BasicPort>();
    	List<BasicPort> westPorts = new ArrayList<BasicPort>();
    	
		final int childrenCount = block.getChildCount();
		for (int i = 0; i < childrenCount; ++i) {
			BasicPort port = (BasicPort) block.getChildAt(i);
			switch (port.getOrientation()) {
			case NORTH:
				northPorts.add(port);
				break;
			case SOUTH:
				southPorts.add(port);
				break;
			case EAST:
				eastPorts.add(port);
				break;
			case WEST:
				westPorts.add(port);
				break;
			default:
				break;
			}
		}
    	
    	map.put(Orientation.NORTH, northPorts);
    	map.put(Orientation.SOUTH, southPorts);
    	map.put(Orientation.EAST, eastPorts);
    	map.put(Orientation.WEST, westPorts);
    	
    	return map;
    }
}
