package org.scilab.modules.shell;
/**
 * Scilab Shell
 * @author Allan CORNET INRIA 2007
  */
public class ScilabShell implements Shell {
	/**
	 * Constructor ScilabShell Class.
	 */
	public ScilabShell() {
	}
	
	/**
	 * Displays a line in the Shell
	 * @param lineToPrint the line to be printed
	 */
	public void printf(String lineToPrint) {
		ScilabShellBridge.printf(lineToPrint);
	}

	/**
	 * Clear the shell
	 */
	public void clear() {
		ScilabShellBridge.clear();
	}

	/**
	 * Get the number of lines displayed
	 * @return the number of lines displayed
	 */
	public int lines() {
		return ScilabShellBridge.lines();
	}

	/**
	 * Reads a line in the Shell
	 * @return the line read
	 */
	public String readLine() {
		return ScilabShellBridge.readLine();
	}

	/**
	 * Put the cursor to the top-left position
	 */
	public void tohome() {
		ScilabShellBridge.tohome();
	}
}
/*--------------------------------------------------------------------------*/

