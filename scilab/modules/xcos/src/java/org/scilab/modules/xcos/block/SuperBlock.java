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
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.hdf5.scilabTypes.ScilabDouble;
import org.scilab.modules.hdf5.scilabTypes.ScilabList;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.CodeGenerationAction;
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
import org.scilab.modules.xcos.utils.XcosEvent;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxEventObject;

public class SuperBlock extends BasicBlock {

    private static final long serialVersionUID = 3005281208417373333L;
    private SuperBlockDiagram child = null;
 
    public SuperBlock(){
	super();
	setVertex(false);
	setVisible(false);
    }

    public SuperBlock(String label) {
	super(label);
	setInterfaceFunctionName("SUPER_f");
	setSimulationFunctionName("super");
	setRealParameters(new ScilabDouble());
	setIntegerParameters(new ScilabDouble());
	setObjectsParameters(new ScilabList());
	setExprs(new ScilabDouble());
	setBlockType("h");
	setNbZerosCrossing(new ScilabDouble(0));
	setNmode(new ScilabDouble(0));
    }
    
    public SuperBlock(String label, boolean masked) {
	this(label);
	if (masked) {
	    mask();
	}
    }

    /**
     * openBlockSettings
     * this method is called when a double click occured on a super block
     * /!\ WARNING /!\ this method must have the same signature as BasicBlock.openBlockSettings
     * @see BasicBlock.openBlockSettings
     * 
     */
    public void openBlockSettings(String[] context) {
	if(getParentDiagram() instanceof PaletteDiagram) {
	    return;
	}

	if (getChild() == null && getSimulationFunctionType().compareTo(SimulationFunctionType.DEFAULT) != 0) {
	    // This means we have a SuperBlock and we generated C code for it.
	    this.setLocked(false);
	    return;
	}

	if (isMasked()) {
	    super.openBlockSettings(context);
	} else {
	if(createChildDiagram() == true) {
	    getChild().setModifiedNonRecursively(false);
	    XcosTab.createTabFromDiagram(getChild());
	    XcosTab.showTabFromDiagram(getChild());
	} else {
	    getChild().setVisible(true);
	}
	
	
	getChild().updateCellsContext();
	}
    }

