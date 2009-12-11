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

public abstract class BasicPort extends XcosUIDObject {

    private static final long serialVersionUID = -5022701071026919015L;
    private int ordering = 0;
    private int connectedLinkId = 0;
    private int dataLines = 0;
    private int dataColumns = 0;
    private DataType dataType = DataType.REAL_MATRIX;
    private int initialAngle = 0;
    private int angle = 0;
    private transient String typeName;

    public enum Type { 
	IMPLICIT,
	EXPLICIT;

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

    public BasicPort(String style) {
	super();
	setVertex(true);
	setStyle(style);
	setTypeName(style);
	setGeometry(new mxGeometry(0, 0, 8, 8));
    }

    public int getDataLines() {
	return dataLines;
    }

    public void setDataLines(int dataLines) {
	this.dataLines = dataLines;
    }

    public int getDataColumns() {
	return dataColumns;
    }

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

    public void setInitialAngle(int initialAngle) {
	this.initialAngle = initialAngle;
    }

    public int getInitialAngle() {
	return initialAngle;
    }

    public void setAngle(int angle) {
	this.angle = (initialAngle + angle) % 360;
    }

    public int getAngle() {
	return angle;
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
}
