/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.properties;

import java.awt.Color;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Font properties: STYLE, SIZE, COLOR, FRACTIONAL
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Font extends ContentLayout {

    /**
     * Components of the property: Font Color.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void fontColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID, final Integer parentFigure) {
        JLabel lFontColor = new JLabel();
        JPanel pFontColor = new JPanel();
        JButton bFontColor = new JButton();
        JLabel cFontColor = new JLabel();
        JDialog dialogFontColor = new JDialog();
        JColorChooser chooserFontColor = new JColorChooser();
        JButton okFontColor = new JButton();

        addColorDialog(dialogFontColor, chooserFontColor, okFontColor, cFontColor, UID,
                       parentFigure, "properties.Font", "setFontColor", this);

        addLabelColorField(panel, lFontColor, MessagesGED.font_color,
                           dialogFontColor, cFontColor, pFontColor, bFontColor,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Font Color
        Integer scilabFontColor = (Integer) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_FONT_COLOR__);
        Double[] rgbFontColor = ColorMapHandler.getRGBcolor(parentFigure, scilabFontColor);
        cFontColor.setBackground(new Color(rgbFontColor[0].intValue(),
                                           rgbFontColor[1].intValue(),
                                           rgbFontColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public final void setFontColor(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }
}