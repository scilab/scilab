/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.awt.Color;
import java.awt.MouseInfo;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyVetoException;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabCanvas;
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
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.DiagramBackgroundAction;
import org.scilab.modules.xcos.actions.SetContextAction;
import org.scilab.modules.xcos.actions.SetupAction;
import org.scilab.modules.xcos.actions.XcosDocumentationAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.block.actions.ShowParentAction;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.io.scicos.DiagramElement;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
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
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.model.mxGraphModel.mxStyleChange;
import com.mxgraph.model.mxIGraphModel.mxAtomicGraphModelChange;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUtils;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.view.mxGraphSelectionModel;
import com.mxgraph.view.mxMultiplicity;
import com.mxgraph.view.mxStylesheet;

/**
 * The base class for a diagram. This class contains jgraphx + Scicos data.
 */
public class XcosDiagram extends ScilabGraph {
	private static final Log LOG = LogFactory.getLog(XcosDiagram.class);
	
	// the associated parameters
	private ScicosParameters scicosParameters;
	
	// the scicos engine current status
	private final transient CompilationEngineStatus engine;
	
    //private Window palette;
    private Tab viewPort;
    
    /*to manage splitLink*/
    private BasicLink splitLink;
    private BasicPort splitPort;
    private mxPoint dragSplitPos;
    private boolean waitSplitRelease;
    
    /*to manage path link*/
    private BasicLink drawLink;
    private boolean waitPathAddEdge;
    private boolean waitPathRelease;

    private CheckBoxMenuItem viewPortMenu;
    private CheckBoxMenuItem gridMenu;
    
    protected mxIEventListener deleteLinkOnMultiPointLinkCreation = new mxIEventListener() {
		public void invoke(Object sender, mxEventObject evt) {
			if (waitPathAddEdge) {
			    if (drawLink != null) {
				getModel().remove(drawLink);
			    }
			    cancelDrawLinkAction();
			}
		}
	};

