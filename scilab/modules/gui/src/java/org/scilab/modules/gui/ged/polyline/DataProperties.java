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
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends BaseProperties {
    protected static JToggleButton bDataProperties;
    private JLabel lDataProperties;
    private JSeparator sDataProperties;
    protected static JPanel pDataProperties;
    private JToggleButton bClipBox;
    private JLabel lClipBox;
    private JTextField cClipBox;
    private JPanel pClipBox;
    private JLabel lClipBoxUpper;
    private JTextField cClipBoxUpper;
    private JLabel lClipBoxPoint;
    private JTextField cClipBoxPoint;
    private JLabel lClipBoxWidth;
    private JTextField cClipBoxWidth;
    private JLabel lClipBoxHeight;
    private JTextField cClipBoxHeight;
    private JLabel lClipState;
    private JComboBox cClipState;
    private JLabel lTag;
    private JTextField cTag;
    private JLabel lData;
    private JPanel pData;
    private JButton bData;
    private JLabel cData;
    protected JDialog dataTableDialog;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of polyline.
    */
    public DataProperties(String objectID){
        super(objectID);
        initDataProperties(objectID);
    }

    /**
    * It has all the components of the section Data Properties.
    */
    @Override
    public void dataPropertiesComponents() {
        ContentLayout layout = new ContentLayout();

        bDataProperties = new JToggleButton();
        lDataProperties = new JLabel();
        sDataProperties = new JSeparator();
        pDataProperties = new JPanel();
        bClipBox = new JToggleButton();
        lClipBox = new JLabel();
        cClipBox = new JTextField();
        pClipBox = new JPanel();
        lClipBoxUpper = new JLabel();
        cClipBoxUpper = new JTextField();
        lClipBoxPoint = new JLabel();
        cClipBoxPoint = new JTextField();
        lClipBoxWidth = new JLabel();
        cClipBoxWidth = new JTextField();
        lClipBoxHeight = new JLabel();
        cClipBoxHeight = new JTextField();
        lClipState = new JLabel();
        cClipState = new JComboBox();
        lTag = new JLabel();
        cTag = new JTextField();
        lData = new JLabel();
        pData = new JPanel();
        bData = new JButton();
        cData = new JLabel();


        //Components of the header: Data Properties.
        bDataProperties.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bDataPropertiesActionPerformed(evt);
            }
        });
        layout.addSHbutton(this, bDataProperties, 0, 3);

        layout.addSectionTitle(this, lDataProperties, MessagesGED.data_properties, 3);

        layout.addSeparator(this, sDataProperties, 4);

        pDataProperties.setLayout(new GridBagLayout());


        //Components of the property: Clip Box.
        bClipBox.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bClipBoxActionPerformed(evt);
            }
        });
        layout.addSHbutton(pDataProperties, bClipBox, 0, 0);

        bClipBox.setSelected(true);

        layout.addJLabel(pDataProperties, lClipBox, MessagesGED.clip_box, 1, 0, 0);

        layout.addJTextField(pDataProperties, cClipBox, false, 2, 0, 4);

        pClipBox.setLayout(new GridBagLayout());

        pClipBox.setVisible(false);

        layout.addJLabel(pClipBox, lClipBoxUpper, MessagesGED.upper_left, 0, 0, 0);

        cClipBoxUpper.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClipBoxUpperActionPerformed(evt);
            }
        });
        cClipBoxUpper.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cClipBoxUpperFocusLost(evt);
            }
        });
        layout.addJTextField(pClipBox, cClipBoxUpper, true, 1, 0, 4);

        layout.addJLabel(pClipBox, lClipBoxPoint, MessagesGED.point, 0, 1, 0);

        cClipBoxPoint.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClipBoxPointActionPerformed(evt);
            }
        });
        cClipBoxPoint.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cClipBoxPointFocusLost(evt);
            }
        });
        layout.addJTextField(pClipBox, cClipBoxPoint, true, 1, 1, 4);

        layout.addJLabel(pClipBox, lClipBoxWidth, MessagesGED.width, 0, 2, 0);

        cClipBoxWidth.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClipBoxWidthActionPerformed(evt);
            }
        });
        cClipBoxWidth.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cClipBoxWidthFocusLost(evt);
            }
        });
        layout.addJTextField(pClipBox, cClipBoxWidth, true, 1, 2, 4);

        layout.addJLabel(pClipBox, lClipBoxHeight, MessagesGED.height, 0, 3, 0);

        cClipBoxHeight.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClipBoxHeightActionPerformed(evt);
            }
        });
        cClipBoxHeight.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cClipBoxHeightFocusLost(evt);
            }
        });
        layout.addJTextField(pClipBox, cClipBoxHeight, true, 1, 3, 4);

        //Positioning JPanel Clip Box.
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.gridwidth = 2;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 0.1;
        pDataProperties.add(pClipBox, gbc);


        //Components of the property: Clip State.
        layout.addJLabel(pDataProperties, lClipState, MessagesGED.clip_state, 1, 2, 0);

        cClipState.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cClipStateActionPerformed(evt);
            }
        });
        layout.addJComboBox(
                pDataProperties,
                cClipState,
                new String[] { MessagesGED.off, MessagesGED.clipgrf, MessagesGED.on },
                2, 2);


        //Components of the property: Data.
        layout.addJLabel(pDataProperties, lData, MessagesGED.data, 1, 3, 0);

        layout.addDataField(pDataProperties, pData, bData, cData, 2, 3);
        bData.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                bDataActionPerformed(evt);
            }
        });


        //Components of the property: Tag.
        layout.addJLabel(pDataProperties, lTag, MessagesGED.tag, 1, 4, 0);

        cTag.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                cTagActionPerformed(evt);
            }
        });
        cTag.addFocusListener(new FocusAdapter() {
            public void focusLost(FocusEvent evt) {
                cTagFocusLost(evt);
            }
        });
        layout.addJTextField(pDataProperties, cTag, true, 2, 4, 4);


        //Positioning JPanel Data Properties.
        gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 5;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.weightx = 0.1;
        gbc.insets = new Insets(0, 0, 12, 0);
        add(pDataProperties, gbc);
    }

    /**
    * Loads the current properties of the section Data Properties.
    *
    * @param objectID Enters the identification of polyline.
    */
    public void initDataProperties(String objectID) {
        if (objectID != null) {
            currentpolyline = objectID;

            //Get the current status of the property: Clip Box
            Double[] currentClipBox = (Double []) GraphicController.getController()
                    .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_BOX__);
            cClipBoxUpper.setText(currentClipBox[0].toString());
            cClipBoxPoint.setText(currentClipBox[1].toString());
            cClipBoxWidth.setText(currentClipBox[2].toString());
            cClipBoxHeight.setText(currentClipBox[3].toString());
            titleClipBox();

            // Get the current status of the property: Clip State
            cClipState.setSelectedIndex( (Integer) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_STATE__) );

            // Get the current status of the property: Data
            //Not implemented yet

            // Get the current status of the property: Tag
            cTag.setText( (String) GraphicController.getController()
                                .getProperty(currentpolyline, GraphicObjectProperties.__GO_TAG__) );
        }
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bDataPropertiesActionPerformed(ActionEvent evt) {
        if (bDataProperties.isSelected()) {
            pDataProperties.setVisible(false);
            HidePolyline.checkAllButtons();
        } else {
            pDataProperties.setVisible(true);
            HidePolyline.checkAllButtons();
        }
    }

    /**
    * Implement the action button to show/hide.
    */
    private void bClipBoxActionPerformed(ActionEvent evt) {
        if (bClipBox.isSelected()) {
            pClipBox.setVisible(false);
            HidePolyline.checkAllButtons();
        } else {
            pClipBox.setVisible(true);
            HidePolyline.checkAllButtons();
        }
    }

    /**
    * Inserts the current situation of the clip box.
    */
    public void titleClipBox(){
        String titleUpper = cClipBoxUpper.getText();
        String titlePoint = cClipBoxPoint.getText();
        String titleWidth = cClipBoxWidth.getText();
        String titleHeight = cClipBoxHeight.getText();
        String titleClipBox = titleUpper + " , " + titlePoint +
                              " , " + titleWidth + " , " + titleHeight;
        cClipBox.setText(" [" + titleClipBox + "]");
    }

    /**
    * Updates the property: Clip Box.
    * upper:0  point:1  width:2 height: 3
    */
    private void updateClipBox() {
        Double[] value = new Double[4];
        value[0] = Double.parseDouble(cClipBoxUpper.getText());
        value[1] = Double.parseDouble(cClipBoxPoint.getText());
        value[2] = Double.parseDouble(cClipBoxWidth.getText());
        value[3] = Double.parseDouble(cClipBoxHeight.getText());
        GraphicController.getController()
                .setProperty(currentpolyline, GraphicObjectProperties.__GO_CLIP_BOX__, value);
        titleClipBox();
    }

    /**
    * Updates the property: Clip Box Upper-left.
    * @param evt ActionEvent.
    */
    private void cClipBoxUpperActionPerformed(ActionEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Upper-left.
    * @param evt FocusEvent.
    */
    private void cClipBoxUpperFocusLost(FocusEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Point.
    * @param evt ActionEvent.
    */
    private void cClipBoxPointActionPerformed(ActionEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Point.
    * @param evt FocusEvent.
    */
    private void cClipBoxPointFocusLost(FocusEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Width.
    * @param evt ActionEvent.
    */
    private void cClipBoxWidthActionPerformed(ActionEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Width.
    * @param evt FocusEvent.
    */
    private void cClipBoxWidthFocusLost(FocusEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Height.
    * @param evt ActionEvent.
    */
    private void cClipBoxHeightActionPerformed(ActionEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip Box Height.
    * @param evt FocusEvent.
    */
    private void cClipBoxHeightFocusLost(FocusEvent evt) {
        updateClipBox();
    }

    /**
    * Updates the property: Clip State.
    *
    * @param evt ActionEvent.
    */
    private void cClipStateActionPerformed(ActionEvent evt) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_CLIP_STATE__,
                cClipState.getSelectedIndex()
                );
    }

    /**
    * Implement the action button to open a data table.
    *
    * @param evt ActionEvent.
    */
    private void bDataActionPerformed(ActionEvent evt) {
        //dataTableDialog.setVisible(true);
        //Not implemented yet
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_TAG__,
                cTag.getText()
                );
    }

    /**
    * Updates the property: Tag.
    *
    * @param evt ActionEvent.
    */
    private void cTagActionPerformed(ActionEvent evt) {
        updateTag();
    }

    /**
    * Updates the property: Tag.
    *
    * @param evt FocusEvent.
    */
    private void cTagFocusLost(FocusEvent evt) {
        updateTag();
    }
}
