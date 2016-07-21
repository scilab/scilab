/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.xcos.block;

import java.awt.MouseInfo;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
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
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.VectorOfString;
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
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;

/**
 * A block on the diagram
 */
@SuppressWarnings(value = { "serial" })
public class BasicBlock extends XcosCell implements Serializable {
    /**
     * Sorted kind of input, useful to sort them by kind
     */
    private static final Class<?>[] sortedChildrenClass = { InputPort.class, OutputPort.class, ControlPort.class, CommandPort.class, Object.class };

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

    protected static final double DEFAULT_POSITION_X = 10.0;
    protected static final double DEFAULT_POSITION_Y = 10.0;
    private static final double DEFAULT_WIDTH = 40.0;
    private static final double DEFAULT_HEIGHT = 40.0;

    /**
     * Internal method to get a base index to compare with depending on the cell type.
     *
     * @param cell
     *            the cell
     * @return the base index
     */
    private static int compareByChildClass(final Object o1, final Object o2) {
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

    /**
     * Hook used to implement specifix behavior after any parameter update and before re-layouting the block
     */
    public void updateBlockView() {
    }

    /**
     * Represent a simulation function type compatible with Scilab/Scicos function type descriptors.
     */
    public static enum SimulationFunctionType {
        /** event select; reduced at compilation */
        ESELECT(-2), /** if then else; reduced at compilation */
        IFTHENELSE(-1), /** first common block */
        DEFAULT(0), /** first native block */
        TYPE_1(1), /** second native block */
        TYPE_2(2), /** third native block */
        TYPE_3(3), /** forth native block */
        C_OR_FORTRAN(4), /** Scilab blocks */
        SCILAB(5), /** Debug blocks */
        DEBUG(99), /** dynamic {@link #TYPE_1} Fortran blocks (fortran_block.sci) */
        DYNAMIC_FORTRAN_1(1001), /** dynamic {@link #TYPE_1} C blocks (c_block.sci) */
        DYNAMIC_C_1(2001), /** Explicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_EXPLICIT_4(2004), /** Implicit {@link #TYPE_1} Fortran blocks (DIFF_f.sci) */
        OLDBLOCKS(10001), /** Implicit {@link #C_OR_FORTRAN} blocks */
        IMPLICIT_C_OR_FORTRAN(10004), /** Implicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_IMPLICIT_4(12004), /** Modelica {@link #C_OR_FORTRAN} blocks */
        MODELICA(30004), /** Magic types */
        UNKNOWN(5);

        private final int value;

        /**
         * Default constructor
         *
         * @param scilabValue
         *            Scilab/Scicos function type descriptor
         */
        private SimulationFunctionType(int scilabValue) {
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
                if (iter.getValue() == scilabValue) {
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
        public int getValue() {
            return value;
        }
    };

    /**
     * Constructor that setup some properties and pass all its arguments this its {@link XcosCell} parent.
     *
     *
     * @param controller The shared controller instance
     * @param uid the uid of the MVC object
     * @param kind {@link Kind#BLOCK} or {@link Kind#ANNOTATION}
     * @param value the value of the block
     * @param geometry the geometry to apply to this JGraphX object
     * @param style the style to apply to this JGraphX object
     * @param id the id to apply to this JGraphX object
     */
    public BasicBlock(final JavaController controller, long uid, Kind kind, Object value, mxGeometry geometry, String style, String id) {
        super(controller, uid, kind, value, geometry, style, id);

        /*
         * Default JGraphX properties for blocks
         */
        this.visible = true;
        this.vertex = true;
        this.connectable = false;
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
     * Does the block update and register on the undo manager
     * @param controller the shared controller
     * @param parent the parent diagram or superblock diagram
     * @param modifiedBlock the new settings
     */
    public void updateBlockSettings(JavaController controller, XcosDiagram parent, BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        /*
         * Update the block settings
         */
        updateFields(controller, parent, modifiedBlock);

        /*
         * Update the children ports
         */
        updateChildren(controller, parent, modifiedBlock);
    }

    /**
     * Update the instance field and the model values
     *
     * @param modifiedBlock
     *            the modified instance
     */
    private void updateFields(JavaController controller, XcosDiagram parent, BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        // TODO these copies are not managed by undo/redo ; fix that

        int[] integer = new int[1];

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.SIM_FUNCTION_API, integer);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SIM_FUNCTION_API, integer[0]);

        String[] str = new String[1];

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.INTERFACE_FUNCTION, str);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.INTERFACE_FUNCTION, str[0]);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.SIM_FUNCTION_NAME, str);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SIM_FUNCTION_NAME, str[0]);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.SIM_BLOCKTYPE, str);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SIM_BLOCKTYPE, str[0]);

        VectorOfDouble vDouble = new VectorOfDouble();

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.EXPRS, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.EXPRS, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.STATE, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.STATE, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.DSTATE, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DSTATE, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.ODSTATE, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.ODSTATE, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.RPAR, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.RPAR, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.OPAR, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.OPAR, vDouble);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.EQUATIONS, vDouble);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.EQUATIONS, vDouble);

        VectorOfInt vInt = new VectorOfInt();

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.SIM_DEP_UT, vInt);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.SIM_DEP_UT, vInt);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.NZCROSS, vInt);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.NZCROSS, vInt);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.NMODE, vInt);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.NMODE, vInt);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.IPAR, vInt);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.IPAR, vInt);

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.COLOR, vInt);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.COLOR, vInt);

        VectorOfString vStr = new VectorOfString();

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.DIAGRAM_CONTEXT, vStr);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.DIAGRAM_CONTEXT, vStr);

        VectorOfScicosID localChildren = new VectorOfScicosID();

        controller.getObjectProperty(modifiedBlock.getUID(), modifiedBlock.getKind(), ObjectProperties.CHILDREN, localChildren);
        controller.setObjectProperty(getUID(), getKind(), ObjectProperties.CHILDREN, localChildren);

        /*
         * JGraphX mapped properties
         */
        parent.getModel().setStyle(this, modifiedBlock.getStyle());
        parent.getModel().setValue(this, modifiedBlock.getValue());
    }

    /**
     * Update the children of the block.
     *
     * @param modifiedBlock
     *            the new block instance
     */
    private void updateChildren(JavaController controller, XcosDiagram parent, BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        /*
         * Checked as port classes only
         */
        Set < Class <? extends mxICell >> types = new HashSet < > (Arrays.asList(InputPort.class, OutputPort.class, ControlPort.class, CommandPort.class));

        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedOlds = getTypedChildren(types);
        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedNews = modifiedBlock.getTypedChildren(types);

        parent.getModel().beginUpdate();
        try {
            for (Class <? extends mxICell > klass : types) {
                final Deque<mxICell> olds = annotatedOlds.get(klass);
                final Deque<mxICell> news = annotatedNews.get(klass);

                // updated ports
                while (!olds.isEmpty() && !news.isEmpty()) {
                    mxICell previous = olds.poll();
                    mxICell modified = news.poll();

                    final int previousIndex = children.indexOf(previous);

                    // relink
                    if (previous.getEdgeCount() != 0) {
                        final mxICell relinked = previous.getEdgeAt(0);
                        final boolean isOutgoing = previous == relinked.getTerminal(true);
                        previous.removeEdge(relinked, isOutgoing);
                        modified.insertEdge(relinked, isOutgoing);
                    }

                    parent.removeCells(new Object[] { previous }, false);
                    parent.addCells(new Object[] { modified }, this, previousIndex);

                    // Clone the geometry to avoid empty geometry on new cells.
                    parent.getModel().setGeometry(modified, (mxGeometry) previous.getGeometry().clone());

                }

                // removed ports
                if (!olds.isEmpty()) {
                    parent.removeCells(olds.toArray(), true);
                }

                // added ports
                if (!news.isEmpty()) {
                    parent.addCells(news.toArray(), this);
                }
            }
        } finally {
            parent.getModel().endUpdate();
        }
    }

    /**
     * Format the children as a typed map for the given class set.
     *
     * @param types
     *            the classes to search for.
     * @return a map which linked foreach type the corresponding cell list.
     */
    private Map < Class <? extends mxICell > , Deque<mxICell >> getTypedChildren(Set < Class <? extends mxICell >> types) {
        Map < Class <? extends mxICell > , Deque<mxICell >> oldPorts = new HashMap < > ();

        // Allocate all types set
        for (Class <? extends mxICell > type : types) {
            oldPorts.put(type, new LinkedList<>());
        }

        if (getChildCount() <= 0) {
            return oldPorts;
        }

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
     * Format the children as a typed map index for the given class set.
     *
     * @param types
     *            the classes to search for.
     * @return a map which linked foreach type the corresponding cell index in the children.
     */
    private Map<Class<? extends mxICell>, ArrayList<Integer>> getTypedChildrenIndexes(Set<Class<? extends mxICell>> types) {
        Map<Class<? extends mxICell>, ArrayList<Integer>> oldPorts = new HashMap<Class<? extends mxICell>, ArrayList<Integer>>();

        // Allocate all types set
        for (Class<? extends mxICell> type : types) {
            oldPorts.put(type, new ArrayList<>());
        }

        if (getChildCount() <= 0) {
            return oldPorts;
        }

        // children lookup
        for (int i = 0; i < children.size(); i++) {
            final Object cell = children.get(i);

            Class<? extends Object> klass = cell.getClass();
            while (klass != null) {
                if (types.contains(klass)) {
                    break;
                }
                klass = klass.getSuperclass();
            }

            final ArrayList<Integer> current = oldPorts.get(klass);
            if (current != null) {
                current.add(i);
            }
        }

        return oldPorts;
    }

    public ArrayList<Integer> getTypedChildrenIndexes(Class<? extends mxICell> type) {
        return getTypedChildrenIndexes(Collections.singleton(type)).get(type);
    }

    public Map<Class<? extends mxICell>, ArrayList<Long>> getTypedChildrenUIDs(Set<Class<? extends mxICell>> types) {
        Map<Class<? extends mxICell>, ArrayList<Long>> ports = new HashMap<>();

        // Allocate all types set
        types.stream().forEach(t -> ports.put(t, new ArrayList<>()));

        if (getChildCount() <= 0) {
            return ports;
        }

        // children lookup
        for (int i = 0; i < children.size(); i++) {
            final Object cell = children.get(i);

            Class<? extends Object> klass = cell.getClass();
            while (klass != null) {
                if (types.contains(klass)) {
                    break;
                }
                klass = klass.getSuperclass();
            }

            final ArrayList<Long> current = ports.get(klass);
            if (current != null) {
                if (cell instanceof XcosCell) {
                    current.add(((XcosCell) cell).getUID());
                } else {
                    current.add(0l);
                }
            }
        }

        return ports;
    }

    public ArrayList<Long> getTypedChildrenUIDs(Class<? extends mxICell> type) {
        return getTypedChildrenUIDs(Collections.singleton(type)).get(type);
    }

    /**
     * @param graph
     *            parent graph
     */
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = null;
        // if (getParentDiagram() instanceof PaletteDiagram) {
        // menu = createPaletteContextMenu(graph);
        // } else {
        menu = createContextMenu(graph);
        // }
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
                    BasicBlock block = null;
                    try {
                        block = (BasicBlock) BasicBlock.this.clone();
                    } catch (CloneNotSupportedException e) {
                        e.printStackTrace();
                    }

                    XcosDiagram theDiagram = new XcosDiagram(controller, controller.createObject(Kind.DIAGRAM), Kind.DIAGRAM, "");
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);

                    XcosTab.get(theDiagram).setVisible(true);
                    BlockPositioning.updateBlockView(theDiagram, block);
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
                    BasicBlock block = null;
                    try {
                        block = (BasicBlock) BasicBlock.this.clone();
                    } catch (CloneNotSupportedException e) {
                        e.printStackTrace();
                    }
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);
                    BlockPositioning.updateBlockView(theDiagram, block);
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
                        BasicBlock block = null;
                        try {
                            block = (BasicBlock) BasicBlock.this.clone();
                        } catch (CloneNotSupportedException e) {
                            e.printStackTrace();
                        }
                        theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                        mxGeometry geom = BasicBlock.this.getGeometry();
                        setDefaultPosition(geom);
                        theDiagram.getModel().setGeometry(block, geom);
                        BlockPositioning.updateBlockView(theDiagram, block);
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

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x,
                MouseInfo.getPointerInfo().getLocation().y);

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
        Map<Class<? extends DefaultAction>, Menu> menuList = new HashMap<>();

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

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x,
                MouseInfo.getPointerInfo().getLocation().y);

        customizeMenu(menuList);

        return menu;
    }

    /**
     * Override this to customize contextual menu
     *
     * @param menuList
     *            list of menu
     */
    protected void customizeMenu(Map<Class<? extends DefaultAction>, Menu> menuList) {
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
}
