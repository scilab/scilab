/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw marks    */
/*        on a polyline                                                   */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;


import org.scilab.modules.renderer.drawers.MarkDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw marks
 *        on a polyline
 * @author Jean-Baptiste Silvy
 */
public class PolylineMarkDrawerGL extends MarkDrawerGL implements PolylineDrawerStrategy {

	/** Get the position of vertices */
	private Vector3D[] verticesPositions;
	
	/**
	 * Default constructor
	 */
	public PolylineMarkDrawerGL() {
		super();
		verticesPositions = null;
	}
	
	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	@Override
	public void show(int parentFigureIndex) {
		drawRectangle();
	}

	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		verticesPositions = new Vector3D[xCoords.length];
		for (int i = 0; i < xCoords.length; i++) {
			verticesPositions[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
		}
		
		drawRectangle();
		
	}
	
	/**
	 * Draw the mark on the polyline using the stored vertices.
	 */
	public void drawRectangle() {
		drawMarks(verticesPositions);
	}

}
