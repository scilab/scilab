/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class returning a color depending on the facet height 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


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
	private double zRange;
	
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
			
			// avoid dividing by 0
			zRange = zMax - zMin;
			if (zRange < Double.MIN_VALUE) {
				zRange = Double.MIN_VALUE;
			}
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
		res[0] = (int) Math.floor(((getColorMap().getSize() - 1) * (facetHeight - zMin) / (zRange)) + OFFSET) + 1;
		
		return res;
	}

}
