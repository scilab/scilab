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

    private String interfaceFunctionName = "c_block";
    private List<InputPort> inputPorts = new ArrayList<InputPort>();
    private List<OutputPort> outputPorts = new ArrayList<OutputPort>();
    private List<CommandPort> commandPorts = new ArrayList<CommandPort>();
    private List<ControlPort> controlPorts = new ArrayList<ControlPort>();

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
	insert(port);
	commandPorts.add(port);
    }

    protected void addPort(ControlPort port) {
	insert(port);
	controlPorts.add(port);
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

	graphics.add(getAllPortsSize(inputPorts)); // pin

	graphics.add(getAllPortsSize(outputPorts)); // pout

	graphics.add(getAllPortsSize(controlPorts)); // pein

	graphics.add(getAllPortsSize(commandPorts)); // peout

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

	model.add(new ScilabString("my_sim_fun")); // sim

	model.add(getAllPortsSize(inputPorts)); // in

	model.add(new ScilabDouble()); // in2

	model.add(new ScilabDouble(1)); // intyp

	model.add(getAllPortsSize(outputPorts)); // out

	model.add(new ScilabDouble()); // out2

	model.add(new ScilabDouble(1)); // outtyp

	model.add(getAllPortsSize(controlPorts)); // evtin

	model.add(getAllPortsSize(commandPorts)); // evtout

	model.add(new ScilabDouble()); // state

	model.add(new ScilabDouble()); // dstate

	model.add(new ScilabDouble()); // odstate

	model.add(new ScilabDouble()); // rpar

	model.add(new ScilabDouble()); // ipar

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

    private ScilabString createScilabGuiProperties() {
	return new ScilabString(interfaceFunctionName);
    }

    private ScilabList createScilabDocProperties() {
	return new ScilabList();
    }

    private ScilabDouble getAllPortsSize(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[1][ports.size()];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    data[0][i] = ((BasicPort) ports.get(i)).getDataSize();
	}

	return new ScilabDouble(data);
    }

    private ScilabType getAllPortsType(List ports) {
	if (ports.isEmpty()) {
	    return new ScilabDouble();
	}
	String[][] data = new String[1][ports.size()];
	for (int i = 0 ; i < ports.size() ; ++i) {
	    if (((BasicPort) ports.get(i)).getType() == BasicPort.Type.IMPLICIT) {
		data[0][i] = "I";
	    }
	    else if (((BasicPort) ports.get(i)).getType() == BasicPort.Type.EXPLICIT) {
		data[0][i] = "E";
	    }
	    else {
		data[0][i] = "";
	    }
	}

	return new ScilabString(data);
    }
}
