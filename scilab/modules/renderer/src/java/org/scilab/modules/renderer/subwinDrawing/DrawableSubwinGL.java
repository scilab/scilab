/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing the driver dependant routines to draw a 
 * subwin object with JoGL 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.subwinDrawing;


import javax.media.opengl.GL;

import org.scilab.modules.renderer.DrawableObjectGL;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.jni.FigureScilabCall;
import org.scilab.modules.renderer.utils.CoordinateTransformation;

/**
 * Class containing functions called by DrawableSubwinJoGL.cpp
 * @author Jean-Baptiste Silvy
 */
public class DrawableSubwinGL extends DrawableObjectGL {
	
	private static final double PIXEL_TO_DEGREE = -0.25;
	
	/**
	 * Default Constructor
	 */
	public DrawableSubwinGL() {
		super();
	}
	
	/**
	 * Specify if the subwin is drawn in 2d or not
	 * @param is2d if true subwindow is considered in 2d mode
	 */
	public void setIs2d(boolean is2d) {
		getParentFigureGL().getCoordinateTransformation().set2dMode(is2d);
	}
	
	/**
	 * Specify the index of the subwindow within other subwindows siblings
	 * @param index index of the subwindow
	 */
	public void setSubwinIndex(int index) {
		GL gl = getGL();
		double nbSubwins = getParentFigureGL().getNbSubwins();
		// the last subwin should be drawn before others
		// so the one with higher index is drawn behind.
		// To acheive this, we put it in the back of the depth range.
		// Consequently first subwin will be put between 0 and 1/nbsubwins
		// last one between (nbsubwinws - 1)/nbsubwins and 1.
		// the depth range is fully initialized.

		CoordinateTransformation transform = getParentFigureGL().getCoordinateTransformation();
		transform.setDepthRange(index / nbSubwins, (index + 1) / nbSubwins);
		
		// default mode, draw in the middle of depth range
		transform.drawMiddle(gl);
	}
	
	/**
	 * Set all the subwin parameters
	 * @param index index of the subwindow
	 * @param is2d if true subwindow is considered in 2d mode
	 */
	public void setSubwinParameters(int index, boolean is2d) {
		setSubwinIndex(index);
		setIs2d(is2d);
	}

	/**
	 * Display the object by displaying its display list
	 * @param parentFigureIndex index of the parent figure in which the object will be drawn
	 */
	public void show(int parentFigureIndex) {
		
	}
	
	/**
	 * Function called at the end of the OpenGL use.
	 */
	public void endDrawing() {
		// back to default
		//getGL().glDepthRange(0.0, 1.0);
		getParentFigureGL().getCoordinateTransformation().setDepthRange(0.0, 1.0);
	}
	
	/**
	 * Perform an interactive rotation of the subwin
	 * @param subwinHandle handle of the subwin
	 */
	public void interactiveRotation(long subwinHandle) {
		
		// get the first click position
		int[] displacement = {0, 0};
		getParentFigureGL().getRendererProperties().getRotationDisplacement(displacement);
		
		// track rotation
		interactiveRotation(subwinHandle, getParentFigureGL());
	}
	
	/**
	 * Perform an interactive rotation of a subwindow
	 * @param subwinHandle handle of the subwin to track
	 * @param trackedCanvas figure on which the displacement is tracked
	 */
	public static void interactiveRotation(long subwinHandle, DrawableFigureGL trackedCanvas) {
		int[] displacement = {0, 0};
		
		// track the rotation
		while (trackedCanvas.getRendererProperties().getRotationDisplacement(displacement)) {
			double deltaAlpha = PIXEL_TO_DEGREE * displacement[1];
			double deltaTheta = PIXEL_TO_DEGREE * displacement[0];
			
			// modify the subwindow viewing angles
			FigureScilabCall.rotateSubwin(subwinHandle, deltaAlpha, deltaTheta);
			
			// redraw the figure with the new angles
			trackedCanvas.drawCanvas();
			
		}
		// the displacement has end or has been canceled
	}

	
}
