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
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyVetoException;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashSet;
import java.util.IllegalFormatException;
import java.util.LinkedHashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.regex.Pattern;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.filechooser.FileChooser;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.utils.SciFileFilter;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.io.scicos.DiagramElement;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.BasicPort.Type;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.PortCheck;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosFileType;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxGraphModel.Filter;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.model.mxGraphModel.mxStyleChange;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel.mxAtomicGraphModelChange;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.view.mxGraphSelectionModel;
import com.mxgraph.view.mxMultiplicity;
import com.mxgraph.view.mxStylesheet;

/**
 * The base class for a diagram. This class contains jgraphx + Scicos data.
 */
public class XcosDiagram extends ScilabGraph {
	private static final Log LOG = LogFactory.getLog(XcosDiagram.class);
	
	/*
	 * Static helpers
	 */

	/**
	 * Only return the instanceof klass 
	 * 
	 * @param selection the selection to filter out
	 * @param klass the class selector
	 * @return the selection with only klass instance.
	 */
	public static Object[] filterByClass(final Object[] selection, final Class<BasicBlock> klass) {
		return mxGraphModel.filterCells(selection, new mxGraphModel.Filter() {
			@Override
			public boolean filter(Object cell) {
				return klass.isInstance(cell);
			}
		});
	}
	
	/*
	 * diagram data
	 */
	
	// the associated parameters
	private ScicosParameters scicosParameters;
	
	// the scicos engine current status
	private final transient CompilationEngineStatus engine;
	
    //private Window palette;
    private Tab viewPort;

    private CheckBoxMenuItem viewPortMenu;

	/**
	 * Hook method that creates the new edge for insertEdge. This implementation
	 * does not set the source and target of the edge, these are set when the
	 * edge is added to the model.
	 * 
	 * @param parent
	 *			Cell that specifies the parent of the new edge.
	 * @param id
	 *			Optional string that defines the Id of the new edge.
	 * @param value
	 *			Object to be used as the user object.
	 * @param source
	 *			Cell that defines the source of the edge.
	 * @param target
	 *			Cell that defines the target of the edge.
	 * @param style
	 *			Optional string that defines the cell style.
	 * @return Returns the new edge to be inserted.
	 * @see com.mxgraph.view.mxGraph#createEdge(java.lang.Object,
	 *	  java.lang.String, java.lang.Object, java.lang.Object,
	 *	  java.lang.Object, java.lang.String)
	 */
	@Override
	public Object createEdge(Object parent, String id, Object value,
			Object source, Object target, String style) {
		Object ret = null;
		
		if (source instanceof BasicPort) {
			BasicPort src = (BasicPort) source;
			BasicLink link = null;
			
			if (src.getType() == Type.EXPLICIT) {
				link = new ExplicitLink();
			} else if (src.getType() == Type.IMPLICIT) {
				link = new ImplicitLink();
			} else {
				link = new CommandControlLink();
			}
			
			// allocate the associated geometry
			link.setGeometry(new mxGeometry());
			ret = link;
		} else if (source instanceof SplitBlock) {
			SplitBlock src = (SplitBlock) source;
			return createEdge(parent, id, value, src.getIn(), target, style);
		} else if (source instanceof BasicLink) {
			BasicLink src = (BasicLink) source;
			BasicLink link = null;
			
			try {
				link = src.getClass().newInstance();
				
				// allocate the associated geometry
				link.setGeometry(new mxGeometry());
				
			} catch (InstantiationException e) {
				LOG.error(e);
			} catch (IllegalAccessException e) {
				LOG.error(e);
			}
			
			ret = link;
		}
		
		if (ret == null) {
			ret = super.createEdge(parent, id, value, source, target, style);
			LOG.debug("Creating a non typed edge");
		}
		
		return ret;
	}
	
	
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
	public Object addCell(Object cell, Object parent, Integer index,
			Object source, Object target) {
		
		// already connected edge or normal block
		if (source == null && target == null) {
			return super.addCell(cell, parent, index, source, target);
		}
		
    	// Command -> Control
    	if (source instanceof CommandPort && target instanceof ControlPort && cell instanceof CommandControlLink) {
    			return super.addCell(cell, parent, index, source, target);
    	}

    	// Control -> Command
    	// Switch source and target !
    	if (target instanceof CommandPort && source instanceof ControlPort && cell instanceof CommandControlLink) {
    			BasicLink current = (BasicLink) cell;
    			current.invertDirection();
    			
    			return super.addCell(cell, parent, index, target, source);
    	}

    	// ExplicitOutput -> ExplicitInput
    	if (source instanceof ExplicitOutputPort && target instanceof ExplicitInputPort && cell instanceof ExplicitLink) {
    			return super.addCell(cell, parent, index, source, target);
    	}
    	// ExplicitInput -> ExplicitOutput
    	// Switch source and target !
    	if (target instanceof ExplicitOutputPort && source instanceof ExplicitInputPort && cell instanceof ExplicitLink) {
    			BasicLink current = (BasicLink) cell;
    			current.invertDirection();
				
    			return super.addCell(cell, parent, index, target, source);
    	}

    	// ImplicitOutput -> ImplicitInput
    	if (source instanceof ImplicitOutputPort && target instanceof ImplicitInputPort && cell instanceof ImplicitLink) {
    			return super.addCell(cell, parent, index, source, target);
    	}
    	// ImplicitInput -> ImplicitOutput
    	// Switch source and target !
    	if (target instanceof ImplicitOutputPort && source instanceof ImplicitInputPort && cell instanceof ImplicitLink) {
    			BasicLink current = (BasicLink) cell;
    			current.invertDirection();
				
    			return super.addCell(cell, parent, index, target, source);
    	}

    	// ImplicitInput -> ImplicitInput
    	if (source instanceof ImplicitInputPort && target instanceof ImplicitInputPort && cell instanceof ImplicitLink) {
    			return super.addCell(cell, parent, index, source, target);
    	}
    	// ImplicitOutputPort -> ImplicitOutput
    	// Switch source and target !
    	if (target instanceof ImplicitOutputPort && source instanceof ImplicitOutputPort && cell instanceof ImplicitLink) {
    			BasicLink current = (BasicLink) cell;
    			current.invertDirection();
				
    			return super.addCell(cell, parent, index, target, source);
    	}
    	
    	/*
    	 * Split management
    	 */
    	
    	// ExplicitLink -> ExplicitInputPort
    	if (source instanceof ExplicitLink && target instanceof ExplicitInputPort && cell instanceof ExplicitLink) {
    			SplitBlock split = addSplitEdge(((BasicLink) cell).getGeometry().getSourcePoint(), (BasicLink) source);
    			return addCell(cell, parent, index, split.getOut2(), target);
    	}
    	// ExplicitOutput -> ExpliciLink
    	// Switch source and target !
    	if (target instanceof ExplicitLink && source instanceof ExplicitInputPort && cell instanceof ExplicitLink) {
    			final BasicLink current = (BasicLink) cell;
    			final SplitBlock split = addSplitEdge(current.getGeometry().getTargetPoint(), (BasicLink) target);
    			
    			current.invertDirection();
    			
    			return addCell(cell, parent, index, split.getOut2(), source);
    	}

    	// ImplicitLink -> ImplicitInputPort
    	if (source instanceof ImplicitLink && target instanceof ImplicitInputPort && cell instanceof ImplicitLink) {
    			SplitBlock split = addSplitEdge(((BasicLink) cell).getGeometry().getSourcePoint(), (BasicLink) source);
    			return addCell(cell, parent, index, split.getOut2(), target);
    	}
    	// ImplicitInputPort -> ImplicitLink
    	// Switch source and target !
    	if (target instanceof ImplicitLink && source instanceof ImplicitInputPort && cell instanceof ImplicitLink) {
    			final BasicLink current = (BasicLink) cell;
    			final SplitBlock split = addSplitEdge(current.getGeometry().getTargetPoint(), (BasicLink) target);
    			
    			current.invertDirection();
    			
    			return addCell(cell, parent, index, split.getOut2(), source);
    	}
    	
    	// ImplicitLink -> ImplicitOutputPort
    	if (source instanceof ImplicitLink && target instanceof ImplicitOutputPort && cell instanceof ImplicitLink) {
    		final BasicLink current = (BasicLink) cell;
			final SplitBlock split = addSplitEdge(current.getGeometry().getTargetPoint(), (BasicLink) source);
			return addCell(cell, parent, index, split.getOut2(), source);
    	}
    	// ImplicitOutputPort -> ImplicitLink
    	// Switch source and target !
    	if (target instanceof ImplicitLink && source instanceof ImplicitOutputPort && cell instanceof ImplicitLink) {
			final BasicLink current = (BasicLink) cell;
			final SplitBlock split = addSplitEdge(current.getGeometry()
					.getTargetPoint(), (ImplicitLink) target);
			return addCell(cell, parent, index, split.getOut2(), source);
    	}

    	// CommandControlLink -> ControlPort
    	if (source instanceof CommandControlLink && target instanceof ControlPort && cell instanceof CommandControlLink) {
    			SplitBlock split = addSplitEdge(((BasicLink) cell).getGeometry().getSourcePoint(), (BasicLink) source);
    			return addCell(cell, parent, index, split.getOut2(), target);
    	}
    	// ControlPort -> CommandControlLink
    	// Switch source and target !
    	if (target instanceof CommandControlLink && source instanceof ControlPort && cell instanceof CommandControlLink) {
    			final BasicLink current = (BasicLink) cell;
    			final SplitBlock split = addSplitEdge(current.getGeometry().getTargetPoint(), (BasicLink) target);
    			
    			current.invertDirection();
    			
    			return addCell(cell, parent, index, split.getOut2(), source);
    	}

    	if (cell instanceof BasicLink && source != null && target != null) {
    		LOG.error("Unable to add a typed link");
    		return null;
    	} else {
    		LOG.error("Adding an untyped edge");
    		return super.addCell(cell, parent, index, source, target);
    	}
    }

