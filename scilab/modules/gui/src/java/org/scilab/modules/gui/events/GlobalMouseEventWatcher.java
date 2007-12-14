package org.scilab.modules.gui.events;

import java.awt.AWTEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.bridge.canvas.SwingScilabCanvas;

/**
 * This class create a global mouse event watcher
 * means at a very high level in the JVM.
 * 
 * @author bruno
 *
 */
public class GlobalMouseEventWatcher implements AWTEventListener {

	private static final int UNMANAGED = -1;
	private static final int PRESSED = 0;
	private static final int RELEASED = -1;
	private static final int CLICKED = 3;
	private static final int DCLICKED = 10;
	private static final int TIMETOSLEEP = 300;
	
	private boolean freedom = true;
	private int action = UNMANAGED;
	private MouseEvent lastMouse;
	private SwingScilabCanvas canvas;
	
	/**
	 * Constructor.
	 */
	public GlobalMouseEventWatcher() { }

	/** 
	 * 
	 * When there is a mouse click on a Canvas, just go through the GlobalFilter.
	 * @param mouseEvent : The mouse event caught.
	 * 
	 * @see java.awt.event.AWTEventListener#eventDispatched(java.awt.AWTEvent)
	 */
	public void eventDispatched(AWTEvent mouseEvent) {
		ClickInfos.getInstance().setXCoordinate(((MouseEvent) mouseEvent).getPoint().getX());
		ClickInfos.getInstance().setYCoordinate(((MouseEvent) mouseEvent).getPoint().getY());
		ClickInfos.getInstance().setMouseButtonNumber(((MouseEvent) mouseEvent).getButton());
		// Manage Scilab Canvas
		if (mouseEvent.getSource() instanceof SwingScilabCanvas) {
			// DEBUG
			//System.out.println(((MouseEvent) mouseEvent).toString());
			this.lastMouse = (MouseEvent) mouseEvent;
			this.canvas = (SwingScilabCanvas) mouseEvent.getSource();
			if (mouseEvent.getID() == MouseEvent.MOUSE_CLICKED) {
				if (((MouseEvent) mouseEvent).getClickCount() == 1) {
					this.action = CLICKED;
				}		
				else if (((MouseEvent) mouseEvent).getClickCount() == 2) {
					this.action = DCLICKED;
					synchronized (this) {
						this.notify();
					}
				}
				else {
					this.action = UNMANAGED;
					synchronized (this) {
						this.notify();
					}
				}				
			}
			if (mouseEvent.getID() == MouseEvent.MOUSE_PRESSED) {
				this.action = PRESSED;
				if (this.freedom) {
					Thread timer = new Thread() {
						public void run() {launchFilter();} 
					};
					timer.start();
				}			
			}
			if (mouseEvent.getID() == MouseEvent.MOUSE_RELEASED) {
				this.action = RELEASED;
			}
		}
	}
	
	/**
	 * Thread to manage old style scilab
	 * click management.
	 */
	public void launchFilter() {
		this.freedom = false;
		try {
			synchronized (this) {
				wait(TIMETOSLEEP);
			}
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		GlobalEventFilter.filter(lastMouse, canvas, this.action + lastMouse.getButton() - 1);
		this.freedom = true;
		this.action = UNMANAGED;		
	}
	

}
