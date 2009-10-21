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

import java.awt.Color;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.PasteAction;
import org.scilab.modules.graph.actions.RedoAction;
import org.scilab.modules.graph.actions.SelectAllAction;
import org.scilab.modules.graph.actions.UndoAction;
import org.scilab.modules.graph.actions.ZoomInAction;
import org.scilab.modules.graph.actions.ZoomOutAction;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.actions.XcosShortCut;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.io.BlockReader;
import org.scilab.modules.xcos.io.BlockWriter;
import org.scilab.modules.xcos.io.XcosCodec;
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
import org.scilab.modules.xcos.utils.Signal;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxMultiplicity;

public class XcosDiagram extends ScilabGraph {

    // Default values : SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
    private double finalIntegrationTime = 100000;
    private double integratorAbsoluteTolerance = 1e-4;
    private double integratorRelativeTolerance = 1e-6;
    private double toleranceOnTime = 1e-10;
    private double maxIntegrationTimeinterval = finalIntegrationTime + 1;
    private double realTimeScaling = 0;
    private double solver = 0;
    private double maximumStepSize = 0;
    private String[] context = new String[]{""};
    private List doc = null;
    private String version = "scicos4.2";

    private Tab parentTab;
    //private Window palette;
    private Tab viewPort;

    private CheckBoxMenuItem viewPortMenu;
    private CheckBoxMenuItem gridMenu;

    public Object addEdge(Object edge, Object parent, Object source,
    		Object target, Integer index)
    {	
    	System.err.println("[DEBUG] AddEdge :");
    	System.err.println("[DEBUG] AddEdge : source "+source.toString());
    	System.err.println("[DEBUG] AddEdge : target "+target.toString());
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

    	// ImplicitInput -> ImplicitInput
    	if (source instanceof ImplicitInputPort) {
    		if(target instanceof ImplicitInputPort) {
    			return super.addEdge(new ImplicitLink(), parent, source, target, index);
    		}
    	}
    	// ImplicitOutputPort -> ImplicitOutput
    	// Switch source and target !
    	if(target instanceof ImplicitOutputPort) {
    		if (source instanceof ImplicitOutputPort) {
    			return super.addEdge(new ImplicitLink(), parent, target, source, index);
    		}
    	}
    	
    	// ExplicitLink -> ExplicitInputPort
    	if (source instanceof ExplicitLink) {
    		if(target instanceof ExplicitInputPort) {

    			BasicLink link = (BasicLink)source;
    			mxCell linkSource =  (mxCell)link.getSource();
    			mxCell linkTarget =  (mxCell)link.getTarget();

    			mxGeometry geomSource = linkSource.getParent().getGeometry();
    			mxGeometry geomTarget = linkTarget.getParent().getGeometry();

    			double offsetX = geomTarget.getCenterX() -  geomSource.getCenterX();
    			double offsetY = geomTarget.getCenterY() -  geomSource.getCenterY();

    			BasicBlock splitBlock = BasicBlock.createBlock("SPLIT_f");
    			splitBlock.setStyle("SPLIT_f");
    			addCell(splitBlock);
    			mxRectangle splitRect = new mxRectangle();
    			splitRect.setX(geomSource.getCenterX() + offsetX / 2);
    			splitRect.setY(geomSource.getCenterY() + offsetY / 2);
    			splitRect.setWidth(6);
    			splitRect.setHeight(6);
    			cellsResized(new Object[]{splitBlock}, new mxRectangle[]{splitRect});

    			//remove old link
    			removeCells(new Object[]{link});
    			
    		    BasicLink newLink1 = new ExplicitLink();
    		    newLink1.setGeometry(new mxGeometry(0,0,80,80));
    		    newLink1.setSource(linkSource);
    		    newLink1.setTarget(splitBlock);
    		    addCell(newLink1);

    		    BasicLink newLink2 = new ExplicitLink();
    		    newLink2.setGeometry(new mxGeometry(0,0,80,80));
    		    newLink2.setSource(splitBlock);
    		    newLink2.setTarget(linkTarget);
    		    addCell(newLink2);

    		    BasicLink newLink3 = new ExplicitLink();
    		    newLink3.setGeometry(new mxGeometry(0,0,80,80));
    		    newLink3.setSource(splitBlock);
    		    newLink3.setTarget((mxCell)target);
    		    addCell(newLink3);

    		    return newLink3;
    		}
    	}
    	// ImplicitOutput -> ExpliciLink
    	// Switch source and target !
    	if(target instanceof ExplicitLink) {
    		if (source instanceof ExplicitInputPort) {
    			System.err.println("Input vers Link");
    			//return super.addEdge(new ImplicitLink(), parent, target, source, index);
    		}
    	}

    	System.err.println("************** link error **************");
    	return null;
    }

