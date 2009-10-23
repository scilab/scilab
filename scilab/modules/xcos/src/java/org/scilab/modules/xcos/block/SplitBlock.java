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
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;



public class SplitBlock extends BasicBlock {

	BasicPort in;
	BasicPort out1;
	BasicPort out2;
	
	public SplitBlock() {
		super();
		setVertex(false);
		setVisible(false);
	}
	
	protected SplitBlock(String label) {
		super(label);
		setInterfaceFunctionName("SPLIT_f");
		getGeometry().setWidth(0);
		getGeometry().setHeight(0);
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
			ExplicitInputPort tmp = new ExplicitInputPort();
			in = tmp;
			addPort(tmp);
		}else if(source instanceof ImplicitOutputPort){
			ImplicitInputPort tmp = new ImplicitInputPort();
			in = tmp;
			addPort(tmp);
		}else if(source instanceof CommandPort){
			ControlPort tmp = new ControlPort();
			in = tmp;
			addPort(tmp);
		}

		//target1
		if(target1 instanceof ExplicitInputPort){
			ExplicitOutputPort tmp = new ExplicitOutputPort();
			out1 = tmp;
			addPort(tmp);
		}else if(target1 instanceof ImplicitInputPort){
			ImplicitOutputPort tmp = new ImplicitOutputPort();
			out1 = tmp;
			addPort(tmp);
		}else if(target1 instanceof ControlPort){
			CommandPort tmp = new CommandPort();
			out1 = tmp;
			addPort(tmp);
		}
		
		//target2
		if(target2 instanceof ExplicitInputPort){
			ExplicitOutputPort tmp = new ExplicitOutputPort();
			out2 = tmp;
			addPort(tmp);
		}else if(target2 instanceof ImplicitInputPort){
			ImplicitOutputPort tmp = new ImplicitOutputPort();
			out2 = tmp;
			addPort(tmp);
		}else if(target2 instanceof ControlPort){
			CommandPort tmp = new CommandPort();
			out2 = tmp;
			addPort(tmp);
		}

		in.setVisible(false);
		out1.setVisible(false);
		out2.setVisible(false);
	}
	
	public BasicPort getIn(){
		return in;
	}

	public BasicPort getOut1(){
		return out1;
	}

	public BasicPort getOut2(){
		return out2;
	}
	
}
