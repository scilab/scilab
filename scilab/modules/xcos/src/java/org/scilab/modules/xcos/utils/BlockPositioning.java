/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Map;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxUtils;

/**
 * Helpers to place port on a block.
 */
public final class BlockPositioning {

    /**
     * The default grid size. This value is used when the grid size isn't
     * accessible (on the palette).
     */
    public static final double DEFAULT_GRIDSIZE = Double.MIN_NORMAL;
    /** The rotation step of the clockwise and anticlockwise rotation */
    public static final int ROTATION_STEP = 90;
    /** The max valid rotation value (always 360 degres) */
    public static final int MAX_ROTATION = 360;

    /** This class is a static singleton, thus it must not be instantiated */
    private BlockPositioning() {
    }

    /**
     *
     */
    public static class PortComparator implements Comparator<BasicPort> {
        @Override
        public int compare(BasicPort arg0, BasicPort arg1) {
            int order_0 = arg0.getOrdering();
            int order_1 = arg1.getOrdering();

            if (order_0 < order_1) {
                return -1;
            } else if (order_0 > order_1) {
                return 1;
            } else {
                return 0;
            }
        }
    }

    /**
     * Dispatch ports on Block's _WEST_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateWestPortsPosition(BasicBlock block, List <? extends BasicPort > ports) {

        double gridSize;
        if (block.getParentDiagram() == null) {
            gridSize = DEFAULT_GRIDSIZE;
        } else {
            gridSize = block.getParentDiagram().getGridSize();
        }

        // BasicBlock.sortsort(List<?> children) takes into account different 
        // parameters to order the ports list. We only need to order the ports
        // given their ordering.
        Collections.sort(ports, new PortComparator());

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getHeight();
        final double segLength = blockLength / (portsSize + 1);

        beginUpdate(block);
        for (int i = 0; i < portsSize; ++i) {
            final BasicPort port = (ports.get(i));
            final mxGeometry portGeom = port.getGeometry();

            double nonVariantPosition = -portGeom.getWidth();
            final int order = i;
            double alignedPosition = calculateAlignedPosition(gridSize, segLength, order);

            portGeom.setX(nonVariantPosition);
            portGeom.setY(alignedPosition);

            port.setLabelPosition(Orientation.WEST);
        }
        endUpdate(block);
    }

    /**
     * Calculate an aligned port position.
     *
     * @param gridSize
     *            the grid size
     * @param segLength
     *            the side length
     * @param i
     *            the current working index
     * @return the aligned position on the grid.
     */
    private static double calculateAlignedPosition(final double gridSize, final double segLength, int i) {
        /*
         * The base position is the origin of the port geometry. It is the
         * upper-left corner position.
         */
        final double basePosition = (i + 1) * segLength;

        /*
         * The aligned base position is the base position aligned on the grid.
         */
        final double alignedBasePosition = basePosition - Math.IEEEremainder(basePosition, gridSize);

        /*
         * The aligned position is the base position translated from origin to
         * the middle of the port.
         */
        final double alignedPosition = alignedBasePosition - (BasicPort.DEFAULT_PORTSIZE / 2.0);

        return alignedPosition;
    }