    public XcosDiagram() {
	super();
	keyboardHandler = new XcosShortCut(this);
	mxCodec codec = new mxCodec();

	try {
	    File uri = new File(System.getenv("SCI"));
	    String xml = mxUtils.readFile(System.getenv("SCI")+"/modules/xcos/etc/Xcos-style.xml");
	    xml = xml.replaceAll("\\$SCILAB", uri.toURI().toURL().toString());
	    Document doc = mxUtils.parse(xml);
	    codec.decode(doc.getDocumentElement(), getStylesheet());
	} catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}

	getAsComponent().setToolTips(true);

	// Forbid disconnecting cells once it is connected.
	setCellsDisconnectable(false);

	// Forbid pending edges.
	setAllowDanglingEdges(false);

	// Cannot connect port to itself.
	setAllowLoops(false);

	// Override isCellResizable to filter what the user can resize
	setCellsResizable(true);

	// force auto resize cell
	setAutoSizeCells(true);

	/* Labels use HTML if not equal to interface function name */
	setHtmlLabels(true);

	// Override isCellEditable to filter what the user can edit
	setCellsEditable(true);
	// This enable stop editing cells when pressing Enter.
	getAsComponent().setEnterStopsCellEditing(false);

	setConnectableEdges(true);
	getAsComponent().setTolerance(1);
	
	getAsComponent().getViewport().setOpaque(false);
	getAsComponent().setBackground(Color.WHITE);

	mxMultiplicity[] multiplicities = new mxMultiplicity[6];

	// Input data port
	multiplicities[0] = new PortCheck(new ExplicitInputPort(), new mxCell[] {new ExplicitOutputPort(), new ImplicitLink()}, XcosMessages.LINK_ERROR_EXPLICIT_IN);
	multiplicities[1] = new PortCheck(new ImplicitInputPort(), new mxCell[] {new ImplicitOutputPort(), new ImplicitInputPort()}, XcosMessages.LINK_ERROR_IMPLICIT_IN);

	//Output data port
	multiplicities[2] = new PortCheck(new ExplicitOutputPort(), new mxCell[] {new ExplicitInputPort()}, XcosMessages.LINK_ERROR_EXPLICIT_OUT);
	multiplicities[3] = new PortCheck(new ImplicitOutputPort(), new mxCell[] {new ImplicitInputPort(), new ImplicitOutputPort()}, XcosMessages.LINK_ERROR_IMPLICIT_OUT);

	//Control
	multiplicities[4] = new PortCheck(new ControlPort(), new mxCell[] {new CommandPort()}, XcosMessages.LINK_ERROR_EVENT_IN);

	//Command port
	multiplicities[5] = new PortCheck(new CommandPort(), new mxCell[] {new ControlPort()}, XcosMessages.LINK_ERROR_EVENT_OUT);

