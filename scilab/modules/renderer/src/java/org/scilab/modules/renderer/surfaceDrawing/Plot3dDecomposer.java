/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to get the list of facet to draw for a plot3d object
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
public class Plot3dDecomposer extends SurfaceDecomposer {

	private static final int PLOT3D_NB_VERTICES = 4;
	
	private int curXindex;
	private int curYindex;
	
	/** number of facets along X axis*/
	private int xLength;
	
	/** number of facets along Y axis */
	private int yLength;
	
	/**
	 * Default constructor
	 */
	public Plot3dDecomposer() {
		curXindex = -1;
		curYindex = 0;
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
		xLength = xCoords.length - 1;
		yLength = yCoords.length - 1;
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
		return !((curXindex == xLength - 1) && (curYindex == yLength - 1));
	}

	/**
	 * @return position of the next facet edges
	 */
	public Vector3D[] next() {
		// go to next facet
		// we move column wise
		curXindex++;
		
		// check if we are at the end of the line
		if (curXindex == xLength) {
			curXindex = 0;
			curYindex++;
		}
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
		// number of vertices is 4
		
		
		Vector3D[] res = new Vector3D[getNbVertices()];
		res[0] = new Vector3D(getXcoord(curXindex), getYcoord(curYindex), getZvalue(curXindex, curYindex));
		res[1] = new Vector3D(getXcoord(curXindex), getYcoord(curYindex + 1), getZvalue(curXindex, curYindex + 1));
		res[2] = new Vector3D(getXcoord(curXindex + 1), getYcoord(curYindex + 1), getZvalue(curXindex + 1, curYindex + 1));
		res[PLOT3D_NB_VERTICES - 1] = new Vector3D(getXcoord(curXindex + 1), getYcoord(curYindex), getZvalue(curXindex + 1, curYindex));
		return res;
	}
	
	/**
	 * Get the z value corresponding to index X and index Y
	 * @param xIndex index along X axis
	 * @param yIndex index along Y axis
	 * @return value corresponding value
	 */
	private double getZvalue(int xIndex, int yIndex) {
		return getZcoord(xIndex + yIndex * (xLength + 1));
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
		/** no need for indice actually */
		return getColorComputer().getFacetColor(getCurrentFacetPosition(), 0);
	}

	/**
	 * @return number of facets composing the surface
	 */
	protected int getNbFacets() {
		return xLength * yLength;
	}

}
