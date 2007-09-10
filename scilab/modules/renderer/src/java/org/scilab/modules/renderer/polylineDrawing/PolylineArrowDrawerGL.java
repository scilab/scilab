/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw arrow    */
/*        heads on polylines                                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.drawers.ArrowHeadDrawerGL;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class containing the driver dependant routines to draw arrow
 * heads on polylines.    
 * @author Jean-Baptiste Silvy
 */
public class PolylineArrowDrawerGL extends ArrowHeadDrawerGL implements PolylineDrawerStrategy {

	/** first point of the segments */
	private Vector3D[] startPoints;
	/** end points of the segments */
	private Vector3D[] endPoints;
	
	/**
	 * Default contructor
	 */
	public PolylineArrowDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		
		startPoints = new Vector3D[xCoords.length - 1];
		endPoints = new Vector3D[xCoords.length - 1];
		
		// store the two endpoints of each arrow
		for (int i = 0; i < xCoords.length - 1; i++) {
			startPoints[i] = new Vector3D(xCoords[i], yCoords[i], zCoords[i]);
			endPoints[i] = new Vector3D(xCoords[i + 1], yCoords[i + 1], zCoords[i + 1]);
		}
		
		drawPolyline();
	}

	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		drawPolyline();
	}
	
	/**
	 * Draw the stored arrows coordinates.
	 */
	public void drawPolyline() {
		drawArrowHeads(startPoints, endPoints);
	}
	
	

}
