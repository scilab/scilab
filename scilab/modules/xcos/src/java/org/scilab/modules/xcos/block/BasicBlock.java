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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
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
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
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
import org.scilab.modules.xcos.utils.StyleMap;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosInterpreterManagement.InterpreterException;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

public class BasicBlock extends XcosUIDObject {
	private static final long serialVersionUID = 2189690915516168262L;
	private static final String INTERNAL_FILE_PREFIX = "xcos";
	private static final String INTERNAL_FILE_EXTENSION = ".h5";
	
    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;
    private transient XcosDiagram parentDiagram;
    
    private transient int angle;
    private transient boolean isFlipped;
    private transient boolean isMirrored;
    

    // TODO : Must make this types evolve, but for now keep a strong link to Scilab
    // !! WARNING !!
    // exprs = [] ; rpar = [] ; ipar = [] ; opar = list()

    //private List<String> exprs = new ArrayList<String>();
    private ScilabType exprs;
    //private List<Double> realParameters = new ArrayList<Double>();
    private ScilabType realParameters;
    //private List<Integer> integerParameters = new ArrayList<Integer>();
    private ScilabType integerParameters;
    //private List objectsParameters = new ArrayList();
    private ScilabType objectsParameters;

    private ScilabType nbZerosCrossing = new ScilabDouble();

    private ScilabType nmode = new ScilabDouble();

    private ScilabType state = new ScilabDouble();
    private ScilabType dState = new ScilabDouble();
    private ScilabType oDState = new ScilabDouble();

    private ScilabType equations;

    private boolean dependsOnU;
    private boolean dependsOnT;

    private String blockType = "c";

    private int ordering;
    private boolean locked;

	/**
	 * Represent a simulation function type compatible with Scilab/Scicos
	 * function type descriptors.
	 */
	public enum SimulationFunctionType {
		ESELECT(-2.0), IFTHENELSE(-1.0), DEFAULT(0.0), TYPE_1(1.0), TYPE_2(2.0),
		TYPE_3(3.0), C_OR_FORTRAN(4.0), SCILAB(5.0), UNKNOWN(5.0),
		OLDBLOCKS(10001.0);

		private double value;

		/**
		 * Default constructor
		 * 
		 * @param scilabValue
		 *            Scilab/Scicos function type descriptor
		 */
		private SimulationFunctionType(double scilabValue) {
			value = scilabValue;
		}

		/**
		 * Get the Java descriptor from the Scilab descriptor.
		 * 
		 * @param scilabValue
		 *            Scilab/Scicos function type descriptor
		 * @return The corresponding java descriptor
		 */
		public static SimulationFunctionType convertScilabValue(int scilabValue) {
			for (SimulationFunctionType iter : SimulationFunctionType.values()) {
				if (iter.getAsDouble() == scilabValue) {
					return iter;
				}
			}
			return UNKNOWN;
		}

		/**
		 * Get the Scilab Descriptor from the Java Descriptor
		 * 
		 * @return The corresponding Scilab/Scicos descriptor
		 */
		public double getAsDouble() {
			return this.value;
		}
	};

	/**
	 * 
	 */
	public BasicBlock() {
		super();
		setDefaultValues();
		setVisible(true);
		setVertex(true);
	}

	/**
	 * @param label block label
	 */
	protected BasicBlock(String label) {
		this();
		setDefaultValues();
		setValue(label);
	}

	/**
	 * @param label block label
	 * @param style initial style
	 */
	protected BasicBlock(String label, String style) {
		this(label);
		setStyle(style);
	}

	/**
	 * Initialize the block with the default values
	 */
	protected void setDefaultValues() {
		setVisible(true);
		setVertex(true);
		setConnectable(false);
		setGeometry(new mxGeometry(0, 0, 40, 40));
		setValue("");
		setStyle("");
	}

    /**
     * @return parent diagram
     */
    public XcosDiagram getParentDiagram() {
        return parentDiagram;
    }

    /**
     * @param parentDiagram parent diagram
     */
    public void setParentDiagram(XcosDiagram parentDiagram) {
        this.parentDiagram = parentDiagram;
    }

    
    /**
     * @return interface function name
     */
    public String getInterfaceFunctionName() {
	return interfaceFunctionName;
    }

