/*------------------------------------------------------------------------*/
/* file: StringMatrix.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Matrix of string which van be drawn using JOGL                  */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.textDrawing;

import java.awt.geom.Rectangle2D;

/**
 * Matrix of string which van be drawn using JOGL
 * @author Jean-Baptiste Silvy
 */
public class StringMatrixGL extends StringMatrix {

	private double[] widestStrings;
	private double tallestString;
	/** width of each column */
	private double[][] width;
	/** height of each column */
	private double[][] height;
	
	/**
	 * Default constructor
	 */
	public StringMatrixGL() {
		super();
	}
	
	/**
	 * @return height of the string with higher height in the matrix.
	 */
	public double getTallestString() {
		return tallestString;
	}
	
	/**
	 * @return Array of the widest string for each column.
	 */
	public double[] getLongestStrings() {
		return widestStrings;
	}
	
	/**
	 * Set the matrix data from an array of string
	 * @param text array of strings conataining the data.
	 * @param nbRow number of row of the matrix.
	 * @param nbCol number of column of the matrix.
	 */
	@Override
	public void setData(String[] text, int nbRow, int nbCol) {
		super.setData(text, nbRow, nbCol);
		widestStrings = new double[nbCol];
		width = new double[nbRow][nbCol];
		height = new double[nbRow][nbCol];
	}
	
	/**
	 * Get the width of a string inside the matrix.
	 * @param numRow index of the row
	 * @param numCol index of the column
	 * @return width of the string in pixels.
	 */
	public double getStringWidth(int numRow, int numCol) {
		return width[numRow][numCol];
	}
	
	/**
	 * Get the height of a string inside the matrix.
	 * @param numRow index of the row
	 * @param numCol index of the column
	 * @return height of the string in pixels.
	 */
	public double getStringHeight(int numRow, int numCol) {
		return height[numRow][numCol];
	}
	
	/**
	 * Compute the widests strings for each row and the tallest string of the matrix
	 * @param renderer renderer used to draw the objects
	 */
	public void update(SciTextRenderer renderer) {
		int nbCol = getNbCol();
		int nbRow = getNbRow();
		
		
		for (int i = 0; i < nbRow; i++) {
			for (int j = 0; j < nbCol; j++) {
				Rectangle2D curRect = renderer.getBounds(getMatrixElement(i, j));
				width[i][j] = curRect.getWidth();
				height[i][j] = curRect.getHeight();
			}
		}
		
		tallestString = 0.0;
		widestStrings = new double[getNbCol()];
		for (int j = 0; j < getNbCol(); j++) {
			double maxWidth = 0.0;
			for (int i = 0; i < getNbRow(); i++) {
				// update width
				if (width[i][j] > maxWidth) {
					maxWidth = width[i][j];
				}
				// update height
				if (height[i][j] > tallestString) {
					tallestString = height[i][j];
				}
			}
			widestStrings[j] = maxWidth;
		}
	}
	
}
