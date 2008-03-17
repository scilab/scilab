/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.events;

import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.SciTranslator;

/**
 * Rubber box whose first conrer is selected by a press and the second a realese
 * @author Jean-Baptiste Silvy
 */
public class PressReleaseRubberBox extends ScilabRubberBox {

	/**
	 * Default constructor
	 * @param selectedCanvas convas on which we will apply the rubberbox
	 */
	public PressReleaseRubberBox(SwingScilabCanvas selectedCanvas) {
		super(selectedCanvas);
	}
	
	/**
	 * Called when mouse button is clicked
	 * @param event click event
	 */
	public void mouseClicked(MouseEvent event) {
		// nothing to do here
		// we just wait for press / release
	}

	/**
	 * Called when the mouse button is pressed
	 * @param event event when the action occured
	 */
	public void mousePressed(MouseEvent event) {
		beginDragging(event.getX(), event.getY(), event.getX(), event.getY());
	}

	/**
	 * Called when the mouse button is realeased
	 * @param event event when the action occured
	 */
	public void mouseReleased(MouseEvent event) {
		// set the exiting button
		// Incompatibility with Scilab 4 here, we don't take keyboerd event into consideration
		setUsedButton(SciTranslator.javaButton2Scilab(event.getButton(), SciTranslator.RELEASED, false));
		endDragging();
	}

}
