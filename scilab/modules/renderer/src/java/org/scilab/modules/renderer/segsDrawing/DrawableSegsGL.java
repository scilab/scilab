/*------------------------------------------------------------------------*/
/* file: DrawablSegsGL.java                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        segs or champ object with JoGL                                  */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.segsDrawing;

import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing functions called by DrawableSegsJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableSegsGL extends DrawableClippedObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableSegsGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) { }
	
}
