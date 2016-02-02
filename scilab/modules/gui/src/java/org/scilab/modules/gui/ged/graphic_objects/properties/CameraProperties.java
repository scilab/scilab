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
import javax.swing.JCheckBox;
import javax.swing.JColorChooser;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JToggleButton;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.ContentLayout;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Camera properties:
* VIEW, ISOVIEW, CUBESCALING, ROTATIONANGLES
*
* @author Marcos CARDINOT <mcardinot@gmail.com>
*/
public class CameraProperties extends ContentLayout {
    private JTextField cRotationAngles;
    private JTextField cAlpha;
    private JTextField cTheta;

    /**
     * Components of the property: Cube Scaling
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void cubeScaling(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lCubeScaling = new JLabel();
        final JComboBox cCubeScaling = new JComboBox();
        addLabelComboBox(panel, lCubeScaling, MessagesGED.cube_scaling,
                         cCubeScaling, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cCubeScaling.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_CUBE_SCALING__,
                    cCubeScaling.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Cube Scaling
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_CUBE_SCALING__);
        cCubeScaling.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Isometric View
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void isometricView(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lIsoView = new JLabel();
        final JComboBox cIsoView = new JComboBox();

        addLabelComboBox(panel, lIsoView, MessagesGED.isometric_view,
                         cIsoView, new String[] {MessagesGED.off , MessagesGED.on},
                         LEFTMARGIN, COLUMN, ROW++);
        cIsoView.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_ISOVIEW__,
                    cIsoView.getSelectedIndex() == 0 ? false : true);
            }
        });
        // Get the current status of the property: Isometric View (isoview)
        boolean enable = (Boolean) GraphicController.getController()
                         .getProperty(UID, GraphicObjectProperties.__GO_ISOVIEW__);
        cIsoView.setSelectedIndex(enable ? 1 : 0);
    }

    /**
     * Components of the property: Rotation Angles
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     */
    public void rotationAngles(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        int LEFTMARGINIP = 0; //left margin - inner panel
        int COLUMNIP = 0; //left column - inner panel
        JLabel lRotationAngles = new JLabel();
        final JPanel pRotationAngles = new JPanel();
        final JToggleButton bRotationAngles = new JToggleButton();
        JLabel lAlpha = new JLabel();
        JLabel lTheta = new JLabel();
        cRotationAngles = new JTextField();
        cAlpha = new JTextField();
        cTheta = new JTextField();

        addInnerPanel(panel, pRotationAngles, bRotationAngles, lRotationAngles,
                      cRotationAngles, MessagesGED.rotation_angles, ROW);
        bRotationAngles.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                pRotationAngles.setVisible(!bRotationAngles.isSelected());
            }
        });
        int rowRotation = 0;
        //Rotation Angles - Alpha
        addLabelTextField(pRotationAngles, lAlpha, MessagesGED.alpha,
                          cAlpha, true, LEFTMARGINIP, COLUMNIP, rowRotation++);
        cAlpha.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateRotationAngles(UID);
            }
        });
        cAlpha.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateRotationAngles(UID);
            }
        });
        //Rotation Angles - Theta
        addLabelTextField(pRotationAngles, lTheta, MessagesGED.theta,
                          cTheta, true, LEFTMARGINIP, COLUMNIP, rowRotation++);
        cTheta.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                updateRotationAngles(UID);
            }
        });
        cTheta.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent evt) {
                updateRotationAngles(UID);
            }
        });
        //Get the current status of the property: Rotation Angles
        getRotationAngles(UID);
    }

    /**
    * Get the current status of the property: Rotation Angles.
    */
    private void getRotationAngles(Integer UID) {
        Double[] currentAngles = (Double []) GraphicController.getController()
                                 .getProperty(UID, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
        cAlpha.setText(currentAngles[0].toString());
        cTheta.setText(currentAngles[1].toString());
        titleRotationAngles();
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
    private void updateRotationAngles(Integer UID) {
        Double[] value = new Double[2];
        value[0] = Double.parseDouble(cAlpha.getText());
        value[1] = Double.parseDouble(cTheta.getText());
        GraphicController.getController()
        .setProperty(UID, GraphicObjectProperties.__GO_ROTATION_ANGLES__, value);
        titleRotationAngles();
    }

    /**
     * Components of the property: View
     * @param panel
     * @param ROW
     * @param COLUMN
     * @param LEFTMARGIN
     * @param UID
     * @param parentFigure
     */
    public void view(JPanel panel, int ROW, int COLUMN, int LEFTMARGIN, final Integer UID) {
        JLabel lView = new JLabel();
        final JComboBox cView = new JComboBox();

        addLabelComboBox(panel, lView, MessagesGED.view,
                         cView, new String[] {MessagesGED._2d , MessagesGED._3d},
                         LEFTMARGIN, COLUMN, ROW++);
        cView.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                GraphicController.getController().setProperty(
                    UID, GraphicObjectProperties.__GO_VIEW__,
                    cView.getSelectedIndex());
                getRotationAngles(UID);
            }
        });

        //Get the current status of the property: View
        cView.setSelectedIndex((Integer) GraphicController.getController()
                               .getProperty(UID, GraphicObjectProperties.__GO_VIEW__));
    }
}