/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw text     */
/*        with JOGL.The text is left aligned                              */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.textDrawing;



/**
 * Class containing the driver dependant routines to draw text
 * with JOGL.The text is left aligned
 * @author Jean-Baptiste Silvy
 */
public class StandardTextDrawerGL extends TextContentDrawerGL {
	
	/**
	 * Default contructor
	 */
	public StandardTextDrawerGL() {
		super();
	}

	/**
	 * Compute the 4 corners of the bounding rectangle of the text.
	 * @return array of size 12 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY, cornerZ}.
	 */
	@Override
	public double[] getBoundingRectangle() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * Get the bounding box of the text in pixels on the screen.
	 * @return array of size 8 which is the concatenation of the 4 corners
	 *         where a corner is the array {cornerX, cornerY}.
	 */
	@Override
	public int[] getScreenBoundingBox() {
		return null;
	}

	/**
	 * Compute the matrix containing the positions of all texts.
	 * @return matrix of positions
	 */
	@Override
	protected TextGrid getStringsPositions() {
		StringMatrixGL curText = getTextMatrix();
		SciTextRenderer curRenderer = getRenderer();
		curText.update(curRenderer);
		
		double cellsHeights = curText.getTallestString();
		double[] heights = new double[curText.getNbRow()];
		for (int i = 0; i < curText.getNbRow(); i++) {
			heights[i] = cellsHeights;
		}
		
		double[] widths = curText.getLongestStrings();
		
		TextGrid res = new TextGrid(curText.getNbRow(), curText.getNbCol(), heights, widths);
		return res;
		
	}
	
}
