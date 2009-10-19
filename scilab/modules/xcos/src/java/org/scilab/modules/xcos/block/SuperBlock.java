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

import java.io.Serializable;
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
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

public class SuperBlock extends BasicBlock {

    private SuperBlockDiagram child = null;
    private XcosDiagram parentDiagram = null;

 
    public SuperBlock(){
	super();
	setVertex(false);
	setVisible(false);
    }

    public SuperBlock(String label) {
	super(label);
	setStyle("SUPER_f");
    }
    
    public XcosDiagram getParentDiagram() {
        return parentDiagram;
    }

    public void setParentDiagram(XcosDiagram parentDiagram) {
        this.parentDiagram = parentDiagram;
    }

    public void openBlockSettings(String context, XcosDiagram diagram) {
	this.setLocked(true);
	if (child == null) {
	    child = new SuperBlockDiagram(this);
	    child.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters()));
	}
	else {
	    SuperBlockDiagram newChild = new SuperBlockDiagram(this);
	    newChild.setModel(child.getModel());
	    child = newChild;
	}
	Xcos.showDiagram(child);
	child.installListeners();
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

    private class SuperBlockDiagram extends XcosDiagram implements Serializable {
	private SuperBlock container = null;

	public SuperBlockDiagram(SuperBlock superBlock) {
	    super();
	    this.container = superBlock;
	}

	protected void updateBlockWithValue(List<mxCell> blocks, String checkValue){
	    int count = getBlocksWithValueCount(blocks, checkValue);
	    if(count != 0){
		List<mxCell> list = getBlocksWithValue(blocks, checkValue);
		mxUtils.setCellStyles(getModel(), new Object[] {list.get(0)}, mxConstants.STYLE_FILLCOLOR, "white");

		Object[] objs = new Object[list.size() - 1];
		for(int j = 1 ; j < list.size() ; j++){
		    objs[j-1] = list.get(j);
		}
		mxUtils.setCellStyles(getModel(), objs, mxConstants.STYLE_FILLCOLOR, "red");
	    }
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
		    Object[] cells = (Object[]) evt.getArgs()[0];

		    for(int i = 0 ; i < cells.length ; i++){
			BasicBlock block = (BasicBlock)cells[i];
			if(block instanceof ExplicitInBlock){
			    List<mxCell> inputs = getAllExplicitInBlock();
			    int count = getBlocksWithValueCount(inputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				container.addPort(new ExplicitInputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}else if(block instanceof ImplicitInBlock){
			    List<mxCell> inputs = getAllImplicitInBlock();
			    int count = getBlocksWithValueCount(inputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				//container.addPort(new ImplicitInputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}else if(block instanceof EventInBlock){
			    List<mxCell> inputs = getAllEventInBlock();
			    int count = getBlocksWithValueCount(inputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				//container.addPort(new EventIntputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}else if(block instanceof ExplicitOutBlock){
			    List<mxCell> outputs = getAllExplicitOutBlock();
			    int count = getBlocksWithValueCount(outputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				//container.addPort(new ImplicitInputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}else if(block instanceof ImplicitOutBlock){
			    List<mxCell> outputs = getAllImplicitOutBlock();
			    int count = getBlocksWithValueCount(outputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				//container.addPort(new ImplicitOutputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}else if(block instanceof EventOutBlock){
			    List<mxCell> outputs = getAllEventOutBlock();
			    int count = getBlocksWithValueCount(outputs, (String)block.getValue());
			    if(count == 1){//1 for Me
				//container.addPort(new EventOuttputPort());
			    }else{
				mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
			    }
			}
		    }
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.CELLS_REMOVED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {
		    Object[] cells = (Object[]) evt.getArgs()[0];
		    List<mxCell> blocks = null;

		    for(int i = 0 ; i < cells.length ; i++){
			BasicBlock block = (BasicBlock)cells[i];
			if(block instanceof ExplicitInBlock){
			    blocks = getAllExplicitInBlock();
			}else if(block instanceof ImplicitInBlock){
			    blocks = getAllImplicitInBlock();
			}else if(block instanceof EventInBlock){
			    blocks = getAllEventInBlock();
			}else if(block instanceof ExplicitOutBlock){
			    blocks = getAllExplicitOutBlock();
			}else if(block instanceof ImplicitOutBlock){
			    blocks = getAllImplicitOutBlock();
			}else if(block instanceof EventOutBlock){
			    blocks = getAllEventOutBlock();
			}
			else{
			    continue;
			}

			updateBlockWithValue(blocks, (String)block.getValue());
		    }

		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> inputs = getAllExplicitInBlock();

		    updateBlockWithValue(inputs, oldVal);
		    updateBlockWithValue(inputs, newVal);
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> inputs = getAllImplicitInBlock();

		    updateBlockWithValue(inputs, oldVal);
		    updateBlockWithValue(inputs, newVal);
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.IN_EVENT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> inputs = getAllEventInBlock();

		    updateBlockWithValue(inputs, oldVal);
		    updateBlockWithValue(inputs, newVal);
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_EXPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> outputs = getAllExplicitOutBlock();

		    updateBlockWithValue(outputs, oldVal);
		    updateBlockWithValue(outputs, newVal);
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_IMPLICIT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> outputs = getAllImplicitOutBlock();

		    updateBlockWithValue(outputs, oldVal);
		    updateBlockWithValue(outputs, newVal);
		    updateExportedPort();
		}
	    });

	    addListener(XcosEvent.OUT_EVENT_VALUE_UPDATED, new mxIEventListener() {
		public void invoke(Object source, mxEventObject evt) {

		    Object[] cells = (Object[]) evt.getArgs();

		    String oldVal = (String)cells[0]; 
		    String newVal = (String)cells[1];

		    List<mxCell> outputs = getAllEventOutBlock();

		    updateBlockWithValue(outputs, oldVal);
		    updateBlockWithValue(outputs, newVal);
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

    protected int getBlocksUniqueValueCount(List<mxCell> blocks){
	if(blocks == null){
	    return 0;
	}

	int count = 0;
	List<String> list = new ArrayList<String>();

	for(int i = 0 ; i < blocks.size() ; i++){
	    boolean find = false;
	    String value = ((String)((BasicBlock)blocks.get(i)).getValue());
	    for(int j = 0 ; j < list.size() ; j++){
		if(list.get(j).compareTo(value) == 0){
		    find = true;
		    break;
		}
	    }
	    if(find == false){
		list.add(value);
		count++;
	    }
	}
	return count;
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
	int blockCount = getBlocksUniqueValueCount(getAllExplicitInBlock());
	List<ExplicitInputPort> ports = getAllExplicitInputPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new ExplicitInputPort());
	portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	portCount = ports.size();
	}
    }

    private void updateExportedImplicitInputPort(){
	int blockCount = getBlocksUniqueValueCount(getAllImplicitInBlock());
	List<ImplicitInputPort> ports = getAllImplicitInputPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new ImplicitInputPort());
	    portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	    portCount = ports.size();
	}
    }

    private void updateExportedEventInputPort(){
	int blockCount = getBlocksUniqueValueCount(getAllEventInBlock());
	List<ControlPort> ports = getAllControlPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new ControlPort());
	    portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	    portCount = ports.size();
	}
    }

    private void updateExportedExplicitOutputPort(){
	int blockCount = getBlocksUniqueValueCount(getAllExplicitOutBlock());
	List<ExplicitOutputPort> ports = getAllExplicitOutputPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new ExplicitOutputPort());
	    portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	    portCount = ports.size();
	}
    }

    private void updateExportedImplicitOutputPort(){
	int blockCount = getBlocksUniqueValueCount(getAllImplicitOutBlock());
	List<ImplicitOutputPort> ports = getAllImplicitOutputPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new ImplicitOutputPort());
	    portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	    portCount = ports.size();
	}
    }

    private void updateExportedEventOutputPort(){
	int blockCount = getBlocksUniqueValueCount(getAllEventOutBlock());
	List<CommandPort> ports = getAllCommandPorts();

	int portCount = ports.size();

	while(blockCount > portCount){ //add if required
	    addPort(new CommandPort());
	    portCount++;
	}

	while(portCount > blockCount){ //remove if required
	    ports.remove(ports.size() - 1);
	    portCount = ports.size();
	}
    }
}
