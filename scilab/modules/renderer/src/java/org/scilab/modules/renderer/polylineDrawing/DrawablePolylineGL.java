/*------------------------------------------------------------------------*/
/* file: DrawablePolylineGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object with JoGL                                       */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing functions called by DrawablePolylineJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawablePolylineGL extends DrawableClippedObjectGL {

	/**
	 * Default constructor
	 */
	public DrawablePolylineGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) { }
	
}
