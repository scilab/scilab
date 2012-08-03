/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    public static void filterKey(int keyPressed, String figureUID, boolean isControlDown) {
        synchronized (ClickInfos.getInstance()) {
            ClickInfos.getInstance().setMouseButtonNumber(SciTranslator.javaKey2Scilab(keyPressed, isControlDown));
            ClickInfos.getInstance().setWindowID(figureUID);
            ClickInfos.getInstance().setXCoordinate(MouseInfo.getPointerInfo().getLocation().x);
            ClickInfos.getInstance().setYCoordinate(MouseInfo.getPointerInfo().getLocation().y);
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
            ClickInfos.getInstance().setWindowID("");
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
    public static void filterCallback(String command, int returnCode, String figureUID) {
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
    public static void filterMouse(MouseEvent mouseEvent, String axesUID, int buttonAction, boolean isControlDown) {
        if (axesUID != null) {
            synchronized (ClickInfos.getInstance()) {
                ClickInfos.getInstance().setMouseButtonNumber(
                    SciTranslator.javaButton2Scilab(mouseEvent.getButton(), buttonAction, isControlDown)
                );
                ClickInfos.getInstance().setWindowID(axesUID);
                try {
                    ClickInfos.getInstance().setXCoordinate(mouseEvent.getX()
                                                            + ((Component) mouseEvent.getSource()).getLocationOnScreen().getX()
                                                            - ((Component) mouseEvent.getSource()).getLocationOnScreen().getX());
                    ClickInfos.getInstance().setYCoordinate(mouseEvent.getY()
                                                            + ((Component) mouseEvent.getSource()).getLocationOnScreen().getY()
                                                            - ((Component) mouseEvent.getSource()).getLocationOnScreen().getY());
                } catch (Exception e) {
                    ClickInfos.getInstance().setXCoordinate(mouseEvent.getX()
                                                            + ((Component) mouseEvent.getSource()).getX()
                                                            - ((Component) mouseEvent.getSource()).getX());
                    ClickInfos.getInstance().setYCoordinate(mouseEvent.getY()
                                                            + ((Component) mouseEvent.getSource()).getY()
                                                            - ((Component) mouseEvent.getSource()).getY());
                } finally {
                    ClickInfos.getInstance().notify();
                }
            }
        }
    }
}
