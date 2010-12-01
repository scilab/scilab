/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * ...
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
 * Scilab String object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciString extends SciAbstractDataType implements java.io.Serializable  
{
	private String typeDescription = "String";

	/**
	 * Constructs a Scilab String from a other SciString
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param stringObj the SciString you want to copy
	 */
  public SciString(String name, SciString stringObj) {
  	String[] pTmpStringArray = new String[1];
  	pTmpStringArray[0] = stringObj.getData();
	sciArray = new SciStringArray(name, 1, 1, pTmpStringArray);
 }

	/**
	 * Constructs a Scilab String
	 * initialized to ""
     * @deprecated
	 * @param name the name of the Scilab Variable 
	 */ 
  public SciString(String name) {
  	sciArray = new SciStringArray(name, 1, 1);
 }

	/**
	 * Constructs a Scilab String from a Java String
     * @deprecated
	 * @param name the name of the Scilab Variable 
	 * @param strValue the value
	 */ 
  public SciString(String name, String strValue) {
  	String[] pTmpStringArray = new String[1];
  	pTmpStringArray[0] = strValue;
	sciArray = new SciStringArray(name, 1, 1, pTmpStringArray);
 }

	/**
	 * Return the string
     * @deprecated
	 * @return the String
	 */
  public String getData() {
    Get();
    String[] pTmpStringArray = ((SciStringArray) sciArray).getData();
    return pTmpStringArray[0];
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
  
