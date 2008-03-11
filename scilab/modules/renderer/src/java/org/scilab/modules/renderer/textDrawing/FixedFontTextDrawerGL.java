/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class displaying text with a font specified by user
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


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
	 * Draw the text using pixel coordinates.
	 * @param textCenterPix center of text to draw in pixels
	 */
	public void drawTextContentPix(Vector3D textCenterPix) {
		SciTextRenderer renderer = getTextRenderer();
		
		StringMatrixGL textMatrix = computeStringSizes(renderer, getTextMatrix());
		TextGrid stringPos = getStringsPositions(textMatrix);
		stringPos = placeTextGrid(stringPos, textCenterPix, getRotationAngle());
		drawText(renderer, textMatrix, stringPos);
		
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
		SciTextRenderer renderer = getTextRenderer();
		
		StringMatrixGL textMatrix = computeStringSizes(renderer, getTextMatrix());
		Vector3D[] bbox = getBoundingBox(textMatrix);
		bbox = placeBoundingBox(bbox, textCenterPix, getRotationAngle());
		
		return bbox;
	}

}
