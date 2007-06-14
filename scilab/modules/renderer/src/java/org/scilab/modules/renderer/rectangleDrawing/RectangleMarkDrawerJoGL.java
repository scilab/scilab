/*------------------------------------------------------------------------*/
/* file: RectangleFillDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw marks    */
/*        on the corners of a rectangle                                   */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.rectangleDrawing;

import org.scilab.modules.renderer.DrawableObjectJoGL;
import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.MarkDrawing.MarkDrawer;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.GLTools;

/**
 * Class containing functions called by RectangleMarkDrawerJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class RectangleMarkDrawerJoGL extends DrawableObjectJoGL {
	
	private static final int NB_CORNERS = 4;
	private static final int NB_COORDS  = 3;
	
	
	/** index of background color */
	private MarkDrawer drawer;
	
	/** position of corners, needed to retrive pixels coordinates */
	private double[][] cornersPos;
	
	/**
	 * Default Constructor
	 */
	public RectangleMarkDrawerJoGL() {
		drawer = null;
		cornersPos = new double[NB_CORNERS][NB_COORDS];
	}
	
	/**
	 * Function called before beginning to use OpenGL methods.
	 * @param parentFigureIndex index of the parent figure.
	 *                          Needed to get the GL context to draw in.
	 */
	public void initializeDrawing(int parentFigureIndex) {
		super.initializeDrawing(parentFigureIndex);
		drawer = new MarkDrawer();
		drawer.initializeDrawing(parentFigureIndex);
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		drawer.endDrawing();
		super.endDrawing();
	}
	
	/**
	 * Set rectangle background color
	 * @param color index of background color
	 */
	public void setBackground(int color) {
		drawer.setBackground(color);
	}
	
	/**
	 * Set rectangle foreground color
	 * @param color index of foreground color
	 */
	public void setForeground(int color) {
		drawer.setForeground(color);
	}
	
	/**
	 * Set the marksize unit to use, ie poitn or tabulated
	 * @param markSizeUnit index of the kind of mark
	 */
	public void setMarkSizeUnit(int markSizeUnit) {
		if (markSizeUnit == 2) {
			drawer.setTabulatedUnit();
		} else {
			drawer.setPointUnit();
		}
	}
	
	/**
	 * Specify a new size for marks
	 * @param markSize new size
	 */
	public void setMarkSize(int markSize) {
		drawer.setMarkSize(markSize);
	}
	
	
	/**
	 * Specify the kind of mark to draw
	 * @param markStyleIndex index of the markStyle
	 */
	public void setMarkStyle(int markStyleIndex) {
		drawer.setMarkStyle(markStyleIndex);
	}
	
	
	/**
	 * A single mothod to set every parameters
	 * @param background index of background color
	 * @param foreground index of foreground color
	 * @param markSizeUnit index of markSize unit
	 * @param markSize markSize in pixels or tabulated
	 * @param markStyleIndex index of mark style
	 */
	public void setMarkParameters(int background, int foreground, int markSizeUnit, int markSize, int markStyleIndex) {
		setBackground(background);
		setForeground(foreground);
		setMarkSizeUnit(markSizeUnit);
		setMarkSize(markSize);
		setMarkStyle(markStyleIndex);
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		drawRectangle();
	}
	
	/**
	 * Draw the marks on the corners of a rectangle
	 * using data stored in cornersPos
	 */
	public void drawRectangle() {
		GL gl = getGL();
		CoordinateTransformation transform = CoordinateTransformation.getTransformation();
		
		// need to perform this befaore swithching to pixel coordinates
		double[][] pixCoords = transform.getCanvasCoordinates(gl, cornersPos);
		
		// find min and max Z value
		double minZ = pixCoords[0][2];
		double maxZ = pixCoords[0][2];
		for (int i = 1; i < NB_CORNERS; i++) {
			double curValue = pixCoords[i][2];
			if (curValue < minZ) {
				minZ = curValue;
			} else if (curValue > maxZ) {
				maxZ = curValue;
			}
		}
		
		if (minZ == maxZ) {
			minZ -= 100.0;
			maxZ += 100.0;
		}
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, minZ, maxZ);
		
		for (int i = 0; i < NB_CORNERS; i++) {
			drawer.drawMark(pixCoords[i][0], pixCoords[i][1], pixCoords[i][2]);
		}
		
		GLTools.endPixelCoordinates(gl);
	}
	
	/**
	 * create the display list for the rectangle
	 * @param corner1X first corner X coordinate
	 * @param corner1Y first corner Y coordinate
	 * @param corner1Z first corner Z coordinate
	 * @param corner2X second corner X coordinate
	 * @param corner2Y second corner Y coordinate
	 * @param corner2Z second corner Z coordinate
	 * @param corner3X third corner X coordinate
	 * @param corner3Y third corner Y coordinate
	 * @param corner3Z third corner Z coordinate
	 * @param corner4X last corner X coordinate
	 * @param corner4Y last corner Y coordinate
	 * @param corner4Z last corner Z coordinate
	 */
	public void drawRectangle(double corner1X, double corner1Y, double corner1Z,
							  double corner2X, double corner2Y, double corner2Z,
							  double corner3X, double corner3Y, double corner3Z,
							  double corner4X, double corner4Y, double corner4Z) {

		
		
		// save rectangle coordinates
		cornersPos[0][0] = corner1X;
		cornersPos[0][1] = corner1Y;
		cornersPos[0][2] = corner1Z;
		cornersPos[1][0] = corner2X;
		cornersPos[1][1] = corner2Y;
		cornersPos[1][2] = corner2Z;
		cornersPos[2][0] = corner3X;
		cornersPos[2][1] = corner3Y;
		cornersPos[2][2] = corner3Z;
		cornersPos[NB_CORNERS - 1][0] = corner4X;
		cornersPos[NB_CORNERS - 1][1] = corner4Y;
		cornersPos[NB_CORNERS - 1][2] = corner4Z;
		
		drawRectangle();
		
	}
	
	/**
	 * To be called when the cpp object is destroyed
	 */
	public void destroy() {
		super.destroy();
		drawer.destroy();
	}
}
