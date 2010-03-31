/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.port;

import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.xcos.utils.XcosConstants;

import com.mxgraph.model.mxGeometry;

/**
 * Common implementation of any Port.
 */
public abstract class BasicPort extends ScilabGraphUniqueObject {

	/**
     * The side-size of any port. All ports must have the same size.
     */
    public static final int DEFAULT_PORTSIZE = 8;
	
	private static final long serialVersionUID = -5022701071026919015L;
    private static final int DEFAULT_DATALINES = -1;
    private static final int DEFAULT_DATACOLUMNS = -2;
    
    private int ordering;
    private int connectedLinkId;
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
    		switch (this) {
    		case UNKNOW_TYPE:
    			return -1;
    		case REAL_MATRIX:
    			return 1;
    		case COMPLEX_MATRIX:
    			return 2;
    		default:
    			return 0;
    		}
    	}

    	/**
    	 * @param val A scicos representation
    	 * @return The java compatible representation 
    	 */
    	public static DataType convertScilabValue(double val) {
    		if (val == -1) {
    			return DataType.UNKNOW_TYPE;
    		} else if (val == 1) {
    			return DataType.REAL_MATRIX;
    		} else if (val == 2) {
    			return DataType.COMPLEX_MATRIX;
    		} else {
    			return DataType.UNKNOW_TYPE;
    		}
    	}
}

    /**
     * Instantiate a port with a style (or typename). 
     * @param style Value to be set as a Style and as TypeName 
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
     * @param dataLines The number of data lines that can pass trough this port.
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
     * @param dataColumns The number of data columns that can pass trough this port.
     */
    public void setDataColumns(int dataColumns) {
	this.dataColumns = dataColumns;
    }

    /** @param dataType the port data type */
    public void setDataType(DataType dataType) {
	this.dataType = dataType;
    }

    /** @return the port data type */
    public DataType getDataType() {
	return dataType;
    }

    /**
     * @param portOrdering a unique order number per type
     */
    public void setOrdering(int portOrdering) {
	this.ordering = portOrdering;
    }

    /**
     * @return the unique order number per type
     */
    public int getOrdering() {
	return ordering;
    }
    
    /** @param connectedLinkId the connected link id */
    public void setConnectedLinkId(int connectedLinkId) {
    	this.connectedLinkId = connectedLinkId;
    }

    /** @return the connected link id */
    public int getConnectedLinkId() {
	return connectedLinkId;
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
		this.orientation = defaultOrientation;
	}
    
	/**
	 * @return An html formatted documentation string
	 */
    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append(XcosConstants.HTML_BEGIN);
	result.append("Port number : " + getOrdering() + XcosConstants.HTML_NEWLINE);
	result.append("Style : " + getStyle() + XcosConstants.HTML_NEWLINE);
	result.append(XcosConstants.HTML_END);
	return result.toString();
    }

    /**
     * @param typeName the typeName to set
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
	}
}
