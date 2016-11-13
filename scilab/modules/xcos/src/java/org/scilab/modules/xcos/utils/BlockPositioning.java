/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxStyleUtils;
import org.scilab.modules.graph.utils.StyleMap;

/**
 * Helpers to place port on a block.
 */
public final class BlockPositioning {

    /**
     * The default grid size. This value is used when the grid size isn't accessible (on the palette).
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
     * Dispatch ports on Block's _WEST_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateWestPortsPosition(final XcosDiagram diag, BasicBlock block, List<? extends BasicPort> ports) {

        double gridSize = diag.getGridSize();

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getHeight();
        final double segLength = blockLength / (portsSize + 1);

        diag.getModel().beginUpdate();
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
        diag.getModel().endUpdate();
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
         * The base position is the origin of the port geometry. It is the upper-left corner position.
         */
        final double basePosition = (i + 1) * segLength;

        /*
         * The aligned base position is the base position aligned on the grid.
         */
        final double alignedBasePosition = basePosition - Math.IEEEremainder(basePosition, gridSize);

        /*
         * The aligned position is the base position translated from origin to the middle of the port.
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
    public static void updateNorthPortsPosition(final XcosDiagram diag, BasicBlock block, List<? extends BasicPort> ports) {
        double gridSize = diag.getGridSize();

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getWidth();
        final double segLength = blockLength / (portsSize + 1);

        diag.getModel().beginUpdate();
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
        diag.getModel().endUpdate();
    }

    /**
     * Dispatch ports on Block's _EAST_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateEastPortsPosition(final XcosDiagram diag, BasicBlock block, List<? extends BasicPort> ports) {
        double gridSize = diag.getGridSize();

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getHeight();
        final double segLength = blockLength / (portsSize + 1);

        diag.getModel().beginUpdate();
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
        diag.getModel().endUpdate();
    }

    /**
     * Dispatch ports on Block's _SOUTH_ side.
     *
     * @param block
     *            The block we have to work on.
     * @param ports
     *            The ports we have to move on the side.
     */
    public static void updateSouthPortsPosition(final XcosDiagram diag, BasicBlock block, List<? extends BasicPort> ports) {
        double gridSize = diag.getGridSize();

        final mxGeometry blockGeom = block.getGeometry();
        assert blockGeom != null;
        final int portsSize = ports.size();
        final double blockLength = blockGeom.getWidth();
        final double segLength = blockLength / (portsSize + 1);

        diag.getModel().beginUpdate();
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
        diag.getModel().endUpdate();
    }

    /**
     * Update all the port position of the block.
     *
     * @param block
     *            The block we have to work on.
     */
    public static void updatePortsPosition(final XcosDiagram diag, BasicBlock block) {
        final Map<Orientation, List<BasicPort>> ports = BasicBlockInfo.getAllOrientedPorts(block);

        diag.getModel().beginUpdate();
        for (Orientation iter : Orientation.values()) {
            List<BasicPort> orientedPorts = ports.get(iter);
            if (orientedPorts != null && !orientedPorts.isEmpty()) {
                updatePortsPositions(diag, block, orientedPorts, iter);
            }
        }
        diag.getModel().endUpdate();
    }

    /**
     * Update the port position for the specified orientation. This function manage the flip and mirror properties.
     *
     * @param block
     *            The block we are working on
     * @param ports
     *            The ports at with the specified orientation
     * @param iter
     *            The orientation.
     */
    private static void updatePortsPositions(final XcosDiagram diag, BasicBlock block, List<BasicPort> ports, Orientation iter) {
        @SuppressWarnings("serial")
        final List<BasicPort> invertedPorts = new ArrayList<BasicPort>(ports) {
            {
                Collections.reverse(this);
            }
        };

        JavaController controller = new JavaController();
        String[] style = new String[1];
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);
        StyleMap styleMap = new StyleMap(style[0]);

