package org.scilab.modules.gui.events;

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Bruno Jofret
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/**
 * This class is to manage some Clicks informations
 * such as the button/key pressed, the mouse position...
 * @author Bruno Jofret
 */
public final class ClickInfos {

	private static ClickInfos self;
	
	private int mouseButtonNumber;
	private double xCoordinate;
	private double yCoordinate;
	private String windowID;
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
		this.mouseButtonNumber = 0;
		this.xCoordinate = 0;
		this.yCoordinate = 0;
		this.windowID = "";
		this.menuCallback = "void";
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
	public String getWindowID() {
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
	public void setWindowID(String windowID) {
		this.windowID = windowID;
	}

	/**
	 * @param menuCallback the menuCallback to set
	 */
	public void setMenuCallback(String menuCallback) {
		this.menuCallback = menuCallback;
	}
	

}
