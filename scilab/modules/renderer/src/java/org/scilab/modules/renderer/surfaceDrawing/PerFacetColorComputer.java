/*------------------------------------------------------------------------*/
/* file: PerFacetColorComputer.java                                       */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class returning colors from an array of specified colors        */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.surfaceDrawing;

import org.scilab.modules.renderer.utils.ColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class returning colors from an array of specified colors
 * @author Jean-Baptiste Silvy
 */
public class PerFacetColorComputer extends FacetColorComputer {

	/**
	 * Default constructor
	 * @param colMap colormap to use
	 * @param decomposer decomposer from which the facet are taken
	 */
	public PerFacetColorComputer(ColorMap colMap, SurfaceDecomposer decomposer) {
		super(colMap, decomposer);
	}
	
	/**
	 * Compute the color of the facet to draw
	 * @param corners array containg the positions of the vetices of the facet
	 * @param facetIndex index of the facet if needed
	 * @return array containg the 3 channels for each corner
	 */
	public int[] getFacetColor(Vector3D[] corners, int facetIndex) {
		int[] res = null;
		if (getNbFacets() == getNbColors()) {
			// only one color per facet
			res = new int[1];
			res[0] = getColorMap().convertScilabToColorMapIndex((int) getColor(facetIndex));
		} else {
			// one color per vertex
			res = new int[getNbVertices()];
			for (int i = 0; i < getNbVertices(); i++) {
				double curColor = getColor(getNbVertices() * facetIndex + i);
				res[i] = getColorMap().convertScilabToColorMapIndex((int) curColor);
			}
		}
		return res;
	}

}
