/*------------------------------------------------------------------------*/
/* file: FillDrawerGL.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class for drawing algorithms which fill shapes           */
/*------------------------------------------------------------------------*/


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
	
}
