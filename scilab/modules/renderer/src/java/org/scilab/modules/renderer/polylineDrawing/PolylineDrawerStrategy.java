/*------------------------------------------------------------------------*/
/* file: ArcDrawerStrategy.java                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Interface for polyline drawing                                  */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

/**
 * Interface for polyline drawing
 * @author Jean-Baptiste Silvy
 */
public interface PolylineDrawerStrategy {

	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 */
	void drawPolyline(double[] xCoords, double[] yCoords, double[] zCoords);
	
}
