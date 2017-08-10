/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - INRIA - Sylvestre LEDRU
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
package org.scilab.modules.javasci;

import java.io.File;
import java.io.FileNotFoundException;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;
import org.scilab.modules.types.ScilabIntegerTypeEnum;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabBoolean;
import org.scilab.modules.types.ScilabBooleanSparse;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabPolynomial;
import org.scilab.modules.types.ScilabSparse;
import org.scilab.modules.javasci.Call_Scilab;
import org.scilab.modules.javasci.JavasciException.AlreadyRunningException;
import org.scilab.modules.javasci.JavasciException.InitializationException;
import org.scilab.modules.javasci.JavasciException.UnsupportedTypeException;
import org.scilab.modules.javasci.JavasciException.UndefinedVariableException;
import org.scilab.modules.javasci.JavasciException.UnknownTypeException;
import org.scilab.modules.javasci.JavasciException.ScilabInternalException;
import org.scilab.modules.javasci.JavasciException.ScilabErrorException;

/**
 * This class provides the capability to access to the Scilab engine from
 * a Java application.<BR>
 * <BR>
 * Example:<BR>
 * <code>
 * Scilab sci = new Scilab();<BR>
 * if (sci.open()) {<BR>
 * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
 * ScilabDouble aOriginal = new ScilabDouble(a);<BR>
 * sci.put("a",aOriginal);<BR>
 * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<BR>
 * }<BR>
 * </code>
 * @see org.scilab.modules.types
 */
public class Scilab {

    private static int notHandledError = -999;

    private String SCI = null;
    private boolean advancedMode = false;

    /**
     * Creator of the Scilab Javasci object. <BR>
     * <UL>
     * <LI>Scilab data path is autodetected (SCI) </LI>
     * <LI>advanced features are disabled (no Java nor TCL/TK features) </LI>
     * </UL>
     */
    public Scilab() throws InitializationException {
        this(null, false);
    }

    /**
     * Creator of the Scilab Javasci object with a specific Scilab path.<BR>
     * Advanced features are disabled (no Java nor TCL/TK features)
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab("/path/to/Scilab/data/dir/");<BR>
     * <BR>
     * </code>
     * @param SCI provide the path to Scilab data
     */
    public Scilab(String SCI) throws InitializationException {
        this(SCI, false);
    }

    /**
     * Creator of the Scilab Javasci object in advanced mode<BR>
     * Scilab data path is autodetected
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab(true); // Starts in advanced mode<BR>
     * <BR>
     * </code>
     * @param advancedMode true enables the advanced mode (GUI, graphics, Tcl/Tk, sciNotes...). Smaller.
     */
    public Scilab(boolean advancedMode) throws InitializationException {
        this(null, advancedMode);
    }

