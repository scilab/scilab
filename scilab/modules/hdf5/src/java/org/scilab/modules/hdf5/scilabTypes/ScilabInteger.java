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

package org.scilab.modules.hdf5.scilabTypes;

public class ScilabInteger implements ScilabType {
	public enum IntegerType{
		TYPE8,
		TYPE16,
		TYPE32,
		TYPE64
	}
	
	public ScilabInteger() {
		data = null;
	}

	public ScilabInteger(byte[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	public ScilabInteger(short[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	public ScilabInteger(int[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	public ScilabInteger(long[][] data, boolean bUnsigned) {
		this.setData(data, bUnsigned);
	}

	public ScilabInteger(byte value) {
		this.data = new Long[1][1];
		this.data[0][0] = new Long(value);
	}

	public ScilabInteger(short value) {
		this.data = new Long[1][1];
		this.data[0][0] = new Long(value);
	}

	public ScilabInteger(int value) {
		this.data = new Long[1][1];
		this.data[0][0] = new Long(value);
	}

	public ScilabInteger(long value) {
		this.data = new Long[1][1];
		this.data[0][0] = new Long(value);
	}

	public void setData(byte[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE8;
		this.bUnsigned = bUnsigned; 
		for(int i = 0 ; i < data.length ; i++) {
			for(int j = 0 ; j < data[0].length ; j++) {
				this.data[i][j] = new Long(data[i][j]);
			}
		}
	}

	public void setData(short[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE16;
		this.bUnsigned = bUnsigned; 
		for(int i = 0 ; i < data.length ; i++) {
			for(int j = 0 ; j < data[0].length ; j++) {
				this.data[i][j] = new Long(data[i][j]);
			}
		}
	}

	public void setData(int[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE32;
		this.bUnsigned = bUnsigned; 
		for(int i = 0 ; i < data.length ; i++) {
			for(int j = 0 ; j < data[0].length ; j++) {
				this.data[i][j] = new Long(data[i][j]);
			}
		}
	}

	public void setData(long[][] data, boolean bUnsigned) {
		this.data = new Long[data.length][data[0].length];
		prec = IntegerType.TYPE64;
		this.bUnsigned = bUnsigned; 
		for(int i = 0 ; i < data.length ; i++) {
			for(int j = 0 ; j < data[0].length ; j++) {
				this.data[i][j] = new Long(data[i][j]);
			}
		}
	}

	public Long[][] getData() {
		return data;
	}
	
	public IntegerType getPrec() {
		return prec;
	}

	public boolean isUnsigned() {
		return bUnsigned;
	}

	public int getHeight() {
		if (data == null) {
			return 0;
		}
		return data.length;
	}

	public int getWidth() {
		if (data == null) {
			return 0;
		}
		return data[0].length;
	}

	// int32(X), int8(x) , int16([x,x,x;x,x,x])
//	 uint32(X), uint8(x) , uint16([x,x,x;x,x,x])
	
    public boolean isEmpty(){
    	return (data == null);
    }
	
	public String toString() {
		StringBuffer result = new StringBuffer();
		
		
		if (isEmpty()) {
			result.append("int()");
			return result.toString();
		}
		
		if (isUnsigned() ){
			result.append("u");
		}
		result.append("int");
		if (getPrec() == IntegerType.TYPE8){
			result.append("8");
		} else if (getPrec() == IntegerType.TYPE16){
			result.append("16");		
		} else if (getPrec() == IntegerType.TYPE32){
			result.append("32");
		} else if (getPrec() == IntegerType.TYPE64){
			result.append("64");
		}

		result.append("(");
		for (int i = 0 ; i < getHeight() ; ++i) {
			for (int j = 0 ; j < getWidth() ; ++j) {

				result.append(getData()[i][j]);
				
				
				if (j != getWidth() - 1) {
					result.append(", ");
				}
			}
			if (i != getHeight() - 1) {
				result.append(" ; ");
			}
		}
		result.append(")");
	
		return result.toString();
	}
	
	
	private Long[][] data;
	private IntegerType prec;
	private boolean bUnsigned;
}
