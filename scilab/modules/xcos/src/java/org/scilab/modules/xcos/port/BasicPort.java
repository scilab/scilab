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

package org.scilab.modules.xcos.port;

import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxConstants;

/**
 * Common implementation of any Port.
 */
public abstract class BasicPort extends ScilabGraphUniqueObject {

    /**
     * The side-size of any port. All ports must have the same size.
     */
    public static final double DEFAULT_PORTSIZE = 8;

    private static final long serialVersionUID = -5022701071026919015L;
    private static final int DEFAULT_DATALINES = -1;
    private static final int DEFAULT_DATACOLUMNS = -2;

    private int ordering;
    private int dataLines;
    private int dataColumns;
    private DataType dataType = DataType.REAL_MATRIX;
    private Orientation orientation;
    private transient String typeName;

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
        public double getAsDouble() {
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
    public BasicPort(String style) {
        super();
        setVertex(true);
        setStyle(style);
        setTypeName(style);
        setGeometry(new mxGeometry(0, 0, DEFAULT_PORTSIZE, DEFAULT_PORTSIZE));
    }

    /**
     * @return The number of data lines that can pass trough this port.
     */
    public int getDataLines() {
        return dataLines;
    }

    /**
     * @param dataLines
     *            The number of data lines that can pass trough this port.
     */
    public void setDataLines(int dataLines) {
        this.dataLines = dataLines;
    }

    /**
     * @return The number of data columns that can pass trough this port.
     */
    public int getDataColumns() {
        return dataColumns;
    }

    /**
     * @param dataColumns
     *            The number of data columns that can pass trough this port.
     */
    public void setDataColumns(int dataColumns) {
        this.dataColumns = dataColumns;
    }

    /**
     * @param dataType
     *            the port data type
     */
    public void setDataType(DataType dataType) {
        this.dataType = dataType;
    }

    /** @return the port data type */
    public DataType getDataType() {
        return dataType;
    }

    /**
     * @param ordering
     *            a unique order number per instance
     */
    public void setOrdering(int ordering) {
        this.ordering = ordering;
    }

    /**
     * @return the unique order number per instance
     */
    public int getOrdering() {
        return ordering;
    }

    /**
     * @return the type of the port (Explicit or Implicit)
     */
    public abstract Type getType();

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
     * @return An html formatted documentation string
     */
    public String getToolTipText() {
        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);
        result.append("Port number : " + getOrdering() + ScilabGraphConstants.HTML_NEWLINE);

        final int length = getStyle().length();
        result.append("Style : ");
        if (length > XcosConstants.MAX_CHAR_IN_STYLE) {
            result.append(getStyle().substring(0, XcosConstants.MAX_CHAR_IN_STYLE));
            result.append(XcosMessages.DOTS);
        } else {
            result.append(getStyle());
        }
        result.append(ScilabGraphConstants.HTML_NEWLINE);

        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    /**
     * @param typeName
     *            the typeName to set
     */
    private void setTypeName(String typeName) {
        this.typeName = typeName;
    }

    /**
     * @return the typeName
     */
    public String getTypeName() {
        return typeName;
    }

    /**
     * Set the default values for newly created port.
     */
    public void setDefaultValues() {
        setDataLines(DEFAULT_DATALINES);
        setDataColumns(DEFAULT_DATACOLUMNS);
        setDataType(DataType.UNKNOW_TYPE);

        setLabelPosition(getOrientation());
    }

    /**
     * Set the label position of the current port according to the orientation.
     *
     * @param current
     *            the port orientation, if null, does nothing.
     */
    public void setLabelPosition(final Orientation current) {
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
                str.append(((BasicBlock) parent).getInterfaceFunctionName()).append('.');
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
