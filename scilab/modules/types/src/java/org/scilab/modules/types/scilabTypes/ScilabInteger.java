/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

package org.scilab.modules.types.scilabTypes;

/**
 * This class provides a wrapping on the Scilab Integer datatype
 */
public class ScilabInteger implements ScilabType {

	private Long[][] data;
	private IntegerType prec;
	private boolean bUnsigned;

	/**
	 * The different size of the integer type.
	 */
	public enum IntegerType {
		/** Integer of size 8 */
		TYPE8,
		/** Integer of size 16 */
		TYPE16,
		/** Integer of size 32 */
		TYPE32,
		/** Integer of size 64 */
		TYPE64
	}
	
	/**
	 * Default constructor
	 */
	public ScilabInteger() {
		data = null;
	}

	/**
	 * Constructor with values
	 * 
	 * @param data the values
	 * @param bUnsigned true, if the values are unsigned; false if they are signed.
	 */
	public ScilabInteger(byte[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	/**
	 * Constructor with values
	 * 
	 * @param data the values
	 * @param bUnsigned true, if the values are unsigned; false if they are signed.
	 */
	public ScilabInteger(short[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	/**
	 * Constructor with values
	 * 
	 * @param data the values
	 * @param bUnsigned true, if the values are unsigned; false if they are signed.
	 */
	public ScilabInteger(int[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	/**
	 * Constructor with values
	 * 
	 * @param data the values
	 * @param bUnsigned true, if the values are unsigned; false if they are signed.
	 */
	public ScilabInteger(long[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	/**
	 * Constructor with single signed value
	 * 
	 * @param value the unique value
	 */
	public ScilabInteger(byte value) {
		this.data = new Long[1][1];
		this.data[0][0] = Long.valueOf(value);
	}

	/**
	 * Constructor with single signed value
	 * 
	 * @param value the unique value
	 */
	public ScilabInteger(short value) {
		this.data = new Long[1][1];
		this.data[0][0] = Long.valueOf(value);
	}

	/**
	 * Constructor with single signed value
	 * 
	 * @param value the unique value
	 */
	public ScilabInteger(int value) {
		this.data = new Long[1][1];
		this.data[0][0] = Long.valueOf(value);
	}

	/**
	 * Constructor with single signed value
	 * 
	 * @param value the unique value
	 */
	public ScilabInteger(long value) {
		this.data = new Long[1][1];
		this.data[0][0] = Long.valueOf(value);
	}

	/**
	 * Set the current values 
	 * 
	 * @param data the values to set
	 * @param bUnsigned true, if these values are unsigned; false otherwise.
	 */
	public void setData(byte[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE8;
		this.bUnsigned = bUnsigned; 
		for (int i = 0; i < data.length; i++) {
			for (int j = 0; j < data[0].length; j++) {
				this.data[i][j] = Long.valueOf(data[i][j]);
			}
		}
	}

	/**
	 * Set the current values 
	 * 
	 * @param data the values to set
	 * @param bUnsigned true, if these values are unsigned; false otherwise.
	 */
	public void setData(short[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE16;
		this.bUnsigned = bUnsigned; 
		for (int i = 0; i < data.length; i++) {
			for (int j = 0; j < data[0].length; j++) {
				this.data[i][j] = Long.valueOf(data[i][j]);
			}
		}
	}

	/**
	 * Set the current values 
	 * 
	 * @param data the values to set
	 * @param bUnsigned true, if these values are unsigned; false otherwise.
	 */
	public void setData(int[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE32;
		this.bUnsigned = bUnsigned; 
		for (int i = 0; i < data.length; i++) {
			for (int j = 0; j < data[0].length; j++) {
				this.data[i][j] = Long.valueOf(data[i][j]);
			}
		}
	}

	/**
	 * Set the current values 
	 * 
	 * @param data the values to set
	 * @param bUnsigned true, if these values are unsigned; false otherwise.
	 */
	public void setData(long[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE64;
		this.bUnsigned = bUnsigned; 
		for (int i = 0; i < data.length; i++) {
			for (int j = 0; j < data[0].length; j++) {
				this.data[i][j] = Long.valueOf(data[i][j]);
			}
		}
	}

	/**
	 * @return the values
	 */
	public Long[][] getData() {
		return data;
	}
	
	/**
	 * @return the precision of the values
	 */
	public IntegerType getPrec() {
		return prec;
	}

	/**
	 * @return true, if the values are signed, false otherwise.
	 */
	public boolean isUnsigned() {
		return bUnsigned;
	}

	/**
	 * @return the height of the value matrix
	 * @see org.scilab.modules.types.scilabTypes.ScilabType#getHeight()
	 */
	@Override
	public int getHeight() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}

	/**
	 * @return the width of the value matrix
	 * @see org.scilab.modules.types.scilabTypes.ScilabType#getWidth()
	 */
	@Override
	public int getWidth() {
		if (data == null) {
			return 0;
		}
		return data[0].length;
	}

	// int32(X), int8(x) , int16([x,x,x;x,x,x])
	//	 uint32(X), uint8(x) , uint16([x,x,x;x,x,x])
	
	/**
	 * @return true, if there is no values; false otherwise.
	 */
    public boolean isEmpty() {
    	return (data == null);
    }
	
    /**
     * @return the pretty-printed values 
     * @see java.lang.Object#toString()
     */
    @Override
	public String toString() {
    	StringBuilder result = new StringBuilder();
		
		
		if (isEmpty()) {
			result.append("int()");
			return result.toString();
		}
		
		if (isUnsigned()) {
			result.append("u");
		}
		result.append("int");
		
		switch (getPrec()) {
		case TYPE8:
			result.append("8");
			break;

		case TYPE16:
			result.append("16");
			break;
			
		case TYPE32:
			result.append("32");
			break;
			
		case TYPE64:
			result.append("64");
			break;
			
		default:
			break;
		}

		result.append("(");
		appendData(result);
		result.append(")");
	
		return result.toString();
	}

	/**
	 * Put each value on the buffer.
	 * 
	 * @param result the current buffer
	 */
	private void appendData(StringBuilder result) {
		for (int i = 0; i < getHeight(); ++i) {
			for (int j = 0; j < getWidth(); ++j) {

				result.append(getData()[i][j]);
				
				
				if (j != getWidth() - 1) {
					result.append(", ");
				}
			}
			if (i != getHeight() - 1) {
				result.append(" ; ");
			}
		}
	}
	
}
