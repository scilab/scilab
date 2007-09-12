/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerGL.java                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to fill the area */
/*        enclosed by the polyline with interpolated colors               */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.polylineDrawing;

import org.scilab.modules.renderer.AutoDrawableObjectGL;

/**
 * Class containing the driver dependant routines to fill the area
 *        enclosed by the polyline with interpolated colors 
 * @author Jean-Baptiste Silvy
 */
public class PolylineInterpColorDrawerGL extends AutoDrawableObjectGL {
	
	/**
	 * Default constructor
	 */
	public PolylineInterpColorDrawerGL() {
		
	}
	
	/**
	 * Create the display list for the polyline
	 * @param xCoords X coordinates of the polylines vertices
	 * @param yCoords Y coordinates of the polylines vertices
	 * @param zCoords Z coordinates of the polylines vertices
	 * @param colors colors index for each vertex
	 */
	public void drawPolyline(double[] xCoords, double[] yCoords,
							 double[] zCoords, int[] colors) {
		System.err.println("Fill with interpolated colors.");
		
	}
	
	
}
