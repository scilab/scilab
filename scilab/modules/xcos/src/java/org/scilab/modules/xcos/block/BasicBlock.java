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
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DefaultAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabString;
import org.scilab.modules.hdf5.scilabTypes.ScilabType;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosUIDObject;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.block.actions.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.ColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxCellState;

public class BasicBlock extends XcosUIDObject {

    private static final long serialVersionUID = 2189690915516168262L;
    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;
    private transient XcosDiagram parentDiagram = null;
    
    private transient int angle = 0;
    private transient boolean isFlipped = false;
    private transient boolean isMirrored = false;
    

    // TODO : Must make this types evolve, but for now keep a strong link to Scilab
    // !! WARNING !!
    // exprs = [] ; rpar = [] ; ipar = [] ; opar = list()

    //private List<String> exprs = new ArrayList<String>();
    private ScilabType exprs = null;
    //private List<Double> realParameters = new ArrayList<Double>();
    private ScilabType realParameters = null;
    //private List<Integer> integerParameters = new ArrayList<Integer>();
    private ScilabType integerParameters = null;
    //private List objectsParameters = new ArrayList();
    private ScilabType objectsParameters = null;

    private ScilabType nbZerosCrossing = new ScilabDouble();

    private ScilabType nmode = new ScilabDouble();

    private ScilabType state = new ScilabDouble();
    private ScilabType dState = new ScilabDouble();
    private ScilabType oDState = new ScilabDouble();

    private ScilabType equations = null;

    private boolean dependsOnU = false;
    private boolean dependsOnT = false;

    private String blockType = "c";

