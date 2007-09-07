/*------------------------------------------------------------------------*/
/* file: RectangleMarkDrawerJoGL.java                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw arrow    */
/*        heads on polylines                                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.drawers.ArrowHeadDrawerGL;

/**
 * Class containing the driver dependant routines to draw arrow
 * heads on polylines.    
 * @author Jean-Baptiste Silvy
 */
public class PolylineArrowDrawerGL extends ArrowHeadDrawerGL implements PolylineDrawerStrategy {

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
		System.err.println("Draw arrows");
	}

	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		
	}
	
	

}
