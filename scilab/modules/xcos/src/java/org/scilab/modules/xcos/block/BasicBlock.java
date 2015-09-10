/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.awt.MouseInfo;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.actions.EditFormatAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;

/**
 * A block on the diagram
 */
@SuppressWarnings(value = { "serial" })
public class BasicBlock extends ScilabGraphUniqueObject implements Serializable {
    /**
     * Sorted kind of input, useful to sort them by kind
     */
    private static final Class<?>[] sortedChildrenClass = {InputPort.class, OutputPort.class, ControlPort.class, CommandPort.class, Object.class};

    /*
     * Default values
     */

    /**
     * Default interface function name
     */
    public static final String DEFAULT_INTERFACE_FUNCTION = "xcos_block";
    /**
     * Default simulation function name
     */
    public static final String DEFAULT_SIMULATION_FUNCTION = "xcos_simulate";

    /*
     * Local constants
     */

    private static final double DEFAULT_POSITION_X = 10.0;
    private static final double DEFAULT_POSITION_Y = 10.0;
    private static final double DEFAULT_WIDTH = 40.0;
    private static final double DEFAULT_HEIGHT = 40.0;

    /**
     * Sort the children list in place.
     *
     * The sort put inputs then outputs the control then command ports. The
     * local port order is preserved.The sort is performed in place and do not
     * emit any event.
     *
     *
     * @param children
     *            the children to sort
     */
    public static final void sort(List<?> children) {
        final List<Object> reference = new ArrayList<Object>(children);

        Collections.sort(children, new Comparator<Object>() {
            @Override
            public int compare(Object o1, Object o2) {
                // diff is the major sorting by kind
                int diff = compareByChildClass(o1, o2);
                if (o1 instanceof BasicPort && o2 instanceof BasicPort) {
                    // first sort with the port list index
                    final int diffIndexOf = Integer.signum(reference.indexOf(o1) - reference.indexOf(o2));
                    // then sort with the ordering value
                    final int diffOrdering = Integer.signum(((BasicPort) o1).getOrdering() - ((BasicPort) o2).getOrdering());
                    // then sort with the port position value
                    final mxGeometry o1Geom = ((BasicPort) o1).getGeometry();
                    final mxGeometry o2Geom = ((BasicPort) o2).getGeometry();
                    final int diffPosition = Integer.signum((int) (o2Geom.getX() - o1Geom.getX() - o2Geom.getY() + o1Geom.getY()));

                    // voting is performed with these equivalent 3 selector
                    diff = diff + diffIndexOf + diffOrdering + diffPosition;
                }

                return diff;
            }
        });
    }

    /**
     * Internal method to get a base index to compare with depending on the cell
     * type.
     *
     * @param cell
     *            the cell
     * @return the base index
     */
    private static final int compareByChildClass(final Object o1, final Object o2) {
        int o1Index = 0;
        int o2Index = 0;

        for (int i = 0; i < sortedChildrenClass.length; i++) {
            final Class<?> klass = sortedChildrenClass[i];

            if (klass.isInstance(o1)) {
                o1Index = i;
                break;
            }
        }
        for (int i = 0; i < sortedChildrenClass.length; i++) {
            final Class<?> klass = sortedChildrenClass[i];

            if (klass.isInstance(o2)) {
                o2Index = i;
                break;
            }
        }

        final int base = o1Index - o2Index;
        return base * (Integer.MAX_VALUE / sortedChildrenClass.length);
    }

    private boolean locked;
    private final long uid;