    /**
     * @param interfaceFunctionName interface function name
     */
    public void setInterfaceFunctionName(String interfaceFunctionName) {
	this.interfaceFunctionName = interfaceFunctionName;
    }

    /**
     * @param simulationFunctionName sumulation function name
     */
    public void setSimulationFunctionName(String simulationFunctionName) {
	this.simulationFunctionName = simulationFunctionName;
    }

    /**
     * @return sumulation function name
     */
    public String getSimulationFunctionName() {
	return simulationFunctionName;
    }

    /**
     * @param scilabValue simulation function type
     */
    public void setSimulationFunctionType(int scilabValue) {
	this.simulationFunctionType = SimulationFunctionType.convertScilabValue(scilabValue);
    }

    /**
     * @param simulationFunctionType simulation function type
     */
    public void setSimulationFunctionType(SimulationFunctionType simulationFunctionType) {
	this.simulationFunctionType = simulationFunctionType;
    }

    /**
     * @return simulation function type
     */
    public SimulationFunctionType getSimulationFunctionType() {
	return simulationFunctionType;
    }

    /**
     * @return real parameter ( rpar )
     */
    public ScilabType getRealParameters() {
	return realParameters;
    }

    /**
     * @param realParameters reaL parameter ( rpar )
     */
    public void setRealParameters(ScilabType realParameters) {
	this.realParameters = realParameters;
    } 

    /**
     * @return integer parameter ( ipar )
     */
    public ScilabType getIntegerParameters() {
	return integerParameters;
    }

    /**
     * @param integerParameters integer parameter ( ipar )
     */
    public void setIntegerParameters(ScilabType integerParameters) {
	this.integerParameters = integerParameters;
    }

    /**
     * @return object parameter ( opar )
     */
    public ScilabType getObjectsParameters() {
	return objectsParameters;
    }

    /**
     * @param objectsParameters object parameter ( opar )
     */
    public void setObjectsParameters(ScilabType objectsParameters) {
	this.objectsParameters = objectsParameters;
    }

    /**
     * @param dependsOnU ?
     */
    public void setDependsOnU(boolean dependsOnU) {
	this.dependsOnU = dependsOnU;
    }

    /**
     * @return ?
     */
    public boolean isDependsOnU() {
	return dependsOnU;
    }

    /**
     * @param dependsOnT ?
     */
    public void setDependsOnT(boolean dependsOnT) {
	this.dependsOnT = dependsOnT;
    }

    /**
     * @return ?
     */
    public boolean isDependsOnT() {
	return dependsOnT;
    }

    /**
     * @param blockType block type
     */
    public void setBlockType(String blockType) {
	this.blockType = blockType;
    }

    /**
     * @return block type
     */
    public String getBlockType() {
	return blockType;
    }

    /**
     * @param ordering order value
     */
    public void setOrdering(int ordering) {
	this.ordering = ordering;
    }

    /**
     * @return order value
     */
    public int getOrdering() {
	return ordering;
    }

    /**
     * @param exprs expression
     */
    public void setExprs(ScilabType exprs) {
	this.exprs = exprs;
    }

    /**
     * @return expression
     */
    public ScilabType getExprs() {
	return exprs;
    }

    /**
     * @return zero crossing value
     */
    public ScilabType getNbZerosCrossing() {
	return nbZerosCrossing;
    }

    /**
     * @param nbZerosCrossing zero crossing value
     */
    public void setNbZerosCrossing(ScilabType nbZerosCrossing) {
	this.nbZerosCrossing = nbZerosCrossing;
    }

    /**
     * @return nmode
     */
    public ScilabType getNmode() {
	return nmode;
    }

    /**
     * @param nmode nmode
     */
    public void setNmode(ScilabType nmode) {
	this.nmode = nmode;
    }

    /**
     * @return current state
     */
    public ScilabType getState() {
	return state;
    }

    /**
     * @param state new state
     */
    public void setState(ScilabType state) {
	this.state = state;
    }

    /**
     * @return current dstate
     */
    public ScilabType getDState() {
	return dState;
    }

    /**
     * @param state new dstate
     */
    public void setDState(ScilabType state) {
	dState = state;
    }

    /**
     * @return current ostate
     */
    public ScilabType getODState() {
	return oDState;
    }

    /**
     * @param state new ostate
     */
    public void setODState(ScilabType state) {
	oDState = state;
    }