    /**
     * Dispatch ports on Block's _NORTH_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateNorthPortsPosition(BasicBlock block, List <? extends BasicPort > ports) {
        double gridSize;
        if (block.getParentDiagram() == null) {
            gridSize = DEFAULT_GRIDSIZE;
        } else {
            gridSize = block.getParentDiagram().getGridSize();
        }

        // BasicBlock.sortsort(List<?> children) takes into account different 
        // parameters to order the ports list. We only need to order the ports
        // given their ordering.
        Collections.sort(ports, new PortComparator());

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getWidth();
        final double segLength = blockLength / (portsSize + 1);

        beginUpdate(block);
        for (int i = 0; i < portsSize; ++i) {
            final BasicPort port = (ports.get(i));
            final mxGeometry portGeom = port.getGeometry();

            double nonVariantPosition = -portGeom.getHeight();
            final int order = i;
            double alignedPosition = calculateAlignedPosition(gridSize, segLength, order);

            portGeom.setX(alignedPosition);
            portGeom.setY(nonVariantPosition);

            port.setLabelPosition(Orientation.NORTH);
        }
        endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateEastPortsPosition(BasicBlock block, List <? extends BasicPort > ports) {
        double gridSize;
        if (block.getParentDiagram() == null) {
            gridSize = DEFAULT_GRIDSIZE;
        } else {
            gridSize = block.getParentDiagram().getGridSize();
        }

        // BasicBlock.sortsort(List<?> children) takes into account different 
        // parameters to order the ports list. We only need to order the ports
        // given their ordering.
        Collections.sort(ports, new PortComparator());

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getHeight();
        final double segLength = blockLength / (portsSize + 1);

        beginUpdate(block);
        for (int i = 0; i < portsSize; ++i) {
            final BasicPort port = (ports.get(i));
            final mxGeometry portGeom = port.getGeometry();

            double nonVariantPosition = blockGeom.getWidth();
            final int order = i;
            double alignedPosition = calculateAlignedPosition(gridSize, segLength, order);

            portGeom.setX(nonVariantPosition);
            portGeom.setY(alignedPosition);

            port.setLabelPosition(Orientation.EAST);
        }
        endUpdate(block);
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateSouthPortsPosition(BasicBlock block, List <? extends BasicPort > ports) {
        double gridSize;
        if (block.getParentDiagram() == null) {
            gridSize = DEFAULT_GRIDSIZE;
        } else {
            gridSize = block.getParentDiagram().getGridSize();
        }

        // BasicBlock.sortsort(List<?> children) takes into account different 
        // parameters to order the ports list. We only need to order the ports
        // given their ordering.
        Collections.sort(ports, new PortComparator());

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getWidth();
        final double segLength = blockLength / (portsSize + 1);

        beginUpdate(block);
        for (int i = 0; i < portsSize; ++i) {
            final BasicPort port = (ports.get(i));
            final mxGeometry portGeom = port.getGeometry();

            double nonVariantPosition = blockGeom.getHeight();
            final int order = i;
            double alignedPosition = calculateAlignedPosition(gridSize, segLength, order);

            portGeom.setX(alignedPosition);
            portGeom.setY(nonVariantPosition);

            port.setLabelPosition(Orientation.SOUTH);
        }
        endUpdate(block);
    }

    /**
     * Update all the port position of the block.
     *
     * @param block
     *            The block we have to work on.
     */
    public static void updatePortsPosition(BasicBlock block) {
        final Map<Orientation, List<BasicPort>> ports = BasicBlockInfo.getAllOrientedPorts(block);

        beginUpdate(block);
        for (Orientation iter : Orientation.values()) {
            List<BasicPort> orientedPorts = ports.get(iter);
            if (orientedPorts != null && !orientedPorts.isEmpty()) {
                updatePortsPositions(block, orientedPorts, iter);
            }
        }
        endUpdate(block);
    }

    /**
     * Update the port position for the specified orientation. This function
     * manage the flip and mirror properties.
     *
     * @param block
     *            The block we are working on
     * @param ports
     *            The ports at with the specified orientation
     * @param iter
     *            The orientation.
     */
    private static void updatePortsPositions(BasicBlock block, List<BasicPort> ports, Orientation iter) {
        @SuppressWarnings("serial")
        final List<BasicPort> invertedPorts = new ArrayList<BasicPort>(ports) {
            {
                Collections.reverse(this);
            }
        };
        final boolean mirrored = block.getMirror();
        final boolean flipped = block.getFlip();
        final int angle = block.getAngle();
        List<BasicPort> working = ports;

        /* List order modification with the flip flag */
        if (flipped) {
            if (iter == Orientation.NORTH || iter == Orientation.SOUTH) {
                working = invertedPorts;
            }
        }

        /* List order modification with the mirror flag */
        if (mirrored) {
            if (iter == Orientation.EAST || iter == Orientation.WEST) {
                working = invertedPorts;
            }
        }

        /*
         * Ugly modification of the iter to update at the right position Works
         * only for 0 - 90 - 180 - 270 angles.
         */
        Orientation rotated = rotateOrientation(iter, mirrored, flipped);

        updatePortsPosition(block, rotated, angle, working);
    }

    /**
     * Ugly modification of the iter to update at the right position. Works only
     * for 0 - 90 - 180 - 270 angles.
     *
     * @param iter
     *            the real orientation
     * @param mirrored
     *            is the block mirrored
     * @param flipped
     *            is the block flipped
     * @return The modified orientation
     */
    private static Orientation rotateOrientation(final Orientation iter, final boolean mirrored, final boolean flipped) {
        final int nbOfOrientations = Orientation.values().length; // 4
        Orientation rotated = iter;

        /* Flip & Mirror management */
        if (mirrored) {
            if (rotated == Orientation.EAST || rotated == Orientation.WEST) {
                rotated = Orientation.values()[(rotated.ordinal() + 2) % nbOfOrientations];
            }
        }
        if (flipped) {
            if (rotated == Orientation.NORTH || rotated == Orientation.SOUTH) {
                rotated = Orientation.values()[(rotated.ordinal() + 2) % nbOfOrientations];
            }
        }
        return rotated;
    }

    /**
     * Update the ports positions according to the angle. This function doesn't
     * handle order inversion.
     *
     * @param block
     *            The block we are working on
     * @param iter
     *            The current port orientation
     * @param angle
     *            The angle we have to rotate
     * @param working
     *            The ordered ports we are working on.
     */
    private static void updatePortsPosition(BasicBlock block, Orientation iter, final int angle, List<BasicPort> working) {
        /*
         * Ugly modification of the iter to update at the right position Works
         * only for 0 - 90 - 180 - 270 angles.
         */
        final int nbOfOrientations = Orientation.values().length; // 4
        Orientation rotated = iter;

        /* Angle management */
        int rotationIndex = angle / ROTATION_STEP;
        rotated = Orientation.values()[(rotated.ordinal() + rotationIndex) % nbOfOrientations];

        /* Call the associated function */
        switch (rotated) {
            case NORTH:
                updateNorthPortsPosition(block, working);
                break;
            case SOUTH:
                updateSouthPortsPosition(block, working);
                break;
            case EAST:
                updateEastPortsPosition(block, working);
                break;
            case WEST:
                updateWestPortsPosition(block, working);
                break;

            default:
                break;
        }
    }

