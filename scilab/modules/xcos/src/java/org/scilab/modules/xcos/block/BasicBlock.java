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


import java.awt.MouseInfo;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.actions.BlockParametersAction;
import org.scilab.modules.xcos.actions.FlipAction;
import org.scilab.modules.xcos.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.actions.RotateAction;
import org.scilab.modules.xcos.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

public class BasicBlock extends mxCell {

    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;

    // TODO :
    // Must make this types evolve, but for now keep a strong link to Scilab
    // !! WARNING !!
    // exprs = [] ; rpar = [] ; ipar = [] ; opar = list()

    //private List<String> exprs = new ArrayList<String>();
    private ScilabType exprs = null;
    //private List<Double> realParameters = new ArrayList<Double>();
    private ScilabType realParameters = new ScilabDouble();
    //private List<Integer> integerParameters = new ArrayList<Integer>();
    private ScilabType integerParameters = new ScilabDouble();
    //private List objectsParameters = new ArrayList();
    private ScilabType objectsParameters = new ScilabList();

    private ScilabType nbZerosCrossing = new ScilabDouble();

    private ScilabType nmode = new ScilabDouble();

    private ScilabType state = new ScilabDouble();
    private ScilabType dState = new ScilabDouble();
    private ScilabType oDState = new ScilabDouble();

    private ScilabType equations = new ScilabList();

    private boolean dependsOnU = false;
    private boolean dependsOnT = false;

    private String blockType = "c";

    private int ordering = 0;
    protected boolean flip = false;
    protected int angle = 0;    
    private boolean locked = false;

    public enum SimulationFunctionType {
	DEFAULT,
	TYPE_1,
	TYPE_2,
	TYPE_3,
	C_OR_FORTRAN,
	SCILAB,
	UNKNOWN;

	public static SimulationFunctionType convertScilabValue(int scilabValue) {
	    switch (scilabValue) {
	    case 0:
		return DEFAULT;
	    case 1:
		return TYPE_1;
	    case 2:
		return TYPE_2;
	    case 3:
		return TYPE_3;
	    case 4:
		return C_OR_FORTRAN;
	    case 5:
		return SCILAB;
	    default:
		return UNKNOWN;
	    }
	}

	public double getAsDouble() {
	    switch (this) {
	    case DEFAULT:
		return 0.0;
	    case TYPE_1:
		return 1.0;
	    case TYPE_2 :
		return 2.0;
	    case TYPE_3 :
		return 3.0;
	    case C_OR_FORTRAN:
		return 4.0;
	    case SCILAB:
		return 5.0;
	    default:
		return -1;
	    }
	}
    };

    public static BasicBlock createBlock(String label) {
	if(label.compareTo("TEXT_f") == 0) { return new TextBlock(label); }
	if(label.compareTo("SUPER_f") == 0) { return new SuperBlock(label); }
	if(label.compareTo("CONST_m") == 0) { return new ConstBlock(label); }
	if(label.compareTo("AFFICH_m") == 0) { return new AfficheBlock(label); }
	if(label.compareTo("GAINBLK_f") == 0) { return new GainBlock(label); }
	else { return new BasicBlock(label); }
    }

    public BasicBlock() {
	super();
	setVertex(false);
	setVisible(false);
    }

