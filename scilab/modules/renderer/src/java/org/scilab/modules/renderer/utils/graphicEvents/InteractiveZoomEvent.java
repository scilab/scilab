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
import org.scilab.modules.renderer.jni.FigureScilabCall;

/**
 *  A graphic event designed to use zoom with an area selection 
 * @author Jean-Baptiste Silvy
 */
public class InteractiveZoomEvent extends GraphicEvent {

	private DrawableFigureGL trackedCanvas;
	private long objectHandle;
	
	/**
	 * @param trackedCanvas figure on which the rotation will apply
	 * @param objectHandle handle on the Figure or Subwin to zoom.
	 */
	public InteractiveZoomEvent(DrawableFigureGL trackedCanvas, long objectHandle) {
		this.trackedCanvas = trackedCanvas;
		this.objectHandle = objectHandle;
	}
	
	/**
	 * Stop the zoom.
	 */
	public void cancel() {
		while (trackedCanvas.getRubberBox() == null) {
			// Wait until the rubberbox is created.
			// This should not be long so using a while
			// could be OK.
		}
		if (trackedCanvas.getRubberBox() != null) {
			trackedCanvas.getRubberBox().cancelRubberbox();
		}
	}

	/**
	 * Perform the interactive zoom
	 */
	public void performEvent() {
		
		int[] area = {0, 0, 0, 0};
		// get the zooming area
		// the info message is modified there
		trackedCanvas.getRendererProperties().rubberBox(trackedCanvas.getFigureId(), true, true, null, area);
		
		// zoom
		FigureScilabCall.zoomObject(objectHandle, area[0], area[1], area[2], area[area.length - 1]);
		
		// redraw the figure to see the changes
		trackedCanvas.drawCanvas();
	}

}