    public void closeBlockSettings() {

	// Do not ask the user, the diagram is saved and closed
	if (getChild().isModified()) {
	    setRealParameters(BlockWriter.convertDiagramToMList(getChild()));
	    getChild().setModified(true);
	    getChild().setModifiedNonRecursively(false);
	}

	if(getChild().canClose() == false) {
	    getChild().setVisible(false);
	    return;
	}

	if(getChild().getParentTab() != null) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getChild().getParentTab().getParentWindowId());
	    xcosWindow.removeTab(getChild().getParentTab());
	    getChild().getViewPort().close();
	    getChild().setOpened(false);
	    XcosTab.closeDiagram(getChild());
	    if(getParentDiagram().isOpened() && getParentDiagram().isVisible() == false) {
		getParentDiagram().closeDiagram();
	    }
	}

	child.removeListener(null);
	setLocked(false);
	child = null;
    }

    public void openContextMenu(ScilabGraph graph) {
	ContextMenu menu = null;

	if(getParentDiagram() instanceof PaletteDiagram) {
	    menu = createPaletteContextMenu(graph);
	} else { 
	    menu = createContextMenu(graph);
	    menu.getAsSimpleContextMenu().addSeparator();
	    menu.add(CodeGenerationAction.createMenu(graph));
	    
	    /* FIXME: It is not possible to use Mask. So remove any possibility.
	     * Mask removing only option is not applicable : if remove the mask
	     * you have no way to edit the values anymore.
	     */
	    /*
	    Menu maskMenu = ScilabMenu.createMenu();
	    maskMenu.setText(XcosMessages.SUPERBLOCK_MASK);
	    
	    if (isMasked()) {
		maskMenu.add(SuperblockMaskRemoveAction.createMenu(graph));
		menu.add(maskMenu);
	    } else {
		maskMenu.add(SuperblockMaskCreateAction.createMenu(graph));
	    }
	    maskMenu.add(SuperblockMaskCustomizeAction.createMenu(graph));
	    */
	}
	menu.setVisible(true);
    }
    
    public boolean createChildDiagram(){
	return createChildDiagram(false);
    }

    public boolean createChildDiagram(boolean generatedUID){
    	if (child == null) {
    	    child = new SuperBlockDiagram(this);
    	    child.installListeners();
    	    child.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters(), false));
    	    child.installSuperBlockListeners();
    	    child.setChildrenParentDiagram();
    	    updateAllBlocksColor();
    	    //only for loading and generate sub block UID
    	    if(generatedUID) {
    		child.generateUID();
    	    }
    	} else {
    		return false;
    	}
    	
    	return true;
    }
    
    public SuperBlockDiagram getChild() {
        return child;
    }

    public void setChild(SuperBlockDiagram child) {
        this.child = child;
    }

    public ScilabMList getAsScilabObj() {
	if (child != null) {
	    setRealParameters(BlockWriter.convertDiagramToMList(child));
	}
	return BasicBlockInfo.getAsScilabObj(this);
    }

    protected List<mxCell> getAllExplicitInBlock(){
	List<mxCell> list = new ArrayList<mxCell>();
	if(child == null){
	    return list;
	}

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof ExplicitInBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected List<mxCell> getAllImplicitInBlock(){
	List<mxCell> list = new ArrayList<mxCell>();
	if(child == null){
	    return list;
	}

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof ImplicitInBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected List<mxCell> getAllEventInBlock(){
	List<mxCell> list = new ArrayList<mxCell>();
	if(child == null){
	    return list;
	}

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof EventInBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected List<mxCell> getAllExplicitOutBlock(){
	List<mxCell> list = new ArrayList<mxCell>();
	if(child == null){
	    return list;
	}

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof ExplicitOutBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected List<mxCell> getAllImplicitOutBlock(){
	List<mxCell> list = new ArrayList<mxCell>();
	if(child == null){
	    return list;
	}

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof ImplicitOutBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected List<mxCell> getAllEventOutBlock(){
	List<mxCell> list = new ArrayList<mxCell>();

	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

	for(int i = 0 ; i < blockCount ; i++){
	    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
	    if(cell instanceof EventOutBlock){
		list.add(cell);
	    }
	}
	return list; 
    }

    protected int getBlocksConsecutiveUniqueValueCount(List<mxCell> blocks){
    	if(blocks == null){
    		return 0;
    	}

    	int count = blocks.size();
    	int[] array = new int[blocks.size()];

    	//initialize
    	for(int i = 0 ; i < array.length; i++){
    		array[i] = 0;
   		}
    	
    	//populate
    	for(int i = 0 ; i < array.length; i++){
    		int index = (Integer)((BasicBlock)blocks.get(i)).getValue();
    		if(index <= array.length){
    			array[index - 1] = 1;	
    		}
    	}
    	
    	//parse
    	for(int i = 0 ; i < array.length; i++){
    		if(array[i] == 0){
    			count = i;
    			break;
    		}
    	}
    	
    	return count;
    }

    public void updateAllBlocksColor(){
    	updateBlocksColor(getAllExplicitInBlock());
    	updateBlocksColor(getAllImplicitInBlock());
    	updateBlocksColor(getAllEventInBlock());

    	updateBlocksColor(getAllExplicitOutBlock());
    	updateBlocksColor(getAllImplicitOutBlock());
    	updateBlocksColor(getAllEventOutBlock());
    }
    
    private void updateBlocksColor(List<mxCell> blocks){

    	try{
    		child.getModel().beginUpdate();
    		if(blocks == null || blocks.size() == 0){
    			return;
    		}

    		int countUnique = getBlocksConsecutiveUniqueValueCount(blocks);
    		boolean isDone[] = new boolean[countUnique];

    		//Initialize
    		for(int i = 0 ; i < countUnique ; i++){
    			isDone[i] = false;
    		}


    		for(int i = 0 ; i < blocks.size() ; i++){
    			int index = (Integer)((BasicBlock)blocks.get(i)).getValue();
    			if(index > countUnique || isDone[index - 1] == true){
    				child.getAsComponent().setCellWarning(blocks.get(i), "Wrong port number");
    			}else{
    				isDone[index - 1] = true;
    				child.getAsComponent().setCellWarning(blocks.get(i), null);
    			}
    		}
    	}finally{
    		child.getModel().endUpdate();
    	}
    }

    public void updateExportedPort(){
    	if (child == null){
    		return;
    	}

    	updateExportedExplicitInputPort();
    	updateExportedImplicitInputPort();
    	updateExportedEventInputPort();
    	updateExportedExplicitOutputPort();
    	updateExportedImplicitOutputPort();
    	updateExportedEventOutputPort();
    	getParentDiagram().fireEvent(XcosEvent.SUPER_BLOCK_UPDATED, new mxEventObject(new Object[] {this}));
    }


    private void updateExportedExplicitInputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllExplicitInBlock());
    	List<ExplicitInputPort> ports = BasicBlockInfo.getAllExplicitInputPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		ExplicitInputPort port = new ExplicitInputPort();
    		port.setDataLines(-1);
    		port.setDataColumns(-2);
    		addPort(port);
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedImplicitInputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitInBlock());
    	List<ImplicitInputPort> ports = BasicBlockInfo.getAllImplicitInputPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new ImplicitInputPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedEventInputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllEventInBlock());
    	List<ControlPort> ports = BasicBlockInfo.getAllControlPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new ControlPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedExplicitOutputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllExplicitOutBlock());
    	List<ExplicitOutputPort> ports = BasicBlockInfo.getAllExplicitOutputPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		ExplicitOutputPort port = new ExplicitOutputPort();
    		port.setDataLines(-1);
    		port.setDataColumns(-2);
    		addPort(port);
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedImplicitOutputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitOutBlock());
    	List<ImplicitOutputPort> ports = BasicBlockInfo.getAllImplicitOutputPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new ImplicitOutputPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedEventOutputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllEventOutBlock());
    	List<CommandPort> ports = BasicBlockInfo.getAllCommandPorts(this, false);

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new CommandPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
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
