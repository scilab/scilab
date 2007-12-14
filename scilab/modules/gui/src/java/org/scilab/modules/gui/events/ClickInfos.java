package org.scilab.modules.gui.events;

/**
 * This class is to manage some Clicks informations
 * such as the button/key pressed, the mouse position...
 * 
 * @author bruno
 *
 */
public final class ClickInfos {

	private static ClickInfos self;
	
	private int mouseButtonNumber;
	private double xCoordinate;
	private double yCoordinate;
	private int windowID;
	private String menuCallback;	

	/**
	 * SINGLETON
	 */
	private ClickInfos() {
		init();
	}
	
	/**
	 * Initialise ClickInfos attributes.
	 */
	public void init() {
		mouseButtonNumber = 0;
		xCoordinate = 0;
		yCoordinate = 0;
		windowID = 0;
		menuCallback = "void";
	}
	
	/**
	 * Singleton.
	 * @return the unique ClickInfos
	 */
	public static ClickInfos getInstance() {
		if (self == null) {
			self = new ClickInfos();
		}
		return self;
	}
	
	/**
	 * @return the mouseButtonNumber
	 */
	public int getMouseButtonNumber() {
		return mouseButtonNumber;
	}
	/**
	 * @return the xCoordinate
	 */
	public double getXCoordinate() {
		return xCoordinate;
	}
	/**
	 * @return the yCoordinate
	 */
	public double getYCoordinate() {
		return yCoordinate;
	}
	/**
	 * @return the windowID
	 */
	public int getWindowID() {
		return windowID;
	}
	/**
	 * @return the menuCallback
	 */
	public String getMenuCallback() {
		return menuCallback;
	}

	/**
	 * @param mouseButtonNumber the mouseButtonNumber to set
	 */
	public void setMouseButtonNumber(int mouseButtonNumber) {
		this.mouseButtonNumber = mouseButtonNumber;
	}

	/**
	 * @param coordinate the xCoordinate to set
	 */
	public void setXCoordinate(double coordinate) {
		this.xCoordinate = coordinate;
	}

	/**
	 * @param coordinate the yCoordinate to set
	 */
	public void setYCoordinate(double coordinate) {
		this.yCoordinate = coordinate;
	}

	/**
	 * @param windowID the windowID to set
	 */
	public void setWindowID(int windowID) {
		this.windowID = windowID;
	}

	/**
	 * @param menuCallback the menuCallback to set
	 */
	public void setMenuCallback(String menuCallback) {
		this.menuCallback = menuCallback;
	}
	

}
