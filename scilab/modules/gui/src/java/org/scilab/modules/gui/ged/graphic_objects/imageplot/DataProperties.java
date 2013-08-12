/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.imageplot;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Construction and startup of all components of the section: Data Properties.
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class DataProperties extends Imageplot implements SimpleSection {
    private String parentFigure;
    private String currentImageplot;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bDataProperties;
    private JLabel lDataProperties;
    private JSeparator sDataProperties;
    private static JPanel pDataProperties;
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
    private JLabel lDataMapping;
    private JComboBox cDataMapping;
    private JLabel lTag;
    private JTextField cTag;
    private JLabel lUserData;
    private JTextField cUserData;

    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of imageplot.
    */
    public DataProperties(String objectID) {
        parentFigure = getFigueID(objectID);
        currentImageplot = objectID;
        constructComponents();
        initMainPanel();
        initComponents();
        loadProperties(objectID);
    }

    /**
    * Construct the Components.
    */
    @Override
    public final void constructComponents() {
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
        lDataMapping = new JLabel();
        cDataMapping = new JComboBox();
        lUserData = new JLabel();
        cUserData = new JTextField();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.data_properties;
        this.setName(SECTIONNAME);

        layout.addHeader(this, pDataProperties, bDataProperties, lDataProperties, sDataProperties, SECTIONNAME);
        bDataProperties.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pDataProperties.setVisible(!bDataProperties.isSelected());
                HideImageplot.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        int ROW = 0;
        int LEFTMARGIN = 0; //to inner components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Clip State.
        layout.addLabelComboBox(pDataProperties, lClipState, MessagesGED.clip_state,
                                cClipState, new String[] {MessagesGED.off, MessagesGED.clipgrf, MessagesGED.on},
                                LEFTMARGIN, COLUMN, ROW++);
        cClipState.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentImageplot, GraphicObjectProperties.__GO_CLIP_STATE__,
                        cClipState.getSelectedIndex());
            }
        });

        //Components of the property: Clip Box.
        ROW = layout.addInnerPanel(pDataProperties, pClipBox, bClipBox, lClipBox, cClipBox, MessagesGED.clip_box, ROW);
        bClipBox.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pClipBox.setVisible(!bClipBox.isSelected());
                HideImageplot.checkAllButtons();
            }
        });
        int rowClipBox = 0;
        //Clip Box Upper
        layout.addLabelTextField(pClipBox, lClipBoxUpper, MessagesGED.upper_left,
                                 cClipBoxUpper, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxUpper.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxUpper.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Point
        layout.addLabelTextField(pClipBox, lClipBoxPoint, MessagesGED.point,
                                 cClipBoxPoint, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxPoint.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxPoint.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Width
        layout.addLabelTextField(pClipBox, lClipBoxWidth, MessagesGED.width,
                                 cClipBoxWidth, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxWidth.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxWidth.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });
        //Clip Box Height
        layout.addLabelTextField(pClipBox, lClipBoxHeight, MessagesGED.height,
                                 cClipBoxHeight, true, LEFTMARGINIP, COLUMNIP, rowClipBox++);
        cClipBoxHeight.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateClipBox();
            }
        });
        cClipBoxHeight.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateClipBox();
            }
        });

        //Components of the property: Data Mapping (only to grayplot).
        if (isGrayplot()) {
            layout.addLabelComboBox(pDataProperties, lDataMapping, MessagesGED.data_mapping,
                                    cDataMapping, new String[] {MessagesGED.scaled, MessagesGED.direct},
                                    LEFTMARGIN, COLUMN, ROW++);
            cDataMapping.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent evt) {
                    GraphicController.getController().setProperty(
                        currentImageplot, GraphicObjectProperties.__GO_DATA_MAPPING__,
                        cDataMapping.getSelectedIndex());
                }
            });
        }

        //Components of the property: Tag.
        layout.addLabelTextField(pDataProperties, lTag, MessagesGED.tag,
                                 cTag, true, LEFTMARGIN, COLUMN, ROW++);
        cTag.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateTag();
            }
        });
        cTag.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateTag();
            }
        });

        //Components of the property: User Data.
        layout.addLabelTextField(pDataProperties, lUserData, MessagesGED.user_data,
                                 cUserData, true, LEFTMARGIN, COLUMN, ROW++);
        cUserData.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateUserData();
            }
        });
        cUserData.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateUserData();
            }
        });
    }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of imageplot.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentImageplot = objectID;

            //Get the current status of the property: Clip Box
            getClipBox();

            // Get the current status of the property: Clip State
            cClipState.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(currentImageplot, GraphicObjectProperties.__GO_CLIP_STATE__));

            // Get the current status of the property: Data Mapping
            if (isGrayplot()) {
                cDataMapping.setSelectedIndex((Integer) GraphicController.getController()
                    .getProperty(currentImageplot, GraphicObjectProperties.__GO_DATA_MAPPING__));
            }

            // Get the current status of the property: Tag
            String tag = (String) GraphicController.getController()
                    .getProperty(currentImageplot, GraphicObjectProperties.__GO_TAG__);
            cTag.setText(tag);

            // Get the current status of the property: User Data
            cUserData.setText((String) GraphicController.getController()
                    .getProperty(currentImageplot, GraphicObjectProperties.__GO_USER_DATA__));
        }
    }

    /**
    * Check type.
    * @return if true, is grayplot
    */
    private boolean isGrayplot() {
        Integer type = (Integer) GraphicController.getController()
                .getProperty(currentImageplot, GraphicObjectProperties.__GO_TYPE__);
        if (type == GraphicObjectProperties.__GO_GRAYPLOT__)
            return true;
        else
            return false;
    }

    /**
    * Inserts the current situation of the clip box.
    */
    public final void titleClipBox() {
        String titleUpper = cClipBoxUpper.getText();
        String titlePoint = cClipBoxPoint.getText();
        String titleWidth = cClipBoxWidth.getText();
        String titleHeight = cClipBoxHeight.getText();
        String titleClipBox = titleUpper + " , " + titlePoint +
                              " , " + titleWidth + " , " + titleHeight;
        cClipBox.setText(" [" + titleClipBox + "]");
    }

    /**
    * Get status of the property: Clip Box.
    */
    private void getClipBox() {
        Double[] currentClipBox = (Double []) GraphicController.getController()
                    .getProperty(currentImageplot, GraphicObjectProperties.__GO_CLIP_BOX__);
        cClipBoxUpper.setText(currentClipBox[0].toString());
        cClipBoxPoint.setText(currentClipBox[1].toString());
        cClipBoxWidth.setText(currentClipBox[2].toString());
        cClipBoxHeight.setText(currentClipBox[3].toString());
        titleClipBox();
    }

    /**
    * Updates the property: Clip Box.
    */
    private void updateClipBox() {
        try {
            if(cClipState.getSelectedIndex()==0)
                cClipState.setSelectedIndex(2);
            Double[] value = new Double[4];
            value[0] = Double.parseDouble(cClipBoxUpper.getText());
            value[1] = Double.parseDouble(cClipBoxPoint.getText());
            value[2] = Double.parseDouble(cClipBoxWidth.getText());
            value[3] = Double.parseDouble(cClipBoxHeight.getText());
            GraphicController.getController()
                       .setProperty(currentImageplot, GraphicObjectProperties.__GO_CLIP_BOX__, value);
            titleClipBox();
        } catch (NumberFormatException e) {
            getClipBox();
        }
    }

    /**
    * Updates the property: Tag.
    */
    private void updateTag() {
        GraphicController.getController().setProperty(
                currentImageplot, GraphicObjectProperties.__GO_TAG__, cTag.getText());
    }

    /**
    * Updates the property: User Data.
    */
    private void updateUserData() {
        GraphicController.getController().setProperty(
                currentImageplot, GraphicObjectProperties.__GO_USER_DATA__, cUserData.getText());
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pDataProperties.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pDataProperties.setVisible(visible);
        bDataProperties.setSelected(!visible);
    }
}