    /**
     * Represent a simulation function type compatible with Scilab/Scicos
     * function type descriptors.
     */
    public static enum SimulationFunctionType {
        /** event select; reduced at compilation */
        ESELECT(-2.0),
        /** if then else; reduced at compilation */
        IFTHENELSE(-1.0),
        /** first common block */
        DEFAULT(0.0),
        /** first native block */
        TYPE_1(1.0),
        /** second native block */
        TYPE_2(2.0),
        /** third native block */
        TYPE_3(3.0),
        /** forth native block */
        C_OR_FORTRAN(4.0),
        /** Scilab blocks */
        SCILAB(5.0),
        /** Debug blocks */
        DEBUG(99),
        /** dynamic {@link #TYPE_1} Fortran blocks (fortran_block.sci) */
        DYNAMIC_FORTRAN_1(1001.0),
        /** dynamic {@link #TYPE_1} C blocks (c_block.sci) */
        DYNAMIC_C_1(2001.0),
        /** Explicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_EXPLICIT_4(2004.0),
        /** Implicit {@link #TYPE_1} Fortran blocks (DIFF_f.sci) */
        OLDBLOCKS(10001.0),
        /** Implicit {@link #C_OR_FORTRAN} blocks */
        IMPLICIT_C_OR_FORTRAN(10004.0),
        /** Implicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_IMPLICIT_4(12004.0),
        /** Modelica {@link #C_OR_FORTRAN} blocks */
        MODELICA(30004.0),
        /** Magic types */
        UNKNOWN(5.0);

        private double value;

        /**
         * Default constructor
         *
         * @param scilabValue
         *            Scilab/Scicos function type descriptor
         */
        private SimulationFunctionType(double scilabValue) {
            value = scilabValue;
        }

        /**
         * Get the Java descriptor from the Scilab descriptor.
         *
         * @param scilabValue
         *            Scilab/Scicos function type descriptor
         * @return The corresponding java descriptor
         */
        public static SimulationFunctionType convertScilabValue(int scilabValue) {
            for (SimulationFunctionType iter : SimulationFunctionType.values()) {
                if (iter.getAsDouble() == scilabValue) {
                    return iter;
                }
            }
            return UNKNOWN;
        }

        /**
         * Get the Scilab Descriptor from the Java Descriptor
         *
         * @return The corresponding Scilab/Scicos descriptor
         */
        public double getAsDouble() {
            return value;
        }
    };

    /**
     * Default constructor.
     */
    public BasicBlock(long uid) {
        super();

        /*
         * Default parameters for blocks
         */
        this.visible = true;
        this.vertex = true;
        this.connectable = false;
        this.geometry = new mxGeometry(DEFAULT_POSITION_X, DEFAULT_POSITION_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT);

        this.uid = uid;
    }

    /**
     * @return associated MVC ID
     */
    public long getUID() {
        return uid;
    }

