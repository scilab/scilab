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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Datatip properties:
* TIP_AUTOORIENTATION, TIP_DATA, TIP_BOX_MODE, TIP_LABEL_MODE,
* TIP_ORIENTATION, TIP_DISPLAY_COMPONENTS, TIP_INTERP_MODE, TIP_DISPLAY_FNC
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Datatip extends ContentLayout {
    private JTextField cTipData = new JTextField();
    private JTextField cTipDataX = new JTextField();
    private JTextField cTipDataY = new JTextField();
    private JTextField cTipDataZ = new JTextField();

    /**
     * Components of the property: Tip display components
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipDisplayComponents(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {

        JLabel lTipDisplayComp = new JLabel();
        final JTextField cTipDisplayComp = new JTextField();

        addLabelTextField(panel, lTipDisplayComp, MessagesGED.display_components,
                          cTipDisplayComp, true, LEFTMARGIN, COLUMN, ROW);
        cTipDisplayComp.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                if (!GraphicController.getController().setProperty(
                            UID, GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__,
                            cTipDisplayComp.getText())) {
                    cTipDisplayComp.setText((String) GraphicController.getController()
                                            .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__));
                }
                getTipData(UID);
            }
        });
        // Get the current status of the property: Tip display components
        String comp = (String) GraphicController.getController()
                      .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_DISPLAY_COMPONENTS__);
        cTipDisplayComp.setText(comp);
    }

    /**
     * Components of the property: Tip Auto Orientation.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipAutoOrientation(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        JLabel lTipAutoOrientation = new JLabel();
        final JComboBox cTipAutoOrientation = new JComboBox();
        addLabelComboBox(panel, lTipAutoOrientation, MessagesGED.auto_orientation,
                         cTipAutoOrientation, messageOffOn, LEFTMARGIN, COLUMN, ROW);
        cTipAutoOrientation.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__,
                    cTipAutoOrientation.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Tip Auto Orientation
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_AUTOORIENTATION__);
        cTipAutoOrientation.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Tip Box Mode.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipBoxMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        JLabel lBoxMode = new JLabel();
        final JComboBox cBoxMode = new JComboBox();
        addLabelComboBox(panel, lBoxMode, MessagesGED.box_mode,
                         cBoxMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cBoxMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_DATATIP_BOX_MODE__,
                    cBoxMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Tip Box Mode
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_BOX_MODE__);
        cBoxMode.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Tip Data.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipData(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        final JToggleButton bTipData = new JToggleButton();
        JLabel lTipData = new JLabel();
        final JPanel pTipData = new JPanel();
        JLabel lTipDataX = new JLabel();
        JLabel lTipDataY = new JLabel();
        JLabel lTipDataZ = new JLabel();
        cTipData = new JTextField();
        cTipDataX = new JTextField();
        cTipDataY = new JTextField();
        cTipDataZ = new JTextField();
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        addInnerPanel(panel, pTipData, bTipData, lTipData, cTipData, MessagesGED.data, ROW);
        bTipData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pTipData.setVisible(!bTipData.isSelected());
            }
        });
        //Tip Data X
        addLabelTextField(pTipData, lTipDataX, "X",
                          cTipDataX, true, LEFTMARGINIP, COLUMNIP, 0);
        cTipDataX.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTipData(UID);
            }
        });
        cTipDataX.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTipData(UID);
            }
        });
        //Tip Data Y
        addLabelTextField(pTipData, lTipDataY, "Y",
                          cTipDataY, true, LEFTMARGINIP, COLUMNIP, 1);
        cTipDataY.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTipData(UID);
            }
        });
        cTipDataY.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTipData(UID);
            }
        });
        //Tip Data Z
        addLabelTextField(pTipData, lTipDataZ, "Z",
                          cTipDataZ, true, LEFTMARGINIP, COLUMNIP, 2);
        cTipDataZ.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTipData(UID);
            }
        });
        cTipDataZ.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTipData(UID);
            }
        });
        //Get the current status of the property: Tip Data
        getTipData(UID);
    }

    /**
    * Inserts the current situation of the tip_data.
    */
    public final void titleTipData() {
        cTipData.setText(" [" + cTipDataX.getText() + " , "
                         + cTipDataY.getText() + " , "
                         + cTipDataY.getText() + "]");
    }

    /**
    * Get status of the property: tip_data.
    */
    private void getTipData(Integer UID) {
        Double[] currentTipData = (Double []) GraphicController.getController()
                                  .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_DATA__);
        cTipDataX.setText(currentTipData[0].toString());
        cTipDataY.setText(currentTipData[1].toString());
        cTipDataZ.setText(currentTipData[2].toString());
        titleTipData();
    }

    /**
    * Updates the property: tip_data.
    */
    private void updateTipData(Integer UID) {
        try {
            Double[] value = new Double[3];
            value[0] = Double.parseDouble(cTipDataX.getText());
            value[1] = Double.parseDouble(cTipDataY.getText());
            value[2] = Double.parseDouble(cTipDataZ.getText());
            //TODO Fix ged set property on datatip by computing ratio instead of fixed values
            //GraphicController.getController().setProperty(UID, GraphicObjectProperties.__GO_DATATIP_DATA__, value);
            titleTipData();
        } catch (NumberFormatException e) {
            getTipData(UID);
        }
    }

    /**
     * Components of the property: Tip Label Mode.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipLabelMode(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        JLabel lTipLabelMode = new JLabel();
        final JComboBox cTipLabelMode = new JComboBox();
        addLabelComboBox(panel, lTipLabelMode, MessagesGED.label_mode,
                         cTipLabelMode, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cTipLabelMode.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__,
                    cTipLabelMode.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_LABEL_MODE__);
        cTipLabelMode.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Tip Orientation.
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void tipOrientation(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lTipOrientation = new JLabel();
        final JComboBox cTipOrientation = new JComboBox();
        String[] options = new String[] {"top_left", "top_right",
                                         "bottom_left", "bottom_right"
                                        };
        addLabelComboBox(panel, lTipOrientation, MessagesGED.orientation,
                         cTipOrientation, options, LEFTMARGIN, COLUMN, ROW++);
        cTipOrientation.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_DATATIP_ORIENTATION__,
                    cTipOrientation.getSelectedIndex());
            }
        });
        // Get the current status of the property:Tip Orientation
        cTipOrientation.setSelectedIndex((Integer) GraphicController.getController()
                                         .getProperty(UID, GraphicObjectProperties.__GO_DATATIP_ORIENTATION__));
    }
}
