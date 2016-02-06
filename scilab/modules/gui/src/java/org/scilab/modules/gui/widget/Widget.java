/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
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

package org.scilab.modules.gui.widget;

import java.awt.Color;
import java.awt.Font;

import org.scilab.modules.gui.dockable.Dockable;
import org.scilab.modules.gui.events.callback.CommonCallBack;

/**
 * Interface for widget associated to objects in Scilab GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public interface Widget extends Dockable {

    /**
     * Set the Background color of the widget.
     * @param color the Color
     */
    void setBackground(Color color);
    void resetBackground();
    /**
     * Get the Background color of the widget.
     * @return the Color
     */
    Color getBackground();

    /**
     * Set the Foreground color of the widget.
     * @param color the Color
     */
    void setForeground(Color color);
    void resetForeground();

    /**
     * Get the Foreground color of the widget.
     * @return the Color
     */
    Color getForeground();

    /**
     * Set the font of the widget.
     * @param font the font
     */
    void setFont(Font font);

    /**
     * Get the font of the widget.
     * @return the font
     */
    Font getFont();

    /**
     * Set the tootip text of the widget
     * @param tooltipText the tooltip text
     */
    void setToolTipText(String tooltipText);

    /**
     * Set the text of the widget
     * @param text the text
     */
    void setText(String text);
    void setEmptyText();

    /**
     * Get the text of the widget
     * @return the text
     */
    String getText();

    /**
     * Set the status of the widget
     * @param status true if the Widget is enabled
     */
    void setEnabled(boolean status);

    /**
     * Gets the enable status of an UIElement
     * @return the enable status of the UIElement (true if the UIElement is enabled, false if not)
     */
    boolean isEnabled();

    /**
     * Set the callback of the widget
     * @param callback the callback to set
     */
    void setCallback(CommonCallBack callback);

    /**
     * Set the horizontal alignment for the widget text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    void setHorizontalAlignment(String alignment);

    /**
     * Set the vertical alignment for the widget text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    void setVerticalAlignment(String alignment);

    /**
     * Set the Relief of the Widget
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    void setRelief(String reliefType);

    /**
     * Delete the Widget
     */
    void destroy();

    /**
     * Give the focus to the Widget
     */
    void requestFocus();
}
