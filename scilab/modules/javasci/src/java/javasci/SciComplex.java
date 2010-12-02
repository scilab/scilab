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
 * Scilab Complex object
 * See SCI/modules/javasci/examples/others for some simple examples 
 * @author Allan CORNET - INRIA 2006
 * @author Sylvestre LEDRU - INRIA 2007
 */
public class SciComplex extends SciAbstractDataType implements java.io.Serializable  
{
	private String complexRepresentation = "i";
	private String typeDescription = "complex";
		
	/**
	 * Constructs a Scilab Complex from a other SciComplex
     * @deprecated
	 * @param name the name of the Scilab variable
	 * @param complexObj the SciComplex you want to copy
	 */
  public SciComplex(String name, SciComplex complexObj) {
  	double[] complexReal = new double[1];
  	double[] complexImaginary = new double[1];

  	complexReal[0] = complexObj.getRealPartData();
  	complexImaginary[0] = complexObj.getImaginaryPartData();
  	
	sciArray = new SciComplexArray(name, 1, 1, complexReal, complexImaginary);
 }

	/**
	 * Constructs a Scilab Complex
	 * initialized to 0 for both part (Real & Imaginary)
     * @deprecated
	 * @param name  the name of the Scilab Variable 
	 */ 
 public SciComplex(String name) {
 		
	sciArray = new SciComplexArray(name, 1, 1);
  }

	/**
	 * Constructs a Scilab Double from two Java Doubles (RealPart & ImaginaryPart)
     * @deprecated
	 * @param name the name of the Scilab Variable 
	 * @param realPart the real value
	 * @param imaginaryPart the imaginary value
	 */ 
  public SciComplex(String name, double realPart, double imaginaryPart) {
	double[] complexReal = new double[1];
  	double[] complexImaginary = new double[1];

  	complexReal[0] = realPart;
  	complexImaginary[0] = imaginaryPart;
	sciArray = new SciComplexArray(name, 1, 1, complexReal, complexImaginary);
 }

	/**
	 * Return the real part
     * @deprecated
	 * @return the real part
	 */
  public double getRealPartData() {
    Get();
    double[] complexReal = ((SciComplexArray) sciArray).getRealPartData();
    return complexReal[0];
  }

	/**
	 * Return the imaginary part
     * @deprecated
	 * @return the imaginary part
	 */
  public double getImaginaryPartData() {
   Get();
   double[] complexImaginary = ((SciComplexArray) sciArray).getImaginaryPartData();
   return complexImaginary[0];
  }

	/** 
	 * Display the complex
     * @deprecated
	 * @return the display of the complex
	 */
  public String toString() {
   	double[] complexImaginary = ((SciComplexArray) sciArray).getImaginaryPartData();
   	double[] complexReal = ((SciComplexArray) sciArray).getRealPartData();
   	String valToReturn;
   	final String addStr = " + ";

    Get();
	if (complexReal[0] != 0 && complexImaginary[0] > 0) {
		valToReturn = complexReal[0] + addStr + complexImaginary[0] + complexRepresentation; }
	else if (complexReal[0] != 0 && complexImaginary[0] < 0) {
    	valToReturn = complexReal[0] + " - " + (-complexImaginary[0]) + complexRepresentation; }
    else if (complexImaginary[0] == 0) {
    	valToReturn = String.valueOf(complexReal[0]); }
    else if (complexReal[0] == 0) {
    	valToReturn = complexImaginary[0] + complexRepresentation; 
    }
    else {
    	valToReturn = complexReal[0] + addStr + complexRepresentation + "*" + complexImaginary[0]; 
     }
    /*(unless Inf or NaN)*/
    return valToReturn;
        
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

