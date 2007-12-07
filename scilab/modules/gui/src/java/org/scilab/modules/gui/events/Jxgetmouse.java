package org.scilab.modules.gui.events;

public class Jxgetmouse {

	public static void xgetmouse() {
		xgetmouse(true, false);
	}
	
	public static void xgetmouse(boolean withMotion, boolean withRelease) {
		
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
		return ClickInfos.getInstance().getXCoordinate();
	}
}
