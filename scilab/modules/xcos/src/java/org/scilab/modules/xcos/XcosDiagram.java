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
import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
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
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.hdf5.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.ShowParentAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.actions.XcosShortCut;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.SuperBlockDiagram;
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
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.ConfigXcosManager;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;

import com.mxgraph.io.mxCodec;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxPoint;
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
    private int debugLevel = 0;
    private String[] context = new String[]{""};
    private List doc = null;
    private String version = "scicos4.2";
    //private Window palette;
    private Tab viewPort;
    
    /*to manage splitLink*/
    private BasicLink splitLink = null;
    private BasicPort splitPort = null;
    private mxPoint dragPos = null;
    private boolean waitRelease = false;
    
    private CheckBoxMenuItem viewPortMenu;
    private CheckBoxMenuItem gridMenu;
    private SetContextAction action;
    
    protected mxIEventListener undoEnabler = new mxIEventListener()
    {
	public void invoke(Object source, mxEventObject evt) {
		if (getParentTab() != null) {
			((XcosTab)getParentTab()).setEnabledUndo(true);
		}
	}
    };

    public Object addEdge(Object edge, Object parent, Object source,
    		Object target, Integer index) {	
    	
    	// Command -> Control
    	if (source instanceof CommandPort) {
    		if (target instanceof ControlPort) {
    			return super.addEdge(new CommandControlLink(), parent, source, target, index);
    		}
    	}

    	// Control -> Command
    	// Switch source and target !
    	if (target instanceof CommandPort) {
    		if (source instanceof ControlPort) {
    			return super.addEdge(new CommandControlLink(), parent, target, source, index);
    		}
    	}

    	// ExplicitOutput -> ExplicitInput
    	if (source instanceof ExplicitOutputPort) {
    		if (target instanceof ExplicitInputPort) {
    			return super.addEdge(new ExplicitLink(), parent, source, target, index);
    		}
    	}
    	// ExplicitInput -> ExplicitOutput
    	// Switch source and target !
    	if (target instanceof ExplicitOutputPort) {
    		if (source instanceof ExplicitInputPort) {
    			return super.addEdge(new ExplicitLink(), parent, target, source, index);
    		}
    	}

    	// ImplicitOutput -> ImplicitInput
    	if (source instanceof ImplicitOutputPort) {
    		if (target instanceof ImplicitInputPort) {
    			return super.addEdge(new ImplicitLink(), parent, source, target, index);
    		}
    	}
    	// ImplicitInput -> ImplicitOutput
    	// Switch source and target !
    	if (target instanceof ImplicitOutputPort) {
    		if (source instanceof ImplicitInputPort) {
    			return super.addEdge(new ImplicitLink(), parent, target, source, index);
    		}
    	}

    	// ImplicitInput -> ImplicitInput
    	if (source instanceof ImplicitInputPort) {
    		if (target instanceof ImplicitInputPort) {
    			return super.addEdge(new ImplicitLink(), parent, source, target, index);
    		}
    	}
    	// ImplicitOutputPort -> ImplicitOutput
    	// Switch source and target !
    	if (target instanceof ImplicitOutputPort) {
    		if (source instanceof ImplicitOutputPort) {
    			return super.addEdge(new ImplicitLink(), parent, target, source, index);
    		}
    	}
    	
    	// ExplicitLink -> ExplicitInputPort
    	if (source instanceof ExplicitLink) {
    		if (target instanceof ExplicitInputPort) {
    			return addSplitEdge((BasicLink) source, (BasicPort) target);
    		}
    	}
    	// ExplicitOutput -> ExpliciLink
    	// Switch source and target !
    	if (target instanceof ExplicitLink) {
    		if (source instanceof ExplicitInputPort) {
    			waitRelease = true;
    			splitLink = (BasicLink) target;
    			splitPort = (BasicPort) source;
    			return null;
    			//return addSplitEdge((BasicLink) target, (BasicPort)source);
    		}
    	}

    	// ImplicitLink -> ImplicitInputPort
    	if (source instanceof ImplicitLink) {
    		if (target instanceof ImplicitInputPort) {
    			return addSplitEdge((BasicLink) source, (BasicPort) target);
    		}
    	}
    	// ImplicitInputPort -> ImplicitLink
    	// Switch source and target !
    	if (target instanceof ImplicitLink) {
    		if (source instanceof ImplicitInputPort) {
    			waitRelease = true;
    			splitLink = (BasicLink) target;
    			splitPort = (BasicPort) source;
    			return null;
    			//return addSplitEdge((BasicLink) target, (BasicPort)source);
    		}
    	}
    	
    	// ImplicitLink -> ImplicitOutputPort
    	if (source instanceof ImplicitLink) {
    		if (target instanceof ImplicitOutputPort) {
    			return addSplitEdge((BasicLink) source, (BasicPort) target);
    		}
    	}
    	// ImplicitOutputPort -> ImplicitLink
    	// Switch source and target !
    	if (target instanceof ImplicitLink) {
    		if (source instanceof ImplicitOutputPort) {
    			waitRelease = true;
    			splitLink = (BasicLink) target;
    			splitPort = (BasicPort) source;
    			return null;
    			//return addSplitEdge((BasicLink) target, (BasicPort)source);
    		}
    	}

    	// CommandControlLink -> ControlPort
    	if (source instanceof CommandControlLink) {
    		if (target instanceof ControlPort) {
    			return addSplitEdge((BasicLink) source, (BasicPort) target);
    		}
    	}
    	// ControlPort -> CommandControlLink
    	// Switch source and target !
    	if (target instanceof CommandControlLink) {
    		if (source instanceof ControlPort) {
    			waitRelease = true;
    			splitLink = (BasicLink) target;
    			splitPort = (BasicPort) source;
    			return null;
    			//return addSplitEdge((BasicLink) target, (BasicPort)source);
    		}
    	}

    	return null;
    }

    private Object addSplitEdge(BasicLink link, BasicPort target) {
    	BasicPort linkSource =  (BasicPort) link.getSource();
    	BasicPort linkTarget =  (BasicPort) link.getTarget();

    	if (dragPos == null) {
    		dragPos = new mxPoint();

            //check splitPosition values
            double srcX = linkSource.getParent().getGeometry().getX() + linkSource.getGeometry().getCenterX();
            double tgtX = linkTarget.getParent().getGeometry().getX() + linkTarget.getGeometry().getCenterX();
            double srcY = linkSource.getParent().getGeometry().getY() + linkSource.getGeometry().getCenterY();
            double tgtY = linkTarget.getParent().getGeometry().getY() + linkTarget.getGeometry().getCenterY();

            double offsetX = (tgtX - srcX) / 2;
            double offsetY = (tgtY - srcY) / 2;
            dragPos.setX(srcX + offsetX);
            dragPos.setY(srcY + offsetY);
    	}
   	
    	SplitBlock splitBlock = new SplitBlock("SPLIT_f", linkSource, linkTarget, (BasicPort) target);
    	splitBlock.setStyle("SPLIT_f");
    	mxGeometry geom = new mxGeometry();
    	geom.setX(dragPos.getX() - 3); //-3 for splitBlock size
    	geom.setY(dragPos.getY() - 3); //-3 for splitBlock size
    	splitBlock.setGeometry(geom);
    	addCell(splitBlock);
    	
    	
    	//Update old link
    	
    	//get breaking segment
    	int pos = link.findNearestSegment(dragPos);

    	//save points after breaking point
    	mxPoint[] saveStartPoints = link.getPoints(pos, true);
    	mxPoint[] saveEndPoints = link.getPoints(pos, false);
    	
    	
    	//disable events
    	getModel().beginUpdate();
    	getModel().remove(link);
    	getModel().endUpdate();

    	BasicLink newLink1 = BasicLink.createLinkFromPorts(linkSource, splitBlock.getIn());
    	newLink1.setGeometry(new mxGeometry(0,0,80,80));
    	newLink1.setSource(linkSource);
    	newLink1.setTarget(splitBlock.getIn());

    	//add points after breaking point in the new link
    	if (saveStartPoints != null) {
    		for (int i = 0; i < saveStartPoints.length; i++) {
    			newLink1.addPoint(saveStartPoints[i].getX(), saveStartPoints[i].getY());
    		}
       	}
    	addCell(newLink1);
    	
    	BasicLink newLink2 = BasicLink.createLinkFromPorts(splitBlock.getOut1(), linkTarget);
    	newLink2.setGeometry(new mxGeometry(0,0,80,80));
    	newLink2.setSource(splitBlock.getOut1());
    	newLink2.setTarget(linkTarget);
    	//add points after breaking point in the new link
    	if (saveEndPoints != null) {
    		for (int i = 0; i < saveEndPoints.length; i++) {
    			newLink2.addPoint(saveEndPoints[i].getX(), saveEndPoints[i].getY());
    		}
       	}
    	addCell(newLink2);
    	
    	BasicLink newLink3 = BasicLink.createLinkFromPorts(splitBlock.getOut2(), (BasicPort) target);
    	newLink3.setGeometry(new mxGeometry(0,0,80,80));
    	newLink3.setSource(splitBlock.getOut2());
    	newLink3.setTarget((mxCell) target);
    	addCell(newLink3);

    	dragPos = null;
		refresh();
    	return splitBlock;
    }
    
    public XcosDiagram() {
	super();
	getModel().addListener(mxEvent.UNDO, undoEnabler);
	getView().addListener(mxEvent.UNDO, undoEnabler);
	keyboardHandler = new XcosShortCut(this);
	mxCodec codec = new mxCodec();

	try {
	    File uri = new File(System.getenv("SCI"));
	    String xml = mxUtils.readFile(System.getenv("SCI")+ "/modules/xcos/etc/Xcos-style.xml");
	    xml = xml.replaceAll("\\$SCILAB", uri.toURI().toURL().toString());
	    Document document = mxUtils.parse(xml);
	    codec.decode(document.getDocumentElement(), getStylesheet());
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

	//
	setCloneInvalidEdges(true);

	// Override isCellEditable to filter what the user can edit
	setCellsEditable(true);
	// This enable stop editing cells when pressing Enter.
	getAsComponent().setEnterStopsCellEditing(false);

	setConnectableEdges(true);
	getAsComponent().setTolerance(1);
	
	getAsComponent().getViewport().setOpaque(false);
	getAsComponent().setBackground(Color.WHITE);

	mxMultiplicity[] multiplicities = new mxMultiplicity[9];

	// Input data port
	multiplicities[0] = new PortCheck(new ExplicitInputPort(), new mxCell[] {new ExplicitOutputPort(), new ExplicitLink()}, XcosMessages.LINK_ERROR_EXPLICIT_IN);
	multiplicities[1] = new PortCheck(new ImplicitInputPort(), new mxCell[] {new ImplicitOutputPort(), new ImplicitInputPort(), new ImplicitLink()}, XcosMessages.LINK_ERROR_IMPLICIT_IN);

	//Output data port
	multiplicities[2] = new PortCheck(new ExplicitOutputPort(), new mxCell[] {new ExplicitInputPort()}, XcosMessages.LINK_ERROR_EXPLICIT_OUT);
	multiplicities[3] = new PortCheck(new ImplicitOutputPort(), new mxCell[] {new ImplicitInputPort(), new ImplicitOutputPort()}, XcosMessages.LINK_ERROR_IMPLICIT_OUT);

	//Control port
	multiplicities[4] = new PortCheck(new ControlPort(), new mxCell[] {new CommandPort(), new CommandControlLink()}, XcosMessages.LINK_ERROR_EVENT_IN);

	//Command port
	multiplicities[5] = new PortCheck(new CommandPort(), new mxCell[] {new ControlPort()}, XcosMessages.LINK_ERROR_EVENT_OUT);

	//ExplicitLink connections
	multiplicities[6] = new PortCheck(new ExplicitLink(), new mxCell[] {new ExplicitInputPort()}, XcosMessages.LINK_ERROR_EVENT_OUT);

	//ImplicitLink connections
	multiplicities[7] = new PortCheck(new ImplicitLink(), new mxCell[] {new ImplicitInputPort(), new ImplicitOutputPort()}, XcosMessages.LINK_ERROR_EVENT_OUT);

	//CommandControlLink connections
	multiplicities[8] = new PortCheck(new CommandControlLink(), new mxCell[] {new ControlPort()}, XcosMessages.LINK_ERROR_EVENT_OUT);

	setMultiplicities(multiplicities);
	
	// Add a listener to track when model is changed
	getModel().addListener(XcosEvent.CHANGE, new ModelTracker());
	
	setGridEnabled(true);
	getAsComponent().setGridVisible(true);
	
	((mxCell) getDefaultParent()).setId((new UID()).toString());
	((mxCell) getModel().getRoot()).setId((new UID()).toString());
    }

    /**
     * Install all needed Listeners.
     */
    public void installListeners() {

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

	// Track when superblock ask a parent refresh.
	addListener(XcosEvent.SUPER_BLOCK_UPDATED, new SuperBlockUpdateTracker()); 

	// Track when cells are added.
	addListener(XcosEvent.CELLS_ADDED, new CellAddedTracker(this)); 

	// Track when cells are deleted.
	addListener(XcosEvent.CELLS_REMOVED, new CellRemovedTracker(this)); 
		
	// Track when resizing a cell.
	addListener(XcosEvent.CELLS_RESIZED, new CellResizedTracker());

	// Track when we have to force a Block to reshape
	addListener(XcosEvent.FORCE_CELL_RESHAPE, new ForceCellReshapeTracker());
	
	// Track when we have to force a Block value
	addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, new ForceCellValueUpdate());
	
	getAsComponent().getGraphControl().addMouseListener(new XcosMouseListener(this));

	addListener(XcosEvent.ADD_PORTS, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getModel().beginUpdate();
		refresh();
		BasicBlock updatedBlock = (BasicBlock) evt.getArgAt(0);
		BlockPositioning.updateBlockView(updatedBlock);
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
	    List changes = (List) evt.getArgAt(0);
	    List<Object> objects = new ArrayList<Object>();
	    getModel().beginUpdate();
	    for (int i = 0; i < changes.size(); ++i) {
		if (changes.get(i) instanceof mxChildChange) {
		    if (((mxChildChange) changes.get(i)).getChild() instanceof SplitBlock) {
			continue;
		    }

		    if (((mxChildChange) changes.get(i)).getChild() instanceof BasicBlock) {
			BasicBlock currentCell = (BasicBlock) ((mxChildChange) changes.get(i)).getChild();
			objects.add(currentCell);
		    }
		}
	    }
	    if (!objects.isEmpty()) {
		Object[] firedCells = new Object[objects.size()];
		for (int j = 0;  j < objects.size(); ++j) {
		    firedCells[j] = objects.get(j);
		}
		//fireEvent(XcosEvent.FORCE_CELL_RESHAPE, new mxEventObject(new Object[] {firedCells}));
		fireEvent(XcosEvent.FORCE_CELL_VALUE_UPDATE, new mxEventObject(new Object[] {firedCells}));
	    }
	    getModel().endUpdate();
	}
    }
    /**
     * ForceCellValueUpdate
     * Called when we want a block content to update.
     */
    private class ForceCellValueUpdate implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getArgs()[0];

	    getModel().beginUpdate();

	    for (int i = 0; i < cells.length; ++i) {
		
		Object cell = cells[i];
		
		if (cell instanceof BasicBlock) {
		    if (getCellStyle(cell).get("displayedLabel") != null) {
			((mxCell) cell).setValue("<html><body> " + getCellStyle(cell).get("displayedLabel") + " </body></html>");
		    }

		    mxRectangle preferedSize = getPreferredSizeForCell(cell);
		    mxGeometry cellSize = ((mxCell) cell).getGeometry();

		    ((mxCell) cell).setGeometry(new mxGeometry(cellSize.getX(), cellSize.getY(),
			    Math.max(preferedSize.getWidth(), cellSize.getWidth()),
			    Math.max(preferedSize.getHeight(), cellSize.getHeight())));
		    cellsResized(new Object[] {cell}, new mxRectangle[]{((mxCell) cell).getGeometry()});
		}
	    }
	    getModel().endUpdate();
	    refresh();
	}
    }
    
    /**
     *  ForceCellReshapeTracker
     *  Called when we want a Block to reshape for it's ports positions.
     */
    private class ForceCellReshapeTracker implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells =  (Object[]) evt.getArgs()[0];
	    getModel().beginUpdate();
	    for (int i = 0; i <  cells.length; ++i) {
		Object cell = cells[i];
		if (cell instanceof BasicBlock) {
		    BlockPositioning.updateBlockView((BasicBlock) cell);
		}
	    }
	    getModel().endUpdate();
	}
    }
    
    /**
     *  SuperBlockUpdateTracker
     *  Called when adding some port in a SuperBlock diagram
     *  to update current sub-diagram (i.e SuperBlock) representation.
     */
    private class SuperBlockUpdateTracker implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    assert evt.getArgs()[0] instanceof SuperBlock;
	    SuperBlock updatedBlock = (SuperBlock) evt.getArgs()[0];
	    updatedBlock.setRealParameters(BlockWriter
		    .convertDiagramToMList(updatedBlock.getChild()));
	    if (updatedBlock.getParentDiagram() instanceof SuperBlockDiagram) {
		SuperBlock parentBlock = ((SuperBlockDiagram) updatedBlock
			.getParentDiagram()).getContainer();
		parentBlock.getParentDiagram().fireEvent(
			XcosEvent.SUPER_BLOCK_UPDATED,
			new mxEventObject(new Object[] { parentBlock }));
	    }
	    BlockPositioning.updateBlockView(updatedBlock);
	    refresh();
	}
    }

    /**
     * CellAddedTracker
     * Called when mxEvents.CELLS_ADDED is fired.
     */
    private class CellAddedTracker implements mxIEventListener {
    	private XcosDiagram diagram = null;

    	public CellAddedTracker(XcosDiagram diagram) {
    		this.diagram = diagram;
    	}

    	public void invoke(Object source, mxEventObject evt) {
    		Object[] cells = (Object[]) evt.getArgs()[0];
    		
    		diagram.getModel().beginUpdate();
    		for (int i = 0; i < cells.length; ++i) {

//    			((mxCell) cells[i]).setId((new UID()).toString());
//    			System.err.println("AddCell setId : " + ((mxCell) cells[i]).getId());

				if (cells[i] instanceof BasicBlock) {
					// Store all AfficheBlocks in a dedicated HasMap
					if (cells[i] instanceof AfficheBlock) {
						AfficheBlock affich = (AfficheBlock) cells[i];
						XcosTab.getAfficheBlocks().put(affich.getHashCode(), affich);
					}
					// Update parent on cell addition
					((BasicBlock) cells[i]).setParentDiagram(diagram);
				}
    		}
    		//fireEvent(XcosEvent.FORCE_CELL_VALUE_UPDATE, new mxEventObject(new Object[] {cells}));
    		diagram.getModel().endUpdate();
    	}
    }

    /**
     * CellRemovedTracker
     * Called when mxEvents.CELLS_REMOVED is fired.
     */
    private class CellRemovedTracker implements mxIEventListener {
    	private XcosDiagram diagram = null;

    	public CellRemovedTracker(XcosDiagram diagram) {
    		this.diagram = diagram;
    	}

    	public void invoke(Object source, mxEventObject evt) {
    		Object[] cells = (Object[]) evt.getArgs()[0];
    		for (int i = 0; i < cells.length; i++) {
    			if (cells[i] instanceof BasicLink) {
    				BasicLink link = (BasicLink) cells[i];
    				removeLink(link);
    			}
    		}
    	}
    }

    private void removeLink(BasicLink link) {
    	BasicPort portSource = (BasicPort) link.getSource();
    	BasicPort portTarget = (BasicPort) link.getTarget();

    	SplitBlock split = null;
    	BasicPort saveSource = null;
    	BasicPort saveTarget = null;

    	if (portSource == null) { return; }
    	if (portTarget == null) { return; }

    	//remove input link
    	if (portTarget.getParent() instanceof SplitBlock) {
    		split = (SplitBlock) portTarget.getParent();
    		
    		Object[] outLinks = getAllEdges(new Object[] {split.getOut1(), split.getOut2()});
    		for (int i = 0; i < outLinks.length; i++) {
    			BasicLink outLink = (BasicLink) outLinks[i];
    			if (outLink.getTarget().getParent() instanceof SplitBlock) {
    				removeCells(new Object[]{outLink});
    			}
    		}
    	}
    	
    	//Finally delete split and old associated links
    	if (split != null) {
    		removeCells(new Object[]{split});
    	}

    	//reset variables
    	split = null;
    	saveSource = null;
    	saveTarget = null;

    	if (portSource.getParent() instanceof SplitBlock) {
    		split = (SplitBlock) portSource.getParent();

    		//remove out1, so link between in.source and out2.target
    		if (split.getOut1() == portSource) {
    			//save source and target ports 
    			saveSource = getOppositePort(split.getIn());
    			saveTarget = getOppositePort(split.getOut2());
    		} else if (split.getOut2() == portSource) {
    			//save source and target ports 
    			saveSource = getOppositePort(split.getIn());
    			saveTarget = getOppositePort(split.getOut1());
    		}
    	}

    	if (saveSource != null && saveTarget != null) {
    		//create new link
    		BasicLink newLink = BasicLink.createLinkFromPorts(saveSource, saveTarget);
    		newLink.setGeometry(new mxGeometry(0,0,80,80));

    		Object[] saveLinks = getAllEdges(new Object[]{saveSource, saveTarget});
    		for (int k = 0; k < saveLinks.length; k++) {
    			mxPoint[] savePts = ((BasicLink) saveLinks[k]).getPoints(0, false);
    			if (savePts != null) {
    				for (int j = 0; j < savePts.length; j++) {
    					newLink.addPoint(savePts[j].getX(), savePts[j].getY());
    				}
    			}
    		}

    		newLink.setSource(saveSource);
    		newLink.setTarget(saveTarget);
    		addCell(newLink);

    		//unlink split and delete unlinked links
    	}

    	if (split != null) {
    		split.unlinkAndClean();
    		removeCells(new Object[]{split});
    	}
    }

    private BasicPort getOppositePort(BasicPort source) {
    	Object[] objs = getAllEdges(new Object[]{source});
    	if (objs.length == 0 || objs.length > 1) {
    		return null;
    	}
    	
    	BasicLink link = (BasicLink) objs[0];
    	if (link.getSource() == source) {
    		return (BasicPort) link.getTarget();
    	} else {
    		return (BasicPort) link.getSource();
    	}
    }
    /**
     * CellResizedTracker
     * Called when mxEvents.CELLS_RESIZED is fired. 
     */
    private class CellResizedTracker implements mxIEventListener {
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getArgs()[0];
	    getModel().beginUpdate();
	    for (int i = 0; i < cells.length; ++i) {
		if (cells[i] instanceof BasicBlock) {
		    BlockPositioning.updateBlockView((BasicBlock) cells[i]);
		}
	    }
	    getModel().endUpdate();
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
		    block.openBlockSettings(buildEntireContext());
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
	    	System.err.println("[DEBUG] Click at position : " + arg0.getX() + " , " + arg0.getY());
	    	if (cell == null) {
	    	    System.err.println("[DEBUG] Click on diagram");
	    	    System.err.println("Default Parent ID : " + ((mxCell) getDefaultParent()).getId());
	    	    System.err.println("Model root ID : " + ((mxCell) getModel().getRoot()).getId());
	    	    System.err.println("getParentWindow : " + getParentTab().getParentWindow());
	    	} else {
	    	    System.err.println("[DEBUG] Click on : " + cell);
	    	    System.err.println("[DEBUG] Style : " + ((mxCell) cell).getStyle());
	    	    System.err.println("[DEBUG] NbEdges : " + ((mxCell) cell).getEdgeCount());
	    	    System.err.println("[DEBUG] NbChildren : " + ((mxCell) cell).getChildCount());
	    	    for (int i = 0; i < ((mxCell) cell).getChildCount(); i++) {
	    		System.err.println("[DEBUG] Child NbEdges : " + ((mxCell) cell).getChildAt(i).getEdgeCount());
	    	    }
	    	    
	    	    if(cell instanceof BasicLink) {
	    		System.err.println("[DEBUG] Link Points : " + ((BasicLink) cell).getPointCount());
	    	    }
	    	}
	    	
	    }

	    // Context menu
	    if ((arg0.getClickCount() == 1 && SwingUtilities.isRightMouseButton(arg0))
	    		|| arg0.isPopupTrigger()
	    		|| isMacOsPopupTrigger(arg0)) {

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
		    
		    if(diagram instanceof SuperBlockDiagram) {
			/*---*/
			menu.getAsSimpleContextMenu().addSeparator();
			/*---*/
			menu.add(ShowParentAction.createMenu(diagram));
		    }
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
		    }
		    if (cell instanceof BasicLink) {
			BasicLink link = (BasicLink) cell;
			link.openContextMenu((ScilabGraph) getAsComponent().getGraph());
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
	}

	public void mouseReleased(MouseEvent arg0) {
		if (waitRelease) {
			dragPos = new mxPoint(arg0.getX(), arg0.getY());
			waitRelease = false;
			addSplitEdge(splitLink, splitPort);
		} else {
			dragPos = null;
		}
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

	public boolean isCellClonable(Object cell) {
		return true;
	}
	
	public boolean isCellMovable(Object cell) {
		if (cell instanceof BasicPort) {
			return false;
		}

		boolean movable = false;
		Object[] cells =  this.getSelectionCells();

		//don't move if selection is only links
		for (int i = 0; i < cells.length; i++) {
			if (!(cells[i] instanceof BasicLink)) {
				movable = true;
				break;
			}
		}

		return movable && super.isCellMovable(cell);
    }

    public boolean isCellResizable(Object cell) {
    	if (cell instanceof SplitBlock) {
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

    public boolean isAutoSizeCell(Object cell) {
    	return (cell instanceof AfficheBlock) || super.isAutoSizeCell(cell);
    }


    public void dumpToHdf5File(String fileName) {
	if (fileName == null) {
	    FileChooser fc = ScilabFileChooser.createFileChooser();
	    fc.setInitialDirectory(getSavedFile());
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		return;
	    }
	    fileName = fc.getSelection()[0];
	    System.out.println("Saving to file : {" + fileName + "}");
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
    	setGridEnabled(status);
    	getAsComponent().setGridVisible(status);
    	getAsComponent().repaint();

    	// (Un)Check the corresponding menu
    	gridMenu.setChecked(status);
    }

//	public mxRectangle getCellBounds(Object cell, boolean includeEdges,
//			boolean includeDescendants, boolean boundingBox) {
//    	//mxRectangle rect = super.getCellBounds(cell, includeEdges, includeDescendants, boundingBox);
//		mxRectangle rect = super.getCellBounds(cell, includeEdges, false, boundingBox);
//		return rect;
//	}

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
	closeDiagram(false);
    }
    
    /**
     * Close Xcos instance including all tabs
     */
    public void closeDiagram(boolean fromScilab) {

	boolean wantToClose = true;

	if(canClose() == false) {
	    setVisible(false);
	    return;
	}
	
	if (isModified()) {
	    // The diagram has been modified
	    // Ask the user want he want to do !
	    
	    AnswerOption answer; 
	    if(fromScilab == true) {
		answer = ScilabModalDialog.show(getParentTab(), XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS, 
			IconType.QUESTION_ICON, ButtonType.YES_NO);
	    } else {
		answer = ScilabModalDialog.show(getParentTab(), XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS, 
			IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL);
	    }

	    switch(answer) {
	    case YES_OPTION :
	    	// Save the diagram
	    	if (!saveDiagram()) {
	    		//if save is canceled, cancel close windows
	    		wantToClose = false;
	    	}
		break;
	    case NO_OPTION :
		break;
	    case CANCEL_OPTION :
		// The user cancels
		wantToClose = false;
		break;
	    }
	}

	if (wantToClose) {
	    if(getParentTab() != null) {
		ScilabWindow xcosWindow = (ScilabWindow) UIElementMapper.getCorrespondingUIElement(getParentTab().getParentWindowId());
		xcosWindow.removeTab(getParentTab());
		viewPort.close();
	    }
	    XcosTab.closeDiagram(this);
	    setOpened(false);
	}
    }

    public boolean saveDiagram() {
	boolean isSuccess = false;
	if (getSavedFile() == null) {
	    isSuccess = saveDiagramAs(null);
	} else {
	    isSuccess = saveDiagramAs(getSavedFile());
	}

	if (isSuccess) {
	    setModified(false);
	}

	return isSuccess;
    }

    public boolean saveDiagramAs(String fileName) {

	boolean isSuccess = false;
	info(XcosMessages.SAVING_DIAGRAM);
	this.getParentTab().getInfoBar().draw();
	if (fileName == null) {
	    // Choose a filename
	    SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
	    fc.setTitle(XcosMessages.SAVE_AS);
	    fc.setUiDialogType(JFileChooser.SAVE_DIALOG);
	    fc.setMultipleSelection(false);
	    if (this.getSavedFile() != null) {
		fc.setSelectedFile(new File(this.getSavedFile()));
	    }
	    XcosFileType defaultFileType = XcosFileType.getDefault();
	    SciFileFilter xcosFilter = new SciFileFilter("*." + defaultFileType.getExtension(), defaultFileType.getDescription(), 0);
	    fc.addChoosableFileFilter(xcosFilter);
	    fc.setFileFilter(xcosFilter);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		info(XcosMessages.EMPTY_INFO);
		return isSuccess;
	    }
	    fileName = fc.getSelection()[0];
	}
	/* Extension checks */
	File file = new File(fileName);
	if(!file.exists()) {
	    String extension = fileName.substring(fileName.lastIndexOf('.') + 1);

	    if (extension.equals(fileName)) {
		/* No extension given --> .xcos added */
		fileName += ".xcos";
	    }
	}

	XcosCodec codec = new XcosCodec();
	String xml = mxUtils.getXml(codec.encode(this));

	try {
	    mxUtils.writeFile(xml, fileName);
	    isSuccess = true;
	    resetUndoCounter();
	} catch (IOException e1) {
	    e1.printStackTrace();
	    isSuccess = false;
	}

	if (isSuccess) {
	    this.setSavedFile(fileName);
	    File theFile = new File(fileName);
	    setTitle(theFile.getName().substring(0, theFile.getName().lastIndexOf('.')));
	    ConfigXcosManager.saveToRecentOpenedFiles(fileName);
	    setModified(false);
	} else {
	    XcosDialogs.couldNotSaveFile(this);
	}
	info(XcosMessages.EMPTY_INFO);
	return isSuccess;
    }

    public void setTitle(String title) {
	super.setTitle(title);
	updateTabTitle();
    }

    public void updateTabTitle() {
	String tabTitle = !isModified() ? getTitle() : "* " + getTitle();
	if (getParentTab() != null) {
	    getParentTab().setName(tabTitle);
	    getParentTab().draw();
	}
    }

    public String[] buildEntireContext() {
	return getContext();
    }
    
    public void setContext(String[] context) {
	this.context = context;
    }

    public String[] getContext() {
	return context;
    }

    public String getVersion() {
	return version;
    }
    
    public int getDebugLevel() {
    	return debugLevel;
    }
    
    public void setDebugLevel(int debugLevel) {
    	this.debugLevel = debugLevel;
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
		XcosDiagram xcosDiagram = Xcos.createEmptyDiagram();
		xcosDiagram.loadDiagram(diagramm);
		setChildrenParentDiagram(xcosDiagram);
	    }
	} else {
	    XcosDialogs.couldNotLoadFile(this);
	}
    }

    /**
     * Load a Diagramm structure into current window.
     * 
     * @param diagramm
     */
    public void loadDiagram(HashMap<String, Object> diagramm) {
	List<BasicBlock> allBlocks = (List) diagramm.get("Blocks");
	List<TextBlock> allTextBlocks = (List) diagramm.get("TextBlocks");
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
	setContext((String[]) properties.get("context"));

	List<BasicPort[]> linkPorts = (List<BasicPort[]>) allLinks.get("Ports");
	List<double[][]> linkPoints = (List<double[][]>) allLinks.get("Points");

	Object[] objs = new Object[allBlocks.size() + linkPorts.size() + allTextBlocks.size()];
	getModel().beginUpdate();
	for (int i = 0; i < allBlocks.size(); ++i) {
		objs[i] = allBlocks.get(i);
	}

	for (int i = 0; i < linkPorts.size(); ++i) {
	    BasicLink link = BasicLink.createLinkFromPorts(linkPorts.get(i)[0], linkPorts.get(i)[1]);
	    link.setGeometry(new mxGeometry(0,0,80,80));
	    link.setSource(linkPorts.get(i)[0]);
	    link.setTarget(linkPorts.get(i)[1]);
	    double[][] points = linkPoints.get(i);

	    if (points != null) {
		for (int point = 0; point < points.length; point++) {
		    link.addPoint(points[point][0], points[point][1]);
		}
	    }
	    objs[i + allBlocks.size()] = link;
	}
	
	for (int i = 0; i < allTextBlocks.size(); ++i) {
		objs[i + allBlocks.size() + linkPorts.size() ] = allTextBlocks.get(i);
	}
	
	addCells(objs);
	getModel().endUpdate();

	//this.setTitle(fileToLoad);
	//this.getParentTab().setName(fileToLoad);

	setTitle((String) properties.get("title"));
	//getParentTab().setName((String) properties.get("title"));

	// Clear all undo events in Undo Manager
	undoManager.reset();
	setModified(false);
    }

    /**
     * Read a diagram from an HDF5 file (ask for creation if the file does not exist) 
     * @param diagramFileName file to open
     */
    public void openDiagramFromFile(String diagramFileName) {
	if (XcosTab.focusOnExistingFile(diagramFileName) == false) {
	    File theFile = new File(diagramFileName);
	    info(XcosMessages.LOADING_DIAGRAM);

	    if (theFile.exists()) {
		transformAndLoadFile(theFile);
	    } else {
		AnswerOption answer = ScilabModalDialog.show(getParentTab(), String.format(
			XcosMessages.FILE_DOESNT_EXIST, theFile.getAbsolutePath()),
			XcosMessages.XCOS, IconType.QUESTION_ICON,
			ButtonType.YES_NO);

		if (answer == AnswerOption.YES_OPTION) {
		    try {
			FileWriter writer = new FileWriter(diagramFileName);
			writer.write("");
			writer.flush();
			writer.close();
			setSavedFile(diagramFileName);
			setTitle(theFile.getName().substring(0,
				theFile.getName().lastIndexOf('.')));
		    } catch (IOException ioexc) {
			JOptionPane.showMessageDialog(this.getAsComponent(), ioexc);
		    }
		}
	    }
	    // TODO
	    //open all SuperBlocks to assign a UID

	    this.resetUndoManager();
	    info(XcosMessages.EMPTY_INFO);
	}
    }
    
    /**
     * Load a file with different method depending on it extension 
     * @param theFile File to load
     */
	private void transformAndLoadFile(File theFile) {
		final File fileToLoad = theFile;
		final XcosFileType filetype = XcosFileType.findFileType(fileToLoad);
		
		
		switch (filetype) {
		case COSF:
		case COS:
			Thread transformAction = new Thread() {
				public void run() {
					File newFile;
					newFile = filetype.exportToHdf5(fileToLoad);
					transformAndLoadFile(newFile);
				}
			};
			transformAction.start();
			break;

		case XCOS:
			Document document = null;
			try {
				document = mxUtils.parse(mxUtils.readFile(theFile.getAbsolutePath()));
			} catch (IOException e1) {
				e1.printStackTrace();
			}

			XcosCodec codec = new XcosCodec(document);

			if (getModel().getChildCount(getDefaultParent()) == 0) {
				codec.decode(document.getDocumentElement(), this);
				setModified(false);
				setSavedFile(theFile.getAbsolutePath());
				setTitle(theFile.getName().substring(0,	theFile.getName().lastIndexOf('.')));
				setChildrenParentDiagram();
			} else {
				XcosDiagram xcosDiagram = Xcos.createEmptyDiagram();
				codec.decode(document.getDocumentElement(), xcosDiagram);
				setSavedFile(theFile.getAbsolutePath());
				setTitle(theFile.getName().substring(0,	theFile.getName().lastIndexOf('.')));
				setChildrenParentDiagram(xcosDiagram);
			}
			generateUID();
			setModified(false);
			break;

		case HDF5:
			openDiagram(BlockReader.readDiagramFromFile(fileToLoad.getAbsolutePath()));
			fileToLoad.delete();
			generateUID();
			setModified(false);
			break;

		default:
			XcosDialogs.couldNotLoadFile(this);
        		break;
		}
	}

	public void generateUID() {
	    for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
		if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
		    BasicBlock block = (BasicBlock)getModel().getChildAt(getDefaultParent(), i);
		    if(block.getRealParameters() instanceof ScilabMList) {
			//we have a hidden SuperBlock, create a real one
			SuperBlock newSP = (SuperBlock)BasicBlock.createBlock("SUPER_f");
			newSP.setRealParameters(block.getRealParameters());
			newSP.createChildDiagram(true);
			newSP.setParentDiagram(this);
			block.setRealParameters(BlockWriter.convertDiagramToMList(newSP.getChild()));
		    } else if(block.getId() == null || block.getId().compareTo("") == 0) {
			block.setId();
		    }
		}
	    }
	}
	
    /**
     * Update all the children of the current graph.
     */
    public void setChildrenParentDiagram() {
    	setChildrenParentDiagram(this);
    }

    /**
     * For each block in the argument, call its setParentDiagram method
     * @param diagram The new parent of the blocks.
     */
    private void setChildrenParentDiagram(XcosDiagram diagram) {
    	for (int i = 0; i < diagram.getModel().getChildCount(diagram.getDefaultParent()); i++) {
    		mxCell cell = (mxCell) diagram.getModel().getChildAt(diagram.getDefaultParent(), i);
    		if (cell instanceof BasicBlock) {
    			BasicBlock block = (BasicBlock) cell;
    			block.setParentDiagram(diagram);
    			if (block instanceof AfficheBlock) {
    				AfficheBlock affich = (AfficheBlock) block;
    				XcosTab.getAfficheBlocks().put(affich.getHashCode(), affich);
    			}
    		}
    	}
    }
    
    /**
     * Getting the root diagram of a decomposed diagram
     * @return Root parent of the whole parent
     */
    public XcosDiagram getRootDiagram() {
	XcosDiagram rootGraph = this;
	while (rootGraph instanceof SuperBlockDiagram) {
	    rootGraph = ((SuperBlockDiagram) rootGraph).getContainer().getParentDiagram();
	}
	return rootGraph;
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

    /**
     * Set any text to an Afficheblock specified by its ID.
     * @param blockID ID of the AfficheBlock to be modified.
     * @param blockValue Content to be apply to the block.
     * @param iRows Number of Row in the blockValue.
     * @param iCols Number of Collumns in the blockValue.
     */
    public static void setBlockTextValue(int blockID, String[] blockValue, int iRows, int iCols) {

       	AfficheBlock block = XcosTab.getAfficheBlocks().get(blockID);
    	if (block == null) {
    		System.err.println("block == null");
    		return;
    	}

    	String blockResult = "";
    	for (int i = 0; i < iRows; i++) {
    		for (int j = 0; j < iCols; j++) {
    			if (iCols != 0) {
    				blockResult += "  ";
    			}
    			blockResult += blockValue[j * iRows + i];
    		}
    		blockResult += System.getProperty("line.separator");
    	}
    	
    	block.setValue(blockResult);
    	block.getParentDiagram().refresh();
    }
    
    
    /**
     * Display the message in info bar.
     * @param message Informations
     */
    public void info(String message) {
	final String localMessage = message;
	if (getParentTab() != null && getParentTab().getInfoBar() != null) {
	    getParentTab().getInfoBar().setText(localMessage);
	}
    }
    
    /**
     * Display the message into an error popup
     * @param message Error of the message
     */
    public void error(String message) {
    	JOptionPane.showMessageDialog(getAsComponent(), message, XcosMessages.XCOS, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Find the block corresponding to the given uid
     * and display a warning message.
     * 
     * @param uid - A String as UID.
     * @param message - The message to display.
     */
    public void warnCellByUID(String uid, String message) {
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
	    if (getModel().getChildAt(getDefaultParent(), i) instanceof mxCell) {
		if (((mxCell) getModel().getChildAt(getDefaultParent(), i)).getId().compareTo(uid) == 0) {
		    //to put on top, only for new message, no for reset
		    if(message.compareTo("") != 0) {
			setVisible(true);
		    }
		    
		    getAsComponent().setCellWarning(getModel().getChildAt(getDefaultParent(), i), message);
		}
	    }
	}
    }

    /**
     * Set the current diagram in a modified state
     * @param modified True or False whether the current diagram must be saved or not. 
     */
    public void setModified(boolean modified) {
		super.setModified(modified);
		updateTabTitle();
	}

    /**
     * Revert an action
     */
	public void undo() {
		super.undo();
		
		if (getParentTab() != null) {
			if (undoManager.canUndo()) {
				((XcosTab) getParentTab()).setEnabledUndo(true);
			} else {
				((XcosTab) getParentTab()).setEnabledUndo(false);
			}
			((XcosTab) getParentTab()).setEnabledRedo(true);
		}

		updateUndoModifiedState();
		/*
		 * if (undoManager.canRedo()){
		 * ((Xcos)getParentTab()).setEnabledRedo(true); } else {
		 * ((Xcos)getParentTab()).setEnabledRedo(false); }
		 */
	}

    

	/**
	 * Apply the previously reverted action
	 */
	public void redo() {
		super.redo();
		
		updateUndoModifiedState();
		
		if (getParentTab() != null) {
			if (undoManager.canUndo()) {
				((XcosTab) getParentTab()).setEnabledUndo(true);
			} else {
				((XcosTab) getParentTab()).setEnabledUndo(false);
			}
			if (undoManager.canRedo()) {
				((XcosTab) getParentTab()).setEnabledRedo(true);
			} else {
				((XcosTab) getParentTab()).setEnabledRedo(false);
			}
		}
	}

	/**
	 * This function will reset the UndoManager in a stable state.
	 */
	public void resetUndoManager() {
		undoManager.reset();
		
		resetUndoCounter();
		
		if (getParentTab() != null) {
			((XcosTab) getParentTab()).setEnabledRedo(false);
			((XcosTab) getParentTab()).setEnabledUndo(false);
		}
	}
	
	private void updateUndoModifiedState() {
		if (isZeroUndoCounter()) {
		    setModified(false);
		}
		else
		{
		    setModified(true);
		}
	    }
	
	/**
	 * This function checks for the popup menu activation under MacOS with Java version 1.5
	 * Related to Scilab bug #5190
	 * @return true if Java 1.5 and MacOS and mouse clic and ctrl activated
	 */
	private boolean isMacOsPopupTrigger(MouseEvent e) {
		return (SwingUtilities.isLeftMouseButton(e) && e.isControlDown() && (System.getProperty("os.name").toLowerCase().indexOf("mac") != -1) && (System.getProperty("java.specification.version").equals("1.5")));
	}
	
	public void setContextAction(SetContextAction action) {
		this.action = action;
	}
	
	public SetContextAction getContextAction() {
		return action;
	}

	    protected BasicBlock getChildById(String uid) {
		BasicBlock returnBlock = null;
		for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
		    if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
			BasicBlock block = (BasicBlock)getModel().getChildAt(getDefaultParent(), i);
			if (block.getId().compareTo(uid) == 0) { //find it
			    returnBlock = block;
			} else {
			    if(block instanceof SuperBlock) {
				boolean created = false;
				if(((SuperBlock)block).getChild() == null) { 
				    //create temporary SuperBlock to find child
				    ((SuperBlock)block).createChildDiagram();
				    created = true;
				}

				//search in child
				returnBlock = ((SuperBlock)block).getChild().getChildById(uid);

				if(created) { //if temporary, destroy it
				    ((SuperBlock)block).getChild().closeDiagram();
				}
			    } else if(block.getRealParameters() instanceof ScilabMList) { 
				//we have a hidden SuperBlock, create a real one
				SuperBlock newSP = (SuperBlock)BasicBlock.createBlock("SUPER_f");
				newSP.setParentDiagram(block.getParentDiagram());
				newSP.setRealParameters(block.getRealParameters());
				newSP.createChildDiagram();
				//search in child
				returnBlock = newSP.getChild().getChildById(uid);
				newSP.getChild().closeDiagram();
				newSP = null;
			    }
			}
		    }
		    
		    if(returnBlock != null) {
			return returnBlock;
		    }
		}
		return returnBlock;
	    }
}

