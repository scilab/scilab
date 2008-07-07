/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to get the list of facet to draw for a fac3d object
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.surfaceDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class used to get the list of facet to draw for a fac3d object
 * @author Jean-Baptiste Silvy
 */
public class Fac3dDecomposer extends SurfaceDecomposer {

	private int curFacet;
	
	/** number of facets in the fac3d object */
	private int nbFacets;
	/**
	 * Default constructor
	 */
	public Fac3dDecomposer() {
		curFacet = -1;
	}
	
	/**
	 * @return index of current facet
	 */
	protected int getCurFacetIndex() {
		return curFacet;
	}
	
	/**
	 * Specify the positions of each vertices
	 * @param xCoords array of size nbFacet * nbVertices
	 * @param yCoords array of size nbFacet * nbVertices
	 * @param zCoords array of size nbFacet * nbVertices
	 * @param nbVertices number of facets in the plot
	 */
	public void setFacetsPositions(double[] xCoords, double[] yCoords, double[] zCoords, int nbVertices) {
		super.setFacetsPositions(xCoords, yCoords, zCoords, nbVertices);
		this.nbFacets = xCoords.length / nbVertices;		
	}
	
	/**
	 * @return array containing the index of vertices already reatrived
	 * by a previous call of next().
	 * Used to prevent from drawing a vertice several times, espacially for marks.
	 */
	public int[] alreadyRetrievedVertices() {
		// in fac3d all vertices are idependent
		return null;
	}

	/**
	 * @return true if there are remaining facet not already retrieved
	 */
	public boolean hasNext() {
		return curFacet < nbFacets - 1;
	}

	/**
	 * @return position of the next facet edges
	 */
	public Vector3D[] next() {
		// go to next facet
		curFacet++;
		
		Vector3D[] res = getCurrentFacetPosition();
		if (isFacetDisplayable(res)) {
			return res;
		} else {
			return null;
		}
	}
	
	/**
	 * @return position of the current facet
	 */
	private Vector3D[] getCurrentFacetPosition() {
		Vector3D[] res = new Vector3D[getNbVertices()];
		for (int i = 0; i < getNbVertices(); i++) {
			int curIndex = getNbVertices() * curFacet + i;
			res[i] = new Vector3D(getXcoord(curIndex), getYcoord(curIndex), getZcoord(curIndex));
		}
		return res;
	}

	/**
	 * Remove a facet, not implemented since useless
	 */
	public void remove() {
		// unused
	}
	
	/**
	 * @return color(s) of the vertices of the current facet
	 */
	public int[] getCurFacetColors() {
		return getColorComputer().getFacetColor(getCurrentFacetPosition(), curFacet);
	}

	/**
	 * @return number of facets composing the surface
	 */
	protected int getNbFacets() {
		return nbFacets;
	}

}
