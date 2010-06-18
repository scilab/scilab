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

import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabTypeEnum;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabInteger;
import org.scilab.modules.javasci.Call_Scilab;

public class Scilab {
    private String SCI = "/pro/git/javasci-v2/scilab/";
    public static int plop = 42;
    
    public void Scilab(String SCI){
        this.SCI = SCI;
    }

    /**
     * Open a connection to the Scilab engine
     * This function is based on StartScilab from call_scilab
     * @return if the operation is successful
     */
    public boolean open() {
//        System.out.println("SCI : " + SCI);
        return Call_Scilab.StartScilab(this.SCI, null, null);
    }

    /**
     * Open a connection to the Scilab engine and run the command job
     * This function is based on StartScilab from call_scilab
     * @param job The job to run on startup
     * @return if the operation is successful
     */
    public boolean open(String job) {
        this.open();
        return this.exec(job);
    }

    /**
     * Open a connection to the Scilab engine and run commands job
     * This function is based on StartScilab from call_scilab
     * @param jobs The serie of jobs to run on startup
     * @return if the operation is successful
     */
    public boolean open(String jobs[]) {
        this.open();
        return this.exec(jobs);
    }


    /**
     * Open a connection to the Scilab engine and run thefile scriptFilename
     * This function is based on StartScilab from call_scilab
     * @param job The script to execute on startup
     * @return if the operation is successful
     */
    public boolean open(File scriptFilename) {
        this.open();
        return this.exec(scriptFilename);
    }


    /**
     * Execute a single command in Scilab
     * This function is based on SendScilabJob from call_scilab
     * @param job the job to execute
     * @return if the operation is successful
     */
    public boolean exec(String job) {
        return (Call_Scilab.SendScilabJob(job) == 1);
    }


    /**
     * Execute several commands in Scilab
     * This function is based on SendScilabJob from call_scilab
     * @param job the serie of job to execute
     * @return if the operation is successful
     */
    public boolean exec(String jobs[]) {
        return (Call_Scilab.SendScilabJobs(jobs, jobs.length) == 1);
    }


    /**
     * Execute a Scilab script .sce/.sci
     * This function is based on SendScilabJob from call_scilab
     * @param job the script to execute
     * @return if the operation is successful
     */
    public boolean exec(File scriptFilename) {
        return this.exec("exec('" + scriptFilename + "');");
    }


    /**
     * Detect if a variable (varname) exists in Scilab
     * @param varname the variable to check
     * @return if the variable exists or not
     */
    public boolean isExistingVariable(String varname) {
        return true;
    }


    /**
     * Shutdown Scilab
     * This function is based on TerminateScilab from call_scilab
     * @return if the operation is successful
     */
    public boolean close() {
        return Call_Scilab.TerminateScilab(null);
    }


    /**
     * Return the last error code
     * @return the error code
     * @todo Check what is actually this number
     */
    public int getLastErrorCode() {
        return Call_Scilab.GetLastErrorCode();
    }


    /**
     * Return the last error message
     * @return the error message itself
     */
    String getLastErrorMessage() {
        return "getLastErrorMessage";
    }


    /**
     * Detect if a Scilab graphic window is still opened
     * This function is based on ScilabHaveAGraph from call_scilab
     * @return if the graphic is open or not
     */
    public boolean isGraphicOpened() {
        return false;
    }


    /**
     * Return the code type of a variable varname
     * The int refers to a Java enum from ScilabType.types
     * @param varname the name of the variable
     * @return the type of the variable
     * @todo check the enum here
    */
    public ScilabTypeEnum getVariableType(String varName) {
        ScilabTypeEnum variableType = null;
        try {
            variableType = Call_Scilab.getVariableType(varName);
        } catch (IllegalArgumentException e) {
            System.err.println("Could not find variable type: " + e.getLocalizedMessage());
        }

//        ScilabTypeEnum typeEnum = ScilabTypeEnum.swigToEnum(variableType);
        return variableType;
        
//        return typeEnum;

//        int varType = 
//        System.out.println(ScilabTypeEnum(varType));

    }




    /**
     * Returns a variable named varname
     * Throws an exception if the datatype is not managed or if the variable is not available
     * @param varname the name of the variable
     * @return return the variable 
    */
    public ScilabType get(String varname) {
		return null;
    }


    /**
     * Send to Scilab a variable theVariable named varname
     * Throws an exception if the datatype is not managed or if the variable is not available
     * @param varname the name of the variable
     * @param varname the variable itself
     * @return if the operation is successful
    */
    public boolean put(String varname, ScilabType theVariable) {
        if (theVariable instanceof ScilabDouble) {
			return true;
		}
		if (theVariable instanceof ScilabInteger) {
			return true;
		}
		//TODO: a remplacer par la bonne exception return new UnsupportedTypeException();
		return false;
    }

}
