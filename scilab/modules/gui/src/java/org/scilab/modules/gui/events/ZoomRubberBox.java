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

import java.awt.Cursor;

import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.MouseEvent;


import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.SciTranslator;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.renderer.FigureMapper;
import org.scilab.modules.renderer.figureDrawing.DrawableFigureGL;

/**
 * Rubber box specialized in zooming.
 * Basically its a rubber box but the cursor is changed
 * @author Jean-Baptiste Silvy
 */
public class ZoomRubberBox extends ClickRubberBox implements FocusListener {

	private static final String ICON_PATH = System.getenv("SCI") + "/modules/gui/images/icons/zoom-area-cursor.png";
	private static final String CURSOR_ICON_NAME = "zoom-area";
	
	private DrawableFigureGL zoomedFigure;
	
	/**
	 * Constructor
	 * @param selectedCanvas Canvas on which the rubberbox will be added
	 */
	public ZoomRubberBox(SwingScilabCanvas selectedCanvas) {
		super(selectedCanvas);
	}
	
	/**
	 * Call the rubber box on the canvas
	 * @param initialRect if not null specify the initial rectangle to draw
	 * @param endRect array [x1,y1,x2,y2] containing the result of rubberbox
	 * @return Scilab code of the pressed button
	 */
	public int getRectangle(int[] initialRect, int[] endRect) {
		
		zoomedFigure = FigureMapper.getCorrespondingFigure(getSelectedCanvas().getFigureIndex());
		String currentInfoMessage = zoomedFigure.getInfoMessage();
		zoomedFigure.setInfoMessage(Messages.gettext("Left click to start selection of the zooming area. Right click to cancel."));
		
		// set the zooming cursor
		getSelectedCanvas().setCursor(ScilabSwingUtilities.createCursorFromIcon(ICON_PATH, CURSOR_ICON_NAME));
		
		// to be able to know when the canvas is losing focus and then cancel the rubberbox
		getSelectedCanvas().addFocusListener(this);
		
		int res = super.getRectangle(initialRect, endRect);
		
		getSelectedCanvas().removeFocusListener(this);
		
		// restore default cursor
		getSelectedCanvas().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
		
		// reset info message
		zoomedFigure.setInfoMessage(currentInfoMessage);
		
		return res;
		
	}
	
	/**
	 * Called when the mouse button is pressed
	 * Override the function to be able to cancel recording when right mouse button is pressed
	 * @param event event when the action occurs
	 */
	public void mousePressed(MouseEvent event) {
		if (event.getButton() == MouseEvent.BUTTON1) {
			// confirmation button do as usual
			// change info message
			zoomedFigure.setInfoMessage(Messages.gettext("Left click to terminate selection. Right click to cancel."));
			super.mousePressed(event);
		} else {
			// cancel operation
			setUsedButton(SciTranslator.javaButton2Scilab(event.getButton(), SciTranslator.PRESSED, false));
			// set an empty box to specify that the selection failed
			setEmptySelection();
			endDragging();
		}
	}

	/**
	 * @param event focus gained event
	 */
	public void focusGained(FocusEvent event) {
		// nothing to do here
		// canvas must always have focus during the recording
	}

	/**
	 * This event occurs when the canvas lost focus but
	 * also when the windows is closed. We then need to wake up every one.
	 * @param event focus lost event
	 */
	public void focusLost(FocusEvent event) {
		// focus lost so stop recording
		// do only this if the opposite component is not the canvas itself
		if (event.getOppositeComponent() != getSelectedCanvas().getAsComponent()) {
			cancelRubberbox();
		}
	}
	
}
