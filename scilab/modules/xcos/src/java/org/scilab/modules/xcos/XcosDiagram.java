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

package org.scilab.modules.xcos;

import java.util.Arrays;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.xcos.actions.XcosShortCut;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.clock.ClockBlock;
import org.scilab.modules.xcos.port.PortCheck;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxMultiplicity;

public class XcosDiagram extends ScilabGraph {

    public XcosDiagram() {
	super();
	keyboardHandler = new XcosShortCut(getAsComponent());
	mxCodec codec = new mxCodec();
	Document doc = mxUtils.loadDocument(System.getenv("SCI")+"/modules/xcos/etc/Xcos-style.xml");
	codec.decode(doc.getDocumentElement(), getStylesheet());
	Object parent = getDefaultParent();

	/**
	 * SAMPLE DATA 
	 */

	ClockBlock clock = new ClockBlock();
	clock.getGeometry().setX(20);
	clock.getGeometry().setY(20);
	addCell(clock, parent);

	getAsComponent().setCellWarning(clock, "ACHTUNG !!!");
	
//	SinusoidBlock sinusoid = new SinusoidBlock();
//	sinusoid.getGeometry().setX(220);
//	sinusoid.getGeometry().setY(120);
//	addCell(sinusoid,parent);
//	
//	GenericBlock fake = new GenericBlock();
//	fake.getGeometry().setX(20);
//	fake.getGeometry().setY(420);
//	addCell(fake, parent);
//	
//	ScopeBlock scope = new ScopeBlock();
//	scope.getGeometry().setX(220);
//	scope.getGeometry().setY(420);
//	addCell(scope, parent);
	
	/**
	 * END
	 */

	// This enable stop editing cells when pressing Enter.
	getAsComponent().setEnterStopsCellEditing(true);

	// Forbid disconnecting cells once it is connected.
	//setCellsDisconnectable(false);

	// Forbid pending edges.
	setAllowDanglingEdges(false);

	// Cannot connect port to itself.
	setAllowLoops(false);

	setCellsResizable(false);
	
	setConnectableEdges(false);


	mxMultiplicity[] multiplicities = new mxMultiplicity[1];

	// Command Port as source can only go to Control Port
	multiplicities[0] = new PortCheck(true, "commandPort", "type", "Command", 1,
		"1", Arrays.asList(new Object[] { "Control" }),
		"Command Port Must Have 1 Control Port",
		"Control Port Must Connect to Command Port", true);
	// Control Port must be connected !
	//	multiplicities[1] = new mxMultiplicity(false, "controlPort", null, null, 1,
	//			"n", Arrays.asList(new Object[] {"commandPort"}),
	//			"Block Should be controled",
	//			"Command port should connect to Control", true);


	//	// Source nodes needs 1..2 connected Targets
	//	multiplicities[0] = new mxMultiplicity(true, "Source", null, null, 1,
	//			"2", Arrays.asList(new Object[] { "Target" }),
	//			"Source Must Have 1 or 2 Targets",
	//			"Source Must Connect to Target", true);
	//
	//	// Output Port as source can only go to Input Port
	//	multiplicities[1] = new mxMultiplicity(true, "outputPort", "PortType", "input", 1,
	//			"n", Arrays.asList(new Object[] { "inputPort" }),
	//			"Input Port Must Have 1 Output Port",
	//			"Output Port Must Connect to Input Port", true);
	//	
	//	// Command Port as source can only go to Control Port
	//	multiplicities[3] = new mxMultiplicity(true, "commandPort", null, null, 1,
	//			"1", Arrays.asList(new Object[] { "controlPort" }),
	//			"Command Port Must Have 1 Control Port",
	//			"Control Port Must Connect to Command Port", true);
	//	// Control Port must be connected !
	//	multiplicities[4] = new mxMultiplicity(false, "controlPort", null, null, 1,
	//			"1", Arrays.asList(new Object[] {"commandPort"}),
	//			"Block Should be controled",
	//			"Command port should connect to Control", true);
	//	// Source node does not want any incoming connections
	//	multiplicities[2] = new mxMultiplicity(false, "Source", null, null, 0,
	//			"0", null, "Source Must Have No Incoming Edge", null, true);
	setMultiplicities(multiplicities);
	getModel().addListener(mxEvent.CHANGE, new mxIEventListener()
	{
	    public void invoke(Object sender, mxEventObject evt)
	    {
		getAsComponent().validateGraph();
	    }
	});
    }


    /*
     * Manage Group to be CellFoldable i.e with a (-) to reduce
     * and a (+) to expand them.
     * Only non-Block / non-Port Cell are foldable. 
     * 
     * (non-Javadoc)
     * @see com.mxgraph.view.mxGraph#isCellFoldable(java.lang.Object, boolean)
     */
    public boolean isCellFoldable(Object cell, boolean collapse)
    {
	return !(cell instanceof BasicBlock) && super.isCellFoldable(cell, collapse);
    }

//    public boolean isCellConnectable(Object cell)
//    {
//	return (cell instanceof InputPort) && super.isCellConnectable(cell);
//    }

}

