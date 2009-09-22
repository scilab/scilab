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

    private int dataSize = 0;
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
	    default:
		return 0;
	    }
	}
    }

    public BasicPort(String style) {
	super();
	setVertex(true);
	setStyle(style);
	setGeometry(new mxGeometry(0, 0, 10, 10));
    }

    public int getDataSize() {
	return dataSize;
    }

    public void setDataSize(int dataSize) {
	this.dataSize = dataSize;
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

    public abstract Type getType();
}
