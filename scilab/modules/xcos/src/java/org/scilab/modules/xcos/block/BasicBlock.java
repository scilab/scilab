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


import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.Signal;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

public class BasicBlock extends mxCell {

    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;

    private List<String> exprs = new ArrayList<String>();
    private List<Double> realParameters = new ArrayList<Double>();
    private List<Integer> integerParameters = new ArrayList<Integer>();
    private List objectsParameters = new ArrayList();

    private boolean dependsOnU = false;
    private boolean dependsOnT = false;

    private String blockType = "c";

    private int ordering = 0;

    public enum SimulationFunctionType {
	DEFAULT, 
	C_OR_FORTRAN,
	SCILAB;

	public static SimulationFunctionType convertScilabValue(int scilabValue) {
	    switch (scilabValue) {
	    case 0:
		return DEFAULT;
	    case 4:
		return C_OR_FORTRAN;
	    case 5:
		return SCILAB;
	    default:
		return DEFAULT;
	    }
	}

	public double getAsDouble() {
	    switch (this) {
	    case DEFAULT:
		return 0.0;
	    case C_OR_FORTRAN:
		return 4.0;
	    case SCILAB:
		return 5.0;
	    default:
		return -1;
	    }
	}
    };

    protected BasicBlock(String label) {
	super();
	setValue(label);
	setStyle("block");
	setVertex(true);
	setConnectable(false);
	setGeometry(new mxGeometry(0,0,80,80));
    }

    protected BasicBlock(String label, String style) {
	super();
	setValue(label);
	setStyle(style);
	setVertex(true);
	setConnectable(false);
	setGeometry(new mxGeometry(0,0,80,80));
    }

    public String getInterfaceFunctionName() {
	return interfaceFunctionName;
    }

    protected void setInterfaceFunctionName(String interfaceFunctionName) {
	this.interfaceFunctionName = interfaceFunctionName;
    }

    public void setSimulationFunctionName(String simulationFunctionName) {
	this.simulationFunctionName = simulationFunctionName;
    }

    public String getSimulationFunctionName() {
	return simulationFunctionName;
    }

    public void setSimulationFunctionType(int scilabValue) {
	this.simulationFunctionType = SimulationFunctionType.convertScilabValue(scilabValue);
    }

    public void setSimulationFunctionType(SimulationFunctionType simulationFunctionType) {
	this.simulationFunctionType = simulationFunctionType;
    }

    public SimulationFunctionType getSimulationFunctionType() {
	return simulationFunctionType;
    }

    public List getRealParameters() {
	return realParameters;
    }

    public void setRealParameters(List<Double> realParameters) {
	this.realParameters = realParameters;
    }

    public List getIntegerParameters() {
	return integerParameters;
    }

    public void setIntegerParameters(List<Integer> integerParameters) {
	this.integerParameters = integerParameters;
    }

    public List getObjectsParameters() {
	return objectsParameters;
    }

    public void setObjectsParameters(List objectsParameters) {
	this.objectsParameters = objectsParameters;
    }

    public void setDependsOnU(boolean dependsOnU) {
	this.dependsOnU = dependsOnU;
    }

    public boolean dependsOnU() {
	return dependsOnU;
    }

    public void setDependsOnT(boolean dependsOnT) {
	this.dependsOnT = dependsOnT;
    }

    public boolean dependsOnT() {
	return dependsOnT;
    }

    public void setBlockType(String blockType) {
	this.blockType = blockType;
    }

    public String getBlockType() {
	return blockType;
    }

    public void setOrdering(int ordering) {
	this.ordering = ordering;
    }

    public int getOrdering() {
	return ordering;
    }

    public void setExprs(List<String> exprs) {
	this.exprs = exprs;
    }

    public List<String> getExprs() {
	return exprs;
    }

