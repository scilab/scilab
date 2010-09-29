/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.javasci;

import java.io.File;
import java.io.FileNotFoundException;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.javasci.Call_Scilab;
import org.scilab.modules.javasci.JavasciException.AlreadyRunningException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.javasci.JavasciException.UnknownTypeException;
import org.scilab.modules.javasci.JavasciException.ScilabInternalException;


/**
 * This class provides the capability to access to the Scilab engine from
 * a Java application.
 * <br>
 * Example:<br />
 * <code>
 * Scilab sci = new Scilab();<br />
 * if (sci.open()) {<br />
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<br />
 * ScilabDouble aOriginal = new ScilabDouble(a);<br />
 * sci.put("a",aOriginal);<br />
 * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<br />
 * }<br />
 * </code>
 * @see org.scilab.modules.types
 */
public class Scilab {

	private String SCI = null;
	private boolean advancedMode = false;

	/**
	 * Creator of the Scilab Javasci object. 
	 * Scilab data path is autodetected and advanced features disabled
	 */
	public Scilab() throws InitializationException {
		this(null, false);
	}

	/**
	 * Creator of the Scilab Javasci object with a specific Scilab path.
	 * Advanced features are disabled (faster)
	 * @param SCI provide the path to Scilab data
	 */
	public Scilab(String SCI) throws InitializationException {
		this(SCI, false);
	}

	/**
	 * Creator of the Scilab Javasci object in advanced mode
	 * Scilab data path is autodetected 
	 * @param advancedMode true enables the advanced mode (GUI, graphics, Tcl/Tk, sciNotes...). Smaller.
	 */
	public Scilab(boolean advancedMode) throws InitializationException {
		this(null, advancedMode);
	}

	/**
	 * Creator of the Scilab Javasci object. 
	 * Under GNU/Linux / Mac OS X, try to detect Scilab base path
	 * if the property SCI is set, use it
	 * if not, try with the global variable SCI
	 * if not, throws a new exception
	 * Under Windows, use also the registery
	 * @param SCIPath the path to Scilab data
	 * @param advancedMode true enables the advanced mode (GUI, graphics, Tcl/Tk, sciNotes...). Smaller.
	 */
	public Scilab(String SCIPath, boolean advancedMode) throws InitializationException {
		String SCI = SCIPath;
		if (!System.getProperty("os.name").toLowerCase().contains("windows")) {
			if (SCI==null) {
				// Auto detect
				try {
					SCI = System.getProperty("SCI");
					if (SCI == null || SCI.length() == 0) {
						SCI = System.getenv("SCI");
						if (SCI == null || SCI.length() == 0) {
							throw new InitializationException("Auto detection of SCI failed.\nSCI empty.");
						}
					}
				} catch (Exception e) {
					throw new InitializationException("Auto detection of SCI failed.\nCould not retrieve the variable SCI.", e);
				}
			}
		}
		this.advancedMode = advancedMode;
		this.initScilab(SCI);

	}


	private void initScilab(String SCI) throws InitializationException {
		/* Let Scilab engine knows that he is run through the Javasci API */
		Call_Scilab.SetFromJavaToON();
		if (!System.getProperty("os.name").toLowerCase().contains("windows")) {
			File f = new File(SCI);
			if (!f.isDirectory()) {
				throw new InitializationException("Could not find directory " + f.getAbsolutePath());
			}
			this.SCI = SCI;
		}
	}

	/**
	 * Open a connection to the Scilab engine
	 * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
	 * @return if the operation is successful
     * @throws AlreadyRunningException Scilab is already running
     * @throws InitializationException Cannot start Scilab
	 */
	public boolean open() throws JavasciException {
		int res = Call_Scilab.Call_ScilabOpen(this.SCI, this.advancedMode, null, -1);
		switch (res) {
			case -1: 
				throw new AlreadyRunningException("Javasci already running.");
			case -2:
				/* Should not occurd (processed before) */
				throw new InitializationException("Could not find SCI.");
			case -3:
				throw new InitializationException("No existing directory.");
		}
		return true;
	}

	/**
	 * Open a connection to the Scilab engine and run the command job
	 * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
	 * @param job The job to run on startup
	 * @return if the operation is successful
	 */
	public boolean open(String job) throws JavasciException {
		if (!this.open()) {
			return false;
		}

		return this.exec(job);
	}

