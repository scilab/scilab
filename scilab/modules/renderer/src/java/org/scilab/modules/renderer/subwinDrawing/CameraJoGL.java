/*------------------------------------------------------------------------*/
/* file: CameraJoGL.java                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines position the     */
/*        camera                                                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.subwinDrawing;

import org.scilab.modules.renderer.DrawableObjectJoGL;

/**
 * Class containing the driver dependant routines position the camera.
 * @author Jean-Baptiste Silvy
 */
public class CameraJoGL extends DrawableObjectJoGL {

	/**
	 * Default constructor
	 */
	public CameraJoGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		super.endDrawing();
		
	}
	
	/**
	 * Position the viewing area in order to respect Subwin position.
	 */
	public void moveViewingArea() {
		System.err.println("Moving  viewing area");
	}
	
	
}
