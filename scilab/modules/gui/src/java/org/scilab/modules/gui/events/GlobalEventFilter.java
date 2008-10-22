package org.scilab.modules.gui.events;

import java.awt.MouseInfo;
import java.awt.event.MouseEvent;

import javax.swing.JComponent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.SciTranslator;

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno Jofret
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/**
 * This class is to manage Events as Scilab used to.
 * Means we need a global overview for a little set of them
 * in particular "event waiting" functions.
 * @author Bruno Jofret
 */
public class GlobalEventFilter {

    private static final int SCILAB_CALLBACK = -2;

    /**
     * Constructor
     */
    protected GlobalEventFilter() {
	throw new UnsupportedOperationException();
    }

    /**
     * Update ClickInfos structure when a KeyEvent occurs.
     * 
     * @param keyPressed : the key pressed.
     * @param figureID Scilab ID of the figure where the even occurred
     * @param isControlDown : is CTRL key modifier activated.
     */
    public static void filterKey(int keyPressed, int figureID, boolean isControlDown) {
	synchronized (ClickInfos.getInstance()) {
	    ClickInfos.getInstance().setMouseButtonNumber(SciTranslator.javaKey2Scilab(keyPressed, isControlDown));
	    ClickInfos.getInstance().setWindowID(figureID);
	    ClickInfos.getInstance().setXCoordinate(MouseInfo.getPointerInfo().getLocation().x);
	    ClickInfos.getInstance().setYCoordinate(MouseInfo.getPointerInfo().getLocation().y);
	    ClickInfos.getInstance().notify();
	}
    }

    /**
     * Update ClickInfos structure when some callback is about to be called.
     * 
     * @param command : the callback that was supposed to be called.
     */
    public static void filterCallback(String command) {
	synchronized (ClickInfos.getInstance()) {
	    ClickInfos.getInstance().setMouseButtonNumber(SCILAB_CALLBACK);
	    ClickInfos.getInstance().setMenuCallback(command);
	    ClickInfos.getInstance().setWindowID(-1);
	    ClickInfos.getInstance().setXCoordinate(-1);
	    ClickInfos.getInstance().setYCoordinate(-1);
	    ClickInfos.getInstance().notify();
	}
    }	

    /**
     * Update ClickInfos structure when some callback is about to be called.
     * 
     * @param command : the callback that was supposed to be called.
     * @param returnCode : used for closing windows.
     * @param figureIndex : the figure ID where callback occured.
     */
    public static void filterCallback(String command, int returnCode, int figureIndex) {
	synchronized (ClickInfos.getInstance()) {
	    ClickInfos.getInstance().setMouseButtonNumber(returnCode);
	    ClickInfos.getInstance().setMenuCallback(command);
	    ClickInfos.getInstance().setWindowID(figureIndex);
	    ClickInfos.getInstance().setXCoordinate(-1);
	    ClickInfos.getInstance().setYCoordinate(-1);
	    ClickInfos.getInstance().notify();
	}
    }		
    /**
     * Update ClickInfos structure when a mouse event occurs on a Canvas.
     * 
     * @param mouseEvent the event caught.
     * @param source the canvas where the event occurs.
     * @param buttonAction the Scilab button code mean PRESSED / RELEASED / CLICKED / DCLICKED.
     * @param isControlDown true if the CTRL key has been pressed
     */
    public static void filterMouse(MouseEvent mouseEvent, SwingScilabCanvas source, int buttonAction, boolean isControlDown) {
	if (source != null) {	
	    synchronized (ClickInfos.getInstance()) {
		ClickInfos.getInstance().setXCoordinate(mouseEvent.getX() 
			+ ((JComponent) mouseEvent.getSource()).getLocationOnScreen().getX() 
			- ((JComponent) source).getLocationOnScreen().getX());
		ClickInfos.getInstance().setYCoordinate(mouseEvent.getY() 
			+ ((JComponent) mouseEvent.getSource()).getLocationOnScreen().getY() 
			- ((JComponent) source).getLocationOnScreen().getY());
		ClickInfos.getInstance().setMouseButtonNumber(
			SciTranslator.javaButton2Scilab(mouseEvent.getButton(), buttonAction, isControlDown)
		);
		ClickInfos.getInstance().setWindowID(source.getFigureIndex());
		ClickInfos.getInstance().notify();
	    }
	}
    }
}
