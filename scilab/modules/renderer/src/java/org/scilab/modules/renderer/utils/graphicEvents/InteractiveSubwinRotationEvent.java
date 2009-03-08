/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Digiteo - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils.graphicEvents;

import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * A graphic event designed to use rotations for subwindows only
 * @author Jean-Baptiste Silvy
 */
public class InteractiveSubwinRotationEvent extends InteractiveRotationEvent {

	
	/**
	 * @param trackedCanvas figure on which the rotation will apply
	 * @param subwinHandle of the subwin to rotate
	 */
	public InteractiveSubwinRotationEvent(DrawableFigureGL trackedCanvas, long subwinHandle) {
		super(trackedCanvas);
		setRotatedSubwinHandle(subwinHandle);
	}
	
	/**
	 * Wait for the first click
	 * @return true if the recording should continue, false otherwise
	 */
	protected boolean getFirstClick() {
		// get the first click position
		int[] displacement = {0, 0};
		getTrackedCanvas().getRendererProperties().getRotationDisplacement(displacement);
		return true;
	}
	
}