        final boolean mirrored = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_MIRROR));
        final boolean flipped = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_FLIP));
        final int intRotation = Double.valueOf(styleMap.getOrDefault(XcosConstants.STYLE_ROTATION, "0")).intValue();
        final int angle = ((Math.round(intRotation)) % 360 + 360) % 360;

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
         * Ugly modification of the iter to update at the right position Works only for 0 - 90 - 180 - 270 angles.
         */
        Orientation rotated = rotateOrientation(iter, mirrored, flipped);

        updatePortsPosition(diag, block, rotated, angle, working);
    }

    /**
     * Ugly modification of the iter to update at the right position. Works only for 0 - 90 - 180 - 270 angles.
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
     * Update the ports positions according to the angle. This function doesn't handle order inversion.
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
    private static void updatePortsPosition(final XcosDiagram diag, BasicBlock block, Orientation iter, final double angle, List<BasicPort> working) {
        /*
         * Ugly modification of the iter to update at the right position Works only for 0 - 90 - 180 - 270 angles.
         */
        final int nbOfOrientations = Orientation.values().length; // 4
        Orientation rotated = iter;

        /* Angle management */
        double rotationIndex = angle / ROTATION_STEP;
        rotated = Orientation.values()[(rotated.ordinal() + (int) rotationIndex) % nbOfOrientations];

        /* Call the associated function */
        switch (rotated) {
            case NORTH:
                updateNorthPortsPosition(diag, block, working);
                break;
            case SOUTH:
                updateSouthPortsPosition(diag, block, working);
                break;
            case EAST:
                updateEastPortsPosition(diag, block, working);
                break;
            case WEST:
                updateWestPortsPosition(diag, block, working);
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
    public static void rotateAllPorts(final XcosDiagram diag, BasicBlock block) {
        JavaController controller = new JavaController();
        String[] style = new String[1];
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);
        StyleMap styleMap = new StyleMap(style[0]);

        final boolean mirrored = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_MIRROR));
        final boolean flipped = Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_FLIP));
        final int intRotation = Double.valueOf(styleMap.getOrDefault(XcosConstants.STYLE_ROTATION, "0")).intValue();
        final int angle = ((Math.round(intRotation)) % 360 + 360) % 360;

        final int childrenCount = block.getChildCount();
        for (int i = 0; i < childrenCount; ++i) {
            if (block.getChildAt(i) instanceof BasicPort) {
                final BasicPort port = (BasicPort) block.getChildAt(i);
                final Orientation orientation = port.getOrientation();

                diag.getModel().beginUpdate();

                /* Apply angle */
                final mxIGraphModel model = diag.getModel();
                final String rot = Integer.toString(orientation.getRelativeAngle(angle, port.getClass(), flipped, mirrored));
                mxStyleUtils.setCellStyles(model, new Object[] { port }, XcosConstants.STYLE_ROTATION, rot);

                diag.getModel().endUpdate();
            }
        }
    }

    /**
     * Update the geometry of the block's ports.
     *
     * @param block
     *            The block to work on
     */
    public static void updateBlockView(final XcosDiagram diag, BasicBlock block) {
        if (block.getKind() != Kind.BLOCK) {
            return;
        }

        diag.getModel().beginUpdate();
        block.updateBlockView();
        updatePortsPosition(diag, block);
        rotateAllPorts(diag, block);
        diag.getModel().endUpdate();

        /*
         * TODO: bug #6705; This placement trick doesn't work on the first block Dnd as the view is not revalidated.
         *
         * On block loading, parentDiagram is null thus placement is not performed.
         */
    }

    /**
     * Flip a block (horizontal inversion).
     *
     * @param block
     *            The block to work on
     */
    public static void toggleFlip(final XcosDiagram diag, BasicBlock block) {
        JavaController controller = new JavaController();
        String[] style = new String[1];
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);

        StyleMap styleMap = new StyleMap(style[0]);
        final boolean invertedFlip = ! Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_FLIP));

        styleMap.put(XcosConstants.STYLE_FLIP, Boolean.toString(invertedFlip));

        controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, styleMap.toString());

        updateBlockView(diag, block);
    }

    /**
     * Mirror a block (vertical inversion).
     *
     * @param block
     *            The block to work on
     */
    public static void toggleMirror(final XcosDiagram diag, BasicBlock block) {
        JavaController controller = new JavaController();
        String[] style = new String[1];
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);

        StyleMap styleMap = new StyleMap(style[0]);
        final boolean invertedFlip = ! Boolean.TRUE.toString().equals(styleMap.get(XcosConstants.STYLE_MIRROR));

        styleMap.put(XcosConstants.STYLE_MIRROR, Boolean.toString(invertedFlip));

        controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, styleMap.toString());

        updateBlockView(diag, block);
    }

    /**
     * Rotate a block with an anti-clockwise next value
     *
     * @param block
     *            The block to work on
     */
    public static void toggleAntiClockwiseRotation(final XcosDiagram diag, BasicBlock block) {
        JavaController controller = new JavaController();
        String[] style = new String[1];
        controller.getObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, style);

        StyleMap styleMap = new StyleMap(style[0]);
        styleMap.put(XcosConstants.STYLE_ROTATION, Integer.toString(getNextAntiClockwiseAngle(styleMap)));

        controller.setObjectProperty(block.getUID(), Kind.BLOCK, ObjectProperties.STYLE, styleMap.toString());
        updateBlockView(diag, block);
    }

    /**
     * Get the next anti-clockwise rotation value
     *
     * @param styleMap
     *            the data to parse
     * @return The angle value
     */
    public static int getNextAntiClockwiseAngle(StyleMap styleMap) {
        final int intRotation = Double.valueOf(styleMap.getOrDefault(XcosConstants.STYLE_ROTATION, "0")).intValue();

        int angle = (intRotation - ROTATION_STEP + MAX_ROTATION) % MAX_ROTATION;
        return angle;
    }

    /**
     * Get the next clockwise rotation value
     *
     * @param styleMap
     *            the data to parse
     * @return The angle value
     */
    public static int getNextClockwiseAngle(StyleMap styleMap) {
        final int intRotation = Double.valueOf(styleMap.getOrDefault(XcosConstants.STYLE_ROTATION, "0")).intValue();
        int angle = (intRotation + ROTATION_STEP) % MAX_ROTATION;
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
}
