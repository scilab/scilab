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
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;

/**
 * Class displaying text with a font specified by user.
 * @author Jean-Baptiste Silvy
 */
public abstract class FixedFontTextDrawerGL extends TextContentDrawerGL {

	private TextGrid stringPos;
	
	/**
	 * Default constructor.
	 */
	public FixedFontTextDrawerGL() {
		super();
		stringPos = null;
	}
	
	/**
	 * Draw the text using pixel coordinates.
	 * @return 4 corners of the rectangle bounding box.
	 */
	public Vector3D[] drawTextContentPix() {
		SciTextRenderer renderer = getTextRenderer();
		
		setTextMatrix(computeStringSizes(renderer, getTextMatrix()));
		
		stringPos = getStringsPositions(getTextMatrix());
		
		//startRecordDL();
		
		
		
		stringPos = placeTextGrid(stringPos, getTextCenterPix(), getRotationAngle());
		
		drawText(renderer, getTextMatrix(), stringPos);
		
		//endRecordDL();
		
		Vector3D[] bbox = stringPos.getExtremBounds();
		return placeBoundingBox(bbox, getTextCenterPix(), getRotationAngle());
	}
	
	/**
	 * Display some from text from already precomputed positions.
	 */
	public void showTextContentPix() {
		SciTextRenderer renderer = getTextRenderer();
		
		placeTextGrid(stringPos, getTextCenterPix(), getRotationAngle());
		
		showText(renderer, getTextMatrix(), stringPos);
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
	 * @return array of size 4 with the four corners.
	 */
	public Vector3D[] getBoundingRectanglePix() {
		// assume we are in pixels coordinates
		SciTextRenderer renderer = getTextRendererNoColor();
		
		setTextMatrix(computeStringSizes(renderer, getTextMatrix()));
		Vector3D[] bbox = getBoundingBox(getTextMatrix());
		bbox = placeBoundingBox(bbox, getTextCenterPix(), getRotationAngle());
		
		return bbox;
	}

}
