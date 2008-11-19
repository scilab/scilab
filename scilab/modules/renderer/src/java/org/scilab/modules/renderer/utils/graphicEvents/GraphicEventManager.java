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
 * Class used to dispatch graphic events
 * @author Jean-Baptiste Silvy
 */
public class GraphicEventManager {

	private GraphicEvent currentEvent;
	private Thread curEnventThread;
	
	/**
	 * Default constructor
	 */
	public GraphicEventManager() {
		currentEvent = null;
		curEnventThread = null;
	}
	
	/**
	 * Remove the current event if one is already running and lauch the new one.
	 * @param event new event to launch
	 */
	public void launchEvent(GraphicEvent event) {
		// first cancel the current event if one
		if (curEnventThread != null && curEnventThread.isAlive()) {
			// cancel the current event
			if (currentEvent.isRunning()) {
				currentEvent.cancel();
			}
			// wait until all actions are done
			// the thread will die
			try {
				curEnventThread.join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
		// launch the new one
		currentEvent = event;
		curEnventThread = new Thread(currentEvent);
		curEnventThread.start();
	}
	
	/**
	 * Launch a zoom event on a figure
	 * @param trackedCanvas figure on which the zoom will apply
	 * @param objectHandle handle of the Figure of Subwin to zoom
	 */
	public void launchZoomEvent(DrawableFigureGL trackedCanvas, long objectHandle) {
		launchEvent(new InteractiveZoomEvent(trackedCanvas, objectHandle));
	}
	
	/**
	 * Launch a rotation event on a figure
	 * @param trackedCanvas figure on which the rotation will apply
	 */
	public void launchRotationEvent(DrawableFigureGL trackedCanvas) {
		launchEvent(new InteractiveRotationEvent(trackedCanvas));
	}
	
	/**
	 * Launch a rotation event on a subwindow
	 * @param trackedCanvas figure on which the rotation will apply
	 * @param subwinHandle of the subwin to rotate
	 */
	public void launchSubwinRotationEvent(DrawableFigureGL trackedCanvas, long subwinHandle) {
		launchEvent(new InteractiveSubwinRotationEvent(trackedCanvas, subwinHandle));
	}
	
	/**
	 * Launch an unzoom event on a subwin
	 * @param subwinHandle handle of the subwin
	 */
	public void launchSubwinUnzoomEvent(long subwinHandle) {
		launchEvent(new SubwinUnzoomEvent(subwinHandle));
	}
	
	
}
