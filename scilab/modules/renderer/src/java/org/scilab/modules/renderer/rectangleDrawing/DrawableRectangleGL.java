/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.rectangleDrawing;

import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing functions called by DrawableRectangleJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableRectangleGL extends DrawableClippedObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableRectangleGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) { }
	
}
