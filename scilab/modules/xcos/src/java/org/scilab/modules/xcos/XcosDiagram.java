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

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.hdf5.scilabTypes.ScilabBoolean;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabTList;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.actions.XcosShortCut;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.PortCheck;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxMultiplicity;

public class XcosDiagram extends ScilabGraph {

    // Default values : SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
    private double[][] wpar = {{600,450,0,0,600,450}}; 
    private String title = "Untitled";
    private double finalIntegrationTime = 100000;
    private double integratorAbsoluteTolerance = 1e-4;
    private double integratorRelativeTolerance = 1e-6;
    private double toleranceOnTime = 1e-10;
    private double maxIntegrationTimeinterval = finalIntegrationTime + 1;
    private double realTimeScaling = 0;
    private double solver = 0;
    private double maximumStepSize = 0;
    private String context = "";
    private List doc = null;


    public Object addEdge(Object edge, Object parent, Object source,
	    Object target, Integer index)
    {	
	// Command -> Control
	if(source instanceof CommandPort) {
	    if (target instanceof ControlPort) {
		return super.addEdge(new CommandControlLink(), parent, source, target, index);
	    }
	}

	// Control -> Command
	// Switch source and target !
	if(target instanceof CommandPort) {
	    if (source instanceof ControlPort) {
		return super.addEdge(new CommandControlLink(), parent, target, source, index);
	    }
	}

	// ExplicitOutput -> ExplicitInput
	if (source instanceof ExplicitOutputPort) {
	    if(target instanceof ExplicitInputPort) {
		return super.addEdge(new ExplicitLink(), parent, source, target, index);
	    }
	}
	// ExplicitInput -> ExplicitOutput
	// Switch source and target !
	if(target instanceof ExplicitOutputPort) {
	    if (source instanceof ExplicitInputPort) {
		return super.addEdge(new ExplicitLink(), parent, target, source, index);
	    }
	}

	// ImplicitOutput -> ImplicitInput
	if (source instanceof ImplicitOutputPort) {
	    if(target instanceof ImplicitInputPort) {
		return super.addEdge(new ImplicitLink(), parent, source, target, index);
	    }
	}
	// ImplicitInput -> ImplicitOutput
	// Switch source and target !
	if(target instanceof ImplicitOutputPort) {
	    if (source instanceof ImplicitInputPort) {
		return super.addEdge(new ImplicitLink(), parent, target, source, index);
	    }
	}

	return null;
    }