    /**
     * @return equations
     */
    public ScilabType getEquations() {
	return equations;
    }

    /**
     * @param equations equations
     */
    public void setEquations(ScilabType equations) {
	this.equations = equations;
    }

    /**
     * @return locked status
     */
    public synchronized boolean isLocked() {
        return locked;
    }

    /**
     * @param locked change locked status
     */
    public synchronized void setLocked(boolean locked) {
        this.locked = locked;
    }

    /**
     * @param port to remove
     */
    public void removePort(BasicPort port) {
	if (port.getEdgeCount() != 0) {
	    getParentDiagram().removeCells(new Object[]{port.getEdgeAt(0)});
	}
	remove(port);
    }
    
    /**
     * @param port input port to add
     */
    public void addPort(InputPort port) {
    	insert(port);
    	BlockPositioning.updateBlockView(this);
    	port.setOrdering(BasicBlockInfo.getAllInputPorts(this, false).size());
    }

    /**
     * @param port output port to add
     */
    public void addPort(OutputPort port) {
    	insert(port);
    	BlockPositioning.updateBlockView(this);
    	port.setOrdering(BasicBlockInfo.getAllOutputPorts(this, false).size());
    }

    /**
     * @param port command port to add
     */
    public void addPort(CommandPort port) {
    	insert(port);
    	BlockPositioning.updateBlockView(this);
    	port.setOrdering(BasicBlockInfo.getAllCommandPorts(this, false).size());
    }

    /**
     * @param port control port to add
     */
    public void addPort(ControlPort port) {
    	insert(port);
    	BlockPositioning.updateBlockView(this);
    	port.setOrdering(BasicBlockInfo.getAllControlPorts(this, false).size());
    }

    /**
     * @return command ports initial state
     */
    public ScilabDouble getAllCommandPortsInitialStates() {
	if (BasicBlockInfo.getAllCommandPorts(this, false).isEmpty()) {
	    return new ScilabDouble();
	}

	double[][] data = new double[BasicBlockInfo.getAllCommandPorts(this, false).size()][1];
	for (int i = 0; i < BasicBlockInfo.getAllCommandPorts(this, false).size(); ++i) {
	    data[i][0] = BasicBlockInfo.getAllCommandPorts(this, false).get(i).getInitialState();
	}

	return new ScilabDouble(data);
    }

