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
package org.scilab.modules.gui.ged.graphic_objects.axes;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;

import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSeparator;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ContentLayout;

import org.scilab.modules.gui.ged.MessagesGED;
import org.scilab.modules.gui.ged.graphic_objects.SimpleSection;

/**
* Construction and startup of all components of the section: Camera.
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class Camera extends Axes implements SimpleSection {
    private String currentAxes;
    private ContentLayout layout = new ContentLayout();

    private static JToggleButton bCamera;
    private JLabel lCamera;
    private JSeparator sCamera;
    private static JPanel pCamera;
    private JToggleButton bRotationAngles;
    private JTextField cAlpha;
    private JComboBox cCubeScaling;
    private JComboBox cIsoView;
    private JTextField cRotationAngles;
    private JTextField cTheta;
    private JComboBox cView;
    private JLabel lAlpha;
    private JLabel lCubeScaling;
    private JLabel lIsoView;
    private JLabel lRotationAngles;
    private JLabel lTheta;
    private JLabel lView;
    private JPanel pRotationAngles;


    /**
    * Initializes the properties and the icons of the buttons.
    * @param objectID Enters the identification of figure.
    */
    public Camera(String objectID) {
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
        bCamera = new JToggleButton();
        lCamera = new JLabel();
        sCamera = new JSeparator();
        pCamera = new JPanel();
        lCubeScaling = new JLabel();
        cCubeScaling = new JComboBox();
        cIsoView = new JComboBox();
        lIsoView = new JLabel();
        bRotationAngles = new JToggleButton();
        lRotationAngles = new JLabel();
        cRotationAngles = new JTextField();
        pRotationAngles = new JPanel();
        lAlpha = new JLabel();
        cAlpha = new JTextField();
        lTheta = new JLabel();
        cTheta = new JTextField();
        lView = new JLabel();
        cView = new JComboBox();
    }

    /**
    * Insert show/hide button, title and main JPanel of section.
    */
    @Override
    public final void initMainPanel() {
        String SECTIONNAME = MessagesGED.camera;
        this.setName(SECTIONNAME);
        layout.addHeader(this, pCamera, bCamera, lCamera, sCamera, SECTIONNAME);
        bCamera.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pCamera.setVisible(!bCamera.isSelected());
                HideAxes.checkAllButtons();
            }
        });
    }

    /**
    * Initialize the Components.
    */
    @Override
    public final void initComponents() {
        String[] messageOffOn = new String[] {MessagesGED.off , MessagesGED.on};
        int ROW = 0;
        int LEFTMARGIN = 0; //to components
        int COLUMN = 1; //first column
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel

        //Components of the property: Cube Scaling.
        layout.addLabelComboBox(pCamera, lCubeScaling, MessagesGED.cube_scaling,
                                cCubeScaling, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cCubeScaling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_CUBE_SCALING__,
                        cCubeScaling.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Isometric View.
        layout.addLabelComboBox(pCamera, lIsoView, MessagesGED.isometric_view,
                                cIsoView, messageOffOn, LEFTMARGIN, COLUMN, ROW++);
        cIsoView.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_ISOVIEW__,
                        cIsoView.getSelectedIndex() == 0 ? false : true);
            }
        });

        //Components of the property: Rotation Angles.
        ROW = layout.addInnerPanel(pCamera, pRotationAngles, bRotationAngles, lRotationAngles,
                cRotationAngles, MessagesGED.rotation_angles, ROW);
        bRotationAngles.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pRotationAngles.setVisible(!bRotationAngles.isSelected());
                HideAxes.checkAllButtons();
            }
        });
        int rowRotation = 0;
        //Rotation Angles - Alpha
        layout.addLabelTextField(pRotationAngles, lAlpha, MessagesGED.alpha,
                                 cAlpha, true, LEFTMARGINIP, COLUMNIP, rowRotation++);
        cAlpha.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateRotationAngles();
            }
        });
        cAlpha.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateRotationAngles();
            }
        });
        //Rotation Angles - Theta
        layout.addLabelTextField(pRotationAngles, lTheta, MessagesGED.theta,
                                 cTheta, true, LEFTMARGINIP, COLUMNIP, rowRotation++);
        cTheta.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateRotationAngles();
            }
        });
        cTheta.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateRotationAngles();
            }
        });

        //Components of the property: View.
        layout.addLabelComboBox(pCamera, lView, MessagesGED.view,
                                cView, new String[] {MessagesGED._2d , MessagesGED._3d},
                                LEFTMARGIN, COLUMN, ROW++);
        cView.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                        currentAxes, GraphicObjectProperties.__GO_VIEW__,
                        cView.getSelectedIndex());
            }
        });
   }

    /**
    * Loads the current properties of the section.
    * @param objectID Enters the identification of axis.
    */
    @Override
    public final void loadProperties(String objectID) {
        if (objectID != null) {
            currentAxes = objectID;
            boolean enable;

            // Get the current status of the property: Cube Scaling
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_CUBE_SCALING__);
            cCubeScaling.setSelectedIndex(enable?1:0);

            // Get the current status of the property: Isometric View (isoview)
            enable = (Boolean) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_ISOVIEW__);
            cIsoView.setSelectedIndex(enable?1:0);

            //Get the current status of the property: Rotation Angles
            Double[] currentAngles = (Double []) GraphicController.getController()
                    .getProperty(currentAxes, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
            cAlpha.setText(currentAngles[0].toString());
            cTheta.setText(currentAngles[1].toString());
            titleRotationAngles();

            //Get the current status of the property: View
            cView.setSelectedIndex((Integer) GraphicController.getController()
                                .getProperty(currentAxes, GraphicObjectProperties.__GO_VIEW__));
        }
    }

    /**
    * Inserts the current state of functionality: Rotation Angles in main label.
    */
    public final void titleRotationAngles() {
        cRotationAngles.setText(" [" + cAlpha.getText() + " , " + cTheta.getText() + "]");
    }

    /**
    * Updates the property: Rotation Angles.
    */
    private void updateRotationAngles() {
        Double[] value = new Double[2];
        value[0] = Double.parseDouble(cAlpha.getText());
        value[1] = Double.parseDouble(cTheta.getText());
        GraphicController.getController()
                .setProperty(currentAxes, GraphicObjectProperties.__GO_ROTATION_ANGLES__, value);
        titleRotationAngles();
    }

    /**
    * Get Status of Main Jpanel.
    * @return visibility
    */
    public static boolean getStatus() {
        return pCamera.isVisible();
    }

    /**
    * Set Visibility of Property Group.
    * @param visible boolean
    */
    public static void setVisibility(boolean visible) {
        pCamera.setVisible(visible);
        bCamera.setSelected(!visible);
    }
}