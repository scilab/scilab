/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerGL.java                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to fill the area */
/*        enclosed by the polyline                                        */
/*------------------------------------------------------------------------*/

package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.drawers.FillDrawerGL;


/**
 * Class containing the driver dependant routines to fill the area
 *        enclosed by the polyline 
 * @author Jean-Baptiste Silvy
 */
public class PolylineFillDrawerGL extends FillDrawerGL implements PolylineDrawerStrategy {

	
	/**
	 * Default constructor.
	 */
	public PolylineFillDrawerGL() {
		super();
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords) {
		System.err.println("Filling Polyline.");
		
	}

	
	
}
