/*------------------------------------------------------------------------*/
/* file: DrawableSubwinGL.java                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object with JoGL                                         */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.subwinDrawing;


import org.scilab.modules.renderer.DrawableObjectGL;

/**
 * Class containing functions called by DrawableSubwinJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableSubwinGL extends DrawableObjectGL {

	/**
	 * Default Constructor
	 */
	public DrawableSubwinGL() {
		super();
	}

	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		
	}

	
}
