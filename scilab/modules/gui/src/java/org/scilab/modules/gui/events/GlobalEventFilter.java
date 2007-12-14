package org.scilab.modules.gui.events;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;
import org.scilab.modules.gui.utils.UIElementMapper;

/**
 * This class is to manage Events as Scilab used to.
 * Means we need a global overview for a little set of them
 * in particular "event waiting" functions.
 * 
 * @author bruno
 *
 */
public class GlobalEventFilter {

	/**
	 * Update ClickInfos structure when a KeyEvent occurs.
	 * 
	 * @param event : the key event caught.
	 */
	public static void filter(KeyEvent event) {
		synchronized (ClickInfos.getInstance()) {
			ClickInfos.getInstance().setMouseButtonNumber(event.getKeyChar());
			ClickInfos.getInstance().notify();
		}
	}
	
	/**
	 * Update ClickInfos structure when some callback is about to be called.
	 * 
	 * @param event : the event caught.
	 * @param command : the callback that was supposed to be called.
	 */
	public static void filter(ActionEvent event, String command) {
		synchronized (ClickInfos.getInstance()) {
			ClickInfos.getInstance().setMenuCallback(command);
			ClickInfos.getInstance().notify();
		}
	}	
	
	/**
	 * Update ClickInfos structure when a mouse event occurs on a Canvas.
	 * 
	 * @param mouseEvent : the event caught.
	 * @param source : the canvas where the event occurs.
	 * @param buttonCode : the Scilab button code.
	 */
	public static void filter(MouseEvent mouseEvent, SwingScilabCanvas source, int buttonCode) {
		synchronized (ClickInfos.getInstance()) {
			// @TODO : Find a way to get the ID.
			ClickInfos.getInstance().setMouseButtonNumber(buttonCode);
			ClickInfos.getInstance().setWindowID(0);
			ClickInfos.getInstance().notify();
		}
	}
}
