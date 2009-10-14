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

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;

public abstract class BasicPort extends mxCell {

    private int ordering = 0;
    private int connectedLinkId = 0;
    private int dataLines = 0;
    private int dataColumns = 0;
    private DataType dataType = DataType.REAL_MATRIX;

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
	    case REAL_MATRIX:
		return 1;
	    case COMPLEX_MATRIX:
		return 2;
		//TODO
	    default:
		return 0;
	    }
	}
    }

    public BasicPort(String style) {
	super();
	setVertex(true);
	setStyle(style);
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
	this.connectedLinkId = Math.max(connectedLinkId, this.connectedLinkId);
    }

    public int getConnectedLinkId() {
	return connectedLinkId;
    }

    public abstract Type getType();

	public abstract void updateStyle(int angle);
	
	public void setStyle(String style){
		super.setStyle(style);
	}
}
