package org.scilab.modules.gui.events;

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Bruno Jofret
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/**
 * This class is to manage some Clicks information
 * such as the button/key pressed, the mouse position...
 * @author Bruno Jofret
 */
public final class ClickInfos {

    private static ClickInfos self;

    private int mouseButtonNumber;
    private double xCoordinate;
    private double yCoordinate;
    private Integer windowID;
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
        this.windowID = 0;
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
    public Integer getWindowID() {
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
    public void setWindowID(Integer windowID) {
        this.windowID = windowID;
    }

    /**
     * @param menuCallback the menuCallback to set
     */
    public void setMenuCallback(String menuCallback) {
        this.menuCallback = menuCallback;
    }


}