	setMultiplicities(multiplicities);
//	getAsComponent().validateGraph();

    }

    /**
     * Install all needed Listeners.
     */
    public void installListeners() {

    	System.err.println("installListeners");
	//
	// Property change Listener
	// Will say if a diagram has been modified or not.
	getAsComponent().addPropertyChangeListener(new PropertyChangeListener() {
	    public void propertyChange(PropertyChangeEvent arg0) {
		if (arg0.getPropertyName().compareTo("modified") == 0) {
		    if ((Boolean) arg0.getOldValue() != (Boolean) arg0.getNewValue()) {
			updateTabTitle();
		    }
		}
	    }
	});

	// Add a listener to track when model is changed
	getModel().addListener(XcosEvent.CHANGE, new ModelTracker());

	addListener(XcosEvent.UPDATE_CELL_SIZE, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
	    }
	});
	
	addListener(XcosEvent.SUPER_BLOCK_UPDATED, new mxIEventListener()
	{
		public void invoke(Object sender, mxEventObject evt)
		{
			if(evt.getArgs()[0] instanceof SuperBlock){
				refresh();
			}
		}
	});


	
	// Track when superblock ask a parent refresh.
	addListener(XcosEvent.SUPER_BLOCK_UPDATED, new SuperBlockUpdateTracker()); 

	// Track when cells are added.
	addListener(XcosEvent.CELLS_ADDED, new CellAddedTracker(this)); 
		
	// Track when resizing a cell.
	addListener(XcosEvent.CELLS_RESIZED, new CellResizedTracker());

	getAsComponent().getGraphControl().addMouseListener(new XcosMouseListener(this));

	addListener(XcosEvent.ADD_PORTS, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getModel().beginUpdate();
		refresh();
		getModel().endUpdate();
	    }
	});	
    }

    /**
     * modelTracker
     * Called when mxEvents.CHANGE occurs on a model
     */
    private class ModelTracker implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    //System.err.println("[DEBUG] Model just changed");
	    List changes = (List) evt.getArgAt(0);
	    for (int i = 0 ; i < changes.size() ; ++i) {
		//System.err.println("args[" + i + "] = " + changes.get(i));
		if (changes.get(i) instanceof mxChildChange) {
		    //System.err.println("Change cell = "+((mxChildChange) changes.get(i)).getChild().toString());
		    if (((mxChildChange) changes.get(i)).getChild() instanceof SplitBlock) {
		    	continue;
		    }
		
		if (((mxChildChange) changes.get(i)).getChild() instanceof BasicBlock) {
			BasicBlock currentCell = (BasicBlock) ((mxChildChange) changes.get(i)).getChild();
			getModel().beginUpdate();

			if (getCellStyle(currentCell).get("displayedLabel") != null) {
				((mxCell) currentCell).setValue("<html><body> "+getCellStyle(currentCell).get("displayedLabel")+" </body></html>");
			}

			mxRectangle preferedSize = getPreferredSizeForCell(currentCell);
			mxGeometry cellSize = ((mxCell) currentCell).getGeometry();

			((mxCell) currentCell).setGeometry(new mxGeometry(cellSize.getX(), cellSize.getY(),
					Math.max(preferedSize.getWidth(), cellSize.getWidth()),
					Math.max(preferedSize.getHeight(), cellSize.getHeight())));
			cellsResized(new Object[] { currentCell }, new mxRectangle[] { ((mxCell) currentCell).getGeometry() });
			//refresh();
			getModel().endUpdate();
			//((BasicBlock) ((mxGeometryChange) changes.get(i)).getCell()).updateBlockView();
		    }
		}
	    }
	}
    }

    private class SuperBlockUpdateTracker implements mxIEventListener {

    	public SuperBlockUpdateTracker() {
    	}

    	public void invoke(Object source, mxEventObject evt) {
    		refresh();
    	}
    }

    /**
     * cellAddedTracker
     * Called when mxEvents.CELLS_ADDED is fired.
     */
    private class CellAddedTracker implements mxIEventListener {
    	private XcosDiagram diagram = null;

    	public CellAddedTracker(XcosDiagram diagram) {
    		this.diagram = diagram;
    	}

    	public void invoke(Object source, mxEventObject evt) {
    		System.err.println("[DEBUG] CELLS_ADDED");
    		Object[] cells = (Object[]) evt.getArgs()[0];
    		for (int i = 0 ; i < cells.length ; ++i) {
    			if (cells[i] instanceof BasicBlock) {
    				if(cells[i] instanceof AfficheBlock){
    					AfficheBlock affich = (AfficheBlock)cells[i];
    					Xcos.getAfficheBlocks().put(affich.getHashCode(), affich);
    				}
    				
    				((BasicBlock) cells[i]).setParentDiagram(diagram);

    				getModel().beginUpdate();

    				if (getCellStyle(cells[i]).get("displayedLabel") != null) {
    					((mxCell) cells[0]).setValue("<html><body> "+getCellStyle(cells[i]).get("displayedLabel")+" </body></html>");
    				}

    				mxRectangle preferedSize = getPreferredSizeForCell(cells[i]);
    				mxGeometry cellSize = ((mxCell) cells[i]).getGeometry();

    				((mxCell) cells[i]).setGeometry(new mxGeometry(cellSize.getX(), cellSize.getY(),
    						Math.max(preferedSize.getWidth(), cellSize.getWidth()),
    						Math.max(preferedSize.getHeight(), cellSize.getHeight())));
    				cellsResized(new Object[] { cells[i] }, new mxRectangle[] { ((mxCell) cells[i]).getGeometry() });
    				refresh();
    				getModel().endUpdate();
    			}
    		}
    		System.err.println("[DEBUG] CELLS_ADDED END");
    	}
    }

    /**
     * cellResizedTracker
     * Called when mxEvents.CELLS_RESIZED is fired. 
     */
    private class CellResizedTracker implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getArgs()[0];

	    for (int i = 0 ; i < cells.length ; ++i) {
		if (cells[i] instanceof BasicBlock) {
		    getModel().beginUpdate();
		    ((BasicBlock) cells[i]).updateBlockView();
		    getModel().endUpdate();
		}
	    }
	}
    }


    /**
     * MouseListener inner class
     */
    private class XcosMouseListener implements MouseListener {
	private XcosDiagram diagram = null;

	public XcosMouseListener(XcosDiagram diagram) {
	    this.diagram = diagram;
	}

	public void mouseClicked(MouseEvent arg0) {
	    Object cell = getAsComponent().getCellAt(arg0.getX(), arg0.getY());

	    // Double Click within empty diagram Area
	    if (arg0.getClickCount() >= 2 && SwingUtilities.isLeftMouseButton(arg0) && cell == null) {
		TextBlock textBlock = new TextBlock("Edit me !!!");
		textBlock.getGeometry().setX(arg0.getX() - textBlock.getGeometry().getWidth() / 2.0);
		textBlock.getGeometry().setY(arg0.getY() - textBlock.getGeometry().getWidth() / 2.0);
		addCell(textBlock);
		return;
	    }

	    // Double Click within some component
	    if (arg0.getClickCount() >= 2 && SwingUtilities.isLeftMouseButton(arg0) && cell != null)
	    {
		getModel().beginUpdate();
		if (cell instanceof BasicBlock 
			&& !(cell instanceof TextBlock)) {
		    BasicBlock block = (BasicBlock) cell;
		    arg0.consume();
		    block.openBlockSettings(getContext());
		    //getAsComponent().setCellWarning(block, "truc a la con");
		}
		if (cell instanceof BasicLink) {
		    ((BasicLink) cell).insertPoint(arg0.getX(), arg0.getY());
		}
		getModel().endUpdate();
		refresh();
	    }

	    // Ctrl + Shift + Right Double Click : for debug !!
	    if (arg0.getClickCount() >= 2 && SwingUtilities.isMiddleMouseButton(arg0)
		    && arg0.isShiftDown() && arg0.isControlDown())
	    {
		System.err.println("[DEBUG] Click at position : "+arg0.getX()+" , "+arg0.getY());
		System.err.println("[DEBUG] Click on : "+cell);
		System.err.println("[DEBUG] Style : "+((mxCell) cell).getStyle());
		if(cell != null) {
		    System.err.println("[DEBUG] NbEdges : "+((mxCell) cell).getEdgeCount());
		    System.err.println("[DEBUG] NbChildren : "+((mxCell) cell).getChildCount());
		}
	    }

	    // Context menu
	    if (arg0.getClickCount() == 1 && SwingUtilities.isRightMouseButton(arg0)) {

		if (cell == null) {
		    // Display diagram context menu
		    ContextMenu menu = ScilabContextMenu.createContextMenu();

		    menu.add(UndoAction.undoMenu((ScilabGraph) getAsComponent().getGraph()));
		    menu.add(RedoAction.redoMenu((ScilabGraph) getAsComponent().getGraph()));
		    menu.add(PasteAction.pasteMenu((ScilabGraph) getAsComponent().getGraph()));
		    menu.add(SelectAllAction.createMenu((ScilabGraph) getAsComponent().getGraph()));
		    /*---*/
		    menu.getAsSimpleContextMenu().addSeparator();
		    /*---*/
		    menu.add(SetContextAction.createMenu((ScilabGraph) getAsComponent().getGraph()));
		    menu.add(SetupAction.createMenu((ScilabGraph) getAsComponent().getGraph()));
		    /*---*/
		    menu.getAsSimpleContextMenu().addSeparator();
		    /*---*/
		    menu.add(ZoomInAction.zoominMenu((ScilabGraph) getAsComponent().getGraph()));
		    menu.add(ZoomOutAction.zoomoutMenu((ScilabGraph) getAsComponent().getGraph()));
		    /*---*/
		    menu.getAsSimpleContextMenu().addSeparator();
		    /*---*/
		    menu.add(DiagramBackgroundAction.createMenu((ScilabGraph) getAsComponent().getGraph()));
		    /*---*/
		    menu.getAsSimpleContextMenu().addSeparator();
		    /*---*/
		    menu.add(XcosDocumentationAction.createMenu((ScilabGraph) getAsComponent().getGraph()));

		    ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
		    
		    menu.setVisible(true);

		} else {
		    // Display object context menu
		    if (cell instanceof BasicBlock && !(cell instanceof TextBlock)) {
			BasicBlock block = (BasicBlock) cell;
			block.openContextMenu((ScilabGraph) getAsComponent().getGraph());
			getAsComponent().doLayout();
		    }
		}
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

	public boolean isCellSelectable(Object cell)
	{
//		if(cell instanceof BasicLink){
//			return false;
//		}
		return super.isCellSelectable(cell);
	}

	public boolean isCellMovable(Object cell) {
	if(cell instanceof BasicPort){
	    return false;
	}

	boolean movable = false;
	Object[] cells =  this.getSelectionCells();

	//don't move if selection is only links
	for(int i = 0 ; i < cells.length ; i++){
	    if(!(cells[i] instanceof BasicLink)){
		movable = true;
		break;
	    }
	}

	return movable && super.isCellMovable(cell);
    }

    public boolean isCellResizable(Object cell) {
    	if(cell instanceof SplitBlock){
    		return false;
    	}
	return (cell instanceof BasicBlock) && super.isCellResizable(cell);
    }

    public boolean isCellDeletable(Object cell) {
	if (cell instanceof BasicBlock && ((BasicBlock) cell).isLocked()) {
	    return false;
	}
	return !(cell instanceof BasicPort)	&& super.isCellDeletable(cell);
    }

    public boolean isCellEditable(Object cell) {
	return (cell instanceof TextBlock) && super.isCellDeletable(cell);
    }

    public boolean isCellConnectable(Object cell)
    {
	return !(cell instanceof BasicBlock) && super.isCellConnectable(cell);
    }

    public boolean isAutoSizeCell(Object cell){
	return (cell instanceof AfficheBlock) || super.isAutoSizeCell(cell);
    }


    public void dumpToHdf5File(String fileName) {
	if (fileName == null) {
	    FileChooser fc = ScilabFileChooser.createFileChooser();
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		return;
	    }
	    fileName = fc.getSelection()[0];
	    System.out.println("Saving to file : {"+fileName+"}");
	}

	BlockWriter.writeDiagramToFile(fileName, this);
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

    public Tab getParentTab() {
	return parentTab;
    }

    public void setParentTab(Tab parentTab) {
	this.parentTab = parentTab;
    }

    /**
     * Set the associated ViewPort
     * @param viewPort the Viewport
     */
    public void setViewPort(Tab viewPort) {
	this.viewPort = viewPort;
    }

    /**
     * Get the associated ViewPort
     * @return the Viewport
     */
    public Tab getViewPort() {
	return viewPort;
    }

    /**
     * Manage the visibility of the associated viewport
     * @param status new status
     */
    public void setViewPortVisible(boolean status) {
	// Hide/Show parent window if the viewport is the only tab
	if (viewPort.getParentWindow().getNbDockedObjects() == 1) {
	    viewPort.getParentWindow().setVisible(status);
	}
	// Hide/Show viewport tab
	viewPort.setVisible(status);

	// (Un)Check the corresponding menu
	viewPortMenu.setChecked(status);
    }

    /**
     * Set menu used to manage Viewport visibility
     * @param menu the menu
     */
    public void setViewPortMenuItem(CheckBoxMenuItem menu) {
	this.viewPortMenu = menu;
    }
    /**
     * Manage the visibility of the grid and the associated menu
     * @param status new status
     */
    public void setGridVisible(boolean status) {
	setGridEnabled(true);
	getAsComponent().setGridVisible(status);
	getAsComponent().repaint();

	// (Un)Check the corresponding menu
	gridMenu.setChecked(status);
    }

    /**
     * Set menu used to manage Grid visibility
     * @param menu the menu
     */
    public void setGridMenuItem(CheckBoxMenuItem menu) {
	this.gridMenu = menu;
    }
    /**
     * Close Xcos instance including all tabs
     */
    public void closeDiagram() {

	boolean wantToClose = true;

	if (isModified()) {
	    // The diagram has been modified
	    // Ask the user want he want to do !
	    int choice = JOptionPane.showConfirmDialog(getAsComponent(), XcosMessages.DIAGRAM_MODIFIED);
	    if (choice == 0) {
		// Save the diagram
		saveDiagram();
	    } else if (choice == 1) {
		// The user selects no !
	    } else if (choice == 2) {
		// The user cancels
		wantToClose = false;
	    }
	}

	if (wantToClose) {
	    ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(parentTab.getParentWindowId());
	    xcosWindow.removeTab(parentTab);
	    //palette.getAsSimpleWindow().close();
	    viewPort.close();
	    Xcos.closeDiagram(this);
	}
    }

    public boolean saveDiagram() {
	boolean isSuccess = false;
	if (getTitle().equals(XcosMessages.UNTITLED)) {
	    isSuccess = saveDiagramAs();
	} 
	else {
	    isSuccess = BlockWriter.writeDiagramToFile(getTitle(), this);
	}

	if(isSuccess) {
	    setModified(false);
	}

	return isSuccess;
    }

    public boolean saveDiagramAs() {

	boolean isSuccess = false;
	String fileName;

	// Choose a filename
	FileChooser fc = ScilabFileChooser.createFileChooser();
	fc.setTitle(XcosMessages.SAVE_AS);
	fc.setUiDialogType(JFileChooser.SAVE_DIALOG);
	fc.setMultipleSelection(false);
	String[] mask = {"*.xcos"};
	String[] maskDesc = {"Xcos file (XML)"};  
	((SwingScilabFileChooser) fc.getAsSimpleFileChooser()).addMask(mask , maskDesc);
	fc.displayAndWait();

	if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
	    return isSuccess;
	}
	fileName = fc.getSelection()[0];

	/* Extension checks */
	String extension = fileName.substring(fileName.lastIndexOf('.') + 1);
	if (extension.equals(fileName)) {
	    /* No extension given --> .xcos added */
	    fileName += ".xcos";
	} else if (!extension.equals(".xcos")) {
	    XcosDialogs.couldNotSaveFile();
	    return false;
	}

	XcosCodec codec = new XcosCodec();
	String xml = mxUtils.getXml(codec.encode(this));

	System.out.println("Saving to file : {" + fileName + "}");

	/* Test if file already exists */
	if (new File(fileName).exists()
		&& JOptionPane.showConfirmDialog(this.getAsComponent(),
			XcosMessages.OVERWRITE_EXISTING_FILE) != JOptionPane.YES_OPTION) {
	    return false;
	}

	try {
	    mxUtils.writeFile(xml, fileName);
	    isSuccess = true;
	} catch (IOException e1) {
	    // TODO Auto-generated catch block
	    e1.printStackTrace();
	    isSuccess = false;
	}

	if (isSuccess) {
	    this.setTitle(fileName);
	    setModified(false);
	} else {
	    XcosDialogs.couldNotSaveFile();
	}

	return isSuccess;
    }

    public void setTitle(String title) {
	super.setTitle(title);
	updateTabTitle();
    }

    public void updateTabTitle() {
	String tabTitle = !isModified() ? getTitle() : "* "+getTitle();
	if (parentTab != null) {
	    parentTab.setName(tabTitle);
	}
    }

    public void setContext(String[] context){
	this.context = context;
    }

    public String[] getContext() {
	return context;
    }

    public String getVersion() {
	return version;
    }

    private BasicLink createLinkFromPorts(BasicPort from, BasicPort to) {
	if (from instanceof ExplicitOutputPort && to instanceof ExplicitInputPort) {
	    return new ExplicitLink();
	}
	if (from instanceof ImplicitOutputPort && to instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	}

	if (from instanceof ImplicitOutputPort && to instanceof ImplicitOutputPort) {
	    return new ImplicitLink();
	}

	if (from instanceof ImplicitInputPort && to instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	}

	if (from instanceof CommandPort && to instanceof ControlPort) {
	    return new CommandControlLink();
	}
	if (to instanceof ExplicitOutputPort && from instanceof ExplicitInputPort) {
	    return new ExplicitLink();
	}
	if (to instanceof ImplicitOutputPort && from instanceof ImplicitInputPort) {
	    return new ImplicitLink();
	}
	if (to instanceof CommandPort && from instanceof ControlPort) {
	    return new CommandControlLink();
	}
	return new ExplicitLink();
    }

    /**
     * Open a Diagram :
     * If current Diagram is empty, open within it
     * else open a new window.
     * 
     * @param diagramm
     */
    public void openDiagram(HashMap<String, Object> diagramm) {
	if (diagramm != null) {
	    if (getModel().getChildCount(getDefaultParent()) == 0) {
		loadDiagram(diagramm);
	    } else {
		System.err.println("openDiagram");
		XcosDiagram xcosDiagram = Xcos.createEmptyDiagram();
		xcosDiagram.loadDiagram(diagramm);
	    }
	} else {
	    XcosDialogs.couldNotLoadFile();
	}
    }

    /**
     * Load a Diagramm structure into current window.
     * 
     * @param diagramm
     */
    public void loadDiagram(HashMap<String, Object> diagramm) {
	List<BasicBlock> allBlocks = (List) diagramm.get("Blocks");
	HashMap<String, Object> allLinks = (HashMap<String, Object>) diagramm.get("Links");
	HashMap<String, Object> properties = (HashMap<String, Object>) diagramm.get("Properties");

	setFinalIntegrationTime((Double) properties.get("finalIntegrationTime"));
	setIntegratorAbsoluteTolerance((Double) properties.get("integratorAbsoluteTolerance"));
	setIntegratorRelativeTolerance((Double) properties.get("integratorRelativeTolerance"));
	setToleranceOnTime((Double) properties.get("toleranceOnTime"));
	setMaxIntegrationTimeinterval((Double) properties.get("maxIntegrationTimeinterval"));
	setRealTimeScaling((Double) properties.get("realTimeScaling"));
	setSolver((Double) properties.get("solver"));
	setMaximumStepSize((Double) properties.get("maximumStepSize"));
	setContext( (String[]) properties.get("context"));

	List<BasicPort[]> linkPorts = (List<BasicPort[]>) allLinks.get("Ports");
	List<double[][]> linkPoints = (List<double[][]>) allLinks.get("Points");

	Object[] objs = new Object[allBlocks.size() + linkPorts.size()];
	getModel().beginUpdate();
	for (int i = 0; i < allBlocks.size(); ++i) {
		objs[i] = allBlocks.get(i);
		System.err.println("Load Blocks : " + (i+1) + "/" + allBlocks.size());
//	    addCell(allBlocks.get(i));
	}

	for (int i = 0; i < linkPorts.size(); ++i) {
	    BasicLink link = createLinkFromPorts(linkPorts.get(i)[0], linkPorts.get(i)[1]);
	    link.setGeometry(new mxGeometry(0,0,80,80));
	    link.setSource(linkPorts.get(i)[0]);
	    link.setTarget(linkPorts.get(i)[1]);
	    double[][] points = linkPoints.get(i);

	    if(points != null){
		for(int point = 0 ; point < points.length ; point++){
		    link.addPoint(points[point][0], points[point][1]);
		}
	    }
		System.err.println("Load Links : " + (i+1) + "/" + linkPorts.size());
	    objs[i + allBlocks.size()] = link;
	    //addCell(link);
	}
	
	addCells(objs);
	getModel().endUpdate();

	//this.setTitle(fileToLoad);
	//this.getParentTab().setName(fileToLoad);

	setTitle((String) properties.get("title"));
	//getParentTab().setName((String) properties.get("title"));

	// Clear all undo events in Undo Manager
	undoManager.reset();
    }

    /**
     * Read a diagram from an HDF5 file (ask for creation if the file does not exist) 
     * @param diagramFileName file to open
     */
    public void openDiagramFromFile(String diagramFileName) {

    	File theFile = new File(diagramFileName);

    	if (theFile.exists()) {

    		String fileToLoad = diagramFileName;
    		// Try to find file type
    		int dotPos = theFile.getName().lastIndexOf('.');
    		String extension = "";
    		if (dotPos > 0 && dotPos <= theFile.getName().length() - 2) {
    			extension = theFile.getName().substring(dotPos + 1);
    		}

    		if (extension.equals("cosf")) {
    			final File tempOutput;
    			try {
    				tempOutput = File.createTempFile("xcos",".h5");
    				String cmd = "exec(\"" + theFile.getAbsolutePath() + "\", -1);";
    				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
    				cmd += "xcosNotify(\"" +tempOutput.getAbsolutePath()+ "\");";
    				InterpreterManagement.requestScilabExec(cmd);
    				Thread launchMe = new Thread() {
    					public void run() {
    						Signal.wait(tempOutput.getAbsolutePath());
    						openDiagramFromFile(tempOutput.getAbsolutePath());
    					}
    				};
    				launchMe.start();
    				fileToLoad = tempOutput.getAbsolutePath();
    			} catch (IOException e) {
    				// TODO Auto-generated catch block
    				e.printStackTrace();
    			}
    		} else if (extension.equals("cos")) {
    			final File tempOutput;
    			try {
    				tempOutput = File.createTempFile("xcos",".h5");
    				String cmd = "load(\"" + theFile.getAbsolutePath() + "\");";
    				cmd += "export_to_hdf5(\"" + tempOutput.getAbsolutePath() + "\", \"scs_m\");";
    				cmd += "xcosNotify(\"" +tempOutput.getAbsolutePath()+ "\");";
    				InterpreterManagement.requestScilabExec(cmd);
    				Thread launchMe = new Thread() {
    					public void run() {
    						Signal.wait(tempOutput.getAbsolutePath());
    						openDiagramFromFile(tempOutput.getAbsolutePath());
    					}
    				};
    				launchMe.start();
    				fileToLoad = tempOutput.getAbsolutePath();
    			} catch (IOException e) {
    				// TODO Auto-generated catch block
    				e.printStackTrace();
    			}
    		} else if (extension.equals("xcos")) {
    			Document document = null;
    			try {
    				document = mxUtils.parse(mxUtils.readFile(theFile.getAbsolutePath()));
    			} catch (IOException e1) {
    				// TODO Auto-generated catch block
    				e1.printStackTrace();
    			}

    			XcosCodec codec = new XcosCodec(document);

    			if (getModel().getChildCount(getDefaultParent()) == 0) {
    				codec.decode(document.getDocumentElement(), this);
    				if (getModel().getChildCount(getDefaultParent()) == 0) {
    					XcosDialogs.couldNotLoadFile();
    				} else {
    					setTitle(theFile.getAbsolutePath());
    				}
    				setChildrenParentDiagram();
    			} else {
    				XcosDiagram xcosDiagram = Xcos.createEmptyDiagram();
    				codec.decode(document.getDocumentElement(), xcosDiagram);
    				if (xcosDiagram.getModel().getChildCount(xcosDiagram.getDefaultParent()) == 0) {
    					XcosDialogs.couldNotLoadFile();
    				} else {
    					setTitle(theFile.getAbsolutePath());
    				}
    				setChildrenParentDiagram(xcosDiagram);
    			}

    		} else if (extension.equals("h5")) {
    			openDiagram(BlockReader.readDiagramFromFile(fileToLoad));
    		} else {
    			XcosDialogs.couldNotLoadFile();
    		}

    	} else {
    		int choice = JOptionPane.showConfirmDialog(this.getAsComponent()
    				, String.format(XcosMessages.FILE_DOESNT_EXIST,theFile.getAbsolutePath()) );
    		if (choice  == 0) {
    			try {
    				FileWriter writer = new FileWriter(diagramFileName);
    				writer.write("");
    				writer.flush();
    				writer.close();

    				openDiagramFromFile(diagramFileName);
    			} catch (IOException ioexc) {
    				JOptionPane.showMessageDialog(this.getAsComponent() , ioexc);
    			}
    		}	

    	}
    }

    private void setChildrenParentDiagram(){
    	setChildrenParentDiagram(this);
    }

    private void setChildrenParentDiagram(XcosDiagram diagram){
    	for (int i = 0 ; i < diagram.getModel().getChildCount(diagram.getDefaultParent()) ; i++){
    		mxCell cell = (mxCell)diagram.getModel().getChildAt(diagram.getDefaultParent(), i);
    		if (cell instanceof SuperBlock){
    			System.err.println("superblock children update diagram");
    			SuperBlock block = (SuperBlock)cell;
    			XcosDiagram superDiagram = block.getParentDiagram();
    			setChildrenParentDiagram(superDiagram);
    			System.err.println("superblock children update diagram END");
    		}else if (diagram.getModel().getChildAt(diagram.getDefaultParent(), i) instanceof BasicBlock){
        		BasicBlock block = (BasicBlock)cell; 
    			block.setParentDiagram(diagram);
    		}
    	}
    }
/**
     * Returns the tooltip to be used for the given cell.
     */
    public String getToolTipForCell(Object cell)
    {
	if (cell instanceof BasicBlock) {
	    return ((BasicBlock) cell).getToolTipText();
	}
	return "";
    }

    public static void setBlockTextValue(int blockID, String[] blockValue, int iRows, int iCols){
    	AfficheBlock block = Xcos.getAfficheBlocks().get(blockID);

    	if(block == null){
    		System.err.println("block == null");
    		return;
    	}

    	String blockResult = "";
    	for(int i = 0 ; i < iRows ; i++){
    		for(int j = 0 ; j < iCols ; j++){
    			if(iCols != 0){
    				blockResult += "  ";
    			}
    			blockResult += blockValue[j * iRows + i];
    		}
    		blockResult += System.getProperty("line.separator");
    	}
    	
    	block.setValue(blockResult);
    	block.getParentDiagram().refresh();
    }
}

