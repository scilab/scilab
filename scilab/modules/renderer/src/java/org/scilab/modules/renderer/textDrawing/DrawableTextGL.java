/*------------------------------------------------------------------------*/
/* file: DrawableTextJoGL.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        text object with JoGL                                       */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.textDrawing;

import org.scilab.modules.renderer.DrawableClippedObjectGL;

/**
 * Class containing the driver dependant routines to draw a
 * text object with JoGL.
 * @author Jean-Baptiste Silvy
 */
public class DrawableTextGL extends DrawableClippedObjectGL {

	/**
	 * Default constructor
	 */
	public DrawableTextGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		System.err.println("I'm printing text. " + parentFigureIndex);
	}
	
}
