package org.scilab.modules.gui.events;


public class Jxclick {

	public static void xclick() {
		GlobalEventWatcher.enable();
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
}
