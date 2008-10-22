/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Parent class for drawing arrow heads (ie triangles)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.drawers;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * Parent class for drawing arrow heads (ie triangles)
 * @author Jean-Baptiste Silvy
 */
public abstract class ArrowHeadDrawerGL extends DrawableObjectGL {
	
	private static final double TRIANGLE_ANGLE = 70.0;
	private static final double COS70 = Math.cos(Math.toRadians(TRIANGLE_ANGLE));
	private static final double SIN70 = Math.sin(Math.toRadians(TRIANGLE_ANGLE));
	
	/** Size of a arrow head compared to the axis size if specified size is 1 */
	private static final double REDUCTION_RATIO = 0.02;
	
	/** Size of a arrow head compared to the axis size if specified size is 1 for segs */
	private static final double SEGS_REDUCTION_RATIO = 0.075;
	
	/** Size of arrow head compared to arrow length if specified size is - */
	private static final double REDUCTION_RATIO_DEPENDING = 0.15;
	
	private int defaultColorIndex;
	
	private double defaultArrowSize;
	private double axesPixelWidth;
	private double axesPixelHeight;
	private double arrowPixelSize;
	
	private Vector3D[] arrowStarts;
	private Vector3D[] arrowEnds;
	
	private double xRange;
	private double yRange;
	
	/** User might specify a diferent color for each arrow head */
	private int[] colors;
	
	/** To know if were drawing a segs object */
	private boolean isSegs;
	
	/**
	 * Default constructor
	 */
	public ArrowHeadDrawerGL() {
		super();
		
		defaultColorIndex = 1;
		defaultArrowSize = 1.0;
		axesPixelWidth = 1.0;
		axesPixelHeight = 1.0;
		arrowPixelSize = 1.0;
		colors = null;
		isSegs = false;
	}
	
	/**
	 * Use the display list of polyline
	 * @param parentFigureIndex index of parent figure
	 */
	public void show(int parentFigureIndex) {
		GL gl = getGL();
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		displayDL();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Set the default color for drawing arrow heads.
	 * @param colorIndex index in the colormap
	 */
	public void setArrowColor(int colorIndex) {
		defaultColorIndex = colorIndex;
	}
	
	/**
	 * Set different colors for each arrow head
	 * @param colors array containing the color index to use for each head
	 */
	public void setColors(int[] colors) {
		this.colors = colors;
	}
	
	/**
	 * @return default size of the arrows
	 */
	public double getDefaultArrowSize() {
		return this.defaultArrowSize;
	}
	
	/**
	 * @return color index of the arrow head
	 */
	public int getArrowColorIndex() {
		return defaultColorIndex;
	}
	
	/**
	 * @return Array of size 3 with the RGB color.
	 */
	public double[] getArrowColor() {
		return getColorMap().getColor(defaultColorIndex);
	}
	
	/**
	 * Set the default size for arrow heads
	 * @param size size of the arrow for default resolution.
	 */
	public void setArrowSize(double size) {
		defaultArrowSize = size;
	}
	
	/**
	 * @param isSegs if true the object is a segs one
	 * TODO : create a specific class for segs drawing instead of this boolean
	 */
	public void setIsSegs(boolean isSegs) {
		this.isSegs = isSegs;
	}
	
	/**
	 * Set all the properties of the arrow head
	 * @param colorIndex index in the colormap
	 * @param size size of the arrow for default resolution.
	 */
	public void setArrowParameters(int colorIndex, double size) {
		setArrowColor(colorIndex);
		setArrowSize(size);
	}
	
	/**
	 * Specify the data bounds of current subwin so its dimension in pixels can be retrieved
	 * @param xMin minimun bound along X axis
	 * @param xMax maximum bound along X axis
	 * @param yMin minimun bound along Y axis
	 * @param yMax maximum bound along Y axis
	 * @param zMin minimun bound along Z axis
	 * @param zMax maximum bound along Z axis
	 */
	public void setAxesBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax) {
		
		GL gl = getGL();
		
		// compute the bounding box on the screen of the current subwin
		Vector3D[] axesCorners = {new Vector3D(xMin, yMin, zMin),
								  new Vector3D(xMin, yMin, zMax),
								  new Vector3D(xMin, yMax, zMin),
								  new Vector3D(xMin, yMax, zMax),
								  new Vector3D(xMax, yMin, zMin),
								  new Vector3D(xMax, yMin, zMax),
								  new Vector3D(xMax, yMax, zMin),
								  new Vector3D(xMax, yMax, zMax)};
		
		// transform axesCorners in pixels
		CoordinateTransformation transform = getCoordinateTransformation();
		axesCorners = transform.getCanvasCoordinates(gl, axesCorners);
		
		// Extrems corner along each axis
		// we don't care about Z coordinate since we use projection
		double minX = axesCorners[0].getX();
		double maxX = axesCorners[0].getX();
		double minY = axesCorners[0].getY();
		double maxY = axesCorners[0].getY();
		
		for (int i = 1; i < axesCorners.length; i++) {
			if (axesCorners[i].getX() < minX) {
				minX = axesCorners[i].getX();
			}
			if (axesCorners[i].getX() > maxX) {
				maxX = axesCorners[i].getX();
			}
			if (axesCorners[i].getY() < minY) {
				minY = axesCorners[i].getY();
			}
			if (axesCorners[i].getY() > maxY) {
				maxY = axesCorners[i].getY();
			}
		}
		
		axesPixelWidth  = maxX - minX;
		axesPixelHeight = maxY - minY;
		
		xRange = xMax - xMin;
		yRange = yMax - yMin;
	}
	
	
	/**
	 * Set a new size for the arrow head size.
	 * @param arrowSize new size of the arrow in pixel
	 */
	protected void setArrowPixelSize(double arrowSize) {
		this.arrowPixelSize = arrowSize;
	}
	
