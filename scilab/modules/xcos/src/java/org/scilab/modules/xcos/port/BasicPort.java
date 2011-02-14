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
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabType;
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
	ordering = portOrdering;
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
		
		setLabelPosition(getOrientation());
	}

	/**
	 * Set the label position of the current port according to the orientation.
	 * 
	 * @param current the port orientation, if null, does nothing.
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
			style.put(mxConstants.STYLE_SPACING, Integer.toString(BasicPort.DEFAULT_PORTSIZE + 2));
			
			setStyle(style.toString());
		}
	}
	
	/**
	 * Hook to update the port label from the associated block expression.
	 * 
	 * The current port index may be found in the ordering data.
	 * @param exprs the associated block expression.
	 */
	public void updateLabel(ScilabType exprs) { }
	
    /*
     * Overriden methods from jgraphx
     */
    
    /**
     * @return true if not already connected
     * @see com.mxgraph.model.mxCell#isConnectable()
     */
    @Override
    public boolean isConnectable() {
    	final int edges = getEdgeCount();
    	return edges == 0 || getEdgeAt(0).getTerminal(false) == null;
    }
    
    /**
     * {@inheritDoc}
     */
    @Override
    public String toString() {
    	final StringBuilder str = new StringBuilder();
    	
    	final mxICell parent = getParent();
    	if (parent != null) {
    		str.append(parent.getClass().getSimpleName()).append('.');
    	}
    	
		if (getChildCount() > 0)  {
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
    	} else if (getEdgeCount() > 1){
    		str.append(" - multiple links (");
    		str.append(getEdgeCount());
    		str.append(')');
    	}
    	
    	return str.toString();
    }
}