    /**
     * Add a split on a edge.
     * 
     * @param splitPoint the split point (center of the split block) 
     * @param link source link
     * @param port target port
     * @return split block
     */
	public SplitBlock addSplitEdge(final mxPoint splitPoint, final BasicLink link) {
		final BasicPort linkSource = (BasicPort) link.getSource();
		final BasicPort linkTarget = (BasicPort) link.getTarget();
		
		final SplitBlock splitBlock = (SplitBlock) BlockFactory
				.createBlock(BlockInterFunction.SPLIT_f);
		
		getModel().beginUpdate();
		try {
			// Origin of the parent, (0,0) as default may be different in case
			mxPoint orig = link.getParent().getGeometry();
			if (orig == null) {
				orig = new mxPoint();
			}
			
			splitBlock.addConnection(linkSource);

			addCell(splitBlock);
			// force resize and align on the grid
			resizeCell(splitBlock, new mxRectangle(splitPoint.getX(), splitPoint.getY(), 0, 0));

			// Update old link

			// get breaking segment and related point
			mxPoint splitTr = new mxPoint(splitPoint.getX() - orig.getX(), splitPoint.getY() - orig.getY());
			final int pos = link.findNearestSegment(splitTr);

			// save points after breaking point
			final List<mxPoint> saveStartPoints = link.getPoints(pos, true);
			final List<mxPoint> saveEndPoints = link.getPoints(pos, false);

			// remove the first end point if the position is near the split
			// position
			if (saveEndPoints.size() > 0) {
				final mxPoint p = saveEndPoints.get(0);
				final double dx = p.getX() - splitTr.getX();
				final double dy = p.getY() - splitTr.getY();
				
				if (!getAsComponent().isSignificant(dx, dy)) {
					saveEndPoints.remove(0);
				}
			}
			
			// disable events
			getModel().beginUpdate();
			getModel().remove(link);
			getModel().endUpdate();

			connect(linkSource, splitBlock.getIn(), saveStartPoints, orig);
			connect(splitBlock.getOut1(), linkTarget, saveEndPoints, orig);

			refresh();
		} finally {
			getModel().endUpdate();
		}

		return splitBlock;
	}

