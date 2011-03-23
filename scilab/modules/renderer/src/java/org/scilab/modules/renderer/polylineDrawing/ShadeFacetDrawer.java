/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.polylineDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.TexturedColorMap;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Interface which allows us to choose between JOGL or GL2PS 

 *
 */
public interface ShadeFacetDrawer {
	
	/**
	 * Paint the polygon given with the table of color
	 * @param coords coordinates of triangle & square
	 * @param color table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	void paintPolygon(Vector3D[] coords, int[] color, GL gl, TexturedColorMap colorMap);

	/**
	 * Paint the polygon given with the table of color
	 * @param triangleCoords coordinates of triangle & square
	 * @param triangleColors table of color
	 * @param gl GL 
	 * @param colorMap TexturedColorMap
	 */
	void paintPolygon(Vector3D[] triangleCoords, double[] triangleColors,
			GL gl, TexturedColorMap colorMap);

}
