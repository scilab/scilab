/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
 * Scilab Double object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciDouble extends SciAbstractDataType implements java.io.Serializable {
	private String typeDescription = "double";
	
	/**
	 * Constructs a Scilab Double from a other SciDouble
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param doubleObj the SciDouble you want to copy
	 */
  public SciDouble(String name, SciDouble doubleObj) {
	  double[] pTmpDoubleArray = new double[1];
	  pTmpDoubleArray[0] = doubleObj.getData();
	  sciArray = new SciDoubleArray(name, 1, 1, pTmpDoubleArray);
 }

	/**
	 * Constructs a Scilab Double 
	 * initialized to 0
     * @deprecated
	 * @param name  the name of the Scilab Variable 
	 */ 
 public SciDouble(String name) {
	sciArray = new SciDoubleArray(name, 1, 1);
 }

	/**
	 * Constructs a Scilab Double from a Java Double
     * @deprecated
	 * @param name the name of the Scilab Variable 
	 * @param value the value
	 */ 
  public SciDouble(String name, double value) {
	double[] pTmpDoubleArray = new double[1];
  	pTmpDoubleArray[0] = value;
	sciArray = new SciDoubleArray(name, 1, 1, pTmpDoubleArray);
 }

	/**
	 * Return the double
     * @deprecated
	 * @return the double
	 */
  public double getData() {
    Get();
    double[] pTmpDoubleArray = ((SciDoubleArray) sciArray).getData();
    return pTmpDoubleArray[0];
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