    /**
     * @return the parent diagram of this graphical object
     */
    public XcosDiagram getParentDiagram() {
        /*
         * Retrieve the parent
         */
        long[] parentBlock = new long[0];
        long[] parentDiagram = new long[0];
        JavaController controller = new JavaController();
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.PARENT_BLOCK, parentBlock);
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.PARENT_DIAGRAM, parentDiagram);

        final long parent;
        final Kind kind;
        if (parentBlock[0] == 0l) {
            parent = parentDiagram[0];
            kind = Kind.DIAGRAM;
        } else {
            parent = parentBlock[0];
            kind = Kind.BLOCK;
        }

        /*
         * Retrieve and create on demand the corresponding Diagram
         */
        XcosDiagram diagram;
        Collection<XcosDiagram> diagrams = Xcos.getInstance().getDiagrams(parentDiagram[0]);
        Optional<XcosDiagram> optDiagram = diagrams.stream().filter(d -> d.getUId() == parent).findFirst();
        if (optDiagram.isPresent()) {
            diagram = optDiagram.get();
        } else {
            diagram = new XcosDiagram(parent, kind);
            Xcos.getInstance().addDiagram(parentDiagram[0], diagram);
        }

        return diagram;
    }

    /**
     * @return locked status
     */
    public synchronized boolean isLocked() {
        return locked;
    }

    /**
     * @param locked
     *            change locked status
     */
    public synchronized void setLocked(boolean locked) {
        this.locked = locked;
    }

    /**
     * Update the children of the block.
     *
     * @param modifiedBlock
     *            the new block instance
     */
    private void updateChildren(BasicBlock modifiedBlock) {
        //        if (modifiedBlock == null) {
        //            return;
        //        }
        //
        //        XcosDiagram graph = getParentDiagram();
        //        if (graph == null) {
        //            setParentDiagram(Xcos.findParent(this));
        //            graph = getParentDiagram();
        //            LOG.finest(PARENT_DIAGRAM_WAS_NULL);
        //        }
        //
        //        /*
        //         * Checked as port classes only
        //         */
        //        @SuppressWarnings("unchecked")
        //        Set < Class <? extends mxICell >> types = new HashSet < Class <? extends mxICell >> (Arrays.asList(InputPort.class, OutputPort.class, ControlPort.class,
        //                CommandPort.class));
        //
        //        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedOlds = getTypedChildren(types);
        //        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedNews = modifiedBlock.getTypedChildren(types);
        //
        //        getParentDiagram().getModel().beginUpdate();
        //        try {
        //            for (Class <? extends mxICell > klass : types) {
        //                final Deque<mxICell> olds = annotatedOlds.get(klass);
        //                final Deque<mxICell> news = annotatedNews.get(klass);
        //
        //                // updated ports
        //                while (!olds.isEmpty() && !news.isEmpty()) {
        //                    mxICell previous = olds.poll();
        //                    mxICell modified = news.poll();
        //
        //                    final int previousIndex = children.indexOf(previous);
        //
        //                    // relink
        //                    if (previous.getEdgeCount() != 0) {
        //                        final mxICell edge = previous.getEdgeAt(0);
        //                        final boolean isOutgoing = previous == edge.getTerminal(true);
        //                        previous.removeEdge(edge, isOutgoing);
        //                        modified.insertEdge(edge, isOutgoing);
        //                    }
        //
        //                    getParentDiagram().removeCells(new Object[] { previous }, false);
        //                    getParentDiagram().addCells(new Object[] { modified }, this, previousIndex);
        //
        //                    // Clone the geometry to avoid empty geometry on new cells.
        //                    getParentDiagram().getModel().setGeometry(modified, (mxGeometry) previous.getGeometry().clone());
        //
        //                }
        //
        //                // removed ports
        //                if (!olds.isEmpty()) {
        //                    getParentDiagram().removeCells(olds.toArray(), true);
        //                }
        //
        //                // added ports
        //                if (!news.isEmpty()) {
        //                    getParentDiagram().addCells(news.toArray(), this);
        //                }
        //            }
        //        } finally {
        //            getParentDiagram().getModel().endUpdate();
        //        }
    }

    /**
     * Format the children as a typed map for the given class set.
     *
     * @param types
     *            the classes to search for.
     * @return a map which linked foreach type the corresponding cell list.
     */
    private Map < Class <? extends mxICell > , Deque<mxICell >> getTypedChildren(Set < Class <? extends mxICell >> types) {
        Map < Class <? extends mxICell > , Deque<mxICell >> oldPorts = new HashMap < Class <? extends mxICell > , Deque<mxICell >> ();

        // Allocate all types set
        for (Class <? extends mxICell > type : types) {
            oldPorts.put(type, new LinkedList<mxICell>());
        }

        if (getChildCount() <= 0) {
            return oldPorts;
        }

        // sort children according to the ordering parameter (useful on
        // scilab-5.2.x diagrams)
        sort(children);

        // children lookup
        for (Object cell : children) {

            Class <? extends Object > klass = cell.getClass();
            while (klass != null) {
                if (types.contains(klass)) {
                    break;
                }
                klass = klass.getSuperclass();
            }

            final Deque<mxICell> current = oldPorts.get(klass);
            if (current != null) {
                current.add((mxICell) cell);
            }
        }

        return oldPorts;
    }

    /**
     * Sort the children list in place.
     *
     * The sort put inputs then outputs the control then command ports. The
     * local port order is preserved.The sort is performed in place and do not
     * emit any event.
     */
    public void sortChildren() {
        if (getChildCount() <= 0) {
            return;
        }

        sort(children);
    }

    /**
     * @param context
     *            parent diagram context
     */
    public void openBlockSettings() {
        // FIXME: implement something
        //        final XcosDiagram graph;
        //        if (getParentDiagram() == null) {
        //            setParentDiagram(Xcos.findParent(this));
        //            graph = getParentDiagram();
        //            LOG.finest(PARENT_DIAGRAM_WAS_NULL);
        //        } else {
        //            graph = getParentDiagram();
        //        }
        //        if (graph instanceof PaletteDiagram) {
        //            return;
        //        }
        //
        //        if (context == null) {
        //            throw new IllegalArgumentException();
        //        }
        //
        //        // prevent to open twice
        //        if (isLocked()) {
        //            return;
        //        }
        //
        //        graph.setCellsLocked(true);
        //        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));
        //
        //        // sort children according to the ordering parameter (useful on
        //        // scilab-5.2.x diagrams)
        //        sortChildren();
        //
        //        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        //        if (handler == null) {
        //            return;
        //        }
        //
        //        try {
        //            // Write scs_m
        //            handler.writeBlock(this);
        //            // Write context
        //            handler.writeContext(context);
        //
        //            final ActionListener action = new ActionListener() {
        //                @Override
        //                public void actionPerformed(ActionEvent e) {
        //                    LOG.finest("Updating data.");
        //
        //                    graph.getView().clear(this, true, true);
        //
        //                    // Now read new Block
        //                    graph.getModel().beginUpdate();
        //                    try {
        //                        final BasicBlock modifiedBlock = handler.readBlock();
        //                        updateBlockSettings(modifiedBlock);
        //
        //                        graph.fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, BasicBlock.this));
        //                    } catch (ScicosFormatException ex) {
        //                        LOG.severe(ex.toString());
        //                    } finally {
        //                        graph.getModel().endUpdate();
        //                        setLocked(false);
        //
        //                        handler.release();
        //
        //                        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        //                        graph.setCellsLocked(false);
        //                    }
        //                }
        //            };
        //
        //            setLocked(true);
        //            ScilabInterpreterManagement.asynchronousScilabExec(action, "blk = xcosBlockInterface", getInterfaceFunctionName().toCharArray(), "set",
        //                    ScilabDirectHandler.BLK.toCharArray(), ScilabDirectHandler.CONTEXT.toCharArray());
        //        } catch (InterpreterException e) {
        //            LOG.severe(e.toString());
        //            setLocked(false);
        //
        //            handler.release();
        //
        //            graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
        //            graph.setCellsLocked(false);
        //        }
    }

    /**
     * @return tooltip text
     */
    public String getToolTipText() {
        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);

        JavaController controller = new JavaController();

        String[] interfaceFunctionName = new String[1];
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunctionName);
        String[] simulationFunctionName = new String[1];
        controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, simulationFunctionName);

        result.append("Block Name : " + interfaceFunctionName[0] + ScilabGraphConstants.HTML_NEWLINE);
        result.append("Simulation : " + simulationFunctionName[1] + ScilabGraphConstants.HTML_NEWLINE);

        //        if (getParentDiagram() instanceof PaletteDiagram) {
        //            if (getIntegerParameters() != null) {
        //                result.append("Integer parameters : " + getIntegerParameters() + ScilabGraphConstants.HTML_NEWLINE);
        //            }
        //
        //            if (getRealParameters() != null && getRealParameters().getHeight() != 0 && getRealParameters().getWidth() != 0) {
        //                result.append("Real parameters : " + getRealParameters() + ScilabGraphConstants.HTML_NEWLINE);
        //            }
        //
        //            if (getObjectsParameters() != null) {
        //                result.append("Object parameters : " + getObjectsParameters() + ScilabGraphConstants.HTML_NEWLINE);
        //            }
        //        } else {
        result.append("UID : " + getId() + ScilabGraphConstants.HTML_NEWLINE);
        final int length = getStyle().length();
        result.append("Style : ");
        if (length > XcosConstants.MAX_CHAR_IN_STYLE) {
            result.append(getStyle().substring(0, XcosConstants.MAX_CHAR_IN_STYLE));
            result.append(XcosMessages.DOTS);
        } else {
            result.append(getStyle());
        }
        result.append(ScilabGraphConstants.HTML_NEWLINE);
        result.append("Input ports : " + BasicBlockInfo.getAllTypedPorts(this, false, InputPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("Output ports : " + BasicBlockInfo.getAllTypedPorts(this, false, OutputPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("Control ports : " + BasicBlockInfo.getAllTypedPorts(this, false, ControlPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("Command ports : " + BasicBlockInfo.getAllTypedPorts(this, false, CommandPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
        //        }

        result.append("x : " + getGeometry().getX() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("y : " + getGeometry().getY() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("w : " + getGeometry().getWidth() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("h : " + getGeometry().getHeight() + ScilabGraphConstants.HTML_NEWLINE);
        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    /**
     * @param graph
     *            parent graph
     */
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = null;
        //        if (getParentDiagram() instanceof PaletteDiagram) {
        //            menu = createPaletteContextMenu(graph);
        //        } else {
        menu = createContextMenu(graph);
        //        }
        menu.setVisible(true);
    }

    /**
     * @param graph
     *            parent graph
     * @return context menu
     */
    // CSOFF: JavaNCSS
    public ContextMenu createPaletteContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();

        final List<XcosDiagram> allDiagrams = Xcos.getInstance().openedDiagrams();

        if (allDiagrams.size() == 0) {
            // No diagram opened: should never happen if Xcos opens an empty
            // diagram when it is launched
            MenuItem addTo = ScilabMenuItem.createMenuItem();

            addTo.setText(XcosMessages.ADDTO_NEW_DIAGRAM);
            addTo.setCallback(new CommonCallBack(XcosMessages.ADDTO_NEW_DIAGRAM) {
                @Override
                public void callBack() {
                    JavaController controller = new JavaController();
                    XcosDiagram theDiagram = new XcosDiagram(controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM);
                    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);

                    XcosTab.get(theDiagram).setVisible(true);
                    BlockPositioning.updateBlockView(block);
                }
            });

            menu.add(addTo);

        } else if (allDiagrams.size() == 1) {
            // A single diagram opened: add to this diagram
            MenuItem addTo = ScilabMenuItem.createMenuItem();

            addTo.setText(XcosMessages.ADDTO + " " + XcosTab.get(allDiagrams.get(0)).getName());
            final XcosDiagram theDiagram = allDiagrams.get(0);
            addTo.setCallback(new CommonCallBack(theDiagram.getTitle()) {
                private static final long serialVersionUID = -99601763227525686L;

                @Override
                public void callBack() {
                    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);
                    BlockPositioning.updateBlockView(block);
                }
            });

            menu.add(addTo);

        } else {
            // The user has to choose
            Menu addTo = ScilabMenu.createMenu();

            addTo.setText(XcosMessages.ADDTO);

            for (int i = 0; i < allDiagrams.size(); i++) {
                MenuItem diagram = ScilabMenuItem.createMenuItem();
                final XcosDiagram theDiagram = allDiagrams.get(i);
                diagram.setText(XcosTab.get(allDiagrams.get(i)).getName());
                diagram.setCallback(new CommonCallBack(theDiagram.getTitle()) {
                    private static final long serialVersionUID = 3345416658377835057L;

                    @Override
                    public void callBack() {
                        BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                        theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                        mxGeometry geom = BasicBlock.this.getGeometry();
                        setDefaultPosition(geom);
                        theDiagram.getModel().setGeometry(block, geom);
                        BlockPositioning.updateBlockView(block);
                    }
                });
                addTo.add(diagram);
            }

            menu.add(addTo);
        }

        menu.getAsSimpleContextMenu().addSeparator();

        MenuItem help = ScilabMenuItem.createMenuItem();
        help.setText(XcosMessages.BLOCK_DOCUMENTATION);
        help.setCallback(new CommonCallBack(XcosMessages.BLOCK_DOCUMENTATION) {
            private static final long serialVersionUID = -1480947262397441951L;

            @Override
            public void callBack() {
                JavaController controller = new JavaController();
                String[] interfaceFunctionName = new String[1];
                controller.getObjectProperty(getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunctionName);

                InterpreterManagement.requestScilabExec("help " + interfaceFunctionName[0]);
            }
        });
        menu.add(help);

        menu.setVisible(true);

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        return menu;
    }

    // CSON: JavaNCSS

    /**
     * @param graph
     *            parent graph
     * @return context menu
     */
    // CSOFF: JavaNCSS
    public ContextMenu createContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();
        Map < Class <? extends DefaultAction > , Menu > menuList = new HashMap < Class <? extends DefaultAction > , Menu > ();

        MenuItem value = BlockParametersAction.createMenu(graph);
        menuList.put(BlockParametersAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        value = CutAction.cutMenu(graph);
        menuList.put(CutAction.class, value);
        menu.add(value);
        value = CopyAction.copyMenu(graph);
        menuList.put(CopyAction.class, value);
        menu.add(value);
        value = DeleteAction.createMenu(graph);
        menuList.put(DeleteAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        value = RegionToSuperblockAction.createMenu(graph);
        menuList.put(RegionToSuperblockAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu format = ScilabMenu.createMenu();
        format.setText(XcosMessages.FORMAT);
        menu.add(format);
        value = RotateAction.createMenu(graph);
        menuList.put(RotateAction.class, value);
        format.add(value);
        value = MirrorAction.createMenu(graph);
        menuList.put(MirrorAction.class, value);
        format.add(value);
        value = FlipAction.createMenu(graph);
        menuList.put(FlipAction.class, value);
        format.add(value);
        value = ShowHideShadowAction.createMenu(graph);
        menuList.put(ShowHideShadowAction.class, value);
        format.add(value);
        /*--- */
        format.addSeparator();
        /*--- */
        Menu alignMenu = ScilabMenu.createMenu();
        alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
        alignMenu.add(AlignBlockActionLeft.createMenu(graph));
        alignMenu.add(AlignBlockActionCenter.createMenu(graph));
        alignMenu.add(AlignBlockActionRight.createMenu(graph));
        alignMenu.addSeparator();
        alignMenu.add(AlignBlockActionTop.createMenu(graph));
        alignMenu.add(AlignBlockActionMiddle.createMenu(graph));
        alignMenu.add(AlignBlockActionBottom.createMenu(graph));
        menuList.put(AlignBlockAction.class, alignMenu);
        format.add(alignMenu);
        /*--- */
        format.addSeparator();
        /*--- */
        if (graph.getSelectionCells().length > 1) {
            format.add(BorderColorAction.createMenu(graph));
            format.add(FilledColorAction.createMenu(graph));
        } else {
            format.add(EditFormatAction.createMenu(graph));
        }
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        menu.add(ViewDetailsAction.createMenu(graph));
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        menu.add(BlockDocumentationAction.createMenu(graph));

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        customizeMenu(menuList);

        return menu;
    }

    /**
     * Override this to customize contextual menu
     *
     * @param menuList
     *            list of menu
     */
    protected void customizeMenu(Map < Class <? extends DefaultAction > , Menu > menuList) {
        // To be overridden by sub-classes
    }


    /**
     * Set the default block position on the geom
     *
     * @param geom
     *            the current geom
     */
    private void setDefaultPosition(mxGeometry geom) {
        geom.setX(DEFAULT_POSITION_X);
        geom.setY(DEFAULT_POSITION_Y);
    }

    /*
     * Overriden methods from jgraphx
     */

    /**
     * @return always false
     * @see com.mxgraph.model.mxCell#isConnectable()
     */
    @Override
    public boolean isConnectable() {
        return false;
    }

    /**
     * {@inheritDoc}
     *
     * Sync the specific child {@link EditFormatAction#HASH_IDENTIFIER}
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        /*
         * Update the id if this is an identifier cell (herited identifier)
         */
        if (child.getId().endsWith(XcosDiagram.HASH_IDENTIFIER)) {
            child.setId(getId() + XcosDiagram.HASH_IDENTIFIER);
        }

        return super.insert(child, index);
    }

    @Override
    public String toString() {
        final StringBuilder str = new StringBuilder();

        JavaController controller = new JavaController();

        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(uid, Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        str.append(interfaceFunction[0]);
        str.append("\n");
        for (Object c : children) {
            str.append(c);
            str.append("\n");
        }

        return str.toString();
    }
}
