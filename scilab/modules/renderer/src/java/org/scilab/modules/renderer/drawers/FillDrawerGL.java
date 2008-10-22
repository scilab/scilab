/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing algorithms which fill shapes  
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.drawers;

import org.scilab.modules.renderer.AutoDrawableObjectGL;

/**
 * Class containing functions called by RectangleFillDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class FillDrawerGL extends AutoDrawableObjectGL {
	
	/** index of background color */
	private int backColor;
	
	/**
	 * Default Constructor
	 */
	public FillDrawerGL() {
		super();
		backColor = -1;
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
	}
	
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
        super.endDrawing();
	}
	
	
	/**
	 * Set rectangle background color
	 * @param color index of background color
	 */
	public void setBackColor(int color) {
		backColor = color;
	}
	
	/**
	 * @return index of background color
	 */
	public int getBackColorIndex() {
		return backColor;
	}
	
	/**
	 * @return Array of size 3 (RGB) of the background color
	 */
	public double[] getBackColor() {
		return getColorMap().getColor(backColor);
	}
	
	/**
	 * This method is used to know if polygon offset is needed in 2d mode.
	 * If this function returns true, then the polygon offset is not needed in 2d mode.
	 * If it returns false, polygon offset is always needed.
	 * @return true if the object is flat along Z coordinate
	 */
	public boolean isZConstant() {
		return false;
	}
	
}
