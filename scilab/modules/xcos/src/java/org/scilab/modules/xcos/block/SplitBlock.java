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

import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxGeometry;



public final class SplitBlock extends BasicBlock {

    private static final long serialVersionUID = 5817243367840540106L;

	public SplitBlock() {
		super();
		setVertex(false);
		setVisible(false);
	}

	protected SplitBlock(String label) {
		super(label);
		setInterfaceFunctionName("SPLIT_f");
		setSimulationFunctionName("lsplit");
		setRealParameters(new ScilabDouble());
		setIntegerParameters(new ScilabDouble());
		setObjectsParameters(new ScilabList());
		setExprs(new ScilabDouble());
	}

	// SPLIT_f <-> lsplit
	// CLKSPLIT_f <-> split
	// IMPSPLIT_F <-> limpsplit

	public SplitBlock(String label, BasicPort source, BasicPort target1,
			BasicPort target2) {
		this(label);

		//source
		if(source instanceof ExplicitOutputPort){
			addPort(new ExplicitInputPort());
		}else if(source instanceof ImplicitOutputPort || source instanceof ImplicitInputPort){
			addPort(new ImplicitInputPort());
		}else if(source instanceof CommandPort){
			addPort(new ControlPort());
		}

		//target1 -> add 3 output ports
		if(target1 instanceof ExplicitInputPort){
			addPort(new ExplicitOutputPort());
			addPort(new ExplicitOutputPort());
			addPort(new ExplicitOutputPort());
		}else if(target1 instanceof ImplicitOutputPort || target1 instanceof ImplicitInputPort){
			addPort(new ImplicitOutputPort());
			addPort(new ImplicitOutputPort());
			addPort(new ImplicitOutputPort());
		}else if(target1 instanceof ControlPort){
			addPort(new CommandPort());
			addPort(new CommandPort());
			addPort(new CommandPort());
		}
		
		getChildAt(0).setVisible(false);
		getChildAt(1).setVisible(false);
		getChildAt(2).setVisible(false);
	}

	public void addPort(CommandPort port) {
		super.addPort(port);
		port.setVisible(false);
	}

	public void addPort(ControlPort port) {
		super.addPort(port);
		port.setVisible(false);
	}

	public void addPort(InputPort port) {
		super.addPort(port);
		port.setVisible(false);
	}

	public void addPort(OutputPort port) {
		super.addPort(port);
		port.setVisible(false);
	}

	public BasicPort getIn(){
		return (BasicPort)getChildAt(0);
	}

	public BasicPort getOut1(){
		return (BasicPort)getChildAt(1);
	}

	public BasicPort getOut2(){
		return (BasicPort)getChildAt(2);
	}

	public void unlinkAndClean() {
	
		Object[] objs = getParentDiagram().getAllEdges(new Object[]{getChildAt(0),getChildAt(1),getChildAt(2)});
		getParentDiagram().getModel().beginUpdate();
		for(int i = 0 ; i < objs.length ; i++){
			if(objs[i] instanceof BasicLink){
				BasicLink link = (BasicLink)objs[i];
					getParentDiagram().getModel().remove(link);
			}
		}
		getParentDiagram().getModel().endUpdate();
	}

	public void setGeometry(mxGeometry geometry) {
		if(geometry != null){
			geometry.setWidth(7);
			geometry.setHeight(7);
		}
		super.setGeometry(geometry);
	}

}
