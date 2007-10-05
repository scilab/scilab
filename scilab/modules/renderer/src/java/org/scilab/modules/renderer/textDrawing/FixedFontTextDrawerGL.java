/*------------------------------------------------------------------------*/
/* file: FixedFontTextDrawerGL.java                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class displaying text with a font specified by user             */
/*------------------------------------------------------------------------*/


package org.scilab.modules.renderer.textDrawing;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class displaying text with a font specified by user.
 * @author Jean-Baptiste Silvy
 */
public abstract class FixedFontTextDrawerGL extends TextContentDrawerGL {

	/**
	 * Default constructor.
	 */
	public FixedFontTextDrawerGL() {
		super();
	}
	
	/**
	 * Compute the matrix containing the positions of all texts.
	 * @param text matrix of string with their size to draw
	 * @return matrix of positions
	 */
	@Override
	public TextGrid getStringsPositions(StringMatrixGL text) {

		double cellsHeights = text.getTallestString();
		double[] heights = new double[text.getNbRow()];
		for (int i = 0; i < text.getNbRow(); i++) {
			heights[i] = cellsHeights;
		}
		
		double[] widths = text.getLongestStrings();
		
		TextGrid res = new TextGrid(text.getNbRow(), text.getNbCol(), heights, widths);
		return res;
		
	}
	
	/**
	 * Draw the text using pixel coordinates.
	 * @param textCenterPix center of text to draw in pixels
	 */
	public void drawTextContentPix(Vector3D textCenterPix) {
		SciTextRenderer renderer = SciTextRenderer.create(getFont(), getFontColor());
		
		StringMatrixGL textMatrix = computeStringSizes(renderer, getTextMatrix());
		TextGrid stringPos = getStringsPositions(textMatrix);
		stringPos = placeTextGrid(stringPos, textCenterPix, getRotationAngle());
		drawText(renderer, textMatrix, stringPos);
		
		renderer.dispose();
	}


	/**
	 * Get the bounding box of the text matrix centerd at the origin.
	 * @param text matrix of strings with the size of each string already computed.
	 * @return 4 corners of the bounding box.
	 */
	@Override
	public Vector3D[] getBoundingBox(StringMatrixGL text) {
		TextGrid grid = getStringsPositions(text);
		return grid.getExtremBounds();
	}
	
	/**
	 * Compute the 4 corners of the bounding rectangle of the text in pixels coordinates.
	 * @param textCenterPix center of the text in pixel coordinates.
	 * @return array of size 4 with the four corners.
	 */
	public Vector3D[] getBoundingRectanglePix(Vector3D textCenterPix) {
		// assume we are in pixels coordinates
		SciTextRenderer renderer = SciTextRenderer.create(getFont(), getFontColor());
		
		StringMatrixGL textMatrix = computeStringSizes(renderer, getTextMatrix());
		Vector3D[] bbox = getBoundingBox(textMatrix);
		bbox = placeBoundingBox(bbox, textCenterPix, getRotationAngle());
		
		renderer.dispose();
		return bbox;
	}

}
