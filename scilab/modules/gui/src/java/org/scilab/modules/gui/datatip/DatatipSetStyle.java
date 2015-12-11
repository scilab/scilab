/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.gui.datatip;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.*;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;

import javax.swing.JOptionPane;

/**
 * Set the datatip style - marker, box, label
 * @author Gustavo Barbosa Libotte
 */
public class DatatipSetStyle {

    /**
    * Drag a datatip over a polyline using mouse
    *
    * @param polylineUID Polyline unique identifier.
    * @param t Integer to choose the marker style (1 = square | 2 = arrow).
    * @param boxed Boolean to show/hide datatip box.
    * @param labeled Boolean to show/hide datatip label.
    */
    public static void datatipSetStyle(int polylineUID, int t, boolean boxed, boolean labeled) {
        Integer[] datatipsUID = (Integer[])GraphicController.getController().getProperty(polylineUID, GraphicObjectProperties.__GO_DATATIPS__);
        Integer style = t == 1 ? 11 : 7;
        GraphicController.getController().setProperty(polylineUID, GraphicObjectProperties.__GO_DATATIP_MARK__, style);
        for (int i = 0 ; i < datatipsUID.length ; i++) {
            Integer datatipsType = (Integer) GraphicController.getController().getProperty(datatipsUID[i], __GO_TYPE__);
            if (datatipsType.equals(__GO_DATATIP__)) {
                GraphicController.getController().setProperty(datatipsUID[i], GraphicObjectProperties.__GO_MARK_STYLE__, style);
                GraphicController.getController().setProperty(datatipsUID[i], GraphicObjectProperties.__GO_DATATIP_BOX_MODE__, boxed);
                GraphicController.getController().setProperty(datatipsUID[i], GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__, labeled);
            }
        }
    }

    /**
     * datatipSetStyle function GUI
     *
     * @return Integer related to the choice.
     */
    public static int datatipSetStyleWindow() {

        String datatipSetStyleOption = (String)JOptionPane.showInputDialog(null, "Please choose an option:", "datatipSetStyle Options",
                                       JOptionPane.QUESTION_MESSAGE, null, new Object[] {"Square marker, boxed label",
                                               "Square marker, simple label",
                                               "Square marker, unlabeled",
                                               "Arrow marker, boxed label",
                                               "Arrow marker, simple label",
                                               "Arrow marker, unlabeled"
                                                                                        }, "Square marker, boxed label");

        if (datatipSetStyleOption == "Square marker, boxed label") {
            return 1;
        } else if (datatipSetStyleOption == "Square marker, simple label") {
            return 2;
        } else if (datatipSetStyleOption == "Square marker, unlabeled") {
            return 3;
        } else if (datatipSetStyleOption == "Arrow marker, boxed label") {
            return 4;
        } else if (datatipSetStyleOption == "Arrow marker, simple label") {
            return 5;
        } else if (datatipSetStyleOption == "Arrow marker, unlabeled") {
            return 6;
        } else {
            return 0;
        }
    }
}
