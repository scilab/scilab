package org.scilab.modules.jvm;

import org.scilab.modules.console.ScilabConsole;

/**
 * Main Class for Scilab
 * @author Allan CORNET INRIA 2007
  */
public class Scilab {
	
	private ScilabConsole console;
	
	private int mode;
	/**
	 * Constructor Scilab Class.
	 * @param mode Mode Scilab -NW -NWNI -STD -API
	 */
	public Scilab(int mode) {
		this.mode = mode;
		console = new ScilabConsole();
        console.printf("First Step\n");
	}
}
/*--------------------------------------------------------------------------*/

