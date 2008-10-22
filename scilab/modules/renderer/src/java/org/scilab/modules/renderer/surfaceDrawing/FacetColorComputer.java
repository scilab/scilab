/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class used to compute facet color from theit positions and indices
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
 * Class used to compute facet color from theit positions and indices
 * @author Jean-Baptiste Silvy
 */
public abstract class FacetColorComputer {

	/** Facet are drawn with only one color */
	protected static final int CONSTANT_COLOR = 0;
	/** Facet color depends on Z coordinate */
	protected static final int Z_DEPENDING_COLOR = 1;
	/** Each facet color is specified by user */
	protected static final int FLAT_SHADING = 2;
	/** Each facet vertex color is sepecified by user */
	protected static final int INTERPOLATED_SHADING = 3;
	/** Same as flat shading here */
	protected static final int MATLAB_FLAT_SHADING = 4;
	
	/** array containing the colors to use */
	private double[] colors;
	
	/** colorMap to use */
	private ColorMap colorMap;
	
	private SurfaceDecomposer decomposer;
	
	private int defaultColor;
	
	/**
	 * Default constructor
	 * @param colMap colormap to use
	 * @param decomposer decomposer from which the facet are taken, null if no colors are needed
	 */
	public FacetColorComputer(ColorMap colMap, SurfaceDecomposer decomposer) {
		this.colorMap = colMap;
		this.decomposer = decomposer;
	}
	
	/**
	 * Factory for FacetColorComputer
	 * @param colorFlag specify the kinf of FacetColorComputer to use
	 * @param colMap colormap to use
	 * @param decomposer decomposer from which the facet are taken, null if no colors are needed
	 * @return new instance of FacetColorComputer
	 */
	public static FacetColorComputer create(int colorFlag, ColorMap colMap, SurfaceDecomposer decomposer) {
		switch (colorFlag) {
		case CONSTANT_COLOR:
			return new ConstantColorComputer(colMap, decomposer);
		case Z_DEPENDING_COLOR:
			return new ZDependingColorComputer(colMap, decomposer);
		case FLAT_SHADING:
		case INTERPOLATED_SHADING:
		case MATLAB_FLAT_SHADING:
			return new PerFacetColorComputer(colMap, decomposer);
		default:
			return new ConstantColorComputer(colMap, decomposer);
		}
	}
	
	/**
	 * @return used colorMap
	 */
	public ColorMap getColorMap() {
		return colorMap;
	}
	
	/**
	 * @return color at index i
	 * @param i index of the color
	 */
	public double getColor(int i) {
		// abs taken from old code
		return Math.abs(colors[i]);
	}
	
	/**
	 * @return number of color in the color array
	 */
	public int getNbColors() {
		if (colors == null) {
			return 0;
		} else {
			return colors.length;
		}
	}
	
	/**
	 * Set the array of colors
	 * @param colors array of colors
	 */
	public void setFacetsColor(double[] colors) {
		this.colors = colors;
	}
	
	/**
	 * Compute the color of the facet to draw
	 * @param corners array containg the positions of the vetices of the facet
	 * @param facetIndex index of the facet if needed
	 * @return array containg the 3 channels for each corner
	 */
	public abstract int[] getFacetColor(Vector3D[] corners, int facetIndex);

	/**
	 * @return number of vertice per facet
	 */
	public int getNbVertices() {
		return decomposer.getNbVertices();
	}
	
	/**
	 * @return number of facets in the surface
	 */
	public int getNbFacets() {
		return decomposer.getNbFacets();
	}
	
	/**
	 * Set the default colorto use
	 * @param defaultColor default color index to use
	 */
	public void setDefaultColor(int defaultColor) {
		this.defaultColor = defaultColor;
	}
	
	/**
	 * @return default color index
	 */
	public int getDefaultColor() {
		return defaultColor;
	}
	
	/**
	 * @return decomposer object used.
	 */
	protected SurfaceDecomposer getDecomposer() {
		return decomposer;
	}
	
}
