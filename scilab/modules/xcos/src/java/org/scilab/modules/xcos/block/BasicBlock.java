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

import static org.scilab.modules.xcos.utils.FileUtils.delete;

import java.awt.MouseInfo;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.EditBlockFormatAction;
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
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

public class BasicBlock extends ScilabGraphUniqueObject {
	private static final double DEFAULT_POSITION_X = 10.0;
	private static final double DEFAULT_POSITION_Y = 10.0;
	private static final double DEFAULT_WIDTH = 40.0;
	private static final double DEFAULT_HEIGHT = 40.0;
	
	private static final PropertyChangeListener styleUpdater = new UpdateStyleFromInterfunction();
	private static final Log LOG = LogFactory.getLog(BasicBlock.class);
	
	/**
	 * Manage events for block parameters.
	 * 
	 * The property name is the field name, is one of:
	 *     - "interfaceFunctionName"
	 *     - "simulationFunctionName"
	 *     - "simulationFunctionType"
	 *     - "exprs"
	 *     - "realParameters"
	 *     - "integerParameters"
	 *     - "objectsParameters"
	 *     - "nbZerosCrossing"
	 *     - "nmode"
	 *     - "state"
	 *     - "dState"
	 *     - "oDState"
	 *     - "equations"
	 *     - "dependsOnU"
	 *     - "dependsOnT"
	 *     - "blockType"
	 *     - "ordering"
	 */
	protected PropertyChangeSupport parameters = new PropertyChangeSupport(this);
	
    private String interfaceFunctionName = "xcos_block";
    private String simulationFunctionName = "xcos_simulate";
    private SimulationFunctionType simulationFunctionType = SimulationFunctionType.DEFAULT;
    private transient XcosDiagram parentDiagram;   
    
    private int angle;
    private boolean isFlipped;
    private boolean isMirrored;
    

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
	public static enum SimulationFunctionType {
		ESELECT(-2.0), IFTHENELSE(-1.0), DEFAULT(0.0), TYPE_1(1.0), TYPE_2(2.0),
		    TYPE_3(3.0), C_OR_FORTRAN(4.0), SCILAB(5.0), MODELICA(30004.0), UNKNOWN(5.0), OLDBLOCKS(10001.0);

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

	private static class UpdateStyleFromInterfunction implements PropertyChangeListener {

		/**
		 * Update the label on interfunction change.
		 * 
		 * @param evt the property change event.
		 * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
		 */
		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			BasicBlock source = (BasicBlock) evt.getSource();
			
			StyleMap style = new StyleMap(source.getStyle());
			style.remove((String) evt.getOldValue());
			style.put((String) evt.getNewValue(), null);
			source.setStyle(style.toString());
		}
		
	}
	
	/**
	 * 
	 */
	public BasicBlock() {
		super();
		setDefaultValues();
		setVisible(true);
		setVertex(true);
		
		parameters.addPropertyChangeListener("interfaceFunctionName",
				styleUpdater);
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
		setGeometry(new mxGeometry(DEFAULT_POSITION_X, DEFAULT_POSITION_Y,
				DEFAULT_WIDTH, DEFAULT_HEIGHT));
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
		if ((this.interfaceFunctionName == null && interfaceFunctionName != null)
				|| !this.interfaceFunctionName.equals(interfaceFunctionName)) {
			
			final String oldValue = this.interfaceFunctionName;
			this.interfaceFunctionName = interfaceFunctionName;
			parameters.firePropertyChange("interfaceFunctionName", oldValue,
					interfaceFunctionName);
		}
    }

    /**
     * @param simulationFunctionName sumulation function name
     */
    public void setSimulationFunctionName(String simulationFunctionName) {
		if ((this.simulationFunctionName == null && simulationFunctionName != null)
				|| !this.simulationFunctionName.equals(simulationFunctionName)) {
			
    		final String oldValue = this.simulationFunctionName;
    		this.simulationFunctionName = simulationFunctionName;
    		parameters.firePropertyChange("simulationFunctionName", oldValue, simulationFunctionName);
    	}
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
		SimulationFunctionType simulationFunctionType = SimulationFunctionType.convertScilabValue(scilabValue);
		setSimulationFunctionType(simulationFunctionType);
    }

