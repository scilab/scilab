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


import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

public class BasicBlock extends mxCell {

    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.SCILAB;
  
    private List<Double> realParameters = new ArrayList<Double>();
    private List<Integer> integerParameters = new ArrayList<Integer>();
    private List objectsParameters = new ArrayList();
    
    private List<InputPort> inputPorts = new ArrayList<InputPort>();
    private List<OutputPort> outputPorts = new ArrayList<OutputPort>();
    private List<CommandPort> commandPorts = new ArrayList<CommandPort>();
    private List<ControlPort> controlPorts = new ArrayList<ControlPort>();

    public enum SimulationFunctionType {
	C_OR_FORTRAN,
	SCILAB;

	public double getAsDouble() {
	    switch (this) {
	    case C_OR_FORTRAN:
		return 4.0;
	    case SCILAB:
		return 5;
	    default:
		return 0;
	    }
	}
    };
    
    protected BasicBlock(String label, String style) {
	super();
	setValue(label);
	setStyle(style);
	setVertex(true);
	setConnectable(false);
	setGeometry(new mxGeometry(0,0,80,80));
    }

    protected String getInterfaceFunctionName() {
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

    public void setSimulationFunctionType(SimulationFunctionType simulationFunctionType) {
	this.simulationFunctionType = simulationFunctionType;
    }

    public SimulationFunctionType getSimulationFunctionType() {
	return simulationFunctionType;
    }

    public List getRealParameters() {
        return realParameters;
    }

    public List getIntegerParameters() {
        return integerParameters;
    }

    public List getObjectsParameters() {
        return objectsParameters;
    }

    protected void addPort(InputPort port) {
	inputPorts.add(port);
	for (int i = 0 ; i < inputPorts.size() ; ++i) {
	    mxGeometry portGeometry = inputPorts.get(i).getGeometry();
	    inputPorts.get(i).setGeometry(new mxGeometry(-portGeometry.getWidth(), 
		    (i + 1.0) / (inputPorts.size() + 1.0) * getGeometry().getHeight() - portGeometry.getHeight() / 2.0,
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	insert(port);
    }

    protected void addPort(OutputPort port) {
	outputPorts.add(port);
	for (int i = 0 ; i < outputPorts.size() ; ++i) {
	    mxGeometry portGeometry = outputPorts.get(i).getGeometry();
	    outputPorts.get(i).setGeometry(new mxGeometry(getGeometry().getWidth(), 
		    (i + 1.0) / (outputPorts.size() + 1.0) * getGeometry().getHeight() - portGeometry.getHeight() / 2.0,
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	insert(port);
    }

    protected void addPort(CommandPort port) {
	commandPorts.add(port);
	for (int i = 0 ; i < commandPorts.size() ; ++i) {
	    mxGeometry portGeometry = commandPorts.get(i).getGeometry();
	    commandPorts.get(i).setGeometry(new mxGeometry((i + 1.0) / (commandPorts.size() + 1.0) * getGeometry().getWidth() - portGeometry.getHeight() / 2.0, 
		    getGeometry().getHeight(),
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	insert(port);
    }

    protected void addPort(ControlPort port) {
	controlPorts.add(port);
	for (int i = 0 ; i < controlPorts.size() ; ++i) {
	    mxGeometry portGeometry = controlPorts.get(i).getGeometry();
	    controlPorts.get(i).setGeometry(new mxGeometry((i + 1.0) / (controlPorts.size() + 1.0) * getGeometry().getWidth() - portGeometry.getHeight() / 2.0, 
		    - portGeometry.getHeight(),
		    portGeometry.getWidth(),
		    portGeometry.getHeight()));
	}
	insert(port);
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

	graphics.add(new ScilabDouble(1)); // flip

	graphics.add(new ScilabDouble(0)); // theta

	graphics.add(new ScilabList()); // exprs

	graphics.add(getAllLinkId(inputPorts)); // pin

	graphics.add(getAllLinkId(outputPorts)); // pout

	graphics.add(getAllLinkId(controlPorts)); // pein

	graphics.add(getAllLinkId(commandPorts)); // peout

	ScilabList gr_i = new ScilabList();
	ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\""+getValue()+"\",sz(1),sz(2));");
	gr_i.add(graphicsInstructions);
	gr_i.add(new ScilabDouble(8));
	graphics.add(gr_i); // gr_i

	graphics.add(new ScilabString("")); // id

	graphics.add(getAllPortsType(inputPorts)); // in_implicit

	graphics.add(getAllPortsType(outputPorts)); // out_implicit

	return graphics;
    }

    private ScilabMList createScilabModelProperties() {
	String[] modelFields = {"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
		"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		"nzcross", "nmode", "equations"};
	ScilabMList model = new ScilabMList(modelFields);

	model.add(getSimulationFunctionNameAndType()); // sim

	model.add(getAllPortsDataLines(inputPorts)); // in

	model.add(getAllPortsDataColumns(inputPorts)); // in2

	model.add(getAllPortsDataType(inputPorts)); // intyp

	model.add(getAllPortsDataLines(outputPorts)); // out

	model.add(getAllPortsDataColumns(inputPorts)); // out2

	model.add(getAllPortsDataType(outputPorts)); // outtyp

	model.add(getAllPortsDataLines(controlPorts)); // evtin

	model.add(getAllPortsDataLines(commandPorts)); // evtout

	model.add(new ScilabDouble()); // state

	model.add(new ScilabDouble()); // dstate

	model.add(new ScilabDouble()); // odstate

	model.add(createScilabRPar()); // rpar

	model.add(createScilabIPar()); // ipar

	model.add(new ScilabList()); // opar

	model.add(new ScilabString("c")); // blocktype

	model.add(new ScilabDouble()); // firing

	double[][] dep_ut = {{1,0}};
	model.add(new ScilabDouble(dep_ut)); // dep_ut

	model.add(new ScilabString("")); // label

	model.add(new ScilabDouble(0)); // nzcross

	model.add(new ScilabDouble(0)); // nmode

	model.add(new ScilabList()); // equations

	return model;
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

	double[][] data = new double[1][ports.size()];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[0][i] = 0;
	}

	return new ScilabDouble(data);
    }

    private ScilabDouble getAllPortsSize(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataSize();
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
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[ports.size()][1];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[i][0] = ((BasicPort) ports.get(i)).getDataColumns();
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
}
