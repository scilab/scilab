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

import org.scilab.modules.renderer.jni.FigureScilabCall;

/**
 * Event unzooming a subwin
 * @author Jean-Baptiste Silvy
 */
public class SubwinUnzoomEvent extends GraphicEvent {

	private long subwinHandle;
	
	/**
	 * @param subwinHandle handle of the subwin
	 */
	public SubwinUnzoomEvent(long subwinHandle) {
		super();
		this.subwinHandle = subwinHandle;
	}
	
	/**
	 * Cancel the unzoom.
	 */
	public void cancel() {
		// actually the unzoom is be quite fast compared to other event, so it's not needed to
		// cancel it
	}

	/**
	 * Actually do the unzoom
	 */
	public void performEvent() {
		FigureScilabCall.unzoomSubwinHandle(subwinHandle);
	}

}
