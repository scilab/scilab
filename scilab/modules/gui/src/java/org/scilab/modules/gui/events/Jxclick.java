package org.scilab.modules.gui.events;

public class Jxclick {

	public static void xclick() {
		initValues();
		GlobalEventWatcher.enable();
	}
	
	private static void initValues() {
		mouseButtonNumber = 0;
		xCoordinate = 0;
		yCoordinate = 0;
		windowID = 0;
		menuCallback = "void";
	}

	/**
	 * @return the mouseButtonNumber
	 */
	public static int getMouseButtonNumber() {
		return mouseButtonNumber;
	}
	/**
	 * @return the xCoordinate
	 */
	public static float getXCoordinate() {
		return xCoordinate;
	}
	/**
	 * @return the yCoordinate
	 */
	public static float getYCoordinate() {
		return yCoordinate;
	}
	/**
	 * @return the windowID
	 */
	public static int getWindowID() {
		return windowID;
	}
	/**
	 * @return the menuCallback
	 */
	public static String getMenuCallback() {
		return menuCallback;
	}

	private static int mouseButtonNumber;
	private static float xCoordinate;
	private static float yCoordinate;
	private static int windowID;
	private static String menuCallback;
}