    public XcosDiagram() {
	super();
	keyboardHandler = new XcosShortCut(this);
	mxCodec codec = new mxCodec();
	Document doc = mxUtils.loadDocument(System.getenv("SCI")+"/modules/xcos/etc/Xcos-style.xml");
	codec.decode(doc.getDocumentElement(), getStylesheet());
	
	// This enable stop editing cells when pressing Enter.
	getAsComponent().setEnterStopsCellEditing(true);

	// Forbid disconnecting cells once it is connected.
	//setCellsDisconnectable(false);

	// Forbid pending edges.
	setAllowDanglingEdges(false);

	// Cannot connect port to itself.
	setAllowLoops(false);

	setCellsResizable(true);

	setCellsEditable(false);

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

	getAsComponent().getGraphControl().addMouseListener(new MouseListener() {

	    public void mouseClicked(MouseEvent arg0) {
		Object cell = getAsComponent().getCellAt(arg0.getX(), arg0.getY());

		if (arg0.getClickCount() >= 2 && cell != null)
		{
		    if (cell instanceof BasicBlock) {
			BasicBlock block = (BasicBlock) cell;
			block.openBlockSettings();
		    }
		}
		else {
		    System.out.println("ClickCount="+arg0.getClickCount());
		}

	    }

	    public void mouseEntered(MouseEvent arg0) {
		// TODO Auto-generated method stub

	    }

	    public void mouseExited(MouseEvent arg0) {
		// TODO Auto-generated method stub

	    }

	    public void mousePressed(MouseEvent arg0) {
		// TODO Auto-generated method stub

	    }

	    public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub

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
    public boolean isCellFoldable(Object cell, boolean collapse) {
	return !(cell instanceof BasicBlock) && super.isCellFoldable(cell, collapse);
    }

    public boolean isCellMovable(Object cell) {
	return !(cell instanceof BasicPort) && super.isCellMovable(cell);
    }

    public boolean isCellResizable(Object cell) {
	return (cell instanceof BasicBlock) && super.isCellResizable(cell);
    }
    
    public boolean isCellDeletable(Object cell) {
	return !(cell instanceof BasicPort) && super.isCellDeletable(cell);
    }
    
    //    public boolean isCellConnectable(Object cell)
    //    {
    //	return (cell instanceof InputPort) && super.isCellConnectable(cell);
    //    }

    public void dumpToHdf5File(String fileName) {
	if (fileName == null) {
	    FileChooser fc = ScilabFileChooser.createFileChooser();
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || 
		    fc.getSelection().length == 0 ||
		    fc.getSelection()[0].isEmpty()) {
		return;
	    }
	    fileName = fc.getSelection()[0];
	    System.out.println("Saving to file : {"+fileName+"}");
	}

	int file_id = H5Write.createFile(fileName);
	String[] diagramFields = {"diagram", "props", "objs", "version"};

	ScilabMList data = new ScilabMList(diagramFields);
	data.add(getDiagramProps());
	data.add(getDiagramObjs());
	data.add(getDiagramVersion());

	H5Write.writeInDataSet(file_id, "scs_m", data);
	H5Write.closeFile(file_id);
    }

    private ScilabTList getDiagramProps() {
	String[] propsFields = {"params", "wpar", "title", "tol", "tf", "context", "void1", "options", "void2", "void3", "doc"};
	ScilabTList data = new ScilabTList(propsFields);
	data.add(new ScilabDouble(wpar)); // wpar
	data.add(new ScilabString(title)); // title
	data.add(new ScilabDouble(createTol())); // tol
	data.add(new ScilabDouble(finalIntegrationTime)); // tf
	data.add(new ScilabString(context)); // context
	data.add(new ScilabDouble()); // void1
	data.add(getDiagramOptions()); // options
	data.add(new ScilabDouble()); // void2
	data.add(new ScilabDouble()); // void3
	data.add(new ScilabList()); // doc

	return data;
    }

    private double[][] createTol() {
	double[][] tol = {{integratorAbsoluteTolerance, integratorRelativeTolerance, toleranceOnTime, maxIntegrationTimeinterval, realTimeScaling, solver, maximumStepSize}};
	return tol;
    }

    private ScilabTList getDiagramOptions() {
	String[] optionsFields = {"scsopt", "3D", "Background", "Link", "ID", "Cmap"};

	ScilabTList data = new ScilabTList(optionsFields);
	ScilabList _3D = new ScilabList();
	_3D.add(new ScilabBoolean(true));
	_3D.add(new ScilabDouble(33));

	double[][] background = {{8, 1}};
	double[][] link = {{1,5}};

	ScilabList ID = new ScilabList();
	double[][] ID_1 = {{5,1}};
	double[][] ID_2 = {{4,1}};
	ID.add(new ScilabDouble(ID_1));
	ID.add(new ScilabDouble(ID_2));
	double[][] Cmap = {{0.8, 0.8, 0.8}};

	data.add(_3D); // 3D
	data.add(new ScilabDouble(background)); // Background
	data.add(new ScilabDouble(link)); // Link
	data.add(ID); // ID
	data.add(new ScilabDouble(Cmap)); // Cmap
	return data;
    }

    private ScilabList getDiagramObjs() {
	ScilabList data = new ScilabList();
	
	int nbObjs = getModel().getChildCount(getDefaultParent());
	
	
	List<BasicBlock> blockList = new ArrayList<BasicBlock>();
	List<BasicLink> linkList = new ArrayList<BasicLink>();
	for (int i = 0 ; i < nbObjs ; ++i)
	{
	    Object currentObject = getModel().getChildAt(getDefaultParent(), i);
	    if (currentObject instanceof BasicBlock) {
		blockList.add((BasicBlock) currentObject);
	    }
	    else if (currentObject instanceof BasicLink) {
		linkList.add((BasicLink) currentObject);
	    }
	    else {
		System.out.println("Not a BasicBlock nor BasicLink");
	    }
	}

	
	
	// Go over all list to set ID
	for (int i = 0 ; i < linkList.size() ; ++i) {
	    linkList.get(i).setOrdering(i + blockList.size() + 1);
	}

	// Go over all blocks to dump it inside Scilab Structure
	for (int i = 0 ; i < blockList.size() ; ++i) {
	    blockList.get(i).setOrdering(i + 1);
	    data.add(blockList.get(i).getAsScilabObj());
	}

	// Go over all link to dump it inside Scilab Structure
	for (int i = 0 ; i < linkList.size() ; ++i) {
	    data.add(linkList.get(i).getAsScilabObj());
	}


	return data;
    }

    private ScilabString getDiagramVersion() {
	return new ScilabString("scicos4.2");
    }

    public double getFinalIntegrationTime() {
	return finalIntegrationTime;
    }

    public void setFinalIntegrationTime(double finalIntegrationTime) {
	this.finalIntegrationTime = finalIntegrationTime;
    }

    public double getIntegratorAbsoluteTolerance() {
	return integratorAbsoluteTolerance;
    }

    public void setIntegratorAbsoluteTolerance(double integratorAbsoluteTolerance) {
	this.integratorAbsoluteTolerance = integratorAbsoluteTolerance;
    }

    public double getIntegratorRelativeTolerance() {
	return integratorRelativeTolerance;
    }

    public void setIntegratorRelativeTolerance(double integratorRelativeTolerance) {
	this.integratorRelativeTolerance = integratorRelativeTolerance;
    }

    public double getMaximumStepSize() {
	return maximumStepSize;
    }

    public void setMaximumStepSize(double maximumStepSize) {
	this.maximumStepSize = maximumStepSize;
    }

    public double getMaxIntegrationTimeinterval() {
	return maxIntegrationTimeinterval;
    }

    public void setMaxIntegrationTimeinterval(double maxIntegrationTimeinterval) {
	this.maxIntegrationTimeinterval = maxIntegrationTimeinterval;
    }

    public double getRealTimeScaling() {
	return realTimeScaling;
    }

    public void setRealTimeScaling(double realTimeScaling) {
	this.realTimeScaling = realTimeScaling;
    }

    public double getSolver() {
	return solver;
    }

    public void setSolver(double solver) {
	this.solver = solver;
    }

    public double getToleranceOnTime() {
	return toleranceOnTime;
    }

    public void setToleranceOnTime(double toleranceOnTime) {
	this.toleranceOnTime = toleranceOnTime;
    }

}

