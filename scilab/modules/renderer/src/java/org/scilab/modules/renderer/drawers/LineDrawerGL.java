/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for Line drawing algorithms 
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
 * Parent class for Line drawing algorithms
 * @author Jean-Baptiste Silvy
 */
public abstract class LineDrawerGL extends AutoDrawableObjectGL {

	private int   lineColor;
	private float thickness;
	private int   lineStyle;
	
	/**
	 * Default constructor
	 */
	public LineDrawerGL() {
		super();
		lineColor = 1;
		thickness = 1.0f;
		lineStyle = 1;
	}
	
	/**
	 * Set line Color
	 * @param lineColor index of the line color in the colormap
	 */
	public void setLineColor(int lineColor) {
		this.lineColor = lineColor;
	}
	
	/**
	 * @return color index of the line
	 */
	public int getLineColorIndex() {
		return lineColor;
	}
	
	/**
	 * @return Array of size 3 with the RGB color.
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Set the thickness
	 * @param thickness thickness of the line in pixels
	 */
	public void setThickness(float thickness) {
		this.thickness = thickness;
	}
	
	/**
	 * @return thickness of the line in pixels
	 */
	public float getThickness() {
		return thickness;
	}
	
	/**
	 * Set the line style
	 * @param lineStyle index of the line Style
	 */
	public void setLineStyle(int lineStyle) {
		this.lineStyle = lineStyle;
	}
	
	/**
	 * @return index of the line Style
	 */
	public int getLineStyle() {
		return lineStyle;
	}
	
	/**
	 * Set all line parameters at once, to avoid multiple Jni calls
	 * @param lineColor index of the line color in the colormap
	 * @param thickness thickness of the line in pixels
	 * @param lineStyle index of the line Style
	 */
	public void setLineParameters(int lineColor, float thickness, int lineStyle) {
		setLineColor(lineColor);
		setThickness(thickness);
		setLineStyle(lineStyle);
	}
	
}
