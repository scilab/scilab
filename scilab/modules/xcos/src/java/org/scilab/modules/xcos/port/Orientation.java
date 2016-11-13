/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
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

package org.scilab.modules.xcos.port;

import com.mxgraph.util.mxConstants;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Represent a port orientation related to the associated block. These
 * orientation semantics are valid when there is no rotation/mirror/flip applied
 * to the block.
 */
public enum Orientation {
    /** The port is on the left (west) side of the block */
    WEST,
    /** The port is on the top (north) side of the block */
    NORTH,
    /** The port is on the right (east) side of the block */
    EAST,
    /** The port is on the bottom (south) side of the block */
    SOUTH;

    private static final int MAX_ROTATION = 360;
    private static final int PERPENDICULAR_ROTATION = 90;

    /**
     * Get the orientation angle linked to its parent block.
     *
     * @param blockAngle
     *            The value of the block angle
     * @param klass
     *            The kind of port
     * @param flipped
     *            The block flip state
     * @param mirrored
     *            The block mirror state
     * @return The value of the angle.
     */
    public int getRelativeAngle(int blockAngle, Class <? extends BasicPort > klass, boolean flipped, boolean mirrored) {
        final int orientation = getOrientationAngle();
        final int base = getBaseAngle(klass, orientation);

        return getFlippedAndMirroredAngle(base + blockAngle, flipped, mirrored);
    }

    /**
     * @param klass
     *            the kind of port
     * @param flipped
     *            the flip status
     * @param mirrored
     *            the mirror status
     * @return the real angle
     */
    public int getAbsoluteAngle(Class <? extends BasicPort > klass,
    boolean flipped, boolean mirrored) {
        final int orientation = getOrientationAngle();
        final int base = getBaseAngle(klass, orientation);

        return getFlippedAndMirroredAngle(base, flipped, mirrored);
    }

    /**
     * Update the base angle according to the flipped and mirrored flag.
     *
     * @param base
     *            the previous angle
     * @param flipped
     *            the flip status
     * @param mirrored
     *            the mirror status
     * @return the updated angle.
     */
    private int getFlippedAndMirroredAngle(int base, boolean flipped, boolean mirrored) {
        int angle = base;

        switch (this) {
            case NORTH:
            case SOUTH:
                if (flipped) {
                    angle = angle + (MAX_ROTATION / 2);
                }
                break;

            case WEST:
            case EAST:
                if (mirrored) {
                    angle = angle + (MAX_ROTATION / 2);
                }
                break;

            default:
                break;
        }

        /* Calculate angle */
        return angle % MAX_ROTATION;
    }

    /**
     * @return the angle associated with this orientation.
     */
    private int getOrientationAngle() {
        return ordinal() * PERPENDICULAR_ROTATION;
    }

    /**
     * As the orientation angle is calculated as an input (default jgraphx
     * triangle direction), we have to update it for output blocks.
     *
     * @param klass
     *            kind of block
     * @param orientationAngle
     *            calculated orientation angle
     * @return updated angle
     */
    private int getBaseAngle(Class <? extends BasicPort > klass, int orientationAngle) {
        final boolean isOutput = OutputPort.class.isAssignableFrom(klass)
                                 || CommandPort.class.isAssignableFrom(klass);

        if (isOutput) {
            return orientationAngle + (MAX_ROTATION / 2);
        }

        return orientationAngle;
    }

    /**
     * @return The label position of the current port.
     * @see com.mxgraph.util.mxConstants#STYLE_LABEL_POSITION
     */
    public String getLabelPosition() {
        final String ret;

        switch (this) {
            case EAST:
                ret = mxConstants.ALIGN_RIGHT;
                break;
            case WEST:
                ret = mxConstants.ALIGN_LEFT;
                break;
            default:
                ret = mxConstants.ALIGN_CENTER;
                break;
        }

        return ret;
    }

    /**
     * @return The vertical label position of the current port.
     * @see com.mxgraph.util.mxConstants#STYLE_VERTICAL_LABEL_POSITION
     */
    public String getVerticalLabelPosition() {
        final String ret;

        switch (this) {
            case NORTH:
                ret = mxConstants.ALIGN_TOP;
                break;
            case SOUTH:
                ret = mxConstants.ALIGN_BOTTOM;
                break;
            default:
                ret = mxConstants.ALIGN_MIDDLE;
                break;
        }

        return ret;
    }

    /**
     * @return the spacing side to increment
     * @see com.mxgraph.util.mxConstants#STYLE_SPACING_BOTTOM
     * @see com.mxgraph.util.mxConstants#STYLE_SPACING_LEFT
     * @see com.mxgraph.util.mxConstants#STYLE_SPACING_RIGHT
     * @see com.mxgraph.util.mxConstants#STYLE_SPACING_TOP
     */
    public String getSpacingSide() {
        final String ret;

        switch (this) {
            case NORTH:
                ret = mxConstants.STYLE_SPACING_TOP;
                break;
            case SOUTH:
                ret = mxConstants.STYLE_SPACING_BOTTOM;
                break;
            case EAST:
                ret = mxConstants.STYLE_SPACING_RIGHT;
                break;
            case WEST:
            default:
                ret = mxConstants.STYLE_SPACING_LEFT;
                break;
        }

        return ret;
    }
}
