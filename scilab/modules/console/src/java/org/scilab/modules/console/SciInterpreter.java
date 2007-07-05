
/* Copyright INRIA 2007 */

package org.scilab.modules.console;

import java.io.Reader;
import java.io.Writer;

import javax.script.ScriptException;

import com.artenum.console.interfaces.core.GenericInterpreter;

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
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#eval(java.lang.String)
	 */
	public Object eval(String script) throws ScriptException {
		return InterpreterManagement.putCommandInScilabQueue(script + "\n");
	}

	/**
	 * Calls Scilab interpreter to evaluate a command
	 * @param reader the command to eval
	 * @return the result of evaluation
	 * @throws ScriptException if an error occurs while evaluating th ecommand
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#eval(java.io.Reader)
	 */
	public Object eval(Reader reader) throws ScriptException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets the value of a key
	 * @param key the key to get
	 * @return the value of the key
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#get(java.lang.String)
	 */
	public Object get(String key) {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets the console error writer
	 * @return the error writer
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#getErrorWriter()
	 */
	public Writer getErrorWriter() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets the console reader
	 * @return the reader
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#getReader()
	 */
	public Reader getReader() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Gets the console writer
	 * @return the writer
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#getWriter()
	 */
	public Writer getWriter() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Sets a new key
	 * @param key the name of the key to be set
	 * @param value the value to set for the key
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#put(java.lang.String, java.lang.Object)
	 */
	public void put(String key, Object value) {
		// TODO Auto-generated method stub
	}

	/**
	 * Sets the console error writer
	 * @param writer the error writer to set
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#setErrorWriter(java.io.Writer)
	 */
	public void setErrorWriter(Writer writer) {
		// TODO Auto-generated method stub
	}

	/**
	 * Sets the console reader
	 * @param reader the reader to set
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#setReader(java.io.Reader)
	 */
	public void setReader(Reader reader) {
		// TODO Auto-generated method stub
		
	}

	/**
	 * Sets the console writer
	 * @param writer the writer to set
	 * @see com.artenum.console.interfaces.core.GenericInterpreter#setWriter(java.io.Writer)
	 */
	public void setWriter(Writer writer) {
		// TODO Auto-generated method stub
		
	}

}
