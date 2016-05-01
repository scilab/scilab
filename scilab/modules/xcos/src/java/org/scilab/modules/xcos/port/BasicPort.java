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

package org.scilab.modules.xcos.port;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.PortKind;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.model.XcosCell;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;

/**
 * Common implementation of any Port.
 */
public abstract class BasicPort extends XcosCell {
    private static final long serialVersionUID = 0L;

    /**
     * The side-size of any port. All ports must have the same size.
     */
    public static final double DEFAULT_PORTSIZE = 8;

    private Orientation orientation;

    /** Type of any dataport */
    public enum Type {
        /** The link direction as no impact on simulation */
        IMPLICIT,
        /** The link keep its direction on simulation */
        EXPLICIT;

        /**
         * @return A scicos compatible representation
         */
        public String getAsString() {
            switch (this) {
                case IMPLICIT:
                    return "I";
                case EXPLICIT:
                    return "E";
                default:
                    return "";
            }
        }
    };

    /** Type of any data on any dataport */
    public enum DataType {
        /** A not specific type */
        UNKNOW_TYPE,
        /** Data is real (double) numbers */
        REAL_MATRIX,
        /** Data is complex (double + i * double) numbers */
        COMPLEX_MATRIX,
        /** Data is int32 (32 bits) numbers */
        INT32_MATRIX,
        /** Data is int16 (16 bits) numbers */
        INT16_MATRIX,
        /** Data is int8 (8bits) numbers */
        INT8_MATRIX,
        /** Data is uint32 (unsigned 32 bits) numbers */
        UINT32_MATRIX,
        /** Data is uint16 (unsigned 16 bits) numbers */
        UINT16_MATRIX,
        /** Data is uint8 (unsigned 8 bits) numbers */
        UINT8_MATRIX;

        /**
         * @return A scicos compatible representation
         */
        public int asScilabValue() {
            if (this.equals(UNKNOW_TYPE)) {
                return -1;
            }

            // We assume that the types are sorted well on the enum definition
            return this.ordinal();
        }

        /**
         * @param val
         *            A scicos representation
         * @return The java compatible representation
         */
        public static DataType convertScilabValue(int val) {
            if (val <= 0 || val > DataType.values().length - 1) {
                return DataType.UNKNOW_TYPE;
            }

            // We assume that the types are sorted well on the enum definition
            return DataType.values()[val];
        }

        public static DataType convertScilabValue(double val) {
            return convertScilabValue((int) val);
        }
    }

    /**
     * Instantiate a port with a style (or typename).
     *
     * @param style
     *            Value to be set as a Style and as TypeName
     */
    public BasicPort(final JavaController controller, long uid, Kind kind, Object value, String style, String id, Orientation orientation, boolean isImplicit, PortKind portKind) {
        super(controller, uid, kind, value, new mxGeometry(0, 0, DEFAULT_PORTSIZE, DEFAULT_PORTSIZE), style, id);

        this.vertex = true;

        controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
        controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.PORT_KIND, portKind.ordinal());

        this.orientation = orientation;
        setLabelPosition(orientation);
    }

    /**
     * @return the type of the port (Explicit or Implicit)
     */
    public abstract Type getType();

    public abstract PortKind getPortKind();

    /** @return The default orientation of this port */
    public final Orientation getOrientation() {
        return orientation;
    }

    /**
     * @param defaultOrientation
     *            The default orientation of this port
     */
    public final void setOrientation(Orientation defaultOrientation) {
        if (orientation != defaultOrientation) {
            orientation = defaultOrientation;
            setLabelPosition(orientation);
        }
    }

    /**
     * Set the label position of the current port according to the orientation.
     *
     * @param current
     *            the port orientation, if null, does nothing.
     */
    public final void setLabelPosition(final Orientation current) {
        if (current != null) {
            StyleMap style = new StyleMap(getStyle());

            // clean up any previously set spacing values
            style.remove(mxConstants.STYLE_LABEL_POSITION);
            style.remove(mxConstants.STYLE_VERTICAL_LABEL_POSITION);
            style.remove(mxConstants.STYLE_SPACING_BOTTOM);
            style.remove(mxConstants.STYLE_SPACING_LEFT);
            style.remove(mxConstants.STYLE_SPACING_RIGHT);
            style.remove(mxConstants.STYLE_SPACING_TOP);

            // set up the port position
            style.put(mxConstants.STYLE_ALIGN, current.getLabelPosition());
            style.put(mxConstants.STYLE_VERTICAL_ALIGN, current.getVerticalLabelPosition());
            style.put(mxConstants.STYLE_SPACING, Double.toString(BasicPort.DEFAULT_PORTSIZE + 2.0));

            setStyle(style.toString());
        }
    }

    /**
     * Hook to update the port label from the associated block expression.
     *
     * The current port index may be found in the ordering data.
     *
     * @param exprs
     *            the associated block expression.
     */
    public void updateLabel(ScilabType exprs) {
    }

    /*
     * Overriden methods from jgraphx
     */

    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
        final StringBuilder str = new StringBuilder();

        final mxICell parent = getParent();
        if (parent != null) {
            if (parent instanceof BasicBlock) {
                JavaController controller = new JavaController();
                String[] interfaceFunctionName = new String[1];
                controller.getObjectProperty(((BasicBlock) parent).getUID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunctionName);
                str.append(interfaceFunctionName[0]).append('.');
            } else {
                str.append(parent.getClass().getSimpleName()).append('.');
            }
        }

        if (getChildCount() > 0) {
            // append the label
            str.append(getChildAt(0).getValue());
        } else {
            str.append(getClass().getSimpleName());
        }
        if (parent != null) {
            str.append('[').append(getParent().getIndex(this)).append(']');
        }
        if (getEdgeCount() == 1) {
            str.append(" (connected)");
        } else if (getEdgeCount() > 1) {
            str.append(" - multiple links (");
            str.append(getEdgeCount());
            str.append(')');
        }

        return str.toString();
    }
}
