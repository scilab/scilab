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
import org.scilab.modules.hdf5.scilabTypes.ScilabString;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

public class BasicBlock extends mxCell {

    private String interfaceFunctionName = null;
    
    public BasicBlock(String label, String style) {
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

    public void setInterfaceFunctionName(String interfaceFunctionName) {
        this.interfaceFunctionName = interfaceFunctionName;
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
	
	graphics.add(new ScilabDouble(0)); // pin
	
	graphics.add(new ScilabDouble(0)); // pout
	
	graphics.add(new ScilabDouble()); // pein
	
	graphics.add(new ScilabDouble()); // peout

	ScilabList gr_i = new ScilabList();
	ScilabString graphicsInstructions = new ScilabString("xstringb(orig(1),orig(2),\""+getValue()+"\",sz(1),sz(2));");
	gr_i.add(graphicsInstructions);
	gr_i.add(new ScilabDouble(8));
	graphics.add(gr_i); // gr_i
	
	graphics.add(new ScilabString("")); // id
	
	graphics.add(new ScilabString("E")); // in_implicit

	graphics.add(new ScilabString("E")); // out_implicit
	
	return graphics;
    }
    
    private ScilabMList createScilabModelProperties() {
	String[] modelFields = {"model", "sim", "in", "in2", "intyp", "out", "out2", "outtyp", "evtin", "evtout",
		"state", "dstate", "odstate", "rpar", "ipar", "opar", "blocktype", "firing", "dep_ut", "label",
		"nzcross", "nmode", "equations"};
	ScilabMList model = new ScilabMList(modelFields);
	
	model.add(new ScilabString("my_sim_fun")); // sim
	
	model.add(new ScilabDouble()); // in
	
	model.add(new ScilabDouble()); // in2
	
	model.add(new ScilabDouble(1)); // intyp
	
	model.add(new ScilabDouble()); // out
	
	model.add(new ScilabDouble()); // out2
	
	model.add(new ScilabDouble(1)); // outtyp
	
	model.add(new ScilabDouble()); // evtin
	
	model.add(new ScilabDouble()); // evtout
	
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
    
}
