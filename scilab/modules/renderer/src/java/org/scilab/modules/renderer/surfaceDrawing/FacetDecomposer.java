/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Interface for retreiving facets compising a surface (ie plot3d or fac3d) object.
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.surfaceDrawing;

import java.util.Iterator;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Interface for reteiving facets compising a surface (ie plot3d
 * or fac3d) object
 * @author Jean-Baptiste Silvy
 */
public interface FacetDecomposer extends Iterator<Vector3D[]> {
	
	int FAC3D = 0;
	int PLOT3D = 1;
	
	/**
	 * @return array containing the index of vertices already reatrived
	 * by a previous call of next().
	 * Used to prevent from drawing a vertice several times, espacially for marks.
	 */
	int[] alreadyRetrievedVertices();
	
	/**
	 * Specify the positions of each vertices
	 * @param xCoords array of size nbFacet * nbVertices
	 * @param yCoords array of size nbFacet * nbVertices
	 * @param zCoords array of size nbFacet * nbVertices
	 * @param nbVertices number of vertices for each facet
	 */
	void setFacetsPositions(double[] xCoords, double[] yCoords, double[] zCoords, int nbVertices);
	
}
