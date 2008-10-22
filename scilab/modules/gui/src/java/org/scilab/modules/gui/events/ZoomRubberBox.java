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
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * Rubber box specialized in zooming.
 * Basically its a rubber box but the cursor is changed
 * @author Jean-Baptiste Silvy
 */
public class ZoomRubberBox extends ClickRubberBox {

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
		
		// set zoom cursor
		Image icon = Toolkit.getDefaultToolkit().getImage(System.getenv("SCI") + "/modules/gui/images/icons/zoom-area-cursor.png");
		getSelectedCanvas().setCursor(Toolkit.getDefaultToolkit().createCustomCursor(icon, new Point(3, 4), "zoom-area"));
		//selectedCanvas.setCursor(Cursor.getPredefinedCursor(Cursor.CROSSHAIR_CURSOR));
		int res = super.getRectangle(initialRect, endRect);
		
		// restore default cursor
		getSelectedCanvas().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
		
		return res;
		
	}
	
}
