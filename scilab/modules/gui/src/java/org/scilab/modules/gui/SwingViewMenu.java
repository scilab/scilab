/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ACCELERATOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MNEMONIC__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.widget.Widget;

/**
 * @author Vincent COUVERT
 */
public final class SwingViewMenu {

    private static final int COLORS_COEFF = 255;

    /**
     * Constructor
     */
    private SwingViewMenu() {
        throw new UnsupportedOperationException();
    }

    /**
     * Update the component in the view
     * @param uimenu the component
     * @param property the property name
     * @param value the property value
     */
    public static void update(Widget uimenu, int property, Object value) {
        Integer uid = ((SwingViewObject) uimenu).getId();
        switch (property) {
            case __GO_CALLBACK__ :
                int cbType = (Integer) GraphicController.getController().getProperty(uid, __GO_CALLBACKTYPE__);
                uimenu.setCallback(CommonCallBack.createCallback((String) value, cbType, uid));
                break;
            case __GO_CALLBACKTYPE__ :
                String cbString = (String) GraphicController.getController().getProperty(uid, __GO_CALLBACK__);
                uimenu.setCallback(CommonCallBack.createCallback(cbString, (Integer) value, uid));
                break;
            case __GO_UI_CHECKED__ :
                if (uimenu instanceof SwingScilabCheckBoxMenuItem) {
                    ((SwingScilabCheckBoxMenuItem) uimenu).setChecked((Boolean) value);
                }
                break;
            case __GO_UI_ENABLE__ :
                uimenu.setEnabled((Boolean) value);
                break;
            case __GO_VISIBLE__ :
                uimenu.setVisible((Boolean) value);
                break;
            case __GO_UI_FOREGROUNDCOLOR__ :
                Double[] allColors = ((Double[]) value);
                uimenu.setForeground(new Color((int) (allColors[0] * COLORS_COEFF),
                                               (int) (allColors[1] * COLORS_COEFF),
                                               (int) (allColors[2] * COLORS_COEFF)));
                break;
            case __GO_UI_ICON__ :
                if (!((String) value).equals("")) {
                    File file = new File((String)value);
                    if (file.exists() == false) {
                        String filename = FindIconHelper.findImage((String)value);
                        file = new File(filename);
                    }

                    try {
                        BufferedImage icon = ImageIO.read(file);
                        if (uimenu instanceof SwingScilabMenuItem) {
                            ((SwingScilabMenuItem) uimenu).setIcon(new ImageIcon(icon));
                        } else if (uimenu instanceof SwingScilabMenu) {
                            ((SwingScilabMenu) uimenu).setIcon(new ImageIcon(icon));
                        } else if (uimenu instanceof SwingScilabMenu) {
                            ((SwingScilabCheckBoxMenuItem) uimenu).setIcon(new ImageIcon(icon));
                        }
                    } catch (IOException e) {
                    }
                } else {
                    if (uimenu instanceof SwingScilabMenuItem) {
                        ((SwingScilabMenuItem) uimenu).setIcon(null);
                    } else if (uimenu instanceof SwingScilabMenu) {
                        ((SwingScilabMenu) uimenu).setIcon(null);
                    } else if (uimenu instanceof SwingScilabCheckBoxMenuItem) {
                        ((SwingScilabCheckBoxMenuItem) uimenu).setIcon(null);
                    }
                }
                break;
            case __GO_UI_LABEL__ :
                String newText = (String) value;
                String label = newText;

                // Try to set a mnemonic according to text (character preeceded by a &)
                for (int charIndex = 0; charIndex < newText.length(); charIndex++) {
                    if (newText.charAt(charIndex) == '&') {

                        boolean canBeAMnemonic = true;

                        // Previous char must not be a &
                        if ((charIndex != 0) && (newText.charAt(charIndex - 1) == '&')) {
                            canBeAMnemonic = false;
                        }

                        if (canBeAMnemonic && newText.charAt(charIndex + 1) != '&') {
                            // A mnemonic
                            if (uimenu instanceof SwingScilabCheckBoxMenuItem) {
                                ((SwingScilabCheckBoxMenuItem) uimenu).setMnemonic(newText.charAt(charIndex + 1));
                            } else if (uimenu instanceof SwingScilabMenuItem) {
                                ((SwingScilabMenuItem) uimenu).setMnemonic(newText.charAt(charIndex + 1));
                            } else if (uimenu instanceof SwingScilabMenu) {
                                ((SwingScilabMenu) uimenu).setMnemonic(newText.charAt(charIndex + 1));
                            }

                            // Have to remove the & used to set a Mnemonic
                            String firstPart = newText.substring(0, Math.max(charIndex, 0)); // Before &
                            String secondPart = newText.substring(Math.min(charIndex + 1, newText.length()), newText.length()); // After &
                            label = firstPart + secondPart;
                            break;
                        }

                    }
                }

                // Set the text after relacing all && (display a & in the label) by &
                uimenu.setText(label.replaceAll("&&", "&"));
                break;
            case __GO_UI_MNEMONIC__ :
                String mnemonic = (String) value;
                if (uimenu instanceof SwingScilabCheckBoxMenuItem) {
                    ((SwingScilabCheckBoxMenuItem) uimenu).setMnemonic(mnemonic.charAt(0));
                } else if (uimenu instanceof SwingScilabMenuItem) {
                    ((SwingScilabMenuItem) uimenu).setMnemonic(mnemonic.charAt(0));
                } else if (uimenu instanceof SwingScilabMenu) {
                    ((SwingScilabMenu) uimenu).setMnemonic(mnemonic.charAt(0));
                }
                break;
            case __GO_UI_ACCELERATOR__ :
                String accelerator = (String) value;
                if (uimenu instanceof SwingScilabCheckBoxMenuItem) {
                    ((SwingScilabCheckBoxMenuItem) uimenu).setAccelerator(ScilabKeyStroke.getKeyStroke(accelerator));
                } else if (uimenu instanceof SwingScilabMenuItem) {
                    ((SwingScilabMenuItem) uimenu).setAccelerator(ScilabKeyStroke.getKeyStroke(accelerator));
                } else if (uimenu instanceof SwingScilabMenu) {
                    ((SwingScilabMenu) uimenu).setAccelerator(ScilabKeyStroke.getKeyStroke(accelerator));
                }
                break;
        }
    }
}

