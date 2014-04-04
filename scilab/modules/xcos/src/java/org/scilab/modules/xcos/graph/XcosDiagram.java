/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.awt.GraphicsEnvironment;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyVetoException;
import java.io.File;
import java.io.IOException;
import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.IllegalFormatException;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.Set;
import java.util.logging.Logger;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.SwingWorker;
import javax.swing.Timer;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.BlockFactory.BlockInterFunction;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
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
import org.scilab.modules.xcos.preferences.XcosOptions;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxGraphModel.Filter;
import com.mxgraph.model.mxGraphModel.mxChildChange;
import com.mxgraph.model.mxGraphModel.mxStyleChange;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.model.mxIGraphModel.mxAtomicGraphModelChange;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxGraphSelectionModel;
import com.mxgraph.view.mxMultiplicity;
import com.mxgraph.view.mxStylesheet;

/**
 * The base class for a diagram. This class contains jgraphx + Scicos data.
 */
public class XcosDiagram extends ScilabGraph {
    private static final Logger LOG = Logger.getLogger(XcosDiagram.class.getName());

    private static final String MODIFIED = "modified";
    private static final String CELLS = "cells";
    public static final String IN = "in";
    public static final String OUT = "out";
    public static final String EIN = "ein";
    public static final String EOUT = "eout";

    /**
     * Prefix used to tag text node.
     */
    public static final String HASH_IDENTIFIER = "#identifier";

    /**
     * Default geometry used while adding a label to a block (on the middle and
     * below the bottom of the parent block)
     */
    private static final mxGeometry DEFAULT_LABEL_GEOMETRY = new mxGeometry(0.5, 1.1, 0.0, 0.0);

    /*
     * diagram data
     */

    // the associated parameters
    private ScicosParameters scicosParameters;

    // the scicos engine current status
    private final transient CompilationEngineStatus engine;

    /**
     * Default constructor for a visible diagram
     */
    public XcosDiagram() {
        this(true);
    }

    /**
     * Constructor
     *
     * @param withVisibleFeatures true if the visible features should be activated, false otherwise. Disable it on encode/decode leads to a huge performance gain.
     */
    public XcosDiagram(final boolean withVisibleFeatures) {
        super();

        // Scicos related setup
        engine = new CompilationEngineStatus();
        setScicosParameters(new ScicosParameters());

        if (withVisibleFeatures) {
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

            setAutoOrigin(true);

            // Add a listener to track when model is changed
            getModel().addListener(mxEvent.CHANGE, ModelTracker.getInstance());
        }

        ((mxCell) getDefaultParent()).setId((new UID()).toString());
        ((mxCell) getModel().getRoot()).setId((new UID()).toString());
    }

    /*
     * Static helpers
     */

    /**
     * Only return the instanceof klass
     *
     * @param selection
     *            the selection to filter out
     * @param klass
     *            the class selector
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

    /**
     * Sort the blocks per first integer parameter value
     *
     * @param blocks
     *            the block list
     * @return the sorted block list (same instance)
     */
    public List <? extends BasicBlock > iparSort(final List <? extends BasicBlock > blocks) {
        Collections.sort(blocks, new Comparator<BasicBlock>() {
            @Override
            public int compare(BasicBlock o1, BasicBlock o2) {
                final ScilabDouble data1 = (ScilabDouble) o1.getIntegerParameters();
                final ScilabDouble data2 = (ScilabDouble) o2.getIntegerParameters();

                int value1 = 0;
                int value2 = 0;

                if (data1.getWidth() >= 1 && data1.getHeight() >= 1) {
                    value1 = (int) data1.getRealPart()[0][0];
                }
                if (data2.getWidth() >= 1 && data2.getHeight() >= 1) {
                    value2 = (int) data2.getRealPart()[0][0];
                }

                return value1 - value2;
            }
        });
        return blocks;
    }

    /**
     * @param <T>
     *            The type to work on
     * @param klass
     *            the class instance to work on
     * @return list of typed block
     */
    @SuppressWarnings("unchecked")
    private <T extends BasicBlock> List<T> getAllTypedBlock(Class<T> klass) {
        final List<T> list = new ArrayList<T>();

        int blockCount = getModel().getChildCount(getDefaultParent());

        for (int i = 0; i < blockCount; i++) {
            Object cell = getModel().getChildAt(getDefaultParent(), i);
            if (klass.isInstance(cell)) {
                // According to the test we are sure that the cell is an
                // instance of T. Thus we can safely cast it.
                list.add((T) cell);
            }
        }
        return list;
    }

    /**
     * @param <T>
     * @param <T>
     *            The type to work on
     * @param klasses
     *            the class instance list to work on
     * @return list of typed block
     */
    private <T extends BasicBlock> List<T> getAllTypedBlock(Class<T>[] klasses) {
        final List<T> list = new ArrayList<T>();
        for (Class<T> klass : klasses) {
            list.addAll(getAllTypedBlock(klass));
        }
        return list;
    }

    /**
     * Fill the context with I/O port
     *
     * @param context
     *            the context to fill
     */
    @SuppressWarnings("unchecked")
    protected void fillContext(final Hashtable<Object, Object> context) {
        if (!context.containsKey(IN)) {
            context.put(IN, iparSort(getAllTypedBlock( new Class [] { ExplicitInBlock.class, ImplicitInBlock.class })));
        }
        if (!context.containsKey(OUT)) {
            context.put(OUT, iparSort(getAllTypedBlock(new Class[] { ExplicitOutBlock.class, ImplicitOutBlock.class })));
        }
        if (!context.containsKey(EIN)) {
            context.put(EIN, iparSort(getAllTypedBlock(new Class[] { EventInBlock.class })));
        }
        if (!context.containsKey(EOUT)) {
            context.put(EOUT, iparSort(getAllTypedBlock(new Class[] { EventOutBlock.class })));
        }
    }

