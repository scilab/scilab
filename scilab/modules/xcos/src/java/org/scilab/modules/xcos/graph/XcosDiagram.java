/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2015 - Scilab Enterprises - Clement DAVID
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.xcos.graph;

import org.scilab.modules.xcos.graph.model.XcosGraphModel;
import java.awt.GraphicsEnvironment;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
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
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.SaveAsAction;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.graph.model.BlockInterFunction;
import org.scilab.modules.xcos.graph.model.ScicosObjectOwner;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.io.XcosFileType;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.CommandControlLink;
import org.scilab.modules.xcos.link.ExplicitLink;
import org.scilab.modules.xcos.link.ImplicitLink;
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
import org.scilab.modules.xcos.utils.Stack;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosDialogs;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxUndoableEdit;
import com.mxgraph.util.mxUndoableEdit.mxUndoableChange;
import com.mxgraph.view.mxGraphSelectionModel;
import com.mxgraph.view.mxMultiplicity;
import java.lang.reflect.Constructor;
import java.rmi.server.UID;
import java.util.Hashtable;
import org.scilab.modules.xcos.io.ScilabTypeCoder;

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

    /**
     * Constructor
     *
     * @param controller the shared controller
     * @param diagramId the diagram MVC ID
     * @param kind DIAGRAM or BLOCK for a root diagram or a super-block
     * @param uid the string UID that will be used on the default parent
     */
    public XcosDiagram(final JavaController controller, final long diagramId, final Kind kind, String uid) {
        super(new XcosGraphModel(controller, diagramId, kind, uid), Xcos.getInstance().getStyleSheet());

        // set the default parent (the JGraphX layer) defined on the model
        setDefaultParent(getModel().getChildAt(getModel().getRoot(), 0));

        setComponent(new GraphComponent(this));
        initComponent();

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
         * by default every label is movable, see XcosDiagram##isLabelMovable(java.lang.Object) for restrictions
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

        // auto-position the diagram origin
        setAutoOrigin(true);

        // do not put loop links inside the common block cell but on the defaultParent
        ((mxGraphModel) getModel()).setMaintainEdgeParent(false);
    }

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

    /**
     * Fill the hierarchy from the first element up to the root diagram
     * (included)
     * <p>
     * Should be used as :
     * <pre>
     *  hierarchy = fillHierarchy(new ScicosObjectOwner(getUID(), getKind()))
     * </pre>
     *
     * @param hierarchy the collection to fill
     * @return the filled collection (the root at the end)
     */
    public static Stack<ScicosObjectOwner> lookForHierarchy(ScicosObjectOwner current) {
        ScicosObjectOwner local = current;
        Stack<ScicosObjectOwner> hierarchy = new Stack<>();
        JavaController controller = new JavaController();

        long[] parent = new long[] {local.getUID()};
        if (local.getKind() == Kind.DIAGRAM) {
            hierarchy.push(local);
            return hierarchy;
        }

        while (parent[0] != 0l) {
            hierarchy.push(new ScicosObjectOwner(parent[0], Kind.BLOCK));
            controller.getObjectProperty(local.getUID(), local.getKind(), ObjectProperties.PARENT_BLOCK, parent);
        }

        controller.getObjectProperty(local.getUID(), local.getKind(), ObjectProperties.PARENT_DIAGRAM, parent);
        hierarchy.push(new ScicosObjectOwner(parent[0], Kind.DIAGRAM));

        return hierarchy;
    }

    /**
     * Sort the blocks per first integer parameter value
     *
     * @param blocks the block list
     * @return the sorted block list (same instance)
     */
    public List<? extends BasicBlock> iparSort(final List<? extends BasicBlock> blocks) {
        Collections.sort(blocks, new Comparator<BasicBlock>() {
            @Override
            public int compare(BasicBlock o1, BasicBlock o2) {

                JavaController controller = new JavaController();

                final VectorOfInt data1 = new VectorOfInt();
                final VectorOfInt data2 = new VectorOfInt();

                controller.getObjectProperty(o1.getUID(), Kind.BLOCK, ObjectProperties.IPAR, data1);
                controller.getObjectProperty(o2.getUID(), Kind.BLOCK, ObjectProperties.IPAR, data2);

                final int value1;
                if (data1.size() >= 1) {
                    value1 = data1.get(0);
                } else {
                    value1 = 0;
                }

                final int value2;
                if (data2.size() >= 1) {
                    value2 = data2.get(0);
                } else {
                    value2 = 0;
                }

                return value1 - value2;
            }
        });
        return blocks;
    }

    /**
     * @param <T> The type to work on
     * @param klass the class instance to work on
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
     * @param <T> The type to work on
     * @param klasses the class instance list to work on
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
     * @param context the context to fill
     */
    @SuppressWarnings("unchecked")
    protected void fillContext(final Hashtable<Object, Object> context) {
        if (!context.containsKey(IN)) {
            context.put(IN, iparSort(getAllTypedBlock(new Class[] {ExplicitInBlock.class, ImplicitInBlock.class})));
        }
        if (!context.containsKey(OUT)) {
            context.put(OUT, iparSort(getAllTypedBlock(new Class[] {ExplicitOutBlock.class, ImplicitOutBlock.class})));
        }
        if (!context.containsKey(EIN)) {
            context.put(EIN, iparSort(getAllTypedBlock(new Class[] {EventInBlock.class})));
        }
        if (!context.containsKey(EOUT)) {
            context.put(EOUT, iparSort(getAllTypedBlock(new Class[] {EventOutBlock.class})));
        }
    }

    @Override
    public String validateCell(final Object cell, final Hashtable<Object, Object> context) {
        if (getKind() == Kind.BLOCK) {
            return validateChildDiagram(cell, context);
        } else {
            // does not perform any validation on a root diagram
            return null;
        }
    }

    /**
     * Validate I/O ports.
     *
     * /!\ No model modification should be made in this method, this is only a
     * validation method.
     *
     * @param cell Cell that represents the cell to validate.
     * @param context Hashtable that represents the global validation state.
     */
    public String validateChildDiagram(final Object cell, final Hashtable<Object, Object> context) {
        String err = null;

        /*
         * Only validate I/O blocks
         */
        // get the key
        final String key;
        if (cell instanceof ExplicitInBlock || cell instanceof ImplicitInBlock) {
            key = IN;
        } else if (cell instanceof ExplicitOutBlock || cell instanceof ImplicitOutBlock) {
            key = OUT;
        } else if (cell instanceof EventInBlock) {
            key = EIN;
        } else if (cell instanceof EventOutBlock) {
            key = EOUT;
        } else {
            return null;
        }
        final BasicBlock block = (BasicBlock) cell;

        /*
         * Prepare validation
         */
        // fill the context once
        fillContext(context);

        /*
         * Validate with ipar
         */
        // get the real index
        final List<? extends BasicBlock> blocks = (List<? extends BasicBlock>) context.get(key);
        final int realIndex = blocks.indexOf(block) + 1;

        // get the user index
        JavaController controller = new JavaController();
        VectorOfInt ipar = new VectorOfInt();
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);
        if (ipar.size() < 1) {
            return err;
        }
        final int userIndex = ipar.get(0);

        // if the indexes are not equals, alert the user.
        if (realIndex != userIndex) {
            final StringBuilder str = new StringBuilder();
            str.append("<html><body><em>");
            str.append(XcosMessages.WRONG_PORT_NUMBER);
            str.append("</em><br/>");
            str.append(String.format(XcosMessages.EXPECTING_NUMBER, realIndex));
            str.append("</body></html>    ");

            err = str.toString();
        }

        return err;
    }

    /*
     * Static diagram listeners
     */
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
         * @param source the source instance
         * @param evt the event data
         * @see
         * com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         * com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final XcosDiagram diagram = (XcosDiagram) source;
            final Object[] cells = (Object[]) evt.getProperty(CELLS);

            diagram.getModel().beginUpdate();
            try {
                for (int i = 0; i < cells.length; ++i) {
                    if (cells[i] instanceof BasicBlock) {
                        BlockPositioning.updateBlockView(diagram, (BasicBlock) cells[i]);
                    }
                }
            } finally {
                diagram.getModel().endUpdate();
            }
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
         * @param source the source instance
         * @param evt the event data
         * @see
         * com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         * com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(final Object source, final mxEventObject evt) {
            final mxUndoableEdit edit = (mxUndoableEdit) evt.getProperty(ScilabGraphConstants.EVENT_CHANGE_EDIT);

            final mxGraphModel model = (mxGraphModel) edit.getSource();
            final List<mxUndoableChange> changes = edit.getChanges();

            final Object[] changedCells = getSelectionCellsForChanges(changes, model);
            model.beginUpdate();
            try {
                // FIXME manage that for the MVC
                // for (final Object object : changedCells) {
                // if (object instanceof BasicBlock) {
                // final BasicBlock current = (BasicBlock) object;
                // final XcosDiagram graph = current.getParentDiagram();
                //
                // // When we change the style property we have to update
                // // some BasiBlock fields
                // if (changes.get(0) instanceof mxStyleChange) {
                // current.updateFieldsFromStyle();
                // }
                //
                // // update the superblock container ports if the block is
                // // inside a superblock diagram
                // if (graph instanceof SuperBlockDiagram) {
                // SuperBlockDiagram superdiagram = (SuperBlockDiagram) current.getParentDiagram();
                // SuperBlock superblock = superdiagram.getContainer();
                // superblock.updateExportedPort();
                // }
                //
                // // Update the block position
                // BlockPositioning.updateBlockView(current);
                //
                // // force a refresh of the block ports and links
                // // connected to these ports
                // final int childCount = model.getChildCount(current);
                // for (int i = 0; i < childCount; i++) {
                // final Object port = model.getChildAt(current, i);
                // graph.getView().clear(port, true, true);
                // final int edgeCount = model.getEdgeCount(port);
                // for (int j = 0; j < edgeCount; j++) {
                // final Object edge = model.getEdgeAt(port, j);
                // graph.getView().clear(edge, true, true);
                // }
                // }
                // // force a refresh of the block
                // graph.getView().clear(current, true, true);
                //
                // graph.getView().validate();
                // graph.repaint();
                // }
                // }
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
         * @param sender the diagram
         * @param evt the event
         * @see
         * com.mxgraph.util.mxEventSource.mxIEventListener#invoke(java.lang.Object,
         * com.mxgraph.util.mxEventObject)
         */
        @Override
        public void invoke(Object sender, mxEventObject evt) {
            final XcosDiagram diagram = (XcosDiagram) sender;

            diagram.getModel().beginUpdate();
            try {
                final BasicBlock updatedBlock = (BasicBlock) evt.getProperty(XcosConstants.EVENT_BLOCK_UPDATED);
                BlockPositioning.updateBlockView(diagram, updatedBlock);

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
     * @param parent Cell that specifies the parent of the new edge.
     * @param id Optional string that defines the Id of the new edge.
     * @param value Object to be used as the user object.
     * @param source Cell that defines the source of the edge.
     * @param target Cell that defines the target of the edge.
     * @param style Optional string that defines the cell style.
     * @return Returns the new edge to be inserted.
     * @see com.mxgraph.view.mxGraph#createEdge(java.lang.Object,
     * java.lang.String, java.lang.Object, java.lang.Object, java.lang.Object,
     * java.lang.String)
     */
    @Override
    public Object createEdge(Object parent, String id, Object value, Object source, Object target, String style) {
        Object ret = null;
        JavaController controller = new JavaController();

        if (source instanceof BasicPort) {
            BasicPort src = (BasicPort) source;
            BasicLink link = null;

            long uid = controller.createObject(Kind.LINK);
            if (src.getType() == Type.EXPLICIT) {
                link = new ExplicitLink(controller, uid, Kind.LINK, value, null, style, id);
            } else if (src.getType() == Type.IMPLICIT) {
                link = new ImplicitLink(controller, uid, Kind.LINK, value, null, style, id);
            } else {
                link = new CommandControlLink(controller, uid, Kind.LINK, value, null, style, id);
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
                Class<? extends BasicLink> klass = src.getClass();

                // call XXXXLink(JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id)
                Constructor<? extends BasicLink> cstr = klass.getConstructor(JavaController.class, Long.TYPE, Kind.class, Object.class, mxGeometry.class, String.class, String.class);
                link = cstr.newInstance(controller, controller.createObject(Kind.LINK), src.getKind(), null, new mxGeometry(), src.getStyle(), new UID().toString());
            } catch (ReflectiveOperationException e) {
                LOG.severe(e.toString());
            }

            ret = link;
        }

        if (ret == null) {
            LOG.warning("Unable to create an edge");
        }

        return ret;
    }

    /**
     * Add an edge from a source to the target.
     *
     * @param cell the edge to add (may be null)
     * @param parent the parent of the source and the target
     * @param source the source cell
     * @param target the target cell
     * @param index the index of the edge
     * @return the added edge or null.
     * @see com.mxgraph.view.mxGraph#addEdge(java.lang.Object, java.lang.Object,
     * java.lang.Object, java.lang.Object, java.lang.Integer)
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
     * @param splitPoint the split point (center of the split block)
     * @param link source link
     * @return split block
     */
    public SplitBlock addSplitEdge(final mxPoint splitPoint, final BasicLink link) {
        final BasicPort linkSource = (BasicPort) link.getSource();
        final BasicPort linkTarget = (BasicPort) link.getTarget();

        /*
         * Select the right split accordingly to the link klass
         */
        BlockInterFunction f;
        if (link instanceof CommandControlLink) {
            f = BlockInterFunction.CLKSPLIT_f;
        } else if (link instanceof ImplicitLink) {
            f = BlockInterFunction.IMPSPLIT_f;
        } else {
            f = BlockInterFunction.SPLIT_f;
        }

        final SplitBlock splitBlock;
        try {
            splitBlock = (SplitBlock) XcosCellFactory.createBlock(f);
        } catch (InterpreterException ex) {
            // something goes wrong
            throw new RuntimeException(ex);
        }

        // snap the center of the split block on the grid
        mxGeometry geom = splitBlock.getGeometry();
        double x = snap(splitPoint.getX()) - (SplitBlock.DEFAULT_SIZE / 2.);
        double y = snap(splitPoint.getY()) - (SplitBlock.DEFAULT_SIZE / 2.);
        geom.setX(x);
        geom.setY(y);
        splitBlock.setGeometry(geom);

        getModel().beginUpdate();
        try {
            // Origin of the parent, (0,0) as default may be different in case
            mxPoint orig = link.getParent().getGeometry();
            if (orig == null) {
                orig = new mxPoint();
            }

            addCell(splitBlock);

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

            getModel().remove(link);
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
     * @param src the source port
     * @param trg the target port
     * @param points the points
     * @param orig the origin point (may be (0,0))
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
    public final void initComponent() {
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
    }

    /**
     * Install the multiplicities (use for link checking)
     */
    private void setMultiplicities() {
        final List<mxMultiplicity> multiplicities = new ArrayList<mxMultiplicity>();

        // Input data port
        multiplicities.add(new PortCheck(ExplicitInputPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = -4987163442006736665L;

            {
                add(ExplicitOutputPort.class);
                add(ExplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_EXPLICIT_IN));
        multiplicities.add(new PortCheck(ImplicitInputPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 886376532181210926L;

            {
                add(ImplicitOutputPort.class);
                add(ImplicitInputPort.class);
                add(ImplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_IMPLICIT_IN));

        // Output data port
        multiplicities.add(new PortCheck(ExplicitOutputPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 4594127972486054821L;

            {
                add(ExplicitInputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EXPLICIT_OUT));
        multiplicities.add(new PortCheck(ImplicitOutputPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = -3719677806532507973L;

            {
                add(ImplicitInputPort.class);
                add(ImplicitOutputPort.class);
                add(ImplicitLink.class);
            }
        }), XcosMessages.LINK_ERROR_IMPLICIT_OUT));

        // Control port
        multiplicities.add(new PortCheck(ControlPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 2941077191386058497L;

            {
                add(CommandPort.class);
                add(CommandControlLink.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_IN));

        // Command port
        multiplicities.add(new PortCheck(CommandPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = -3470370027962480362L;

            {
                add(ControlPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // ExplicitLink connections
        multiplicities.add(new PortCheck(ExplicitLink.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 7423543162930147373L;

            {
                add(ExplicitInputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // ImplicitLink connections
        multiplicities.add(new PortCheck(ImplicitLink.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 7775100011122283282L;

            {
                add(ImplicitInputPort.class);
                add(ImplicitOutputPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // CommandControlLink connections
        multiplicities.add(new PortCheck(CommandControlLink.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
            private static final long serialVersionUID = 3260421433507192386L;

            {
                add(ControlPort.class);
            }
        }), XcosMessages.LINK_ERROR_EVENT_OUT));

        // Already connected port
        multiplicities.add(new PortCheck(BasicPort.class, Collections.unmodifiableList(new ArrayList<Class<? extends mxCell>>() {
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
        removeListener(CellResizedTracker.getInstance());
        getUndoManager().removeListener(UndoUpdateTracker.getInstance());
        removeListener(RefreshBlockTracker.getInstance());

        // Track when resizing a cell.
        addListener(mxEvent.CELLS_RESIZED, CellResizedTracker.getInstance());

        // Update the blocks view on undo/redo
        getUndoManager().addListener(mxEvent.UNDO, UndoUpdateTracker.getInstance());
        getUndoManager().addListener(mxEvent.REDO, UndoUpdateTracker.getInstance());

    }

    /**
     * Translate the cell and align any split block
     *
     * @param cell any object
     * @param dx the X delta
     * @param dy the Y delta
     */
    @Override
    public void translateCell(Object cell, double dx, double dy) {
        if (cell instanceof SplitBlock) {
            mxGeometry geom = model.getGeometry(cell);

            final double posX = snap(geom.getX() + dx) - (SplitBlock.DEFAULT_SIZE / 2.);
            final double posY = snap(geom.getY() + dy) - (SplitBlock.DEFAULT_SIZE / 2.);

            dx = posX - geom.getX();
            dy = posY - geom.getY();
        }

        super.translateCell(cell, dx, dy);
    }

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
     * @param terminal the current terminal (instance of BasicPort)
     * @param removedCells the "to be removed" set
     * @param loopCells the "while loop" set
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
     * @param linkSource the normal source link
     * @param linkTerminal the normal target link
     * @param removedCells the set of removed objects
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
     * @param cell the selected cell
     * @param collapse the collapse settings
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
     * @param cell the cell
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
     *
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
     * A cell is deletable if it is not a locked block or an identifier cell
     *
     * @param cell the cell
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
     * @param cell the cell
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
     * @param cell the cell to check
     * @return the identifier cell
     */
    public mxCell getOrCreateCellIdentifier(final mxCell cell) {
        if (cell.getId().endsWith(HASH_IDENTIFIER)) {
            return cell;
        }

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
     * @param cell the cell to check
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
     * @param cell the cell
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
     * @param cell the cell object
     * @return a representative the string (block name) or a style specific
     * style.
     * @see com.mxgraph.view.mxGraph#convertValueToString(java.lang.Object)
     */
    @Override
    public String convertValueToString(Object cell) {
        String ret = null;

        if (cell != null) {
            JavaController controller = new JavaController();
            final Map<String, Object> style = getCellStyle(cell);

            final String displayedLabel = (String) style.get("displayedLabel");
            if (displayedLabel != null) {
                if (cell instanceof BasicBlock) {
                    BasicBlock block = (BasicBlock) cell;
                    VectorOfDouble v = new VectorOfDouble();
                    controller.getObjectProperty(block.getUID(), block.getKind(), ObjectProperties.EXPRS, v);

                    try {
                        ret = new ScilabTypeCoder().format(displayedLabel, v);
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
                    BasicBlock block = (BasicBlock) cell;

                    String[] interfaceFunction = new String[1];
                    controller.getObjectProperty(block.getUID(), block.getKind(), ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);
                    ret = interfaceFunction[0];
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
     * @return the model ID
     */
    public long getUID() {
        return ((XcosCell) getDefaultParent()).getUID();
    }

    /**
     * @return Kind.DIAGRAM or Kind.BLOCK
     */
    public Kind getKind() {
        return ((XcosCell) getDefaultParent()).getKind();
    }

    /**
     * Manage the visibility of the grid and the associated menu
     *
     * @param status new status
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
        return saveDiagramAs(getSavedFile());
    }

    /**
     * @param fileName diagram filename
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
                     * Escape file to handle not supported character in file name (may be Windows only) see http://msdn.microsoft.com/en -us/library/windows/desktop/aa365247%28v=vs.85%29.aspx
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
         * Look for the user extension if it does not exists, append a default one
         *
         * if the specified extension is handled, update the save format ; else append a default extension and use the default format
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
         * If the file exists, ask for confirmation if this is not the previously saved file
         */
        if (writeFile.exists() && !writeFile.equals(getSavedFile())) {
            final boolean overwrite = ScilabModalDialog.show(XcosTab.get(this), XcosMessages.OVERWRITE_EXISTING_FILE, XcosMessages.XCOS, IconType.QUESTION_ICON,
                                      ButtonType.YES_NO) == AnswerOption.YES_OPTION;

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

        updateTabTitle();
        info(XcosMessages.EMPTY_INFO);
        return isSuccess;
    }

    /**
     * Perform post loading initialization.
     *
     * @param file the loaded file
     */
    public void postLoad(final File file) {
        final String name = file.getName();

        if (XcosFileType.getAvailableSaveFormats().contains(XcosFileType.findFileType(file))) {
            setSavedFile(file);
        }
        setTitle(name.substring(0, name.lastIndexOf('.')));
        setModified(false);
        updateTabTitle();

        fireEvent(new mxEventObject(mxEvent.ROOT));

        info(XcosMessages.EMPTY_INFO);
    }

    @Override
    public void setSavedFile(File savedFile) {
        super.setSavedFile(savedFile);

        if (savedFile != null) {
            JavaController controller = new JavaController();
            controller.setObjectProperty(getUID(), getKind(), ObjectProperties.PATH, savedFile.getAbsolutePath());
        }
    }

    /**
     * Set the title of the diagram
     *
     * @param title the title
     * @see org.scilab.modules.graph.ScilabGraph#setTitle(java.lang.String)
     */
    @Override
    public void setTitle(final String title) {
        super.setTitle(title);

        JavaController controller = new JavaController();
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.TITLE, title);
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
        if (getKind() == Kind.DIAGRAM) {
            final File savedFile = getSavedFile();
            if (savedFile != null) {
                try {
                    final String path = savedFile.getCanonicalPath();
                    formattedPath = new StringBuilder().append(" (").append(path).append(')');
                } catch (final IOException e) {
                    LOG.warning(e.toString());
                }
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
     * Load a file with different method depending on it extension
     *
     * @param controller the used controller
     * @param file File to load (can be null)
     */
    public void transformAndLoadFile(final JavaController controller, final String file) {
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
                        } else {
                            XcosCellFactory.insertChildren(controller, XcosDiagram.this);
                        }

                        instance.setLastError("");
                    } catch (Exception e) {
                        e.printStackTrace();

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
     * Getting the root diagram of a decomposed diagram
     *
     * @return Root parent of the whole parent
     */
    public XcosDiagram getRootDiagram() {
        if (getKind() == Kind.DIAGRAM) {
            return this;
        }

        JavaController controller = new JavaController();
        long[] parent = new long[1];
        controller.getObjectProperty(getUID(), getKind(), ObjectProperties.PARENT_DIAGRAM, parent);

        Collection<XcosDiagram> diagrams = Xcos.getInstance().getDiagrams(parent[0]);
        return diagrams.stream().filter(d -> d.getUID() == parent[0]).findFirst().get();
    }

    /**
     * Returns the tooltip to be used for the given cell.
     *
     * @param cell block
     * @return cell tooltip
     */
    @Override
    public String getToolTipForCell(final Object cell) {
        if (cell instanceof BasicBlock) {
            return getToolTipForCell((BasicBlock) cell);
        } else if (cell instanceof BasicPort) {
            return getToolTipForCell((BasicPort) cell);
        } else if (cell instanceof BasicLink) {
            return getToolTipForCell((BasicLink) cell);
        }
        return "";
    }

    private String getToolTipForCell(final BasicBlock o) {
        JavaController controller = new JavaController();
        String[] strValue = {""};
        VectorOfDouble vecValue = new VectorOfDouble();
        VectorOfInt vecInteger = new VectorOfInt();

        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.INTERFACE_FUNCTION, strValue);
        result.append(XcosMessages.TOOLTIP_BLOCK).append(ScilabGraphConstants.HTML_BEGIN_CODE)
        .append(strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.SIM_FUNCTION_NAME, strValue);
        result.append(XcosMessages.TOOLTIP_BLOCK_SIMULATION).append(ScilabGraphConstants.HTML_BEGIN_CODE)
        .append(strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.UID, strValue);
        result.append(XcosMessages.TOOLTIP_BLOCK_UID).append(ScilabGraphConstants.HTML_BEGIN_CODE)
        .append(strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.STYLE, strValue);
        result.append(XcosMessages.TOOLTIP_BLOCK_STYLE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        result.append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.RPAR, vecValue);
        result.append(XcosMessages.TOOLTIP_BLOCK_RPAR).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, ScilabTypeCoder.toString(vecValue))
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.IPAR, vecInteger);
        result.append(XcosMessages.TOOLTIP_BLOCK_IPAR).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, ScilabTypeCoder.toString(vecInteger))
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.OPAR, vecValue);
        result.append(XcosMessages.TOOLTIP_BLOCK_OPAR).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, ScilabTypeCoder.toString(vecValue))
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    private String getToolTipForCell(final BasicPort o) {
        JavaController controller = new JavaController();
        boolean[] boolValue = {false};
        String[] strValue = {""};
        VectorOfInt intVecValue = new VectorOfInt();

        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.DATATYPE, intVecValue);
        result.append(XcosMessages.TOOLTIP_PORT_DATATYPE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        formatDatatype(result, intVecValue)
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.IMPLICIT, boolValue);
        result.append(XcosMessages.TOOLTIP_PORT_IMPLICIT).append(ScilabGraphConstants.HTML_BEGIN_CODE)
        .append(boolValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.STYLE, strValue);
        result.append(XcosMessages.TOOLTIP_PORT_STYLE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    private String getToolTipForCell(final BasicLink o) {
        JavaController controller = new JavaController();
        long[] longValue = {0l};
        boolean[] boolValue = {false};
        String[] strValue = {""};
        VectorOfInt intVecValue = new VectorOfInt();

        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.SOURCE_PORT, longValue);
        if (longValue[0] != 0l) {
            controller.getObjectProperty(longValue[0], Kind.PORT, ObjectProperties.DATATYPE, intVecValue);
            result.append(XcosMessages.TOOLTIP_LINK_SRC_DATATYPE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
            formatDatatype(result, intVecValue)
            .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);
        }
        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.DESTINATION_PORT, longValue);
        if (longValue[0] != 0l) {
            controller.getObjectProperty(longValue[0], Kind.PORT, ObjectProperties.DATATYPE, intVecValue);
            result.append(XcosMessages.TOOLTIP_LINK_TRG_DATATYPE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
            formatDatatype(result, intVecValue)
            .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);
        }

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.LABEL, strValue);
        result.append(XcosMessages.TOOLTIP_LINK_LABEL).append(ScilabGraphConstants.HTML_BEGIN_CODE)
        .append(strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.STYLE, strValue);
        result.append(XcosMessages.TOOLTIP_LINK_STYLE).append(ScilabGraphConstants.HTML_BEGIN_CODE);
        appendReduced(result, strValue[0])
        .append(ScilabGraphConstants.HTML_END_CODE).append(ScilabGraphConstants.HTML_NEWLINE);

        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    private StringBuilder appendReduced(final StringBuilder result, final String msg) {
        if (msg.length() > XcosConstants.MAX_CHAR_IN_STYLE) {
            result.append(msg.substring(0, XcosConstants.MAX_CHAR_IN_STYLE));
            result.append(XcosMessages.DOTS);
        } else {
            result.append(msg);
        }

        return result;
    }

    private StringBuilder formatDatatype(final StringBuilder result, final VectorOfInt intVecValue) {
        if (intVecValue.size() != 3) {
            result.append(ScilabTypeCoder.toString(intVecValue));
        } else {
            // this is a known encoding, output representative strings
            int rows = intVecValue.get(0);
            int cols = intVecValue.get(1);
            int type = intVecValue.get(2);

            String strType;
            // should be similar to the naming used on scicos_model doc
            String[] typeTable = {"real", "complex", "int32", "int16", "int8", "uint32", "uint16", "uint8"};
            if (0 <= type && type < typeTable.length) {
                strType = typeTable[type - 1];
            } else {
                strType = "auto";
            }

            result.append(String.format("%s [%d %d]", strType, rows, cols));
        }

        return result;
    }

    /**
     * Display the message in info bar.
     *
     * @param message Information
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
     * @param message Error of the message
     */
    public void error(final String message) {
        JOptionPane.showMessageDialog(getAsComponent(), message, XcosMessages.XCOS, JOptionPane.ERROR_MESSAGE);
    }

    /**
     * Find the block corresponding to the given uid and display a warning
     * message.
     *
     * @param uid - A String as UID.
     * @param message - The message to display.
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

    @Override
    public File getSavedFile() {
        if (getKind() == Kind.DIAGRAM) {
            return super.getSavedFile();
        } else {
            return getRootDiagram().getSavedFile();
        }
    }

    /**
     * Read the applicable context on this diagram.
     * <p>
     * This function retrieve the current diagram's context and all its parent
     *
     * @return the full context
     */
    public String[] getContext() {
        final ArrayList<String> allContext = new ArrayList<>();
        final Stack<ScicosObjectOwner> hierarchy = lookForHierarchy(new ScicosObjectOwner(getUID(), getKind()));

        final JavaController controller = new JavaController();
        final VectorOfString context = new VectorOfString();

        hierarchy.stream().forEach(o -> {
            controller.getObjectProperty(o.getUID(), o.getKind(), ObjectProperties.DIAGRAM_CONTEXT, context);

            final int length = context.size();
            for (int i = 0; i < length; i++) {
                allContext.add(context.get(i));
            }
            allContext.add("");
        });

        return allContext.toArray(new String[allContext.size()]);
    }

    /**
     * Evaluate the current context
     *
     * @return The resulting data. Keys are variable names and Values are
     * evaluated values.
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
     * Returns true if the given cell is a not a block nor a port.
     *
     * @param cell the drop target
     * @param cells the cells to be dropped
     * @return the drop status
     * @see com.mxgraph.view.mxGraph#isValidDropTarget(java.lang.Object,
     * java.lang.Object[])
     */
    @Override
    public boolean isValidDropTarget(final Object cell, final Object[] cells) {
        return !(cell instanceof BasicBlock) && !(cell instanceof BasicBlock) && !(cell instanceof BasicPort) && super.isValidDropTarget(cell, cells);
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
             * @see
             * com.mxgraph.view.mxGraphSelectionModel#setCell(java.lang.Object)
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
