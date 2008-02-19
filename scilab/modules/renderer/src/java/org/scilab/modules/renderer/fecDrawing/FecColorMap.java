/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Special colormap used by fec objects 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.fecDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.ColorMap;
import org.scilab.modules.renderer.utils.TexturedColorMap;


/**
 * Special colormap used by fec objects
 * @author Jean-Baptiste Silvy
 */
public class FecColorMap extends TexturedColorMap {

	private static final double OFFSET = 0.5;
	
	private double boundMin;
	private double boundMax;
	
	/**
	 * Color map on which the Fec object is build
	 */
	private ColorMap colorMap;
	
	/**
	 * Constructor from an other colorMap
	 * @param colMap base color map
	 */
	public FecColorMap(ColorMap colMap) {
		super();
		colorMap = colMap;
		boundMin = 0.0;
		boundMax = 0.0;
	}
	
	/**
	 * Set the parameters which modify the color used to draw the fec
	 * @param zMin with zMax define the interval of values from the fec to use,
	 *             if value == zMin its color will be colMin,
	 *             if value == zMax its color will be colMax.
	 *             if zMin < value < zMax its color is within colMin and colMax
	 *             if value < zMin it is colOutMin
	 *             if value > zMax it is colOutMax
	 * @param zMax see zMin
	 * @param colMin index considered as the beginning of the colormap
	 * @param colMax index considered at the en of the colormap
	 * @param colOutLow color to use if indices are outside the colorMap
	 * @param colOutUp color to use if indices are outside the colorMap
	 */
	public void setFecWeirdValues(double zMin, double zMax, int colMin, int colMax, int colOutLow, int colOutUp) {
		int useCMapSize = colMax - colMin + 1;
		
		// schema of the color map we will create
		// between zMin and zMax we will use the part of the colormap
		// which is between colMin and colMax so its length is colMax - colMin
		// to find lower size and upper size, we just use proportionality
		//
		//    <-- lowerSize --> <-- colMax - colMin --> <-- upperSize -->
		//    |----------------|-----------------------|-----------------|
		// minBoud            zmin                    zMax             maxBound
		//
		int upperSize = computeUpperSizeLength(useCMapSize, zMin, zMax, boundMin);
		int lowerSize = computeLowerSizeLength(useCMapSize, zMin, zMax, boundMax);
		
		setSize(useCMapSize + upperSize + lowerSize);
		
		fillLowerPart(lowerSize, colOutLow, colMin);
		fillMiddlePart(lowerSize, colMin, colMax);
		fillUpperPart(lowerSize + useCMapSize, upperSize, colOutUp, colMax);
	}
	
	/**
	 * Set the range of the fec values
	 * @param min minimum bound
	 * @param max maximum bound
	 */
	public void setRealBounds(double min, double max) {
		boundMin = min;
		boundMax = max;
	}
	
	/**
	 * Compute the size of the lowerPart of the colorMap
	 * @param centerSize size of the colorMap between colMin and colMax
	 * @param zMin value corresponding to colMin
	 * @param zMax value corresponding to colMax
	 * @param boundMin mimimum of the values of the fec
	 * @return size of the lower part in number of colors
	 */
	protected int computeLowerSizeLength(int centerSize, double zMin, double zMax, double boundMin) {
		if (boundMin >= zMin) {
			// no lower part needed
			return 0;
		}
		
		// ceil to be sure to enter the colormap
		return (int) Math.ceil(centerSize * (zMin - boundMin) / (zMax - zMin));
	}
	
	/**
	 * Compute the size of the upperPart of the colorMap
	 * @param centerSize size of the colorMap between colMin and colMax
	 * @param zMin value corresponding to colMin
	 * @param zMax value corresponding to colMax
	 * @param boundMax maximum of the values of the fec
	 * @return size of the lower part in number of colors
	 */
	protected int computeUpperSizeLength(int centerSize, double zMin, double zMax, double boundMax) {
		if (boundMax <= zMax) {
			// no lower part needed
			return 0;
		}
		
		// ceil to be sure to enter the colormap
		return (int) Math.ceil(centerSize * (boundMax - zMax) / (zMax - zMin));
	}
	
	/**
	 * Fill the lower part of the colormap (if one exists)
	 * @param lowerSize size of the lower part of the colormap, may be 0.
	 * @param colOutLow colOut for lower part
	 * @param colMin color with index lowerSize + 1
	 */
	protected void fillLowerPart(int lowerSize, int colOutLow, int colMin) {
		double[] color = null;
		if (colOutLow <= 0) {
			// use colMin as color
			color = colorMap.getColor(colMin - 1);
		} else {
			// use colOutLow as color
			color = colorMap.getColor(colOutLow - 1);
		}
		for (int i = 0; i < lowerSize; i++) {
			setColor(i, color);
		}
		
	}
	
	/**
	 * Fill the middle part of the colormap (if one exists)
	 * @param colMapStart starting index to use in the colorMap
	 * @param colMin color with index lowerSize + 1
	 * @param colMax last index of part of the colorMap which is between zMin and zMax
	 */
	protected void fillMiddlePart(int colMapStart, int colMin, int colMax) {
		int endIndex = colMapStart + colMax - colMin + 1;
		for (int i = colMapStart; i < endIndex; i++) {
			double[] color = colorMap.getColor(i + colMin - 1 - colMapStart);
			setColor(i, color);
		}
		
	}
	
	/**
	 * Fill the upper part of the colormap (if one exists)
	 * @param colMapStart starting index to write in the new colormap
	 * @param upperSize size of the upper part of the colormap, may be 0.
	 * @param colOutUp colOut for upper part
	 * @param colMax last index of part of the colorMap which is between zMin and zMax
	 */
	protected void fillUpperPart(int colMapStart, int upperSize, int colOutUp, int colMax) {
		double[] color = null;
		if (colOutUp <= 0) {
			// use colMax as color
			color = colorMap.getColor(colMax - 1);
		} else {
			// use colOutLow as color
			color = colorMap.getColor(colOutUp - 1);
		}
		for (int i = colMapStart; i < colMapStart + upperSize; i++) {
			setColor(i, color);
		}
		
	}
	
	/**
	 * Use a value to set the color of the colorMap
	 * @param gl current GL pipelin
	 * @param value value to use
	 */
	public void useValue(GL gl, double value) {
		// experimental value
		double colorIndex = ((getSize()) * (value - boundMin) / (boundMax - boundMin)) + OFFSET;
		applyTexCoord(gl, colorIndex);
	}
	
}
