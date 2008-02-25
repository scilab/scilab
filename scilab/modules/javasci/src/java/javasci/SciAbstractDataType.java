/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
 * Defines the basic method that very SciDataType should implement
 * Cannot be implemented
 * @author Sylvestre LEDRU - INRIA 2007
 */
public abstract class SciAbstractDataType {
	/**
	 * the data 
	 */
	protected SciAbstractArray sciArray;

	/**
	 * Return the description of the DataType
	 * Description must set in any class which extends SciAbstractDataType
	 * @return the Description
	 *
	 */
	abstract String getTypeDescription();

	/**
	 * Returns the name of the Scilab variable
	 * @return the name
	 */
	public String getName()	{
		return sciArray.getName();
	}

	/**
	 * Load the data from Scilab
	 */
	public void Get() {
		sciArray.Get();
	}
	
	/**
	 * Send the data to Scilab 
	 */
	public void Send() {
		sciArray.Send();
	}
	

	/**
	 * Execute a command in Scilab 
	 * Deprecated. Use Scilab.Exec instead.
	 * @param job the Scilab job 
	 * @return the result of the operation
	 * @deprecated
	 */
	public boolean Job(String job) {
		return sciArray.Job(job);
	}  

	/** 
	 * Display the variable 
	 */
	public void disp() {
		Get();
		System.out.println(getTypeDescription() + " " + getName() + "=");
		Scilab.Exec("disp(" + getName() + ");");
		System.out.flush();
	}
}
