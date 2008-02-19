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

/**
 * 
 * @author Jean-Baptiste Silvy
 */
public class FilledTextDrawerGL extends TextContentDrawerGL {

	private static final double DEFAULT_FONT_SIZE = 10.0;
	
	private double filledBoxWidth;
	private double filledBoxHeight;
	
	/**
	 * Default constructor
	 */
	public FilledTextDrawerGL() {
		super();
		filledBoxWidth = 0.0;
		filledBoxHeight = 0.0;
	}
	
	/**
	 * Set the size of the box which the text will fill.
	 * @param boxWidth width of the box.
	 * @param boxHeight height of the box.
	 */
	public void setFilledBoxSize(int boxWidth, int boxHeight) {
		filledBoxWidth = (double) boxWidth;
		filledBoxHeight = (double) boxHeight;
	}
	
	/**
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param rotationAngle text rotationAngle.
	 * @param boxWidth width of the box.
	 * @param boxHeight height of the box.
	 */
	public void setTextParameters(int textAlignement, int color, int fontTypeIndex,
								  double rotationAngle,
                                  int boxWidth, int boxHeight) {
		super.setTextParameters(textAlignement, color, fontTypeIndex, DEFAULT_FONT_SIZE, rotationAngle);
		setFilledBoxSize(boxWidth, boxHeight);
	}
	
	/**
	 * Draw the text using pixel coordinates.
	 * @param textCenterPix center of text to draw in pixels
	 */
	@Override
	public void drawTextContentPix(Vector3D textCenterPix) {
		SciTextRenderer renderer = getTextRenderer();
		
		StringMatrixGL textMatrix = computeStringSizes(renderer, getTextMatrix());
		// get default position with size 1.
		TextGrid stringPos = getStringsPositions(textMatrix);

		// Compute a new font size which will fill the box.
		Vector3D[] bounds = stringPos.getExtremBounds();
		double curWidth = bounds[2].getX() - bounds[1].getX();
		double curHeight = bounds[0].getY() - bounds[1].getY();
		
		Vector3D newCenter = new Vector3D(textCenterPix);
		double newBoxWidth = filledBoxWidth;
		double newBoxHeight = filledBoxHeight;
		// when axes are reversed size might be negative
		if (newBoxWidth < 0) {
			newBoxWidth = -newBoxWidth;
			newCenter.setX(newCenter.getX() - newBoxWidth);
		}
		
		if (newBoxHeight < 0) {
			newBoxHeight = -newBoxHeight;
			newCenter.setY(newCenter.getY() - newBoxHeight);
		}
		
		// compute the needed size for each dimension.
		double xFactor = newBoxWidth / curWidth;
		double yFactor = newBoxHeight / curHeight;
		
		// apply scale factor
		double factor = Math.min(xFactor, yFactor);
		
		float newFontSize = (float) (getFont().getSize2D() * factor);
		stringPos.scale(factor);
		
		// create a new renderer with a new font.
		destroyTextRenderer();
		setFont(getFont().deriveFont(newFontSize));
		//renderer = SciTextRenderer.create(getFont().deriveFont(newFontSize), getFontColor());
		renderer = getTextRenderer();
		
		// update StringSizes with the new renderer
		textMatrix.update(renderer);
		stringPos = placeTextGrid(stringPos, newCenter, textCenterPix, getRotationAngle());
		
		
		drawText(renderer, textMatrix, stringPos);
		
		destroyTextRenderer();
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
	 * @param textCenterPix center of the text in pixel coordinates.
	 * @return array of size 4 with the four corners.
	 */
	@Override
	public Vector3D[] getBoundingRectanglePix(Vector3D textCenterPix) {
		Vector3D[] bbox = getBoundingBox(null);
		return placeBoundingBox(bbox, textCenterPix, getRotationAngle());
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
	//@Override
	public TextGrid placeTextGrid(TextGrid stringPositions,
								  Vector3D textCenterPix, Vector3D rotationCenter, double rotationAngle) {
		GL gl = getGL();
		gl.glTranslated(rotationCenter.getX(), rotationCenter.getY(), rotationCenter.getZ());
		gl.glRotated(Math.toDegrees(rotationAngle), 0.0, 0.0, 1.0);
		gl.glTranslated(-rotationCenter.getX(), -rotationCenter.getY(), -rotationCenter.getZ());
		
		// move the text in the middle of the bounfing box
		Vector3D[] bbox = stringPositions.getExtremBounds();
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		
		gl.glTranslated(textCenterPix.getX(), textCenterPix.getY(), textCenterPix.getZ());
		gl.glTranslated(filledBoxWidth / 2.0 - halfBoxWidth, filledBoxHeight / 2.0 - halfBoxHeight, 0.0);
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
		// TODO Auto-generated method stub
		return null;
	}

}
