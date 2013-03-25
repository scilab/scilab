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
package org.scilab.modules.gui.ged.graphic_objects.axes;

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
    protected JColorChooser chooser;
    protected JButton okBack;
    protected Color choice;

    protected JColorChooser chooser1;
    protected JButton okFore;
    protected Color choice1;

    protected JColorChooser chooser2;
    protected JButton okFont;
    protected Color choice2;

    /**
    * Receives and passes the objectID to the parent class.
    *
    * @param objectID Enters the identification of Axes.
    */
    public ColorDialog(String objectID) {
        super(objectID);
    }

    /**
    * JDialog - Selection of background colors.
    */
    @Override
    public void dialogBackgroundColor() {
        backColorDialog = new JDialog();
        chooser = new JColorChooser();
        okBack = new JButton();

        backColorDialog.setTitle(MessagesGED.choose_background_color);
        backColorDialog.setMinimumSize(new Dimension(670, 450));
        backColorDialog.setModal(true);
        backColorDialog.setResizable(false);

        okBack.setText("OK");
        okBack.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okBackActionPerformed(evt);
            }
        });

        GroupLayout backColorDialogLayout = new GroupLayout(backColorDialog.getContentPane());
        backColorDialog.getContentPane().setLayout(backColorDialogLayout);
        backColorDialogLayout.setHorizontalGroup(
            backColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backColorDialogLayout.createSequentialGroup()
                .addComponent(chooser, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, backColorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okBack)
                .addContainerGap())
        );
        backColorDialogLayout.setVerticalGroup(
            backColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backColorDialogLayout.createSequentialGroup()
                .addComponent(chooser, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okBack)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * JDialog - Selection of foreground colors.
    */
    @Override
    public void dialogForegroundColor() {        
        foreColorDialog = new JDialog();
        chooser1 = new JColorChooser();
        okFore = new JButton();

        foreColorDialog.setTitle(MessagesGED.choose_foreground_color);
        foreColorDialog.setMinimumSize(new Dimension(670, 450));
        foreColorDialog.setModal(true);
        foreColorDialog.setResizable(false);

        okFore.setText("OK");
        okFore.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okForeActionPerformed(evt);
            }
        });

        GroupLayout foreColorDialogLayout = new GroupLayout(foreColorDialog.getContentPane());
        foreColorDialog.getContentPane().setLayout(foreColorDialogLayout);
        foreColorDialogLayout.setHorizontalGroup(
            foreColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(foreColorDialogLayout.createSequentialGroup()
                .addComponent(chooser1, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, foreColorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFore)
                .addContainerGap())
        );
        foreColorDialogLayout.setVerticalGroup(
            foreColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(foreColorDialogLayout.createSequentialGroup()
                .addComponent(chooser1, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFore)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * JDialog - Selection of font colors.
    */
    @Override
    public void dialogFontColor() {
        fontColorDialog = new JDialog();
        chooser2 = new JColorChooser();
        okFont = new JButton();

        fontColorDialog.setTitle(MessagesGED.choose_font_color);
        fontColorDialog.setMinimumSize(new Dimension(670, 450));
        fontColorDialog.setModal(true);
        fontColorDialog.setResizable(false);

        okFont.setText("OK");
        okFont.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okFontActionPerformed(evt);
            }
        });

        GroupLayout fontColorDialogLayout = new GroupLayout(fontColorDialog.getContentPane());
        fontColorDialog.getContentPane().setLayout(fontColorDialogLayout);
        fontColorDialogLayout.setHorizontalGroup(
            fontColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(fontColorDialogLayout.createSequentialGroup()
                .addComponent(chooser2, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 10, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, fontColorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFont)
                .addContainerGap())
        );
        fontColorDialogLayout.setVerticalGroup(
            fontColorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(fontColorDialogLayout.createSequentialGroup()
                .addComponent(chooser2, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFont)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okBackActionPerformed(ActionEvent evt) {
        choice = chooser.getColor();
        double red = choice.getRed();
        double green = choice.getGreen();
        double blue = choice.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setBackgroungColor(scilabColor);
        cBackColor.setBackground(choice);
        backColorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okForeActionPerformed(ActionEvent evt) {
        choice1 = chooser1.getColor();
        double red = choice1.getRed();
        double green = choice1.getGreen();
        double blue = choice1.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setForegroundColor(scilabColor);
        cForeColor.setBackground(choice1);
        foreColorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okFontActionPerformed(ActionEvent evt) {
        choice2 = chooser2.getColor();
        double red = choice2.getRed();
        double green = choice2.getGreen();
        double blue = choice2.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setFontColor(scilabColor);
        cFontColor.setBackground(choice2);
        fontColorDialog.dispose();
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setBackgroungColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setForegroundColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setFontColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentaxes, GraphicObjectProperties.__GO_FONT_COLOR__, scilabColor);
    }
}