	/**
	 * Get the size of the arrow in pixel
	 * @return size of the arrow head in pixels
	 */
	protected double getArrowPixelSize() {
		return this.arrowPixelSize;
	}
	
	/**
	 * Update the value of arrowPixelSize
	 * @param startPixCoords pixel coordinates of segment starts
	 * @param endPixCoords pixel coordinates of segment ends
	 */
	public void updateArrowPixelSize(Vector3D[] startPixCoords, Vector3D[] endPixCoords) {
		// we use the min between width and height to avoid to large arrows when one of the dimension is very small.
		
		if (defaultArrowSize < 0) {
			arrowPixelSize = computeLengthDependingArrowSize(startPixCoords, endPixCoords);
		} else if (isSegs) {
			// special case for segs, arrow head depends on axes size
			arrowPixelSize =  defaultArrowSize * Math.min(axesPixelWidth, axesPixelHeight)
			                / Math.max(xRange, yRange) * SEGS_REDUCTION_RATIO;
		} else {
			arrowPixelSize = defaultArrowSize * Math.min(axesPixelWidth, axesPixelHeight) * REDUCTION_RATIO;
		}
	}
	
	/**
	 * For negative values, size of the arrow depends on the length of segements
	 * @param startPixCoords pixel coordinates of segment starts
	 * @param endPixCoords pixel coordinates of segment ends
	 * @return actual arrow size to use
	 */
	public double computeLengthDependingArrowSize(Vector3D[] startPixCoords, Vector3D[] endPixCoords) {
		int nbSegs = arrowStarts.length;
		
		// compute the mean of arrow sizes in pixels
		double meanArrowPixelLength = 0.0;
		for (int i = 0; i < nbSegs; i++) {
			// get length in pixel of the segment
			double dx = endPixCoords[i].getX() - startPixCoords[i].getX();
			double dy = endPixCoords[i].getY() - startPixCoords[i].getY(); 
			meanArrowPixelLength += Math.sqrt(dx * dx + dy * dy);
		}
		meanArrowPixelLength /= nbSegs;
		
		return -defaultArrowSize * meanArrowPixelLength * REDUCTION_RATIO_DEPENDING;
	}
	
	/**
	 * Draw all the arrows head of segments specified by position with default color and size.
	 * @param startPoints Array containing the first point of each segment.
	 * @param endPoints Arraty containing the last point of each segment.
	 */
	public void drawArrowHeads(Vector3D[] startPoints, Vector3D[] endPoints) {
		this.arrowStarts = startPoints;
		this.arrowEnds = endPoints;
		redrawArrowHeads();
	}
	
	/**
	 * Draw arrow heads using precomputed data
	 */
	public void redrawArrowHeads() {
		GL gl = getGL();
		
		CoordinateTransformation transform = getCoordinateTransformation();
		
		// need to perform this befaore swithching to pixel coordinates
		Vector3D[] startPixCoords = transform.getCanvasCoordinates(gl, arrowStarts);
		Vector3D[] endPixCoords = transform.getCanvasCoordinates(gl, arrowEnds);
		
		updateArrowPixelSize(startPixCoords, endPixCoords);
		
		// switch to pixel coordinates
		GLTools.usePixelCoordinates(gl, getParentFigureGL());
		
		// begin to record display list
		startRecordDL();
		
		// set color
		double[] color = getArrowColor();
		gl.glColor3d(color[0], color[1], color[2]);
		
		gl.glBegin(GL.GL_TRIANGLES);
		// draw equilaterals triangles at the end of segments.
		for (int i = 0; i < startPixCoords.length; i++) {
			
			// compute the position of the three vertices of the triangle
			Vector3D segmentDir = endPixCoords[i].substract(startPixCoords[i]);
			
			// check that there is a segment
			if (segmentDir.getNorm() == 0.0) { continue; }
			
			if (colors != null) {
				// specify a different color for each head
				double[] curColor = getColorMap().getColor(colors[i]);
				gl.glColor3d(curColor[0], curColor[1], curColor[2]);
			}
			
			// set the length of the arrow
			segmentDir.normalize();
			segmentDir.scalarMultSelf(arrowPixelSize);
			
			Vector3D orthoDir = new Vector3D(-segmentDir.getY(), segmentDir.getX(), 0.0);
			
			// secondPoint = end - SIN70.dir + COS70.ortho
			// thirdPoint = end - SIN70.dir - COS70.ortho
			orthoDir.scalarMultSelf(COS70);
			
			Vector3D projOnDir = endPixCoords[i].substract(segmentDir.scalarMult(SIN70));
			Vector3D secondPoint = projOnDir.add(orthoDir);
			Vector3D thirdPoint = projOnDir.substract(orthoDir);
			
			
			
			// switch back to the new frame
			drawTriangle(gl, endPixCoords[i], secondPoint, thirdPoint);
		}
		gl.glEnd();
		
		endRecordDL();
		
		GLTools.endPixelCoordinates(gl, getParentFigureGL());
	}
	
	/**
	 * Draw a triangle with OpenGL
	 * @param gl current GL pipeline
	 * @param firstPoint first point of the triangle
	 * @param secondPoint second point of the triangle
	 * @param thirdPoint third point of the triangle
	 */
	public void drawTriangle(GL gl, Vector3D firstPoint, Vector3D secondPoint, Vector3D thirdPoint) {
		gl.glVertex3d(firstPoint.getX(), firstPoint.getY(), firstPoint.getZ());
		gl.glVertex3d(secondPoint.getX(), secondPoint.getY(), secondPoint.getZ());
		gl.glVertex3d(thirdPoint.getX(), thirdPoint.getY(), thirdPoint.getZ());
	}
	

}
