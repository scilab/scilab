/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.gui.events;

import java.awt.Component;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;

import org.scilab.modules.gui.utils.SciTranslator;

/**
 * This class is to manage Events as Scilab used to.
 * Means we need a global overview for a little set of them
 * in particular "event waiting" functions.
 * @author Bruno Jofret
 */
public class GlobalEventFilter {

    private static final int SCILAB_CALLBACK = -2;

    /**
     * Constructor
     */
    protected GlobalEventFilter() {
        throw new UnsupportedOperationException();
    }

    /**
     * Update ClickInfos structure when a KeyEvent occurs.
     *
     * @param keyPressed : the key pressed.
     * @param figureUID Scilab ID of the figure where the even occurred
     * @param isControlDown : is CTRL key modifier activated.
     */
    public static void filterKey(int keyPressed, Integer figureUID, boolean isControlDown, Component source) {
        synchronized (ClickInfos.getInstance()) {
            ClickInfos.getInstance().setMouseButtonNumber(SciTranslator.javaKey2Scilab(keyPressed, isControlDown));
            ClickInfos.getInstance().setWindowID(figureUID);
            ClickInfos.getInstance().setXCoordinate(MouseInfo.getPointerInfo().getLocation().x - source.getLocationOnScreen().getX());
            ClickInfos.getInstance().setYCoordinate(MouseInfo.getPointerInfo().getLocation().y - source.getLocationOnScreen().getY());
            ClickInfos.getInstance().notify();
        }
    }

    /**
     * Update ClickInfos structure when some callback is about to be called.
     *
     * @param command : the callback that was supposed to be called.
     */
    public static void filterCallback(String command) {
        synchronized (ClickInfos.getInstance()) {
            ClickInfos.getInstance().setMouseButtonNumber(SCILAB_CALLBACK);
            ClickInfos.getInstance().setMenuCallback(command);
            ClickInfos.getInstance().setWindowID(0);
            ClickInfos.getInstance().setXCoordinate(-1);
            ClickInfos.getInstance().setYCoordinate(-1);
            ClickInfos.getInstance().notify();
        }
    }

    /**
     * Update ClickInfos structure when some callback is about to be called.
     *
     * @param command : the callback that was supposed to be called.
     * @param returnCode : used for closing windows.
     * @param figureUID : the figure ID where callback occurred.
     */
    public static void filterCallback(String command, int returnCode, Integer figureUID) {
        synchronized (ClickInfos.getInstance()) {
            ClickInfos.getInstance().setMouseButtonNumber(returnCode);
            ClickInfos.getInstance().setMenuCallback(command);
            ClickInfos.getInstance().setWindowID(figureUID);
            ClickInfos.getInstance().setXCoordinate(-1);
            ClickInfos.getInstance().setYCoordinate(-1);
            ClickInfos.getInstance().notify();
        }
    }
    /**
     * Update ClickInfos structure when a mouse event occurs on a Canvas.
     *
     * @param mouseEvent the event caught.
     * @param axesUID the axes where the event occurs.
     * @param buttonAction the Scilab button code mean PRESSED / RELEASED / CLICKED / DCLICKED.
     * @param isControlDown true if the CTRL key has been pressed
     */
    public static void filterMouse(MouseEvent mouseEvent, Integer axesUID, int buttonAction, boolean isControlDown) {
        if (axesUID != null) {
            synchronized (ClickInfos.getInstance()) {
                ClickInfos.getInstance().setMouseButtonNumber(
                    SciTranslator.javaButton2Scilab(mouseEvent.getButton(), buttonAction, isControlDown)
                );
                ClickInfos.getInstance().setWindowID(axesUID);
                try {
                    ClickInfos.getInstance().setXCoordinate(mouseEvent.getX());
                    ClickInfos.getInstance().setYCoordinate(mouseEvent.getY());
                } catch (Exception e) {
                    ClickInfos.getInstance().setXCoordinate(mouseEvent.getX());
                    ClickInfos.getInstance().setYCoordinate(mouseEvent.getY());
                } finally {
                    ClickInfos.getInstance().notify();
                }
            }
        }
    }
}
