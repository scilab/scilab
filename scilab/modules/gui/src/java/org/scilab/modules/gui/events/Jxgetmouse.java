package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * This class implements the xgetmouse Scilab method
 * by updating the ClickInfos attributes.
 * 
 * @author bruno
 *
 */
public final class Jxgetmouse {

	private static int keyChar;
	private static boolean isControlDown;
	private static boolean watchMotion;
	private static boolean watchRelease; 
	private static final int MOVED = -1;

	/**
	 * Tool Class, constructor is private.
	 */
	private Jxgetmouse() { }
	
	/**
	 * Default call for xgetmouse.
	 */
	public static void xgetmouse() {
		xgetmouse(true, false);
	}

	/**
	 * Scilab call point for xgetmouse.
	 *
	 * @param withMotion : Means xgetmouse is mouse motion sensitive.
	 * @param withRelease : Means xgetmouse is release sensitive.
	 */
	public static void xgetmouse(boolean withMotion, boolean withRelease) {
		watchMotion = withMotion;
		watchRelease = withRelease;
		/*
		 *  Enable the local keyActionFilter function
		 *  to be called when a keyEvent is caught.
		 */		
		GlobalEventWatcher.enable(new GlobalKeyEventWatcher() {
			public void keyEventFilter(KeyEvent keyEvent) {
				keyActionFilter(keyEvent);
			}
		});
		long eventMask = AWTEvent.MOUSE_EVENT_MASK;
		if (watchMotion) {
			eventMask += AWTEvent.MOUSE_MOTION_EVENT_MASK;
		}
		/*
		 *  Enable the local mouseActionFilter function
		 *  to be called when a mouseEvent is caught.
		 */		
		GlobalEventWatcher.enable(new GlobalMouseEventWatcher(eventMask)
		{
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
	 * Manage xgetmouse behaviour for keyboard entry.
	 * 1 - Key has been pressed.
	 * 2 - CTRL + Key has been pressed.
	 * 3 - Key has been released
	 * 
	 * @param keyEvent : the Key Event caught
	 */
	private static void keyActionFilter(KeyEvent keyEvent) {	
		/*
		 * If a key is pressed, only remember the key and if CTRL is pressed either.
		 */
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
		} else if (keyEvent.getSource() instanceof SwingScilabCanvas) {
			/*
			 * Now we have have to be sure we are in a Canvas.
			 */
			
			/*
			 * If a RELEASED is seen use -keyChar
			 */
			if (keyEvent.getID() == KeyEvent.KEY_RELEASED) {
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filterKey(-keyChar, isControlDown);
				}
			} else if (keyEvent.getID() == KeyEvent.KEY_TYPED) {
				/*
				 * Or If a TYPED is seen use keyChar
				 */
				if (GlobalEventWatcher.isActivated()) {
					GlobalEventFilter.filterKey(keyChar, isControlDown);
				}	
			}	
		}
	}
	/**
	 * Manage xgetmouse behaviour for mouse entry.
	 * 
	 * @param mouseEvent : the Mouse Event caught
	 * @param scilabMouseAction : the integer scilab code for mouse action.
	 * @param canvas : the canvas where action occurs.
	 * @param isControlDown true if the CTRL key has been pressed
	 */
	private static void mouseActionFilter(MouseEvent mouseEvent, SwingScilabCanvas canvas, int scilabMouseAction, boolean isControlDown) {	
		if (scilabMouseAction != GlobalMouseEventWatcher.MOVED
				&& scilabMouseAction != GlobalMouseEventWatcher.RELEASED) {
			GlobalEventFilter.filterMouse(mouseEvent, canvas, scilabMouseAction, isControlDown);
		} else if (watchMotion && scilabMouseAction == GlobalMouseEventWatcher.MOVED) {
			// Force false value to isControlDown
			// MOVED do not care about CTRL Key...
			GlobalEventFilter.filterMouse(mouseEvent, canvas, MOVED, false);
		} else if (watchRelease && scilabMouseAction == GlobalMouseEventWatcher.RELEASED) {
			// Force false value to isControlDown
			// RELEASED do not care about CTRL Key...
			GlobalEventFilter.filterMouse(mouseEvent, canvas, scilabMouseAction, false);
		}
	}


}