	/**
	 * Open a connection to the Scilab engine and run commands job
	 * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
	 * @param jobs The serie of jobs to run on startup
	 * @return if the operation is successful
	 */
	public boolean open(String jobs[]) throws JavasciException {
		if (!this.open()) {
			return false;
		}

		return this.exec(jobs);
	}


	/**
	 * Open a connection to the Scilab engine and run thefile scriptFilename
	 * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
	 * @param scriptFilename The script to execute on startup
	 * @return if the operation is successful
	 */
	public boolean open(File scriptFilename) throws JavasciException, FileNotFoundException {
		if (!this.open()) {
			return false;
		}

		return this.exec(scriptFilename);
	}


	/**
	 * Execute a single command in Scilab
	 * This function is based on SendScilabJob from call_scilab
	 * @param job the job to execute
	 * @return if the operation is successful
	 */
	public boolean exec(String job) {
		return (Call_Scilab.SendScilabJob(job) == 0);
	}


	/**
	 * Execute several commands in Scilab
	 * This function is based on SendScilabJob from call_scilab
	 * @param jobs the serie of job to execute
	 * @return if the operation is successful
	 */
	public boolean exec(String jobs[]) {
		return (Call_Scilab.SendScilabJobs(jobs, jobs.length) == 0);
	}


	/**
	 * Execute a Scilab script .sce/.sci
	 * This function is based on SendScilabJob from call_scilab
	 * @param scriptFilename the script to execute
	 * @return if the operation is successful
	 */
	public boolean exec(File scriptFilename) throws FileNotFoundException {
		if (!scriptFilename.exists()) {
			throw new FileNotFoundException("Could not find " + scriptFilename);
		}
		return this.exec("exec('" + scriptFilename + "');");
	}


	/**
	 * Detect if a variable (varname) exists in Scilab
	 * @param varname the variable to check
	 * @return if the variable exists or not
	 */
	public boolean isExistingVariable(String varname) {
		return Call_Scilab.isExistingVariable(varname);
	}


	/**
	 * Shutdown Scilab
	 * This function is based on TerminateScilab from call_scilab
	 * @return if the operation is successful
	 */
	public boolean close() {
		boolean res = Call_Scilab.TerminateScilab(null);
		return res;
	}


	/**
	 * Return the last error code
	 * @return the error code
	 */
	public int getLastErrorCode() {
		return Call_Scilab.GetLastErrorCode();
	}


	/**
	 * Return the last error message
	 * @return the error message itself
	 */
	public String getLastErrorMessage() {
		return Call_Scilab.getLastErrorMessage();
	}


	/**
	 * Detect if a Scilab graphic window is still opened
	 * This function is based on ScilabHaveAGraph from call_scilab
	 * @return if the graphic is open or not
	 */
	public boolean isGraphicOpened() {
		return Call_Scilab.isGraphicOpened();
	}

	/**
	 * Return the code type of a variable varname
	 * The int refers to a Java enum from ScilabType.types
	 * @param varName the name of the variable
	 * @return the type of the variable
     * @throws UndefinedVariableException The variable does not exist
     * @throws UnknownTypeException Cannot find the type
	 */
	public ScilabTypeEnum getVariableType(String varName) throws JavasciException {
		ScilabTypeEnum variableType = null;
		try {
			variableType = Call_Scilab.getVariableType(varName);
			if (variableType == null ) {
				throw new UndefinedVariableException("Could not find the type of the variable '"+varName+"'");
			}
		} catch (IllegalArgumentException e) {
			String lastWord = e.getMessage().substring(e.getMessage().lastIndexOf(' ')+1);
			if (lastWord.equals("-2")) { /* Crappy workaround. Parse the exception */
				throw new UndefinedVariableException("Could not find variable '"+varName+"'");
			}
			throw new UnknownTypeException("Type of "+varName+" unknown");

		}
		return variableType;
	}

