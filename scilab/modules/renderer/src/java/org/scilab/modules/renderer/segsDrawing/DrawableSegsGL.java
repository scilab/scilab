/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a
 *  segs or champ object with JoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
