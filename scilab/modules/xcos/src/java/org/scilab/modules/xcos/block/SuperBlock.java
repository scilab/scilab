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

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
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
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

public class SuperBlock extends BasicBlock {

	private XcosDiagram child = null;

	public SuperBlock(String label) {
		super(label);
		setStyle("SUPER_f");
	}

	public void openBlockSettings(String context, XcosDiagram diagram) {
		this.setLocked(true);
		System.err.println("child = " + child);
		if (child == null) {
			child = new SuperBlockDiagram(this);
			child.loadDiagram(BlockReader.convertMListToDiagram((ScilabMList) getRealParameters()));
			String innerContext = child.getContext();

			if(!innerContext.contains(context)) {
				innerContext = innerContext + ";" + context;
				child.setContext(innerContext);
			}

			child.getAsComponent().addPropertyChangeListener(new PropertyChangeListener() {
				public void propertyChange(PropertyChangeEvent arg0) {
					if(arg0.getPropertyName().compareTo("modified") == 0) {
						//diagram.fireEvent(mxEvent.NOTIFY);
					}
				}
			});
		}

		// FIXME : Should work like this !!!
		Xcos.showDiagram(child);

//		XcosDiagram dg = Xcos.createEmptyDiagram();
//		dg.setModel(child.getModel());
//		child = dg;
	}

	public String getToolTipText() {
		StringBuffer result = new StringBuffer();
		result.append("<html>");
		result.append("SUPER BLOCK"+"<br>");
		result.append("Input ports : "+getAllInputPorts().size()+"<br>");
		result.append("Output ports : "+getAllOutputPorts().size()+"<br>");
		result.append("Control ports : "+getAllControlPorts().size()+"<br>");
		result.append("Command ports : "+getAllCommandPorts().size()+"<br>");
		result.append("isLocked : "+isLocked()+"<br>");
		result.append("</html>");
		return result.toString();
	}

	private class SuperBlockDiagram extends XcosDiagram{
		private SuperBlock container = null;

		public SuperBlockDiagram(SuperBlock superBlock) {
			super();
			this.container = superBlock;

			addListener(XcosEvent.CELLS_ADDED, new mxIEventListener() {
				public void invoke(Object source, mxEventObject evt) {
					System.err.println("ADD CELL");
					Object[] cells = (Object[]) evt.getArgs()[0];

					List<OutBlock> outputs = (List)getAllOutBlock();
					List<InBlock> inputs = (List)getAllInBlock();
					
					for(int i = 0 ; i < cells.length ; i++){
						BasicBlock block = (BasicBlock)cells[i];
						if(block instanceof InBlock){
							int count = getBlocksWithValueCount((List)inputs, (String)block.getValue());
							if(count == 1){//1 for Me
								//container.addPort(new ExplicitInputPort());
							}else{
								mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
								System.err.println("Can't add external input ports");
							}
						}else if(block instanceof OutBlock){
							int count = getBlocksWithValueCount((List)outputs, (String)block.getValue());
							if(count == 1){//1 for Me
								//container.addPort(new ExplicitOutputPort());
							}else{
								mxUtils.setCellStyles(getModel(), new Object[] {block}, mxConstants.STYLE_FILLCOLOR, "red");
								System.err.println("Can't add external output ports");
							}
						}
					}
				}
			});

			addListener(XcosEvent.CELLS_REMOVED, new mxIEventListener() {
				public void invoke(Object source, mxEventObject evt) {
					System.err.println("REMOVE CELL");
					Object[] cells = (Object[]) evt.getArgs()[0];
					List<InBlock> inputs = (List)getAllInBlock();
					List<OutBlock> outputs = (List)getAllOutBlock();
					
					for(int i = 0 ; i < cells.length ; i++){
						BasicBlock block = (BasicBlock)cells[i];
						if(block instanceof InBlock){
							String value = (String)block.getValue();

							int count = getBlocksWithValueCount((List)inputs, value);
							System.err.println("getBlocksWithValueCount : " + (String)block.getValue() + " -> " + count);
							if(count != 0){
								List<mxCell> list = getBlocksWithValue((List)inputs, value);
								System.err.println("getBlocksWithValue : " + list.size());
								mxUtils.setCellStyles(getModel(), new Object[] {list.get(0)}, mxConstants.STYLE_FILLCOLOR, "white");
								for(int j = 1 ; j < list.size() ; j++){
									System.err.println("On passe la ?");
									mxUtils.setCellStyles(getModel(), new Object[] {list.get(j)}, mxConstants.STYLE_FILLCOLOR, "red");
								}
							}
						}else if(block instanceof OutBlock){
						}
					}
					
					int inputport = getBlocksUniqueValueCount((List)inputs);
					if(container.getAllInputPorts().size() > inputport){
						while(container.getAllInputPorts().size() > inputport){
							//container.getAllInputPorts().remove(container.getAllInputPorts().size()-1);
						}
					}else{
						while(container.getAllInputPorts().size() < inputport){
							//container.addPort(new ExplicitInputPort());
						}
					}
					
					int outputport = getBlocksUniqueValueCount((List)outputs);
					if(container.getAllOutputPorts().size() > outputport){
						while(container.getAllOutputPorts().size() > outputport){
							//container.getAllOutputPorts().remove(container.getAllOutputPorts().size()-1);
						}
					}else{
						while(container.getAllOutputPorts().size() < outputport){
							//container.addPort(new ExplicitOutputPort());
						}
					}
				}
			});

			addListener(XcosEvent.VALUE_UPDATED, new mxIEventListener() {
				public void invoke(Object source, mxEventObject evt) {
					System.err.println("VALUE_UPDATED");
					
					if(evt.getArgCount() != 2){
						return;
					}
					
					
					
				}
			});
			
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
	}

    protected List<mxCell> getAllInBlock(){
    	List<mxCell> list = new ArrayList<mxCell>();

    	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

    	for(int i = 0 ; i < blockCount ; i++){
    		mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
    		if(cell instanceof InBlock){
    			list.add(cell);
    		}
    	}
    	return list; 
    }

    protected List<mxCell> getAllOutBlock(){
    	List<mxCell> list = new ArrayList<mxCell>();

    	int blockCount = child.getModel().getChildCount(child.getDefaultParent());

    	for(int i = 0 ; i < blockCount ; i++){
    		mxCell cell = (mxCell)child.getModel().getChildAt(child.getDefaultParent(), i);
    		if(cell instanceof OutBlock){
    			list.add((InBlock)cell);
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
}