    /**
     * @return name and type of the simulation function
     */
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
	doUpdateBlockSettings(modifiedBlock);
    }

    /**
     * Does the block update without using the undo manager 
     * @param modifiedBlock the new settings
     */
    public void doUpdateBlockSettings(BasicBlock modifiedBlock) {
	setDependsOnT(modifiedBlock.isDependsOnT());
	setDependsOnU(modifiedBlock.isDependsOnU());
	setExprs(modifiedBlock.getExprs());

	setRealParameters(modifiedBlock.getRealParameters());
	setIntegerParameters(modifiedBlock.getIntegerParameters());
	setObjectsParameters(modifiedBlock.getObjectsParameters());

	setState(modifiedBlock.getState());
	setDState(modifiedBlock.getDState());
	setODState(modifiedBlock.getODState());

	setEquations(modifiedBlock.getEquations());


	List< ? extends BasicPort> modifiedPorts = null;
	List< ? extends BasicPort> ports = null;
	
	// Check if new input port have been added
	modifiedPorts = BasicBlockInfo.getAllInputPorts(modifiedBlock, false);
	ports = BasicBlockInfo.getAllInputPorts(this, false);
	if (modifiedPorts.size() > ports.size()) {
	    while (ports.size() < modifiedPorts.size()) {
		addPort((InputPort) modifiedPorts.get(ports.size()));
		ports = BasicBlockInfo.getAllInputPorts(this, false);
	    }
	} else { // Check if input ports have been removed
	    modifiedPorts = BasicBlockInfo.getAllInputPorts(modifiedBlock, false);
	    ports = BasicBlockInfo.getAllInputPorts(this, false);
	    if (modifiedPorts.size() < ports.size()) {
		while (ports.size() > modifiedPorts.size()) {
		    removePort((BasicPort) ports.get(ports.size() - 1));
		    ports = BasicBlockInfo.getAllInputPorts(this, false);
		}
	    }
	}

	// Check if new output port have been added
	modifiedPorts = BasicBlockInfo.getAllOutputPorts(modifiedBlock, false);
	ports = BasicBlockInfo.getAllOutputPorts(this, false);
	if (modifiedPorts.size() > ports.size()) {
	    while (ports.size() < modifiedPorts.size()) {
		addPort((OutputPort) modifiedPorts.get(ports.size()));
		ports = BasicBlockInfo.getAllOutputPorts(this, false);
	    }
	} else { // Check if output ports have been removed
	    modifiedPorts = BasicBlockInfo.getAllOutputPorts(modifiedBlock, false);
	    ports = BasicBlockInfo.getAllOutputPorts(this, false);
	    if (modifiedPorts.size() < ports.size()) {
		while (ports.size() > modifiedPorts.size()) {
		    removePort((BasicPort) ports.get(ports.size() - 1));
		    ports = BasicBlockInfo.getAllOutputPorts(this, false);
		}
	    }
	}


	// Check if new command port have been added
	modifiedPorts = BasicBlockInfo.getAllCommandPorts(modifiedBlock, false);
	ports = BasicBlockInfo.getAllCommandPorts(this, false);
	if (modifiedPorts.size() > ports.size()) {
	    while (ports.size() < modifiedPorts.size()) {
		addPort((CommandPort) modifiedPorts.get(ports.size()));
		ports = BasicBlockInfo.getAllCommandPorts(this, false);
	    }
	} else { // Check if command ports have been removed
	    modifiedPorts = BasicBlockInfo.getAllCommandPorts(modifiedBlock, false);
	    ports = BasicBlockInfo.getAllCommandPorts(this, false);
	    if (modifiedPorts.size() < ports.size()) {
		while (ports.size() > modifiedPorts.size()) {
		    removePort((BasicPort) ports.get(ports.size() - 1));
		    ports = BasicBlockInfo.getAllCommandPorts(this, false);
		}
	    }
	}

	// Check if new control port have been added
	modifiedPorts = BasicBlockInfo.getAllControlPorts(modifiedBlock, false);
	ports = BasicBlockInfo.getAllControlPorts(this, false);
	if (modifiedPorts.size() > ports.size()) {
	    while (ports.size() < modifiedPorts.size()) {
		addPort((ControlPort) modifiedPorts.get(ports.size()));
		ports = BasicBlockInfo.getAllControlPorts(this, false);
	    }
	} else { // Check if control ports have been removed
	    modifiedPorts = BasicBlockInfo.getAllControlPorts(modifiedBlock, false);
	    ports = BasicBlockInfo.getAllControlPorts(this, false);
	    if (modifiedPorts.size() < ports.size()) {
		while (ports.size() > modifiedPorts.size()) {
		    removePort((BasicPort) ports.get(ports.size() - 1));
		    ports = BasicBlockInfo.getAllControlPorts(this, false);
		}
	    }
	}

	/*
	 * If the block is in a superblock then update it.
	 */
	if (getParentDiagram() instanceof SuperBlockDiagram) {
	    SuperBlock parentBlock = ((SuperBlockDiagram) getParentDiagram()).getContainer();
	    parentBlock.getParentDiagram().fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED, 
		    XcosConstants.EVENT_BLOCK_UPDATED, parentBlock));
	}
	
    }

    /**
     * @param context parent diagram context
     */
    public void openBlockSettings(String[] context) {
	
	if (getParentDiagram() instanceof PaletteDiagram) {
	    return;
	}
	
	//prevent to open twice
	if (isLocked()) {
	    return;
	}
	
	final File tempOutput;
	final File tempInput;
	final File tempContext;
	try {
	    tempInput = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION, new File(System.getenv("TMPDIR")));
	    tempInput.deleteOnExit();

	    // Write scs_m
	    tempOutput = exportBlockStruct();
	    // Write context
	    tempContext = exportContext(context);

	    String cmd;
	    
	    cmd = "xcosBlockInterface(\"" + tempOutput.getAbsolutePath() + "\"";
	    cmd += ", \"" + tempInput.getAbsolutePath() + "\"";
	    cmd += ", " + getInterfaceFunctionName();
	    cmd += ", \"set\"";
	    cmd += ", \"" + tempContext.getAbsolutePath() + "\");";
	    
	    final BasicBlock currentBlock = this;
	    try {
			XcosInterpreterManagement.asynchronousScilabExec(cmd, new ActionListener() {
				public void actionPerformed(ActionEvent arg0) {
					// Now read new Block
				    BasicBlock modifiedBlock = BlockReader.readBlockFromFile(tempInput.getAbsolutePath());
				    updateBlockSettings(modifiedBlock);
				    getParentDiagram().fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, 
					    currentBlock));
				    setLocked(false);
				}
			});
		} catch (InterpreterException e) {
			e.printStackTrace();
		}
	    setLocked(true);

	} catch (IOException e) {
	    e.printStackTrace();
	}
    }

    /**
     * @return exported file
     */
    protected File exportBlockStruct() {

	// Write scs_m
	File tempOutput;
	try {
	    tempOutput = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION, new File(System.getenv("TMPDIR")));
	    tempOutput.deleteOnExit();
	    int fileId = H5Write.createFile(tempOutput.getAbsolutePath());
	    H5Write.writeInDataSet(fileId, "scs_m", BasicBlockInfo.getAsScilabObj(this));
	    H5Write.closeFile(fileId);
	    return tempOutput;
	} catch (IOException e) {
	    e.printStackTrace();
	} catch (HDF5Exception e) {
	    e.printStackTrace();
	}
	return null;
    }
    
    /**
     * @param context parent diagram context
     * @return exported file
     */
    protected File exportContext(String[] context) {

	// Write context
	try {
	    File tempContext = File.createTempFile(INTERNAL_FILE_PREFIX, INTERNAL_FILE_EXTENSION);
	    tempContext.deleteOnExit();
	    int contextFileId = H5Write.createFile(tempContext.getAbsolutePath());
	    H5Write.writeInDataSet(contextFileId, "context", new ScilabString(context));
	    H5Write.closeFile(contextFileId);
	    return tempContext;
	} catch (IOException e) {
	    e.printStackTrace();
	} catch (HDF5Exception e) {
	    e.printStackTrace();
	}
	return null;
    }
    
    /**
     * @return tooltip text
     */
    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append(XcosConstants.HTML_BEGIN);
	result.append("Block Name : " + getInterfaceFunctionName() + XcosConstants.HTML_NEWLINE);
	result.append("Simulation : " + getSimulationFunctionName() + XcosConstants.HTML_NEWLINE);

	if (getParentDiagram() instanceof PaletteDiagram) {
	    if (getIntegerParameters() != null) {
		result.append("Integer parameters : " + getIntegerParameters() + XcosConstants.HTML_NEWLINE);
	    }
	    
	    if (getRealParameters() != null && getRealParameters().getHeight() != 0 && getRealParameters().getWidth() != 0) {
		result.append("Real parameters : " + getRealParameters() + XcosConstants.HTML_NEWLINE);
	    }
	    
	    if (getObjectsParameters() != null) {
		result.append("Object parameters : " + getObjectsParameters() + XcosConstants.HTML_NEWLINE);
	    }
	} else {
	    result.append("UID : " + getId() + XcosConstants.HTML_NEWLINE);
	    result.append("Block Style : " + getStyle() + XcosConstants.HTML_NEWLINE);
	    result.append("Flip : " + getFlip() + XcosConstants.HTML_NEWLINE);
	    result.append("Mirror : " + getMirror() + XcosConstants.HTML_NEWLINE);
	    result.append("Input ports : " + BasicBlockInfo.getAllInputPorts(this, false).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Output ports : " + BasicBlockInfo.getAllOutputPorts(this, false).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Control ports : " + BasicBlockInfo.getAllControlPorts(this, false).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Command ports : " + BasicBlockInfo.getAllCommandPorts(this, false).size() + XcosConstants.HTML_NEWLINE);
	}

	result.append("x : " + getGeometry().getX() + XcosConstants.HTML_NEWLINE);
	result.append("y : " + getGeometry().getY() + XcosConstants.HTML_NEWLINE);
	result.append("w : " + getGeometry().getWidth() + XcosConstants.HTML_NEWLINE);
	result.append("h : " + getGeometry().getHeight() + XcosConstants.HTML_NEWLINE);
	result.append(XcosConstants.HTML_END);
	return result.toString();
    }

    /**
     * @param graph parent graph
     */
    public void openContextMenu(ScilabGraph graph) {
	ContextMenu menu = null;
	if (getParentDiagram() instanceof PaletteDiagram) {
	    menu = createPaletteContextMenu(graph);
	} else {
	    menu = createContextMenu(graph);
	}
	menu.setVisible(true);
    }

    /**
     * @param graph parent graph
     * @return context menu
     */
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
		    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
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
		    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
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
			BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
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
		XcosInterpreterManagement.requestScilabExec("help " + getInterfaceFunctionName());
	    }
	});
	menu.add(help);

	menu.setVisible(true);

	((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(
		MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
	
	return menu;
    }

    /**
     * @param graph parent graph
     * @return context menu
     */
    public ContextMenu createContextMenu(ScilabGraph graph) {
		ContextMenu menu = ScilabContextMenu.createContextMenu();
		Map<Class< ? extends DefaultAction>, Menu> menuList = new HashMap<Class< ? extends DefaultAction>, Menu>();
		
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
		alignMenu.add(AlignBlockActionLeft.createMenu(graph));
		alignMenu.add(AlignBlockActionCenter.createMenu(graph));
		alignMenu.add(AlignBlockActionRight.createMenu(graph));
		alignMenu.addSeparator();
		alignMenu.add(AlignBlockActionTop.createMenu(graph));
		alignMenu.add(AlignBlockActionMiddle.createMenu(graph));
		alignMenu.add(AlignBlockActionBottom.createMenu(graph));
		menuList.put(AlignBlockAction.class, alignMenu);
		format.add(alignMenu);
		/*--- */
		format.addSeparator();
		/*--- */
		format.add(BorderColorAction.createMenu(graph));
		format.add(FilledColorAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(ViewDetailsAction.createMenu(graph));
		/*--- */
		menu.getAsSimpleContextMenu().addSeparator();
		/*--- */
		menu.add(BlockDocumentationAction.createMenu(graph));

		((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, 
			MouseInfo.getPointerInfo().getLocation().y);
		
		customizeMenu(menuList);
		
		return menu;
    }
    
    /**
     * @param flip value
     */
    public void setFlip(boolean flip) {
	if (getParentDiagram() != null) {
	    isFlipped = flip;
	    if (flip) {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, "true");
	    } else {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, "false");
	    }
	}
    }

    /**
     * Override this to customize contextual menu
     * @param menuList list of menu
     */
    protected void customizeMenu(Map<Class< ? extends DefaultAction>, Menu> menuList) {
	// To be overridden by sub-classes
    }
    

    /**
     * @return mirror value
     */
    public boolean getMirror(){
	return isMirrored;
    }
    
    /**
     * @param mirror new mirror value
     */
    public void setMirror(boolean mirror) {
	if (getParentDiagram() != null) {
	    isMirrored = mirror;
	    if (mirror) {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_MIRROR, "true");
	    } else {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_MIRROR, "false");
	    }
	}
    }

    /**
     * @return flip status
     */
    public boolean getFlip() {
	return isFlipped;
    }

    /**
     * invert flip status
     */
    public void toggleFlip() {
	BlockPositioning.toggleFlip(this);
    }

    /**
     * invert mirror value
     */
    public void toggleMirror() {
	BlockPositioning.toggleMirror(this);
    }

    /**
     * 
     */
    public void toggleAntiClockwiseRotation() {
	BlockPositioning.toggleAntiClockwiseRotation(this);

    }

    /**
     * @return current angle
     */
    public int getAngle() {
	return angle;
    }

    /**
     * @param angle new block angle
     */
    public void setAngle(int angle) {
	this.angle = angle;
	
	if (getParentDiagram() != null) {
	    mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_ROTATION, new Integer(angle).toString());
	}
    }

    /**
     * Useful when we need to update local properties with mxCell style properties 
     */
	public void updateFieldsFromStyle() {
		StyleMap map = new StyleMap(getStyle());

		if (map.get(XcosConstants.STYLE_ROTATION) != null) {
			angle = Integer.parseInt(map.get(XcosConstants.STYLE_ROTATION));
		} else {
			angle = 0;
		}
		
		isFlipped = Boolean.parseBoolean(map.get(XcosConstants.STYLE_FLIP));
		isMirrored = Boolean.parseBoolean(map.get(XcosConstants.STYLE_MIRROR));
	}
}
