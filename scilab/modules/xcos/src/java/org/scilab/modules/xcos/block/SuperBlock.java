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

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCustomizeAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.io.scicos.DiagramElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.util.mxEventObject;

/**
 * A SuperBlock contains an entire diagram on it. Thus it can be easily
 * customized by the user.
 * 
 * A SuperBlock can be created from any part of the diagram y selecting blocks
 * and applying the {@link RegionToSuperblockAction}.
 * 
 * It can also appear to users as a normal block by applying a mask on it. In
 * this case the creator can use any SuperBlock context defined variable on a
 * prompt to the user.
 * 
 * @see SuperBlockDiagram
 * @see SuperblockMaskCreateAction
 * @see SuperblockMaskCustomizeAction
 * @see SuperblockMaskRemoveAction
 */
public final class SuperBlock extends BasicBlock {
	private static final long serialVersionUID = 3005281208417373333L;
	/**
	 * The simulation name (linked to Xcos-core)
	 */
	private static final String SIMULATION_NAME = "super";
	/**
	 * The interfunction name (linked to Xcos-core)
	 */
	private static final String INTERFUNCTION_NAME = "SUPER_f";
	/**
	 * The simulation name on a masked status (linked to Xcos-core)
	 */
	private static final String MASKED_SIMULATION_NAME = "csuper";
	/**
	 * The interfunction name on a masked status (linked to Xcos-core)
	 */
	private static final String MASKED_INTERFUNCTION_NAME = "DSUPER";
	
	private SuperBlockDiagram child;

	/**
	 * Constructor
	 */
	public SuperBlock() {
		super();
	}

	/**
	 * @param label block label
	 */
	protected SuperBlock(String label) {
		this();
		setValue(label);
	}

	/**
	 * @param masked masked super block
	 */
	protected SuperBlock(boolean masked) {
		this();
		if (masked) {
			mask();
		}
	}
	
	/**
	 * @param label block label
	 * @param masked masked super block
	 */
	protected SuperBlock(String label, boolean masked) {
		this(label);
		if (masked) {
			mask();
		}
	}
	
	/**
	 * Initialize the block with the default values
	 */
	@Override
	protected void setDefaultValues() {
		super.setDefaultValues();
		setInterfaceFunctionName(INTERFUNCTION_NAME);
		setSimulationFunctionName(SIMULATION_NAME);
		setRealParameters(new ScilabMList());
		setIntegerParameters(new ScilabDouble());
		setObjectsParameters(new ScilabList());
		setExprs(new ScilabDouble());
		setBlockType("h");
		setNbZerosCrossing(new ScilabDouble(0));
		setNmode(new ScilabDouble(0));
	}

	/**
	 * openBlockSettings this method is called when a double click occurred on a
	 * super block 
	 * @param context parent diagram context
	 * @see BasicBlock.openBlockSettings
	 */
	@Override
	public void openBlockSettings(String[] context) {
		
		/*
		 * Do nothing when something happen on the Palette
		 */
		if (getParentDiagram() instanceof PaletteDiagram) {
			return;
		}
		
		/*
		 * Specific case when we want to generate code.
		 */
		if (getChild() == null
				&& getSimulationFunctionType().compareTo(
						SimulationFunctionType.DEFAULT) != 0) {
			return;
		}

		/*
		 * When the block is masked it perform actions like any other blocks.
		 */
		if (isMasked()) {
			super.openBlockSettings(context);
			return;
		}
		
		// Lock the block because we are really performing actions
		setLocked(true);
		
		/*
		 * Compatibility with older diagrams.
		 * 
		 * Before Scilab 5.2.2, saved diagrams don't contains XML children but
		 * use a pseudo scs_m structure instead.
		 * 
		 * In this case child was null and we need to reconstruct child diagram
		 * from scs_m.
		 */
		if (getChild() == null) {
			createChildDiagram();
		}
		
		/*
		 * Construct the view or set it visible.
		 */
		if (!getChild().isOpened()) {
			updateAllBlocksColor();
			getChild().setModifiedNonRecursively(false);
			XcosTab.createTabFromDiagram(getChild());
			XcosTab.showTabFromDiagram(getChild());
			getChild().setOpened(true);
			getChild().setVisible(true);
			
		} else {
			getChild().setVisible(true);
		}
		
		/*
		 * Update the cells from the context values.
		 */
		getChild().updateCellsContext();
		
		XcosTab.getAllDiagrams().add(getChild());
	}

