/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
 * Scilab Integer object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - DIGITEO 2009
 */
public class SciInteger extends SciAbstractDataType implements java.io.Serializable {
	private String typeDescription = "integer";
	
	/**
	 * Constructs a Scilab Integer from a other SciInteger
	 * @param name the name of the Scilab variable
	 * @param intObj the SciInteger you want to copy
	 */
  public SciInteger(String name, SciInteger intObj) {
	  int[] pTmpIntegerArray = new int[1];
	  pTmpIntegerArray[0] = intObj.getData();
	  sciArray = new SciIntegerArray(name, 1, 1, pTmpIntegerArray);
 }

	/**
	 * Constructs a Scilab Integer 
	 * initialized to 0
	 * @param name  the name of the Scilab Variable 
	 */ 
 public SciInteger(String name) {
	sciArray = new SciIntegerArray(name, 1, 1);
 }

	/**
	 * Constructs a Scilab Integer from a Java Integer
	 * @param name the name of the Scilab Variable 
	 * @param value the value
	 */ 
  public SciInteger(String name, int value) {
	int[] pTmpIntegerArray = new int[1];
  	pTmpIntegerArray[0] = value;
	sciArray = new SciIntegerArray(name, 1, 1, pTmpIntegerArray);
 }

	/**
	 * Return the int
	 * @return the int
	 */
  public int getData() {
    Get();
    int[] pTmpIntegerArray = ((SciIntegerArray) sciArray).getData();
    return pTmpIntegerArray[0];
}

	/**
	 * Return the description of the DataType
	 * Description must set in any class which extends SciAbstractDataType
	 * @return the Description
	 *
	 */
	public String getTypeDescription() {
		return this.typeDescription;
	}

}
