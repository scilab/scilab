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
import org.scilab.modules.gui.datatip.DatatipManagerMode;
import org.scilab.modules.gui.editor.EditorManager;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.customComponents.JComboBox.MarkStyleRenderer;
import org.scilab.modules.gui.ged.graphic_objects.contouredObject.MarkStyle;

/**
* ContouredObject properties: LINE, FILLMODE, BACKGROUND, MARK
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class ContouredObject extends ContentLayout {
    JComboBox cMarkMode;

    /**
     * Components of the property: Background Color.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void backgroundColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID, final Integer parentFigure) {
        JLabel lBackground = new JLabel();
        JPanel pBackground = new JPanel();
        JButton bBackground = new JButton();
        JLabel cBackground = new JLabel();
        JDialog dialogBackground = new JDialog();
        JColorChooser chooserBackground = new JColorChooser();
        JButton okBackground = new JButton();

        addColorDialog(dialogBackground, chooserBackground, okBackground, cBackground, UID,
                       parentFigure, "properties.ContouredObject", "setBackgroundColor", this);

        addLabelColorField(panel, lBackground, MessagesGED.background_color,
                           dialogBackground, cBackground, pBackground, bBackground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Background Color
        Integer scilabBackgroundColor = (Integer) GraphicController.getController()
                                        .getProperty(UID, GraphicObjectProperties.__GO_BACKGROUND__);
        Double[] rgbBackgroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabBackgroundColor);
        cBackground.setBackground(new Color(rgbBackgroundColor[0].intValue(),
                                            rgbBackgroundColor[1].intValue(),
                                            rgbBackgroundColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    * @param UID objectID.
    */
    public void setBackgroundColor(int scilabColor, Integer UID) {
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }

    /**
     * Components of the property: Fill Mode
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void fillMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lFillMode = new JLabel();
        final JComboBox cFillMode = new JComboBox();
        addLabelComboBox(panel, lFillMode, MessagesGED.fill_mode,
                         cFillMode, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cFillMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_FILL_MODE__,
                    cFillMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Fill Mode
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_FILL_MODE__);
        cFillMode.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Line Color
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void lineColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN,
                          final Integer UID, final Integer parentFigure, String packClass, Object methobj) {
        JLabel lForeground = new JLabel();
        JPanel pForeground = new JPanel();
        JButton bForeground = new JButton();
        JLabel cForeground = new JLabel();
        JDialog dialogForeground = new JDialog();
        JColorChooser chooserForeground = new JColorChooser();
        JButton okForeground = new JButton();

        addColorDialog(dialogForeground, chooserForeground, okForeground, cForeground, UID,
                       parentFigure, packClass, "setForegroundColor", methobj);

        addLabelColorField(panel, lForeground, MessagesGED.foreground_color,
                           dialogForeground, cForeground, pForeground, bForeground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Line Color
        Integer scilabForegroundColor = (Integer) GraphicController.getController()
                                        .getProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__);
        Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
        cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                            rgbForegroundColor[1].intValue(),
                                            rgbForegroundColor[2].intValue()));
    }

    /**
     * Components of the property: Foreground Color (Line Color).
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void foregroundColor(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN,
                                final Integer UID, final Integer parentFigure) {
        JLabel lForeground = new JLabel();
        JPanel pForeground = new JPanel();
        JButton bForeground = new JButton();
        JLabel cForeground = new JLabel();
        JDialog dialogForeground = new JDialog();
        JColorChooser chooserForeground = new JColorChooser();
        JButton okForeground = new JButton();

        addColorDialog(dialogForeground, chooserForeground, okForeground, cForeground, UID,
                       parentFigure, "properties.ContouredObject", "setForegroundColor", this);

        addLabelColorField(panel, lForeground, MessagesGED.foreground_color,
                           dialogForeground, cForeground, pForeground, bForeground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Foreground Color
        Integer scilabForegroundColor;
        scilabForegroundColor = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__);

        Double[] rgbForegroundColor = ColorMapHandler.getRGBcolor(parentFigure, scilabForegroundColor);
        cForeground.setBackground(new Color(rgbForegroundColor[0].intValue(),
                                            rgbForegroundColor[1].intValue(),
                                            rgbForegroundColor[2].intValue()));
    }

    /**
    * Change the color of the object.
    * @param scilabColor index of the color map.
    */
    public void setForegroundColor(int scilabColor, Integer UID) {
        Integer parentFigure = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }

    /**
     * Components of the property: Line Mode
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void lineMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lLineMode = new JLabel();
        final JComboBox cLineMode = new JComboBox();
        addLabelComboBox(panel, lLineMode, MessagesGED.line_mode,
                         cLineMode, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cLineMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_LINE_MODE__,
                    cLineMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Line Mode
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_LINE_MODE__);
        cLineMode.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Line Style
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void lineStyle(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lLine = new JLabel();
        final JComboBox cLine = new JComboBox();
        String[] options = new String[] {MessagesGED.dash, MessagesGED.dash_dot, MessagesGED.longdash_dot,
                                         MessagesGED.bigdash_dot, MessagesGED.bigdash_longdash, MessagesGED.dot, MessagesGED.double_dot
                                        };
        addLabelComboBox(panel, lLine, MessagesGED.line_style,
                         cLine, options,
                         LEFTMARGIN, COLUMN, ROW++);
        cLine.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_LINE_STYLE__,
                    cLine.getSelectedIndex() + 1);
            }
        });
        // Get the current status of the property: Line Style
        int currentLineStyle = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_LINE_STYLE__);
        cLine.setSelectedIndex(currentLineStyle - 1);
    }

    /**
     * Components of the property: Mark Background Color.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void markBackgroundColor(JPanel panel, MarkStyleRenderer markStyleRenderer, JColorChooser chooserMarkBackground,
                                    int ROW, int COLUMN, int LEFTMARGIN,
                                    final Integer UID, final Integer parentFigure, String packClass, Object methobj) {
        JLabel lMarkBackground = new JLabel();
        JPanel pMarkBackground = new JPanel();
        JButton bMarkBackground = new JButton();
        JLabel cMarkBackground = new JLabel();
        JDialog dialogMarkBackground = new JDialog();
        JButton okMarkBackground = new JButton();

        addColorDialog(dialogMarkBackground, chooserMarkBackground, okMarkBackground, cMarkBackground,
                       UID, parentFigure, packClass, "setMarkBackground", methobj);

        addLabelColorField(panel, lMarkBackground, MessagesGED.mark_background,
                           dialogMarkBackground, cMarkBackground, pMarkBackground, bMarkBackground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Mark Background Color
        Integer scilabMarkBackground = -1;
        String type[] = packClass.split("\\.");
        if ("datatip".equals(type[0])) {
            scilabMarkBackground = DatatipManagerMode.getInstance().getMarkColor();
        } else {
            scilabMarkBackground = (Integer) GraphicController.getController()
                                   .getProperty(UID, GraphicObjectProperties.__GO_MARK_BACKGROUND__);
        }
        Double[] rgbMarkBackground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkBackground);
        Color markBackgroundCOLOR = new Color(rgbMarkBackground[0].intValue(),
                                              rgbMarkBackground[1].intValue(),
                                              rgbMarkBackground[2].intValue());
        cMarkBackground.setBackground(markBackgroundCOLOR);
        markStyleRenderer.setMarkBackground(markBackgroundCOLOR);
    }

    /**
     * Components of the property: Mark Foreground Color.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void markForegroundColor(JPanel panel, MarkStyleRenderer markStyleRenderer, JColorChooser chooserMarkForeground,
                                    int ROW, int COLUMN, int LEFTMARGIN,
                                    final Integer UID, final Integer parentFigure, String packClass, Object methobj) {
        JLabel lMarkForeground = new JLabel();
        JPanel pMarkForeground = new JPanel();
        JButton bMarkForeground = new JButton();
        JLabel cMarkForeground = new JLabel();
        JDialog dialogMarkForeground = new JDialog();
        JButton okMarkForeground = new JButton();

        addColorDialog(dialogMarkForeground, chooserMarkForeground, okMarkForeground, cMarkForeground,
                       UID, parentFigure, packClass, "setMarkForeground", methobj);

        addLabelColorField(panel, lMarkForeground, MessagesGED.mark_foreground,
                           dialogMarkForeground, cMarkForeground, pMarkForeground, bMarkForeground,
                           LEFTMARGIN, COLUMN, ROW);

        // Get the current status of the property: Mark Foreground Color
        Integer scilabMarkForeground = -1;
        String type[] = packClass.split("\\.");
        scilabMarkForeground = (Integer) GraphicController.getController().getProperty(UID, GraphicObjectProperties.__GO_MARK_FOREGROUND__);
        Double[] rgbMarkForeground = ColorMapHandler.getRGBcolor(parentFigure, scilabMarkForeground);
        Color markForegroundCOLOR = new Color(rgbMarkForeground[0].intValue(),
                                              rgbMarkForeground[1].intValue(),
                                              rgbMarkForeground[2].intValue());
        cMarkForeground.setBackground(markForegroundCOLOR);
        markStyleRenderer.setMarkForeground(markForegroundCOLOR);
    }

    /**
     * Components of the property: Mark Mode.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void markMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lMarkMode = new JLabel();
        cMarkMode = new JComboBox();
        addLabelComboBox(panel, lMarkMode, MessagesGED.mark_mode,
                         cMarkMode, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW);
        cMarkMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_MODE__,
                    cMarkMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Mark Mode
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_MARK_MODE__);
        cMarkMode.setSelectedIndex(enable ? 1 : 0);
    }

    public JComboBox getMarkMode() {
        return cMarkMode;
    }

    /**
     * Components of the property: Mark Size.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void markSize(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lMarkSize = new JLabel();
        final JTextField cMarkSize = new JTextField();
        addLabelTextField(panel, lMarkSize, MessagesGED.mark_size,
                          cMarkSize, true, LEFTMARGIN, COLUMN, ROW);
        cMarkSize.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setMarkSize(cMarkSize, cMarkSize.getText(), UID);
            }
        });
        cMarkSize.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setMarkSize(cMarkSize, cMarkSize.getText(), UID);
            }
        });
        // Get the current status of the property: Mark Size
        cMarkSize.setText(Integer.toString((Integer) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE__)));
    }

    /**
    * Set Mark Size.
    * @param size mark size.
    */
    private void setMarkSize(JTextField cMarkSize, String size, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_MARK_SIZE__,
                Integer.parseInt(size));
        } catch (NumberFormatException e) {
            cMarkSize.setText(Integer.toString((Integer) GraphicController.getController()
                                               .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE__)));
        }
    }

    /**
     * Components of the property: Mark Size Unit.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void markSizeUnit(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lMarkSizeUnit = new JLabel();
        final JComboBox cMarkSizeUnit = new JComboBox();
        addLabelComboBox(panel, lMarkSizeUnit, MessagesGED.mark_size_unit,
                         cMarkSizeUnit, new String[] {MessagesGED.point , MessagesGED.tabulated},
                         LEFTMARGIN, COLUMN, ROW);
        cMarkSizeUnit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__,
                    cMarkSizeUnit.getSelectedIndex());
            }
        });
        // Get the current status of the property: Mark Size Unit
        cMarkSizeUnit.setSelectedIndex((Integer) GraphicController.getController()
                                       .getProperty(UID, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__));
    }

    /**
     * Components of the property: Mark Style.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void markStyle(JPanel panel, MarkStyleRenderer markStyleRenderer,
                          int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lMarkStyle = new JLabel();
        final JComboBox cMarkStyle = new JComboBox(MarkStyle.values());
        addLabelComboBox(panel, lMarkStyle, MessagesGED.mark_style,
                         cMarkStyle, null,
                         LEFTMARGIN, COLUMN, ROW);
        cMarkStyle.setRenderer(markStyleRenderer);
        cMarkStyle.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                try {
                    if (cMarkMode.getSelectedIndex() == 0) { //If mark_mode is off
                        cMarkMode.setSelectedIndex(1);    //turn on
                    }
                } catch (NullPointerException e) { }

                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_MARK_STYLE__, cMarkStyle.getSelectedIndex());
            }
        });
        // Get the current status of the property: Mark Style
        int currentMarkStyle = (Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_MARK_STYLE__);
        if (cMarkMode.getSelectedIndex() == 1) {
            cMarkStyle.setSelectedIndex(currentMarkStyle);
        }
    }

    /**
     * Components of the property: Thickness
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void thickness(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lThickness = new JLabel();
        final JTextField cThickness = new JTextField();
        addLabelTextField(panel, lThickness, MessagesGED.thickness,
                          cThickness, true, LEFTMARGIN, COLUMN, ROW++);
        cThickness.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                setThickness(cThickness, UID);
            }
        });
        cThickness.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                setThickness(cThickness, UID);
            }
        });
        // Get the current status of the property: Color Flag
        cThickness.setText(Double.toString((Double) GraphicController.getController()
                                           .getProperty(UID, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
    }

    /**
    * Set Thickness.
    * @param thickness.
    */
    public void setThickness(JTextField cThickness, Integer UID) {
        try {
            GraphicController.getController().setProperty(
                UID,
                GraphicObjectProperties.__GO_LINE_THICKNESS__,
                Double.parseDouble(cThickness.getText()));
        } catch (NumberFormatException e) {
            cThickness.setText(Double.toString((Double) GraphicController.getController()
                                               .getProperty(UID, GraphicObjectProperties.__GO_LINE_THICKNESS__)));
        }
    }
}
