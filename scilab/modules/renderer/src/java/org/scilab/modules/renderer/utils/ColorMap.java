/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class to handle Scilab colormaps
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils;


/**
 * Class to handle Scilab colormaps
 * @author Jean-Baptiste Silvy
 */
public class ColorMap {
	
	private static final String COMMA = ", ";
	
	/** number of column in the colormap, represent the number of channels (ie RGB)*/
	private static final int NBCOL = 3;
	
	private static final int BLACK_INDEX = -1;
	private static final int WHITE_INDEX = -2;
	
	/** array containing red intensituy for each color */
	private double[] redChannel;
	/** array containing green intensituy for each color */
	private double[] greenChannel;
	/** array containing blue intensituy for each color */
	private double[] blueChannel;
	/** number of color in the colormap */
	private int colormapSize;
	
	/**
	 * Default constructor
	 */
	public ColorMap() {
          redChannel   = null;
          greenChannel = null;
          blueChannel  = null;
          colormapSize = 0;
	}
	
	/**
	 * Factory
	 * @return a new instance of colormap
	 */
	public static ColorMap create() {
		return new ColorMap();
	}
	
	/**
	 * Get the channels of a color given its index in the colormap
	 * @param colorIndex color index in the colormap
	 * @param color return result, should be an array of size 3 to store the 3 channels
	 */
	public void getColor(int colorIndex, double[] color) {
		color[0] = getRedChannel(colorIndex);
		color[1] = getGreenChannel(colorIndex);
		color[2] = getBlueChannel(colorIndex);
	}
	
	/**
	 * Get the channels of a color given its index in the colormap
	 * @param colorIndex color index in the colormap
	 * @return An array of size 3 to store the 3 channels
	 */
	public double[] getColor(int colorIndex) {
		double[] res = new double[NBCOL];
		getColor(colorIndex, res);
		return res;
	}
	
	/**
	 * Get the 3 color channels of several color indices
	 * @param colorIndices color indices in the colormap
	 * @return Array of same size as colorIndices
	 */
	public double[][] getColors(int[] colorIndices) {
		int nbIndices = colorIndices.length;
		double[][] res = new double[nbIndices][];
		for (int i = 0; i < nbIndices; i++) {
			res[i] = getColor(colorIndices[i]);
		}
		return res;
	}
	
	/**
	 * @param colorIndex color index in the colormap
	 * @return the red channel of the color corresponding to the colorIndex
	 */
	public double getRedChannel(int colorIndex) {
		return redChannel[clampColorIndex(colorIndex)];
	}
	
	/**
	 * @param colorIndex color index in the colormap
	 * @return the red channel of the color corresponding to the colorIndex
	 */
	public double getGreenChannel(int colorIndex) {
		return greenChannel[clampColorIndex(colorIndex)];
	}
	
	/**
	 * @param colorIndex color index in the colormap
	 * @return the red channel of the color corresponding to the colorIndex
	 */
	public double getBlueChannel(int colorIndex) {
		return blueChannel[clampColorIndex(colorIndex)];
	}
	
	/**
	 * Clamp a colormap index to the valid range of colormap.
	 * To be used for index which are sent to renderer
	 * directly in colormap index (and not in Scilab one).
	 * @param colorIndex imput index we want to use
	 * @return clamped index betwwen 0 and whiteId
	 */
	public int clampColorIndex(int colorIndex) {
		if (colorIndex <= 0) {
			return 0;
		} else if (colorIndex >= colormapSize + 1) {
			return colormapSize + 1;
		} else {
			return colorIndex;
		}
	}
	
	/**
	 * Set new data to the colorMap
	 * @param newData nbColor x 3 matrix containign the 3 channels
	 *                the matrix is stored rowwise like in Scilab
	 */
	public void setData(double[] newData) {
		
		// new size
		setSize(newData.length / NBCOL);
		
		// red channel
		System.arraycopy(newData, 0, redChannel, 0, colormapSize);
		
		// green channel
		System.arraycopy(newData, colormapSize, greenChannel, 0, colormapSize);

		// blue channel
		System.arraycopy(newData, 2 * colormapSize, blueChannel, 0, colormapSize);

	}
	
	/**
	 * Get the nbColor x 3 matrix of the colormap
	 * Should only be used from C code
	 * @return matrix of the colormap
	 */
	public double[] getData() {
		double[] res = new double[NBCOL * colormapSize];
		getData(res);
		return res;
	}
	
	/**
	 * Fill the data array with the colormap data
	 * data should be of size colormapSize x 3
	 * @param data matrix wich will be filled
	 */
	public void getData(double[] data) {
		// copy channels
		System.arraycopy(redChannel  , 0, data,                0, colormapSize);
		System.arraycopy(greenChannel, 0, data,     colormapSize, colormapSize);
		System.arraycopy(blueChannel , 0, data, 2 * colormapSize, colormapSize);
	}
	
	/**
	 * Get number of color in the colorMap
	 * @return number of color
	 */
	public int getSize() {
		return colormapSize;
	}
	
	/**
	 * Specify a new size for the colorMap
	 * @param newSize new number of colors in the colormap
	 */
	public void setSize(int newSize) {
		colormapSize = newSize;
		redChannel = new double[colormapSize + 2]; // +2 for black and white
		greenChannel = new double[colormapSize + 2]; // +2 for black and white
		blueChannel = new double[colormapSize + 2]; // +2 for black and white
		
		// add black and whit colors
		redChannel[colormapSize    ] = 0.0; // black
		redChannel[colormapSize + 1] = 1.0; // white
		
		// green channel
		greenChannel[colormapSize    ] = 0.0; // black
		greenChannel[colormapSize + 1] = 1.0; // white
		
		// blue channel
		blueChannel[colormapSize    ] = 0.0; // black
		blueChannel[colormapSize + 1] = 1.0; // white
	}
	
	/**
	 * Specify a new color for the colorMaps
	 * @param index index of the color to modify
	 * @param color array of size 3 containing the 3 channels
	 */
	public void setColor(int index, double[] color) {
		redChannel[index] = color[0];
		greenChannel[index] = color[1];
		blueChannel[index] = color[2];
	}
	
	/**
	 * Convert scilab color index to colormap index
	 * @param scilabIndex index in scilab
	 * @return correspondinf index in the colormap
	 */
	public int convertScilabToColorMapIndex(int scilabIndex) {
		if (scilabIndex == BLACK_INDEX || scilabIndex == 0) {
			// black color
			return colormapSize;
		} else if (scilabIndex == WHITE_INDEX) {
			// white color
			return colormapSize + 1;
		} else if (scilabIndex < WHITE_INDEX) {
			// first index
			return 0;
		} else if (scilabIndex >= colormapSize) {
			// last index
			return colormapSize - 1;
		} else {
			return scilabIndex - 1;
		}
	}
	
	/**
	 * @return string representation of the class
	 */
	public String toString() {
		String res = "";
		for (int i = 0; i < getSize() + 2; i++) {
			res += "[" + redChannel[i] + COMMA + greenChannel[i] + COMMA + blueChannel[i] + "]\n";
		}
		return res;
	}
}
