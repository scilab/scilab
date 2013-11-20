/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab Copyright (C)
 * 2011 - DIGITEO - Clement DAVID This file must be used under the terms of the
 * CeCILL. This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms are also
 * available at http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.modules.xcos.block.actions;

import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;
import java.util.TreeSet;
import java.util.logging.Logger;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.actions.base.VertexSelectionDependantAction;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.io.ContextUpdate;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxRectangle;

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
        public Broken(mxGraphModel parentModel, BasicPort source, BasicPort target, BasicLink link, boolean containsSource) {
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
                this.x = pos.getX() + parent.getX() + (pos.getWidth() / 2) - (parent.getWidth() / 2);
                this.y = pos.getY() + parent.getY() + (pos.getHeight() / 2) - (parent.getHeight() / 2);
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
                try {
                    if (containsSource) {
                        parentPort = IOBlocks.getOpposite(target.getClass()).newInstance();
                    } else {
                        parentPort = IOBlocks.getOpposite(source.getClass()).newInstance();
                    }
                } catch (InstantiationException e) {
                    Logger.getLogger(RegionToSuperblockAction.class.getName()).severe(e.toString());
                } catch (IllegalAccessException e) {
                    Logger.getLogger(RegionToSuperblockAction.class.getName()).severe(e.toString());
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
                childBlock.getGeometry().setX(x);
                childBlock.getGeometry().setY(y);
            }

            return childBlock;
        }

        /**
         * Set the ordering on the I/O block and port
         *
         * @param ordering
         *            the ordering to set
         */
        public void setOrdering(int ordering) {
            // update the child ordering
            getChildBlock().setIntegerParameters(new ScilabDouble(ordering));
            getChildBlock().setExprs(new ScilabString(Integer.toString(ordering)));

            // update the port value
            getParentPort().setOrdering(ordering);
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

            final SuperBlock superBlock;
            final Collection<Broken> brokenLinks;
            final Set<Object> inSelectionCells;

            /*
             * Allocate superBlock
             */
            final Object[] selection = parentGraph.getSelectionCells();
            final Object[] blocks = XcosDiagram.filterByClass(selection, BasicBlock.class);
            inSelectionCells = new LinkedHashSet<Object>(Arrays.asList(blocks));

            superBlock = allocateSuperBlock(parentGraph, selection);

            /*
             * First perform all modification on the parent diagram to handle
             * well undo/redo operations.
             */
            brokenLinks = updateParent(parentGraph, superBlock, inSelectionCells);

            /*
             * Then move some cells to the child diagram
             */
            final SuperBlockDiagram childGraph = moveToChild(parentGraph, superBlock, brokenLinks, inSelectionCells);

            /*
             * Finish the install on the child and sync it.
             */
            childGraph.installListeners();
            childGraph.installSuperBlockListeners();
            superBlock.invalidateRpar();
            superBlock.updateExportedPort();

            /*
             * Clear the transaction log in the child.
             * From the user point of view, the operation cannot be undo'ed.
             */
            childGraph.getUndoManager().clear();

            Xcos.getInstance().addDiagram(parentGraph.getSavedFile(), childGraph);
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
    private SuperBlock allocateSuperBlock(final XcosDiagram parentGraph, final Object[] selection) {
        final SuperBlock superBlock = (SuperBlock) BlockFactory.createBlock(INTERFUNCTION_NAME);
        superBlock.setStyle(INTERFUNCTION_NAME);

        /*
         * Allocate the diagram
         */
        final SuperBlockDiagram diag = new SuperBlockDiagram(superBlock);
        superBlock.setChild(diag);
        superBlock.setParentDiagram(parentGraph);

        /*
         * Place the super block
         */
        final mxRectangle dims = parentGraph.getBoundingBoxFromGeometry(selection);
        final double minX = dims.getX();
        final double maxX = minX + dims.getWidth();

        final double minY = dims.getY();
        final double maxY = minY + dims.getHeight();

        superBlock.getGeometry().setX((maxX + minX - superBlock.getGeometry().getWidth()) / 2.0);
        superBlock.getGeometry().setY((maxY + minY - superBlock.getGeometry().getHeight()) / 2.0);

        /*
         * get statistics
         */
        int angleCounter = 0;
        int flipCounter = 0;
        int mirrorCounter = 0;
        for (Object object : selection) {
            if (object instanceof BasicBlock) {
                final BasicBlock b = (BasicBlock) object;

                angleCounter += b.getAngle();
                if (b.getFlip()) {
                    flipCounter++;
                }
                if (b.getMirror()) {
                    mirrorCounter++;
                }
            }
        }

        /*
         * apply statistics
         */
        final int halfSize = selection.length / 2;
        superBlock.setAngle(BlockPositioning.roundAngle(angleCounter / selection.length));
        superBlock.setFlip(flipCounter > halfSize);
        superBlock.setMirror(mirrorCounter > halfSize);

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
    private Collection<Broken> updateParent(final XcosDiagram parentGraph, final SuperBlock superBlock, final Set<Object> inSelectionCells) {
        final Collection<Broken> brokenLinks;
        final mxGraphModel parentModel = (mxGraphModel) parentGraph.getModel();

        parentModel.beginUpdate();
        try {
            /*
             * Add the internal links and fill border links Sort the broken
             * links by position (to perform a good numbering order) and keep
             * only one occurrence of a broken link.
             */
            brokenLinks = new TreeSet<Broken>();
            fillLinks(parentModel, inSelectionCells, brokenLinks);

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
                incrementOrdering(ordering, broken);

                connectParent(parentGraph, parentModel, superBlock, broken);
                connectChild(parentGraph, parentModel, broken);

                /*
                 * Update the view
                 */
                BlockPositioning.updateBlockView(broken.getChildBlock());
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
     * @param parentModel
     *            the model
     * @param inSelectionCells
     *            the selected blocks (with fast contains operation)
     * @param brokenLinks
     *            the broken links to find.
     */
    private void fillLinks(final mxGraphModel parentModel, final Collection<Object> inSelectionCells, final Collection<Broken> brokenLinks) {
        final Queue<Object> loopQueue = new LinkedList<Object>(inSelectionCells);

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

                    if (containsSource && containsTarget) {
                        inSelectionCells.add(edge);
                        continue;
                    }

                    /*
                     * Handle a broken link case
                     */

                    if (containsSource) {
                        brokenLinks.add(new Broken(parentModel, (BasicPort) source, (BasicPort) target, (BasicLink) edge, true));
                        continue;
                    }

                    if (containsTarget) {
                        brokenLinks.add(new Broken(parentModel, (BasicPort) source, (BasicPort) target, (BasicLink) edge, false));
                        continue;
                    }
                }
            }
        }
    }

    /**
     * Increment and set the ordering to the broken entry.
     *
     * @param ordering
     *            4x1 array of ordering
     * @param broken
     *            the current broken entry.
     */
    // CSOFF: MagicNumber
    private void incrementOrdering(final int[] ordering, Broken broken) {
        if (broken.getTerminal() instanceof InputPort) {
            broken.setOrdering(++ordering[0]);
        } else if (broken.getTerminal() instanceof OutputPort) {
            broken.setOrdering(++ordering[1]);
        } else if (broken.getTerminal() instanceof ControlPort) {
            broken.setOrdering(++ordering[2]);
        } else { // if (broken.getTerminal() instanceof CommandPort)
            broken.setOrdering(++ordering[3]);
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
    private SuperBlockDiagram moveToChild(final XcosDiagram parentGraph, final SuperBlock superBlock, final Collection<Broken> brokenLinks,
                                          final Set<Object> inSelectionCells) {
        final SuperBlockDiagram childGraph = superBlock.getChild();
        final mxGraphModel childModel = (mxGraphModel) childGraph.getModel();

        childModel.beginUpdate();
        try {
            final Collection<Object> cellsToCopy = new ArrayList<Object>();

            /*
             * create a collection with all the cells to move
             */
            cellsToCopy.addAll(inSelectionCells);
            for (Broken b : brokenLinks) {
                cellsToCopy.add(b.getChildBlock());
                cellsToCopy.add(b.getChildLink());
            }

            // create the local array to use the JGraphX API
            final Object[] cells = cellsToCopy.toArray();

            /*
             * Really copy the cells
             */
            parentGraph.removeCells(cells, false);
            childGraph.addCells(cells);

            /*
             * Translate the cells to the origin
             *
             * In this algorithm only block position are handled to avoid any
             * placement issue and a static margin is added to avoid
             * misplacement.
             */
            final double margin = 10.0;
            mxPoint orig = null;
            for (int i = 0; i < cells.length; i++) {
                if (cells[i] instanceof BasicBlock) {
                    final mxGeometry geom = ((BasicBlock) cells[i]).getGeometry();
                    if (orig == null) {
                        orig = geom;
                    } else {
                        orig = new mxPoint(Math.min(geom.getX(), orig.getX()), Math.min(geom.getY(), orig.getY()));
                    }
                }
            }
            childGraph.moveCells(cells, -orig.getX() + margin, -orig.getY() + margin);

            childGraph.setChildrenParentDiagram();
            BlockPositioning.updateBlockView(superBlock);
        } finally {
            childModel.endUpdate();
        }

        return childGraph;
    }
}
// CSON: ClassFanOutComplexity
