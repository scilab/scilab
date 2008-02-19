/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized positioning labels in order to get the
 * best display
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


package org.scilab.modules.renderer.labelDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.subwinDrawing.BoxTrimmingObjectGL;
import org.scilab.modules.renderer.utils.CoordinateTransformation;
import org.scilab.modules.renderer.utils.geom3D.Vector3D;

/**
 * Class specialized positioning labels in order to get the
 * best display
 * @author Jean-Baptiste Silvy
 */
public abstract class LabelPositionerGL extends BoxTrimmingObjectGL {
	
	private static final int NB_CORNERS = 4;
	/** increase mimimum distance from label to axis with this factor */
	private static final double OFFSET = 1.25;
	private double distToAxis;
	
	private Vector3D[] labelBoxCorners;
	
	/**
	 * Default constructor
	 */
	public LabelPositionerGL() {
		super();
		distToAxis = 0.0;
		labelBoxCorners = new Vector3D[NB_CORNERS];
		for (int i = 0; i < NB_CORNERS; i++) {
			labelBoxCorners[i] = new Vector3D(0.0, 0.0, 0.0);
		}
	}

	/**
	 * @return minimum disctna ce to axis to use
	 */
	public double getDistToAxis() {
		return distToAxis;
	}
	
	/**
	 * Specify the bounding box of the labels
	 * @param cornersCoords array of size 8, containg the pixelCoordinates of the
	 *                      four corners of the bounding box in pixels
	 */
	public void setLabelPixBoundingBox(int[] cornersCoords) {
		for (int i = 0; i < NB_CORNERS; i++) {
			// we are working in pixel coordinates Z coordinate is not used
			labelBoxCorners[i] = new Vector3D(cornersCoords[2 * i], cornersCoords[2 * i + 1], 0.0);
		}
	}
	
	/**
	 * Compute the length of the bounding box enclosing circle
	 * @return size of the radius in pixels
	 */
	protected double getBoundingBoxRadius() {
		// return half of the rectangle diagonal
		return labelBoxCorners[0].substract(labelBoxCorners[2]).getNorm() / 2.0;
	}
	
	/**
	 * @return height of the label bounding box
	 */
	protected double getBoundingBoxHeight() {
		return Math.abs(labelBoxCorners[0].substract(labelBoxCorners[2]).getY());
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @param axisStart one edge of the corresponding axis
	 * @param axisEnd other edge of the corresponding axis
	 * @param ticksDir direction of the ticks
	 * @return vector containg the position 
	 */
	protected Vector3D getLabelPosition(Vector3D axisStart, Vector3D axisEnd, Vector3D ticksDir) {
		GL gl = getGL();
		
		// compute position of axis segment start and end in pixels
		CoordinateTransformation transform = CoordinateTransformation.getTransformation(gl);
		Vector3D axisStartPix = transform.getCanvasCoordinates(gl, axisStart);
		Vector3D axisEndPix = transform.getCanvasCoordinates(gl, axisEnd);
		Vector3D ticksDirPix = transform.getCanvasCoordinates(gl, ticksDir);
		Vector3D originPix = transform.getCanvasCoordinates(gl, Vector3D.ORIGIN);
		ticksDirPix = ticksDirPix.substract(originPix).getNormalized();
		
		// label position is in fornt of the middle of the axis segment.
		Vector3D axisMiddlePix = axisStartPix.add(axisEndPix).scalarMult(1.0 / 2.0);
		Vector3D labelPosPix = axisMiddlePix.add(ticksDirPix.scalarMult(getDistToAxis() * OFFSET + getBoundingBoxRadius()));
		
		return transform.retrieveSceneCoordinates(gl, labelPosPix);
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @param distanceToAxis width of the space used nera the axis
	 * @return array of size 3 containg the position
	 */
	public double[] getLabelPosition(double distanceToAxis) {
		this.distToAxis = distanceToAxis;
		return getLabelPosition().getValues();
	}
	
	/**
	 * Compute the position of the label for best display.
	 * @return vector containg the position 
	 */
	public abstract Vector3D getLabelPosition();
	
}
