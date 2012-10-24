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
package org.scilab.modules.gui.ged.polyline;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.Insets;

import javax.swing.JComboBox;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Position.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Position extends DataProperties {
    protected static JToggleButton bPosition;
    private JLabel lPosition;
    private JSeparator sPosition;
    protected static JPanel pPosition;
    private JLabel lMarkSizeUnit;
    private JComboBox cMarkSizeUnit;
    private JLabel lShiftX;
    private JLabel cShiftX;
    private JButton bShiftX;
    private JPanel pShiftX;
    private JLabel lShiftY;
    private JLabel cShiftY;
    private JButton bShiftY;
    private JPanel pShiftY;
    private JLabel lShiftZ;
    private JLabel cShiftZ;
    private JButton bShiftZ;
    private JPanel pShiftZ;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public Position(String objectID){
        super(objectID);
        initPosition(objectID);
    }

    /**
    * It has all the components of the section Position.
    */
    @Override
    public void positionComponents() {
        ContentLayout layout = new ContentLayout();

        bPosition = new JToggleButton();
        lPosition = new JLabel();
        sPosition = new JSeparator();
        pPosition = new JPanel();
        lMarkSizeUnit = new JLabel();
        cMarkSizeUnit = new JComboBox();
        lShiftX = new JLabel();
        cShiftX = new JLabel();
        bShiftX = new JButton();
        pShiftX = new JPanel();
        lShiftY = new JLabel();
        cShiftY = new JLabel();
        bShiftY = new JButton();
        pShiftY = new JPanel();
        lShiftZ = new JLabel();
        cShiftZ = new JLabel();
        bShiftZ = new JButton();
        pShiftZ = new JPanel();

        //Components of the header: Position.
        bPosition.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bPositionActionPerformed(evt);
            }
        });
        layout.addSHbutton(this, bPosition, 0, 6);

        layout.addSectionTitle(this, lPosition, MessagesGED.position, 6);

        layout.addSeparator(this, sPosition, 7);

        pPosition.setLayout(new GridBagLayout());

        //Components of the property: Mark Size Unit.
        layout.addJLabel(pPosition, lMarkSizeUnit, MessagesGED.mark_size_unit, 0, 0, 16);

        cMarkSizeUnit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cMarkSizeUnitActionPerformed(evt);
            }
        });
        layout.addJComboBox(pPosition, cMarkSizeUnit,
                new String[] { MessagesGED.point, MessagesGED.tabulated },
                1, 0);

        //Components of the property: X Shift.
        layout.addJLabel(pPosition, lShiftX, MessagesGED.x_shift, 0, 1, 16);
        layout.addDataField(pPosition, pShiftX, bShiftX, cShiftX, 1, 1);
        bShiftX.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bShiftXActionPerformed(evt);
            }
        });

        //Components of the property: Y Shift.
        layout.addJLabel(pPosition, lShiftY, MessagesGED.y_shift, 0, 2, 16);
        layout.addDataField(pPosition, pShiftY, bShiftY, cShiftY, 1, 2);
        bShiftY.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bShiftYActionPerformed(evt);
            }
        });

        //Components of the property: Z Shift.
        layout.addJLabel(pPosition, lShiftZ, MessagesGED.z_shift, 0, 3, 16);
        layout.addDataField(pPosition, pShiftZ, bShiftZ, cShiftZ, 1, 3);
        bShiftZ.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bShiftZActionPerformed(evt);
            }
        });

        //Positioning JPanel - Position
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 8;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 12, 0);
        add(pPosition, gbc);
    }

    /**
    * Loads the current properties of the section Data Properties.
    *
    * @param objectID Enters the identification of polyline.
    */
    public void initPosition(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            //Get the current status of the property: Mark Size Unit
            cMarkSizeUnit.setSelectedIndex( (Integer) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__) );

            //Get the current status of the property: Shift
            String teste1 = (String) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_X_SHIFT__);

            //cShiftX.setText(teste.toString());
            /*cShiftY.setText((String) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_Y_SHIFT__));
            cShiftZ.setText((String) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_Z_SHIFT__));*/
            //titleShift();
        }
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bPositionActionPerformed(ActionEvent evt) {
        if (bPosition.isSelected()) {
            pPosition.setVisible(false);
            HidePolyline.checkAllButtons();
        } else {
            pPosition.setVisible(true);
            HidePolyline.checkAllButtons();
        }
    }

    /**
    * Updates the property: Mark Size Unit.
    *
    * @param evt ActionEvent.
    */
    private void cMarkSizeUnitActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_SIZE_UNIT__,
                cMarkSizeUnit.getSelectedIndex()
                );
    }

    /**
    * Updates the property: X Shift.
    * @param evt ActionEvent.
    */
    private void bShiftXActionPerformed(ActionEvent evt) {
        //Not implemented yet
    }

    /**
    * Updates the property: Y Shift.
    * @param evt ActionEvent.
    */
    private void bShiftYActionPerformed(ActionEvent evt) {
        //Not implemented yet
    }

    /**
    * Updates the property: Z Shift.
    * @param evt ActionEvent.
    */
    private void bShiftZActionPerformed(ActionEvent evt) {
        //Not implemented yet
    }
}
