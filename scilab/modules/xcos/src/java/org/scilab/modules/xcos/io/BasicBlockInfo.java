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

	private BasicBlockInfo() {

	}

	public static ScilabDouble getAllLinkId(List ports) {
		if (ports.isEmpty()) {
			return new ScilabDouble();
		}

		double[][] data = new double[ports.size()][1];
		for (int i = 0 ; i < ports.size() ; ++i) {
			data[i][0] = ((BasicPort) ports.get(i)).getConnectedLinkId();
		}

		return new ScilabDouble(data);
	}

	public static ScilabDouble getAllPortsDataLines(List ports) {
		if (ports.isEmpty()) {
			return new ScilabDouble();
		}
		double[][] data = new double[ports.size()][1];
		for (int i = 0 ; i < ports.size() ; ++i) {
			data[i][0] = ((BasicPort) ports.get(i)).getDataLines();
		}

		return new ScilabDouble(data);
	}

	public static ScilabDouble getAllPortsDataColumns(List ports) {
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

	public static ScilabType getAllPortsDataType(List ports) {
		if (ports.isEmpty()) {
			return new ScilabDouble();
		}
		double[][] data = new double[ports.size()][1];
		for (int i = 0 ; i < ports.size() ; ++i) {
			data[i][0] = ((BasicPort) ports.get(i)).getDataType().getAsDouble();
		}

		return new ScilabDouble(data);
	}

	public static ScilabType getAllPortsType(List ports) {
		if (ports.isEmpty()) {
			return new ScilabDouble();
		}
		String[][] data = new String[ports.size()][1];
		for (int i = 0 ; i < ports.size() ; ++i) {
			data[i][0] = ((BasicPort) ports.get(i)).getType().getAsString();
		}

		return new ScilabString(data);
	}

	public static ScilabMList createScilabGraphicsProperties(BasicBlock block) {
		String[] graphicsFields = {"graphics", "orig", "sz", "flip", "theta", "exprs", "pin", "pout", "pein", "peout", "gr_i",
				"id", "in_implicit", "out_implicit"};
		ScilabMList graphics = new ScilabMList(graphicsFields);

		double[][] orig = {{block.getGeometry().getX(), - block.getGeometry().getY()}};
		graphics.add(new ScilabDouble(orig)); // orig

		double[][] sz = {{block.getGeometry().getWidth(), block.getGeometry().getHeight()}};
		graphics.add(new ScilabDouble(sz)); // sz

		//    	graphics.add(new ScilabBoolean(!flip)); // flip
		graphics.add(new ScilabBoolean(true)); // flip

		//    	mxCellState state = getParentDiagram().getView().getState(this);
		//    	String currentBlockDirection = mxUtils.getString(state.getStyle(), mxConstants.STYLE_DIRECTION, mxConstants.DIRECTION_EAST);
		//
		//    	double theta = 0;
		//    	if(currentBlockDirection.compareTo(mxConstants.DIRECTION_EAST) == 0){
		//    		if(flip){
		//    			theta = 180;	
		//    		}else{
		//    			theta = 0;
		//    		}
		//    	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_NORTH) == 0){
		//    		if(flip){
		//    			theta = 270;	
		//    		}else{
		//    			theta = 90;
		//    		}
		//    	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_WEST) == 0){
		//    		if(flip){
		//    			theta = 0;	
		//    		}else{
		//    			theta = 180;
		//    		}
		//    	}else if(currentBlockDirection.compareTo(mxConstants.DIRECTION_SOUTH) == 0){
		//    		if(flip){
		//    			theta = 90;	
		//    		}else{
		//    			theta = 270;
		//    		}
		//    	}
		//    	graphics.add(new ScilabDouble(theta)); // theta

		graphics.add(new ScilabDouble(0)); // theta

		graphics.add(block.getExprs()); // exprs

		graphics.add(getAllLinkId(getAllInputPorts(block))); // pin

		graphics.add(getAllLinkId(getAllOutputPorts(block))); // pout

		graphics.add(getAllLinkId(getAllControlPorts(block))); // pein

		graphics.add(getAllLinkId(getAllCommandPorts(block))); // peout

		ScilabList gr_i = new ScilabList();
		ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\"" + block.getInterfaceFunctionName() + "\",sz(1),sz(2));");
		gr_i.add(graphicsInstructions);
		gr_i.add(new ScilabDouble(8));
		graphics.add(gr_i); // gr_i

		graphics.add(new ScilabString("")); // id

		graphics.add(getAllPortsType(getAllInputPorts(block))); // in_implicit

		graphics.add(getAllPortsType(getAllOutputPorts(block))); // out_implicit

		return graphics;
	}


	public static ScilabMList createScilabModelProperties(BasicBlock block) {
		String[] modelFields = {"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
				"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
				"nzcross", "nmode", "equations"};
		ScilabMList model = new ScilabMList(modelFields);

		model.add(block.getSimulationFunctionNameAndType()); // sim

		model.add(getAllPortsDataLines(getAllInputPorts(block))); // in

		model.add(getAllPortsDataColumns(getAllInputPorts(block))); // in2

		model.add(getAllPortsDataType(getAllInputPorts(block))); // intyp

		model.add(getAllPortsDataLines(getAllOutputPorts(block))); // out

		model.add(getAllPortsDataColumns(getAllOutputPorts(block))); // out2

		model.add(getAllPortsDataType(getAllOutputPorts(block))); // outtyp

		model.add(getAllPortsDataLines(getAllControlPorts(block))); // evtin

		model.add(getAllPortsDataLines(getAllCommandPorts(block))); // evtout

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

	public static ScilabString createScilabGuiProperties(BasicBlock block) {
		return new ScilabString(block.getInterfaceFunctionName());
	}

	public static ScilabList createScilabDocProperties(BasicBlock block) {
		ScilabList result = new ScilabList();
		// Store UID in doc so that Scilab will now it without being disturbed.
		result.add(new ScilabString(block.getId()));

		return result;
	}

	public static ScilabMList getAsScilabObj(BasicBlock block) {
		String[] objFields = {"Block", "graphics", "model", "gui", "doc"};
		ScilabMList obj = new ScilabMList(objFields);

		obj.add(BasicBlockInfo.createScilabGraphicsProperties(block));
		obj.add(BasicBlockInfo.createScilabModelProperties(block));
		obj.add(BasicBlockInfo.createScilabGuiProperties(block));
		obj.add(BasicBlockInfo.createScilabDocProperties(block));

		return obj;
	}

	public static List<InputPort> getAllInputPorts(BasicBlock block) {
		List<InputPort> data = new ArrayList<InputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof InputPort) {
				data.add((InputPort) block.getChildAt(i));
			}
		}

		return data;
	}

	public static List<ExplicitInputPort> getAllExplicitInputPorts(BasicBlock block) {
		List<ExplicitInputPort> data = new ArrayList<ExplicitInputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof ExplicitInputPort) {
				data.add((ExplicitInputPort) block.getChildAt(i));
			}
		}
		return data;
	}

	public static List<ImplicitInputPort> getAllImplicitInputPorts(BasicBlock block) {
		List<ImplicitInputPort> data = new ArrayList<ImplicitInputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof ImplicitInputPort) {
				data.add((ImplicitInputPort) block.getChildAt(i));
			}
		}
		return data;
	}

	public static List<OutputPort> getAllOutputPorts(BasicBlock block) {
		List<OutputPort> data = new ArrayList<OutputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof OutputPort) {
				data.add((OutputPort) block.getChildAt(i));
			}
		}

		return data;
	}

	public static List<ExplicitOutputPort> getAllExplicitOutputPorts(BasicBlock block) {
		List<ExplicitOutputPort> data = new ArrayList<ExplicitOutputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof ExplicitOutputPort) {
				data.add((ExplicitOutputPort) block.getChildAt(i));
			}
		}
		return data;
	}

	public static List<ImplicitOutputPort> getAllImplicitOutputPorts(BasicBlock block) {
		List<ImplicitOutputPort> data = new ArrayList<ImplicitOutputPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof ImplicitOutputPort) {
				data.add((ImplicitOutputPort) block.getChildAt(i));
			}
		}
		return data;
	}

	public static List<CommandPort> getAllCommandPorts(BasicBlock block) {
		List<CommandPort> data = new ArrayList<CommandPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof CommandPort) {
				data.add((CommandPort) block.getChildAt(i));
			}
		}

		return data;
	}

	public static List<ControlPort> getAllControlPorts(BasicBlock block) {
		List<ControlPort> data = new ArrayList<ControlPort>();
		int childrenCount = block.getChildCount();

		for (int i = 0 ; i < childrenCount ; ++i) {
			if(block.getChildAt(i) instanceof ControlPort) {
				data.add((ControlPort) block.getChildAt(i));
			}
		}

		return data;
	}
}
