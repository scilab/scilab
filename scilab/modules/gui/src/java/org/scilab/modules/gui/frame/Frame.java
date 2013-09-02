/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.frame;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.container.Container;
import org.scilab.modules.gui.events.callback.CommonCallBack;

/**
 * Interface for Scilab frames in GUIs
 * @author Vincent COUVERT
 */
public interface Frame extends Container {

    /**
     * Gets this Bridge component object
     * @return this Bridge component object
     */
    SimpleFrame getAsSimpleFrame();

    /**
     * Set the Background color of the Frame.
     * @param color the Color
     */
    void setBackground(Color color);

    /**
     * Get the Background color of the Frame.
     * @return the Color
     */
    Color getBackground();

    /**
     * Set the Foreground color of the Frame.
     * @param color the Color
     */
    void setForeground(Color color);

    /**
     * Get the Foreground color of the Frame.
     * @return the Color
     */
    Color getForeground();

    /**
     * Set the font of the Frame.
     * @param font the font
     */
    void setFont(Font font);

    /**
     * Get the font of the Frame.
     * @return the font
     */
    Font getFont();

    /**
     * Set the text of the Frame
     * @param text the text
     */
    void setText(String text);

    /**
     * Get the text of the Frame
     * @return the text
     */
    String getText();

    /**
     * Set the status of the Frame
     * @param status true if the Widget is enabled
     */
    void setEnabled(boolean status);

    /**
     * Gets the enable status of the Frame
     * @return the enable status of the Frame (true if the Frame is enabled, false if not)
     */
    boolean isEnabled();

    /**
     * Set the callback of the Frame
     * @param callback the callback to set
     */
    void setCallback(CommonCallBack callback);

    /**
     * Set the Relief of the Frame
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    void setRelief(String reliefType);

    /**
     * Delete the Frame
     */
    void destroy();

    /**
     * Give the focus to the Frame
     */
    void requestFocus();

}
