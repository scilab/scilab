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

import org.scilab.modules.localization.Messages;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;
import org.scilab.modules.renderer.FigureScilabCall;

/**
 * A graphic event designed to use rotations for figures
 * @author Jean-Baptiste Silvy
 */
public class InteractiveRotationEvent extends GraphicEvent {

	private static final double PIXEL_TO_DEGREE = -0.25;
	
	private DrawableFigureGL trackedCanvas;
	private String curInfoMessage;
	private long subwinHandle;
	
	/**
	 * @param trackedCanvas figure on which the rotation will apply
	 */
	public InteractiveRotationEvent(DrawableFigureGL trackedCanvas) {
		this.trackedCanvas = trackedCanvas;
		setRotatedSubwinHandle(0);
	}
	
	/**
	 * Cancel the rotation
	 */
	public void cancel() {
		trackedCanvas.getRendererProperties().stopRotationRecording();
	}

	/**
	 * Start the rotation
	 */
	public void performEvent() {
		// get the current info message
		curInfoMessage = trackedCanvas.getInfoMessage();
		
		if (getFirstClick()) {
			interactiveRotation(getRotatedSubwinHandle());
		}
		
		trackedCanvas.setInfoMessage(curInfoMessage);
	}
	
	/**
	 * Wait for the first click
	 * @return true if the recording should continue, false otherwise
	 */
	protected boolean getFirstClick() {
		// set a new infoMessage
		trackedCanvas.setInfoMessage(Messages.gettext("Click on an Axes object to start rotation. Right click or ESCAPE to cancel."));
		
		// first get the clicked subwin
		int[] clickPos = {0, 0};
		boolean keepOnRecording = trackedCanvas.getRendererProperties().getRotationDisplacement(clickPos);
		
		if (!keepOnRecording) {
			// recording has been canceled
			return false;
		}
		
		// get the clicked subwin handle or 0 if no handle has been clicked
		long clickedSubwinHandle = FigureScilabCall.getClickedSubwinHandle(trackedCanvas.getFigureId(), clickPos[0], clickPos[1]);
		
		if (clickedSubwinHandle == 0) {
			// no subwindow has been founds
			cancel();
			
			// to be sure clean everything
			trackedCanvas.getRendererProperties().getRotationDisplacement(clickPos);
			return false;
		}
		
		trackedCanvas.setInfoMessage(Messages.gettext("Click or press ESCAPE to terminate rotation."));
		
		setRotatedSubwinHandle(clickedSubwinHandle);
		
		
		
		return true;
	}
	
	/**
	 * Perform an interactive rotation of a subwindow
	 * @param subwinHandle handle of the subwin to track
	 */
	protected void interactiveRotation(long subwinHandle) {
		
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
	}
	
	/**
	 * @return trackedCanvas
	 */
	protected DrawableFigureGL getTrackedCanvas() {
		return trackedCanvas;
	}
	
	/**
	 * @return handle of the rotated subwindow
	 */
	protected long getRotatedSubwinHandle() {
		return subwinHandle;
	}
	
	/**
	 * @param subwinHandle handle of the subwindow to rotate
	 */
	protected void setRotatedSubwinHandle(long subwinHandle) {
		this.subwinHandle = subwinHandle;
	}

}
