/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.actions;

import javax.swing.ImageIcon;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.SwingInspector;
import org.scilab.modules.gui.ged.graphic_objects.axes.HideAxes;
import org.scilab.modules.gui.ged.graphic_objects.datatip.HideDatatip;
import org.scilab.modules.gui.ged.graphic_objects.figure.HideFigure;
import org.scilab.modules.gui.ged.graphic_objects.imageplot.HideImageplot;
import org.scilab.modules.gui.ged.graphic_objects.legend.HideLegend;
import org.scilab.modules.gui.ged.graphic_objects.polyline.HidePolyline;
import org.scilab.modules.gui.ged.graphic_objects.surface.HideSurface;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.pushbutton.ScilabPushButton;

/**
 * Show/Hide button class.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class ShowHide extends CommonCallBack {
    private static boolean click = true;
    private static PushButton button;

    /**
     * Constructor.
     *
     * @param name the name of the action.
     */
    public ShowHide(String name) {
        super(name);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callBack() {
        toggleButton();
    }

    /**
     * Create a button for a tool bar.
     *
     * @param title tooltip for the button.
     * @return the button.
     */
    public static PushButton createButton(String title) {
        button = ScilabPushButton.createPushButton();
        ((SwingScilabPushButton) button.getAsSimplePushButton()).addActionListener(new ShowHide(title));
        button.setToolTipText(title);
        setIcon(1);

        return button;
    }

    /**
     * Implements a toggle button.
     */
    public static void toggleButton() {
	if (click) {
            //Hide all sections.
            action(true);
            //Update the icon and tooltip.
            setIcon(0);
            button.setToolTipText(MessagesGED.show);

            click = false;
	} else {
            //Show all sections.
            action(false);
            //Update the icon and tooltip.
            setIcon(1);
            button.setToolTipText(MessagesGED.hide);

            click = true;
	}
    }

    /**
    * Set the Status of Button.
    * @return true = clicked
    */
    public static void setStatus(boolean clicked) {
        click = clicked;
        toggleButton();
    }

    /**
    * Get the Status of Button.
    * @return true = clicked
    */
    public static boolean getStatus() {
        return click;
    }

    /**
     * Action Button.
     *
     * @param hide Boolean.
     */
    public static void action(boolean hide) {
        new HideAxes(hide);
        new HideDatatip(hide);
        new HideFigure(hide);
        new HideImageplot(hide);
        new HideLegend(hide);
        new HidePolyline(hide);
        new HideSurface(hide);
    }

    /**
    * Change the icons on buttons.
    *
    * @param intValue Integer value.
    */
    public static void setIcon(int intValue) {
        switch (intValue) {
            case 0:
                ((SwingScilabPushButton) button.getAsSimplePushButton())
                        .setIcon(new ImageIcon(SwingInspector.icon_expand_all));
                break;
            case 1:
                ((SwingScilabPushButton) button.getAsSimplePushButton())
                        .setIcon(new ImageIcon(SwingInspector.icon_collapse_all));
                break;
            default:
                ((SwingScilabPushButton) button.getAsSimplePushButton())
                        .setIcon(new ImageIcon(SwingInspector.icon_expand_all));
                break;
        }
    }
}