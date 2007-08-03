/*------------------------------------------------------------------------*/
/* file: DrawableObjectJoGL.java                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class which can be automatically shown                          */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer;

import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class which can be automatically shown.
 * @author Jean-Baptiste Silvy
 */
public class AutoDrawableObjectGL extends DrawableObjectGL {

	
	/**
	 * Default constructor
	 */
	public AutoDrawableObjectGL() {
		super();
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		// the display list should already have been created.
		if (getDlIndex() != GLTools.UNINIT_DL_INDEX) {
			super.initializeDrawing(parentFigureIndex);
			displayDL();
			super.endDrawing();
		}
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		// get the context from the drawing canvas
		super.initializeDrawing(parentFigureIndex);
		startRecordDL();
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		endRecordDL();
		super.endDrawing();
	}
	
}
