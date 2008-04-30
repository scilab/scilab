/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class which can be automatically shown 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer;

/**
 * Class which can be automatically shown.
 * @author Jean-Baptiste Silvy
 */
public class AutoDrawableObjectGL extends DrawableClippedObjectGL {

	
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
		if (getGL().glIsList(getDlIndex())) {
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
	@Override
	public void initializeDrawing(int parentFigureIndex) {
		// get the context from the drawing canvas
		super.initializeDrawing(parentFigureIndex);
		startRecordDL();
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	@Override
	public void endDrawing() {
		endRecordDL();
		super.endDrawing();
	}
	
}
