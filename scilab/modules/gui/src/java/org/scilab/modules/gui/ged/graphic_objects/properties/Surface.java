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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.editor.EditorManager;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Surface properties:
* SURFACEMODE, COLORMODE, COLORFLAG, HIDDENCOLOR
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Surface extends ContentLayout {

    /**
     * Components of the property: Color Flag
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void colorFlag(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lColorFlag = new JLabel();
        final JTextField cColorFlag = new JTextField();
        addLabelTextField(panel, lColorFlag, MessagesGED.color_flag,
                          cColorFlag, true, LEFTMARGIN, COLUMN, ROW++);
        cColorFlag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setColorFlag(cColorFlag, UID);
            }
        });
        cColorFlag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setColorFlag(cColorFlag, UID);
            }
        });
        // Get the current status of the property: Color Flag
        cColorFlag.setText(GraphicController.getController()
                           .getProperty(UID, GraphicObjectProperties.__GO_COLOR_FLAG__).toString());
    }

    /**
    * Set Color Flag.
    * @param flag Color flag.
    */
    private void setColorFlag(JTextField cColorFlag, Integer UID) {
        try {
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_COLOR_FLAG__, Integer.parseInt(cColorFlag.getText()));
        } catch (NumberFormatException e) {
            cColorFlag.setText(GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_COLOR_FLAG__).toString());
        }
    }

    /**
     * Components of the property: Color Mode
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void colorMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lColorMode = new JLabel();
        final JTextField cColorMode = new JTextField();
        addLabelTextField(panel, lColorMode, MessagesGED.color_mode,
                          cColorMode, true, LEFTMARGIN, COLUMN, ROW++);
        cColorMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setColorMode(cColorMode, UID);
            }
        });
        cColorMode.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setColorMode(cColorMode, UID);
            }
        });
        // Get the current status of the property: Color Mode
        cColorMode.setText(GraphicController.getController()
                           .getProperty(UID, GraphicObjectProperties.__GO_COLOR_MODE__).toString());
    }

    /**
    * Set Color Mode.
    * @param mode Color Mode.
    * @param UID ObjectID.
    */
    private void setColorMode(JTextField cColorMode, Integer UID) {
        try {
            GraphicController.getController()
            .setProperty(UID, GraphicObjectProperties.__GO_COLOR_MODE__, Integer.parseInt(cColorMode.getText()));
        } catch (NumberFormatException e) {
            cColorMode.setText(GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_COLOR_MODE__).toString());
        }
    }

    /**
     * Components of the property: Hidden Color.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void hiddenColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID, final Integer parentFigure) {
        JLabel label = new JLabel();
        JPanel ipanel = new JPanel();
        JButton bucketButton = new JButton();
        JLabel cHiddenColor = new JLabel();
        JDialog dialog = new JDialog();
        JColorChooser chooserHiddenColor = new JColorChooser();
        JButton okButton = new JButton();

        addColorDialog(dialog, chooserHiddenColor, okButton, cHiddenColor, UID,
                       parentFigure, "properties.Surface", "setHiddenColor", this);

        addLabelColorField(panel, label, MessagesGED.hidden_color,
                           dialog, cHiddenColor, ipanel, bucketButton,
                           LEFTMARGIN, COLUMN, ROW++);

        // Get the current status of the property: Background Color
        Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                                        .getProperty(UID, GraphicObjectProperties.__GO_BACKGROUND__);
        Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
        cHiddenColor.setBackground(new Color(rgbBackgroundColor[0].intValue(),
                                             rgbBackgroundColor[1].intValue(),
                                             rgbBackgroundColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setHiddenColor(int scilabColor, Integer UID) {
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_HIDDEN_COLOR__, scilabColor);
    }

    /**
     * Components of the property: Surface Mode
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void surfaceMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        JLabel lSurfaceMode = new JLabel();
        final JComboBox cSurfaceMode = new JComboBox();
        addLabelComboBox(panel, lSurfaceMode, MessagesGED.surface_mode,
                         cSurfaceMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cSurfaceMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_SURFACE_MODE__,
                    cSurfaceMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Surface Mode
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_SURFACE_MODE__);
        cSurfaceMode.setSelectedIndex(enable ? 1 : 0);
    }
}