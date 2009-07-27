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
package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.utils.SciTranslator;

/**
 * This class implements the xclick Scilab method
 * by updating the ClickInfos attributes.
 * @author Bruno Jofret
 */
public final class Jxclick {

	private static int keyChar;

	/**
	 * PRIVATE constructor
	 * We do not want a Jxclick object.
	 */
	private Jxclick() { }

	/**
	 * Scilab call point for xclick.
	 */
	public static void xclick() {
		/*
		 *  Enable the local keyActionFilter function
		 *  to be called when a keyEvent is caught.
		 */	
		GlobalEventWatcher.enable(new GlobalKeyEventWatcher() {
			public void keyEventFilter(KeyEvent keyEvent) {
				keyActionFilter(keyEvent);
			}
		});
		/*
		 *  Enable the local mouseActionFilter function
		 *  to be called when a mouseEvent is caught.
		 */		
		GlobalEventWatcher.enable(new GlobalMouseEventWatcher(AWTEvent.MOUSE_EVENT_MASK) {
			public void mouseEventFilter(MouseEvent mouseEvent,
					SwingScilabAxes axes, int scilabMouseAction, boolean isControlDown) {
				mouseActionFilter(mouseEvent, axes, scilabMouseAction, isControlDown);	
			}
		});
		synchronized (ClickInfos.getInstance()) {
			try {
				ClickInfos.getInstance().init();
				ClickInfos.getInstance().wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		GlobalEventWatcher.disable();
	}

	/**
	 * @return the mouseButtonNumber
	 */
	public static int getMouseButtonNumber() {
		return ClickInfos.getInstance().getMouseButtonNumber();
	}
	/**
	 * @return the xCoordinate
	 */
	public static double getXCoordinate() {
		return ClickInfos.getInstance().getXCoordinate();
	}
	/**
	 * @return the yCoordinate
	 */
	public static double getYCoordinate() {
		return ClickInfos.getInstance().getYCoordinate();
	}
	/**
	 * @return the windowID
	 */
	public static int getWindowID() {
		return ClickInfos.getInstance().getWindowID();
	}
	/**
	 * @return the menuCallback
	 */
	public static String getMenuCallback() {
		return ClickInfos.getInstance().getMenuCallback();
	}

	/**
	 * Manage xclick behaviour for keyboard entry.
	 * 1 - Key has been pressed.
	 * 2 - CTRL + Key has been pressed.
	 * 
	 * @param keyEvent : the Key Event caught
	 */
	private static void keyActionFilter(KeyEvent keyEvent) {	
		if (keyEvent.getID() == KeyEvent.KEY_PRESSED) {
			if (!Character.isJavaIdentifierStart(keyEvent.getKeyChar())) {
				keyChar = keyEvent.getKeyChar(); 
			} 
			else {
				if (keyEvent.isShiftDown()) {
					keyChar = keyEvent.getKeyCode();
				} 
				else {
					keyChar = Character.toLowerCase(keyEvent.getKeyCode());
				}
			}
		} 
		else if (keyEvent.getID() == KeyEvent.KEY_TYPED) {	
			if (keyEvent.getSource() instanceof SwingScilabAxes) {
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filterKey(keyChar, ((SwingScilabAxes)keyEvent.getSource()).getFigureId(),((KeyEvent) keyEvent).isControlDown());
				}
			}
		} 
	}
	/**
	 * Manage xclick behaviour for mouse entry.
	 * 
	 * @param mouseEvent : the Mouse Event caught
	 * @param scilabMouseAction : the integer scilab code for mouse action.
	 * @param axes : the axes where action occurs.
	 * @param isControlDown true if the CTRL key has been pressed
	 */
	private static void mouseActionFilter(MouseEvent mouseEvent, SwingScilabAxes axes, int scilabMouseAction, boolean isControlDown) {	
		if (scilabMouseAction == SciTranslator.PRESSED
				|| scilabMouseAction == SciTranslator.CLICKED
				|| scilabMouseAction == SciTranslator.DCLICKED) {
			GlobalEventFilter.filterMouse(mouseEvent, axes, scilabMouseAction, isControlDown);
		}
	}	
}
