/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizer.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class used to manage java threads from C++ code                 */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.utils.graphicSynchronization;

/**
 * Class used to manage java threads from C++ code
 * @author Jean-Baptiste Silvy
 */
public class GraphicSynchronizerJava {

	/**
	 * Default constructor
	 */
	public GraphicSynchronizerJava() {
		super();
	}
	
	/**
	 * @return Id of current thread
	 */
	public int getCurrentThreadId() {
		return (int) Thread.currentThread().getId();
	}
	
}