    private int ordering = 0;
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
	if (label.compareTo("TEXT_f") == 0) {
	    return new TextBlock(label);
	}
	if (label.compareTo("SUPER_f") == 0) {
	    return new SuperBlock(label);
	}
	if (label.compareTo("DSUPER") == 0) {
	    return new SuperBlock(label, true);
	}
	if (label.compareTo("CONST_m") == 0 || label.compareTo("CONST") == 0
		|| label.compareTo("CONST_f") == 0) {
	    return new ConstBlock(label);
	}
	if (label.compareTo("AFFICH_m") == 0
		|| label.compareTo("AFFICH_f") == 0) {
	    return new AfficheBlock(label);
	}
	if (label.compareTo("GAINBLK_f") == 0
		|| label.compareTo("GAINBLK") == 0
		|| label.compareTo("GAIN_f") == 0) {
	    return new GainBlock(label);
	}
	if (label.compareTo("IN_f") == 0) {
	    return new ExplicitInBlock(label);
	}
	if (label.compareTo("OUT_f") == 0) {
	    return new ExplicitOutBlock(label);
	}
	if (label.compareTo("INIMPL_f") == 0) {
	    return new ImplicitInBlock(label);
	}
	if (label.compareTo("OUTIMPL_f") == 0) {
	    return new ImplicitOutBlock(label);
	}
	if (label.compareTo("CLKINV_f") == 0) {
	    return new EventInBlock(label);
	}
	if (label.compareTo("CLKOUTV_f") == 0
		|| label.compareTo("CLKOUT_f") == 0) {
	    return new EventOutBlock(label);
	}
	if (label.compareTo("SPLIT_f") == 0
		|| label.compareTo("IMPSPLIT_f") == 0
		|| label.compareTo("CLKSPLIT_f") == 0) {
	    return new SplitBlock(label);
	} else {
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

    public synchronized boolean isLocked() {
        return locked;
    }

    public synchronized void setLocked(boolean locked) {
        this.locked = locked;
    }

    public void removePort(BasicPort port){
	if(port.getEdgeCount() != 0) {
	    getParentDiagram().removeCells(new Object[]{port.getEdgeAt(0)});
	}
	remove(port);
    }
    
    public void addPort(InputPort port) {
    	insert(port);
    	BlockPositioning.updatePortsPosition(this);
    	port.setOrdering(BasicBlockInfo.getAllInputPorts(this, false).size());
    }

    public void addPort(OutputPort port) {
    	insert(port);
    	BlockPositioning.updatePortsPosition(this);
    	port.setOrdering(BasicBlockInfo.getAllOutputPorts(this, false).size());
    }

    public void addPort(CommandPort port) {
    	insert(port);
    	BlockPositioning.updatePortsPosition(this);
    	port.setOrdering(BasicBlockInfo.getAllCommandPorts(this, false).size());
    }

    public void addPort(ControlPort port) {
    	insert(port);
    	BlockPositioning.updatePortsPosition(this);
    	port.setOrdering(BasicBlockInfo.getAllControlPorts(this, false).size());
    }

    public ScilabDouble getAllCommandPortsInitialStates() {
	if (BasicBlockInfo.getAllCommandPorts(this, false).isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[BasicBlockInfo.getAllCommandPorts(this, false).size()][1];
	for (int i = 0 ; i < BasicBlockInfo.getAllCommandPorts(this, false).size() ; ++i) {
	    data[i][0] = BasicBlockInfo.getAllCommandPorts(this, false).get(i).getInitialState();
	}

	return new ScilabDouble(data);
    }

    public ScilabType getSimulationFunctionNameAndType() {
	if (getSimulationFunctionType() == SimulationFunctionType.DEFAULT) {
	    return new ScilabString(getSimulationFunctionName());
	}
	ScilabList data = new ScilabList();

	data.add(new ScilabString(getSimulationFunctionName()));
	data.add(new ScilabDouble(getSimulationFunctionType().getAsDouble()));

	return data;
    }

    /**
     * Does the block update and register on the undo manager 
     * @param modifiedBlock the new settings
     */
    public void updateBlockSettings(BasicBlock modifiedBlock) {
	
	/* TODO: emit changes on update */
//	mxUndoableEdit edit = new mxUndoableEdit(getParentDiagram().getModel()) {
//	    public void dispatch()
//		{
//			((mxGraphModel) source).fireEvent(mxEvent.CHANGE,
//					new mxEventObject(new Object[] { changes }));
//		}
//	};
//	edit.add(new BlockChange(modifiedBlock, this));
	
	doUpdateBlockSettings(modifiedBlock);
    }

    /**
     * Does the block update without using the undo manager 
     * @param modifiedBlock the new settings
     */
    public void doUpdateBlockSettings(BasicBlock modifiedBlock) {
	setDependsOnT(modifiedBlock.dependsOnT());
	setDependsOnU(modifiedBlock.dependsOnU());
	setExprs(modifiedBlock.getExprs());

	setRealParameters(modifiedBlock.getRealParameters());
	setIntegerParameters(modifiedBlock.getIntegerParameters());
	setObjectsParameters(modifiedBlock.getObjectsParameters());

	setState(modifiedBlock.getState());
	setDState(modifiedBlock.getDState());
	setODState(modifiedBlock.getODState());

	setEquations(modifiedBlock.getEquations());


	List<? extends BasicPort> modifiedPorts = null;
	List<? extends BasicPort> ports = null;
	
	// Check if new input port have been added
	if ((modifiedPorts = BasicBlockInfo.getAllInputPorts(modifiedBlock, false)).size() > (ports = BasicBlockInfo.getAllInputPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllInputPorts(this, false)).size() < modifiedPorts.size()) {
		addPort((InputPort)modifiedPorts.get(ports.size()));
	    }
	}
	// Check if input ports have been removed
	else if ((modifiedPorts = BasicBlockInfo.getAllInputPorts(modifiedBlock, false)).size() < (ports = BasicBlockInfo.getAllInputPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllInputPorts(this, false)).size() > modifiedPorts.size()) {
		removePort((BasicPort)ports.get(ports.size() - 1));
	    }
	}

	// Check if new output port have been added
	if ((modifiedPorts = BasicBlockInfo.getAllOutputPorts(modifiedBlock, false)).size() > (ports = BasicBlockInfo.getAllOutputPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllOutputPorts(this, false)).size() < modifiedPorts.size()) {
		addPort((OutputPort)modifiedPorts.get(ports.size()));
	    }
	}
	// Check if output ports have been removed
	else if ((modifiedPorts = BasicBlockInfo.getAllOutputPorts(modifiedBlock, false)).size() < (ports = BasicBlockInfo.getAllOutputPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllOutputPorts(this, false)).size() > modifiedPorts.size()) {
		removePort((BasicPort)ports.get(ports.size() - 1));
	    }
	}


	// Check if new command port have been added
	if ((modifiedPorts = BasicBlockInfo.getAllCommandPorts(modifiedBlock, false)).size() > (ports = BasicBlockInfo.getAllCommandPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllCommandPorts(this, false)).size() < modifiedPorts.size()) {
		addPort((CommandPort)modifiedPorts.get(ports.size()));
	    }
	}
	// Check if command ports have been removed
	else if ((modifiedPorts = BasicBlockInfo.getAllCommandPorts(modifiedBlock, false)).size() < (ports = BasicBlockInfo.getAllCommandPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllCommandPorts(this, false)).size() > modifiedPorts.size()) {
		removePort((BasicPort)ports.get(ports.size() - 1));
	    }
	}

	// Check if new control port have been added
	if ((modifiedPorts = BasicBlockInfo.getAllControlPorts(modifiedBlock, false)).size() > (ports = BasicBlockInfo.getAllControlPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllControlPorts(this, false)).size() < modifiedPorts.size()) {
		addPort((ControlPort)modifiedPorts.get(ports.size()));
	    }
	}
	// Check if control ports have been removed
	else if ((modifiedPorts = BasicBlockInfo.getAllControlPorts(modifiedBlock, false)).size() < (ports = BasicBlockInfo.getAllControlPorts(this, false)).size()) {
	    while((ports = BasicBlockInfo.getAllControlPorts(this, false)).size() > modifiedPorts.size()) {
		removePort((BasicPort)ports.get(ports.size() - 1));
	    }
	}

	/*
	 * If the block is in a superblock then update it.
	 */
	if (getParentDiagram() instanceof SuperBlockDiagram) {
	    SuperBlock parentBlock = ((SuperBlockDiagram) getParentDiagram()).getContainer();
	    parentBlock.getParentDiagram().fireEvent(XcosEvent.SUPER_BLOCK_UPDATED,new mxEventObject(new Object[] { parentBlock }));
	}
	
    }

    public void openBlockSettings(String context[]) {
	
	if(getParentDiagram() instanceof PaletteDiagram) {
	    return;
	}
	
	//prevent to open twice
	if(isLocked()) {
	    return;
	}
	
	final File tempOutput;
	final File tempInput;
	final File tempContext;
	try {
	    tempInput = File.createTempFile("xcos",".h5");
	    tempInput.deleteOnExit();

	    // Write scs_m
	    tempOutput = exportBlockStruct();
	    // Write context
	    tempContext = exportContext(context);

	    String cmd;
	    
	    cmd = "xcosBlockInterface(\""+tempOutput.getAbsolutePath()+"\"";
	    cmd += ", \""+tempInput.getAbsolutePath()+"\"";
	    cmd += ", "+getInterfaceFunctionName();
	    cmd += ", \"set\"";
	    cmd += ", \""+tempContext.getAbsolutePath()+"\");";
	    
	    InterpreterManagement.putCommandInScilabQueue(cmd);
	    final BasicBlock currentBlock = this;
	    Thread launchMe = new Thread() {
		public void run() {
		    Signal.wait(tempInput.getAbsolutePath());
		    // Now read new Block
		    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
		    updateBlockSettings(modifiedBlock);
		    getParentDiagram().fireEvent(XcosEvent.ADD_PORTS, new mxEventObject(new Object[] {currentBlock}));
		    setLocked(false);
		}
	    };
	    launchMe.start();
	    setLocked(true);

	} catch (IOException e) {
	    e.printStackTrace();
	}
    }

    protected File exportBlockStruct() {

	// Write scs_m
	File tempOutput;
	try {
	    tempOutput = File.createTempFile("xcos",".h5");
	    tempOutput.deleteOnExit();
	    int file_id = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(file_id, "scs_m", BasicBlockInfo.getAsScilabObj(this));
	    H5Write.closeFile(file_id);
	    return tempOutput;
	} catch (IOException e) {
	    e.printStackTrace();
	} catch (HDF5Exception e) {
	    e.printStackTrace();
	}
	return null;
    }
    
    protected File exportContext(String[] context) {

	// Write context
	try {
	    File tempContext = File.createTempFile("xcos",".h5");
	    tempContext.deleteOnExit();
	    int context_file_id = H5Write.createFile(tempContext.getAbsolutePath());
	    H5Write.writeInDataSet(context_file_id, "context", new ScilabString(context));
	    H5Write.closeFile(context_file_id);
	    return tempContext;
	} catch (IOException e) {
	    e.printStackTrace();
	} catch (HDF5Exception e) {
	    e.printStackTrace();
	}
	return null;
    }
    
    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append("<html>");
	result.append("Block Name : "+ getInterfaceFunctionName() + "<br>");
	result.append("Simulation : "+ getSimulationFunctionName() + "<br>");

	if(getParentDiagram() instanceof PaletteDiagram) {
	    if(getIntegerParameters() != null) {
		result.append("Integer parameters : "+ getIntegerParameters() + "<br>");
	    }
	    
	    if(getRealParameters() != null && getRealParameters().getHeight() != 0 && getRealParameters().getWidth() != 0) {
		result.append("Real parameters : "+ getRealParameters() + "<br>");
	    }
	    
	    if(getObjectsParameters() != null) {
		result.append("Object parameters : "+ getObjectsParameters() + "<br>");
	    }
	} else {
	    result.append("UID : "+ getId() + "<br>");
	    result.append("Block Style : " + getStyle() + "<br>");
	    result.append("Flip : " + getFlip() + "<br>");
	    result.append("Mirror : " + getMirror() + "<br>");
	    result.append("Input ports : " + BasicBlockInfo.getAllInputPorts(this, false).size() + "<br>");
	    result.append("Output ports : " + BasicBlockInfo.getAllOutputPorts(this, false).size() + "<br>");
	    result.append("Control ports : " + BasicBlockInfo.getAllControlPorts(this, false).size() + "<br>");
	    result.append("Command ports : " + BasicBlockInfo.getAllCommandPorts(this, false).size() + "<br>");
	}

	result.append("x : " + getGeometry().getX() + "<br>");
	result.append("y : " + getGeometry().getY() + "<br>");
	result.append("w : " + getGeometry().getWidth() + "<br>");
	result.append("h : " + getGeometry().getHeight() + "<br>");
	result.append("</html>");
	return result.toString();
    }

    public void openContextMenu(ScilabGraph graph) {
	ContextMenu menu = null;
	if(getParentDiagram() instanceof PaletteDiagram) {
	    menu = createPaletteContextMenu(graph);
	} else {
	    menu = createContextMenu(graph);
	}
	menu.setVisible(true);
    }

    public ContextMenu createPaletteContextMenu(ScilabGraph graph) {
	ContextMenu menu = ScilabContextMenu.createContextMenu();

	final List<XcosDiagram> allDiagrams = Xcos.getDiagrams();

	if (allDiagrams.size() == 0) {
	    // No diagram opened: should never happen if Xcos opens an empty diagram when it is launched
	    MenuItem addTo = ScilabMenuItem.createMenuItem();

	    addTo.setText(XcosMessages.ADDTO_NEW_DIAGRAM);
	    addTo.setCallback(new CallBack(XcosMessages.ADDTO_NEW_DIAGRAM) {
		private static final long serialVersionUID = 8370536280449900878L;

		public void callBack() {
		    XcosDiagram theDiagram = Xcos.createEmptyDiagram();
		    BasicBlock block = (BasicBlock)BasicBlock.this.createClone();
		    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
		    mxGeometry geom = BasicBlock.this.getGeometry();
		    geom.setX(10);
		    geom.setY(10);
		    theDiagram.getModel().setGeometry(block, geom);
		    BlockPositioning.updateBlockView(block);
		}
	    });

	    menu.add(addTo);

	} else if (allDiagrams.size() == 1) {
	    // A single diagram opened: add to this diagram
	    MenuItem addTo = ScilabMenuItem.createMenuItem();

	    addTo.setText(XcosMessages.ADDTO + " " + allDiagrams.get(0).getParentTab().getName());
	    final XcosDiagram theDiagram = allDiagrams.get(0);
	    addTo.setCallback(new CallBack(theDiagram.getTitle()) {
		private static final long serialVersionUID = -99601763227525686L;

		public void callBack() {
		    BasicBlock block = (BasicBlock)BasicBlock.this.createClone();
		    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
		    mxGeometry geom = BasicBlock.this.getGeometry();
		    geom.setX(10);
		    geom.setY(10);
		    theDiagram.getModel().setGeometry(block, geom);
		    BlockPositioning.updateBlockView(block);
		    block.setParentDiagram(theDiagram);
		}
	    });

	    menu.add(addTo);

	} else {
	    // The user has to choose
	    Menu addTo = ScilabMenu.createMenu();

	    addTo.setText(XcosMessages.ADDTO);

	    for (int i = 0; i < allDiagrams.size(); i++) {
		MenuItem diagram = ScilabMenuItem.createMenuItem();
		final XcosDiagram theDiagram = allDiagrams.get(i);
		diagram.setText(allDiagrams.get(i).getParentTab().getName());
		diagram.setCallback(new CallBack(theDiagram.getTitle()) {
		    private static final long serialVersionUID = 3345416658377835057L;

		    public void callBack() {
			BasicBlock block = (BasicBlock)BasicBlock.this.createClone();
			theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
			mxGeometry geom = BasicBlock.this.getGeometry();
			geom.setX(10);
			geom.setY(10);
			theDiagram.getModel().setGeometry(block, geom);
			BlockPositioning.updateBlockView(block);
		    }
		});
		addTo.add(diagram);
	    }

	    menu.add(addTo);
	}


	menu.getAsSimpleContextMenu().addSeparator();

	MenuItem help = ScilabMenuItem.createMenuItem();
	help.setText(XcosMessages.BLOCK_DOCUMENTATION);
	help.setCallback(new CallBack(XcosMessages.BLOCK_DOCUMENTATION) {
	    private static final long serialVersionUID = -1480947262397441951L;

	    public void callBack() {
		InterpreterManagement.requestScilabExec("help " + getInterfaceFunctionName());
	    }
	});
	menu.add(help);

	menu.setVisible(true);

	((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
		MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
	
	return menu;
    }

    public ContextMenu createContextMenu(ScilabGraph graph) {
		ContextMenu menu = ScilabContextMenu.createContextMenu();
		Map<Class<? extends DefaultAction>, Menu> menuList = new HashMap<Class<? extends DefaultAction>, Menu>();
		
		MenuItem value = BlockParametersAction.createMenu(graph);
		menuList.put(BlockParametersAction.class, value);
		menu.add(value);
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		value = CutAction.cutMenu(graph);
		menuList.put(CutAction.class, value);
		menu.add(value);
		value = CopyAction.copyMenu(graph);
		menuList.put(CopyAction.class, value);
		menu.add(value);
		value = DeleteAction.createMenu(graph);
		menuList.put(DeleteAction.class, value);
		menu.add(value);
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		value = RegionToSuperblockAction.createMenu(graph);
		menuList.put(RegionToSuperblockAction.class, value);
		menu.add(value);
//		Menu mask = ScilabMenu.createMenu();
//		mask.setText(XcosMessages.SUPERBLOCK_MASK);
//		menu.add(mask);
//		mask.add(SuperblockMaskCreateAction.createMenu(graph));
//		mask.add(SuperblockMaskRemoveAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		Menu format = ScilabMenu.createMenu();
		format.setText(XcosMessages.FORMAT);
		menu.add(format);
		value = RotateAction.createMenu(graph);
		menuList.put(RotateAction.class, value);
		format.add(value);
		value = MirrorAction.createMenu(graph);
		menuList.put(MirrorAction.class, value);
		format.add(value);
		value = FlipAction.createMenu(graph);
		menuList.put(FlipAction.class, value);
		format.add(value);
		value = ShowHideShadowAction.createMenu(graph);
		menuList.put(ShowHideShadowAction.class, value);
		format.add(value);
		/*--- */
		format.addSeparator();
		/*--- */
		Menu alignMenu = ScilabMenu.createMenu();
		alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_LEFT, mxConstants.ALIGN_LEFT));
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_CENTER, mxConstants.ALIGN_CENTER));
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_RIGHT, mxConstants.ALIGN_RIGHT));
		alignMenu.addSeparator();
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_TOP, mxConstants.ALIGN_TOP));
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_MIDDLE, mxConstants.ALIGN_MIDDLE));
		alignMenu.add(AlignBlockAction.createMenu(graph, XcosMessages.ALIGN_BOTTOM, mxConstants.ALIGN_BOTTOM));
		menuList.put(AlignBlockAction.class, alignMenu);
		format.add(alignMenu);
		/*--- */
		format.addSeparator();
		/*--- */
		format.add(ColorAction.createMenu(graph, XcosMessages.BORDER_COLOR, mxConstants.STYLE_STROKECOLOR));
		format.add(ColorAction.createMenu(graph, XcosMessages.FILL_COLOR, mxConstants.STYLE_FILLCOLOR));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(ViewDetailsAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(BlockDocumentationAction.createMenu(graph));

		((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
		
		customizeMenu(menuList);
		
		return menu;
    }
    
    public void setFlip(boolean flip) {
	if(getParentDiagram() != null) {
	    isFlipped = flip;
	    if(flip == true) {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, "true");
	    } else {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, "false");
	    }
	}
    }

    /**
     * Override this to customize contextual menu
     * @param menuList
     */
    protected void customizeMenu(Map<Class<? extends DefaultAction>, Menu> menuList) {
	// To be overridden by sub-classes
    }
    

    public boolean getMirror(){
	return isMirrored;
    }
    
    public void setMirror(boolean mirror) {
	if(getParentDiagram() != null) {
	    isMirrored = mirror;
	    if(mirror == true) {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_MIRROR, "true");
	    } else {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_MIRROR, "false");
	    }
	}
    }

    public boolean getFlip(){
	return isFlipped;
    }

    public void toggleFlip() {
	BlockPositioning.toggleFlip(this);
    }

    public void toggleMirror() {
	BlockPositioning.toggleMirror(this);
    }

    public void toggleAntiClockwiseRotation() {
	BlockPositioning.toggleAntiClockwiseRotation(this);

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

	    return clone;
	} catch (CloneNotSupportedException e) {
	    e.printStackTrace();
	    return null;
	}
    }

    public int getAngle() {
	return angle;
    }

    public void setAngle(int angle) {
	this.angle = angle;
	
	if(getParentDiagram() != null) {
	    mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_ROTATION, new Integer(angle).toString());
	}
    }

    /**
     * Usefull when we need to update local properties with mxCell style properties 
     */
    public void updateFieldsFromStyle() {
	if (getParentDiagram() != null) {
	    mxCellState state = getParentDiagram().getView().getState(this);
	    if(state != null) {
		// Angle field
		String  currentAngle = mxUtils.getString(state.getStyle(), XcosConstants.STYLE_ROTATION, "0");
		this.angle = Integer.parseInt(currentAngle);
	    
		// Flip field
		String  currentFlip = mxUtils.getString(state.getStyle(), XcosConstants.STYLE_FLIP, "false");
		if (currentFlip.compareTo("true") == 0) {
		    isFlipped = true;
		} else {
		    isFlipped = false;
		}
		
		// Mirror field
		String  currentMirror = mxUtils.getString(state.getStyle(), XcosConstants.STYLE_MIRROR, "false");
		if (currentMirror.compareTo("true") == 0) {
		    isMirrored = true;
		} else {
		    isMirrored = false;
		}
	    }
	    
	}
	
    }
}
