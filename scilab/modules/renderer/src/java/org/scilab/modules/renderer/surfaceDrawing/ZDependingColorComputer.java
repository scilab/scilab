/*------------------------------------------------------------------------*/
/* file: ZDependingColorComputer.java                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class returning a color depending on the facet height           */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.surfaceDrawing;

import org.scilab.modules.renderer.utils.ColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class returning the default color
 * @author Jean-Baptiste Silvy
 */
public class ZDependingColorComputer extends FacetColorComputer {

	private static final double OFFSET = 0.5;
	
	private double zMin;
	private double zMax;
	
	private boolean areMinMaxInit;
	
	/**
	 * Default constructor
	 * @param colMap colormap to use
	 * @param decomposer decomposer from which the facet are taken
	 */
	public ZDependingColorComputer(ColorMap colMap, SurfaceDecomposer decomposer) {
		super(colMap, decomposer);
		areMinMaxInit = false;
	}
	
	/**
	 * Compute the color of the facet to draw
	 * @param corners array containg the positions of the vetices of the facet
	 * @param facetIndex index of the facet if needed
	 * @return array containg the 3 channels for each corner
	 */
	public int[] getFacetColor(Vector3D[] corners, int facetIndex) {
		
		if (!areMinMaxInit) {
			double[] minAndMax = getDecomposer().getZminAndMax();
			zMin = minAndMax[0];
			zMax = minAndMax[1];
			areMinMaxInit = true;
		}
		
		// compute average height of the facet
		double facetHeight = 0.0;
		for (int i = 0; i < getNbVertices(); i++) {
			facetHeight += corners[i].getZ();
		}
		facetHeight /=  getNbVertices();
		
		// only one color for the facet
		int[] res = new int[1];
		// code taken from old Scilab C code
		res[0] = (int) Math.floor(((getColorMap().getSize() - 1) * (facetHeight - zMin) / (zMax - zMin)) + OFFSET);
		
		return res;
	}

}
