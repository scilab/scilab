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
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

public class BasicBlock extends mxCell {

	private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;
    private transient XcosDiagram parentDiagram = null;

    // TODO :
    // Must make this types evolve, but for now keep a strong link to Scilab
    // !! WARNING !!
    // exprs = [] ; rpar = [] ; ipar = [] ; opar = list()

    //private List<String> exprs = new ArrayList<String>();
    private ScilabType exprs = null;
    //private List<Double> realParameters = new ArrayList<Double>();
    private ScilabType realParameters = null;
    //private List<Integer> integerParameters = new ArrayList<Integer>();
    private ScilabType integerParameters = null;
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
    private boolean flip = false;
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
    	if(label.compareTo("IN_f") == 0) { return new ExplicitInBlock(label); }
    	if(label.compareTo("OUT_f") == 0) { return new ExplicitOutBlock(label); }
    	if(label.compareTo("INIMPL_f") == 0) { return new ImplicitInBlock(label); }
    	if(label.compareTo("OUTIMPL_f") == 0) { return new ImplicitOutBlock(label); }
    	if(label.compareTo("CLKINV_f") == 0) { return new EventInBlock(label); }
    	if(label.compareTo("CLKOUTV_f") == 0) { return new EventOutBlock(label); }
    	if(label.compareTo("SPLIT_f") == 0) { return new SplitBlock(label); }
    	else { 
    		return new BasicBlock(label); 
    	}
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
	setGeometry(new mxGeometry(0,0,40,40));
    }

    protected BasicBlock(String label, String style) {
	super();
	setValue(label);
	setStyle(style);
	setVertex(true);
	setConnectable(false);
	setGeometry(new mxGeometry(0,0,40,40));
    }

    public XcosDiagram getParentDiagram() {
        return parentDiagram;
    }

    public void setParentDiagram(XcosDiagram parentDiagram) {
        this.parentDiagram = parentDiagram;
    }

    
    public String getInterfaceFunctionName() {
	return interfaceFunctionName;
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

    public boolean isDependsOnU() {
    	return this.dependsOnU;
       }
    
    public boolean dependsOnU() {
	return dependsOnU;
    }

    public void setDependsOnT(boolean dependsOnT) {
	this.dependsOnT = dependsOnT;
    }

    public boolean isDependsOnT() {
    	return this.dependsOnT;
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

    public List<InputPort> getAllInputPorts() {
	List<InputPort> data = new ArrayList<InputPort>();
	int childrenCount = getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof InputPort) {
		data.add((InputPort) getChildAt(i));
	    }
	}

	return data;
    }

    public List<ExplicitInputPort> getAllExplicitInputPorts() {
    	List<ExplicitInputPort> data = new ArrayList<ExplicitInputPort>();
    	int childrenCount = getChildCount();

    	for (int i = 0 ; i < childrenCount ; ++i) {
    		if(getChildAt(i) instanceof ExplicitInputPort) {
    			data.add((ExplicitInputPort) getChildAt(i));
    		}
    	}
    	return data;
    }

    public List<ImplicitInputPort> getAllImplicitInputPorts() {
    	List<ImplicitInputPort> data = new ArrayList<ImplicitInputPort>();
    	int childrenCount = getChildCount();

    	for (int i = 0 ; i < childrenCount ; ++i) {
    		if(getChildAt(i) instanceof ImplicitInputPort) {
    			data.add((ImplicitInputPort) getChildAt(i));
    		}
    	}
    	return data;
    }

    public List<OutputPort> getAllOutputPorts() {
	List<OutputPort> data = new ArrayList<OutputPort>();
	int childrenCount = getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof OutputPort) {
		data.add((OutputPort) getChildAt(i));
	    }
	}

	return data;
    }

    public List<ExplicitOutputPort> getAllExplicitOutputPorts() {
    	List<ExplicitOutputPort> data = new ArrayList<ExplicitOutputPort>();
    	int childrenCount = getChildCount();

    	for (int i = 0 ; i < childrenCount ; ++i) {
    		if(getChildAt(i) instanceof ExplicitOutputPort) {
    			data.add((ExplicitOutputPort) getChildAt(i));
    		}
    	}
    	return data;
    }

    public List<ImplicitOutputPort> getAllImplicitOutputPorts() {
    	List<ImplicitOutputPort> data = new ArrayList<ImplicitOutputPort>();
    	int childrenCount = getChildCount();

    	for (int i = 0 ; i < childrenCount ; ++i) {
    		if(getChildAt(i) instanceof ImplicitOutputPort) {
    			data.add((ImplicitOutputPort) getChildAt(i));
    		}
    	}
    	return data;
    }

    public List<CommandPort> getAllCommandPorts() {
	List<CommandPort> data = new ArrayList<CommandPort>();
	int childrenCount = getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof CommandPort) {
		data.add((CommandPort) getChildAt(i));
	    }
	}

	return data;
    }

    public List<ControlPort> getAllControlPorts() {
	List<ControlPort> data = new ArrayList<ControlPort>();
	int childrenCount = getChildCount();

	for (int i = 0 ; i < childrenCount ; ++i) {
	    if(getChildAt(i) instanceof ControlPort) {
		data.add((ControlPort) getChildAt(i));
	    }
	}

	return data;
    }


    public void removePort(InputPort port){
    	remove(port);
    }
    
    public void removePort(OutputPort port){
    	remove(port);
    }
    
    public void removePort(CommandPort port){
    	remove(port);
    }
    
    public void removePort(ControlPort port){
    	remove(port);
    }

    public void addPort(InputPort port) {
	insert(port);
	updatePortsPosition(mxConstants.DIRECTION_EAST);
	port.setOrdering(getAllInputPorts().size());
    }

    public void addPort(OutputPort port) {
	insert(port);
	updatePortsPosition(mxConstants.DIRECTION_EAST);
	port.setOrdering(getAllOutputPorts().size());
    }

    public void addPort(CommandPort port) {
	insert(port);
	updatePortsPosition(mxConstants.DIRECTION_EAST);
	port.setOrdering(getAllCommandPorts().size());
    }

    public void addPort(ControlPort port) {
	insert(port);
	updatePortsPosition(mxConstants.DIRECTION_EAST);
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

//	graphics.add(new ScilabBoolean(!flip)); // flip
	graphics.add(new ScilabBoolean(true)); // flip

//	mxCellState state = getParentDiagram().getView().getState(this);
//	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);
//
//	double theta = 0;
//	if(currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0){
//		if(flip){
//			theta = 180;	
//		}else{
//			theta = 0;
//		}
//	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0){
//		if(flip){
//			theta = 270;	
//		}else{
//			theta = 90;
//		}
//	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0){
//		if(flip){
//			theta = 0;	
//		}else{
//			theta = 180;
//		}
//	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0){
//		if(flip){
//			theta = 90;	
//		}else{
//			theta = 270;
//		}
//	}
//	graphics.add(new ScilabDouble(theta)); // theta
	
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
	    }
	}
	
    }

    public void openBlockSettings(String context[]) {
	final File tempOutput;
	final File tempInput;
	final File tempContext;
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
		    getParentDiagram().fireEvent(XcosEvent.ADD_PORTS);
		    
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
	result.append("Block Address : " + this + "<br>");
	result.append("Block Name : "+ getInterfaceFunctionName() + "<br>");
	result.append("Block Style : " + getStyle() + "<br>");
	result.append("Input ports : " + getAllInputPorts().size() + "<br>");
	result.append("Output ports : " + getAllOutputPorts().size() + "<br>");
	result.append("Control ports : " + getAllControlPorts().size() + "<br>");
	result.append("Command ports : " + getAllCommandPorts().size() + "<br>");
	result.append("Diagram : " + getParentDiagram() + "<br>");
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
		format.add(ShowHideShadowAction.createMenu(graph));
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

    public void toggleFlip() {

    	flip = !flip;

    	mxCellState state = getParentDiagram().getView().getState(this);
    	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);

    	updatePortsPosition(getNextFlipDirection(currentBlockDirection));
    	updateBlockDirection(getNextFlipDirection(currentBlockDirection));
    }

    
    public void toggleAntiClockwiseRotation(XcosDiagram graph) {
    	mxCellState state = graph.getView().getState(this);
    	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);

    	updatePortsPosition(getNextAntiClockwiseDirection(currentBlockDirection));
    	updateBlockDirection(getNextAntiClockwiseDirection(currentBlockDirection));
    }

    private String getNextAntiClockwiseDirection(String currentBlockDirection) {
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_NORTH; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_WEST; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_SOUTH; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_EAST; }
    	return null;
    }

    private String getNextClockwiseDirection(String currentBlockDirection) {
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_SOUTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_EAST; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_NORTH; }
	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_WEST; }
	return null;
    }
    
    private String getNextFlipDirection(String currentBlockDirection) {
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_WEST; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_SOUTH; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_EAST; }
    	if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_NORTH; }
    	return null;
        }

 
    private void updateBlockDirection(String newBlockDirection) {
	mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, mxConstants.STYLE_DIRECTION, newBlockDirection);
	
	rotatePorts(getAllInputPorts(), getDataPortsDirection(newBlockDirection));
	rotatePorts(getAllOutputPorts(), getDataPortsDirection(newBlockDirection));
	rotatePorts(getAllCommandPorts(), getEventPortsDirection(newBlockDirection));
	rotatePorts(getAllControlPorts(), getEventPortsDirection(newBlockDirection));
    }
    
    private String getEventPortsDirection(String currentBlockDirection) {
    	if(flip){
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_NORTH; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_WEST; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_SOUTH; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_EAST; }
    		return null;
    	}else{
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) { return mxConstants.DIRECTION_SOUTH; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) { return mxConstants.DIRECTION_EAST; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) { return mxConstants.DIRECTION_NORTH; }
    		if (currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) { return mxConstants.DIRECTION_WEST; }
    		return null;
    	}
    }

    private String getDataPortsDirection(String currentBlockDirection) {
    	return currentBlockDirection;
    }

    /**
     * Dispatch ports on Block's _WEST_ side.
     * @param ports
     */
    private void updateWestPortsPosition(List ports) {
    	mxGeometry blockGeom = getGeometry();
    	if(blockGeom == null){
    		return;
    	}
    	for (int i = 0 ; i < ports.size() ; ++i) {
    		mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
    		portGeom.setX(- portGeom.getWidth());
    		portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
    				- (portGeom.getHeight() / 2.0));
    	}
    }

    /**
     * Dispatch ports on Block's _NORTH_ side.
     * @param ports
     */
    private void updateNorthPortsPosition(List ports) {
    	mxGeometry blockGeom = getGeometry();
    	if(blockGeom == null){
    		return;
    	}
    	for (int i = 0 ; i < ports.size() ; ++i) {
    		mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
    		portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
    				- (portGeom.getWidth() / 2.0));
    		portGeom.setY(- portGeom.getHeight());
    	}
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     * @param ports
     */
    private void updateEastPortsPosition(List ports) {
    	mxGeometry blockGeom = getGeometry();
    	if(blockGeom == null){
    		return;
    	}
    	for (int i = 0 ; i < ports.size() ; ++i) {
    		mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
    		portGeom.setX(blockGeom.getWidth());
    		portGeom.setY((i + 1.0) * (blockGeom.getHeight() / (ports.size() + 1.0))
    				- (portGeom.getHeight() / 2.0));
    	}
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     * @param ports
     */
    private void updateSouthPortsPosition(List ports) {
    	mxGeometry blockGeom = getGeometry();
    	if(blockGeom == null){
    		return;
    	}
    	for (int i = 0 ; i < ports.size() ; ++i) {
    		mxGeometry portGeom = ((BasicPort) ports.get(i)).getGeometry();
    		portGeom.setX((i + 1.0) * (blockGeom.getWidth() / (ports.size() + 1.0))
    				- (portGeom.getWidth() / 2.0));
    		portGeom.setY(blockGeom.getHeight());
    	}
    }

    private void updatePortsPosition(String blockDirection) {
    	// Block -> EAST
    	// East <=> Out / North <=> Control / West <=> In / South <=> Command
    	if (blockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0) {
    		if(flip){
    			updateEastPortsPosition(getAllOutputPorts());
    			updateSouthPortsPosition(getAllControlPorts());
    			updateWestPortsPosition(getAllInputPorts());
    			updateNorthPortsPosition(getAllCommandPorts());
    		}else{
    			updateEastPortsPosition(getAllOutputPorts());
    			updateNorthPortsPosition(getAllControlPorts());
    			updateWestPortsPosition(getAllInputPorts());
    			updateSouthPortsPosition(getAllCommandPorts());
    		}
    	}
    	// Block -> NORTH
    	// East <=> Command / North <=> Out / West <=> Control / South <=> In
    	if (blockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0) {
    		if(flip){
    			updateWestPortsPosition(getAllCommandPorts());
    			updateNorthPortsPosition(getAllOutputPorts());
    			updateEastPortsPosition(getAllControlPorts());
    			updateSouthPortsPosition(getAllInputPorts());
    		}else{
    			updateEastPortsPosition(getAllCommandPorts());
    			updateNorthPortsPosition(getAllOutputPorts());
    			updateWestPortsPosition(getAllControlPorts());
    			updateSouthPortsPosition(getAllInputPorts());
    		}
    	}
    	// Block -> WEST
    	// East <=> In / North <=> Command / West <=> Out / South <=> Control
    	if (blockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0) {
    		if(flip){
    			updateEastPortsPosition(getAllInputPorts());
    			updateSouthPortsPosition(getAllCommandPorts());
    			updateWestPortsPosition(getAllOutputPorts());
    			updateNorthPortsPosition(getAllControlPorts());
    		}else{
    			updateEastPortsPosition(getAllInputPorts());
    			updateNorthPortsPosition(getAllCommandPorts());
    			updateWestPortsPosition(getAllOutputPorts());
    			updateSouthPortsPosition(getAllControlPorts());
    		}
    	}
    	// Block -> SOUTH
    	// East <=> Control / North <=> In / West <=> Command / South <=> Out
    	if (blockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0) {
    		if(flip){
    			updateWestPortsPosition(getAllControlPorts());
    			updateNorthPortsPosition(getAllInputPorts());
    			updateEastPortsPosition(getAllCommandPorts());
    			updateSouthPortsPosition(getAllOutputPorts());
    		}else{
    			updateEastPortsPosition(getAllControlPorts());
    			updateNorthPortsPosition(getAllInputPorts());
    			updateWestPortsPosition(getAllCommandPorts());
    			updateSouthPortsPosition(getAllOutputPorts());
    		}
    	}
    }    

    private void rotatePorts(List ports , String portOrientation){
    	for(int i = 0 ; i < ports.size() ; ++i) {
    		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {ports.get(i)}, mxConstants.STYLE_DIRECTION, portOrientation);
    	}
    }

    public void updateBlockView() {
    	if (getParentDiagram().getView() != null && getParentDiagram().getView().getState(this) != null) {
    		mxCellState state = getParentDiagram().getView().getState(this);
    		String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);
    		//currentBlockDirection = mxConstants.DIRECTION_EAST;
    		updatePortsPosition(currentBlockDirection);
    		rotatePorts(getAllInputPorts(), getDataPortsDirection(currentBlockDirection));
    		rotatePorts(getAllOutputPorts(), getDataPortsDirection(currentBlockDirection));
    		rotatePorts(getAllCommandPorts(), getEventPortsDirection(currentBlockDirection));
    		rotatePorts(getAllControlPorts(), getEventPortsDirection(currentBlockDirection));
    	}
    }

    /**
     * Create a clone for block added by context menu in palette
     * @return the clone
     */
    public Object createClone() {
    	try {
    		BasicBlock clone = (BasicBlock) clone();

    		/* Clone children */
    		for (int i = 0; i < getChildCount(); i++) {
    			if (getChildAt(i) instanceof InputPort) {
    				clone.addPort((InputPort) getChildAt(i).clone());
    			} else if (getChildAt(i) instanceof OutputPort) {
    				clone.addPort((OutputPort) getChildAt(i).clone());
    			} else if (getChildAt(i) instanceof CommandPort) {
    				clone.addPort((CommandPort) getChildAt(i).clone());
    			} else if (getChildAt(i) instanceof ControlPort) {
    				clone.addPort((ControlPort) getChildAt(i).clone());
    			}
    		}

    		/* Make the block appear into the diagram */
    		clone.getGeometry().setX(10);
    		clone.getGeometry().setY(10);
    		return clone;
    	} catch (CloneNotSupportedException e) {
    		e.printStackTrace();
    		return null;
    	}
    }
}