    protected List<InputPort> getAllInputPorts() {
	List<InputPort> data = new ArrayList<InputPort>();
	int childrenCount = getChildCount();
	
	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof InputPort) {
		data.add((InputPort) getChildAt(i));
	    }
	}
	
	return data;
    }
    
    protected List<OutputPort> getAllOutputPorts() {
	List<OutputPort> data = new ArrayList<OutputPort>();
	int childrenCount = getChildCount();
	
	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof OutputPort) {
		data.add((OutputPort) getChildAt(i));
	    }
	}
	
	return data;
    }

    protected List<CommandPort> getAllCommandPorts() {
	List<CommandPort> data = new ArrayList<CommandPort>();
	int childrenCount = getChildCount();
	
	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof CommandPort) {
		data.add((CommandPort) getChildAt(i));
	    }
	}
	
	return data;
    }

    protected List<ControlPort> getAllControlPorts() {
	List<ControlPort> data = new ArrayList<ControlPort>();
	int childrenCount = getChildCount();
	
	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof ControlPort) {
		data.add((ControlPort) getChildAt(i));
	    }
	}
	
	return data;
    }
    
    
    protected void addPort(InputPort port) {
	insert(port);
	List<InputPort> allInputPorts = getAllInputPorts();
	for (int i = 0 ; i < allInputPorts.size() ; ++i) {
	    mxGeometry portGeometry = allInputPorts.get(i).getGeometry();
	    allInputPorts.get(i).setGeometry(new mxGeometry(-portGeometry.getWidth(), 
		    (i + 1.0) / (getAllInputPorts().size() + 1.0) * getGeometry().getHeight() - portGeometry.getHeight() / 2.0,
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
    }

    protected void addPort(OutputPort port) {
	insert(port);
	List<OutputPort> allOutputPorts = getAllOutputPorts();
	for (int i = 0 ; i < allOutputPorts.size() ; ++i) {
	    mxGeometry portGeometry = allOutputPorts.get(i).getGeometry();
	    allOutputPorts.get(i).setGeometry(new mxGeometry(getGeometry().getWidth(), 
		    (i + 1.0) / (getAllOutputPorts().size() + 1.0) * getGeometry().getHeight() - portGeometry.getHeight() / 2.0,
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	port.setOrdering(getAllOutputPorts().size());
    }

    protected void addPort(CommandPort port) {
	insert(port);
	List<CommandPort> allCommandPorts = getAllCommandPorts();
	for (int i = 0 ; i < allCommandPorts.size() ; ++i) {
	    mxGeometry portGeometry = allCommandPorts.get(i).getGeometry();
	    allCommandPorts.get(i).setGeometry(new mxGeometry((i + 1.0) / (getAllCommandPorts().size() + 1.0) * getGeometry().getWidth() - portGeometry.getHeight() / 2.0, 
		    getGeometry().getHeight(),
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	port.setOrdering(getAllCommandPorts().size());
    }

    protected void addPort(ControlPort port) {
	insert(port);
	List<ControlPort> allControlPorts = getAllControlPorts();
	for (int i = 0 ; i < allControlPorts.size() ; ++i) {
	    mxGeometry portGeometry = allControlPorts.get(i).getGeometry();
	    allControlPorts.get(i).setGeometry(new mxGeometry((i + 1.0) / (getAllControlPorts().size() + 1.0) * getGeometry().getWidth() - portGeometry.getHeight() / 2.0, 
		    - portGeometry.getHeight(),
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	port.setOrdering(getAllControlPorts().size());
    }

    public ScilabMList getAsScilabObj() {
	String[] objFields = {"Block", "graphics", "model", "gui", "doc"};
	ScilabMList obj = new ScilabMList(objFields);

	obj.add(createScilabGraphicsProperties());
	obj.add(createScilabModelProperties());
	obj.add(createScilabGuiProperties());
	obj.add(createScilabDocProperties());

	return obj;
    }

    private ScilabMList createScilabGraphicsProperties() {
	String[] graphicsFields = {"graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout", "gr_i",
		"id", "in_implicit", "out_implicit"};
	ScilabMList graphics = new ScilabMList(graphicsFields);

	double[][] orig = {{getGeometry().getX(), - getGeometry().getY()}};
	graphics.add(new ScilabDouble(orig)); // orig

	double[][] sz = {{getGeometry().getWidth(), getGeometry().getHeight()}};
	graphics.add(new ScilabDouble(sz)); // sz

	graphics.add(new ScilabBoolean(true)); // flip

	graphics.add(new ScilabDouble(0)); // theta

	graphics.add(getAllExprs()); // exprs

	graphics.add(getAllLinkId(getAllInputPorts())); // pin

	graphics.add(getAllLinkId(getAllOutputPorts())); // pout

	graphics.add(getAllLinkId(getAllControlPorts())); // pein

	graphics.add(getAllLinkId(getAllCommandPorts())); // peout

	ScilabList gr_i = new ScilabList();
	ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\""+getValue()+"\",sz(1),sz(2));");
	gr_i.add(graphicsInstructions);
	gr_i.add(new ScilabDouble(8));
	graphics.add(gr_i); // gr_i

	graphics.add(new ScilabString("")); // id

	graphics.add(getAllPortsType(getAllInputPorts())); // in_implicit

	graphics.add(getAllPortsType(getAllOutputPorts())); // out_implicit

	return graphics;
    }

    private ScilabType getAllExprs() {
	if (getExprs().isEmpty()) {
	    return new ScilabList();
	}

	String[][] data = new String[getExprs().size()][1];
	for (int i = 0 ; i < getExprs().size() ; ++i) {
	    data[i][0] = getExprs().get(i);
	}

	return new ScilabString(data);
    }

    private ScilabMList createScilabModelProperties() {
	String[] modelFields = {"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
		"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		"nzcross", "nmode", "equations"};
	ScilabMList model = new ScilabMList(modelFields);

	model.add(getSimulationFunctionNameAndType()); // sim

	model.add(getAllPortsDataLines(getAllInputPorts())); // in

	model.add(getAllPortsDataColumns(getAllInputPorts())); // in2

	model.add(getAllPortsDataType(getAllInputPorts())); // intyp

	model.add(getAllPortsDataLines(getAllOutputPorts())); // out

	model.add(getAllPortsDataColumns(getAllOutputPorts())); // out2

	model.add(getAllPortsDataType(getAllOutputPorts())); // outtyp

	model.add(getAllPortsDataLines(getAllControlPorts())); // evtin

	model.add(getAllPortsDataLines(getAllCommandPorts())); // evtout

	model.add(new ScilabDouble()); // state

	model.add(new ScilabDouble()); // dstate

	model.add(new ScilabList()); // odstate

	model.add(createScilabRPar()); // rpar

	model.add(createScilabIPar()); // ipar

	model.add(new ScilabList()); // opar

	model.add(new ScilabString(getBlockType())); // blocktype

	model.add(getAllCommandPortsInitialStates()); // firing

	boolean[][] dep_ut = {{dependsOnU() , dependsOnT()}};
	model.add(new ScilabBoolean(dep_ut)); // dep_ut

	model.add(new ScilabString("")); // label

	model.add(new ScilabDouble(0)); // nzcross

	model.add(new ScilabDouble(0)); // nmode

	model.add(new ScilabList()); // equations

	return model;
    }

    private ScilabDouble getAllCommandPortsInitialStates() {
	if (getAllCommandPorts().isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[getAllCommandPorts().size()][1];
	for (int i = 0 ; i < getAllCommandPorts().size() ; ++i) {
	    data[i][0] = getAllCommandPorts().get(i).getInitialState();
	}

	return new ScilabDouble(data);
    }

    private ScilabType createScilabRPar() {
	if (realParameters.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[realParameters.size()][1];
	for (int i = 0 ; i < realParameters.size() ; ++i) {
	    data[i][0] = realParameters.get(i).doubleValue();
	}

	return new ScilabDouble(data);
    }

    private ScilabType createScilabIPar() {
	if (integerParameters.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[integerParameters.size()][1];
	for (int i = 0 ; i < integerParameters.size() ; ++i) {
	    data[i][0] = integerParameters.get(i).doubleValue();
	}

	return new ScilabDouble(data);
    }


    private ScilabString createScilabGuiProperties() {
	return new ScilabString(interfaceFunctionName);
    }

    private ScilabList createScilabDocProperties() {
	return new ScilabList();
    }

    private ScilabList getSimulationFunctionNameAndType() {
	ScilabList data = new ScilabList();

	data.add(new ScilabString(getSimulationFunctionName()));
	data.add(new ScilabDouble(getSimulationFunctionType().getAsDouble()));

	return data;
    }

    private ScilabDouble getAllLinkId(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getConnectedLinkId();
	}

	return new ScilabDouble(data);
    }

    private ScilabDouble getAllPortsDataLines(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataLines();
	}

	return new ScilabDouble(data);
    }

    private ScilabDouble getAllPortsDataColumns(List ports) {
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

	if(allZeros) {
	    return new ScilabDouble();
	}

	return new ScilabDouble(data);
    }

    private ScilabType getAllPortsDataType(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataType().getAsDouble();
	}

	return new ScilabDouble(data);
    }

    private ScilabType getAllPortsType(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	String[][] data = new String[1][ports.size()];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[0][i] = ((BasicPort) ports.get(i)).getType().getAsString();
	}

	return new ScilabString(data);
    }

    public void updateBlockSettings(BasicBlock modifiedBlock) {
	this.setDependsOnT(modifiedBlock.dependsOnT());
	this.setDependsOnU(modifiedBlock.dependsOnU());
	this.setExprs(modifiedBlock.getExprs());

	this.setRealParameters(modifiedBlock.getRealParameters());
	this.setIntegerParameters(modifiedBlock.getIntegerParameters());
	this.setObjectsParameters(modifiedBlock.getObjectsParameters());

	System.out.println("pin = "+modifiedBlock.getAllInputPorts().size());
    }

    public void openBlockSettings() {
	final File tempOutput;
	final File tempInput;
	try {
	    tempOutput = File.createTempFile("xcos",".hdf5");
	    tempInput = File.createTempFile("xcos",".hdf5");
	    tempOutput.delete();
	    tempInput.delete();
	    int file_id = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(file_id, "scs_m", getAsScilabObj());
	    H5Write.closeFile(file_id);
	    InterpreterManagement.requestScilabExec("xcosBlockInterface(\""+tempOutput.getAbsolutePath()+"\", \""+tempInput.getAbsolutePath()+"\", "+getInterfaceFunctionName()+", \"set\");");
	    Thread launchMe = new Thread() {
		public void run() {
		    Signal.wait(tempInput.getAbsolutePath());
		    // Now read new Block
		    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
		    updateBlockSettings(modifiedBlock);
		    //tempOutput.delete();
		    //tempInput.delete();
		}
	    };
	    launchMe.start();

	} catch (Exception e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
    }
}
