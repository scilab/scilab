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

import org.scilab.modules.xcos.XcosUIDObject;

import com.mxgraph.model.mxGeometry;

/**
 * Common implementation of any Port.
 */
public abstract class BasicPort extends XcosUIDObject {

	/**
	 * Represent a port orientation related to the associated block. These
	 * orientation semantics are valid when there is no rotation/mirror/flip
	 * applied to the block.
	 */
	public static enum Orientation {
		/** The port is on the left (west) side of the block */
		WEST,
		/** The port is on the top (north) side of the block */
		NORTH,
		/** The port is on the right (east) side of the block */
		EAST,
		/** The port is on the bottom (south) side of the block */
		SOUTH;

		/**
		 * Get the orientation angle where the associated block angle is
		 * blockAngle.
		 * 
		 * @param blockAngle
		 *            The value of the block angle
		 * @param flipped
		 *            The block flip state
		 * @param mirrored
		 *            The block mirror state
		 * @return The value of the angle.
		 */
		public int getAngle(int blockAngle, boolean flipped, boolean mirrored) {
			int angle;

			/* Specific settings */
			switch (this) {
			case WEST:
			case EAST:
				angle = 0;
				if (flipped) {
					angle = angle + 180;
				}
				break;

			case NORTH:
			case SOUTH:
				angle = 90;
				if (mirrored) {
					angle = angle + 180;
				}
				break;

			default:
				angle = 0;
				break;
			}

			/* Calculate angle */
			return (angle + blockAngle) % 360;
		}
	}
	
    private static final long serialVersionUID = -5022701071026919015L;
    private static final int DEFAULT_DATALINES = -1;
    private static final int DEFAULT_DATACOLUMNS = -2;
    
    private static final int DEFAULT_PORTSIZE = 8;
    
    private int ordering;
    private int connectedLinkId;
    private int dataLines;
    private int dataColumns;
    private DataType dataType = DataType.REAL_MATRIX;
    private int angle;
    private Orientation defaultOrientation;
    private transient String typeName;

    /** Type of any dataport */
    public enum Type { 
	IMPLICIT,
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
    	UNKNOW_TYPE,
    	REAL_MATRIX,
    	COMPLEX_MATRIX,
    	INT32_MATRIX,
    	INT16_MATRIX,
    	INT8_MATRIX,
    	UINT32_MATRIX,
    	UINT16_MATRIX,
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

    public void setDataType(DataType dataType) {
	this.dataType = dataType;
    }

    public DataType getDataType() {
	return dataType;
    }

    public void setOrdering(int portOrdering) {
	this.ordering = portOrdering;
    }

    public int getOrdering() {
	return ordering;
    }
    
    public void setConnectedLinkId(int connectedLinkId) {
//    	this.connectedLinkId = Math.max(connectedLinkId, this.connectedLinkId);
    	this.connectedLinkId = connectedLinkId;
    }

    public int getConnectedLinkId() {
	return connectedLinkId;
    }

    public abstract Type getType();

    public void setStyle(String style){
	super.setStyle(style);
    }

    public void setAngle(int angle) {
	this.angle = angle % 360;
    }

    public int getAngle() {
	return angle;
    }

	/** @return The default orientation of this port */
	public final Orientation getDefaultOrientation() {
		return defaultOrientation;
	}

	/**
	 * @param defaultOrientation
	 *            The default orientation of this port
	 */
	public final void setDefaultOrientation(Orientation defaultOrientation) {
		this.defaultOrientation = defaultOrientation;
	}
    
    public String getToolTipText() {
	StringBuffer result = new StringBuffer();
	result.append("<html>");
	result.append("Port number : " + getOrdering() + "<br>");
	result.append("</html>");
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
