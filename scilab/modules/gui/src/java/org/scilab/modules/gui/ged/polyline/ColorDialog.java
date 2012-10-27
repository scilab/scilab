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
package org.scilab.modules.gui.ged.polyline;

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

import org.scilab.modules.gui.editor.EditorManager;

import org.scilab.modules.gui.ged.ColorMapHandler;
import org.scilab.modules.gui.ged.MessagesGED;

/**
* Implementation of the windows to color selection.
*
* @author Marcos Cardinot <mcardinot@gmail.com>
*/
public class ColorDialog extends Style {
    protected static JColorChooser chooserBack;
    protected static JButton okBack;
    protected static Color choiceBack;

    protected static JColorChooser chooserFore;
    protected static JButton okFore;
    protected static Color choiceFore;

    protected static JColorChooser chooserMarkBackground;
    protected static JButton okMarkBackground;
    protected static Color choiceMarkBackground;

    protected static JColorChooser chooserMarkForeground;
    protected static JButton okMarkForeground;
    protected static Color choiceMarkForeground;

    /**
    * Receives and passes the objectID to the parent class.
    * @param objectID Enters the identification of polyline.
    */
    public ColorDialog(String objectID) {
        super(objectID);
    }

    /**
    * JDialog - Selection of background colors.
    */
    @Override
    public void dialogBackgroundColor() {
        backcolorDialog = new JDialog();
        chooserBack = new JColorChooser();
        okBack = new JButton();

        backcolorDialog.setTitle(MessagesGED.choose_background_color);
        backcolorDialog.setMinimumSize(new Dimension(670, 450));
        backcolorDialog.setModal(true);
        backcolorDialog.setResizable(false);

        okBack.setText("OK");
        okBack.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okBackActionPerformed(evt);
            }
        });

        //Position settings.
        GroupLayout backcolorDialogLayout = new GroupLayout(backcolorDialog.getContentPane());
        backcolorDialog.getContentPane().setLayout(backcolorDialogLayout);
        backcolorDialogLayout.setHorizontalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserBack, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, backcolorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okBack)
                .addContainerGap())
        );
        backcolorDialogLayout.setVerticalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserBack, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
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
        forecolorDialog = new JDialog();
        chooserFore = new JColorChooser();
        okFore = new JButton();

        forecolorDialog.setTitle(MessagesGED.choose_foreground_color);
        forecolorDialog.setMinimumSize(new Dimension(670, 450));
        forecolorDialog.setModal(true);
        forecolorDialog.setResizable(false);

        okFore.setText("OK");
        okFore.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okForeActionPerformed(evt);
            }
        });

        //Position settings.
        GroupLayout forecolorDialogLayout = new GroupLayout(forecolorDialog.getContentPane());
        forecolorDialog.getContentPane().setLayout(forecolorDialogLayout);
        forecolorDialogLayout.setHorizontalGroup(
            forecolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(forecolorDialogLayout.createSequentialGroup()
                .addComponent(chooserFore, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, forecolorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okFore)
                .addContainerGap())
        );
        forecolorDialogLayout.setVerticalGroup(
            forecolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(forecolorDialogLayout.createSequentialGroup()
                .addComponent(chooserFore, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okFore)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * JDialog - Selection of mark background colors.
    */
    @Override
    public void dialogMarkBackground() {
        markBackgroundDialog = new JDialog();
        chooserMarkBackground = new JColorChooser();
        okMarkBackground = new JButton();

        markBackgroundDialog.setTitle(MessagesGED.choose_background_color);
        markBackgroundDialog.setMinimumSize(new Dimension(670, 450));
        markBackgroundDialog.setModal(true);
        markBackgroundDialog.setResizable(false);

        okMarkBackground.setText("OK");
        okMarkBackground.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okMarkBackgroundActionPerformed(evt);
            }
        });

        //Position settings.
        GroupLayout backcolorDialogLayout = new GroupLayout(markBackgroundDialog.getContentPane());
        markBackgroundDialog.getContentPane().setLayout(backcolorDialogLayout);
        backcolorDialogLayout.setHorizontalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserMarkBackground, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, backcolorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okMarkBackground)
                .addContainerGap())
        );
        backcolorDialogLayout.setVerticalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserMarkBackground, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okMarkBackground)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * JDialog - Selection of mark foreground colors.
    */
    @Override
    public void dialogMarkForeground() {
        markForegroundDialog = new JDialog();
        chooserMarkForeground = new JColorChooser();
        okMarkForeground = new JButton();

        markForegroundDialog.setTitle(MessagesGED.choose_foreground_color);
        markForegroundDialog.setMinimumSize(new Dimension(670, 450));
        markForegroundDialog.setModal(true);
        markForegroundDialog.setResizable(false);

        okMarkForeground.setText("OK");
        okMarkForeground.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent evt) {
                okMarkForegroundActionPerformed(evt);
            }
        });

        //Position settings.
        GroupLayout backcolorDialogLayout = new GroupLayout(markForegroundDialog.getContentPane());
        markForegroundDialog.getContentPane().setLayout(backcolorDialogLayout);
        backcolorDialogLayout.setHorizontalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserMarkForeground, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, Short.MAX_VALUE))
            .addGroup(GroupLayout.Alignment.TRAILING, backcolorDialogLayout.createSequentialGroup()
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(okMarkForeground)
                .addContainerGap())
        );
        backcolorDialogLayout.setVerticalGroup(
            backcolorDialogLayout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(backcolorDialogLayout.createSequentialGroup()
                .addComponent(chooserMarkForeground, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(okMarkForeground)
                .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okBackActionPerformed(ActionEvent evt) {
        choiceBack = chooserBack.getColor();
        double red = choiceBack.getRed();
        double green = choiceBack.getGreen();
        double blue = choiceBack.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setBackgroungColor(scilabColor);
        cBackColor.setBackground(choiceBack);
        backcolorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okForeActionPerformed(ActionEvent evt) {
        choiceFore = chooserFore.getColor();
        double red = choiceFore.getRed();
        double green = choiceFore.getGreen();
        double blue = choiceFore.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setForegroundColor(scilabColor);
        cForeColor.setBackground(choiceFore);
        forecolorDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okMarkBackgroundActionPerformed(ActionEvent evt) {
        choiceMarkBackground = chooserMarkBackground.getColor();
        double red = choiceMarkBackground.getRed();
        double green = choiceMarkBackground.getGreen();
        double blue = choiceMarkBackground.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setMarkBackground(scilabColor);
        cMarkBackground.setBackground(choiceMarkBackground);
        markBackgroundDialog.dispose();
    }

    /**
    * Implement the action on the OK button to save the color chosen by the user.
    *
    * @param evt ActionEvent.
    */
    private void okMarkForegroundActionPerformed(ActionEvent evt) {
        choiceMarkForeground = chooserMarkForeground.getColor();
        double red = choiceMarkForeground.getRed();
        double green = choiceMarkForeground.getGreen();
        double blue = choiceMarkForeground.getBlue();
        Integer scilabColor = ColorMapHandler.getScilabColor(red, green, blue, parentFigure);
        setMarkForeground(scilabColor);
        cMarkForeground.setBackground(choiceMarkForeground);
        markForegroundDialog.dispose();
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setBackgroungColor(Integer scilabColor) {
        GraphicController.getController()
              .setProperty(currentpolyline, GraphicObjectProperties.__GO_BACKGROUND__, scilabColor);
    }
    
    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setForegroundColor(Integer scilabColor) {
    	if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == false) {
    		EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	} else {
    		GraphicController.getController().setProperty(currentpolyline, GraphicObjectProperties.__GO_LINE_COLOR__, scilabColor);
    	}
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setMarkBackground(Integer scilabColor) {
        GraphicController.getController().setProperty(
                currentpolyline, GraphicObjectProperties.__GO_MARK_BACKGROUND__, scilabColor);
    }

    /**
    * Change the color of the object.
    *
    * @param scilabColor index of the color map.
    */
    private void setMarkForeground(Integer scilabColor) {
    	if ((Boolean)GraphicController.getController().getProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_MODE__) == true) {
    		EditorManager.getFromUid(parentFigure).setOriColor(scilabColor);
    	} else {
    		GraphicController.getController().setProperty(currentpolyline, GraphicObjectProperties.__GO_MARK_FOREGROUND__, scilabColor);
    	}
    }
}