    /**
     * Function to update IO block numbering
     * @param block
     * @param ioBlockClass
     */
    @SuppressWarnings("unchecked")
    private void updateIOBlockByType(BasicBlock block, Hashtable<Object, Object> context, String type) {
        List <ContextUpdate> listOfBlocks = (List <ContextUpdate>) context.get(type);
        if (listOfBlocks.contains(block)) {
            int newIndex = 0;

            /*  Get an empty index :
             *  The list should always have a size greater of equal to one
             *  since new added element is always added to the list
             */
            if (listOfBlocks.size() > 1) {
                // if a hole exists, then assign a port from this hole
                for (int i = 0; i < listOfBlocks.size() - 1; i++) {
                    int indexNext = (int) ((ScilabDouble) listOfBlocks.get(i + 1).getIntegerParameters()).getRealPart()[0][0];
                    int indexPrevious = (int) ((ScilabDouble) listOfBlocks.get(i).getIntegerParameters()).getRealPart()[0][0];
                    if (indexNext - indexPrevious > 1) {
                        newIndex = indexPrevious + 1;
                        break;
                    }
                }
                // if no hole is present, then detect multiple items
                if (newIndex == 0) {
                    for (int i = 0; i < listOfBlocks.size() - 1; i++) {
                        int indexNext = (int) ((ScilabDouble) listOfBlocks.get(i + 1).getIntegerParameters()).getRealPart()[0][0];
                        int indexPrevious = (int) ((ScilabDouble) listOfBlocks.get(i).getIntegerParameters()).getRealPart()[0][0];
                        if (indexNext - indexPrevious == 0) {
                            newIndex = (int) ((ScilabDouble) listOfBlocks.get(listOfBlocks.size() - 1).getIntegerParameters()).getRealPart()[0][0] + 1;
                            break;
                        }
                    }
                    // if no multiple items are present, item is already at the right place
                    if (newIndex == 0) {
                        newIndex = (int) ((ScilabDouble) block.getIntegerParameters()).getRealPart()[0][0];
                    }
                }
            } else {
                newIndex = 1;
            }

            /*
             * Update the IO block with this new index
             */
            block.setIntegerParameters(new ScilabDouble(newIndex));
            block.setExprs(new ScilabString(Integer.toString(newIndex)));
            block.setOrdering(newIndex);
        }
    }

    /**
     * If the block is a IO block, update its index to a free index
     * @param block
     */
    private void updateIOBlocks(BasicBlock block) {
        Hashtable<Object, Object> context = new Hashtable<Object, Object> ();

        fillContext(context);

        updateIOBlockByType(block, context, IN);
        updateIOBlockByType(block, context, OUT);
        updateIOBlockByType(block, context, EIN);
        updateIOBlockByType(block, context, EOUT);
    }

    /*
     * Static diagram listeners
     */

    /**
     * CellAddedTracker Called when mxEvents.CELLS_ADDED is fired.
     */
    private static final class CellAddedTracker implements mxIEventListener {

        private static CellAddedTracker instance;

        /**
         * Default constructor
         */
        private CellAddedTracker() {
        }

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
         *
         * @param source
         *            the source instance
         * @param evt
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final XcosDiagram diagram = (XcosDiagram) source;
            final Object[] cells = (Object[]) evt.getProperty(CELLS);