    /**
     * @param simulationFunctionType simulation function type
     */
	public void setSimulationFunctionType(SimulationFunctionType simulationFunctionType) {
		if ((this.simulationFunctionType == null && simulationFunctionType != null)
				|| !this.simulationFunctionType.equals(simulationFunctionType)) {
			
			final SimulationFunctionType oldValue = this.simulationFunctionType;
			this.simulationFunctionType = simulationFunctionType;
			parameters.firePropertyChange("simulationFunctionType", oldValue,
					simulationFunctionType);
		}
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
		if ((this.realParameters == null && realParameters != null)
				|| !this.realParameters.equals(realParameters)) {
			
			final ScilabType oldValue = this.realParameters;
			this.realParameters = realParameters;
			parameters.firePropertyChange("realParameters", oldValue, realParameters);
		}
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
		if ((this.integerParameters == null && integerParameters != null)
				|| !this.integerParameters.equals(integerParameters)) {
			
			final ScilabType oldValue = this.integerParameters;
			this.integerParameters = integerParameters;
			parameters.firePropertyChange("integerParameters", oldValue, integerParameters);
		}
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
		if ((this.objectsParameters == null && objectsParameters != null)
				|| !this.objectsParameters.equals(objectsParameters)) {
			
			final ScilabType oldValue = this.objectsParameters;
			this.objectsParameters = objectsParameters;
			parameters.firePropertyChange("objectsParameters", oldValue, objectsParameters);
		}
    }

