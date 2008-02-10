/*------------------------------------------------------------------------*/
/* file: DrawableAxesGL.java                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw an       */
/*        axes object with JoGL                                           */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.axesDrawing;


import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing functions called by DrawableSubwinJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableAxesGL extends DrawableClippedObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableAxesGL() {
		super();
	}

	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		
	}

	
}
