/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos CARDINOT
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
package org.scilab.modules.gui.ged.actions;

import javax.swing.ImageIcon;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.SwingInspector;

/**
 * Show/Hide button class.
 *
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class ShowHide extends CommonCallBack {
    private static boolean click = true;
    private static SwingScilabPushButton button;

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
    public static SwingScilabPushButton createButton(String title) {
        button = new SwingScilabPushButton();
        button.addActionListener(new ShowHide(title));
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
        arc(hide);
        axes(hide);
        champ(hide);
        datatip(hide);
        fec(hide);
        figure(hide);
        imageplot(hide);
        label(hide);
        legend(hide);
        polyline(hide);
        rectangle(hide);
        segs(hide);
        surface(hide);
    }

    public static void arc(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.arc.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.arc.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.arc.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void axes(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.axes.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Box.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Camera.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Tick.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Label.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Position.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.axes.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void champ(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.champ.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.champ.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void datatip(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.datatip.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.datatip.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.datatip.Label.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.datatip.Mark.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void fec(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.fec.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.fec.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void figure(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.figure.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.figure.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.figure.Control.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.figure.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void imageplot(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.imageplot.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.imageplot.DataProperties.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void label(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.label.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.label.Font.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.label.Position.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.label.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void legend(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.legend.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.legend.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.legend.Font.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.legend.Position.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.legend.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void polyline(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.polyline.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.polyline.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.polyline.Mark.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.polyline.Shift.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.polyline.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void rectangle(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.rectangle.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.rectangle.Mark.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.rectangle.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void segs(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.segs.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.segs.Mark.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.segs.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    public static void surface(boolean hide) {
        try {
            org.scilab.modules.gui.ged.graphic_objects.surface.BaseProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.surface.DataProperties.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.surface.Mark.getInstance().setVisibility(!hide);
            org.scilab.modules.gui.ged.graphic_objects.surface.Style.getInstance().setVisibility(!hide);
        } catch (NullPointerException e1) {
        } catch (NoSuchMethodError e2) { }
    }

    /**
     * Check the status of the buttons of all sections.
     * Updates the button's icon in the toolbar
     */
    public static void checkAllButtons(String type) {
        try {
            if ("axes".equals(type)) {
                boolean A = org.scilab.modules.gui.ged.graphic_objects.axes.BaseProperties.getInstance().getStatus();
                boolean B = org.scilab.modules.gui.ged.graphic_objects.axes.Box.getInstance().getStatus();
                boolean C = org.scilab.modules.gui.ged.graphic_objects.axes.Camera.getInstance().getStatus();
                boolean D = org.scilab.modules.gui.ged.graphic_objects.axes.Tick.getInstance().getStatus();
                boolean E = org.scilab.modules.gui.ged.graphic_objects.axes.Label.getInstance().getStatus();
                boolean F = org.scilab.modules.gui.ged.graphic_objects.axes.Position.getInstance().getStatus();
                boolean G = org.scilab.modules.gui.ged.graphic_objects.axes.Style.getInstance().getStatus();
                if (A && B && C && D && E && F && G) {
                    setStatus(false);
                } else if (!A && !B && !C && !D && !E && !F && !G) {
                    setStatus(true);
                }
            } else if ("arc".equals(type)) {
                boolean A = org.scilab.modules.gui.ged.graphic_objects.arc.BaseProperties.getInstance().getStatus();
                boolean B = org.scilab.modules.gui.ged.graphic_objects.arc.DataProperties.getInstance().getStatus();
                boolean C = org.scilab.modules.gui.ged.graphic_objects.arc.Style.getInstance().getStatus();
                if (A && B && C) {
                    setStatus(false);
                } else if (!A && !B && !C) {
                    setStatus(true);
                }
            } else if ("champ".equals(type)) {
                boolean A = org.scilab.modules.gui.ged.graphic_objects.champ.DataProperties.getInstance().getStatus();
                boolean B = org.scilab.modules.gui.ged.graphic_objects.champ.Style.getInstance().getStatus();
                if (A && B) {
                    setStatus(false);
                } else if (!A && !B) {
                    setStatus(true);
                }
            } else if ("datatip".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.datatip.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.datatip.DataProperties.getInstance().getStatus();
                boolean LA = org.scilab.modules.gui.ged.graphic_objects.datatip.Label.getInstance().getStatus();
                boolean MK = org.scilab.modules.gui.ged.graphic_objects.datatip.Mark.getInstance().getStatus();
                if (BP && DP && LA && MK) {
                    setStatus(false);
                } else if (!BP && !DP && !LA && !MK) {
                    setStatus(true);
                }
            } else if ("fec".equals(type)) {
                boolean A = org.scilab.modules.gui.ged.graphic_objects.fec.DataProperties.getInstance().getStatus();
                boolean B = org.scilab.modules.gui.ged.graphic_objects.fec.Style.getInstance().getStatus();
                if (A && B) {
                    setStatus(false);
                } else if (!A && !B) {
                    setStatus(true);
                }
            } else if ("figure".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.figure.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.figure.DataProperties.getInstance().getStatus();
                boolean CT = org.scilab.modules.gui.ged.graphic_objects.figure.Control.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.figure.Style.getInstance().getStatus();
                if (BP && DP && CT && ST) {
                    setStatus(false);
                } else if (!BP && !DP && !CT && !ST) {
                    setStatus(true);
                }
            } else if ("imageplot".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.imageplot.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.imageplot.DataProperties.getInstance().getStatus();
                if (BP && DP) {
                    setStatus(false);
                } else if (!BP && !DP) {
                    setStatus(true);
                }
            } else if ("label".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.label.BaseProperties.getInstance().getStatus();
                boolean FT = org.scilab.modules.gui.ged.graphic_objects.label.Font.getInstance().getStatus();
                boolean PO = org.scilab.modules.gui.ged.graphic_objects.label.Position.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.label.Style.getInstance().getStatus();
                if (BP && FT && PO && ST) {
                    setStatus(false);
                } else if (!BP && !FT && !PO && !ST) {
                    setStatus(true);
                }
            } else if ("legend".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.legend.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.legend.DataProperties.getInstance().getStatus();
                boolean FT = org.scilab.modules.gui.ged.graphic_objects.legend.Font.getInstance().getStatus();
                boolean PO = org.scilab.modules.gui.ged.graphic_objects.legend.Position.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.legend.Style.getInstance().getStatus();
                if (BP && DP && FT && PO && ST) {
                    setStatus(false);
                } else if (!BP && !DP && !FT && !PO && !ST) {
                    setStatus(true);
                }
            } else if ("polyline".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.polyline.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.polyline.DataProperties.getInstance().getStatus();
                boolean MK = org.scilab.modules.gui.ged.graphic_objects.polyline.Mark.getInstance().getStatus();
                boolean SH = org.scilab.modules.gui.ged.graphic_objects.polyline.Shift.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.polyline.Style.getInstance().getStatus();
                if (BP && DP && MK && SH && ST) {
                    setStatus(false);
                } else if (!BP && !DP && !MK && !SH && !ST) {
                    setStatus(true);
                }
            } else if ("rectangle".equals(type)) {
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.rectangle.DataProperties.getInstance().getStatus();
                boolean MK = org.scilab.modules.gui.ged.graphic_objects.rectangle.Mark.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.rectangle.Style.getInstance().getStatus();
                if (DP && MK && ST) {
                    setStatus(false);
                } else if (!DP && !MK && !ST) {
                    setStatus(true);
                }
            } else if ("segs".equals(type)) {
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.surface.DataProperties.getInstance().getStatus();
                boolean MK = org.scilab.modules.gui.ged.graphic_objects.surface.Mark.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.surface.Style.getInstance().getStatus();
                if (DP && MK && ST) {
                    setStatus(false);
                } else if (!DP && !MK && !ST) {
                    setStatus(true);
                }
            } else if ("surface".equals(type)) {
                boolean BP = org.scilab.modules.gui.ged.graphic_objects.surface.BaseProperties.getInstance().getStatus();
                boolean DP = org.scilab.modules.gui.ged.graphic_objects.surface.DataProperties.getInstance().getStatus();
                boolean MK = org.scilab.modules.gui.ged.graphic_objects.surface.Mark.getInstance().getStatus();
                boolean ST = org.scilab.modules.gui.ged.graphic_objects.surface.Style.getInstance().getStatus();
                if (BP && DP && MK && ST) {
                    setStatus(false);
                } else if (!BP && !DP && !MK && !ST) {
                    setStatus(true);
                }
            }
        } catch (NoSuchMethodError e2) { }
    }

    /**
    * Change the icons on buttons.
    *
    * @param intValue Integer value.
    */
    public static void setIcon(int intValue) {
        switch (intValue) {
            case 0:
                button.setIcon(new ImageIcon(SwingInspector.icon_expand_all));
                break;
            case 1:
                button.setIcon(new ImageIcon(SwingInspector.icon_collapse_all));
                break;
            default:
                button.setIcon(new ImageIcon(SwingInspector.icon_expand_all));
                break;
        }
    }
}