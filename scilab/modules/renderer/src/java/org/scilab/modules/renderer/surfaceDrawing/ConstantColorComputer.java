/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class returning the default color  
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
public class ConstantColorComputer extends FacetColorComputer {

	/**
	 * Default constructor
	 * @param colMap colormap to use
	 * @param decomposer decomposer from which the facet are taken
	 */
	public ConstantColorComputer(ColorMap colMap, SurfaceDecomposer decomposer) {
		super(colMap, decomposer);
	}
	
	/**
	 * Compute the color of the facet to draw
	 * @param corners array containg the positions of the vetices of the facet
	 * @param facetIndex index of the facet if needed
	 * @return array containg the 3 channels for each corner
	 */
	public int[] getFacetColor(Vector3D[] corners, int facetIndex) {
		int[] res = {getDefaultColor()};
		return res;
	}

}
