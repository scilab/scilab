/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceGL.java                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        surface object with JoGL                                        */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.surfaceDrawing;

import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing functions called by DrawablePolylineJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableSurfaceGL extends DrawableClippedObjectGL {

	/**
	 * Default constructor
	 */
	public DrawableSurfaceGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) { }
	
}
