/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw text
 * wich filled a box
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.textDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.textRendering.FontManager;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;

/**
 * 
 * @author Jean-Baptiste Silvy
 */
public class FilledTextDrawerGL extends TextContentDrawerGL {
	
	private static final double[] FACTOR_CHANGE_INTERVAL = {0.95, 1.05};
	
	private double filledBoxWidth;
	private double filledBoxHeight;
	
	private TextGrid stringPos;
	
	private Vector3D realCenter;
	
	/**
	 * Default constructor
	 */
	public FilledTextDrawerGL() {
		super();
		filledBoxWidth = 0.0;
		filledBoxHeight = 0.0;
		stringPos = null;
		realCenter = null;
	}
	
	/**
	 * Set the size of the box which the text will fill.
	 * @param boxWidth width of the box.
	 * @param boxHeight height of the box.
	 */
	public void setFilledBoxSize(double boxWidth, double boxHeight) {
		filledBoxWidth = boxWidth;
		filledBoxHeight = boxHeight;
	}
	
	/**
	 * @return get the Scilab size of the font (usefull for filled text since actual font size)
	 * 		   may vary
	 */
	public double getScilabFontSize() {
		return FontManager.awtSizeToScilabSize(getFont().getSize2D());
	}
	
	/**
	 * Draw the text using pixel coordinates.
	 * @return 4 corners of the rectangle bounding box.
	 */
	public Vector3D[] drawTextContentPix() {
		SciTextRenderer renderer = getTextRenderer();
		
		StringMatrixGL mat = computeStringSizes(renderer, getTextMatrix());
		
		setTextMatrix(mat);
		
		// get default position with size 1.
		stringPos = getStringsPositions(getTextMatrix());

		// Compute a new font size which will fill the box.
		Vector3D[] bounds = stringPos.getExtremBounds();
		
		double curWidth = bounds[2].getX() - bounds[1].getX();
		double curHeight = bounds[0].getY() - bounds[1].getY();
		
		realCenter = new Vector3D(getTextCenterPix());
		double newBoxWidth = filledBoxWidth;
		double newBoxHeight = filledBoxHeight;
		
		// when axes are reversed size might be negative
		if (newBoxWidth < 0) {
			newBoxWidth = -newBoxWidth;
			realCenter.setX(realCenter.getX() - newBoxWidth);
		}
		
		if (newBoxHeight < 0) {
			newBoxHeight = -newBoxHeight;
			realCenter.setY(realCenter.getY() - newBoxHeight);
		}
		
		// compute the needed size for each dimension.
		double xFactor = newBoxWidth / curWidth;
		double yFactor = newBoxHeight / curHeight;
				
		// apply scale factor
		double factor = Math.min(xFactor, yFactor);
		
		
		// check if resizing is really needed
		if (factor < FACTOR_CHANGE_INTERVAL[0] || factor >  FACTOR_CHANGE_INTERVAL[1]) {
			float newSize = (float) (getFont().getSize2D() * factor);
			
			
			stringPos.scale(factor);
		
			// update text renderer
			setFont(getFont().deriveFont(newSize));
			renderer = getTextRenderer();
			// update StringSizes with the new renderer using the real display size.
			// Display size might not be equal to the requested font if fractional fonts are disabled
			getTextMatrix().scale(factor * renderer.getDisplayedFontSize() / newSize);
		}
			//
		//startRecordDL();
		stringPos = placeTextGrid(stringPos, realCenter, getTextCenterPix(), getRotationAngle());

		drawText(renderer, getTextMatrix(), stringPos);
		//endRecordDL();
		
		
		return getBoundingRectanglePix();
		
	}
	
	/**
	 * Display some from text from already precomputed positions.
	 */
	public void showTextContentPix() {
		SciTextRenderer renderer = getTextRenderer();
		
		placeTextGrid(stringPos, realCenter, getTextCenterPix(), getRotationAngle());
		
		showText(renderer, getTextMatrix(), stringPos);

	}
	
	

	/**
	 * Get the bounding box of the text matrix centered at the origin.
	 * @param text matrix of strings with the size of each string already computed.
	 * @return 4 corners of the bounding box.
	 */
	@Override
	public Vector3D[] getBoundingBox(StringMatrixGL text) {
		Vector3D[] res = {new Vector3D(0.0, filledBoxHeight, 0.0),
						  new Vector3D(0.0, 0.0, 0.0),
						  new Vector3D(filledBoxWidth, 0.0, 0.0),
						  new Vector3D(filledBoxWidth, filledBoxHeight, 0.0)};
		return res;
	}
	
	

	/**
	 * Compute the 4 corners of the bounding rectangle of the text in pixels coordinates.
	 * @return array of size 4 with the four corners.
	 */
	public Vector3D[] getBoundingRectanglePix() {
		Vector3D[] bbox = getBoundingBox(null);
		return placeBoundingBox(bbox, getTextCenterPix(), getRotationAngle());
	}

	/**
	 * Move the bounding box to the right position.
	 * @param bbox intial bounding box centered at the origin.
	 * @param textCenter text center
	 * @param rotationAngle rotation angle around the text center
	 * @return new bouding box turned
	 */
	@Override
	public Vector3D[] placeBoundingBox(Vector3D[] bbox, Vector3D textCenter,
									   double rotationAngle) {
		Vector3D rotationAxis = new Vector3D(0.0, 0.0, 1.0);
		for (int i = 0; i < bbox.length; i++) {
			// translate to textCenter
			bbox[i] = bbox[i].add(textCenter);
			// rotate around textCenter
			bbox[i] = bbox[i].rotate(textCenter, rotationAxis, rotationAngle);
		}
		return bbox;
	}

	/**
	 * Put the text grid at the righ tposition
	 * @param stringPositions Initial position of strings, centered on (0,0).
	 * @param textCenterPix position of the center in pixel coordinates
	 * @param rotationCenter center of rotation
	 * @param rotationAngle angle in radian.
	 * @return the new text grid ut at the right position.
	 */
	public TextGrid placeTextGrid(TextGrid stringPositions,
								  Vector3D textCenterPix, Vector3D rotationCenter, double rotationAngle) {
		GL gl = getGL();
		Vector3D rotationRound = getRoundedVector(rotationCenter);
		gl.glTranslated(rotationRound.getX(), rotationRound.getY(), rotationRound.getZ());
		gl.glRotated(Math.toDegrees(rotationAngle), 0.0, 0.0, 1.0);
		gl.glTranslated(-rotationRound.getX(), -rotationRound.getY(), -rotationRound.getZ());
		
		// move the text in the middle of the bounfing box
		Vector3D[] bbox = stringPositions.getExtremBounds();
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		
		gl.glTranslated(Math.round(textCenterPix.getX() + filledBoxWidth / 2.0 - halfBoxWidth),
						Math.round(textCenterPix.getY() + filledBoxHeight / 2.0 - halfBoxHeight),
						textCenterPix.getZ());
		return stringPositions;
	}

	/**
	 * Not used
	 * @param stringPositions Initial position of strings, centered on (0,0).
	 * @param textCenterPix position of the center in pixel coordinates
	 * @param rotationAngle angle in radian.
	 * @return the new text grid ut at the right position.
	 */
	@Override
	public TextGrid placeTextGrid(TextGrid stringPositions, Vector3D textCenterPix, double rotationAngle) {
		return null;
	}

}