	/**
	 * Returns a variable named varname
	 * Throws an exception if the datatype is not managed or if the variable is not available
	 * @param varname the name of the variable
	 * @return return the variable 
     * @throws UnsupportedTypeException Type not managed yet.
	 */
	public ScilabType get(String varname) throws JavasciException {
		ScilabTypeEnum sciType = this.getVariableType(varname);
		switch (sciType) {
			case sci_matrix:
				if (!Call_Scilab.isComplex(varname)) {
					return new ScilabDouble(Call_Scilab.getDouble(varname));
				} else {
					return new ScilabDouble(Call_Scilab.getDoubleComplexReal(varname), Call_Scilab.getDoubleComplexImg(varname));
				}
  
			case sci_boolean:
				return new ScilabBoolean(Call_Scilab.getBoolean(varname));
 
			case sci_strings:
				return new ScilabString(Call_Scilab.getString(varname));
 
			case sci_ints:
				ScilabIntegerTypeEnum typeInt = Call_Scilab.getIntegerPrecision(varname);
 
				switch (typeInt) {
					case sci_int8:
						return new ScilabInteger(Call_Scilab.getByte(varname), false);
					case sci_uint8:
						return new ScilabInteger(Call_Scilab.getUnsignedByte(varname), true);
					case sci_int16:
						return new ScilabInteger(Call_Scilab.getShort(varname), false);
					case sci_uint16:
						return new ScilabInteger(Call_Scilab.getUnsignedShort(varname), true);
					case sci_int32:
						return new ScilabInteger(Call_Scilab.getInt(varname), false);
					case sci_uint32:
						return new ScilabInteger(Call_Scilab.getUnsignedInt(varname), true);
					case sci_int64:
					case sci_uint64:
						// will be available in Scilab 6
						throw new UnsupportedTypeException("64 bit (signed and unsigned) integer types not managed in Scilab 5.X");

				}

			default:
				throw new UnsupportedTypeException("Type not managed: " + sciType);
		
		}
	}



	/**
	 * Send to Scilab a variable theVariable named varname
	 * Throws an exception if the datatype is not managed or if the variable is not available
	 * @param varname the name of the variable
	 * @param theVariable the variable itself
	 * @return true if the operation is successful
     * @throws UnsupportedTypeException Type not managed yet.
	 */
	public boolean put(String varname, ScilabType theVariable) throws JavasciException {
		int err = -999; /* -999: if the type is not handled */

		if (theVariable instanceof ScilabDouble) {
			ScilabDouble sciDouble = (ScilabDouble)theVariable;
			if (sciDouble.isReal()) {
				err = Call_Scilab.putDouble(varname, sciDouble.getRealPart());
			} else {
				// Special case. Serialize the matrix from Scilab same way
				//  Scilab stores them (columns by columns)
				// plus the complex values at the second part of the array
				err = Call_Scilab.putDoubleComplex(varname,sciDouble.getSerializedComplexMatrix(), sciDouble.getHeight(), sciDouble.getWidth());
			}
		}

		if (theVariable instanceof ScilabInteger) {
			ScilabInteger sciInteger = (ScilabInteger)theVariable;
			switch (sciInteger.getPrec()) {
				case sci_uint8:
					err = Call_Scilab.putUnsignedByte(varname, sciInteger.getDataAsByte());
					break;
				case sci_int8:
					err = Call_Scilab.putByte(varname, sciInteger.getDataAsByte());
					break;
				case sci_uint16:
					err = Call_Scilab.putUnsignedShort(varname, sciInteger.getDataAsShort());
					break;
				case sci_int16:
					err = Call_Scilab.putShort(varname, sciInteger.getDataAsShort());
					break;
				case sci_uint32:
					err = Call_Scilab.putUnsignedInt(varname, sciInteger.getDataAsInt());
					break;
				case sci_int32:
					err = Call_Scilab.putInt(varname, sciInteger.getDataAsInt());
					break;
				case sci_uint64:
//					err = Call_Scilab.putUnsignedLong(varname, sciInteger.getData_());
				case sci_int64:
//					err = Call_Scilab.putLong(varname, sciInteger.getData_());
					break;
			}
		}

		if (theVariable instanceof ScilabBoolean) {
			ScilabBoolean sciBoolean = (ScilabBoolean)theVariable;
			err = Call_Scilab.putBoolean(varname, sciBoolean.getData());
		}

		if (theVariable instanceof ScilabString) {
			ScilabString sciString = (ScilabString)theVariable;
			err = Call_Scilab.putString(varname, sciString.getData());
		}

		if (err == -999) {
			throw new UnsupportedTypeException("Type not managed: " + theVariable.getClass());
		} else {
			if (err != 0) {
				throw new ScilabInternalException("Storage of the variable '"+varname+"' ("+theVariable.getClass()+") failed.");
			}
		}
		return true;
	}
}
