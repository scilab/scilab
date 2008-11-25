/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.utils.graphicEvents;

/**
 * Describe a graphic event 
 * @author Jean-Baptiste Silvy
 */
public abstract class GraphicEvent implements Runnable {

	private boolean hasFinish;
	
	/**
	 * default constructor
	 */
	public GraphicEvent() {
		runStarts();
	}
	
	/**
	 * Cancel the event
	 */
	public abstract void cancel();
	
	/**
	 * Function containing the event code
	 */
	public abstract void performEvent();
	
	/**
	 * Called when the event is launched
	 */
	public void run() {
		performEvent();
		runEnds();
	}
	
	/**
	 * @return true if the event is still running, false if it has ended.
	 */
	public synchronized boolean isRunning() {
		return !hasFinish;
	}
	
	/**
	 * Specify that the envent is now finished
	 */
	public synchronized void runEnds() {
		hasFinish = true;
	}
	
	/**
	 * Specify that the run starts
	 */
	public synchronized void runStarts() {
		hasFinish = false;
	}
	
}