    /**
     * @param dependsOnU ?
     */
    public void setDependsOnU(boolean dependsOnU) {
		if (this.dependsOnU != dependsOnU) {
			
			final boolean oldValue = this.dependsOnU;
			this.dependsOnU = dependsOnU;
			parameters.firePropertyChange("dependsOnU", oldValue, dependsOnU);
		}
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
		if (this.dependsOnT != dependsOnT) {
			
			final boolean oldValue = this.dependsOnT;
			this.dependsOnT = dependsOnT;
			parameters.firePropertyChange("dependsOnT", oldValue, dependsOnT);
		}
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
		if ((this.blockType == null && blockType != null)
				|| !this.blockType.equals(blockType)) {
			
			final String oldValue = this.blockType;
			this.blockType = blockType;
			parameters.firePropertyChange("blockType", oldValue, blockType);
		}
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
		if (this.ordering != ordering) {
			
			final int oldValue = this.ordering;
			this.ordering = ordering;
			parameters.firePropertyChange("ordering", oldValue, ordering);
		}
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
		if ((this.exprs == null && exprs != null)
				|| !this.exprs.equals(exprs)) {
			
			final ScilabType oldValue = this.exprs;
			this.exprs = exprs;
			parameters.firePropertyChange("exprs", oldValue, exprs);
		}
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
		if ((this.nbZerosCrossing == null && nbZerosCrossing != null)
				|| !this.nbZerosCrossing.equals(nbZerosCrossing)) {
			
			final ScilabType oldValue = this.nbZerosCrossing;
			this.nbZerosCrossing = nbZerosCrossing;
			parameters.firePropertyChange("nbZerosCrossing", oldValue, nbZerosCrossing);
		}
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
		if ((this.nmode == null && nmode != null)
				|| !this.nmode.equals(nmode)) {
			
			final ScilabType oldValue = this.nmode;
			this.nmode = nmode;
			parameters.firePropertyChange("nmode", oldValue, nmode);
		}
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
		if ((this.state == null && state != null)
				|| !this.state.equals(state)) {
			
			final ScilabType oldValue = this.state;
			this.state = state;
			parameters.firePropertyChange("state", oldValue, state);
		}
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
    public void setDState(ScilabType dState) {
		if ((this.dState == null && dState != null)
				|| !this.dState.equals(dState)) {
			
			final ScilabType oldValue = this.dState;
			this.dState = dState;
			parameters.firePropertyChange("dState", oldValue, dState);
		}
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
    public void setODState(ScilabType oDState) {
		if ((this.oDState == null && oDState != null)
				|| !this.oDState.equals(oDState)) {
			
			final ScilabType oldValue = this.oDState;
			this.oDState = oDState;
			parameters.firePropertyChange("oDState", oldValue, oDState);
		}
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
		if ((this.equations == null && equations != null)
				|| !this.equations.equals(equations)) {
			
			final ScilabType oldValue = this.equations;
			this.equations = equations;
			parameters.firePropertyChange("equations", oldValue, equations);
		}
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
     * Add a port on the block.
     * @param port The port to be added to the block
     */
    public void addPort(BasicPort port) {
    	insert(port);
    	BlockPositioning.updateBlockView(this);
    	port.setOrdering(BasicBlockInfo.getAllTypedPorts(this, false, port.getClass()).size());
    }

	/**
	 * @return command ports initial state
	 */
	public ScilabDouble getAllCommandPortsInitialStates() {
		final List<CommandPort> cmdPorts = BasicBlockInfo.getAllTypedPorts(
				this, false, CommandPort.class);
		if (cmdPorts.isEmpty()) {
			return new ScilabDouble();
		}

		double[][] data = new double[cmdPorts.size()][1];
		for (int i = 0; i < cmdPorts.size(); ++i) {
			data[i][0] = cmdPorts.get(i).getInitialState();
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

		// Update the children according to the modified block children. We are
		// working on the last index in order to simplify the List.remove()
		// call.
		final int oldChildCount = getChildCount();
		final int newChildCount = modifiedBlock.getChildCount();
		final int portStep = newChildCount - oldChildCount;
		if (portStep > 0) {
			for (int i = portStep - 1; i >= 0; i--) {
				addPort((BasicPort) modifiedBlock.getChildAt(oldChildCount + i - 1));
			}
		} else {
			for (int i = -portStep - 1; i >= 0; i--) {
				removePort((BasicPort) getChildAt(newChildCount + i));
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
	final BasicBlock currentBlock = this;
	
	try {
	    tempInput = FileUtils.createTempFile();

	    // Write scs_m
	    tempOutput = exportBlockStruct();
	    // Write context
	    tempContext = exportContext(context);
	    
	    final ActionListener action = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				if (tempInput.exists()) {
					LOG.trace("Updating data.");
					
				// Now read new Block
			    BasicBlock modifiedBlock = new H5RWHandler(tempInput).readBlock();
			    updateBlockSettings(modifiedBlock);
			    
			    getParentDiagram().fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, 
				    currentBlock));
				} else {
					LOG.trace("No needs to update data.");
				}
				
			    setLocked(false);
			    delete(tempInput);
			    delete(tempOutput);
			    delete(tempContext);
			}
		};
		
	    try {
			ScilabInterpreterManagement.asynchronousScilabExec(action, 
				"xcosBlockInterface", 
				tempOutput.getAbsolutePath(),
				tempInput.getAbsolutePath(),
				getInterfaceFunctionName().toCharArray(),
				"set",
				tempContext.getAbsolutePath());
		} catch (InterpreterException e) {
			LOG.error(e);
		}
	    setLocked(true);

	} catch (IOException e) {
	    LOG.error(e);
	}
    }

    /**
     * @return exported file
     */
    protected File exportBlockStruct() {

	// Write scs_m
	File tempOutput;
	try {
	    tempOutput = FileUtils.createTempFile();
	    tempOutput.deleteOnExit();
	    
	    new H5RWHandler(tempOutput).writeBlock(this);
	    return tempOutput;
	} catch (IOException e) {
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
	    File tempContext = FileUtils.createTempFile();
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
	StringBuilder result = new StringBuilder();
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
		final int length = getStyle().length();
		result.append("Style : ");
		if (length > XcosConstants.MAX_CHAR_IN_STYLE) {
			result.append(getStyle().substring(0, XcosConstants.MAX_CHAR_IN_STYLE));
			result.append(XcosMessages.DOTS);
		} else {
			result.append(getStyle());
		}
		result.append(XcosConstants.HTML_NEWLINE);
	    result.append("Flip : " + getFlip() + XcosConstants.HTML_NEWLINE);
	    result.append("Mirror : " + getMirror() + XcosConstants.HTML_NEWLINE);
	    result.append("Input ports : " + BasicBlockInfo.getAllTypedPorts(this, false, InputPort.class).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Output ports : " + BasicBlockInfo.getAllTypedPorts(this, false, OutputPort.class).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Control ports : " + BasicBlockInfo.getAllTypedPorts(this, false, ControlPort.class).size() + XcosConstants.HTML_NEWLINE);
	    result.append("Command ports : " + BasicBlockInfo.getAllTypedPorts(this, false, CommandPort.class).size() + XcosConstants.HTML_NEWLINE);
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
		    setDefaultPosition(geom);
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
		    setDefaultPosition(geom);
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
		    setDefaultPosition(geom);
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
		ScilabInterpreterManagement.requestScilabExec("help " + getInterfaceFunctionName());
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
		if (graph.getSelectionCells().length > 1) {
			format.add(BorderColorAction.createMenu(graph));
			format.add(FilledColorAction.createMenu(graph));
		} else {
			format.add(EditBlockFormatAction.createMenu(graph));
		}
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
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, Boolean.TRUE.toString());
	    } else {
		mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_FLIP, Boolean.FALSE.toString());
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
    public boolean getMirror() {
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
	    mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] {this}, XcosConstants.STYLE_ROTATION, Integer.toString(angle));
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

	/**
	 * Set the default block position on the geom
	 * @param geom the current geom
	 */
	private void setDefaultPosition(mxGeometry geom) {
		geom.setX(DEFAULT_POSITION_X);
		geom.setY(DEFAULT_POSITION_Y);
	}
	
	/**
	 * Get the parameters change support.
	 * 
	 * The property name for each event is the field name, so one of:
	 *     - "interfaceFunctionName"
	 *     - "simulationFunctionName"
	 *     - "simulationFunctionType"
	 *     - "exprs"
	 *     - "realParameters"
	 *     - "integerParameters"
	 *     - "objectsParameters"
	 *     - "nbZerosCrossing"
	 *     - "nmode"
	 *     - "state"
	 *     - "dState"
	 *     - "oDState"
	 *     - "equations"
	 *     - "dependsOnU"
	 *     - "dependsOnT"
	 *     - "blockType"
	 *     - "ordering"
	 * 
	 * @return the associated {@link PropertyChangeSupport} instance
	 */
	public PropertyChangeSupport getParametersPCS() {
		return parameters;
	}
}
