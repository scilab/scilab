/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerGL.java                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw bar      */
/*        plots                                                           */
/*------------------------------------------------------------------------*/



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
	private double barWidth;
	
	/**
	 * Default constructor
	 */
	public PolylineBarDrawerGL() {
		super();
		backColor = -1;
		lineColor = -1;
		thickness = 1.0f;
		lineStyle = 1;
		barWidth = 1;
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
	 * Set the width of bar
	 * @param barWidth width of bar in user coordinates
	 */
	public void setBarWidth(double barWidth) {
		this.barWidth = barWidth;
	}
	
	/**
	 * @return width of bar in user coordinates
	 */
	public double getBarWidth() {
		return barWidth;
	}
	
	/**
	 * Set all parameters to draw bars in one function
	 * @param background background color index
	 * @param foreground line color index
	 * @param thickness thickness of the outlin ein pixel
	 * @param lineStyle index of the line style
	 * @param barWidth width of bars in pixel coordinates
	 */
	public void setBarParameters(int background, int foreground, float thickness, int lineStyle, double barWidth) {
		setBackColorIndex(background);
		setLineColorIndex(foreground);
		setThickness(thickness);
		setLineStyle(lineStyle);
		setBarWidth(barWidth);
	}
	
	/**
	 * Draw the bar plot. The number of bar to draw is the length of the array.
	 * The length of each array must be the same.
	 * @param topX X coordinate of the middle of the top segment of each bar
	 * @param topY Y coordinate of the middle of the top segment of each bar
	 * @param topZ Z coordinate of the middle of the top segment of each bar
	 * @param height height of each bar
	 */
	public void drawPolyline(double[] topX, double[] topY, double[] topZ, double[] height) {
		
		if (getBarWidth() > 0.0) {
			fillBars(topX, topY, topZ, height);
		}
		encloseBars(topX, topY, topZ, height);
		
	}
	
	/**
	 * Fill the inside of bars
	 * @param topX X coordinate of the middle of the top segment of each bar
	 * @param topY Y coordinate of the middle of the top segment of each bar
	 * @param topZ Z coordinate of the middle of the top segment of each bar
	 * @param height height of each bar
	 */
	private void fillBars(double[] topX, double[] topY, double[] topZ, double[] height) {
		
		GL gl = getGL();
		double semiBarWidth = getBarWidth() / 2.0;


		// set color
		double[] color = getBackColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_QUADS);
		for (int i = 0; i < topX.length; i++) {
			gl.glVertex3d(topX[i] - semiBarWidth, topY[i] - height[i], topZ[i]);
			gl.glVertex3d(topX[i] + semiBarWidth, topY[i] - height[i], topZ[i]);
			gl.glVertex3d(topX[i] + semiBarWidth, topY[i], topZ[i]);
			gl.glVertex3d(topX[i] - semiBarWidth, topY[i], topZ[i]);
		}
		gl.glEnd();
	}
	
	/**
	 * Draw the outline of every bars
	 * @param topX X coordinate of the middle of the top segment of each bar
	 * @param topY Y coordinate of the middle of the top segment of each bar
	 * @param topZ Z coordinate of the middle of the top segment of each bar
	 * @param height height of each bar
	 */
	private void encloseBars(double[] topX, double[] topY, double[] topZ, double[] height) {
		
		GL gl = getGL();
		double semiBarWidth = getBarWidth() / 2.0;


		// set color
		double[] color = getLineColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		// set dash mode
		gl.glLineWidth(getThickness());
		GLTools.beginDashMode(gl, getLineStyle(), getThickness());
				
		for (int i = 0; i < topX.length; i++) {
			gl.glBegin(GL.GL_LINE_LOOP);
			gl.glVertex3d(topX[i] - semiBarWidth, topY[i] - height[i], topZ[i]);
			gl.glVertex3d(topX[i] + semiBarWidth, topY[i] - height[i], topZ[i]);
			gl.glVertex3d(topX[i] + semiBarWidth, topY[i], topZ[i]);
			gl.glVertex3d(topX[i] - semiBarWidth, topY[i], topZ[i]);
			gl.glEnd();
		}
		
		GLTools.endDashMode(gl);
		
	}
	
	
}
