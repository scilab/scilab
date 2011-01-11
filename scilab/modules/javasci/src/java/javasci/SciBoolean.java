/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package javasci;

/**
 * Scilab Boolean object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2007
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciBoolean extends SciAbstractDataType implements java.io.Serializable  
{
	private String typeDescription = "boolean";
	
	/**
	 * Constructs a Scilab Boolean from a other SciBoolean
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param booleanObj the Sciboolean you want to copy
	 */
	public SciBoolean(String name, SciBoolean booleanObj) {
		boolean[] pTmpBooleanArray = new boolean[1];
		pTmpBooleanArray[0] = booleanObj.getData();
		sciArray = new SciBooleanArray(name, 1, 1, pTmpBooleanArray);
	}

	/**
	 * Constructs a Scilab Boolean
     * @deprecated
	 * @param name the name of the Scilab variable
	 */
	public SciBoolean(String name) {
		sciArray = new SciBooleanArray(name, 1, 1);

	}

	/**
	 * Constructs a Scilab Boolean from a java boolean
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param value the boolean value
	 */
	public SciBoolean(String name, boolean value) {
		boolean[] pTmpBooleanArray = new boolean[1];
		pTmpBooleanArray[0] = value;
		sciArray = new SciBooleanArray(name, 1, 1, pTmpBooleanArray);
	}

	/**
	 * Returns the boolean value
     * @deprecated
	 * @return the value
	 */
	public boolean getData() {
		Get();
		boolean[] pTmpBooleanArray = ((SciBooleanArray) sciArray).getData();
		return pTmpBooleanArray[0];
	}

	/**
	 * Return the description of the DataType
	 * Description must set in any class which extends SciAbstractDataType
     * @deprecated
	 * @return the Description
	 *
	 */
	public String getTypeDescription() {
		return this.typeDescription;
	}

}
/********************************************************************************************************/  
