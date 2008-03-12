/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw bar plots
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */



package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.AutoDrawableObjectGL;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Class containing the driver dependant routines to draw bar
 *        plots 
 * @author Jean-Baptiste Silvy
 */
public class PolylineBarDrawerGL extends AutoDrawableObjectGL {

	private int backColor;
	private int lineColor;
	private float thickness;
	private int lineStyle;
	
	/**
	 * Default constructor
	 */
	public PolylineBarDrawerGL() {
		super();
		backColor = -1;
		lineColor = -1;
		thickness = 1.0f;
		lineStyle = 1;
	}
	
	/**
	 * Set bar fillling color
	 * @param colorIndex color index in the colormap
	 */
	public void setBackColorIndex(int colorIndex) {
		this.backColor = colorIndex;
	}
	
	/**
	 * @return array of size 3 containing the RGB channels
	 */
	public double[] getBackColor() {
		return getColorMap().getColor(backColor);
	}
	
	/**
	 * Set color of outline
	 * @param colorIndex color index in the colormap
	 */
	public void setLineColorIndex(int colorIndex) {
		this.lineColor = colorIndex;
	}
	
	/**
	 * @return array of size 3 containing the RGB channels
	 */
	public double[] getLineColor() {
		return getColorMap().getColor(lineColor);
	}
	
	/**
	 * Set the thickness of the outline
	 * @param thickness thickness in pixel
	 */
	public void setThickness(float thickness) {
		this.thickness = thickness;
	}
	
	/**
	 * @return thikness in pixel for this line
	 */
	public float getThickness() {
		return thickness;
	}
	
	/**
	 * Set the line style
	 * @param styleIndex line style index
	 */
	public void setLineStyle(int styleIndex) {
		this.lineStyle = styleIndex;
	}
	
	/**
	 * @return line style index
	 */
	public int getLineStyle() {
		return lineStyle;
	}
	
	/**
	 * Set all parameters to draw bars in one function
	 * @param background background color index
	 * @param foreground line color index
	 * @param thickness thickness of the outlin ein pixel
	 * @param lineStyle index of the line style
	 */
	public void setBarParameters(int background, int foreground, float thickness, int lineStyle) {
		setBackColorIndex(background);
		setLineColorIndex(foreground);
		setThickness(thickness);
		setLineStyle(lineStyle);
	}
	
	/**
	 * Draw the bar plot. The number of bar to draw is the length of the array.
	 * The length of each array must be the same.
	 * @param left abscissa of left edge of each bar
	 * @param right abscissa of right edge of each bar
	 * @param bottom ordinate of bottom edge of each bar
	 * @param top ordinate of top edge of each bar
	 * @param zCoord z coordinate of each bar
	 */
	public void drawPolyline(double[] left, double[] right,
							 double[] bottom, double[] top,
							 double[] zCoord) {
		
		fillBars(left, right, bottom, top, zCoord);
		encloseBars(left, right, bottom, top, zCoord);
		
	}
	
	/**
	 * Fill the inside of bars
	 * @param left abscissa of left edge of each bar
	 * @param right abscissa of right edge of each bar
	 * @param bottom ordinate of bottom edge of each bar
	 * @param top ordinate of top edge of each bar
	 * @param zCoord z coordinate of each bar
	 */
	private void fillBars(double[] left, double[] right,
			 			  double[] bottom, double[] top,
			 			  double[] zCoord) {
		
		GL gl = getGL();


		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_QUADS);
		for (int i = 0; i < left.length; i++) {
			gl.glVertex3d(left[i], bottom[i], zCoord[i]);
			gl.glVertex3d(right[i], bottom[i], zCoord[i]);
			gl.glVertex3d(right[i], top[i], zCoord[i]);
			gl.glVertex3d(left[i], top[i], zCoord[i]);
		}
		gl.glEnd();
	}
	
	/**
	 * Draw the outline of every bars
	 * @param left abscissa of left edge of each bar
	 * @param right abscissa of right edge of each bar
	 * @param bottom ordinate of bottom edge of each bar
	 * @param top ordinate of top edge of each bar
	 * @param zCoord z coordinate of each bar
	 */
	private void encloseBars(double[] left, double[] right,
			  double[] bottom, double[] top,
 			  double[] zCoord) {
		
		GL gl = getGL();


		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		// set dash mode
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());

		// push a little bars to the background
		GLTools.pushPolygonsBack(gl);
		
		for (int i = 0; i < left.length; i++) {
			gl.glBegin(GL.GL_LINE_LOOP);
			gl.glVertex3d(left[i], bottom[i], zCoord[i]);
			gl.glVertex3d(right[i], bottom[i], zCoord[i]);
			gl.glVertex3d(right[i], top[i], zCoord[i]);
			gl.glVertex3d(left[i], top[i], zCoord[i]);
			gl.glEnd();
		}
		
		GLTools.endPushPolygonsBack(gl);
		
		GLTools.endDashMode(gl);
		
	}
	
	
}