	/**
	 * Add an edge from a source to the target.
	 * 
	 * @param edge the edge to add (may be null)
	 * @param parent the parent of the source and the target
	 * @param source the source cell
	 * @param target the target cell
	 * @param index the index of the edge
	 * @return the added edge or null.
	 * @see com.mxgraph.view.mxGraph#addEdge(java.lang.Object, java.lang.Object, java.lang.Object, java.lang.Object, java.lang.Integer)
	 */
	@Override
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
    			waitSplitRelease = true;
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
    			waitSplitRelease = true;
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
    			waitSplitRelease = true;
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
    			waitSplitRelease = true;
    			splitLink = (BasicLink) target;
    			splitPort = (BasicPort) source;
    			return null;
    			//return addSplitEdge((BasicLink) target, (BasicPort)source);
    		}
    	}

    	
    	if (source != null && target == null) {
    	    drawLink = null;
    	    if (source instanceof ExplicitInputPort || source instanceof ExplicitOutputPort) {
    		drawLink = (BasicLink) super.addEdge(new ExplicitLink(), getDefaultParent(), source, null, index);
    	    } else if (source instanceof ImplicitInputPort || source instanceof ImplicitOutputPort) {
    		drawLink = (BasicLink) super.addEdge(new ImplicitLink(), getDefaultParent(), source, null, index);
    	    } else if (source instanceof ControlPort || source instanceof CommandPort) {
    		drawLink = (BasicLink) super.addEdge(new CommandControlLink(), getDefaultParent(), source, null, index);
    	    } else if (source instanceof BasicLink) {
				/*
				 * This is a very specific case: user wants to put a split block
				 * on a link and start a new partial link. It's not handled as a
				 * SplitBlock must not be partially initialized.
				 */
    	    	return null;
    	    }

    	    if (drawLink != null) {
    	    	waitPathRelease = true;
    	    }

    	    info(XcosMessages.DRAW_LINK);
    	    return drawLink;
    	}
    	return null;
    }

    /**
     * Add a split on a edge.
     * 
     * @param link source link
     * @param target target port
     * @return split block
     */
    private Object addSplitEdge(BasicLink link, mxICell target) {
    	BasicPort linkSource =  (BasicPort) link.getSource();
    	BasicPort linkTarget =  (BasicPort) link.getTarget();

    	getModel().beginUpdate();
    	if (dragSplitPos == null) {
    		dragSplitPos = new mxPoint();

            //check splitPosition values
            double srcX = linkSource.getParent().getGeometry().getX() + linkSource.getGeometry().getCenterX();
            double tgtX = linkTarget.getParent().getGeometry().getX() + linkTarget.getGeometry().getCenterX();
            double srcY = linkSource.getParent().getGeometry().getY() + linkSource.getGeometry().getCenterY();
            double tgtY = linkTarget.getParent().getGeometry().getY() + linkTarget.getGeometry().getCenterY();

            double offsetX = (tgtX - srcX) / 2;
            double offsetY = (tgtY - srcY) / 2;
            dragSplitPos.setX(srcX + offsetX);
            dragSplitPos.setY(srcY + offsetY);
    	}
   	
    	SplitBlock splitBlock = (SplitBlock) BlockFactory.createBlock(BlockInterFunction.SPLIT_f);
    	if (target instanceof BasicLink) {
    	    splitBlock.setConnection(linkSource, linkTarget, (BasicPort) ((BasicLink) target).getSource());
    	} else {
    		splitBlock.setConnection(linkSource, linkTarget, (BasicPort) target);
    	}
    	
    	mxGeometry geom = splitBlock.getGeometry();
    	geom.setX(dragSplitPos.getX());
    	geom.setY(dragSplitPos.getY());
    	BlockPositioning.alignPoint(geom, getGridSize(), (SplitBlock.DEFAULT_SIZE / 2));
    	
    	addCell(splitBlock);
    	
    	
    	//Update old link
    	
    	//get breaking segment
    	int pos = link.findNearestSegment(dragSplitPos);

    	//save points after breaking point
    	mxPoint[] saveStartPoints = link.getPoints(pos, true);
    	mxPoint[] saveEndPoints = link.getPoints(pos, false);
    	
    	
    	//disable events
    	getModel().beginUpdate();
    	getModel().remove(link);
    	getModel().endUpdate();

    	BasicLink newLink1 = BasicLink.createLinkFromPorts(linkSource, splitBlock.getIn());
    	newLink1.setGeometry(new mxGeometry(0, 0, 80, 80));
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
    	newLink2.setGeometry(new mxGeometry(0, 0, 80, 80));
    	newLink2.setSource(splitBlock.getOut1());
    	newLink2.setTarget(linkTarget);
    	//add points after breaking point in the new link
    	if (saveEndPoints != null) {
    		for (int i = 0; i < saveEndPoints.length; i++) {
    			newLink2.addPoint(saveEndPoints[i].getX(), saveEndPoints[i].getY());
    		}
       	}
    	addCell(newLink2);

    	if (target != null) {
    	
    	if (target instanceof BasicLink) {
    	    //build link inverted ! it will be invert later
    	    ((BasicLink) target).setTarget(splitBlock.getOut2());
    	    addCell(target);
    	} else {
    	    BasicLink newLink3 = BasicLink.createLinkFromPorts(splitBlock.getOut2(), (BasicPort) target);
    	    newLink3.setGeometry(new mxGeometry(0, 0, 80, 80));
    	    newLink3.setSource(splitBlock.getOut2());
    	    newLink3.setTarget((mxCell) target);
    	    addCell(newLink3);
    	}
    	
    	}
    	dragSplitPos = null;
	refresh();
	getModel().endUpdate();
	
    	return splitBlock;
    }
    
    /**
     * Constructor
     */
    public XcosDiagram() {
	super();
	
	// Scicos related setup
	engine = new CompilationEngineStatus();
	setScicosParameters(new ScicosParameters());
	
	// Add a default listener to update the modification status when
	// something has changed on the ScicosParameters
	scicosParameters.addPropertyChangeListener(new PropertyChangeListener() {
		@Override
		public void propertyChange(PropertyChangeEvent evt) {
			setModified(true);
		}
	});
	
	
	getUndoManager().addListener(mxEvent.UNDO, deleteLinkOnMultiPointLinkCreation);
	
	installStylesheet();

	getAsComponent().setToolTips(true);

	// Forbid disconnecting cells once it is connected.
	setCellsDisconnectable(false);

	// Forbid pending edges.
	//setAllowDanglingEdges(false);
	setAllowDanglingEdges(true);

	// Cannot connect port to itself.
	setAllowLoops(false);

	// Override isCellResizable to filter what the user can resize
	setCellsResizable(true);

	// force auto resize cell
	setAutoSizeCells(true);

	/* Labels use HTML if not equal to interface function name */
	setHtmlLabels(true);
	
	//
	//setCloneInvalidEdges(false);
	setCloneInvalidEdges(true);

	// Override isCellEditable to filter what the user can edit
	setCellsEditable(true);
	// This enable stop editing cells when pressing Enter.
	getAsComponent().setEnterStopsCellEditing(false);

	setConnectableEdges(true);
	getAsComponent().setTolerance(1);
	
	getAsComponent().getViewport().setOpaque(false);
	getAsComponent().setBackground(Color.WHITE);

	setMultiplicities();
	
	// Add a listener to track when model is changed
	getModel().addListener(XcosEvent.CHANGE, new ModelTracker());
	
	setGridVisible(true);
	
	((mxCell) getDefaultParent()).setId((new UID()).toString());
	((mxCell) getModel().getRoot()).setId((new UID()).toString());
    }

	/**
	 * Install the default style sheet and the user stylesheet on the diagram.
	 */
	private void installStylesheet() {
		final mxStylesheet styleSheet = Xcos.getInstance().getStyleSheet();
		setStylesheet(styleSheet);
		
		// Set Canvas background
		URL background = null;
		try {
			Map<String, Object> style = styleSheet.getCellStyle("Icon", null);
			if (style != null) {
				background = new URL((String) style.get(XcosConstants.STYLE_IMAGE));
			}
		} catch (MalformedURLException e) {
			LOG.warn(e);
		}
		((ScilabCanvas) getAsComponent().getCanvas())
				.setSvgBackgroundImage(background);
	}
    
    /**
     * Install the multiplicities (use for link checking)
     */
    private void setMultiplicities() {
	mxMultiplicity[] multiplicities = new mxMultiplicity[10];

	
	// Input data port
	multiplicities[0] = new PortCheck(ExplicitInputPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = -4987163442006736665L;
		    {
			add(ExplicitOutputPort.class);
			add(ExplicitLink.class);
		    }
		}), XcosMessages.LINK_ERROR_EXPLICIT_IN);
	multiplicities[1] = new PortCheck(ImplicitInputPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 886376532181210926L;
		    {
			add(ImplicitOutputPort.class);
			add(ImplicitInputPort.class);
			add(ImplicitLink.class);
		    }
		}), XcosMessages.LINK_ERROR_IMPLICIT_IN);

	// Output data port
	multiplicities[2] = new PortCheck(ExplicitOutputPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 4594127972486054821L;
		    {
			add(ExplicitInputPort.class);
		    }
		}), XcosMessages.LINK_ERROR_EXPLICIT_OUT);
	multiplicities[3] = new PortCheck(ImplicitOutputPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = -3719677806532507973L;
		    {
			add(ImplicitInputPort.class);
			add(ImplicitOutputPort.class);
			add(ImplicitLink.class);
		    }
		}), XcosMessages.LINK_ERROR_IMPLICIT_OUT);

	// Control port
	multiplicities[4] = new PortCheck(ControlPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 2941077191386058497L;
		    {
			add(CommandPort.class);
			add(CommandControlLink.class);
		    }
		}), XcosMessages.LINK_ERROR_EVENT_IN);

	// Command port
	multiplicities[5] = new PortCheck(CommandPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = -3470370027962480362L;
		    {
			add(ControlPort.class);
		    }
		}), XcosMessages.LINK_ERROR_EVENT_OUT);

	// ExplicitLink connections
	multiplicities[6] = new PortCheck(ExplicitLink.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 7423543162930147373L;

		    {
			add(ExplicitInputPort.class);
		    }
		}), XcosMessages.LINK_ERROR_EVENT_OUT);

	// ImplicitLink connections
	multiplicities[7] = new PortCheck(ImplicitLink.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 7775100011122283282L;

		    {
			add(ImplicitInputPort.class);
			add(ImplicitOutputPort.class);
		    }
		}), XcosMessages.LINK_ERROR_EVENT_OUT);

	// CommandControlLink connections
	multiplicities[8] = new PortCheck(CommandControlLink.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 3260421433507192386L;

		    {
			add(ControlPort.class);
		    }
		}), XcosMessages.LINK_ERROR_EVENT_OUT);

	// Already connected port
	multiplicities[9] = new PortCheck(BasicPort.class, Collections
		.unmodifiableList(new ArrayList<Class< ? extends mxCell>>() {
		    private static final long serialVersionUID = 6376349598052836660L;

		    {
			add(BasicPort.class);
		    }
		}), XcosMessages.LINK_ERROR_ALREADY_CONNECTED);

	setMultiplicities(multiplicities);
    }

    /**
     * Install all needed Listeners.
     */
    public void installListeners() {

	// Property change Listener
	// Will say if a diagram has been modified or not.
	getAsComponent().addPropertyChangeListener(new PropertyChangeListener() {
	    public void propertyChange(PropertyChangeEvent e) {
		if (e.getPropertyName().compareTo("modified") == 0) {
		    if (!e.getOldValue().equals(e.getNewValue())) {
			updateTabTitle();
		    }
		}
	    }
	});

	// Track when superblock ask a parent refresh.
	addListener(XcosEvent.SUPER_BLOCK_UPDATED, new SuperBlockUpdateTracker()); 
	
	// Track when cells are added.
	addListener(XcosEvent.CELLS_ADDED, new CellAddedTracker(this)); 
	addListener(XcosEvent.CELLS_ADDED, getEngine());
	
	// Track when cells are deleted.
	addListener(XcosEvent.CELLS_REMOVED, new CellRemovedTracker(this)); 
	addListener(XcosEvent.CELLS_REMOVED, getEngine());
	
	// Track when resizing a cell.
	addListener(XcosEvent.CELLS_RESIZED, new CellResizedTracker());
	
	// Track when we have to force a Block value
	addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, new ForceCellValueUpdate());
	addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, getEngine());
	
	// Update the blocks view on undo/redo
	getUndoManager().addListener(mxEvent.UNDO, new UndoUpdateTracker());
	getUndoManager().addListener(mxEvent.REDO, new UndoUpdateTracker());
	
	getAsComponent().getGraphControl().addMouseListener(new XcosMouseListener(this));

	getAsComponent().addKeyListener(new XcosKeyListener(this));

	addListener(XcosEvent.ADD_PORTS, new mxIEventListener() {
	    public void invoke(Object source, mxEventObject evt) {
		getModel().beginUpdate();
		refresh();
		BasicBlock updatedBlock = (BasicBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
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
	/**
	 * Constructor
	 */
	public ModelTracker() {
	    super();
	}

	/**
	 * Fire cell value update on any change 
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    List<mxAtomicGraphModelChange> changes = (List<mxAtomicGraphModelChange>) (evt.getProperty("changes"));
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
		fireEvent(new mxEventObject(XcosEvent.FORCE_CELL_VALUE_UPDATE, "cells", firedCells));
	    }
	    getModel().endUpdate();
	}
    }
    /**
     * ForceCellValueUpdate
     * Called when we want a block content to update.
     */
    private class ForceCellValueUpdate implements mxIEventListener {
	/**
	 * Constructor
	 */
	public ForceCellValueUpdate() {
	    super();
	}

	/**
	 * Handle cell value update on force cell value (update size and value)
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getProperty("cells");

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
	/**
	 * Constructor
	 */
	public ForceCellReshapeTracker() {
	    super();
	}

	/**
	 * Update the block view
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells =  (Object[]) evt.getProperty("cells");
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
	
	/**
	 * Constructor 
	 */
	public SuperBlockUpdateTracker() {
	    super();
	}

	/**
	 * Update the superblock values (rpar) on update
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    assert evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED) instanceof SuperBlock;
	    SuperBlock updatedBlock = (SuperBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
	    updatedBlock.setRealParameters(new DiagramElement().encode(updatedBlock.getChild()));
	    if (updatedBlock.getParentDiagram() instanceof SuperBlockDiagram) {
		SuperBlock parentBlock = ((SuperBlockDiagram) updatedBlock
			.getParentDiagram()).getContainer();
		parentBlock.getParentDiagram().fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED,
			XcosConstants.EVENT_BLOCK_UPDATED, parentBlock));
	    }
	    BlockPositioning.updateBlockView(updatedBlock);
	    refresh();
	}
    }

    /**
     * CellAddedTracker
     * Called when mxEvents.CELLS_ADDED is fired.
     */
    private static class CellAddedTracker implements mxIEventListener {
    	private XcosDiagram diagram;

    	/**
    	 * @param diagram diagram
    	 */
    	public CellAddedTracker(XcosDiagram diagram) {
    		this.diagram = diagram;
    	}

    	/**
    	 * Update block values on add 
    	 * @param source the source instance
    	 * @param evt the event data
    	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
    	 */
    	public void invoke(Object source, mxEventObject evt) {
    		Object[] cells = (Object[]) evt.getProperty("cells");
    		
    		diagram.getModel().beginUpdate();
    		for (int i = 0; i < cells.length; ++i) {

//    			((mxCell) cells[i]).setId((new UID()).toString());

				if (cells[i] instanceof BasicBlock) {
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
	/**
	 * @param diagram diagram
	 */
	public CellRemovedTracker(XcosDiagram diagram) {
	}

	/**
	 * Clear any currently drawn link 
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getProperty("cells");
	    for (int i = 0; i < cells.length; i++) {
		if (cells[i] instanceof BasicLink) {
		    BasicLink link = (BasicLink) cells[i];
		    removeLink(link);
		    if (waitPathAddEdge) {
			cancelDrawLinkAction();
		    }
		}
	    }

	}
    }

    /**
     * @param link link to remove
     */
    private void removeLink(BasicLink link) {
	if (!(link.getSource() instanceof BasicPort)) {
	    return;
	}

	if (!(link.getTarget() instanceof BasicPort)) {
	    return;
	}

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
	    newLink.setGeometry(new mxGeometry(0, 0, 80, 80));

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

    /**
     * @param source origin port
     * @return opposite port
     */
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
	/**
	 * Constructor
	 */
	public CellResizedTracker() {
	    super();
	}

	/**
	 * Update the cell view
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
	    Object[] cells = (Object[]) evt.getProperty("cells");
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
     * Update the modified block on undo/redo
     */
   private class UndoUpdateTracker implements mxIEventListener {
        /**
         * Constructor
         */
        public UndoUpdateTracker() {
	    super();
	}

	/**
	 * Update the block and style on undo
	 * @param source the source instance
	 * @param evt the event data
	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
	 */
	public void invoke(Object source, mxEventObject evt) {
            List<mxUndoableChange> changes = ((mxUndoableEdit) evt.getProperty(XcosConstants.EVENT_CHANGE_EDIT)).getChanges();
            Object[] changedCells = getSelectionCellsForChanges(changes);
            getModel().beginUpdate();
            for (Object object : changedCells) {
		if (object instanceof BasicBlock) {
		    BasicBlock current = (BasicBlock) object;
		    
		    // When we change the style property we have to update some
		    // BasiBlock fields
		    if (changes.get(0) instanceof mxStyleChange) {
			current.updateFieldsFromStyle();
		    }
		    
		    // Update the block position
		    BlockPositioning.updateBlockView(current);
		}
	    }
            getModel().endUpdate();
            refresh();
        }
    };
    
    /**
     * @author Antoine ELIAS
     *
     */
    private class XcosKeyListener implements KeyListener {

	/**
	 * @param diagram diagram
	 */
	public XcosKeyListener(XcosDiagram diagram) { }

	/**
	 * Do nothing
	 * @param e the event
	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
	 */
	@Override
	public void keyTyped(KeyEvent e) { }

	/**
	 * Do nothing
	 * @param e the event
	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
	 */
	@Override
	public void keyPressed(KeyEvent e) { }	

	/**
	 * Cancel current link on escape
	 * @param e the event
	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
	 */
	@Override
	public void keyReleased(KeyEvent e) {
	    if (e.getKeyChar() == KeyEvent.VK_ESCAPE) {
		if (drawLink != null) {
		    getModel().remove(drawLink);
		    cancelDrawLinkAction();
		}		    
	    }
	}
    }
    /**
     * MouseListener inner class
     */
    private class XcosMouseListener implements MouseListener {
    	private XcosDiagram diagram;

    	/**
    	 * @param diagram diagram
    	 */
    	public XcosMouseListener(XcosDiagram diagram) {
    		this.diagram = diagram;
    	}

    	/**
    	 * Handle click on the diagram component
    	 * @param e the event
    	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
    	 */
    	@Override
    	public void mouseClicked(MouseEvent e) {
    		Object cell = getAsComponent().getCellAt(e.getX(), e.getY());
    		double scale = getView().getScale();

    		// Double Click within empty diagram Area
    		if (e.getClickCount() >= 2 && SwingUtilities.isLeftMouseButton(e) && cell == null) {
    			TextBlock textBlock = (TextBlock) BlockFactory.createBlock(BlockInterFunction.TEXT_f);
    			mxGeometry geom = textBlock.getGeometry();
    			mxGeometry parent = ((mxICell) getDefaultParent()).getGeometry();
    			if (parent != null) {
    				// update the geometry in place
    				geom.setX(geom.getX() - parent.getX());
    				geom.setY(geom.getY() - parent.getY());
    			}
    			addCell(textBlock);
    			return;
    		}

    		// Double Click within some component
    		if (e.getClickCount() >= 2 && SwingUtilities.isLeftMouseButton(e) && cell != null)
    		{
    			getModel().beginUpdate();
    			if (cell instanceof BasicBlock) {
    				BasicBlock block = (BasicBlock) cell;
    				e.consume();
    				block.openBlockSettings(getScicosParameters().getContext());
    			}
    			if (cell instanceof BasicLink) {
    				mxGeometry parent = ((BasicLink) cell).getParent().getGeometry();
    				mxPoint p = new mxPoint(e.getX() / scale, e.getY() / scale);
    				if (parent != null) {
    					p.setX(p.getX() - parent.getX());
    					p.setY(p.getY() - parent.getY());
    				}
    				BlockPositioning.alignPoint(p, getGridSize(), 0);
    				((BasicLink) cell).insertPoint(p);
    			}
    			getModel().endUpdate();
    			refresh();
    		}

    		// Ctrl + Shift + Right Middle Click : for debug !!
    		if (e.getClickCount() >= 2 && SwingUtilities.isMiddleMouseButton(e)
    				&& e.isShiftDown() && e.isControlDown()) {
    			System.err.println("[DEBUG] Click at position : " + e.getX() + " , " + e.getY());
    			if (cell == null) {
    				System.err.println("[DEBUG] Click on diagram");
    				System.err.println("Default Parent ID : " + ((mxCell) getDefaultParent()).getId());
    				System.err.println("Model root ID : " + ((mxCell) getModel().getRoot()).getId());
    				System.err.println("getParentWindow : " + (getParentTab() == null ? null : getParentTab().getParentWindow()));
    			} else {
    				System.err.println("[DEBUG] Click on : " + cell);
    				System.err.println("[DEBUG] Style : " + ((mxCell) cell).getStyle());
    				System.err.println("[DEBUG] NbEdges : " + ((mxCell) cell).getEdgeCount());
    				System.err.println("[DEBUG] NbChildren : " + ((mxCell) cell).getChildCount());
    				for (int i = 0; i < ((mxCell) cell).getChildCount(); i++) {
    					System.err.println("[DEBUG] Child NbEdges : " + ((mxCell) cell).getChildAt(i).getEdgeCount());
    				}

    				if (cell instanceof BasicLink) {
    					System.err.println("[DEBUG] Link Points : " + ((BasicLink) cell).getPointCount());
    				}
    			}

    		}

    		// Context menu
    		if ((e.getClickCount() == 1 && SwingUtilities.isRightMouseButton(e))
    				|| e.isPopupTrigger()
    				|| XcosMessages.isMacOsPopupTrigger(e)) {

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

    				if (diagram instanceof SuperBlockDiagram) {
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

    				((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x,
    					MouseInfo.getPointerInfo().getLocation().y);

    				menu.setVisible(true);

    			} else {
    				// Display object context menu
    				if (cell instanceof BasicBlock) {
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

    	/**
    	 * Do nothing
    	 * @param e the event
    	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
    	 */
    	@Override
    	public void mouseEntered(MouseEvent e) {
    	}

    	/**
    	 * Do nothing
    	 * @param e the event
    	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
    	 */
    	@Override
    	public void mouseExited(MouseEvent e) {
    	}

    	/**
    	 * Do nothing
    	 * @param e the event
    	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
    	 */
    	@Override
    	public void mousePressed(MouseEvent e) {
    	}

    	/**
    	 * Cancel split and link if running
    	 * @param e the event
    	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
    	 */
    	@Override
    	public void mouseReleased(MouseEvent e) {
    		Object cell = getAsComponent().getCellAt(e.getX(), e.getY());
    		double scale = getView().getScale();
    		
    		if (SwingUtilities.isLeftMouseButton(e)) {
    			if (waitSplitRelease) {
    				dragSplitPos = new mxPoint(e.getX() / scale, e.getY() / scale);
    				waitSplitRelease = false;
    				addSplitEdge(splitLink, splitPort);
    			} else if (waitPathRelease) {
    				//Tips for ignore first mouse release after drag
    				waitPathRelease = false;
    				waitPathAddEdge = true;

    				if (!e.isControlDown()) {
    					//adjust final point
    					mxGeometry geoPort = drawLink.getSource().getGeometry();
    					mxGeometry geoBlock;
    					final mxICell parent = drawLink.getSource().getParent();
    					if (parent == null) {
    						geoBlock = new mxGeometry();
    					} else {
    						geoBlock = drawLink.getSource().getParent().getGeometry();
    					}
    					mxPoint lastPoint = new mxPoint(geoBlock.getX() + geoPort.getCenterX(),
    						geoBlock.getY() + geoPort.getCenterY());
    					mxPoint click = new mxPoint(e.getX() / scale, e.getY() / scale);
    					mxPoint point = getPointPosition(lastPoint, click);

    					getModel().beginUpdate();
    					drawLink.getGeometry().setTargetPoint(point);
    					getModel().endUpdate();
    					refresh();
    				}
    			} else if (waitPathAddEdge) {
    				if (drawLink != null) {
    					getModel().beginUpdate();
    					//move end of link and add point at old position
    					mxGeometry geo = drawLink.getGeometry();
    					drawLink.addPoint(geo.getTargetPoint().getX(), geo.getTargetPoint().getY());
    					setSelectionCell(drawLink);

    					if (cell != null) {
    						mxICell source = drawLink.getSource();

    						StringBuffer error = checkMultiplicities(drawLink, source, cell);
    						if (error == null) {
    							if (cell instanceof BasicLink && cell != drawLink) { //no loop
    								//draw link with a SplitBlock
    								dragSplitPos = new mxPoint(e.getX() / scale, e.getY() / scale);
    								addSplitEdge((BasicLink) cell, drawLink);
    							} else {
    								getModel().setTerminal(drawLink, cell, false);
    							}

    							//invert source and target if needed
    							if (!checkEdgeDirection(source, cell)) {
    								getModel().beginUpdate();
    								drawLink.invertDirection();
    								getModel().endUpdate();
    							}

    							//reset info, flags and object
    							cancelDrawLinkAction();
    						} else {
    							if (cell != drawLink) {
    								JOptionPane.showMessageDialog(getAsComponent(), error);
    							}
    							setSelectionCell(drawLink);
    						}
    					} else {
    						mxPoint click = new mxPoint(e.getX() / scale, e.getY() / scale);
    						if (!e.isControlDown()) {
    							geo.setTargetPoint(getPointPosition(geo.getTargetPoint(), click));
    						} else {
    							geo.setTargetPoint(click);
    						}
    					}
    					getModel().endUpdate();
    					refresh();
    				} else {
    					cancelDrawLinkAction();
    				}
    			} else {
    				dragSplitPos = null;
    			}
    		}
    	}
    }

	/**
	 * Get the point position according to the scale.
	 * 
	 * @param origin
	 *            last point in the draklink
	 * @param click
	 *            clicked point in diagram window
	 * @return new real point
	 */
	mxPoint getPointPosition(mxPoint origin, mxPoint click) {
		final double origX = origin.getX();
		final double origY = origin.getY();

		final double clickX = click.getX();
		final double clickY = click.getY();

		final boolean signX = clickX > origX;
		final boolean signY = clickY > origY;
		
		double diffX = Math.abs(clickX - origX);
		double diffY = Math.abs(clickY - origY);
		if (diffX > diffY) {
			if (diffY > (diffX / 2)) { // diagonal
				diffY = diffX;
			} else { // orthogonal
				diffY = 0;
			}
		} else { // < or ==
			if (diffX > (diffY / 2)) { // diagonal
				diffX = diffY;
			} else { // orthogonal
				diffX = 0;
			}
		}

		// restore signs
		if (!signX) {
			diffX = -diffX;
		}

		if (!signY) {
			diffY = -diffY;
		}

		mxPoint p = new mxPoint(origX + diffX, origY + diffY);
		BlockPositioning.alignPoint(p, getGridSize(), 0);		
		return p;
	}

	/**
	 * Manage Group to be CellFoldable i.e with a (-) to reduce and a (+) to
	 * expand them. Only non-Block / non-Port Cell are foldable.
	 * 
	 * @param cell
	 *            the selected cell
	 * @param collapse
	 *            the collapse settings
	 * @return <code>true</code> if the cell is foldable, <code>false</code>
	 *         otherwise.
	 * @see com.mxgraph.view.mxGraph#isCellFoldable(java.lang.Object, boolean)
	 */
	@Override
    public boolean isCellFoldable(Object cell, boolean collapse) {
		return !(cell instanceof BasicBlock) && super.isCellFoldable(cell, collapse);
    }

	/**
	 * Return true if selectable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellSelectable(java.lang.Object)
	 */
	@Override
    public boolean isCellSelectable(Object cell) {
	if (cell instanceof BasicPort) {
	    return false;
	}
	return super.isCellSelectable(cell);
    }

	/**
	 * Return true if movable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellMovable(java.lang.Object)
	 */
	@Override
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

	/**
	 * Return true if resizable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellResizable(java.lang.Object)
	 */
	@Override
    public boolean isCellResizable(Object cell) {
    	if (cell instanceof SplitBlock) {
    		return false;
    	}
    	return (cell instanceof BasicBlock) && super.isCellResizable(cell);
    }

	/**
	 * Return true if deletable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellDeletable(java.lang.Object)
	 */
	@Override
    public boolean isCellDeletable(Object cell) {
    	if (cell instanceof BasicBlock && ((BasicBlock) cell).isLocked()) {
    		return false;
    	}

    	return !(cell instanceof BasicPort)	&& super.isCellDeletable(cell);
    }

	/**
	 * Return true if editable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellEditable(java.lang.Object)
	 */
	@Override
    public boolean isCellEditable(Object cell) {
    	return (cell instanceof TextBlock) && super.isCellDeletable(cell);
    }

	/**
	 * Return true if disconnectable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellDisconnectable(java.lang.Object)
	 */
	@Override
    public boolean isCellDisconnectable(Object cell, Object terminal,boolean source) {
        return super.isCellDisconnectable(cell, terminal, source);
    }
    
	/**
	 * Return true if connectable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellConnectable(java.lang.Object)
	 */
	@Override
    public boolean isCellConnectable(Object cell) {
	//currently in draw link action
	if (waitPathAddEdge) {
	    if (drawLink != null) {
		StringBuffer error = checkMultiplicities(drawLink, drawLink.getSource(), cell);
		if (error == null) {
		    return true;
		}
		return false;
	    }
	}
	
	if (cell instanceof BasicBlock)  {
	    return false;
	}
	
	if (cell instanceof BasicPort) {
	    int sourceOut = mxGraphModel.getDirectedEdgeCount(getModel(), cell, true);
	    int targetIn = mxGraphModel.getDirectedEdgeCount(getModel(), cell, false);
	    
	    if (sourceOut > 0 || targetIn > 0) {
		return false;
	    }
	}
    	return !(cell instanceof BasicBlock) && super.isCellConnectable(cell);
    }

	/**
	 * Return true if auto sized
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isAutoSizeCell(java.lang.Object)
	 */
	@Override
    public boolean isAutoSizeCell(Object cell) {
		boolean status = super.isAutoSizeCell(cell);
		
		if (cell instanceof AfficheBlock) {
			status |= true;
		}
		
		if (cell instanceof TextBlock) {
			status &= false;
		}
		
    	return status;
    }


    /**
     * @param fileName HDF5 filename
     */
    public void dumpToHdf5File(String fileName) {
	String writeFile = fileName;
	if (fileName == null) {
	    FileChooser fc = ScilabFileChooser.createFileChooser();
	    fc.setInitialDirectory(getSavedFile());
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		return;
	    }
	    writeFile = fc.getSelection()[0];
	}
	
	new H5RWHandler(writeFile).writeDiagram(this);
    }
    
    /**
	 * @return the parameters
	 */
	public ScicosParameters getScicosParameters() {
		return scicosParameters;
	}

	/**
	 * @param scicosParameters the simulation parameters
	 */
	public void setScicosParameters(ScicosParameters scicosParameters) {
		this.scicosParameters = scicosParameters;
		
		scicosParameters.addPropertyChangeListener(getEngine());
	}
	
	/**
	 * @return the engine
	 */
	public CompilationEngineStatus getEngine() {
		return engine;
	}

	/**
     * @param finalIntegrationTime set integration time
	 * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setFinalIntegrationTime(double finalIntegrationTime) throws PropertyVetoException {
    	scicosParameters.setFinalIntegrationTime(finalIntegrationTime);
    }

    /**
     * @param integratorAbsoluteTolerance set integrator absolute tolerance
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorAbsoluteTolerance(double integratorAbsoluteTolerance) throws PropertyVetoException {
		scicosParameters.setIntegratorAbsoluteTolerance(integratorAbsoluteTolerance);
    }

    /**
     * @param integratorRelativeTolerance integrator relative tolerance
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorRelativeTolerance(double integratorRelativeTolerance) throws PropertyVetoException {
    	scicosParameters.setIntegratorRelativeTolerance(integratorRelativeTolerance);
    }

    /**
     * @param maximumStepSize set max step size
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaximumStepSize(double maximumStepSize) throws PropertyVetoException {
    	scicosParameters.setMaximumStepSize(maximumStepSize);
    }

    /**
     * @param maxIntegrationTimeinterval set max integration time
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaxIntegrationTimeinterval(double maxIntegrationTimeinterval) throws PropertyVetoException {
	 scicosParameters.setMaxIntegrationTimeInterval(maxIntegrationTimeinterval);
    }

    /**
     * @param realTimeScaling set real time scaling
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setRealTimeScaling(double realTimeScaling) throws PropertyVetoException {
    	scicosParameters.setRealTimeScaling(realTimeScaling);
    }

    /**
     * @param solver set solver
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setSolver(double solver) throws PropertyVetoException {
    	scicosParameters.setSolver(solver);
    }

    /**
     * @param toleranceOnTime set tolerance time
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setToleranceOnTime(double toleranceOnTime) throws PropertyVetoException {
		scicosParameters.setToleranceOnTime(toleranceOnTime);
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
    }

    /**
     * Set menu used to manage Grid visibility
     * @param menu the menu
     */
    public void setGridMenuItem(CheckBoxMenuItem menu) {
	this.gridMenu = menu;
    }

	/**
	 * Close the current diagram.
	 * 
	 * This operation doesn't remove the diagram instance from the Xcos session.
	 * To completely close a diagram please use
	 * {@link Xcos#close(XcosDiagram, boolean)} instead.
	 * 
	 * @param force
	 *            true if the close must be forced.
	 * @return status of the close operation (true if the close is successful,
	 *         false otherwise)
	 */
	public boolean close(boolean force) {

		if (!canClose()) {
			close();
			return false;
		}

		boolean wantToClose = true;
		if (isModified()) {
			// Ask the user want he want to do !
			final AnswerOption answer;
			if (force) {
				answer = ScilabModalDialog.show(getParentTab(),
						XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS,
						IconType.QUESTION_ICON, ButtonType.YES_NO);
			} else {
				answer = ScilabModalDialog.show(getParentTab(),
						XcosMessages.DIAGRAM_MODIFIED, XcosMessages.XCOS,
						IconType.QUESTION_ICON, ButtonType.YES_NO_CANCEL);
			}

			switch (answer) {
			case YES_OPTION:
				if (!saveDiagram()) {
					// if save is canceled, cancel close windows
					wantToClose = false;
				}
				break;
			case CANCEL_OPTION:
				wantToClose = false;
				break;
			case NO_OPTION:
			default:
				break;
			}
		}

		if (wantToClose) {
			close();
		}
		
		return true;
	}

	/**
	 * Close the current diagram without prompting anything
	 */
	private void close() {
		if (getParentTab() != null) {
			getParentTab().close();
			setParentTab(null);
		}
		
		if (viewPort != null) {
		    viewPort.close();
		    viewPort = null;
		}
	}

    /**
     * @return save status
     */
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

    /**
     * @param fileName diagram filename
     * @return save status
     */
    public boolean saveDiagramAs(String fileName) {

	boolean isSuccess = false;
	String writeFile = fileName;
	info(XcosMessages.SAVING_DIAGRAM);
	if (fileName == null) {
	    // Choose a filename
	    SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
	    fc.setTitle(XcosMessages.SAVE_AS);
	    fc.setUiDialogType(JFileChooser.SAVE_DIALOG);
	    fc.setMultipleSelection(false);
	    if (this.getSavedFile() != null) {
		fc.setSelectedFile(new File(this.getSavedFile()));
	    }

	    SciFileFilter xcosFilter = new SciFileFilter("*.xcos", null, 0);
	    SciFileFilter allFilter = new SciFileFilter("*.*", null, 1);
	    fc.addChoosableFileFilter(xcosFilter);
	    fc.addChoosableFileFilter(allFilter);
	    fc.setFileFilter(xcosFilter);

	    fc.setAcceptAllFileFilterUsed(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		info(XcosMessages.EMPTY_INFO);
		return isSuccess;
	    }
	    writeFile = fc.getSelection()[0];
	}
	/* Extension checks */
	File file = new File(writeFile);
	if (!file.exists()) {
	    String extension = writeFile.substring(writeFile.lastIndexOf('.') + 1);

	    if (extension.equals(writeFile)) {
		/* No extension given --> .xcos added */
		writeFile += ".xcos";
	    }
	}

	XcosCodec codec = new XcosCodec();
	String xml = mxUtils.getXml(codec.encode(this));

	try {
	    mxUtils.writeFile(xml, writeFile);
	    isSuccess = true;
	} catch (IOException e1) {
	    e1.printStackTrace();
	    isSuccess = false;
	}

	if (isSuccess) {
	    this.setSavedFile(writeFile);
	    File theFile = new File(writeFile);
	    setTitle(theFile.getName().substring(0, theFile.getName().lastIndexOf('.')));
	    ConfigurationManager.getInstance().addToRecentFiles(writeFile);
	    ConfigurationManager.getInstance().saveConfig();
	    setModified(false);
	} else {
	    XcosDialogs.couldNotSaveFile(this);
	}
	info(XcosMessages.EMPTY_INFO);
	return isSuccess;
    }

    /**
     * Set the title of the diagram
     * @param title the title
     * @see org.scilab.modules.graph.ScilabGraph#setTitle(java.lang.String)
     */
    @Override
    public void setTitle(String title) {
	super.setTitle(title);
	updateTabTitle();
    }

    /**
     * Update the title
     */
    public void updateTabTitle() {
	String tabTitle = !isModified() ? getTitle() : "* " + getTitle();
	if (getParentTab() != null) {
	    getParentTab().setName(tabTitle);
	    getParentTab().draw();
	}
    }
    
    /**
     * @param context set context
     * @throws PropertyVetoException when the new value is invalid 
     */
    public void setContext(String[] context) throws PropertyVetoException {
    	scicosParameters.setContext(context);
    	fireEvent(new mxEventObject(XcosEvent.DIAGRAM_UPDATED));
	updateCellsContext();
    }

    /**
     * Update context value in diagram children
     */
    public void updateCellsContext() {
	Object rootParent = getDefaultParent();
	int childCount = getModel().getChildCount(rootParent);
	for (int i = 0; i < childCount; ++i) {
	    Object obj = getModel().getChildAt(rootParent, i);
	    if (obj instanceof ContextUpdate) {
		String[] globalContext = getScicosParameters().getContext();

		/* Determine if the context is not empty */
		int nbOfDetectedChar = 0;
		for (int j = 0; j < globalContext.length; j++) {
		    globalContext[j] = globalContext[j].replaceFirst("\\s", "");
		    nbOfDetectedChar += globalContext[j].length();
		    if (nbOfDetectedChar != 0) {
			break;
		    }
		}

		if (nbOfDetectedChar != 0) {
		    ((ContextUpdate) obj).onContextChange(globalContext);
		}

	    } else if (obj instanceof SuperBlock) {
		SuperBlock superBlock = (SuperBlock) obj;
		if (superBlock.getChild() != null) {
		    superBlock.getChild().updateCellsContext();
		}
	    }
	}
    }
    
    /**
     * @param debugLevel change debug level
     * @category XMLCompatibility
     * @throws PropertyVetoException when the new value is invalid
     */
    public void setDebugLevel(int debugLevel) throws PropertyVetoException {
    	scicosParameters.setDebugLevel(debugLevel);
    }

    /**
     * Read a diagram from an HDF5 file (ask for creation if the file does not exist) 
     * @param diagramFileName file to open
     */
    public void openDiagramFromFile(String diagramFileName) {
	    File theFile = new File(diagramFileName);
	    info(XcosMessages.LOADING_DIAGRAM);

	    if (theFile.exists()) {
	    	transformAndLoadFile(theFile, false);
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
	    info(XcosMessages.EMPTY_INFO);
	    getUndoManager().clear();
    }
    

    /**
     * Load a file with different method depending on it extension 
     * @param theFile File to load
     * @param wait wait end transform 
     */
    protected boolean transformAndLoadFile(File theFile, boolean wait) {
	final File fileToLoad = theFile;
	final XcosFileType filetype = XcosFileType.findFileType(fileToLoad);
	boolean result = false;

	if (!fileToLoad.exists()) {
		XcosDialogs.couldNotLoadFile(this);
		return false;
	}
	
	switch (filetype) {
	case COSF:
	case COS:
	    if (wait) {
		File newFile;
		newFile = filetype.exportToHdf5(fileToLoad);
		result = transformAndLoadFile(newFile, wait);
	    } else {
		Thread transformAction = new Thread() {
			@Override
		    public void run() {
			File newFile;
			newFile = filetype.exportToHdf5(fileToLoad);
			transformAndLoadFile(newFile, false);
		    }
		};
		transformAction.start();
		result = true;
	    }
	    break;

	case XCOS:
	    Document document = loadXcosDocument(theFile.getAbsolutePath());
	    if (document == null) {
		XcosDialogs.couldNotLoadFile(this);
		return false;
	    }

	    XcosCodec codec = new XcosCodec(document);

	    if (getModel().getChildCount(getDefaultParent()) == 0) {
		codec.decode(document.getDocumentElement(), this);
		setModified(false);
		setSavedFile(theFile.getAbsolutePath());
		setTitle(theFile.getName().substring(0,	theFile.getName().lastIndexOf('.')));
		setChildrenParentDiagram();
		generateUID();
	    } else {
	    	info(XcosMessages.LOADING_DIAGRAM);
		XcosDiagram xcosDiagram = new XcosDiagram();
		codec.decode(document.getDocumentElement(), xcosDiagram);
		xcosDiagram.setModified(false);
		xcosDiagram.setSavedFile(theFile.getAbsolutePath());
		xcosDiagram.setTitle(theFile.getName().substring(0,	theFile.getName().lastIndexOf('.')));
		xcosDiagram.setChildrenParentDiagram();
		xcosDiagram.generateUID();
		new XcosTab(xcosDiagram).setVisible(true);
		info(XcosMessages.EMPTY_INFO);
	    }
	    
	    result = true;
	    break;

	case HDF5:
	    H5RWHandler handler = new H5RWHandler(fileToLoad);
	    handler.readDiagram(this);
	    generateUID();
	    setModified(false);
	    result = true;
	    break;

	default:
	    XcosDialogs.couldNotLoadFile(this);
	    break;
	}
	return result;
    }

    /**
     * @param xcosFile xcos file
     * @return opened document
     */
    static Document loadXcosDocument(String xcosFile) {
	DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
	DocumentBuilder docBuilder;
	try {
	    docBuilder = docBuilderFactory.newDocumentBuilder();
	    return docBuilder.parse(xcosFile);
	} catch (ParserConfigurationException e) {
	    return null;
	} catch (SAXException e) {
	    return null;
	} catch (IOException e) {
	    return null;
	}
    }
    
    /**
     * generate unique id to all blocks in diagram
     */
    public void generateUID() {
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
	    if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
		BasicBlock block = (BasicBlock) getModel().getChildAt(getDefaultParent(), i);
		if (block.getRealParameters() instanceof ScilabMList) {
		    //we have a hidden SuperBlock, create a real one
		    SuperBlock newSP = (SuperBlock) BlockFactory.createBlock("SUPER_f");
		    newSP.setRealParameters(block.getRealParameters());
		    newSP.createChildDiagram(true);
		    newSP.setParentDiagram(this);
		    block.setRealParameters(new DiagramElement().encode(newSP.getChild()));
		} else if (block.getId() == null || block.getId().compareTo("") == 0) {
		    block.generateId();
		}
	    }
	}
    }

    /**
     * Update all the children of the current graph.
     */
    public void setChildrenParentDiagram() {
    	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
    	    mxCell cell = (mxCell) getModel().getChildAt(getDefaultParent(), i);
    	    if (cell instanceof BasicBlock) {
    		BasicBlock block = (BasicBlock) cell;
    		block.setParentDiagram(this);
    	    }
    	}
    }

    /**
     * For each block in the argument, call its setParentDiagram method
     * @param diagram The new parent of the blocks.
     * 
     * @deprecated please use {@link #setChildrenParentDiagram()} instead
     */
    @Deprecated
    private void setChildrenParentDiagram(XcosDiagram diagram) {
    	diagram.setChildrenParentDiagram();
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
     * @param cell block
     * @return cell tooltip
     */
	@Override
    public String getToolTipForCell(Object cell) {
	if (cell instanceof BasicBlock) {
	    return ((BasicBlock) cell).getToolTipText();
	} else if (cell instanceof BasicPort) {
	    return ((BasicPort) cell).getToolTipText();
	}
	return "";
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
		    if (message.compareTo("") != 0) {
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
	@Override
    public void setModified(boolean modified) {
	super.setModified(modified);
	updateTabTitle();
    }
    
    /**
     * Evaluate the current context
     * 
     * @return The resulting data. Keys are variable names and Values are 
     *         evaluated values. 
     */
	public Map<String, String> evaluateContext() {
		Map<String, String> result = null;
		
		try {
			StringBuilder str = new StringBuilder();
			str.append('[');
			for (String s : getScicosParameters().getContext()) {
				str.append('\"');
				str.append(s);
				str.append("\" ");
			}
			str.append(']');
				
			final File temp = FileUtils.createTempFile();
			
			ScilabInterpreterManagement.synchronousScilabExec(
						  "vars = script2var(" + str.toString() + ", struct());"
						+ "export_to_hdf5('" + temp.getAbsolutePath() + "', 'vars');");
			
			result = new H5RWHandler(temp).readContext();
		} catch (IOException e) {
			info("Unable to create file");
			e.printStackTrace();
		} catch (InterpreterException e) {
			info("Unable to evaluate the contexte");
			e.printStackTrace();
		}
		
		return result;
	}
    
    /**
     * @param uid block ID
     * @return block
     */
    public BasicBlock getChildById(String uid) {
	BasicBlock returnBlock = null;
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
	    if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
		BasicBlock block = (BasicBlock) getModel().getChildAt(getDefaultParent(), i);
		if (block.getId().compareTo(uid) == 0) { //find it
		    returnBlock = block;
		} else {
		    if (block instanceof SuperBlock) {
			boolean created = false;
			if (((SuperBlock) block).getChild() == null) { 
			    //create temporary SuperBlock to find child
			    ((SuperBlock) block).createChildDiagram();
			    created = true;
			}

			//search in child
			returnBlock = ((SuperBlock) block).getChild().getChildById(uid);

			if (created) { //if temporary, destroy it
			    ((SuperBlock) block).closeBlockSettings();
			}
		    } else if (block.getRealParameters() instanceof ScilabMList) { 
			//we have a hidden SuperBlock, create a real one
			SuperBlock newSP = (SuperBlock) BlockFactory.createBlock("SUPER_f");
			newSP.setParentDiagram(block.getParentDiagram());
			newSP.setRealParameters(block.getRealParameters());
			newSP.createChildDiagram();
			//search in child
			returnBlock = newSP.getChild().getChildById(uid);
			newSP.closeBlockSettings();
			newSP = null;
		    }
		}
	    }

	    if (returnBlock != null) {
		return returnBlock;
	    }
	}
	return returnBlock;
    }

    /**
     * @return child visibility
     */
	public boolean isChildVisible() {
		for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
			Object child = getModel().getChildAt(getDefaultParent(), i);
			if (child instanceof SuperBlock) {
				XcosDiagram diag = ((SuperBlock) child).getChild();
				if (diag != null && (diag.isChildVisible() || diag.isVisible())) {
					// if child or sub child is visible
					return true;
				}
			}
		}
		return false;
	}

    /**
     * @return close capability
     */
    public boolean canClose() {
	if (!isChildVisible()) {
	    return true;
	}
	return false;
    }

    /**
     * close children diagram
     */
    public void closeChildren() {
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
	    Object child = getModel().getChildAt(getDefaultParent(), i);
	    if (child instanceof SuperBlock) {
		SuperBlock diag = (SuperBlock) child;

		if (diag.getChild() != null) {
		    diag.closeBlockSettings();
		}
	    }
	}
    }
    
    /**
     * @param edge new edge
     * @param source egde source
     * @param target edge target
     * @return string error
     */
    private StringBuffer checkMultiplicities(Object edge, Object source, Object target) {
	mxMultiplicity[] multi = getMultiplicities();
	StringBuffer error = new StringBuffer();
	for (mxMultiplicity current : multi) {
	    if (current instanceof PortCheck) {
		int sourceOut = mxGraphModel.getDirectedEdgeCount(getModel(), source, true);
		int targetIn = mxGraphModel.getDirectedEdgeCount(getModel(), target, false);
		
		String str = ((PortCheck) current).checkDrawLink(source, target, sourceOut, targetIn);
		if (str != null) {
		    error.append(str);
		}
	    }
	}
	
	if (error.length() > 0) {
	    return error;
	}
	return null;
    }

    /**
     * Check the direction of an edge.
     * 
     * @param source edge source
     * @param target edge target
     * @return true, if the two parameters are in the right order, false if 
     *         they must be inverted.
     */
    private boolean checkEdgeDirection(Object source, Object target) {
		if (source instanceof InputPort && target instanceof OutputPort) {
		    return false;
		}
	
		if (source instanceof ControlPort && target instanceof CommandPort) {
		    return false;
		}
	
		if ((source instanceof InputPort || source instanceof ControlPort) &&  target instanceof BasicLink) {
		    return false;
		}
		
		return true;
    }
    
    /**
     * cancel draw link action 
     */
    private void cancelDrawLinkAction() {
	waitPathAddEdge = false;
	waitPathRelease = false;
	drawLink = null;
	info(XcosMessages.EMPTY_INFO);
    }
    
    /**
     * Construct a new selection model used on this graph.
     * 
     * @return a new selection model instance.
     * @see com.mxgraph.view.mxGraph#createSelectionModel()
     */
    @Override
    protected mxGraphSelectionModel createSelectionModel() {
    	return new mxGraphSelectionModel(this) {
			/**
			 * When we only want to select a cell which is a port, select the
			 * parent block.
			 * 
			 * @param cell the cell
			 * @see com.mxgraph.view.mxGraphSelectionModel#setCell(java.lang.Object)
			 */
    		@Override
    		public void setCell(Object cell) {
    			final Object current;
    			if (cell instanceof BasicPort) {
    				current = getModel().getParent(cell);
    			} else {
    				current = cell;
    			}
    			super.setCell(current);
    		}
    	};
    }
}
