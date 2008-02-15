
/* Copyright INRIA 2008 */

package org.scilab.modules.gui.events;

/**
 * Class use for dialogs that wait for a user input
 * 
 * @author Bruno JOFRET
 */
public final class BlockingResult {

	private static BlockingResult me;
	
	private String theResult;
	
	/**
	 * Constructor
	 */
	private BlockingResult() {
		theResult = null;
	}

	/**
	 * Get the current instance of BlockingResult
	 * @return this instance
	 */
	public static BlockingResult getInstance() {
		if (me == null) {
			me = new BlockingResult();	
		}
		return me;
	}
	
	/**
	 * Get the user input (wait until it)
	 * @return the user input
	 */
	public String getResult() {
		synchronized (me) {
			try {
				me.wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return me.theResult;
	}

	/**
	 * Set the result for this BlockingResult and notify
	 * @param theResult the user input to set
	 */
	public void setResult(String theResult) {
		this.theResult = theResult;
		synchronized (me) {
			me.notify();
		}
	}
	
	
	
}