            diagram.getModel().beginUpdate();
            try {
                final Filter filter = new Filter() {
                    @Override
                    public boolean filter(Object cell) {
                        if (cell instanceof BasicBlock) {
                            final BasicBlock blk = (BasicBlock) cell;

                            // Update parent on cell addition
                            blk.setParentDiagram(diagram);

                            // update port numbering
                            diagram.updateIOBlocks(blk);

                            // Fire an identifier update to let the I/O ports update their labels
                            mxCell identifier = diagram.getCellIdentifier(blk);
                            if (identifier != null) {
                                final Object current = diagram.getModel().getValue(identifier);
                                if (current != null) {
                                    final String text = mxUtils.getBodyMarkup(current.toString(), false);
                                    diagram.fireEvent(new mxEventObject(mxEvent.LABEL_CHANGED, "cell", identifier, "value", text, "parent", blk));
                                }
                            }

                            diagram.getView().invalidate();
                        }
                        return false;
                    }
                };

                for (int i = 0; i < cells.length; ++i) {
                    mxGraphModel.filterDescendants(diagram.getModel(), filter, cells[i]);
                }
            } finally {
                diagram.getModel().endUpdate();
            }
        }
    }

    /**
     * CellResizedTracker Called when mxEvents.CELLS_RESIZED is fired.
     */
    private static final class CellResizedTracker implements mxIEventListener {

        private static CellResizedTracker instance;

        /**
         * Constructor
         */
        private CellResizedTracker() {
        }

        /**
         * @return the instance
         */
        public static CellResizedTracker getInstance() {
            if (instance == null) {
                instance = new CellResizedTracker();
            }
            return instance;
        }

        /**
         * Update the cell view
         *
         * @param source
         *            the source instance
         * @param evt
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final XcosDiagram diagram = (XcosDiagram) source;
            final Object[] cells = (Object[]) evt.getProperty(CELLS);

            diagram.getModel().beginUpdate();
            try {
                for (int i = 0; i < cells.length; ++i) {
                    if (cells[i] instanceof BasicBlock) {
                        BlockPositioning.updateBlockView((BasicBlock) cells[i]);
                    }
                }
            } finally {
                diagram.getModel().endUpdate();
            }
        }
    }

    /**
     * ModelTracker called when mxEvents.CHANGE occurs on a model
     */
    private static final class ModelTracker implements mxIEventListener {
        private static ModelTracker instance;

        /**
         * Constructor
         */
        private ModelTracker() {
        }

        /**
         * @return the instance
         */
        public static ModelTracker getInstance() {
            if (instance == null) {
                instance = new ModelTracker();
            }
            return instance;
        }

        /**
         * Fire cell value update on any change
         *
         * @param source
         *            the source instance
         * @param evt
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final mxGraphModel model = (mxGraphModel) source;
            @SuppressWarnings("unchecked")
            final List<mxAtomicGraphModelChange> changes = (List<mxAtomicGraphModelChange>) (evt.getProperty("changes"));

            final List<Object> objects = new ArrayList<Object>();
            model.beginUpdate();
            try {

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
                    for (int j = 0; j < objects.size(); ++j) {
                        firedCells[j] = objects.get(j);
                    }

                    model.fireEvent(new mxEventObject(XcosEvent.FORCE_CELL_VALUE_UPDATE, CELLS, firedCells));
                }

            } finally {
                model.endUpdate();
            }
        }
    }

    /**
     * SuperBlockUpdateTracker Called when adding some port in a SuperBlock
     * diagram to update current sub-diagram (i.e SuperBlock) representation.
     */
    private static final class SuperBlockUpdateTracker implements mxIEventListener {
        private static SuperBlockUpdateTracker instance;

        /**
         * Constructor
         */
        private SuperBlockUpdateTracker() {
        }

        /**
         * @return the instance
         */
        public static SuperBlockUpdateTracker getInstance() {
            if (instance == null) {
                instance = new SuperBlockUpdateTracker();
            }
            return instance;
        }

        /**
         * Update the superblock values (rpar) on update
         *
         * @param source
         *            the source instance
         * @param evt
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            assert evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED) instanceof SuperBlock;

            final XcosDiagram diagram = (XcosDiagram) source;
            final SuperBlock updatedBlock = (SuperBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);

            assert diagram == updatedBlock.getParentDiagram();

            /*
             * The rpar value is set as invalid, encode the child on demand.
             */
            updatedBlock.invalidateRpar();

            if (diagram instanceof SuperBlockDiagram) {
                final SuperBlock parentBlock = ((SuperBlockDiagram) diagram).getContainer();
                parentBlock.getParentDiagram().fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED, XcosConstants.EVENT_BLOCK_UPDATED, parentBlock));
            }

            BlockPositioning.updateBlockView(updatedBlock);

            // force super block to refresh
            diagram.getView().clear(updatedBlock, true, true);

            // force links connected to super block to refresh
            final int childCount = diagram.getModel().getChildCount(updatedBlock);
            for (int i = 0; i < childCount; i++) {
                final Object port = diagram.getModel().getChildAt(updatedBlock, i);

                final int edgeCount = diagram.getModel().getEdgeCount(port);
                for (int j = 0; j < edgeCount; j++) {
                    final Object edge = diagram.getModel().getEdgeAt(port, j);
                    diagram.getView().clear(edge, true, true);
                }
            }

            diagram.getView().validate();
            diagram.repaint();
        }
    }

    /**
     * Update the modified block on undo/redo
     */
    private static final class UndoUpdateTracker implements mxIEventListener {
        private static UndoUpdateTracker instance;

        /**
         * Constructor
         */
        public UndoUpdateTracker() {
        }

        /**
         * @return the instance
         */
        public static UndoUpdateTracker getInstance() {
            if (instance == null) {
                instance = new UndoUpdateTracker();
            }
            return instance;
        }

        /**
         * Update the block and style on undo
         *
         * @param source
         *            the source instance
         * @param evt
         *            the event data
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final mxUndoableEdit edit = (mxUndoableEdit) evt.getProperty(ScilabGraphConstants.EVENT_CHANGE_EDIT);

            final mxGraphModel model = (mxGraphModel) edit.getSource();
            final List<mxUndoableChange> changes = edit.getChanges();

            final Object[] changedCells = getSelectionCellsForChanges(changes, model);
            model.beginUpdate();
            try {
                for (final Object object : changedCells) {
                    if (object instanceof BasicBlock) {
                        final BasicBlock current = (BasicBlock) object;
                        final XcosDiagram graph = current.getParentDiagram();

                        // When we change the style property we have to update
                        // some BasiBlock fields
                        if (changes.get(0) instanceof mxStyleChange) {
                            current.updateFieldsFromStyle();
                        }

                        // update the superblock container ports if the block is
                        // inside a superblock diagram
                        if (graph instanceof SuperBlockDiagram) {
                            SuperBlockDiagram superdiagram = (SuperBlockDiagram) current.getParentDiagram();
                            SuperBlock superblock = superdiagram.getContainer();
                            superblock.updateExportedPort();
                        }

                        // Update the block position
                        BlockPositioning.updateBlockView(current);

                        // force a refresh of the block ports and links
                        // connected to these ports
                        final int childCount = model.getChildCount(current);
                        for (int i = 0; i < childCount; i++) {
                            final Object port = model.getChildAt(current, i);
                            graph.getView().clear(port, true, true);
                            final int edgeCount = model.getEdgeCount(port);
                            for (int j = 0; j < edgeCount; j++) {
                                final Object edge = model.getEdgeAt(port, j);
                                graph.getView().clear(edge, true, true);
                            }
                        }
                        // force a refresh of the block
                        graph.getView().clear(current, true, true);

                        graph.getView().validate();
                        graph.repaint();
                    }
                }
            } finally {
                model.endUpdate();
            }
        }
    }

    /**
     * Refresh each block on modification (update port position, etc...)
     */
    private static final class RefreshBlockTracker implements mxIEventListener {
        private static RefreshBlockTracker instance;

        /**
         * Default constructor
         */
        private RefreshBlockTracker() {
        }

        /**
         * @return the instance
         */
        public static RefreshBlockTracker getInstance() {
            if (instance == null) {
                instance = new RefreshBlockTracker();
            }
            return instance;
        }

        /**
         * Refresh the block on port added
         *
         * @param sender
         *            the diagram
         * @param evt
         *            the event
         * @see com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         *      com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(Object sender, mxEventObject evt) {
            final XcosDiagram diagram = (XcosDiagram) sender;

            diagram.getModel().beginUpdate();
            try {
                final BasicBlock updatedBlock = (BasicBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
                BlockPositioning.updateBlockView(updatedBlock);

                diagram.getView().clear(updatedBlock, true, true);

                // validate display errors
                diagram.getAsComponent().clearCellOverlays();
                diagram.getAsComponent().validateGraph();

                diagram.getView().validate();
            } finally {
                diagram.getModel().endUpdate();
            }
        }
    }

    /**
     * Hook method that creates the new edge for insertEdge. This implementation
     * does not set the source and target of the edge, these are set when the
     * edge is added to the model.
     *
     * @param parent
     *            Cell that specifies the parent of the new edge.
     * @param id
     *            Optional string that defines the Id of the new edge.
     * @param value
     *            Object to be used as the user object.
     * @param source
     *            Cell that defines the source of the edge.
     * @param target
     *            Cell that defines the target of the edge.
     * @param style
     *            Optional string that defines the cell style.
     * @return Returns the new edge to be inserted.
     * @see com.mxgraph.view.mxGraph#createEdge(java.lang.Object,
     *      java.lang.String, java.lang.Object, java.lang.Object,
     *      java.lang.Object, java.lang.String)
     */
    @Override
    public Object createEdge(Object parent, String id, Object value, Object source, Object target, String style) {
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
                LOG.severe(e.toString());
            } catch (IllegalAccessException e) {
                LOG.severe(e.toString());
            }

            ret = link;
        }

        if (ret == null) {
            ret = super.createEdge(parent, id, value, source, target, style);
            LOG.warning("Creating a non typed edge");
        }

        return ret;
    }

    /**
     * Add an edge from a source to the target.
     *
     * @param cell
     *            the edge to add (may be null)
     * @param parent
     *            the parent of the source and the target
     * @param source
     *            the source cell
     * @param target
     *            the target cell
     * @param index
     *            the index of the edge
     * @return the added edge or null.
     * @see com.mxgraph.view.mxGraph#addEdge(java.lang.Object, java.lang.Object,
     *      java.lang.Object, java.lang.Object, java.lang.Integer)
     */
    @Override
    public Object addCell(Object cell, Object parent, Integer index, Object source, Object target) {

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
            final SplitBlock split = addSplitEdge(current.getGeometry().getTargetPoint(), (ImplicitLink) target);
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
            LOG.severe("Unable to add a typed link");
            return null;
        } else {
            LOG.severe("Adding an untyped edge");
            return super.addCell(cell, parent, index, source, target);
        }
    }

    /**
     * Add a split on a edge.
     *
     * @param splitPoint
     *            the split point (center of the split block)
     * @param link
     *            source link
     * @return split block
     */
    public SplitBlock addSplitEdge(final mxPoint splitPoint, final BasicLink link) {
        final BasicPort linkSource = (BasicPort) link.getSource();
        final BasicPort linkTarget = (BasicPort) link.getTarget();

        final SplitBlock splitBlock = (SplitBlock) BlockFactory.createBlock(BlockInterFunction.SPLIT_f);

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
     * @param orig
     *            the origin point (may be (0,0))
     */
    public void connect(BasicPort src, BasicPort trg, List<mxPoint> points, mxPoint orig) {
        mxGeometry geometry;

        /*
         * Add the link with a default geometry
         */
        final Object newLink1 = createEdge(null, null, null, src, trg, null);
        addCell(newLink1, null, null, src, trg);
        geometry = getModel().getGeometry(newLink1);
        if (getModel().getParent(newLink1) instanceof BasicBlock) {
            // on a loop link, translate the points as the cell has been moved to the parent
            orig.setX(orig.getX() + geometry.getX());
            orig.setY(orig.getY() + geometry.getY());
        }
        geometry.setPoints(points);
        getModel().setGeometry(newLink1, geometry);

        /*
         * Update the geometry
         */
        // should be cloned to generate an event
        geometry = (mxGeometry) getModel().getGeometry(newLink1).clone();
        final double dx = orig.getX();
        final double dy = orig.getY();

        geometry.translate(dx, dy);
        getModel().setGeometry(newLink1, geometry);
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

        getAsComponent().setBackground(XcosOptions.getEdition().getGraphBackground());

        final boolean gridEnable = XcosOptions.getEdition().isGraphGridEnable();
        setGridVisible(gridEnable);
        if (gridEnable) {
            setGridSize(XcosOptions.getEdition().getGraphGrid());
        }

        /*
         * Reinstall related listeners
         */

        // Property change Listener
        // Will say if a diagram has been modified or not.
        final PropertyChangeListener p = new PropertyChangeListener() {
            @Override
            public void propertyChange(final PropertyChangeEvent e) {
                if (e.getPropertyName().compareTo(MODIFIED) == 0) {
                    if (!e.getOldValue().equals(e.getNewValue())) {
                        updateTabTitle();
                    }
                }
            }
        };
        getAsComponent().removePropertyChangeListener(MODIFIED, p);
        getAsComponent().addPropertyChangeListener(MODIFIED, p);
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
        final List<mxMultiplicity> multiplicities = new ArrayList<mxMultiplicity>();

        // Input data port
        multiplicities.add(new PortCheck(ExplicitInputPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = -4987163442006736665L;
            {
                add(ExplicitOutputPort.class);
                add(ExplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_EXPLICIT_IN));
        multiplicities.add(new PortCheck(ImplicitInputPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 886376532181210926L;
            {
                add(ImplicitOutputPort.class);
                add(ImplicitInputPort.class);
                add(ImplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_IMPLICIT_IN));

        // Output data port
        multiplicities.add(new PortCheck(ExplicitOutputPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 4594127972486054821L;
            {
                add(ExplicitInputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EXPLICIT_OUT));
        multiplicities.add(new PortCheck(ImplicitOutputPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = -3719677806532507973L;
            {
                add(ImplicitInputPort.class);
                add(ImplicitOutputPort.class);
                add(ImplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_IMPLICIT_OUT));

        // Control port
        multiplicities.add(new PortCheck(ControlPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 2941077191386058497L;
            {
                add(CommandPort.class);
                add(CommandControlLink.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_IN));

        // Command port
        multiplicities.add(new PortCheck(CommandPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = -3470370027962480362L;
            {
                add(ControlPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // ExplicitLink connections
        multiplicities.add(new PortCheck(ExplicitLink.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 7423543162930147373L;

            {
                add(ExplicitInputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // ImplicitLink connections
        multiplicities.add(new PortCheck(ImplicitLink.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 7775100011122283282L;

            {
                add(ImplicitInputPort.class);
                add(ImplicitOutputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // CommandControlLink connections
        multiplicities.add(new PortCheck(CommandControlLink.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 3260421433507192386L;

            {
                add(ControlPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // Already connected port
        multiplicities.add(new PortCheck(BasicPort.class, Collections.unmodifiableList(new ArrayList < Class <? extends mxCell >> () {
            private static final long serialVersionUID = 6376349598052836660L;

            {
                add(BasicPort.class);
            }
        }), XcosMessages.LINK_ERROR_ALREADY_CONNECTED));

        setMultiplicities(multiplicities.toArray(new mxMultiplicity[multiplicities.size()]));
    }

    /**
     * Install all needed Listeners.
     */
    public void installListeners() {
        /*
         * First remove all listeners if present
         */
        removeListener(SuperBlockUpdateTracker.getInstance());
        removeListener(CellAddedTracker.getInstance());
        removeListener(getEngine());
        getModel().removeListener(getEngine());
        removeListener(CellResizedTracker.getInstance());
        getUndoManager().removeListener(UndoUpdateTracker.getInstance());
        removeListener(RefreshBlockTracker.getInstance());

        // Track when superblock ask a parent refresh.
        addListener(XcosEvent.SUPER_BLOCK_UPDATED, SuperBlockUpdateTracker.getInstance());

        // Track when cells are added.
        addListener(mxEvent.CELLS_ADDED, CellAddedTracker.getInstance());
        addListener(mxEvent.CELLS_ADDED, getEngine());

        // Track when cells are deleted.
        addListener(mxEvent.CELLS_REMOVED, getEngine());

        // Track when resizing a cell.
        addListener(mxEvent.CELLS_RESIZED, CellResizedTracker.getInstance());

        // Track when we have to force a Block value
        addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, getEngine());
        getModel().addListener(XcosEvent.FORCE_CELL_VALUE_UPDATE, getEngine());

        // Update the blocks view on undo/redo
        getUndoManager().addListener(mxEvent.UNDO, UndoUpdateTracker.getInstance());
        getUndoManager().addListener(mxEvent.REDO, UndoUpdateTracker.getInstance());

        // Refresh port position on update
        addListener(XcosEvent.ADD_PORTS, RefreshBlockTracker.getInstance());

    }

    /**
     * Removes the given cells from the graph including all connected edges if
     * includeEdges is true. The change is carried out using cellsRemoved.
     *
     * @param cells
     *            the cells to be removed
     * @param includeEdges
     *            true if the edges must be removed, false otherwise.
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
        final Queue<Object> loopCells = new LinkedList<Object>(Arrays.asList(initialCells));
        // the cells that need to be really
        final Set<Object> removedCells = new HashSet<Object>(loopCells);
        // couple of cells to reconnect
        final List<BasicPort[]> connectedCells = new ArrayList<BasicPort[]>();
        final List<List<mxPoint>> connectedPoints = new ArrayList<List<mxPoint>>();

        /*
         * Then loop on the algorithm to select the right edges
         */
        // /!\ not bounded algorithm
        while (loopCells.size() > 0) {
            Object cell = loopCells.remove();

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

                if (splitBlock.getIn().getEdgeCount() == 0 || splitBlock.getOut1().getEdgeCount() == 0 || splitBlock.getOut2().getEdgeCount() == 0) {
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
                 * Explicit case, if the in link is deleted; all the out links
                 * also should.
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
                    // only implicit or event case, log otherwise
                    if (out1Link instanceof ExplicitLink || out2Link instanceof ExplicitLink) {
                        LOG.severe("Reconnection failed for explicit links");
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
                if (!removedCells.contains(connection[0].getParent()) && !removedCells.contains(connection[1].getParent())) {
                    connect(connection[0], connection[1], points, new mxPoint());
                }
            }
        } finally {
            getModel().endUpdate();
        }
        return ret;
    }

    /**
     * Add any terminal parent to the removed cells
     *
     * @param terminal
     *            the current terminal (instance of BasicPort)
     * @param removedCells
     *            the "to be removed" set
     * @param loopCells
     *            the "while loop" set
     */
    private void addTerminalParent(mxICell terminal, Collection<Object> removedCells, Collection<Object> loopCells) {
        assert (terminal == null || terminal instanceof BasicPort);
        assert (removedCells != null);
        assert (loopCells != null);

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
     * @param linkSource
     *            the normal source link
     * @param linkTerminal
     *            the normal target link
     * @param removedCells
     *            the set of removed objects
     * @return the {source, target} connection
     */
    private BasicPort[] findTerminals(final mxICell linkSource, final mxICell linkTerminal, final Set<Object> removedCells) {
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

        return new BasicPort[] { src, tgt };
    }

    /**
     * Get the direct points from inLink.getSource() to outLink.getTarget().
     *
     * @param splitBlock
     *            the current splitblock (added as a mid-point)
     * @param inLink
     *            the link before the split
     * @param outLink
     *            the link after the split
     * @return the points
     */
    private List<mxPoint> getDirectPoints(final SplitBlock splitBlock, final mxICell inLink, final mxICell outLink) {
        List<mxPoint> points;
        // add the points before the split
        points = new ArrayList<mxPoint>();
        if (inLink.getGeometry().getPoints() != null) {
            points.addAll(inLink.getGeometry().getPoints());
        }

        // add a new point at the split location
        points.add(new mxPoint(snap(splitBlock.getGeometry().getCenterX()), snap(splitBlock.getGeometry().getCenterY())));

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
     *
     * @param cell
     *            the cell
     * @return true if the corresponding label is moveable
     * @see com.mxgraph.view.mxGraph#isLabelMovable(java.lang.Object)
     */
    @Override
    public boolean isLabelMovable(Object cell) {
        return !(cell instanceof BasicBlock);
    }

    /**
     * Return true if selectable
     *
     * @param cell
     *            the cell
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
     *
     * @param cell
     *            the cell
     * @return status
     * @see com.mxgraph.view.mxGraph#isCellMovable(java.lang.Object)
     */
    @Override
    public boolean isCellMovable(final Object cell) {
        if (cell instanceof BasicPort) {
            return false;
        }

        boolean movable = false;
        final Object[] cells = getSelectionCells();

        // don't move if selection is only links
        for (Object c : cells) {
            if (!(c instanceof BasicLink)) {
                movable = true;
                break;
            }
        }

        return movable && super.isCellMovable(cell);
    }

    /**
     * Return true if resizable
     *
     * @param cell
     *            the cell
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
     * A cell is deletable if it is not a locked block or an identifier cell
     *
     * @param cell
     *            the cell
     * @return status
     * @see com.mxgraph.view.mxGraph#isCellDeletable(java.lang.Object)
     */
    @Override
    public boolean isCellDeletable(final Object cell) {
        final boolean isALockedBLock = cell instanceof BasicBlock && ((BasicBlock) cell).isLocked();
        final boolean isAnIdentifier = cell.getClass().equals(mxCell.class);

        if (isALockedBLock) {
            return false;
        }
        if (isAnIdentifier) {
            return true;
        }

        return super.isCellDeletable(cell);
    }

    /**
     * Return true if editable
     *
     * @param cell
     *            the cell
     * @return status
     * @see com.mxgraph.view.mxGraph#isCellEditable(java.lang.Object)
     */
    @Override
    public boolean isCellEditable(final Object cell) {
        return (cell instanceof TextBlock) && super.isCellDeletable(cell);
    }

    /**
     * Return or create the identifier for the cell
     *
     * @param cell
     *            the cell to check
     * @return the identifier cell
     */
    public mxCell getOrCreateCellIdentifier(final mxCell cell) {
        final mxGraphModel graphModel = (mxGraphModel) getModel();

        final mxCell identifier;
        final String cellId = cell.getId() + HASH_IDENTIFIER;
        if (graphModel.getCell(cellId) == null) {
            // create the identifier
            identifier = createCellIdentifier(cell);

            // add the identifier
            graphModel.add(cell, identifier, cell.getChildCount());
        } else {
            identifier = (mxCell) graphModel.getCell(cellId);
        }
        return identifier;
    }

    /**
     * Return the identifier for the cell
     *
     * @param cell
     *            the cell to check
     * @return the identifier cell
     */
    public mxCell getCellIdentifier(final mxCell cell) {
        final mxGraphModel graphModel = (mxGraphModel) getModel();
        final String cellId = cell.getId() + HASH_IDENTIFIER;

        return (mxCell) graphModel.getCell(cellId);
    }

    /**
     * Create a cell identifier for a specific cell
     *
     * @param cell
     *            the cell
     * @return the cell identifier.
     */
    public mxCell createCellIdentifier(final mxCell cell) {
        final mxCell identifier;
        final String cellId = cell.getId() + HASH_IDENTIFIER;

        identifier = new mxCell(null, (mxGeometry) DEFAULT_LABEL_GEOMETRY.clone(), "noLabel=0;opacity=0;");
        identifier.getGeometry().setRelative(true);
        identifier.setVertex(true);
        identifier.setConnectable(false);
        identifier.setId(cellId);

        return identifier;
    }

    /**
     * Get the label for the cell according to its style.
     *
     * @param cell
     *            the cell object
     * @return a representative the string (block name) or a style specific
     *         style.
     * @see com.mxgraph.view.mxGraph#convertValueToString(java.lang.Object)
     */
    @Override
    public String convertValueToString(Object cell) {
        String ret = null;

        if (cell != null) {
            final Map<String, Object> style = getCellStyle(cell);

            final String displayedLabel = (String) style.get("displayedLabel");
            if (displayedLabel != null) {
                if (cell instanceof BasicBlock) {
                    try {
                        ret = String.format(displayedLabel, ((BasicBlock) cell).getExprsFormat());
                    } catch (IllegalFormatException e) {
                        LOG.severe(e.toString());
                        ret = displayedLabel;
                    }
                } else {
                    ret = displayedLabel;
                }
            } else {
                final String label = super.convertValueToString(cell);
                if (label.isEmpty() && cell instanceof BasicBlock) {
                    ret = ((BasicBlock) cell).getInterfaceFunctionName();
                } else {
                    ret = label;
                }
            }
        }

        return ret;
    }

    /**
     * Return true if auto sized
     *
     * @param cell
     *            the cell
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
     * {@inheritDoc} Do not extends if the port position is north or south.
     */
    @Override
    public boolean isExtendParent(Object cell) {
        final boolean extendsParents;

        if (cell instanceof BasicPort) {
            final BasicPort p = (BasicPort) cell;
            extendsParents = !(p.getOrientation() == Orientation.NORTH || p.getOrientation() == Orientation.SOUTH) && super.isExtendParent(p);
        } else {
            extendsParents = super.isExtendParent(cell);
        }
        return extendsParents;
    }

    /**
     * @return the parameters
     */
    public ScicosParameters getScicosParameters() {
        return scicosParameters;
    }

    /**
     * @param scicosParameters
     *            the simulation parameters
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
     * @param finalIntegrationTime
     *            set integration time
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setFinalIntegrationTime(final double finalIntegrationTime) throws PropertyVetoException {
        scicosParameters.setFinalIntegrationTime(finalIntegrationTime);
    }

    /**
     * @param integratorAbsoluteTolerance
     *            set integrator absolute tolerance
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorAbsoluteTolerance(final double integratorAbsoluteTolerance) throws PropertyVetoException {
        scicosParameters.setIntegratorAbsoluteTolerance(integratorAbsoluteTolerance);
    }

    /**
     * @param integratorRelativeTolerance
     *            integrator relative tolerance
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setIntegratorRelativeTolerance(final double integratorRelativeTolerance) throws PropertyVetoException {
        scicosParameters.setIntegratorRelativeTolerance(integratorRelativeTolerance);
    }

    /**
     * @param maximumStepSize
     *            set max step size
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaximumStepSize(final double maximumStepSize) throws PropertyVetoException {
        scicosParameters.setMaximumStepSize(maximumStepSize);
    }

    /**
     * @param maxIntegrationTimeinterval
     *            set max integration time
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setMaxIntegrationTimeinterval(final double maxIntegrationTimeinterval) throws PropertyVetoException {
        scicosParameters.setMaxIntegrationTimeInterval(maxIntegrationTimeinterval);
    }

    /**
     * @param realTimeScaling
     *            set real time scaling
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setRealTimeScaling(final double realTimeScaling) throws PropertyVetoException {
        scicosParameters.setRealTimeScaling(realTimeScaling);
    }

    /**
     * @param solver
     *            set solver
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setSolver(final double solver) throws PropertyVetoException {
        scicosParameters.setSolver(solver);
    }

    /**
     * @param toleranceOnTime
     *            set tolerance time
     * @throws PropertyVetoException
     *             when the value is invalid
     * @category XMLCompatibility
     */
    public void setToleranceOnTime(final double toleranceOnTime) throws PropertyVetoException {
        scicosParameters.setToleranceOnTime(toleranceOnTime);
    }

    /**
     * Get the current diagram context
     *
     * @return the context at the current node
     */
    public String[] getContext() {
        return scicosParameters.getContext();
    }

    /**
     * Manage the visibility of the grid and the associated menu
     *
     * @param status
     *            new status
     */
    public void setGridVisible(final boolean status) {
        setGridEnabled(status);
        getAsComponent().setGridVisible(status);
        getAsComponent().repaint();
    }

    /**
     * @return save status
     */
    public boolean saveDiagram() {
        final boolean isSuccess = saveDiagramAs(getSavedFile());

        if (isSuccess) {
            setModified(false);
            Xcos.getInstance().addDiagram(getSavedFile(), this);
        }

        return isSuccess;
    }

    /**
     * @param fileName
     *            diagram filename
     * @return save status
     */
    public boolean saveDiagramAs(final File fileName) {
        boolean isSuccess = false;
        File writeFile = fileName;
        XcosFileType format = XcosOptions.getPreferences().getFileFormat();

        info(XcosMessages.SAVING_DIAGRAM);
        if (fileName == null) {
            final SwingScilabFileChooser fc = SaveAsAction.createFileChooser();
            SaveAsAction.configureFileFilters(fc);
            ConfigurationManager.configureCurrentDirectory(fc);

            if (getSavedFile() != null) {
                // using save-as, the file chooser should have a filename
                // without extension as default
                String filename = getSavedFile().getName();
                filename = filename.substring(0, filename.lastIndexOf('.'));
                fc.setSelectedFile(new File(filename));
            } else {
                final String title = getTitle();
                if (title != null) {
                    /*
                     * Escape file to handle not supported character in file
                     * name (may be Windows only) see
                     * http://msdn.microsoft.com/en
                     * -us/library/windows/desktop/aa365247%28v=vs.85%29.aspx
                     */
                    final char[] regex = "<>:\"/\\|?*".toCharArray();
                    String escaped = title;
                    for (char c : regex) {
                        escaped = escaped.replace(c, '-');
                    }

                    fc.setSelectedFile(new File(escaped));
                }
            }

            int status = fc.showSaveDialog(this.getAsComponent());
            if (status != JFileChooser.APPROVE_OPTION) {
                info(XcosMessages.EMPTY_INFO);
                return isSuccess;
            }

            // change the format if the user choose a save-able file format
            final XcosFileType selectedFilter = XcosFileType.findFileType(fc.getFileFilter());
            if (XcosFileType.getAvailableSaveFormats().contains(selectedFilter)) {
                format = selectedFilter;
            }
            writeFile = fc.getSelectedFile();
        }

        /* Extension/format update */

        // using a String filename also works on a non-existing file
        final String filename = writeFile.getName();

        /*
         * Look for the user extension if it does not exists, append a default
         * one
         *
         * if the specified extension is handled, update the save format ; else
         * append a default extension and use the default format
         */
        XcosFileType userExtension = XcosFileType.findFileType(filename);
        if (userExtension == null) {
            writeFile = new File(writeFile.getParent(), filename + format.getDottedExtension());
            userExtension = format;
        }
        if (XcosFileType.getAvailableSaveFormats().contains(userExtension)) {
            format = userExtension;
        } else {
            writeFile = new File(writeFile.getParent(), filename + format.getDottedExtension());
        }

        /*
         * If the file exists, ask for confirmation if this is not the
         * previously saved file
         */
        if (writeFile.exists() && !writeFile.equals(getSavedFile())) {
            final boolean overwrite = ScilabModalDialog.show(XcosTab.get(this), XcosMessages.OVERWRITE_EXISTING_FILE, XcosMessages.XCOS,
                                      IconType.QUESTION_ICON, ButtonType.YES_NO) == AnswerOption.YES_OPTION;

            if (!overwrite) {
                info(XcosMessages.EMPTY_INFO);
                return false;
            }
        }

        /*
         * Really save the data
         */
        try {
            format.save(writeFile.getCanonicalPath(), getRootDiagram());
            setSavedFile(writeFile);

            setTitle(writeFile.getName().substring(0, writeFile.getName().lastIndexOf('.')));
            ConfigurationManager.getInstance().addToRecentFiles(writeFile);
            setModified(false);
            isSuccess = true;
        } catch (final Exception e) {
            LOG.severe(e.toString());

            XcosDialogs.couldNotSaveFile(this);
        }

        info(XcosMessages.EMPTY_INFO);
        return isSuccess;
    }

    /**
     * Perform post loading initialization.
     *
     * @param file
     *            the loaded file
     */
    public void postLoad(final File file) {
        final String name = file.getName();

        if (XcosFileType.getAvailableSaveFormats().contains(XcosFileType.findFileType(file))) {
            setSavedFile(file);
            Xcos.getInstance().addDiagram(file, this);
        }
        setTitle(name.substring(0, name.lastIndexOf('.')));
        setModified(false);

        fireEvent(new mxEventObject(mxEvent.ROOT));

        info(XcosMessages.EMPTY_INFO);
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
                formattedPath = new StringBuilder().append(" (").append(path).append(')');
            } catch (final IOException e) {
                LOG.warning(e.toString());
            }
        }

        // Product name
        final String product = Xcos.TRADENAME;

        final String tabTitle = new StringBuilder().append(modified).append(title).append(formattedPath).append(" - ").append(product).toString();

        final SwingScilabDockablePanel tab = ScilabTabFactory.getInstance().getFromCache(getGraphTab());
        if (tab != null) {
            tab.setName(tabTitle);
        }
    }

    /**
     * @param context
     *            set context
     * @throws PropertyVetoException
     *             when the new value is invalid
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
     * @param debugLevel
     *            change debug level
     * @category XMLCompatibility
     * @throws PropertyVetoException
     *             when the new value is invalid
     */
    public void setDebugLevel(final int debugLevel) throws PropertyVetoException {
        scicosParameters.setDebugLevel(debugLevel);
    }

    /**
     * Popup a dialog to ask for a file creation
     *
     * @param f
     *            the file to create
     * @return true if creation is has been performed
     */
    public boolean askForFileCreation(final File f) {
        AnswerOption answer;
        try {
            answer = ScilabModalDialog.show(getAsComponent(), new String[] { String.format(XcosMessages.FILE_DOESNT_EXIST, f.getCanonicalFile()) },
                                            XcosMessages.XCOS, IconType.QUESTION_ICON, ButtonType.YES_NO);
        } catch (final IOException e) {
            LOG.severe(e.toString());
            answer = AnswerOption.YES_OPTION;
        }

        if (answer == AnswerOption.YES_OPTION) {
            return saveDiagramAs(f);
        }

        return false;
    }

    /**
     * Load a file with different method depending on it extension
     *
     * @param file
     *            File to load (can be null)
     * @param variable
     *            the variable to decode (can be null)
     */
    public void transformAndLoadFile(final String file, final String variable) {
        final File f;
        final XcosFileType filetype;
        if (file != null) {
            f = new File(file);
            filetype = XcosFileType.findFileType(f);
        } else {
            f = null;
            filetype = null;
        }
        new SwingWorker<XcosDiagram, ActionEvent>() {
            int counter = 0;
            final Timer t = new Timer(1000, new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    counter = (counter + 1) % (XcosMessages.DOTS.length() + 1);
                    String str = XcosMessages.LOADING_DIAGRAM + XcosMessages.DOTS.substring(0, counter);

                    XcosDiagram.this.info(str);
                }
            });

            @Override
            protected XcosDiagram doInBackground() {
                t.start();

                final Xcos instance = Xcos.getInstance();
                XcosDiagram diag = XcosDiagram.this;

                diag.setReadOnly(true);

                /*
                 * Load, log errors and notify
                 */
                synchronized (instance) {
                    try {

                        if (f != null && filetype != null) {
                            filetype.load(file, XcosDiagram.this);
                        }

                        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
                        try {
                            if (variable != null && handler != null) {
                                handler.readDiagram(XcosDiagram.this, variable);
                            }
                        } finally {
                            if (handler != null) {
                                handler.release();
                            }
                        }

                        instance.setLastError("");
                    } catch (Exception e) {
                        Throwable ex = e;
                        while (ex instanceof RuntimeException) {
                            ex = ex.getCause();
                        }
                        instance.setLastError(ex.getMessage());
                    }
                    instance.notify();
                }

                return diag;
            }

            @Override
            protected void done() {
                t.stop();
                XcosDiagram.this.setReadOnly(false);
                XcosDiagram.this.getUndoManager().clear();
                XcosDiagram.this.refresh();

                /*
                 * Load has finished
                 */
                if (f != null && filetype != null) {
                    postLoad(f);
                }
                XcosDiagram.this.info(XcosMessages.EMPTY_INFO);
            }

        } .execute();
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
     *
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
     *
     * @param cell
     *            block
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
     *
     * @param message
     *            Information
     */
    public void info(final String message) {
        final XcosTab tab = XcosTab.get(this);

        if (tab != null && tab.getInfoBar() != null) {
            tab.getInfoBar().setText(message);
        }
    }

    /**
     * Display the message into an error popup
     *
     * @param message
     *            Error of the message
     */
    public void error(final String message) {
        JOptionPane.showMessageDialog(getAsComponent(), message, XcosMessages.XCOS, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Find the block corresponding to the given uid and display a warning
     * message.
     *
     * @param uid
     *            - A String as UID.
     * @param message
     *            - The message to display.
     */
    public void warnCellByUID(final String uid, final String message) {
        final Object cell = ((mxGraphModel) getModel()).getCell(uid);
        if (cell == null) {
            return;
        }

        if (GraphicsEnvironment.isHeadless()) {
            System.err.printf("%s at %s\n    %s: %s\n", "warnCell", getRootDiagram().getTitle(), uid, message);
            return;
        }

        // open the tab
        if (XcosTab.get(this) == null) {
            XcosTab.restore(this);
        }

        if (message.isEmpty()) {
            getAsComponent().clearCellOverlays(cell);
        } else {
            getAsComponent().setCellWarning(cell, message, null, true);
        }
    }

    /**
     * Set the current diagram in a modified state
     *
     * @param modified
     *            True or False whether the current diagram must be saved or
     *            not.
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
    public Map<String, String> evaluateContext() {
        Map<String, String> result = Collections.emptyMap();

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return result;
        }

        try {
            // first write the context strings
            handler.writeContext(getContext());

            // evaluate using script2var
            ScilabInterpreterManagement.synchronousScilabExec(ScilabDirectHandler.CONTEXT + " = script2var(" + ScilabDirectHandler.CONTEXT + ", struct());");

            // read the structure
            result = handler.readContext();
        } catch (final InterpreterException e) {
            info("Unable to evaluate the contexte");
            e.printStackTrace();
        } finally {
            handler.release();
        }

        return result;
    }

    /**
     * @param uid
     *            block ID
     * @return block
     */
    public BasicBlock getChildById(final String uid) {
        BasicBlock returnBlock = null;
        for (int i = 0; i < getModel().getChildCount(getDefaultParent()); ++i) {
            if (getModel().getChildAt(getDefaultParent(), i) instanceof BasicBlock) {
                final BasicBlock block = (BasicBlock) getModel().getChildAt(getDefaultParent(), i);
                if (block.getId().compareTo(uid) == 0) { // find it
                    returnBlock = block;
                } else {
                    if (block instanceof SuperBlock) {
                        ((SuperBlock) block).createChildDiagram();

                        // search in child
                        returnBlock = ((SuperBlock) block).getChild().getChildById(uid);

                    } else if (block.getRealParameters() instanceof ScilabMList) {
                        // we have a hidden SuperBlock, create a real one
                        SuperBlock newSP = (SuperBlock) BlockFactory.createBlock(SuperBlock.INTERFUNCTION_NAME);
                        newSP.setParentDiagram(block.getParentDiagram());

                        newSP.setRealParameters(block.getRealParameters());
                        newSP.createChildDiagram(true);

                        // search in child
                        returnBlock = newSP.getChild().getChildById(uid);
                        block.setRealParameters(newSP.getRealParameters());
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
     *
     * @param cell
     *            the drop target
     * @param cells
     *            the cells to be dropped
     * @return the drop status
     * @see com.mxgraph.view.mxGraph#isValidDropTarget(java.lang.Object,
     *      java.lang.Object[])
     */
    @Override
    public boolean isValidDropTarget(final Object cell, final Object[] cells) {
        return !(cell instanceof BasicBlock) && !(cell instanceof BasicBlock) && !(cell instanceof BasicPort) && super.isValidDropTarget(cell, cells);
    }

    /**
     * @return child visibility
     */
    @Deprecated
    public boolean isChildVisible() {
        // FIXME check

        return false;

        // for (int i = 0; i < getModel().getChildCount(getDefaultParent());
        // i++) {
        // final Object child = getModel().getChildAt(getDefaultParent(), i);
        // if (child instanceof SuperBlock) {
        // final XcosDiagram diag = ((SuperBlock) child).getChild();
        // if (diag != null && (diag.isChildVisible() || diag.isVisible())) {
        // // if child or sub child is visible
        // return true;
        // }
        // }
        // }
        // return false;
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
             * @param cell
             *            the cell
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
