package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;


/**
 * This class implements the xclick Scilab method
 * by updating the ClickInfos attributes.
 * 
 * @author bruno
 *
 */
public final class Jxclick {

	private static int keyChar;
	private static boolean isControlDown;

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
					SwingScilabCanvas canvas, int scilabMouseAction, boolean isControlDown) {
				mouseActionFilter(mouseEvent, canvas, scilabMouseAction, isControlDown);	
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
		// DEBUG
		//System.out.println(keyEvent.toString());
		if (keyEvent.getID() == KeyEvent.KEY_PRESSED) {
			if (Character.isJavaIdentifierStart(keyEvent.getKeyChar())) {
				keyChar = keyEvent.getKeyChar(); 
			} else {
				if (keyEvent.isShiftDown()) {
					keyChar = keyEvent.getKeyCode();
				} else {
					keyChar = Character.toLowerCase(keyEvent.getKeyCode());
				}
			}
			isControlDown = ((KeyEvent) keyEvent).isControlDown();
		} else if (keyEvent.getID() == KeyEvent.KEY_TYPED) {	
			if (keyEvent.getSource() instanceof SwingScilabCanvas) {
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filterKey(keyChar, isControlDown);
				}
			}
		} 
	}
	/**
	 * Manage xclick behaviour for mouse entry.
	 * 
	 * @param mouseEvent : the Mouse Event caught
	 * @param scilabMouseAction : the integer scilab code for mouse action.
	 * @param canvas : the canvas where action occurs.
	 */
	private static void mouseActionFilter(MouseEvent mouseEvent, SwingScilabCanvas canvas, int scilabMouseAction, boolean isControlDown) {	
		if (scilabMouseAction == GlobalMouseEventWatcher.PRESSED
				|| scilabMouseAction == GlobalMouseEventWatcher.CLICKED
				|| scilabMouseAction == GlobalMouseEventWatcher.DCLICKED) {
			GlobalEventFilter.filterMouse(mouseEvent, canvas, scilabMouseAction, isControlDown);
		}
	}	
}
