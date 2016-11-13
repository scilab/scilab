/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011-2016 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.xcos.block.actions;

import java.awt.event.ActionEvent;
import java.util.Arrays;
import java.util.Collection;
import java.util.LinkedList;
import java.util.Queue;
import java.util.TreeSet;
import java.util.logging.Logger;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxRectangle;
import java.nio.LongBuffer;
import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import static java.util.stream.Collectors.toList;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.graph.model.XcosCellFactory;
import org.scilab.modules.xcos.io.ScilabTypeCoder;
import org.scilab.modules.xcos.utils.XcosConstants;

/**
 * Move the Selected cells to a new SuperBlock diagram
 */
// CSOFF: ClassFanOutComplexity
@SuppressWarnings(value = { "serial" })
public class RegionToSuperblockAction extends VertexSelectionDependantAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.REGION_TO_SUPERBLOCK;
    /** Icon name of the action */
    public static final String SMALL_ICON = "object-group";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = 0;
    /** Accelerator key for the action */
    public static final int ACCELERATOR_KEY = 0;

    private static final String INTERFUNCTION_NAME = "SUPER_f";

    /**
     * Default Constructor
     *
     * @param scilabGraph
     *            the graph
     */
    public RegionToSuperblockAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     *            graph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, RegionToSuperblockAction.class);
    }

    /**
     * Represent a broken link at selection time
     */
    private static final class Broken implements Comparable<Broken> {
        private final mxGraphModel parentModel;

        private final BasicPort source;
        private final BasicPort target;

        private final BasicLink parentLink;
        private final boolean containsSource;

        /*
         * Out of selection port position
         */
        private final double x;
        private final double y;

        /*
         * lazy allocated fields
         */
        private BasicLink childLink;
        private BasicPort parentPort;
        private ContextUpdate childBlock;

        /**
         * Default Constructor
         *
         * @param parentGraph
         *            the parent graph
         * @param parentModel
         *            the parent model
         * @param source
         *            the source
         * @param target
         *            the target
         * @param link
         *            the link
         * @param containsSource
         *            is the source in selection
         */
        public Broken(XcosDiagram parentGraph, mxGraphModel parentModel, BasicPort source, BasicPort target, BasicLink link, boolean containsSource) {
            super();
            this.parentModel = parentModel;

            this.source = source;
            this.target = target;
            this.parentLink = link;
            this.containsSource = containsSource;

            final BasicPort terminal;
            if (containsSource) {
                terminal = target;
            } else {
                terminal = source;
            }

            /*
             * Update position
             */
            final mxGeometry pos = parentModel.getGeometry(terminal);
            final mxGeometry parent = parentModel.getGeometry(parentModel.getParent(terminal));
            if (pos != null && parent != null) {
                this.x = parentGraph.snap(pos.getX() + parent.getX() - (pos.getHeight() / 2));
                this.y = parentGraph.snap(pos.getY() + parent.getY() - (pos.getWidth() / 2));
            } else {
                this.x = 0.0;
                this.y = 0.0;
            }
        }

        /**
         * @return the link on the parent diagram
         */
        public BasicLink getParentLink() {
            return parentLink;
        }

        /**
         * @param isSource
         *            is a source port needed
         * @return the parent terminal (source or not)
         */
        public mxICell getParentTerminal(boolean isSource) {
            final mxICell ret;

            if (containsSource) {
                if (isSource) {
                    ret = getParentPort();
                } else {
                    ret = target;
                }
            } else {
                if (isSource) {
                    ret = source;
                } else {
                    ret = getParentPort();
                }
            }

            return ret;
        }

        /**
         * @param isSource
         *            is a source port needed
         * @return the child terminal (source or not)
         */
        public mxICell getChildTerminal(boolean isSource) {
            final mxICell ret;

            if (containsSource) {
                if (isSource) {
                    ret = source;
                } else {
                    ret = getChildBlock().getChildAt(0);
                }
            } else {
                if (isSource) {
                    ret = getChildBlock().getChildAt(0);
                } else {
                    ret = target;
                }
            }

            return ret;
        }

        /**
         * @return the terminal in the selection
         */
        public BasicPort getTerminal() {
            if (containsSource) {
                return source;
            } else {
                return target;
            }
        }

        /**
         * @return the link on the child diagram
         */
        public BasicLink getChildLink() {
            if (childLink == null) {
                childLink = (BasicLink) parentModel.cloneCells(new Object[] { parentLink }, true)[0];
            }
            return childLink;
        }

        /**
         * @return the port on the parent diagram
         */
        public BasicPort getParentPort() {
            if (parentPort == null) {
                JavaController controller = new JavaController();
                long uid = controller.createObject(Kind.PORT);

                try {
                    if (containsSource) {
                        parentPort = IOBlocks.getOpposite(target.getClass())
                                     .getConstructor(JavaController.class, Long.TYPE, Kind.class, Object.class, String.class, String.class)
                                     .newInstance(controller, uid, Kind.PORT, null, null, new UID().toString());
                    } else {
                        parentPort = IOBlocks.getOpposite(source.getClass())
                                     .getConstructor(JavaController.class, Long.TYPE, Kind.class, Object.class, String.class, String.class)
                                     .newInstance(controller, uid, Kind.PORT, null, null, new UID().toString());;
                    }
                } catch (ReflectiveOperationException e) {
                    Logger.getLogger(RegionToSuperblockAction.class.getName()).severe(e.toString());
                } catch (IllegalArgumentException ex) {
                    Logger.getLogger(RegionToSuperblockAction.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

            return parentPort;
        }

        /**
         * @return the child block to put on the child diagram
         */
        public ContextUpdate getChildBlock() {
            if (childBlock == null) {
                final BasicPort terminal = getTerminal();
                childBlock = IOBlocks.createBlock(terminal);

                /*
                 * Set the child position
                 */
                mxGeometry geom = childBlock.getGeometry();
                geom.setX(x);
                geom.setY(y);
                childBlock.setGeometry(geom);
            }

            return childBlock;
        }

        /**
         * Set the ordering on the I/O block and port

         * @param ordering
         *            the ordering to set
         */
        public void setOrdering(JavaController controller, int ordering) {
            // update the child ordering
            VectorOfInt ipar = new VectorOfInt(1);
            ipar.set(0, ordering);
            controller.setObjectProperty(getChildBlock().getUID(), getChildBlock().getKind(), ObjectProperties.IPAR, ipar);

            VectorOfDouble exprs = new ScilabTypeCoder().var2vec(new ScilabString(Integer.toString(ordering)));
            controller.setObjectProperty(getChildBlock().getUID(), getChildBlock().getKind(), ObjectProperties.EXPRS, exprs);
        }

        /**
         * {@inheritDoc}
         *
         * This function is used to sort a {@link TreeSet} of {@link Broken}.
         */
        @Override
        public int compareTo(Broken o) {
            final int xdiff = (int) (x - o.x);
            final int ydiff = (int) (y - o.y);

            if (xdiff == 0 && ydiff == 0) {
                // same position, sort by hashcode
                return hashCode() - o.hashCode();
            } else {
                return (ydiff << (Integer.SIZE / 2)) + xdiff;
            }
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public String toString() {
            final String labelSep = ": ";
            final String link = " -> ";

            final StringBuilder str = new StringBuilder();
            if (getParentLink().getChildCount() > 0) {
                // append the label
                str.append(getParentLink().getChildAt(0).getValue());
                str.append(labelSep);
            }

            str.append(getParentTerminal(true));
            str.append(link);
            str.append(getParentTerminal(false));

            str.append('\n');

            if (getChildLink().getChildCount() > 0) {
                // append the label
                str.append(getChildLink().getChildAt(0).getValue());
                str.append(labelSep);
            }

            str.append(getChildTerminal(true));
            str.append(link);
            str.append(getChildTerminal(false));

            return str.toString();
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram parentGraph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) parentGraph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        parentGraph.info(XcosMessages.GENERATE_SUPERBLOCK);
        parentGraph.getModel().beginUpdate();
        try {
            final JavaController controller = new JavaController();
            final SuperBlock superBlock;
            final Collection<Broken> brokenLinks;

            /*
             * Allocate superBlock
             */
            final Object[] selection = parentGraph.getSelectionCells();
            List<XcosCell> toBeMoved = Arrays.stream(selection)
                                       .filter(o -> o instanceof XcosCell)
                                       .map(o -> (XcosCell) o)
                                       .collect(toList());

            superBlock = allocateSuperBlock(controller, parentGraph, selection);

            /*
             * First perform all modification on the parent diagram to handle
             * well undo/redo operations.
             */
            brokenLinks = updateParent(controller, parentGraph, superBlock, toBeMoved);

            /*
             * Then move some cells to the child diagram
             */
            moveToChild(controller, parentGraph, superBlock, brokenLinks, toBeMoved);

            BlockPositioning.updateBlockView(parentGraph, superBlock);
        } catch (ScilabInterpreterManagement.InterpreterException ex) {
            // Scilab seems to be blocked, just consume the exception at this point
        } finally {
            parentGraph.getModel().endUpdate();
            parentGraph.info(XcosMessages.EMPTY_INFO);
        }
    }

    /**
     * Allocate a superBlock
     *
     * @param parentGraph
     *            the base graph
     * @param selection
     *            the selected blocks
     * @return the allocated super block (without specific listeners)
     */
    private SuperBlock allocateSuperBlock(final JavaController controller, final XcosDiagram parentGraph, final Object[] selection) throws ScilabInterpreterManagement.InterpreterException {
        final SuperBlock superBlock = (SuperBlock) XcosCellFactory.createBlock(INTERFUNCTION_NAME);

        /*
         * Remove the default allocated ports
         */
        while (superBlock.getChildCount() > 0) {
            superBlock.remove(superBlock.getChildCount() - 1);
        }
        VectorOfScicosID children = new VectorOfScicosID();
        controller.setObjectProperty(superBlock.getUID(), superBlock.getKind(), ObjectProperties.CHILDREN, children);

        /*
         * Place the super block
         */
        final mxRectangle dims = parentGraph.getBoundingBoxFromGeometry(selection);
        final double minX = dims.getX();
        final double maxX = minX + dims.getWidth();

        final double minY = dims.getY();
        final double maxY = minY + dims.getHeight();

        mxGeometry geom = superBlock.getGeometry();
        geom.setX((maxX + minX - geom.getWidth()) / 2.0);
        geom.setY((maxY + minY - geom.getHeight()) / 2.0);
        superBlock.setGeometry(geom);

        /*
         * get statistics to flip and rotate
         */
        VectorOfDouble mvcAngle = new VectorOfDouble();

        int angleCounter = 0;
        int flipCounter = 0;
        int mirrorCounter = 0;
        for (Object object : selection) {
            if (object instanceof BasicBlock) {
                final BasicBlock b = (BasicBlock) object;

                String[] style = new String[1];
                controller.getObjectProperty(b.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);
                StyleMap styleMap = new StyleMap(style[0]);

                final boolean mirrored = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_MIRROR));
                final boolean flipped = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_FLIP));
                final int intRotation = Double.valueOf(styleMap.getOrDefault(XcosConstants.STYLE_ROTATION, "0")).intValue();

                angleCounter += intRotation;
                if (flipped) {
                    flipCounter++;
                }
                if (mirrored) {
                    mirrorCounter++;
                }
            }
        }

        /*
         * apply statistics to flip and rotate
         */
        final int halfSize = selection.length / 2;
        String[] style = new String[1];
        controller.getObjectProperty(superBlock.getUID(), superBlock.getKind(), ObjectProperties.STYLE, style);
        StyleMap styleMap = new StyleMap(style[0]);

        styleMap.put(XcosConstants.STYLE_ROTATION, Integer.toString(BlockPositioning.roundAngle(angleCounter / selection.length)));
        if (flipCounter > halfSize) {
            styleMap.put(XcosConstants.STYLE_FLIP, Boolean.toString(true));
        }
        if (mirrorCounter > halfSize) {
            styleMap.put(XcosConstants.STYLE_MIRROR, Boolean.toString(true));
        }

        controller.setObjectProperty(superBlock.getUID(), superBlock.getKind(), ObjectProperties.STYLE, styleMap.toString());

        return superBlock;
    }

    /**
     * Create child cells and add them to the parent diagram. All links are also
     * reconnected
     *
     * @param parentGraph
     *            the parent diagram
     * @param superBlock
     *            the superblock
     * @param inSelectionCells
     *            the cells in the selection
     * @return the broken descriptor set
     */
    private Collection<Broken> updateParent(final JavaController controller, final XcosDiagram parentGraph, final SuperBlock superBlock, final List<XcosCell> inSelectionCells) {
        final Collection<Broken> brokenLinks;
        final mxGraphModel parentModel = (mxGraphModel) parentGraph.getModel();

        parentModel.beginUpdate();
        try {
            /*
             * Add the internal links and fill border links Sort the broken
             * links by position (to perform a good numbering order) and keep
             * only one occurrence of a broken link.
             */
            brokenLinks = new TreeSet<>();
            fillLinks(parentGraph, parentModel, inSelectionCells, brokenLinks);

            /*
             * Disconnect the broken links
             */
            for (Broken broken : brokenLinks) {
                mxGraphModel.setTerminals(parentModel, broken.getParentLink(), null, null);
            }

            /*
             * Add the super block
             */
            parentGraph.addCell(superBlock);

            /*
             * Main broken loop
             */
            // ordering access is : IN, OUT, e_IN, e_OUT
            final int[] ordering = { 0, 0, 0, 0 };
            for (Broken broken : brokenLinks) {

                // set the ordering
                incrementOrdering(controller, ordering, broken);

                connectParent(parentGraph, parentModel, superBlock, broken);
                connectChild(parentGraph, parentModel, broken);

                /*
                 * Update the view
                 */
                BlockPositioning.updateBlockView(parentGraph, broken.getChildBlock());
            }
        } finally {
            parentModel.endUpdate();
        }

        return brokenLinks;
    }

    /**
     * Fill the internalLinks links from a selected block to a selected block
     * and broken links with a broken object. Also disconnect all the broken
     * links.
     *
     * @param parentGraph
     *            the graph
     * @param parentModel
     *            the model
     * @param inSelectionCells
     *            the selected blocks (with fast contains operation)
     * @param brokenLinks
     *            the broken links to find.
     */
    private void fillLinks(final XcosDiagram parentGraph, final mxGraphModel parentModel, final Collection<XcosCell> inSelectionCells, final Collection<Broken> brokenLinks) {
        final Queue<Object> loopQueue = new LinkedList<>(inSelectionCells);

        while (!loopQueue.isEmpty()) {
            final Object cell = loopQueue.remove();

            final int childCount = parentModel.getChildCount(cell);
            for (int i = 0; i < childCount; i++) {
                final Object port = parentModel.getChildAt(cell, i);

                final int edgeCount = parentModel.getEdgeCount(port);
                for (int j = 0; j < edgeCount; j++) {
                    final Object edge = parentModel.getEdgeAt(port, j);

                    final Object source = parentModel.getTerminal(edge, true);
                    final Object target = parentModel.getTerminal(edge, false);

                    /*
                     * Add the links
                     */
                    final boolean containsSource = inSelectionCells.contains(parentModel.getParent(source));
                    final boolean containsTarget = inSelectionCells.contains(parentModel.getParent(target));

                    if (containsSource && containsTarget && edge instanceof XcosCell) {
                        inSelectionCells.add((XcosCell) edge);
                        continue;
                    }

                    /*
                     * Handle a broken link case
                     */

                    if (containsSource) {
                        brokenLinks.add(new Broken(parentGraph, parentModel, (BasicPort) source, (BasicPort) target, (BasicLink) edge, true));
                        continue;
                    }

                    if (containsTarget) {
                        brokenLinks.add(new Broken(parentGraph, parentModel, (BasicPort) source, (BasicPort) target, (BasicLink) edge, false));
                        continue;
                    }
                }
            }
        }
    }

    /**
     * Increment and set the ordering to the broken entry.
     *
     * @param controller the controller
     * @param ordering
     *            4x1 array of ordering
     * @param broken
     *            the current broken entry.
     */
    // CSOFF: MagicNumber
    private void incrementOrdering(JavaController controller, final int[] ordering, Broken broken) {
        if (broken.getTerminal() instanceof InputPort) {
            broken.setOrdering(controller, ++ordering[0]);
        } else if (broken.getTerminal() instanceof OutputPort) {
            broken.setOrdering(controller, ++ordering[1]);
        } else if (broken.getTerminal() instanceof ControlPort) {
            broken.setOrdering(controller, ++ordering[2]);
        } else { // if (broken.getTerminal() instanceof CommandPort)
            broken.setOrdering(controller, ++ordering[3]);
        }
    }

    // CSON: MagicNumber

    /**
     * Add I/O port and reconnect them
     *
     * @param parentGraph
     *            the parent graph
     * @param parentModel
     *            the parent graph model
     * @param superBlock
     *            the super block
     * @param broken
     *            the broken entry
     */
    private void connectParent(final XcosDiagram parentGraph, final mxGraphModel parentModel, final SuperBlock superBlock, Broken broken) {
        parentGraph.addCell(broken.getParentPort(), superBlock);
        parentGraph.addCell(broken.getParentLink());

        final mxICell source = broken.getParentTerminal(true);
        final mxICell target = broken.getParentTerminal(false);

        // then connect the link
        mxGraphModel.setTerminals(parentModel, broken.getParentLink(), source, target);
    }

    /**
     * Add I/O blocks and reconnect them
     *
     * @param childGraph
     *            the child graph
     * @param childModel
     *            the child graph model
     * @param broken
     *            the broken entry
     */
    private void connectChild(final XcosDiagram childGraph, final mxGraphModel childModel, Broken broken) {
        childGraph.addCell(broken.getChildBlock());
        childGraph.addCell(broken.getChildLink());

        final mxICell source = broken.getChildTerminal(true);
        final mxICell target = broken.getChildTerminal(false);

        // then connect the link
        mxGraphModel.setTerminals(childModel, broken.getChildLink(), source, target);
    }

    /**
     * Move the cells to the child graph
     *
     * @param parentGraph
     *            the parent graph
     * @param superBlock
     *            the superBlock
     * @param brokenLinks
     *            the broken links set
     * @param inSelectionCells
     *            the cells in selection
     * @return the superblock child diagram
     */
    private void moveToChild(final JavaController controller, final XcosDiagram parentGraph, final SuperBlock superBlock, final Collection<Broken> brokenLinks, final List<XcosCell> inSelectionCells) {
        final Collection<XcosCell> cellsToCopy = new ArrayList<>();

        /*
         * create a collection with all the cells to move
         */
        cellsToCopy.addAll(inSelectionCells);
        for (Broken b : brokenLinks) {
            cellsToCopy.add(b.getChildBlock());
            cellsToCopy.add(b.getChildLink());
        }

        /*
         * Really move the cells
         */
        parentGraph.removeCells(cellsToCopy.toArray(), false);

        VectorOfScicosID children = new VectorOfScicosID(cellsToCopy.size());
        LongBuffer childrenUIDs = children.asByteBuffer(0, cellsToCopy.size()).asLongBuffer();

        final long parentBlock = superBlock.getUID();
        final long parentDiagram;
        if (parentGraph.getKind() == Kind.DIAGRAM) {
            parentDiagram = parentGraph.getUID();
        } else {
            parentDiagram = parentGraph.getRootDiagram().getUID();
        }

        cellsToCopy.stream().forEach(c -> {
            childrenUIDs.put(c.getUID());
            controller.referenceObject(c.getUID());

            controller.setObjectProperty(c.getUID(), c.getKind(), ObjectProperties.PARENT_BLOCK, parentBlock);
            controller.setObjectProperty(c.getUID(), c.getKind(), ObjectProperties.PARENT_DIAGRAM, parentDiagram);
        });

        controller.setObjectProperty(superBlock.getUID(), superBlock.getKind(), ObjectProperties.CHILDREN, children);
    }
}
// CSON: ClassFanOutComplexity