    protected BasicBlock(String label) {
	super();
	setValue(label);
	setStyle("");
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

    public void setGeometry(mxGeometry geometry)
    {
	super.setGeometry(geometry);
	updateInputPortsPositions();
	updateOutputPortsPositions();
	updateCommandPortsPositions();
	updateControlPortsPositions();
    }

    public void setInterfaceFunctionName(String interfaceFunctionName) {
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

    public ScilabType getRealParameters() {
	return realParameters;
    }

    public void setRealParameters(ScilabType realParameters) {
	this.realParameters = realParameters;
    } 

    public ScilabType getIntegerParameters() {
	return integerParameters;
    }

    public void setIntegerParameters(ScilabType integerParameters) {
	this.integerParameters = integerParameters;
    }

    public ScilabType getObjectsParameters() {
	return objectsParameters;
    }

    public void setObjectsParameters(ScilabType objectsParameters) {
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

    public void setExprs(ScilabType exprs) {
	this.exprs = exprs;
    }

    public ScilabType getExprs() {
	return exprs;
    }

    public ScilabType getNbZerosCrossing() {
	return nbZerosCrossing;
    }

    public void setNbZerosCrossing(ScilabType nbZerosCrossing) {
	this.nbZerosCrossing = nbZerosCrossing;
    }

    public ScilabType getNmode() {
	return nmode;
    }

    public void setNmode(ScilabType nmode) {
	this.nmode = nmode;
    }

    public ScilabType getState() {
	return state;
    }

    public void setState(ScilabType state) {
	this.state = state;
    }

    public ScilabType getDState() {
	return dState;
    }

    public void setDState(ScilabType state) {
	dState = state;
    }

    public ScilabType getODState() {
	return oDState;
    }

    public void setODState(ScilabType state) {
	oDState = state;
    }

    public ScilabType getEquations() {
	return equations;
    }

    public void setEquations(ScilabType equations) {
	this.equations = equations;
    }

    public boolean isLocked() {
        return locked;
    }

    public void setLocked(boolean locked) {
        this.locked = locked;
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


    public void addPort(InputPort port) {
	insert(port);
	updateInputPortsPositions();
	port.setOrdering(getAllInputPorts().size());
    }

    private void updateInputPortsPositions() {
	   	int inputAngle = (getAngle() + 180) % 360;
    	List<BasicPort> allInputPorts = (List)getAllInputPorts();
    	rotatePorts(allInputPorts, inputAngle);
    }

    public void addPort(OutputPort port) {
	insert(port);
	updateOutputPortsPositions();
	port.setOrdering(getAllOutputPorts().size());
    }

    private void updateOutputPortsPositions() {
	   	int outputAngle = getAngle();
    	List<BasicPort> allOutputPorts = (List)getAllOutputPorts();
    	rotatePorts(allOutputPorts, outputAngle);
    }

    public void addPort(CommandPort port) {
	insert(port);
	updateCommandPortsPositions();
	port.setOrdering(getAllCommandPorts().size());
    }

    private void updateCommandPortsPositions() {
	   	int commandAngle = (getAngle() + 270) % 360;
    	List<BasicPort> allCommandPorts = (List)getAllCommandPorts();
    	rotatePorts(allCommandPorts, commandAngle);
    }

    public void addPort(ControlPort port) {
	insert(port);
	updateControlPortsPositions();
	port.setOrdering(getAllControlPorts().size());
    }

    private void updateControlPortsPositions() {
    int controlAngle = (getAngle() + 90) % 360;
	List<BasicPort> allControlPorts = (List)getAllControlPorts();
	rotatePorts(allControlPorts, controlAngle);
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
	//	if (getExprs().isEmpty()) {
	//	    return new ScilabList();
	//	}
	//
	//	String[][] data = new String[getExprs().size()][1];
	//	for (int i = 0 ; i < getExprs().size() ; ++i) {
	//	    data[i][0] = getExprs().get(i);
	//	}
	//
	//	return new ScilabString(data);
	return getExprs();
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

	model.add(getState()); // state

	model.add(getDState()); // dstate

	model.add(getODState()); // odstate

	model.add(createScilabRPar()); // rpar

	model.add(createScilabIPar()); // ipar

	model.add(getObjectsParameters()); // opar

	model.add(new ScilabString(getBlockType())); // blocktype

	model.add(getAllCommandPortsInitialStates()); // firing

	boolean[][] dep_ut = {{dependsOnU() , dependsOnT()}};
	model.add(new ScilabBoolean(dep_ut)); // dep_ut

	model.add(new ScilabString("")); // label

	model.add(getNbZerosCrossing()); // nzcross

	model.add(getNmode()); // nmode

	model.add(getEquations()); // equations

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
	//	if (realParameters.isEmpty()) {
	//	    return new ScilabDouble();
	//	}
	//	double[][] data = new double[realParameters.size()][1];
	//	for (int i = 0 ; i < realParameters.size() ; ++i) {
	//	    data[i][0] = realParameters.get(i).doubleValue();
	//	}
	//
	//	return new ScilabDouble(data);
	return getRealParameters();
    }

    private ScilabType createScilabIPar() {
	//	if (integerParameters.isEmpty()) {
	//	    return new ScilabDouble();
	//	}
	//	double[][] data = new double[integerParameters.size()][1];
	//	for (int i = 0 ; i < integerParameters.size() ; ++i) {
	//	    data[i][0] = integerParameters.get(i).doubleValue();
	//	}
	//
	//	return new ScilabDouble(data);
	return getIntegerParameters();
    }


    private ScilabString createScilabGuiProperties() {
	return new ScilabString(interfaceFunctionName);
    }

    private ScilabList createScilabDocProperties() {
	return new ScilabList();
    }

    private ScilabType getSimulationFunctionNameAndType() {
	if (getSimulationFunctionType() == SimulationFunctionType.DEFAULT) {
	    return new ScilabString(getSimulationFunctionName());
	}
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
	String[][] data = new String[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getType().getAsString();
	}

	return new ScilabString(data);
    }

    public void updateBlockSettings(BasicBlock modifiedBlock) {
    	System.err.println("updateBlockSettings");
	this.setDependsOnT(modifiedBlock.dependsOnT());
	this.setDependsOnU(modifiedBlock.dependsOnU());
	this.setExprs(modifiedBlock.getExprs());

	this.setRealParameters(modifiedBlock.getRealParameters());
	this.setIntegerParameters(modifiedBlock.getIntegerParameters());
	this.setObjectsParameters(modifiedBlock.getObjectsParameters());

	this.setState(modifiedBlock.getState());
	this.setDState(modifiedBlock.getDState());
	this.setODState(modifiedBlock.getODState());

	this.setEquations(modifiedBlock.getEquations());

	// Check if new input port have been added
	if (modifiedBlock.getAllInputPorts().size() > getAllInputPorts().size()) {
	    for(int i = getAllInputPorts().size() - 1 ; i < modifiedBlock.getAllInputPorts().size() ; ++i)
	    {
		addPort(modifiedBlock.getAllInputPorts().get(i));
	    }
	}
	// Check if input ports have been removed
	else if (modifiedBlock.getAllInputPorts().size() < getAllInputPorts().size()) {
	    List<InputPort> removedPorts = new ArrayList<InputPort>();
	    for(int i = modifiedBlock.getAllInputPorts().size() ; i < getAllInputPorts().size() ; ++i)
	    {
		removedPorts.add(getAllInputPorts().get(i));
	    }
	    for(int i = 0 ; i < removedPorts.size() ; ++i) {
		remove(removedPorts.get(i));
		getAllInputPorts().remove(removedPorts.get(i));
		updateInputPortsPositions();
	    }
	}
	
	// Check if new output port have been added
	if (modifiedBlock.getAllOutputPorts().size() > getAllOutputPorts().size()) {
	    for(int i = getAllOutputPorts().size() - 1 ; i < modifiedBlock.getAllOutputPorts().size() ; ++i)
	    {
		addPort(modifiedBlock.getAllOutputPorts().get(i));
	    }
	}
	// Check if output ports have been removed
	else if (modifiedBlock.getAllOutputPorts().size() < getAllOutputPorts().size()) {
	    List<OutputPort> removedPorts = new ArrayList<OutputPort>();
	    for(int i = modifiedBlock.getAllOutputPorts().size() ; i < getAllOutputPorts().size() ; ++i)
	    {
		removedPorts.add(getAllOutputPorts().get(i));
	    }
	    for(int i = 0 ; i < removedPorts.size() ; ++i) {
		remove(removedPorts.get(i));
		getAllOutputPorts().remove(removedPorts.get(i));
		updateOutputPortsPositions();
	    }
	}
	

	// Check if new command port have been added
	if (modifiedBlock.getAllCommandPorts().size() > getAllCommandPorts().size()) {
	    for(int i = getAllCommandPorts().size() - 1 ; i < modifiedBlock.getAllCommandPorts().size() ; ++i)
	    {
		addPort(modifiedBlock.getAllCommandPorts().get(i));
	    }
	}
	// Check if output ports have been removed
	else if (modifiedBlock.getAllCommandPorts().size() < getAllCommandPorts().size()) {
	    List<CommandPort> removedPorts = new ArrayList<CommandPort>();
	    for(int i = modifiedBlock.getAllCommandPorts().size() ; i < getAllCommandPorts().size() ; ++i)
	    {
		removedPorts.add(getAllCommandPorts().get(i));
	    }
	    for(int i = 0 ; i < removedPorts.size() ; ++i) {
		remove(removedPorts.get(i));
		getAllCommandPorts().remove(removedPorts.get(i));
		updateCommandPortsPositions();
	    }
	}
	
	// Check if new control port have been added
	if (modifiedBlock.getAllControlPorts().size() > getAllControlPorts().size()) {
	    for(int i = getAllControlPorts().size() - 1 ; i < modifiedBlock.getAllControlPorts().size() ; ++i)
	    {
		addPort(modifiedBlock.getAllControlPorts().get(i));
	    }
	}
	// Check if output ports have been removed
	else if (modifiedBlock.getAllControlPorts().size() < getAllControlPorts().size()) {
	    List<ControlPort> removedPorts = new ArrayList<ControlPort>();
	    for(int i = modifiedBlock.getAllControlPorts().size() ; i < getAllControlPorts().size() ; ++i)
	    {
		removedPorts.add(getAllControlPorts().get(i));
	    }
	    for(int i = 0 ; i < removedPorts.size() ; ++i) {
		remove(removedPorts.get(i));
		getAllControlPorts().remove(removedPorts.get(i));
		updateControlPortsPositions();
	    }
	}
    }

    public void openBlockSettings(String context, XcosDiagram diagram) {
	final File tempOutput;
	final File tempInput;
	final File tempContext;
	final XcosDiagram parent = diagram;
	try {
	    tempOutput = File.createTempFile("xcos",".hdf5");
	    tempInput = File.createTempFile("xcos",".hdf5");
	    tempContext = File.createTempFile("xcos",".hdf5");
	    tempOutput.delete();
	    tempInput.delete();
	    tempContext.delete();
	    // Write scs_m
	    int file_id = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(file_id, "scs_m", getAsScilabObj());
	    H5Write.closeFile(file_id);

	    // Write context
	    int context_file_id = H5Write.createFile(tempContext.getAbsolutePath());
	    H5Write.writeInDataSet(context_file_id, "context", new ScilabString(context));
	    H5Write.closeFile(context_file_id);

	    InterpreterManagement.requestScilabExec("xcosBlockInterface(\""+tempOutput.getAbsolutePath()+"\""+
		    ", \""+tempInput.getAbsolutePath()+"\""+
		    ", "+getInterfaceFunctionName()+
		    ", \"set\""+
		    ", \""+tempContext.getAbsolutePath()+"\");");
	    Thread launchMe = new Thread() {
		public void run() {
		    Signal.wait(tempInput.getAbsolutePath());
		    // Now read new Block
		    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
		    updateBlockSettings(modifiedBlock);
		    parent.fireEvent(XcosEvent.ADD_PORTS);
		    //tempOutput.delete();
		    //tempInput.delete();
		    setLocked(false);
		}
	    };
	    launchMe.start();
	    setLocked(true);

	} catch (Exception e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
    }

    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append("<html>");
	result.append("Block Name : "+getInterfaceFunctionName()+"<br>");
	result.append("Block Style : "+getStyle()+"<br>");
	result.append("Input ports : "+getAllInputPorts().size()+"<br>");
	result.append("Output ports : "+getAllOutputPorts().size()+"<br>");
	result.append("Control ports : "+getAllControlPorts().size()+"<br>");
	result.append("Command ports : "+getAllCommandPorts().size()+"<br>");
	//exprs
	if (getExprs() != null) {
	    result.append("Exprs : "+getExprs().toString()+"<br>");
	}
	else {
	    result.append("Exprs : (null)<br>");
	}
	//ipar
	if (getIntegerParameters() != null ) {
	    result.append("Ipar : "+getIntegerParameters().toString()+"<br>");
	}
	else {
	    result.append("Ipar : (null)<br>");
	}
	//rpar
	if (getRealParameters() != null) {
	    result.append("Rpar : "+getRealParameters().toString()+"<br>");
	}
	else {
	    result.append("Rpar : (null)<br>");
	}
	//opar
	if (getObjectsParameters() != null) {
	    result.append("Opar : "+getObjectsParameters().toString()+"<br>");
	}
	else {
	    result.append("Opar : (null)<br>");
	}
	
	result.append("</html>");
	return result.toString();
    }

    public void openContextMenu(ScilabGraph graph) {
		ContextMenu menu = ScilabContextMenu.createContextMenu();
		
		menu.add(BlockParametersAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(CutAction.cutMenu(graph));
		menu.add(CopyAction.copyMenu(graph));
		menu.add(DeleteAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(RegionToSuperblockAction.createMenu(graph));
		Menu mask = ScilabMenu.createMenu();
		mask.setText(XcosMessages.SUPERBLOCK_MASK);
		menu.add(mask);
		mask.add(SuperblockMaskCreateAction.createMenu(graph));
		mask.add(SuperblockMaskRemoveAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		Menu format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		menu.add(format);
		format.add(RotateAction.createMenu(graph));
		format.add(FlipAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(BlockDocumentationAction.createMenu(graph));

		menu.setVisible(true);
		
		((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
    }
    
    public boolean getFlip(){
    	return flip;
    }

    public void toggleFlip(){
    	flip = !flip;
    	
		List<InputPort> inputs = getAllInputPorts();
		List<OutputPort> outputs = getAllOutputPorts();

    	if(flip){//change coordinate to flip I/O ports
    		for(int i = 0 ; i < inputs.size() ; i++){
    			InputPort input = inputs.get(i);
    			input.getGeometry().setX(input.getGeometry().getX() + (input.getGeometry().getWidth() + getGeometry().getWidth()));
    			input.setStyle("ExplicitInputPort180");
    		}

    		for(int i = 0 ; i < outputs.size() ; i++){
    			OutputPort output = outputs.get(i);
    			output.getGeometry().setX(output.getGeometry().getX() - (output.getGeometry().getWidth() + getGeometry().getWidth()));
    			output.setStyle("ExplicitOutputPort180");
    		}
    	}else{
    		for(int i = 0 ; i < inputs.size() ; i++){
    			InputPort input = inputs.get(i);
    			input.getGeometry().setX(input.getGeometry().getX() - (input.getGeometry().getWidth() + getGeometry().getWidth()));
    			input.setStyle("ExplicitInputPort");
    		}

    		for(int i = 0 ; i < outputs.size() ; i++){
    			OutputPort output = outputs.get(i);
    			output.getGeometry().setX(output.getGeometry().getX() + (output.getGeometry().getWidth() + getGeometry().getWidth()));
    			output.setStyle("ExplicitOutputPort");
    		}
    	}
    }
    
    public int getAngle(){
    	return this.angle;
    }
    
    public void restoreRotation(){
    	setRotation(-this.angle);
    }
    
    public void setRotation(int angle){
		this.angle = (360 + (this.angle + angle)) % 360;

		updateInputPortsPositions();
		updateOutputPortsPositions();
		updateCommandPortsPositions();
		updateControlPortsPositions();
    }
    
    private void rotatePorts(List<BasicPort> ports , int angle){
    	
 		mxGeometry blockGeom = getGeometry();
		if(blockGeom == null){
			return;
		}
		switch(angle){
    	case 0 :
    	{
			double offsetY = blockGeom.getHeight() / (ports.size() + 1); 
    		for(int i = 0 ; i < ports.size() ; i++){
    			BasicPort port = ports.get(i);
        		mxGeometry portGeom = port.getGeometry();

        		portGeom.setX(blockGeom.getWidth());
        		portGeom.setY(offsetY * (i + 1) - (portGeom.getHeight()/2));
        		port.updateStyle(angle);
    		}
    		break;
    	}
    	case 90 :
    	{
			double offsetX = blockGeom.getWidth() / (ports.size() + 1); 
    		for(int i = 0 ; i < ports.size() ; i++){
    			BasicPort port = ports.get(i);
        		mxGeometry portGeom = port.getGeometry();

        		portGeom.setX(offsetX * (i + 1) - (portGeom.getWidth()/2));
        		portGeom.setY(- portGeom.getHeight());
        		port.updateStyle(angle);
    		}
    		break;
    	}
    	case 180 :
    	{
			double offsetY = blockGeom.getHeight() / (ports.size() + 1); 
    		for(int i = 0 ; i < ports.size() ; i++){
    			BasicPort port = ports.get(i);
        		mxGeometry portGeom = port.getGeometry();

        		portGeom.setX(- portGeom.getWidth());
        		portGeom.setY(offsetY * (i + 1) - (portGeom.getHeight()/2));
        		port.updateStyle(angle);
    		}
    		break;
    	}
    	case 270 :
    	{
			double offsetX = blockGeom.getWidth() / (ports.size() + 1); 
    		for(int i = 0 ; i < ports.size() ; i++){
    			BasicPort port = ports.get(i);
        		mxGeometry portGeom = port.getGeometry();

        		portGeom.setX(offsetX * (i + 1) - (portGeom.getWidth()/2));
        		portGeom.setY(blockGeom.getHeight());
        		port.updateStyle(angle);
    		}
    		break;
    	}
    	default :
    		return;
    	}
  	
    }
}