    /**
     * Rotate all the port of the block.
     *
     * @param block
     *            The block to work on.
     */
    public static void rotateAllPorts(BasicBlock block) {
        final int angle = block.getAngle();
        final boolean flipped = block.getFlip();
        final boolean mirrored = block.getMirror();

        final int childrenCount = block.getChildCount();
        for (int i = 0; i < childrenCount; ++i) {
            if (block.getChildAt(i) instanceof BasicPort) {
                final BasicPort port = (BasicPort) block.getChildAt(i);
                final Orientation orientation = port.getOrientation();

                beginUpdate(block);

                /* Apply angle */
                if (block.getParentDiagram() != null) {
                    final mxIGraphModel model = block.getParentDiagram().getModel();
                    final String rot = Integer.toString(orientation.getRelativeAngle(angle, port.getClass(), flipped, mirrored));
                    mxUtils.setCellStyles(model, new Object[] { port }, XcosConstants.STYLE_ROTATION, rot);
                } else {
                    final StyleMap m = new StyleMap(port.getStyle());
                    final int rot = orientation.getRelativeAngle(angle, port.getClass(), flipped, mirrored);
                    m.put(XcosConstants.STYLE_ROTATION, Integer.toString(rot));
                    port.setStyle(m.toString());
                }

                endUpdate(block);
            }
        }
    }

    /**
     * Update the geometry of the block's ports.
     *
     * @param block
     *            The block to work on
     */
    public static void updateBlockView(BasicBlock block) {
        beginUpdate(block);
        updatePortsPosition(block);
        rotateAllPorts(block);
        endUpdate(block);

        /*
         * FIXME: #6705; This placement trick doesn't work on the first block
         * Dnd as the view is not revalidated.
         *
         * On block loading, parentDiagram is null thus placement is not
         * performed.
         */
    }

    /**
     * Flip a block (horizontal inversion).
     *
     * @param block
     *            The block to work on
     */
    public static void toggleFlip(BasicBlock block) {

        block.setFlip(!block.getFlip());
        updateBlockView(block);
    }

    /**
     * Mirror a block (vertical inversion).
     *
     * @param block
     *            The block to work on
     */
    public static void toggleMirror(BasicBlock block) {

        block.setMirror(!block.getMirror());
        updateBlockView(block);
    }

    /**
     * Rotate a block with an anti-clockwise next value
     *
     * @param block
     *            The block to work on
     */
    public static void toggleAntiClockwiseRotation(BasicBlock block) {
        block.setAngle(getNextAntiClockwiseAngle(block));
        updateBlockView(block);
    }

    /**
     * Get the next anti-clockwise rotation value
     *
     * @param block
     *            The block to work on
     * @return The angle value
     */
    public static int getNextAntiClockwiseAngle(BasicBlock block) {
        int angle = (block.getAngle() - ROTATION_STEP + MAX_ROTATION) % MAX_ROTATION;
        return angle;
    }

    /**
     * Get the next clockwise rotation value
     *
     * @param block
     *            The block to work on
     * @return The angle value
     */
    public static int getNextClockwiseAngle(BasicBlock block) {
        int angle = (block.getAngle() + ROTATION_STEP) % MAX_ROTATION;
        return angle;
    }

    /**
     * Convert any angle value to a valid block value
     *
     * @param angle
     *            the non valid value
     * @return the nearest graph valid value
     */
    public static int roundAngle(int angle) {
        int ret = angle;
        if (angle < 0 || angle > MAX_ROTATION) {
            ret = (angle + MAX_ROTATION) % MAX_ROTATION;
        }

        for (int i = 0; i < (MAX_ROTATION / ROTATION_STEP); i++) {
            int min = i * ROTATION_STEP;
            int max = (i + 1) * ROTATION_STEP;

            if (ret < (min + max) / 2) {
                ret = min;
                break;
            }
        }
        return ret;
    }

    /**
     * Helper function that protect the block model.
     *
     * @param block
     *            The block to protect
     */
    private static void beginUpdate(BasicBlock block) {
        if (block != null && block.getParentDiagram() != null) {
            block.getParentDiagram().getModel().beginUpdate();
        }
    }

    /**
     * Helper function that end the protection of the block model.
     *
     * @param block
     *            The block previously protected
     */
    private static void endUpdate(BasicBlock block) {
        if (block != null && block.getParentDiagram() != null) {
            block.getParentDiagram().getModel().endUpdate();
        }
    }
}
