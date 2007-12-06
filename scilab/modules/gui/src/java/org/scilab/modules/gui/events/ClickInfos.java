package org.scilab.modules.gui.events;

public class ClickInfos {
	
	private ClickInfos() {
		init();
	}
	
	public void init() {
		mouseButtonNumber = 0;
		xCoordinate = 0;
		yCoordinate = 0;
		windowID = 0;
		menuCallback = "void";
	}
	
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
	public float getXCoordinate() {
		return xCoordinate;
	}
	/**
	 * @return the yCoordinate
	 */
	public float getYCoordinate() {
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
	public void setXCoordinate(float coordinate) {
		this.xCoordinate = coordinate;
	}

	/**
	 * @param coordinate the yCoordinate to set
	 */
	public void setYCoordinate(float coordinate) {
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
	
	private int mouseButtonNumber;
	private float xCoordinate;
	private float yCoordinate;
	private int windowID;
	private String menuCallback;
	
	private static ClickInfos self;
}
