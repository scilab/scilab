/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.utils;

/**
 * This class aims at converting Java constants that stay
 * as user input : keychar, mouseclick, pressed, released...
 * into scilab "standard" integer encryption.
 * @author bruno
 *
 */
public class SciTranslator {
    /**
     * Those static final constant are only for reverse compatibility
     * With old Scilab.
     * Java button are :
     * 1 = left
     * 2 = middle
     * 3 = right
     * We add those statics state integers such as
     * button + state = scilab return value.
     *
     * ex : left PRESSED = 1 + (-1) = 0
     *
     * MOVE does not feat the rule because
     * it does not have any Button associated with.
     */
    /** Internal state PRESSED */
    public static final int PRESSED = -1;
    /** Internal state RELEASED */
    public static final int RELEASED = -6;
    /** Internal state CLICKED */
    public static final int CLICKED = 2;
    /** Internal state DCLICKED */
    public static final int DCLICKED = 9;
    /**
     * Internal state SCIMOVED
     * differs from move because otherwise
     * other clicks can raise such a code
     */
    public static final int SCIMOVED = -1;
    /** Internal state CLOSE */
    public static final int SCICLOSE = -1000;

    /** Internal state MOVED */
    public static final int MOVED = -1000;
    /** Internal initial state */
    public static final int UNMANAGED = -10000;

    private static final int SCILAB_CTRL_OFFSET = 1000;

    private static final int TIMETOSLEEP = 300;

    private int clickAction = UNMANAGED;

    public SciTranslator() {
        clickAction = UNMANAGED;
    }

    public static int javaKey2Scilab(int keyPressed, boolean isControlDown) {
        int keyCode = keyPressed;
        if (isControlDown) {
            keyCode += SCILAB_CTRL_OFFSET;
        }
        return keyCode;
    }

    public static int javaButton2Scilab(int mouseEventButton, int buttonAction, boolean isControlDown) {
        int buttonCode = mouseEventButton + buttonAction;
        if (isControlDown) {
            buttonCode += SCILAB_CTRL_OFFSET;
        }
        return buttonCode;
    }

    public void setClickAction(int newClickAction) {
        clickAction = newClickAction;
    }

    public int getClickAction() {
        return clickAction;
    }

    public int javaClick2Scilab() {
        try {
            synchronized (this) {
                wait(TIMETOSLEEP);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return getClickAction();
    }
}
