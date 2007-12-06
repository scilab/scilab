package org.scilab.modules.gui.events.callback;


import java.awt.event.ActionListener;

/**
 * Abstract class to manage all callbacks.
 * Those that need Java code, and those that need Scilab execution.
 * 
 * @author bruno
 *
 */
public abstract class CallBack implements ActionListener {
	
	/**
	 * Unmanaged command type constant for Scilab storecommand
	 */
	public static final int UNTYPED = -1;
	/**
	 * Scilab instruction command type constant for Scilab storecommand
	 */
	public static final int SCILAB_INSTRUCTION = 0;
	/**
	 * C or Fortran function type constant for Scilab storecommand
	 */
	public static final int C_FORTRAN = 1;
	/**
	 * Scilab function type constant for Scilab storecommand
	 */
	public static final int SCILAB_FUNCTION = 2;
	
	/**
	 * Callback function that will be call on some event.
	 */
	public abstract void callBack();
		
}
