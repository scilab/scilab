/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.window;

import org.scilab.modules.gui.uielement.UIElement;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.TextBox;

/**
 * Interface for Scilab GUIs windows
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 */
public interface Window extends UIElement {

    /**
     * Gets a much more dummy Objects
     * @return the component.
     */
    SimpleWindow getAsSimpleWindow();

    /**
     * Gets the title of a window
     * @return the title of the window
     */
    String getTitle();

    /**
     * Sets the title of a window
     * @param newWindowTitle the title we want to set for the window
     */
    void setTitle(String newWindowTitle);

    /**
     * Add a tab to a window
     * @param newTab the tab to add to the window
     */
    void addTab(Tab newTab);

    /**
     * Remove a tab from a window
     * @param tab tab to remove
     */
    void removeTab(Tab tab);

    /**
     * Sets a InfoBar to a window
     * @param newInfoBar the Scilab InfoBar to set to the Scilab window
     */
    void addInfoBar(TextBox newInfoBar);

    /**
     * @return number of objects (tabs) docked in this window
     */
    int getNbDockedObjects();

    /**
     * Get a InfoBar from this window
     * @return this window InfoBar
     */
    TextBox getInfoBar();

    /**
     * Deiconify the window and put it in front of other window
     */
    void raise();

    /**
     * Update the dimension of the window and its component.
     * Only useful when the window is not yet visible.
     */
    void updateDimensions();

    /**
    * Deiconified Window
    */
    void windowDeiconified();

    /**
    * Iconified Window
    */
    void windowIconified();

    /**
    * Maximized Window
    */
    void windowMaximized();

    /**
    * Window is in the "normal" state.
    */
    void windowNormal();

}
