/*------------------------------------------------------------------------*/
/* file: StringMatrix.java                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Matrix of string which van be drawn using JOGL                  */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.textDrawing;

import java.awt.geom.Rectangle2D;

import com.sun.opengl.util.j2d.TextRenderer;

/**
 * Matrix of string which van be drawn using JOGL
 * @author Jean-Baptiste Silvy
 */
public class StringMatrixGL extends StringMatrix {

	/**
	 * Defualt constructor
	 */
	public StringMatrixGL() {
		super();
	}
	
	/**
	 * @param renderer renderer used to render the matrix
	 * @return height of the string with higher height in the matrix.
	 */
	public double getHighestString(TextRenderer renderer) {
		double maxHeight = 0.0;
		for (int i = 0; i < getNbRow(); i++) {
			for (int j = 0; j < getNbCol(); j++) {
				Rectangle2D curRect = renderer.getBounds(getMatrixElement(i, j));
				if (curRect.getHeight() > maxHeight) {
					maxHeight = curRect.getHeight();
				}
			}
		}
		return maxHeight;
	}
	
	/**
	 * @param renderer renderer used to render the matrix
	 * @param numCol column index, must be lower than nbCol.
	 * @return Width of the String with higher width along a certain column
	 */
	public double getLongestString(TextRenderer renderer, int numCol) {
		double maxWidth = 0.0;
		for (int i = 0; i < getNbRow(); i++) {
			Rectangle2D curRect = renderer.getBounds(getMatrixElement(i, numCol));
			if (curRect.getWidth() > maxWidth) {
				maxWidth = curRect.getWidth();
			}
		}
		return maxWidth;
	}
	
}
