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

import javax.swing.JOptionPane;

import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosDiagram;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxEventObject;

public class SuperBlock extends BasicBlock {

    private transient SuperBlockDiagram child = null;
 
    public SuperBlock(){
	super();
	setVertex(false);
	setVisible(false);
    }

    public SuperBlock(String label) {
	super(label);
    }
    
    /**
     * openBlockSettings
     * this method is called when a double click occured on a super block
     * /!\ WARNING /!\ this method must have the same signature as BasicBlock.openBlockSettings
     * @see BasicBlock.openBlockSettings
     * 
     */
    public void openBlockSettings(String[] context) {
	this.setLocked(true);
	if (child == null) {
	    child = new SuperBlockDiagram(this);
	    child.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters()));
	    child.installListeners();
	    updateAllBlocksColor();
		int blockCount = child.getModel().getChildCount(child.getDefaultParent());
	    for(int i = 0 ; i < blockCount ; i++){
		    mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
		    if(cell instanceof BasicBlock){
		    	BasicBlock block = (BasicBlock)cell;
		    	block.setParentDiagram(child);
		    }
	    }
	}
	else {
	    SuperBlockDiagram newChild = new SuperBlockDiagram(this);
	    newChild.setModel(child.getModel());
	    child = newChild;
	    
	    child.installListeners();
	    updateAllBlocksColor();
	}
	Xcos.showDiagram(child);
    }
    
    public ScilabMList getAsScilabObj() {
	if (child != null) {
	    setRealParameters(BlockWriter.convertDiagramToMList(child));
	}
	return super.getAsScilabObj();
    }
    
    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append("<html>");
	result.append("SUPER BLOCK"+"<br>");
	result.append("Explicit input ports : "+getAllExplicitInputPorts().size()+"<br>");
	result.append("Implicit input ports : "+getAllImplicitInputPorts().size()+"<br>");
	result.append("Explicit output ports : "+getAllExplicitOutputPorts().size()+"<br>");
	result.append("Implicit output ports : "+getAllImplicitOutputPorts().size()+"<br>");
	result.append("Control ports : "+getAllControlPorts().size()+"<br>");
	result.append("Command ports : "+getAllCommandPorts().size()+"<br>");
	result.append("isLocked : "+isLocked()+"<br>");
	result.append("</html>");
	return result.toString();
    }

    private class SuperBlockDiagram extends XcosDiagram {
	private SuperBlock container = null;

	public SuperBlockDiagram(SuperBlock superBlock) {
	    super();
	    this.container = superBlock;
	}

	public void closeDiagram() {
	    boolean wantToClose = true;

	    if (isModified()) {
		// The diagram has been modified
		// Ask the user want he want to do !
		int choice = JOptionPane.showConfirmDialog(getAsComponent(), XcosMessages.DIAGRAM_MODIFIED);
		if (choice == 0) {
		    // Save the diagram
		    container.setRealParameters(BlockWriter.convertDiagramToMList(this));
		} else if (choice == 1) {
		    // The user selects no !
		} else if (choice == 2) {
		    // The user cancels
		    wantToClose = false;
		}
	    }

	    if (wantToClose) {
		ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentTab().getParentWindowId());
		xcosWindow.removeTab(getParentTab());
		getViewPort().close();
		Xcos.closeDiagram(this);
		this.removeListener(null);
		container.setLocked(false);
	    }
	}

	public void installListeners() {
	    super.installListeners();
	    addListener(XcosEvent.CELLS_ADDED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.CELLS_REMOVED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
			updateAllBlocksColor();
		    updateExportedPort();
		}
	    });
	}

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

    protected int getBlocksWithValueCount(List<mxCell> blocks, String checkValue){
	int count = 0;
	for(int i = 0 ; i < blocks.size() ; i++){
	    if(((String)((BasicBlock)blocks.get(i)).getValue()).compareTo(checkValue) == 0){
		count++;
	    }
	}
	return count;
    }

    protected List<mxCell> getBlocksWithValue(List<mxCell> blocks, String checkValue){
	List<mxCell> list = new ArrayList<mxCell>();

	for(int i = 0 ; i < blocks.size() ; i++){
	    if(((String)((BasicBlock)blocks.get(i)).getValue()).compareTo(checkValue) == 0){
		list.add((BasicBlock)blocks.get(i));
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
    		int index = Integer.parseInt((String)((BasicBlock)blocks.get(i)).getValue());
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

    private void updateAllBlocksColor(){
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
    			int index = Integer.parseInt((String)((BasicBlock)blocks.get(i)).getValue());
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

    private void updateExportedPort(){
    	if(child == null){
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
    	List<ExplicitInputPort> ports = getAllExplicitInputPorts();

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new ExplicitInputPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedImplicitInputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitInBlock());
    	List<ImplicitInputPort> ports = getAllImplicitInputPorts();

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
    	List<ControlPort> ports = getAllControlPorts();

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
    	List<ExplicitOutputPort> ports = getAllExplicitOutputPorts();

    	int portCount = ports.size();

    	while(blockCount > portCount){ //add if required
    		addPort(new ExplicitOutputPort());
    		portCount++;
    	}

    	while(portCount > blockCount){ //remove if required
    		removePort(ports.get(portCount - 1));
    		portCount--;
    	}
    }

    private void updateExportedImplicitOutputPort(){
    	int blockCount = getBlocksConsecutiveUniqueValueCount(getAllImplicitOutBlock());
    	List<ImplicitOutputPort> ports = getAllImplicitOutputPorts();

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
    	List<CommandPort> ports = getAllCommandPorts();

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
}
