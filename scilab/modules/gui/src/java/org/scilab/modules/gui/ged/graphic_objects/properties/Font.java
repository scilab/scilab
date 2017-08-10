/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
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