	/**
	 * Connect two port together with the associated points.
	 * 
	 * This method perform the connection in two step in order to generate the
	 * right UndoableChangeEdits.
	 * 
	 * @param src
	 *            the source port
	 * @param trg
	 *            the target port
	 * @param points
	 *            the points
	 * @param the origin point (may be (0,0))
	 */
	public void connect(BasicPort src, BasicPort trg, List<mxPoint> points, mxPoint orig) {
		/*
		 * Add the link with a default geometry
		 */
		final Object newLink1 = createEdge(null, null, null, src, trg, null);
		addCell(newLink1, null, null, src, trg);
		{
			final mxGeometry geometry = getModel().getGeometry(newLink1);
			geometry.setPoints(points);
			getModel().setGeometry(newLink1, geometry);
		}

		/*
		 * Update the geometry
		 */
		// should be cloned to generate an event
		final mxGeometry geometry = (mxGeometry) getModel().getGeometry(newLink1).clone();
		final double dx = orig.getX();
		final double dy = orig.getY();
		
		geometry.translate(dx, dy);
		getModel().setGeometry(newLink1, geometry);
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
		public void propertyChange(final PropertyChangeEvent evt) {
			setModified(true);
		}
	});

	setComponent(new GraphComponent(this));
	initComponent();
	installStylesheet();

	// Forbid disconnecting cells once it is connected.
	setCellsDisconnectable(false);

	// Forbid pending edges.
	setAllowDanglingEdges(false);

	// Cannot connect port to itself.
	setAllowLoops(false);

	// Override isCellResizable to filter what the user can resize
	setCellsResizable(true);

	/* Labels use HTML if not equal to interface function name */
	setHtmlLabels(true);
	/*
	 * by default every label is movable, see
	 * XcosDiagram##isLabelMovable(java.lang.Object) for restrictions
	 */
	setVertexLabelsMovable(true);
	setEdgeLabelsMovable(true);
	
	// 
	setCloneInvalidEdges(true);

	// Override isCellEditable to filter what the user can edit
	setCellsEditable(true);

	setConnectableEdges(true);
	
	// Do not clear edge points on connect
	setResetEdgesOnConnect(false);

	setMultiplicities();
	
	// Add a listener to track when model is changed
	getModel().addListener(mxEvent.CHANGE, new ModelTracker());
	
	((mxCell) getDefaultParent()).setId((new UID()).toString());
	((mxCell) getModel().getRoot()).setId((new UID()).toString());
    }

    /**
     * Initialize component settings for a graph.
     * 
     * This method *must* be used to setup the component after any
     * reassociation.
     */
    public void initComponent() {
    	getAsComponent().setToolTips(true);
    	
    	// This enable stop editing cells when pressing Enter.
    	getAsComponent().setEnterStopsCellEditing(false);
    	
    	getAsComponent().setTolerance(1);
    	
    	getAsComponent().getViewport().setOpaque(false);
    	getAsComponent().setBackground(Color.WHITE);
    	
    	setGridVisible(true);
    	
    	/*
    	 * Reinstall related listeners
    	 */
    	
    	// Property change Listener
    	// Will say if a diagram has been modified or not.
    	getAsComponent().addPropertyChangeListener(new PropertyChangeListener() {
    	    @Override
			public void propertyChange(final PropertyChangeEvent e) {
    		if (e.getPropertyName().compareTo("modified") == 0) {
    		    if (!e.getOldValue().equals(e.getNewValue())) {
    			updateTabTitle();
    		    }
    		}
    	    }
    	});
    }
    
	/**
	 * Install the default style sheet and the user stylesheet on the diagram.
	 */
	public void installStylesheet() {
		final mxStylesheet styleSheet = Xcos.getInstance().getStyleSheet();
		setStylesheet(styleSheet);
	}
    
    /**
     * Install the multiplicities (use for link checking)
     */
    private void setMultiplicities() {
	final mxMultiplicity[] multiplicities = new mxMultiplicity[10];

	
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

	// Track when superblock ask a parent refresh.
	addListener(XcosEvent.SUPER_BLOCK_UPDATED, new SuperBlockUpdateTracker()); 
	
	// Track when cells are added.
	addListener(mxEvent.CELLS_ADDED, CellAddedTracker.getInstance()); 
	addListener(mxEvent.CELLS_ADDED, getEngine());
	
	// Track when cells are deleted.
	addListener(mxEvent.CELLS_REMOVED, getEngine());
	
	// Track when resizing a cell.
	addListener(mxEvent.CELLS_RESIZED, new CellResizedTracker());
	
	// Track when we have to force a Block value
	addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, getEngine());
	
	// Update the blocks view on undo/redo
	getUndoManager().addListener(mxEvent.UNDO, new UndoUpdateTracker());
	getUndoManager().addListener(mxEvent.REDO, new UndoUpdateTracker());
	
	addListener(XcosEvent.ADD_PORTS, new mxIEventListener() {
	    @Override
		public void invoke(final Object source, final mxEventObject evt) {
		getModel().beginUpdate();
		final BasicBlock updatedBlock = (BasicBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
		BlockPositioning.updateBlockView(updatedBlock);
		getView().clear(updatedBlock, true, true);
		getView().validate();
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
	@Override
	public void invoke(final Object source, final mxEventObject evt) {
	    final List<mxAtomicGraphModelChange> changes = (List<mxAtomicGraphModelChange>) (evt.getProperty("changes"));
	    final List<Object> objects = new ArrayList<Object>();
	    getModel().beginUpdate();
	    for (int i = 0; i < changes.size(); ++i) {
		if (changes.get(i) instanceof mxChildChange) {
		    if (((mxChildChange) changes.get(i)).getChild() instanceof SplitBlock) {
			continue;
		    }

		    if (((mxChildChange) changes.get(i)).getChild() instanceof BasicBlock) {
			final BasicBlock currentCell = (BasicBlock) ((mxChildChange) changes.get(i)).getChild();
			objects.add(currentCell);
		    }
		}
	    }
	    if (!objects.isEmpty()) {
		final Object[] firedCells = new Object[objects.size()];
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
	@Override
	public void invoke(final Object source, final mxEventObject evt) {
	    assert evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED) instanceof SuperBlock;
	    final SuperBlock updatedBlock = (SuperBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
	    updatedBlock.setRealParameters(new DiagramElement().encode(updatedBlock.getChild()));
	    if (updatedBlock.getParentDiagram() instanceof SuperBlockDiagram) {
		final SuperBlock parentBlock = ((SuperBlockDiagram) updatedBlock
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
    	private static CellAddedTracker instance;
    	/**
    	 * Default constructor
    	 */
    	private CellAddedTracker() { }

    	/**
		 * @return the instance
		 */
		public static synchronized CellAddedTracker getInstance() {
			if (instance == null) {
				instance = new CellAddedTracker();
			}
			return instance;
		}
    	
    	/**
    	 * Update block values on add 
    	 * @param source the source instance
    	 * @param evt the event data
    	 * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object, com.mxgraph.util.mxEventObject)
    	 */
    	@Override
		public void invoke(final Object source, final mxEventObject evt) {
    		final XcosDiagram diagram = (XcosDiagram) source;
    		final Object[] cells = (Object[]) evt.getProperty("cells");
    		
    		diagram.getModel().beginUpdate();
    		
    		final Filter filter = new Filter() {
				@Override
				public boolean filter(Object cell) {
					if (cell instanceof BasicBlock) {
						// Update parent on cell addition
						((BasicBlock) cell).setParentDiagram(diagram);
					}
					return false;
				}
			};
    		
    		for (int i = 0; i < cells.length; ++i) {
    			mxGraphModel.filterDescendants(diagram.getModel(), filter, cells[i]);
    		}
    		
    		diagram.getModel().endUpdate();
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
	@Override
	public void invoke(final Object source, final mxEventObject evt) {
	    final Object[] cells = (Object[]) evt.getProperty("cells");
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
	@Override
	public void invoke(final Object source, final mxEventObject evt) {
            final List<mxUndoableChange> changes = ((mxUndoableEdit) evt.getProperty(ScilabGraphConstants.EVENT_CHANGE_EDIT)).getChanges();
            final Object[] changedCells = getSelectionCellsForChanges(changes);
            getModel().beginUpdate();
            for (final Object object : changedCells) {
		if (object instanceof BasicBlock) {
		    final BasicBlock current = (BasicBlock) object;
		    
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
	 * Removes the given cells from the graph including all connected edges if
	 * includeEdges is true. The change is carried out using cellsRemoved.
	 * 
	 * @param cells the cells to be removed
	 * @param includeEdges true if the edges must be removed, false otherwise.
	 * @return the deleted cells
	 * @see com.mxgraph.view.mxGraph#removeCells(java.lang.Object[], boolean)
	 */
	@Override
	public Object[] removeCells(final Object[] cells, final boolean includeEdges) {
		if (cells == null || cells.length == 0) {
			return super.removeCells(cells, includeEdges);
		}
		
		/*
		 * First remove all links connected to a removed Split if applicable 
		 */
		final Object[] initialCells;
		if (includeEdges) {
			initialCells = addAllEdges(cells);
		} else {
			initialCells = cells;
		}
		
		// stash used on the loop
		final LinkedList<Object> loopCells = new LinkedList<Object>(Arrays.asList(initialCells));
		// the cells that need to be really 
		final HashSet<Object> removedCells = new HashSet<Object>(loopCells);
		// couple of cells to reconnect
		final ArrayList<BasicPort[]> connectedCells = new ArrayList<BasicPort[]>();
		final ArrayList<List<mxPoint>> connectedPoints = new ArrayList<List<mxPoint>>();
		
		/*
		 * Then loop on the algorithm to select the right edges
		 */
		// /!\ not bounded algorithm
		while (loopCells.size() > 0) {
			Object cell = loopCells.pop();
			
			if (cell instanceof BasicLink) {
				/*
				 * Continue on non fully connected links
				 */
				if (((BasicLink) cell).getSource() == null) {
					continue;
				}
				if (((BasicLink) cell).getTarget() == null) {
					continue;
				}
				
				/*
				 * Add any split to a link
				 */
				addTerminalParent(((BasicLink) cell).getSource(), removedCells, loopCells);
				addTerminalParent(((BasicLink) cell).getTarget(), removedCells, loopCells);
				
			} else if (cell instanceof SplitBlock) {
				final SplitBlock splitBlock = (SplitBlock) cell;
				
				/*
				 * Remove related connection or not and reconnect.
				 */
				
				if (splitBlock.getIn().getEdgeCount() == 0
						|| splitBlock.getOut1().getEdgeCount() == 0
						|| splitBlock.getOut2().getEdgeCount() == 0) {
					// corner case, all links will be removed
					continue;
				}
				
				final mxICell inLink = splitBlock.getIn().getEdgeAt(0);
				final mxICell out1Link = splitBlock.getOut1().getEdgeAt(0);
				final mxICell out2Link = splitBlock.getOut2().getEdgeAt(0);
				
				final boolean inRemoved = removedCells.contains(inLink);
				final boolean out1Removed = removedCells.contains(out1Link);
				final boolean out2Removed = removedCells.contains(out2Link);
				
				/*
				 * Explicit case, if the in link is deleted; all the out links also should.
				 */
				if (inLink instanceof ExplicitLink && inRemoved) {
					if (removedCells.add(out1Link)) {
						loopCells.add(out1Link);
					}
					
					if (removedCells.add(out2Link)) {
						loopCells.add(out2Link);
					}
				}
				
				/*
				 * Global case reconnect if not removed
				 */
				final BasicPort[] connection;
				List<mxPoint> points = null;
				if (!inRemoved && !out1Removed && out2Removed) {
					connection = findTerminals(inLink, out1Link, removedCells);
					points = getDirectPoints(splitBlock, inLink, out1Link);
				} else if (!inRemoved && out1Removed && !out2Removed) {
					connection = findTerminals(inLink, out2Link, removedCells);
					points = getDirectPoints(splitBlock, inLink, out2Link);
				} else if (inRemoved && !out1Removed && !out2Removed) {
					// only implicit case, log otherwise
					if (out1Link instanceof ExplicitLink || out2Link instanceof ExplicitLink) {
						LOG.error("Reconnection failed for explicit links");
						connection = null;
					} else {
						connection = findTerminals(out1Link, out2Link, removedCells);
						points = getDirectPoints(splitBlock, out1Link, out2Link);
					}
				} else {
					connection = null;
				}
				
				if (connection != null) {
					connectedCells.add(connection);
					connectedPoints.add(points);
				}
			}
		}
		
		final Object[] ret;
		getModel().beginUpdate();
		try {
			ret = super.removeCells(removedCells.toArray(), includeEdges);
			for (int i = 0; i < connectedCells.size(); i++) {
				final BasicPort[] connection = connectedCells.get(i);
				final List<mxPoint> points = connectedPoints.get(i);
				connect(connection[0], connection[1], points, new mxPoint());
			}
		} finally {
			getModel().endUpdate();
		}
		return ret;
	}

	/**
	 * Add any terminal parent to the removed cells 
	 * @param terminal the current terminal (instance of BasicPort)
	 * @param removedCells the "to be removed" set
	 * @param loopCells the "while loop" set
	 */
	private final void addTerminalParent(mxICell terminal, Collection<Object> removedCells, Collection<Object> loopCells) {
		assert(terminal == null || terminal instanceof BasicPort);
		assert(removedCells != null);
		assert(loopCells != null);
		
		// getting terminal parent
		mxICell target = null;
		if (terminal != null) {
			target = terminal.getParent();
		} else {
			target = null;
		}
		
		// add target if applicable
		if (target instanceof SplitBlock) {
			if (removedCells.add(target)) {
				loopCells.add(target);
			}
		}
	}
	
	/**
	 * Find the terminals when relinking the 2 links
	 * 
	 * This method ensure that {source, target} are not child of removed blocks.
	 * 
	 * @param linkSource the normal source link
	 * @param linkTerminal the normal target link
	 * @param removedCells the set of removed objects
	 * @return the {source, target} connection
	 */
	private BasicPort[] findTerminals(final mxICell linkSource, final mxICell linkTerminal, final HashSet<Object> removedCells) {
		BasicPort src = (BasicPort) linkSource.getTerminal(true);
		BasicPort tgt = (BasicPort) linkTerminal.getTerminal(false);
		if (linkSource instanceof ImplicitLink) {
			if (removedCells.contains(src.getParent())) {
				src = (BasicPort) linkSource.getTerminal(false);
			}
			if (removedCells.contains(tgt.getParent())) {
				tgt = (BasicPort) linkTerminal.getTerminal(true);
			}
		}
		
		return new BasicPort[] {src, tgt};
	}


	/**
	 * Get the direct points from inLink.getSource() to outLink.getTarget().
	 * 
	 * @param splitBlock the current splitblock (added as a mid-point)
	 * @param inLink the link before the split
	 * @param outLink the link after the split
	 * @return the points
	 */
	private List<mxPoint> getDirectPoints(final SplitBlock splitBlock,
			final mxICell inLink, final mxICell outLink) {
		List<mxPoint> points;
		// add the points before the split
		points = new ArrayList<mxPoint>();
		if (inLink.getGeometry().getPoints() != null) {
			points.addAll(inLink.getGeometry().getPoints());
		}
		
		// add a new point at the split location 
		points.add(new mxPoint(
				snap(splitBlock.getGeometry().getCenterX()),
				snap(splitBlock.getGeometry().getCenterY())));
		
		// add the points after the split
		if (outLink.getGeometry().getPoints() != null) {
			points.addAll(outLink.getGeometry().getPoints());
		}
		
		return points;
	}
	
	/**
	 * Manage Group to be CellFoldable i.e with a (-) to reduce and a (+) to
	 * expand them. Labels (mxCell instance with value) should not have a
	 * visible foldable sign.
	 * 
	 * @param cell
	 *            the selected cell
	 * @param collapse
	 *            the collapse settings
	 * @return always <code>false</code>
	 * @see com.mxgraph.view.mxGraph#isCellFoldable(java.lang.Object, boolean)
	 */
	@Override
	public boolean isCellFoldable(final Object cell, final boolean collapse) {
		return false;
	}

	/**
	 * Not BasicBLock cell have a moveable label.
	 * @param cell the cell
	 * @return true if the corresponding label is moveable 
	 * @see com.mxgraph.view.mxGraph#isLabelMovable(java.lang.Object)
	 */
	@Override
	public boolean isLabelMovable(Object cell) {
		if (cell instanceof BasicBlock) {
			return false;
		} else {
			return true;
		}
	}
	
	/**
	 * Return true if selectable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellSelectable(java.lang.Object)
	 */
	@Override
    public boolean isCellSelectable(final Object cell) {
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
    public boolean isCellMovable(final Object cell) {
	if (cell instanceof BasicPort) {
	    return false;
	}

	boolean movable = false;
	final Object[] cells =  getSelectionCells();

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
    public boolean isCellResizable(final Object cell) {
    	if (cell instanceof SplitBlock) {
    		return false;
    	}
    	return (cell instanceof BasicBlock) && super.isCellResizable(cell);
    }

	/**
	 * A cell is deletable is it is not a locked block or an identifier cell 
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellDeletable(java.lang.Object)
	 */
	@Override
    public boolean isCellDeletable(final Object cell) {
    	if (cell instanceof BasicBlock && ((BasicBlock) cell).isLocked()) {
    		return false;
    	}
    	
    	return !cell.getClass().equals(mxCell.class) && super.isCellDeletable(cell);
    }

	/**
	 * Return true if editable
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isCellEditable(java.lang.Object)
	 */
	@Override
    public boolean isCellEditable(final Object cell) {
    	return (cell instanceof TextBlock) && super.isCellDeletable(cell);
    }
	
	/**
	 * Get the label for the cell according to its style. 
	 * 
	 * @param cell the cell object
	 * @return a representative the string (block name) or a style specific style.
	 * @see com.mxgraph.view.mxGraph#convertValueToString(java.lang.Object)
	 */
	@Override
	public String convertValueToString(Object cell) {
		if (cell != null) {
			final Map<String, Object> style = getCellStyle(cell);
			
			final String customLabel = (String) style.get("displayedLabel");
			if (customLabel != null && cell instanceof BasicBlock) {
				try {
					return String.format(customLabel, ((BasicBlock) cell).getExprsFormat());
				} catch (IllegalFormatException e) {
					LOG.error(e);
				}
			} else {
				final String label = super.convertValueToString(cell);
				if (label.isEmpty() && cell instanceof BasicBlock) {
					return ((BasicBlock) cell).getInterfaceFunctionName();
				}
				return label;
			}
			
		}
		return null;
	}
	
	/**
	 * Return true if auto sized
	 * @param cell the cell
	 * @return status
	 * @see com.mxgraph.view.mxGraph#isAutoSizeCell(java.lang.Object)
	 */
	@Override
    public boolean isAutoSizeCell(final Object cell) {
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
	 * {@inheritDoc}
	 * Do not extends if the port position is north or south.
	 */
	@Override
	public boolean isExtendParent(Object cell) {
		final boolean extendsParents;
		
		if (cell instanceof BasicPort) {
			final BasicPort p = (BasicPort) cell;
			extendsParents = !(p.getOrientation() == Orientation.NORTH || p
					.getOrientation() == Orientation.SOUTH)
					&& super.isExtendParent(p);
		} else {
			extendsParents = super.isExtendParent(cell);
		}
		return extendsParents;
	}
	
    /**
     * @param fileName HDF5 filename
     */
    public void dumpToHdf5File(final File fileName) {
    	File writeFile = fileName;
	if (fileName == null) {
	    final FileChooser fc = ScilabFileChooser.createFileChooser();
	    if (getSavedFile() != null) {
	    	try {
				fc.setInitialDirectory(getSavedFile().getCanonicalPath());
			} catch (final IOException e) {
				LOG.error(e);
			}
	    }
	    fc.setMultipleSelection(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		return;
	    }
	    writeFile = new File(fc.getSelection()[0]);
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
	public void setScicosParameters(final ScicosParameters scicosParameters) {
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
    public void setFinalIntegrationTime(final double finalIntegrationTime) throws PropertyVetoException {
    	scicosParameters.setFinalIntegrationTime(finalIntegrationTime);
    }

    /**
     * @param integratorAbsoluteTolerance set integrator absolute tolerance
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorAbsoluteTolerance(final double integratorAbsoluteTolerance) throws PropertyVetoException {
		scicosParameters.setIntegratorAbsoluteTolerance(integratorAbsoluteTolerance);
    }

    /**
     * @param integratorRelativeTolerance integrator relative tolerance
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorRelativeTolerance(final double integratorRelativeTolerance) throws PropertyVetoException {
    	scicosParameters.setIntegratorRelativeTolerance(integratorRelativeTolerance);
    }

    /**
     * @param maximumStepSize set max step size
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaximumStepSize(final double maximumStepSize) throws PropertyVetoException {
    	scicosParameters.setMaximumStepSize(maximumStepSize);
    }

    /**
     * @param maxIntegrationTimeinterval set max integration time
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaxIntegrationTimeinterval(final double maxIntegrationTimeinterval) throws PropertyVetoException {
	 scicosParameters.setMaxIntegrationTimeInterval(maxIntegrationTimeinterval);
    }

    /**
     * @param realTimeScaling set real time scaling
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setRealTimeScaling(final double realTimeScaling) throws PropertyVetoException {
    	scicosParameters.setRealTimeScaling(realTimeScaling);
    }

    /**
     * @param solver set solver
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setSolver(final double solver) throws PropertyVetoException {
    	scicosParameters.setSolver(solver);
    }

    /**
     * @param toleranceOnTime set tolerance time
     * @throws PropertyVetoException when the value is invalid
     * @category XMLCompatibility
     */
    public void setToleranceOnTime(final double toleranceOnTime) throws PropertyVetoException {
		scicosParameters.setToleranceOnTime(toleranceOnTime);
    }

    /**
     * Get the current diagram context
     * @return the context at the current node
     */
    public String[] getContext() {
		return scicosParameters.getContext();
    }
    
    /**
     * Set the associated ViewPort
     * @param viewPort the Viewport
     */
    public void setViewPort(final Tab viewPort) {
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
    public void setViewPortVisible(final boolean status) {
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
    public void setViewPortMenuItem(final CheckBoxMenuItem menu) {
	viewPortMenu = menu;
    }
    /**
     * Manage the visibility of the grid and the associated menu
     * @param status new status
     */
    public void setGridVisible(final boolean status) {
	setGridEnabled(status);
	getAsComponent().setGridVisible(status);
	getAsComponent().repaint();
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
	public boolean close(final boolean force) {

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
		
		return wantToClose;
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
    public boolean saveDiagramAs(final File fileName) {
	boolean isSuccess = false;
	File writeFile = fileName; 
	info(XcosMessages.SAVING_DIAGRAM);
	if (fileName == null) {
	    // Choose a filename
	    final SwingScilabFileChooser fc = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
	    fc.setTitle(XcosMessages.SAVE_AS);
	    fc.setUiDialogType(JFileChooser.SAVE_DIALOG);
	    fc.setMultipleSelection(false);
	    if (getSavedFile() != null) {
	    	fc.setSelectedFile(getSavedFile());
	    } else {
	    	final String title = getTitle();
	    	if (title != null) {
	    		fc.setSelectedFile(new File(title + XcosFileType.XCOS.getDottedExtension()));
	    	}
	    }

	    final SciFileFilter xcosFilter = new SciFileFilter("*.xcos", null, 0);
	    final SciFileFilter allFilter = new SciFileFilter("*.*", null, 1);
	    fc.addChoosableFileFilter(xcosFilter);
	    fc.addChoosableFileFilter(allFilter);
	    fc.setFileFilter(xcosFilter);

	    fc.setAcceptAllFileFilterUsed(false);
	    fc.displayAndWait();

	    if (fc.getSelection() == null || fc.getSelection().length == 0 || fc.getSelection()[0].equals("")) {
		info(XcosMessages.EMPTY_INFO);
		return isSuccess;
	    }
	    writeFile = new File(fc.getSelection()[0]);
	}
	
	/* Extension checks */
	if (!writeFile.exists()) {
		final String filename = writeFile.getName();
	    if (!filename.endsWith(XcosFileType.XCOS.getDottedExtension())) {
	    	/* No extension given --> .xcos added */
	    	writeFile = new File(writeFile.getParent(), filename + XcosFileType.XCOS.getDottedExtension());
	    }
	}
	
	try {
		save(writeFile);
	    setSavedFile(writeFile);
	    
	    setTitle(writeFile.getName().substring(0, writeFile.getName().lastIndexOf('.')));
	    ConfigurationManager.getInstance().addToRecentFiles(writeFile);
	    setModified(false);
	    isSuccess = true;
	} catch (final TransformerException e) {
		LogFactory.getLog(XcosDiagram.class).error(e);
		XcosDialogs.couldNotSaveFile(this);
	}
	
	info(XcosMessages.EMPTY_INFO);
	return isSuccess;
    }

	/**
	 * Save to a file
	 * 
	 * @param file the file
	 * @throws TransformerException on error
	 */
	private void save(final File file) throws TransformerException {
		final XcosCodec codec = new XcosCodec();
		final TransformerFactory tranFactory = TransformerFactory.newInstance();
		final Transformer aTransformer = tranFactory.newTransformer();
		
		final DOMSource src = new DOMSource(codec.encode(this));
		final StreamResult result = new StreamResult(file);
		aTransformer.transform(src, result);
	}
	
	/**
	 * Load from a file
	 * 
	 * @param file the file
	 * @throws TransformerException on error
	 */
	private void load(final File file) throws TransformerException {
		final XcosCodec codec = new XcosCodec();
		final TransformerFactory tranFactory = TransformerFactory.newInstance();
		final Transformer aTransformer = tranFactory.newTransformer();
		
		final StreamSource src = new StreamSource(file);
		final DOMResult result = new DOMResult();
		aTransformer.transform(src, result);
		
		codec.decode(result.getNode().getFirstChild(), this);
	}
	
	/**
	 * Perform post loading initialization.
	 * @param file the loaded file
	 */
	private void postLoad(final File file) {
		final String name = file.getName();

		setModified(false);
		setSavedFile(file);
		setTitle(name.substring(0, name.lastIndexOf('.')));
		generateUID();
	}
	
	/**
	 * Set the title of the diagram
	 * 
	 * @param title
	 *            the title
	 * @see org.scilab.modules.graph.ScilabGraph#setTitle(java.lang.String)
	 */
	@Override
	public void setTitle(final String title) {
		super.setTitle(title);
		updateTabTitle();
	}

	/**
	 * Update the title
	 */
	public void updateTabTitle() {
		// get the modifier string 
		final String modified;
		if (isModified()) {
			modified = "*";
		} else {
			modified = "";
		}
		
		// get the title string
		final String title = getTitle();
		
		// get the path
		CharSequence formattedPath = "";
		final File savedFile = getSavedFile();
		if (savedFile != null) {
			try {
				final String path = savedFile.getCanonicalPath();
				formattedPath = new StringBuilder()
					.append(" (")
					.append(path)
					.append(')');
			} catch (final IOException e) {
				LOG.debug(e);
			}
		}
		
		// Product name
		final String product = Xcos.TRADENAME;
		
		final String tabTitle = new StringBuilder()
			.append(modified)
			.append(title)
			.append(formattedPath)
			.append(" - ")
			.append(product)
			.toString();
		
		if (getParentTab() != null) {
			getParentTab().setName(tabTitle);
			getParentTab().draw();
		}
	}
    
    /**
     * @param context set context
     * @throws PropertyVetoException when the new value is invalid 
     */
    public void setContext(final String[] context) throws PropertyVetoException {
    	scicosParameters.setContext(context);
    	fireEvent(new mxEventObject(XcosEvent.DIAGRAM_UPDATED));
	updateCellsContext();
    }

    /**
     * Update context value in diagram children
     */
    public void updateCellsContext() {
	final Object rootParent = getDefaultParent();
	final int childCount = getModel().getChildCount(rootParent);
	for (int i = 0; i < childCount; ++i) {
	    final Object obj = getModel().getChildAt(rootParent, i);
	    if (obj instanceof ContextUpdate) {
		final String[] globalContext = getContext();

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
		final SuperBlock superBlock = (SuperBlock) obj;
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
    public void setDebugLevel(final int debugLevel) throws PropertyVetoException {
    	scicosParameters.setDebugLevel(debugLevel);
    }

    /**
     * Read a diagram from an HDF5 file (ask for creation if the file does not exist) 
     * @param diagramFileName file to open
     */
    public void openDiagramFromFile(final File diagramFileName) {
	    info(XcosMessages.LOADING_DIAGRAM);

	    if (diagramFileName.exists()) {
	    	transformAndLoadFile(diagramFileName, false);
	    } else {
		AnswerOption answer;
		try {
			answer = ScilabModalDialog.show(getParentTab(), String.format(
				XcosMessages.FILE_DOESNT_EXIST, diagramFileName.getCanonicalFile()),
				XcosMessages.XCOS, IconType.QUESTION_ICON,
				ButtonType.YES_NO);
		} catch (final IOException e) {
			LOG.error(e);
			answer = AnswerOption.YES_OPTION;
		}

		if (answer == AnswerOption.YES_OPTION) {
		    try {
			final FileWriter writer = new FileWriter(diagramFileName);
			writer.write("");
			writer.flush();
			writer.close();
			setSavedFile(diagramFileName);
		    } catch (final IOException ioexc) {
		    	LOG.error(ioexc);
		    	JOptionPane.showMessageDialog(getAsComponent(), ioexc);
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
     * @return the status of the operation
     */
    protected boolean transformAndLoadFile(final File theFile, final boolean wait) {
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
		final Thread transformAction = new Thread() {
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
		try {
			if (getModel().getChildCount(getDefaultParent()) == 0) {
		    	load(theFile);
		    	postLoad(theFile);
		    	setVisible(true);
		    } else {
		    	info(XcosMessages.LOADING_DIAGRAM);
		    	final XcosDiagram xcosDiagram = new XcosDiagram();
				xcosDiagram.load(theFile);
				xcosDiagram.postLoad(theFile);
				new XcosTab(xcosDiagram).setVisible(true);
				info(XcosMessages.EMPTY_INFO);
		    }
			result = true;
		} catch (final TransformerException e) {
			LOG.error(e);
			result = false;
		}
	    break;

	case HDF5:
	    final H5RWHandler handler = new H5RWHandler(fileToLoad);
	    handler.readDiagram(this);
	    generateUID();
	    updateTabTitle();
    	setVisible(true);
	    result = true;
	    break;

	default:
	    XcosDialogs.couldNotLoadFile(this);
	    break;
	}
	return result;
    }

    /**
     * generate unique id to all blocks in diagram
     */
    public void generateUID() {
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
	    if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
		final BasicBlock block = (BasicBlock) getModel().getChildAt(getDefaultParent(), i);
		if (block.getRealParameters() instanceof ScilabMList) {
			if (block instanceof SuperBlock) {
				// generate a child diagram with UID
				((SuperBlock) block).createChildDiagram(true);
			} else {
				// we have a hidden SuperBlock, create a real one
				final SuperBlock newSP = (SuperBlock) BlockFactory
						.createBlock("SUPER_f");
				newSP.setRealParameters(block.getRealParameters());
				newSP.createChildDiagram(true);
				newSP.setParentDiagram(this);
				block.setRealParameters(new DiagramElement()
						.encode(newSP.getChild()));
			}
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
    	getModel().beginUpdate();
    	try {
	    	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
	    	    final mxCell cell = (mxCell) getModel().getChildAt(getDefaultParent(), i);
	    	    if (cell instanceof BasicBlock) {
	    		final BasicBlock block = (BasicBlock) cell;
	    		block.setParentDiagram(this);
	    	    }
	    	}
    	} finally {
    		getModel().endUpdate();
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
     * @param cell block
     * @return cell tooltip
     */
	@Override
    public String getToolTipForCell(final Object cell) {
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
    public void info(final String message) {
	final String localMessage = message;
	if (getParentTab() != null && getParentTab().getInfoBar() != null) {
	    getParentTab().getInfoBar().setText(localMessage);
	}
    }

    /**
     * Display the message into an error popup
     * @param message Error of the message
     */
    public void error(final String message) {
	JOptionPane.showMessageDialog(getAsComponent(), message, XcosMessages.XCOS, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Find the block corresponding to the given uid
     * and display a warning message.
     * 
     * @param uid - A String as UID.
     * @param message - The message to display.
     */
    public void warnCellByUID(final String uid, final String message) {
    	final Object cell = ((mxGraphModel) getModel()).getCell(uid);
    	if (cell == null) {
		return;
    	}
    	
    	if (message.isEmpty()) {
    		// put the current tab on top
    		setVisible(true);
    	}
    	
    	getAsComponent().setCellWarning(cell, message);
    }

    /**
     * Set the current diagram in a modified state
     * @param modified True or False whether the current diagram must be saved or not. 
     */
	@Override
    public void setModified(final boolean modified) {
	super.setModified(modified);
	updateTabTitle();
    }
    
    /**
     * Evaluate the current context
     * 
     * @return The resulting data. Keys are variable names and Values are 
     *         evaluated values. 
     */
	public LinkedHashMap<String, String> evaluateContext() {
		LinkedHashMap<String, String> result = null;
		
		try {
			final Pattern p = Pattern.compile("('|\")");
			
			final StringBuilder str = new StringBuilder();
			str.append('[');
			for (final String s : getContext()) {
				str.append('\"');
				str.append(p.matcher(s).replaceAll("''"));
				str.append("\" ");
			}
			str.append(']');
				
			final File temp = FileUtils.createTempFile();
			
			ScilabInterpreterManagement.synchronousScilabExec(
						  "vars = script2var(" + str.toString() + ", struct());"
						+ "export_to_hdf5('" + temp.getAbsolutePath() + "', 'vars');");
			
			result = new H5RWHandler(temp).readContext();
		} catch (final IOException e) {
			info("Unable to create file");
			e.printStackTrace();
		} catch (final InterpreterException e) {
			info("Unable to evaluate the contexte");
			e.printStackTrace();
		}
		
		return result;
	}
    
    /**
     * @param uid block ID
     * @return block
     */
    public BasicBlock getChildById(final String uid) {
	BasicBlock returnBlock = null;
	for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
	    if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
		final BasicBlock block = (BasicBlock) getModel().getChildAt(getDefaultParent(), i);
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
     * Returns true if the given cell is a not a block nor a port.
     * @param cell the drop target
     * @param cells the cells to be dropped
     * @return the drop status
     * @see com.mxgraph.view.mxGraph#isValidDropTarget(java.lang.Object, java.lang.Object[])
     */
    @Override
    public boolean isValidDropTarget(final Object cell, final Object[] cells) {
		return !(cell instanceof BasicBlock) && !(cell instanceof BasicBlock)
				&& !(cell instanceof BasicPort)
				&& super.isValidDropTarget(cell, cells);
    }
    
    /**
     * @return child visibility
     */
	public boolean isChildVisible() {
		for (int i = 0; i < getModel().getChildCount(getDefaultParent()); i++) {
			final Object child = getModel().getChildAt(getDefaultParent(), i);
			if (child instanceof SuperBlock) {
				final XcosDiagram diag = ((SuperBlock) child).getChild();
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
	    final Object child = getModel().getChildAt(getDefaultParent(), i);
	    if (child instanceof SuperBlock) {
		final SuperBlock diag = (SuperBlock) child;

		if (diag.getChild() != null) {
		    diag.closeBlockSettings();
		}
	    }
	}
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
    		public void setCell(final Object cell) {
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