	/**
	 * Action to be performed when the diagram is closed
	 */
	public void closeBlockSettings() {
		/*
		 * Do not ask the user, the diagram is saved and closed.
		 * 
		 * By this way we are sure that the main scs_m structure is always
		 * valid.
		 */
		if (getChild().isModified()) {
			setRealParameters(new DiagramElement().encode(getChild()));
			getChild().setModified(true);
			getChild().setModifiedNonRecursively(false);
		}

		/*
		 * Hide the current child window
		 */
		getChild().setVisible(false);
		setLocked(false);
		XcosTab.getAllDiagrams().remove(getChild());
	}

	/**
	 * @param graph parent diagram
	 */
	public void openContextMenu(ScilabGraph graph) {
		ContextMenu menu = null;

		if (getParentDiagram() instanceof PaletteDiagram) {
			menu = createPaletteContextMenu(graph);
		} else {
			menu = createContextMenu(graph);
			menu.getAsSimpleContextMenu().addSeparator();
			menu.add(CodeGenerationAction.createMenu(graph));

			Menu maskMenu = ScilabMenu.createMenu();
			maskMenu.setText(XcosMessages.SUPERBLOCK_MASK);

			if (isMasked()) {
				maskMenu.add(SuperblockMaskRemoveAction.createMenu(graph));
				menu.add(maskMenu);
			} else {
				maskMenu.add(SuperblockMaskCreateAction.createMenu(graph));
			}
			maskMenu.add(SuperblockMaskCustomizeAction.createMenu(graph));
			menu.add(maskMenu);
			 
		}
		menu.setVisible(true);
	}

	/**
	 * @return status
	 */
	public boolean createChildDiagram() {
		return createChildDiagram(false);
	}

	
	/**
	 * @param generatedUID does we need to generated a new unique ID
	 * @return status
	 */
	public boolean createChildDiagram(boolean generatedUID) {
		if (child == null) {
			child = new SuperBlockDiagram(this);
			child.installListeners();
			try {
				new DiagramElement().decode(getRealParameters(), child, false);
			} catch (ScicosFormatException e) {
				LogFactory.getLog(SuperBlock.class).error(e);
				return false;
			}
			
			child.installSuperBlockListeners();
			child.setChildrenParentDiagram();
			updateAllBlocksColor();
			// only for loading and generate sub block UID
			if (generatedUID) {
				child.generateUID();
			}
		} else {
			return false;
		}

		return true;
	}

	/**
	 * @return diagram
	 */
	public SuperBlockDiagram getChild() {
		return child;
	}

	/**
	 * @param child update diagram
	 */
	public void setChild(SuperBlockDiagram child) {
		this.child = child;
	}

