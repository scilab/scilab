/*------------------------------------------------------------------------*/
/* file: ArrowHeadDrawerGL.java                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Parent class for drawing arrow heads (ie triangles)             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.drawers;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Parent class for drawing arrow heads (ie triangles)
 * @author Jean-Baptiste Silvy
 */
public abstract class ArrowHeadDrawerGL extends DrawableObjectGL {
	
	private static final int AXES_NB_CORNERS = 8;
	
	private int defaultColorIndex;
	
	private double defaultArrowSize;
	
	private double axesPixelWidth;
	private double axesPixelHeight;
	
	/**
	 * Default constructor
	 */
	public ArrowHeadDrawerGL() {
		super();
	}
	
	/**
	 * Set the default color for drawing arrow heads.
	 * @param colorIndex index in the colormap
	 */
	public void setArrowColor(int colorIndex) {
		defaultColorIndex = colorIndex;
	}
	
	/**
	 * Set the default size for arrow heads
	 * @param size size of the arrow for default resolution.
	 */
	public void setArrowSize(double size) {
		defaultArrowSize = size;
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
		CoordinateTransformation transform = CoordinateTransformation.getTransformation();
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
		
		axesPixelWidth  = maxX  - minX;
		axesPixelHeight = maxY - minY;
		
	}
	
	

}
