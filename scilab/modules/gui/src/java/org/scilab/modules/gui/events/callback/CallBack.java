package org.scilab.modules.gui.events.callback;


import javax.swing.AbstractAction;

/**
 * Abstract class to manage all callbacks.
 * Those that need Java code, and those that need Scilab execution.
 * 
 * @author bruno
 *
 */
public abstract class CallBack extends AbstractAction {
	
	/**
	 * Unmanaged command type constant
	 */
	public static final int UNTYPED = -1;
	/**
	 * Scilab instruction command type constant
	 */
	public static final int SCILAB_INSTRUCTION = 0;
	/**
	 * C or Fortran function type constant
	 */
	public static final int C_FORTRAN = 1;
	/**
	 * Scilab function type constant
	 */
	public static final int SCILAB_FUNCTION = 2;

	/**
	 * Java function type constant
	 */
	public static final int JAVA = 3;	
	
	/**
	 * The Command to Store and remember.
	 */
	protected String command;
	
	/**
	 * Constructor to be seen by specifics CallBack.
	 * @param command the command associated to the ccallback
	 */
	public CallBack(String command) {
		this.command = command;
	}
	
	/**
	 * @return the command if it's a Scilab instruction.
	 */
	public String getCommand() {
		return command;
	}
	
	/**
	 * Callback function that will be call on some event.
	 */
	public abstract void callBack();
		
	/**
	 * Create a Callback from Scilab data
	 * @param command the instruction
	 * @param callbackType the type of the instruction
	 * @return the Callback
	 */
	public static CallBack createCallback(String command, int callbackType) {
		if (callbackType == CallBack.JAVA) {
			return JavaCallBack.create(command);
		} else {
			return ScilabCallBack.create(command);
		}
	}

	/**
	 * Create a Callback from Scilab data
	 * @param command the instruction
	 * @param callbackType the type of the instruction
	 * @param objectIndex the index of the object in the UIElementMapper
	 * @return the Callback
	 */
	public static CallBack createCallback(String command, int callbackType, int objectIndex) {
		if (callbackType == CallBack.JAVA) {
			return JavaCallBack.create(command);
		} else {
			return ScilabCallBack.create("%oldgcbo = []; if exists(\"gcbo\") then %oldgcbo = gcbo; end;"
					+ "gcbo = getcallbackobject(" + objectIndex + ");"
					+ command 
					+ "; gcbo = %oldgcbo;clear gcbo");
		}
	}

}
