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

import javax.swing.DefaultComboBoxModel;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Text properties: TEXT, FONTSTYLE, FONTSIZE, FONTCOLOR, FRACTIONALFONT
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class TextObject extends ContentLayout {
    private String[] text;
    private int index = 0;

    /**
     * Components of the property: Font Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fontColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFontColor = new JLabel();
        JLabel cFontColor = new JLabel();
        JPanel pFontColor = new JPanel();
        JButton bFontColor = new JButton();
        JDialog dialog = new JDialog();
        JColorChooser chooser = new JColorChooser();
        JButton okButton = new JButton();
        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        addColorDialog(dialog, chooser, okButton, cFontColor, UID,
                       parentFigure, "properties.TextObject", "setFontColor", this);

        addLabelColorField(panel, lFontColor, MessagesGED.font_color,
                           dialog, cFontColor, pFontColor, bFontColor,
                           LEFTMARGIN, COLUMN, ROW++);

        //Get the current status of the property: Font Color.
        Integer scilabColor = (Integer) GraphicController.getController()
                              .getProperty(UID, GraphicObjectProperties.__GO_FONT_COLOR__);
        Double[] rgbColor = ColorMapHandler.getRGBcolor(parentFigure, scilabColor);
        cFontColor.setBackground(new Color(rgbColor[0].intValue(),
                                           rgbColor[1].intValue(),
                                           rgbColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public final void setFontColor(int scilabColor, Integer UID) {
        GraphicController.getController().setProperty(
            UID, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }

    /**
     * Components of the property: Font Size
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fontSize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFontSize = new JLabel();
        final JTextField cFontSize = new JTextField();
        addLabelTextField(panel, lFontSize, MessagesGED.font_size,
                          cFontSize, true, LEFTMARGIN, COLUMN, ROW++);
        cFontSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setFontSize(cFontSize, UID);
            }
        });
        cFontSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setFontSize(cFontSize, UID);
            }
        });

        //Get the current status of the property: Font Size.
        cFontSize.setText(Double.toString((Double) GraphicController.getController()
                                          .getProperty(UID, GraphicObjectProperties.__GO_FONT_SIZE__)));
    }

    /**
    * Set Font Size.
    * @param size Font Size.
    */
    private void setFontSize(JTextField cFontSize, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_FONT_SIZE__,
                Double.parseDouble(cFontSize.getText()));
        } catch (NumberFormatException e) {
            cFontSize.setText(Double.toString((Double) GraphicController.getController()
                                              .getProperty(UID, GraphicObjectProperties.__GO_FONT_SIZE__)));
        }
    }

    /**
     * Components of the property: Font Style
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fontStyle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFontStyle = new JLabel();
        final JComboBox cFontStyle = new JComboBox();
        String[] options = new String[] { "Monospaced", "Symbol", "Serif", "Serif Italic", "Serif Bold",
                                          "Serif Bold Italic", "SansSerif", "SansSerif Italic",
                                          "SansSerif Bold", "SansSerif Bold Italic"
                                        };
        addLabelComboBox(panel, lFontStyle, MessagesGED.font_style,
                         cFontStyle, options,
                         LEFTMARGIN, COLUMN, ROW++);
        cFontStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_FONT_STYLE__,
                    cFontStyle.getSelectedIndex());
            }
        });
        //Get the current status of the property: Font Style.
        cFontStyle.setSelectedIndex((Integer) GraphicController.getController()
                                    .getProperty(UID, GraphicObjectProperties.__GO_FONT_STYLE__));
    }

    /**
     * Components of the property: Fractional Font
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fractionalFont(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFractionalFont = new JLabel();
        final JComboBox cFractionalFont = new JComboBox();
        addLabelComboBox(panel, lFractionalFont, MessagesGED.fractional_font,
                         cFractionalFont, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cFractionalFont.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_FONT_FRACTIONAL__,
                    cFractionalFont.getSelectedIndex() == 0 ? false : true);
            }
        });

        // Get the current status of the property: Fractional Font
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_FONT_FRACTIONAL__);
        cFractionalFont.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Text Strings
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void text(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lText = new JLabel();
        final JComboBox cText = new JComboBox();
        addLabelComboBox(panel, lText, MessagesGED.text,
                         cText, null, LEFTMARGIN, COLUMN, ROW++);
        cText.setEditable(true);
        cText.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                if (cText.getSelectedIndex() != -1) {
                    index = cText.getSelectedIndex();
                }
                if ("comboBoxEdited".equals(evt.getActionCommand())) {
                    text[index] = (String) cText.getSelectedItem();
                    cText.setModel(new DefaultComboBoxModel(text));
                    cText.setSelectedIndex(index);
                    GraphicController.getController()
                    .setProperty(UID, GraphicObjectProperties.__GO_TEXT_STRINGS__, text);
                }
            }
        });
        // Get the current status of the property: Text
        text = (String[]) GraphicController.getController()
               .getProperty(UID, GraphicObjectProperties.__GO_TEXT_STRINGS__);
        cText.setModel(new DefaultComboBoxModel(text));
    }
}