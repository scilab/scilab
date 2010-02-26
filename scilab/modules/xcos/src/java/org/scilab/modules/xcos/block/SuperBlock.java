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

import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
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
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.io.BasicBlockInfo;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
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
		setInterfaceFunctionName("SUPER_f");
		setSimulationFunctionName("super");
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
			setRealParameters(BlockWriter.convertDiagramToMList(getChild()));
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
			child.loadDiagram(BlockReader.convertMListToDiagram(
					(ScilabMList) getRealParameters(), false));
			
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
	 * @return block as mlist structure
	 */
	public ScilabMList getAsScilabObj() {
		if (child != null) {
			setRealParameters(BlockWriter.convertDiagramToMList(child));
		}
		return BasicBlockInfo.getAsScilabObj(this);
	}

	/**
	 * @return list of input explicit block
	 */
	protected List<mxCell> getAllExplicitInBlock() {
		List<mxCell> list = new ArrayList<mxCell>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof ExplicitInBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @return list of input implicit block
	 */
	protected List<mxCell> getAllImplicitInBlock() {
		List<mxCell> list = new ArrayList<mxCell>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof ImplicitInBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @return list of input event block
	 */
	protected List<mxCell> getAllEventInBlock() {
		List<mxCell> list = new ArrayList<mxCell>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof EventInBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @return list of ouput explicit block
	 */
	protected List<mxCell> getAllExplicitOutBlock() {
		List<mxCell> list = new ArrayList<mxCell>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof ExplicitOutBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @return list of output implicit block
	 */
	protected List<mxCell> getAllImplicitOutBlock() {
		List<mxCell> list = new ArrayList<mxCell>();
		if (child == null) {
			return list;
		}

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof ImplicitOutBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @return list of output event block
	 */
	protected List<mxCell> getAllEventOutBlock() {
		List<mxCell> list = new ArrayList<mxCell>();

		int blockCount = child.getModel().getChildCount(
				child.getDefaultParent());

		for (int i = 0; i < blockCount; i++) {
			mxCell cell = (mxCell) child.getModel().getChildAt(
					child.getDefaultParent(), i);
			if (cell instanceof EventOutBlock) {
				list.add(cell);
			}
		}
		return list;
	}

	/**
	 * @param blocks in/output blocks
	 * @return greater block value
	 */
	protected int getBlocksConsecutiveUniqueValueCount(List<mxCell> blocks) {
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
		updateBlocksColor(getAllExplicitInBlock());
		updateBlocksColor(getAllImplicitInBlock());
		updateBlocksColor(getAllEventInBlock());

		updateBlocksColor(getAllExplicitOutBlock());
		updateBlocksColor(getAllImplicitOutBlock());
		updateBlocksColor(getAllEventOutBlock());
	}

	/**
	 * @param blocks block list
	 */
	private void updateBlocksColor(List<mxCell> blocks) {

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

		updateExportedExplicitInputPort();
		updateExportedImplicitInputPort();
		updateExportedEventInputPort();
		updateExportedExplicitOutputPort();
		updateExportedImplicitOutputPort();
		updateExportedEventOutputPort();
		getParentDiagram().fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED, XcosConstants.EVENT_BLOCK_UPDATED, this));
	}

	/**
	 * update explicit input super block ports in parent diagram
	 */
	private void updateExportedExplicitInputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllExplicitInBlock());
		List<ExplicitInputPort> ports = BasicBlockInfo
				.getAllExplicitInputPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			ExplicitInputPort port = new ExplicitInputPort();
			port.setDefaultValues();
			addPort(port);
			portCount++;
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * update implicit input super block ports in parent diagram
	 */
	private void updateExportedImplicitInputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitInBlock());
		List<ImplicitInputPort> ports = BasicBlockInfo
				.getAllImplicitInputPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			addPort(new ImplicitInputPort());
			portCount++;
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * update event input super block ports in parent diagram
	 */
	private void updateExportedEventInputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllEventInBlock());
		List<ControlPort> ports = BasicBlockInfo
				.getAllControlPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			addPort(new ControlPort());
			portCount++;
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * update explicit output super block ports in parent diagram
	 */
	private void updateExportedExplicitOutputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllExplicitOutBlock());
		List<ExplicitOutputPort> ports = BasicBlockInfo
				.getAllExplicitOutputPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			ExplicitOutputPort port = new ExplicitOutputPort();
			port.setDefaultValues();
			addPort(port);
			portCount++;
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * update implicit output super block ports in parent diagram
	 */
	private void updateExportedImplicitOutputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitOutBlock());
		List<ImplicitOutputPort> ports = BasicBlockInfo
				.getAllImplicitOutputPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			addPort(new ImplicitOutputPort());
			portCount++;
		}

		while (portCount > blockCount) { // remove if required
			removePort(ports.get(portCount - 1));
			portCount--;
		}
	}

	/**
	 * update event output super block ports in parent diagram
	 */
	private void updateExportedEventOutputPort() {
		int blockCount = getBlocksConsecutiveUniqueValueCount(getAllEventOutBlock());
		List<CommandPort> ports = BasicBlockInfo
				.getAllCommandPorts(this, false);

		int portCount = ports.size();

		while (blockCount > portCount) { // add if required
			addPort(new CommandPort());
			portCount++;
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
		setInterfaceFunctionName("DSUPER");
		setSimulationFunctionName("csuper");
	}

	/**
	 * Unmask the SuperBlock
	 */
	public void unmask() {
		setInterfaceFunctionName("SUPER_f");
		setSimulationFunctionName("super");
	}

	/**
	 * @return True is the SuperBlock is masked, false otherwise
	 */
	public boolean isMasked() {
		return getInterfaceFunctionName().compareTo("SUPER_f") != 0;
	}
}