	/**
	 * @param <T> The type to work on
	 * @param klass the class instance to work on
	 * @return list of typed block
	 */
	protected < T extends BasicBlock> List<T> getAllTypedBlock(Class<T> klass) {
		List<T> list = new ArrayList<T>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			Object cell = child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (klass.isInstance(cell)) {
				// According to the test we are sure that the cell is an
				// instance of T. Thus we can safely cast it.
				list.add((T) cell);
			}
		}
		return list;
	}
	
	/**
	 * @return list of input explicit block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllExplicitInBlock() {
		return getAllTypedBlock(ExplicitInBlock.class);
	}

	/**
	 * @return list of input implicit block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllImplicitInBlock() {
		return getAllTypedBlock(ImplicitInBlock.class);
	}

	/**
	 * @return list of input event block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllEventInBlock() {
		return getAllTypedBlock(EventInBlock.class);
	}

	/**
	 * @return list of ouput explicit block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllExplicitOutBlock() {
		return getAllTypedBlock(ExplicitOutBlock.class);
	}

	/**
	 * @return list of output implicit block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllImplicitOutBlock() {
		return getAllTypedBlock(ImplicitOutBlock.class);
	}

	/**
	 * @return list of output event block
	 */
	@Deprecated
	protected List< ? extends BasicBlock> getAllEventOutBlock() {
		return getAllTypedBlock(EventOutBlock.class);
	}

	/**
	 * @param blocks in/output blocks
	 * @return greater block value
	 */
	protected int getBlocksConsecutiveUniqueValueCount(List< ? extends BasicBlock> blocks) {
		if (blocks == null) {
			return 0;
		}

		int count = blocks.size();
		int[] array = new int[blocks.size()];

		// initialize
		for (int i = 0; i < array.length; i++) {
			array[i] = 0;
		}

		// populate
		for (int i = 0; i < array.length; i++) {
			int index = (Integer) ((BasicBlock) blocks.get(i)).getValue();
			if (index <= array.length) {
				array[index - 1] = 1;
			}
		}

		// parse
		for (int i = 0; i < array.length; i++) {
			if (array[i] == 0) {
				count = i;
				break;
			}
		}

		return count;
	}

	/**
	 * force blocks update
	 */
	public void updateAllBlocksColor() {
		for (IOBlocks block : IOBlocks.values()) {
			updateBlocksColor(getAllTypedBlock(block.getReferencedClass()));
		}
	}

	/**
	 * @param blocks block list
	 */
	private void updateBlocksColor(List< ? extends BasicBlock> blocks) {

		try {
			child.getModel().beginUpdate();
			if (blocks == null || blocks.size() == 0) {
				return;
			}

			int countUnique = getBlocksConsecutiveUniqueValueCount(blocks);
			boolean[] isDone = new boolean[countUnique];

			// Initialize
			for (int i = 0; i < countUnique; i++) {
				isDone[i] = false;
			}

			for (int i = 0; i < blocks.size(); i++) {
				int index = (Integer) ((BasicBlock) blocks.get(i)).getValue();
				if (index > countUnique || isDone[index - 1]) {
					child.getAsComponent().setCellWarning(blocks.get(i),
							"Wrong port number");
				} else {
					isDone[index - 1] = true;
					child.getAsComponent().setCellWarning(blocks.get(i), null);
				}
			}
		} finally {
			child.getModel().endUpdate();
		}
	}

	/**
	 * update super block ports in parent diagram
	 */
	public void updateExportedPort() {
		if (child == null) {
			return;
		}

		for (IOBlocks block : IOBlocks.values()) {
			updateExportedTypedPort(block);
		}
		getParentDiagram().fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED, XcosConstants.EVENT_BLOCK_UPDATED, this));
	}

	/**
	 * Update the superblock IO ports according to the values of its child.
	 * @param block The blocks to work on
	 */
	private void updateExportedTypedPort(IOBlocks block) {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllTypedBlock(block.getReferencedClass()));
		List< ? extends BasicPort> ports = BasicBlockInfo
				.getAllTypedPorts(this, false, block.getReferencedPortClass());

		int portCount = ports.size();

		try {
			while (blockCount > portCount) { // add if required
				BasicPort port;
					port = block.getReferencedPortClass().newInstance();
					port.setDefaultValues();
					addPort(port);
					portCount++;
			}
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * Mask the SuperBlock
	 */
	public void mask() {
		setInterfaceFunctionName(MASKED_INTERFUNCTION_NAME);
		setSimulationFunctionName(MASKED_SIMULATION_NAME);
	}

	/**
	 * Unmask the SuperBlock
	 */
	public void unmask() {
		setInterfaceFunctionName(INTERFUNCTION_NAME);
		setSimulationFunctionName(SIMULATION_NAME);
	}

	/**
	 * @return True is the SuperBlock is masked, false otherwise
	 */
	public boolean isMasked() {
		return getInterfaceFunctionName().compareTo(INTERFUNCTION_NAME) != 0;
	}
}
