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
import java.util.List;

import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

public final class BasicBlockInfo {

    /**
     * 
     */
    private BasicBlockInfo() {

    }

    /**
     * @param ports : list of links
     * @return array of links id
     */
    public static ScilabDouble getAllLinkId(List<? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getConnectedLinkId();
	}

	return new ScilabDouble(data);
    }


    /**
     * @param ports
     * @return array of ports data lines
     */
    public static ScilabDouble getAllPortsDataLines(List<? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataLines();
	}

	return new ScilabDouble(data);
    }

    /**
     * @param ports
     * @return array of ports data columns
     */
    public static ScilabDouble getAllPortsDataColumns(List<? extends BasicPort> ports) {
	boolean allZeros = true;
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataColumns();
	    if(data[i][0] != 0) {
		allZeros = false;
	    }
	}

	if (allZeros) {
	    return new ScilabDouble();
	}

	return new ScilabDouble(data);
    }

    /**
     * @param ports
     * @return array of ports data type
     */
    public static ScilabType getAllPortsDataType(List<? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataType().getAsDouble();
	}

	return new ScilabDouble(data);
    }

    /**
     * @param ports
     * @return array of ports type
     */
    public static ScilabType getAllPortsType(List<? extends BasicPort> ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	String[][] data = new String[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getType().getAsString();
	}

	return new ScilabString(data);
    }

    /**
     * @param block
     * @return graphic structure of given block
     */
    public static ScilabMList createScilabGraphicsProperties(BasicBlock block) {
	String[] graphicsFields = {"graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout", "gr_i",
		"id", "in_implicit", "out_implicit"};
	ScilabMList graphics = new ScilabMList(graphicsFields);

	// Adjust block cause Scilab(0,0) is bottom left
	double y = block.getGeometry().getY() + block.getGeometry().getHeight();
	
	double[][] orig = {{block.getGeometry().getX(), - y}};
	graphics.add(new ScilabDouble(orig)); // orig

	double[][] sz = {{block.getGeometry().getWidth(), block.getGeometry().getHeight()}};
	graphics.add(new ScilabDouble(sz)); // sz

	//    	graphics.add(new ScilabBoolean(!flip)); // flip
	graphics.add(new ScilabBoolean(true)); // flip

	graphics.add(new ScilabDouble(0)); // theta

	graphics.add(block.getExprs()); // exprs

	graphics.add(getAllLinkId(getAllInputPorts(block, false))); // pin

	graphics.add(getAllLinkId(getAllOutputPorts(block, false))); // pout

	graphics.add(getAllLinkId(getAllControlPorts(block, false))); // pein

	graphics.add(getAllLinkId(getAllCommandPorts(block, false))); // peout

	ScilabList gr_i = new ScilabList();
	ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\"" + block.getInterfaceFunctionName() + "\",sz(1),sz(2));");
	gr_i.add(graphicsInstructions);
	gr_i.add(new ScilabDouble(8));
	graphics.add(gr_i); // gr_i

	graphics.add(new ScilabString("")); // id

	graphics.add(getAllPortsType(getAllInputPorts(block, false))); // in_implicit

	graphics.add(getAllPortsType(getAllOutputPorts(block, false))); // out_implicit

	return graphics;
    }


    /**
     * @param block
     * @return model structure of given block
     */
    public static ScilabMList createScilabModelProperties(BasicBlock block) {
	String[] modelFields = {"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
		"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		"nzcross", "nmode", "equations"};
	ScilabMList model = new ScilabMList(modelFields);

	model.add(block.getSimulationFunctionNameAndType()); // sim

	model.add(getAllPortsDataLines(getAllInputPorts(block, false))); // in

	model.add(getAllPortsDataColumns(getAllInputPorts(block, false))); // in2

	model.add(getAllPortsDataType(getAllInputPorts(block, false))); // intyp

	model.add(getAllPortsDataLines(getAllOutputPorts(block, false))); // out

	model.add(getAllPortsDataColumns(getAllOutputPorts(block, false))); // out2

	model.add(getAllPortsDataType(getAllOutputPorts(block, false))); // outtyp

	model.add(getAllPortsDataLines(getAllControlPorts(block, false))); // evtin

	model.add(getAllPortsDataLines(getAllCommandPorts(block, false))); // evtout

	model.add(block.getState()); // state

	model.add(block.getDState()); // dstate

	model.add(block.getODState()); // odstate

	model.add(block.getRealParameters()); // rpar

	model.add(block.getIntegerParameters()); // ipar

	model.add(block.getObjectsParameters()); // opar

	model.add(new ScilabString(block.getBlockType())); // blocktype

	model.add(block.getAllCommandPortsInitialStates()); // firing

	boolean[][] dep_ut = {{block.dependsOnU() , block.dependsOnT()}};
	model.add(new ScilabBoolean(dep_ut)); // dep_ut

	model.add(new ScilabString("")); // label

	model.add(block.getNbZerosCrossing()); // nzcross

	model.add(block.getNmode()); // nmode

	if (block.getEquations() == null) {
	    model.add(new ScilabList()); // equations
	}
	else {
	    model.add(block.getEquations()); // equations
	}

	return model;
    }

    /**
     * @param block
     * @return gui structure of given block
     */
    public static ScilabString createScilabGuiProperties(BasicBlock block) {
	return new ScilabString(block.getInterfaceFunctionName());
    }

    /**
     * @param block
     * @return doc structure of given block
     */
    public static ScilabList createScilabDocProperties(BasicBlock block) {
	ScilabList result = new ScilabList();
	// Store UID in doc so that Scilab will now it without being disturbed.
	result.add(new ScilabString(block.getId()));

	return result;
    }

    /**
     * @param block
     * @return Scilab structure of given block
     */
    public static ScilabMList getAsScilabObj(BasicBlock block) {
	String[] objFields = {"Block", "graphics", "model", "gui", "doc"};
	ScilabMList obj = new ScilabMList(objFields);

	obj.add(BasicBlockInfo.createScilabGraphicsProperties(block));
	obj.add(BasicBlockInfo.createScilabModelProperties(block));
	obj.add(BasicBlockInfo.createScilabGuiProperties(block));
	obj.add(BasicBlockInfo.createScilabDocProperties(block));

	return obj;
    }

    /**
     * @param block
     * @return input ports of given block
     */
    public static List<InputPort> getAllInputPorts(BasicBlock block, boolean revert) {
	List<InputPort> data = new ArrayList<InputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof InputPort) {
		if(revert) {
		    data.add(0, (InputPort) block.getChildAt(i));
		} else {
		    data.add((InputPort) block.getChildAt(i));
		}
	    }
	}

	return data;
    }

    /**
     * @param block
     * @return explicit input ports of given block
     */
    public static List<ExplicitInputPort> getAllExplicitInputPorts(BasicBlock block, boolean revert) {
	List<ExplicitInputPort> data = new ArrayList<ExplicitInputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof ExplicitInputPort) {
		if(revert) {
		    data.add(0, (ExplicitInputPort) block.getChildAt(i));
		} else {
		    data.add((ExplicitInputPort) block.getChildAt(i));
		}
	    }
	}
	return data;
    }

    /**
     * @param block
     * @return implicit input ports of given block
     */
    public static List<ImplicitInputPort> getAllImplicitInputPorts(BasicBlock block, boolean revert) {
	List<ImplicitInputPort> data = new ArrayList<ImplicitInputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof ImplicitInputPort) {
		if(revert) {
		    data.add(0, (ImplicitInputPort) block.getChildAt(i));
		} else {
		    data.add((ImplicitInputPort) block.getChildAt(i));
		}
	    }
	}
	return data;
    }

    /**
     * @param block
     * @return output ports of given block
     */
    public static List<OutputPort> getAllOutputPorts(BasicBlock block, boolean revert) {
	List<OutputPort> data = new ArrayList<OutputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof OutputPort) {
		if(revert) {
		    data.add(0 ,(OutputPort) block.getChildAt(i));
		} else {
		    data.add((OutputPort) block.getChildAt(i));
		}
	    }
	}

	return data;
    }

    /**
     * @param block
     * @return explicit output ports of given block
     */
    public static List<ExplicitOutputPort> getAllExplicitOutputPorts(BasicBlock block, boolean revert) {
	List<ExplicitOutputPort> data = new ArrayList<ExplicitOutputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof ExplicitOutputPort) {
		if(revert) {
		    data.add(0, (ExplicitOutputPort) block.getChildAt(i));
		} else {
		    data.add((ExplicitOutputPort) block.getChildAt(i));
		}
	    }
	}
	return data;
    }

    /**
     * @param block
     * @return implicit outputs ports of given block
     */
    public static List<ImplicitOutputPort> getAllImplicitOutputPorts(BasicBlock block, boolean revert) {
	List<ImplicitOutputPort> data = new ArrayList<ImplicitOutputPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof ImplicitOutputPort) {
		if(revert) {
		    data.add(0, (ImplicitOutputPort) block.getChildAt(i));
		} else {
		    data.add((ImplicitOutputPort) block.getChildAt(i));
		}
	    }
	}
	return data;
    }

    /**
     * @param block
     * @return command ports of given block
     */
    public static List<CommandPort> getAllCommandPorts(BasicBlock block, boolean revert) {
	List<CommandPort> data = new ArrayList<CommandPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof CommandPort) {
		if(revert) {
		    data.add(0, (CommandPort) block.getChildAt(i));
		} else {
		    data.add((CommandPort) block.getChildAt(i));
		}
	    }
	}
	
	return data;
    }

    /**
     * @param block
     * @return control ports of given block
     */
    public static List<ControlPort> getAllControlPorts(BasicBlock block, boolean revert) {
	List<ControlPort> data = new ArrayList<ControlPort>();
	int childrenCount = block.getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if (block.getChildAt(i) instanceof ControlPort) {
		if(revert) {
		    data.add(0, (ControlPort) block.getChildAt(i));
		} else {
		    data.add((ControlPort) block.getChildAt(i));
		}
	    }
	}

	return data;
    }
}
