/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
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

package org.scilab.modules.console;

import java.io.Reader;
import java.io.Writer;

import com.artenum.rosetta.exception.ScriptException;

import org.scilab.modules.action_binding.InterpreterManagement;

import com.artenum.rosetta.interfaces.core.GenericInterpreter;

/**
 * Scilab interpreter associated to the Scilab console
 * @author Vincent COUVERT
 */
public class SciInterpreter implements GenericInterpreter {

    /**
     * Constructor
     */
    public SciInterpreter() {
    }

    /**
     * Calls Scilab interpreter to evaluate a command
     * @param script the command to eval
     * @return the result of evaluation
     * @throws ScriptException if an error occurs while evaluating th ecommand
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#eval(java.lang.String)
     */
    public Object eval(String script) throws ScriptException {
        return InterpreterManagement.putCommandInScilabQueue(script + "\n");
    }

    /**
     * Calls Scilab interpreter to evaluate a command
     * @param reader the command to eval
     * @return the result of evaluation
     * @throws ScriptException if an error occurs while evaluating th ecommand
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#eval(java.io.Reader)
     */
    public Object eval(Reader reader) throws ScriptException {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Gets the value of a key
     * @param key the key to get
     * @return the value of the key
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#get(java.lang.String)
     */
    public Object get(String key) {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Gets the console error writer
     * @return the error writer
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#getErrorWriter()
     */
    public Writer getErrorWriter() {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Gets the console reader
     * @return the reader
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#getReader()
     */
    public Reader getReader() {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Gets the console writer
     * @return the writer
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#getWriter()
     */
    public Writer getWriter() {
        // TODO Auto-generated method stub
        return null;
    }

    /**
     * Sets a new key
     * @param key the name of the key to be set
     * @param value the value to set for the key
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#put(java.lang.String, java.lang.Object)
     */
    public void put(String key, Object value) {
        // TODO Auto-generated method stub
    }

    /**
     * Sets the console error writer
     * @param writer the error writer to set
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#setErrorWriter(java.io.Writer)
     */
    public void setErrorWriter(Writer writer) {
        // TODO Auto-generated method stub
    }

    /**
     * Sets the console reader
     * @param reader the reader to set
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#setReader(java.io.Reader)
     */
    public void setReader(Reader reader) {
        // TODO Auto-generated method stub

    }

    /**
     * Sets the console writer
     * @param writer the writer to set
     * @see com.artenum.rosetta.interfaces.core.GenericInterpreter#setWriter(java.io.Writer)
     */
    public void setWriter(Writer writer) {
        // TODO Auto-generated method stub

    }

}
