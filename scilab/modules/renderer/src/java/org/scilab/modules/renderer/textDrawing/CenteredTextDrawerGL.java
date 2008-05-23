/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class drawing text with tex_box_mode set to centered.
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
 * Class drawing text with tex_box_mode set to centered.
 * @author Jean-Baptiste Silvy
 */
public class CenteredTextDrawerGL extends FixedFontTextDrawerGL {

	private double halfCenteredBoxWidth;
	private double halfCenteredBoxHeight;
	
	/**
	 * Default constructor
	 */
	public CenteredTextDrawerGL() {
		super();
		halfCenteredBoxWidth = 0.0;
		halfCenteredBoxHeight = 0.0;
	}
	
	/**
	 * Set the size of the box which the text will fill.
	 * @param boxWidth width of the box.
	 * @param boxHeight height of the box.
	 */
	public void setFilledBoxSize(double boxWidth, double boxHeight) {
		halfCenteredBoxWidth = (boxWidth / 2.0);
		halfCenteredBoxHeight = (boxHeight / 2.0);
	}
	
	/**
	 * Move the bounding box to the right position.
	 * @param bbox intial bounding box centered at the origin.
	 * @param textCenter text center
	 * @param rotationAngle rotation angle around the text center
	 * @return new bouding box turned
	 */
	public Vector3D[] placeBoundingBox(Vector3D[] bbox, Vector3D textCenter, double rotationAngle) {
		// text center refer here to the bottom left corner.
		// So we need to add half of the box size.
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		
		Vector3D rotationAxis = new Vector3D(0.0, 0.0, 1.0);
		for (int i = 0; i < bbox.length; i++) {
			
			// translate to textCenter
			bbox[i] = bbox[i].add(textCenter);
			bbox[i] = bbox[i].add(new Vector3D(halfCenteredBoxWidth - halfBoxWidth, halfCenteredBoxHeight - halfBoxHeight, 0.0));
			
//			 rotate around textCenter
			bbox[i] = bbox[i].rotate(textCenter, rotationAxis, rotationAngle);
		}
		return bbox;
	}

	/**
	 * Put the text grid at the right position
	 * @param stringPositions Initial position of strings, centered on (0,0).
	 * @param textCenterPix position of the center in pixel coordinates
	 * @param rotationAngle angle in radian.
	 * @return the new text grid ut at the right position.
	 */
	public TextGrid placeTextGrid(TextGrid stringPositions, Vector3D textCenterPix, double rotationAngle) {
		//		 text center refer here to the bottom left corner.
		// So we need to add half of the box size.
		Vector3D[] bbox = stringPositions.getExtremBounds();
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		
		GL gl = getGL();
		gl.glTranslated(textCenterPix.getX(), textCenterPix.getY(), textCenterPix.getZ());
		gl.glRotated(Math.toDegrees(rotationAngle), 0.0, 0.0, 1.0);
		gl.glTranslated(halfCenteredBoxWidth - halfBoxWidth, halfCenteredBoxHeight - halfBoxHeight, 0.0);
		return stringPositions;
	}

}
