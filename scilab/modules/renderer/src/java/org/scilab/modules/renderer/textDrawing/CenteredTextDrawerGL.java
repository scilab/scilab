/*------------------------------------------------------------------------*/
/* file: StandardTextDrawerGL.java                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing text with tex_box_mode set to centered.           */
/*------------------------------------------------------------------------*/


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
	public void setFilledBoxSize(int boxWidth, int boxHeight) {
		halfCenteredBoxWidth = ((double) boxWidth / 2.0);
		halfCenteredBoxHeight = ((double) boxHeight / 2.0);
	}
	
	/**
	 * Set many text parameters in one function.
	 * @param textAlignement kind of alignement.
	 * @param color index of the color in the colormap.
	 * @param fontTypeIndex index of the font in the font array.
	 * @param fontSize font size to use.
	 * @param rotationAngle text rotationAngle.
	 * @param boxWidth width of the box.
	 * @param boxHeight height of the box.
	 */
	public void setTextParameters(int textAlignement, int color, int fontTypeIndex,
								  double fontSize, double rotationAngle,
                                  int boxWidth, int boxHeight) {
		super.setTextParameters(textAlignement, color, fontTypeIndex, fontSize, rotationAngle);
		setFilledBoxSize(boxWidth, boxHeight);
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
		Vector3D realCenter = textCenter.add(new Vector3D(halfCenteredBoxWidth, halfCenteredBoxHeight, 0.0));
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		Vector3D translation = realCenter.substract(new Vector3D(halfBoxWidth, halfBoxHeight, 0.0));
		
		Vector3D rotationAxis = new Vector3D(0.0, 0.0, 1.0);
		for (int i = 0; i < bbox.length; i++) {
			// translate to textCenter
			bbox[i] = bbox[i].add(translation);
			// rotate around textCenter
			bbox[i] = bbox[i].rotate(realCenter, rotationAxis, rotationAngle);
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
		Vector3D realCenter = textCenterPix.add(new Vector3D(halfCenteredBoxWidth, halfCenteredBoxHeight, 0.0));
		double halfBoxWidth = (bbox[2].getX() - bbox[1].getX()) / 2.0;
		double halfBoxHeight = (bbox[0].getY() - bbox[1].getY()) / 2.0;
		
		GL gl = getGL();
		gl.glTranslated(realCenter.getX(), realCenter.getY(), realCenter.getZ());
		gl.glRotated(Math.toDegrees(rotationAngle), 0.0, 0.0, 1.0);
		gl.glTranslated(-halfBoxWidth, -halfBoxHeight, 0.0);
		return stringPositions;
	}

}
