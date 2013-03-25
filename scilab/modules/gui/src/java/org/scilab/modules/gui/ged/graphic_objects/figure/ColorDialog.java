/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Marcos Cardinot
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.ged.graphic_objects.figure;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.LayoutStyle;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Implementation of the windows to color selection.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class ColorDialog extends Style {
    private JColorChooser chooser;
    private JButton ok;
    private Color choice;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Figure.
    */
    public ColorDialog(String objectID) {
        super(objectID);
    }

    /**
    * JDialog - Selection of background colors.
    */
    @Override
    public void dialogColor() {
        colorDialog = new JDialog();
        chooser = new JColorChooser();
        ok = new JButton();

        colorDialog.setTitle(MessagesGED.choose_background_color);
        colorDialog.setMinimumSize(new Dimension(670, 450));
        colorDialog.setModal(true);
        colorDialog.setResizable(false);

        ok.setText("OK");
        ok.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okActionPerformed(evt);
            }
        });

        //Position settings.
        GroupLayout colorDialogLayout = new GroupLayout(colorDialog.getContentPane());
        colorDialog.getContentPane().setLayout(colorDialogLayout);
        colorDialogLayout.setHorizontalGroup(
            colorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(colorDialogLayout.createSequentialGroup()
                .addComponent(chooser, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, colorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(ok)
                .addContainerGap())
        );
        colorDialogLayout.setVerticalGroup(
            colorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(colorDialogLayout.createSequentialGroup()
                .addComponent(chooser, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(ok)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okActionPerformed(ActionEvent evt) {
        choice = chooser.getColor();
        double red = choice.getRed();
        double green = choice.getGreen();
        double blue = choice.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, currentfigure);
        setColor(scilabColor);
        cColor.setBackground(choice);
        colorDialog.dispose();
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentfigure, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
}