    /**
     * Creator of the Scilab Javasci object. <BR>
     * Under GNU/Linux / Mac OS X, try to detect Scilab base path<BR>
     * if the property SCI is set, use it<BR>
     * if not, try with the global variable SCI<BR>
     * if not, throws a new exception<BR>
     * Under Windows, use also the registery<BR>
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab("/path/to/Scilab/data/dir/",true); // Starts in advanced mode<BR>
     * <BR>
     * </code>
     * @param SCIPath the path to Scilab data
     * @param advancedMode true enables the advanced mode (GUI, graphics, Tcl/Tk, sciNotes...). Smaller.
     */
    public Scilab(String SCIPath, boolean advancedMode) throws InitializationException {
        String SCI = SCIPath;
        if (!System.getProperty("os.name").toLowerCase().contains("windows")) {
            if (SCI == null) {
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
     * Open a connection to the Scilab engine<BR>
     * This function is based on Call_ScilabOpen from call_scilab<BR>
     * Note: For now, only one instance of Scilab can be launched<BR>
     * A second launch will return FALSE<BR>
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab();<BR>
     * sci.open();<BR>
     * <BR>
     * </code>
     * @return if the operation is successful
     * @throws AlreadyRunningException Scilab is already running
     * @throws InitializationException Cannot start Scilab
     */
    public boolean open() throws JavasciException {
        int res = Call_Scilab.Call_ScilabOpen(this.SCI, this.advancedMode, null, -1);
        switch (res) {
            case 0: /* Success */
                return true;
            case -1:
                throw new AlreadyRunningException("Javasci already running.");
            case -2:
                /* Should not occurd (processed before) */
                throw new InitializationException("Could not find SCI.");
            case -3:
                throw new InitializationException("No existing directory.");
            case 10001:
                throw new InitializationException("Stacksize failed (not enought memory ?).");
            default:
                throw new InitializationException("Unknown startup error: " + res);
        }
    }

    /**
     * Open a connection to the Scilab engine and run the command job<BR>
     * This function is based on Call_ScilabOpen from call_scilab<BR>
     * Note: For now, only one instance of Scilab can be launched<BR>
     * A second launch will return FALSE<BR>
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab();<BR>
     * sci.open("a=%pi;");<BR>
     * <BR>
     * </code>
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
     * Open a connection to the Scilab engine and run commands job<BR>
     * This function is based on Call_ScilabOpen from call_scilab<BR>
     * Note: For now, only one instance of Scilab can be launched<BR>
     * A second launch will return FALSE<BR>
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab();<BR>
     * sci.open(new String[]{"a=42*2;","b=44*2", "c=(a==b)"});<BR>
     * <BR>
     * </code>
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
     * Open a connection to the Scilab engine and run thefile scriptFilename<BR>
     * This function is based on Call_ScilabOpen from call_scilab<BR>
     * Note: For now, only one instance of Scilab can be launched<BR>
     * A second launch will return FALSE<BR>
     * <BR>
     * Example:<BR>
     * <code>
     * Scilab sci = new Scilab();<BR>
     * sci.open(new File("/tmp/myscript.sce"));<BR>
     * <BR>
     * </code>
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
     * Execute a single command in Scilab<BR>
     * This function is based on SendScilabJob from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec("a=2*%pi");<BR>
     * <BR>
     * </code>
     * @param job the job to execute
     * @return if the operation is successful
     */
    public boolean exec(String job) {
        try {
            this.execException(job);
            return true;
        } catch (Exception e) {
            return false;
        }
    }


    /**
     * Execute a single command in Scilab<BR>
     * Returns a ScilabErrorException in case of Scilab problem<BR>
     * This function is based on SendScilabJob from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec("a=2*%pi");<BR>
     * <BR>
     * </code>
     * @param job the job to execute
     * @since 5.4.0
     */
    public void execException(String job) throws ScilabErrorException {
        int result = Call_Scilab.SendScilabJob(job);
        if (result != 0) {
            throw new ScilabErrorException("A Scilab error occurred: " + this.getLastErrorMessage(), this.getLastErrorCode());
        }
    }


    /**
     * Execute several commands in Scilab<BR>
     * This function is based on SendScilabJob from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec(new String[]{"a=42*2;","b=44*2", "c=(a==b)"});<BR>
     * <BR>
     * </code>
     * @param jobs the serie of job to execute
     * @return if the operation is successful
     */
    public boolean exec(String jobs[]) {
        try {
            this.execException(jobs);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    /**
     * Execute several commands in Scilab<BR>
     * Returns a ScilabErrorException in case of Scilab problem<BR>
     * This function is based on SendScilabJob from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec(new String[]{"a=42*2;","b=44*2", "c=(a==b)"});<BR>
     * <BR>
     * </code>
     * @param jobs the serie of job to execute
     * @since 5.4.0
     */
    public void execException(String jobs[]) throws ScilabErrorException {
        int result = Call_Scilab.SendScilabJobs(jobs, jobs.length);
        if (result != 0) {
            throw new ScilabErrorException("A Scilab error occurred: " + this.getLastErrorMessage(), this.getLastErrorCode());
        }
    }

    /**
     * Execute a Scilab script .sce/.sci and throws an exception in case<BR>
     * of a Scilab error<BR>
     * Returns a ScilabErrorException in case of Scilab problem<BR>
     * This function is based on SendScilabJob from call_scilab<BR>
     * Note that this function is a direct call on the Scilab function exec:
     * <code> this.exec("exec('" + scriptFilename + "');");</code>
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec(new File("/tmp/myscript.sci"));<BR>
     * <BR>
     * </code>
     * @param scriptFilename the script to execute
     * @since 5.4.0
     */
    public void execException(File scriptFilename) throws FileNotFoundException, ScilabErrorException {
        if (!scriptFilename.exists()) {
            throw new FileNotFoundException("Could not find " + scriptFilename);
        }
        this.execException("exec('" + scriptFilename + "');");
    }

    /**
     * Execute a Scilab script .sce/.sci and throws an exception in case<BR>
     * the file is not found<BR>
     * This function is based on SendScilabJob from call_scilab<BR>
     * Note that this function is a direct call on the Scilab function exec:
     * <code> this.exec("exec('" + scriptFilename + "');");</code>
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec(new File("/tmp/myscript.sci"));<BR>
     * <BR>
     * </code>
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
     * <BR>
     * Example:<BR>
     * <code>
     * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
     * ScilabDouble aOriginal = new ScilabDouble(a);<BR>
     * sci.put("a",aOriginal);<BR>
     * assert sci.isExistingVariable("a") == true;<BR>
     * <BR>
     * </code>
     * @param varname the variable to check
     * @return if the variable exists or not
     */
    public boolean isExistingVariable(String varname) {
        return Call_Scilab.isExistingVariable(varname);
    }


    /**
     * Shutdown Scilab<BR>
     * This function is based on TerminateScilab from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.close();<BR>
     * <BR>
     * </code>
     * @return if the operation is successful
     */
    public boolean close() {
        return Call_Scilab.TerminateScilab(null);
    }


    /**
     * Return the last error code
     * <BR>
     * Example:<BR>
     * <code>
     * sci.open("a=1+"); // Wrong operation<BR>
     * sci.getLastErrorCode() // Returns 2<BR>
     * <BR>
     * </code>
     * @return the error code
     */
    public int getLastErrorCode() {
        return Call_Scilab.GetLastErrorCode();
    }


    /**
     * Return the last error message
     * <BR>
     * Example:<BR>
     * <code>
     * sci.open("a=1+");<BR>
     * System.err.println(sci.getLastErrorMessage());<BR>
     * <BR>
     * </code>
     * @return the error message itself
     */
    public String getLastErrorMessage() {
        return Call_Scilab.getLastErrorMessage();
    }


    /**
     * Detect if a Scilab graphic window is still opened<BR>
     * This function is based on ScilabHaveAGraph from call_scilab
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec("plot3d();");<BR>
     * sci.isGraphicOpened();<BR>
     * <BR>
     * </code>
     * @return if the graphic is open or not
     */
    public boolean isGraphicOpened() {
        return Call_Scilab.isGraphicOpened();
    }

    /**
     * Return the code type of a variable varname
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec("a = 2*%pi");<BR>
     * if (sci.getVariableType("a") == ScilabTypeEnum.sci_matrix) {<BR>
     *      System.out.println("a is a double matrix");<BR>
     * }<BR>
     * <BR>
     * </code>
     * @param varName the name of the variable
     * @return the type of the variable
     * @throws UndefinedVariableException The variable does not exist
     * @throws UnknownTypeException Cannot find the type
     */
    public ScilabTypeEnum getVariableType(String varName) throws JavasciException {
        return getVariableTypeInCurrentScilabSession(varName);
    }

    /**
     * Return the code type of a variable varname in the current Scilab session
     * <BR>
     * Example:<BR>
     * <code>
     * sci.exec("a = 2*%pi");<BR>
     * if (sci.getVariableType("a") == ScilabTypeEnum.sci_matrix) {<BR>
     *      System.out.println("a is a double matrix");<BR>
     * }<BR>
     * <BR>
     * </code>
     * @param varName the name of the variable
     * @return the type of the variable
     * @throws UndefinedVariableException The variable does not exist
     * @throws UnknownTypeException Cannot find the type
     */
    public static ScilabTypeEnum getVariableTypeInCurrentScilabSession(String varName) throws JavasciException {
        ScilabTypeEnum variableType = null;
        try {
            variableType = Call_Scilab.getVariableType(varName);
            if (variableType == null ) {
                throw new UndefinedVariableException("Could not find the type of the variable '" + varName + "'");
            }
        } catch (IllegalArgumentException e) {
            String lastWord = e.getMessage().substring(e.getMessage().lastIndexOf(' ') + 1);
            if (lastWord.equals("-2")) { /* Crappy workaround. Parse the exception */
                throw new UndefinedVariableException("Could not find variable '" + varName + "'");
            }
            throw new UnknownTypeException("Type of " + varName + " unknown");

        }
        return variableType;
    }

    /**
     * Returns a variable named varname<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
     * double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};<BR>
     * ScilabDouble aOriginal = new ScilabDouble(a, aImg);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @return return the variable
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public ScilabType get(String varname) throws JavasciException {
        return getInCurrentScilabSession(varname);
    }

    /**
     * Returns a reference variable named varname<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
     * double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};<BR>
     * ScilabDouble aOriginal = new ScilabDouble(a, aImg);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @return return the variable
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public ScilabType getByReference(String varname) throws JavasciException {
        return getInCurrentScilabSession(varname, true);
    }

    /**
     * Returns a variable named varname in the current Scilab session<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
     * double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};<BR>
     * ScilabDouble aOriginal = new ScilabDouble(a, aImg);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @return return the variable
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public static ScilabType getInCurrentScilabSession(String varname) throws JavasciException {
        return getInCurrentScilabSession(varname, false);
    }

    /**
     * Returns a variable named varname in the current Scilab session<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * double [][]a={{21.2, 22.0, 42.0, 39.0},{23.2, 24.0, 44.0, 40.0}};<BR>
     * double [][]aImg={{212.2, 221.0, 423.0, 393.0},{234.2, 244.0, 441.0, 407.0}};<BR>
     * ScilabDouble aOriginal = new ScilabDouble(a, aImg);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabDouble aFromScilab = (ScilabDouble)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @return return the variable
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public static ScilabType getInCurrentScilabSession(String varname, boolean byref) throws JavasciException {
        ScilabTypeEnum sciType = getVariableTypeInCurrentScilabSession(varname);
        switch (sciType) {
            case sci_matrix:
            case sci_boolean:
            case sci_strings:

            case sci_poly:
            case sci_sparse:
            case sci_boolean_sparse:
            case sci_list:
            case sci_tlist:
            case sci_mlist:
                return ScilabVariablesJavasci.getScilabVariable(varname, true, byref);
            case sci_ints:
                ScilabIntegerTypeEnum typeInt = Call_Scilab.getIntegerPrecision(varname);

                switch (typeInt) {
                    case sci_int8:
                    case sci_uint8:
                    case sci_int16:
                    case sci_uint16:
                    case sci_int32:
                    case sci_uint32:
                        return ScilabVariablesJavasci.getScilabVariable(varname, true, byref);
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
     * Send to Scilab a variable theVariable named varname<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * boolean [][]a={{true, true, false, false},{true, false, true, false}};<BR>
     * ScilabBoolean aOriginal = new ScilabBoolean(a);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @param theVariable the variable itself
     * @return true if the operation is successful
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public boolean put(String varname, ScilabType theVariable) throws JavasciException {
        return putInCurrentScilabSession(varname, theVariable);
    }

    /**
     * Send to the current Scilab session a variable theVariable named varname<BR>
     * Throws an exception if the datatype is not managed or if the variable is not available
     * <BR>
     * Example:<BR>
     * <code>
     * boolean [][]a={{true, true, false, false},{true, false, true, false}};<BR>
     * ScilabBoolean aOriginal = new ScilabBoolean(a);<BR>
     * sci.put("a",aOriginal);<BR>
     * ScilabBoolean aFromScilab = (ScilabBoolean)sci.get("a");<BR>
     * <BR>
     * </code>
     * @param varname the name of the variable
     * @param theVariable the variable itself
     * @return true if the operation is successful
     * @throws UnsupportedTypeException Type not managed yet.
     */
    public static boolean putInCurrentScilabSession(String varname, ScilabType theVariable) throws JavasciException {
        int err = notHandledError; /* -999: if the type is not handled */

        switch (theVariable.getType()) {
            case sci_matrix :
                ScilabDouble sciDouble = (ScilabDouble) theVariable;
                if (sciDouble.isReal()) {
                    err = Call_Scilab.putDouble(varname, sciDouble.getRealPart());
                } else {
                    err = Call_Scilab.putDoubleComplex(varname, sciDouble.getRealPart(), sciDouble.getImaginaryPart());
                }
                break;
            case sci_poly :
                ScilabPolynomial sciPoly = (ScilabPolynomial) theVariable;
                if (sciPoly.isReal()) {
                    err = Call_Scilab.putPolynomial(varname, sciPoly.getPolyVarName(), sciPoly.getRealPart());
                } else {
                    err = Call_Scilab.putComplexPolynomial(varname, sciPoly.getPolyVarName(), sciPoly.getRealPart(), sciPoly.getImaginaryPart());
                }
                break;
            case sci_boolean :
                ScilabBoolean sciBoolean = (ScilabBoolean) theVariable;
                err = Call_Scilab.putBoolean(varname, sciBoolean.getData());
                break;
            case sci_sparse :
                ScilabSparse sciSparse = (ScilabSparse) theVariable;
                if (sciSparse.isReal()) {
                    err = Call_Scilab.putSparse(varname, sciSparse.getHeight(), sciSparse.getWidth(), sciSparse.getNbItemRow(), sciSparse.getScilabColPos(), sciSparse.getRealPart());
                } else {
                    err = Call_Scilab.putComplexSparse(varname, sciSparse.getHeight(), sciSparse.getWidth(), sciSparse.getNbItemRow(), sciSparse.getScilabColPos(), sciSparse.getRealPart(), sciSparse.getImaginaryPart());
                }
                break;
            case sci_boolean_sparse :
                ScilabBooleanSparse sciBooleanSparse = (ScilabBooleanSparse) theVariable;
                err = Call_Scilab.putBooleanSparse(varname, sciBooleanSparse.getHeight(), sciBooleanSparse.getWidth(), sciBooleanSparse.getNbItemRow(), sciBooleanSparse.getScilabColPos());
                break;
            case sci_ints :
                ScilabInteger sciInteger = (ScilabInteger) theVariable;
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
                        //                    err = Call_Scilab.putUnsignedLong(varname, sciInteger.getData_());
                    case sci_int64:
                        //                    err = Call_Scilab.putLong(varname, sciInteger.getData_());
                        break;
                }
                break;
            case sci_strings :
                ScilabString sciString = (ScilabString) theVariable;
                err = Call_Scilab.putString(varname, sciString.getData());
                break;
            case sci_list :
                ScilabList sciList = (ScilabList) theVariable;
                err = Call_ScilabJNI.putList(varname, sciList.getSerializedObject(), 'l');
                break;
            case sci_tlist :
                ScilabTList sciTList = (ScilabTList) theVariable;
                err = Call_ScilabJNI.putList(varname, sciTList.getSerializedObject(), 't');
                break;
            case sci_mlist :
                ScilabMList sciMList = (ScilabMList) theVariable;
                err = Call_ScilabJNI.putList(varname, sciMList.getSerializedObject(), 'm');
                break;
        }

        if (err == notHandledError) {
            throw new UnsupportedTypeException("Type not managed: " + theVariable.getClass());
        } else {
            if (err != 0) {
                throw new ScilabInternalException("Storage of the variable '" + varname + "' (" + theVariable.getClass() + ") failed.");
            }
        }
        return true;
    }
}
