/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.colorchooser;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.colorchooser.SimpleColorChooser;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;

/**
 * Swing implementation of a Scilab ColorChooser
 * @author Vincent COUVERT
 */
public class SwingScilabColorChooser extends JDialog implements SimpleColorChooser {

    private static final long serialVersionUID = 1L;

    private static final int HGAP = 10;
    private static final int VGAP = 2;

    private int elementId;

    private Color selectedColor;

    private final JColorChooser colorChooser;

    /**
     * Default constructor
     * @param color the default Color
     */
    public SwingScilabColorChooser(Color color) {
        super(new JFrame(), "Color Chooser", true);
        getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));

        /* Color chooser panel */
        JPanel chooserPanel = new JPanel(new BorderLayout());
        chooserPanel.setBorder(new TitledBorder(new EtchedBorder(), "Color"));
        colorChooser = new JColorChooser();
        chooserPanel.add(colorChooser);
        if (color != null) {
            colorChooser.setColor(color);
        }

        getContentPane().add(chooserPanel);

        ScilabSwingUtilities.closeOnEscape(this);

        /* Buttons panel */
        JPanel buttonsContainer = new JPanel(new FlowLayout());
        JPanel buttonsPanel = new JPanel(new GridLayout(1, 2, HGAP, VGAP));

        JButton okButton = new JButton("Ok");
        buttonsPanel.add(okButton);
        getRootPane().setDefaultButton(okButton);
        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectedColor = colorChooser.getColor();
                dispose();
                setVisible(false);
            }
        });

        JButton canButton = new JButton("Cancel");
        buttonsPanel.add(canButton);
        canButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectedColor = null;
                dispose();
                setVisible(false);
            }
        });

        buttonsContainer.add(buttonsPanel);
        getContentPane().add(buttonsContainer);

        pack();

        setResizable(false);
    }

    /**
     * Set the element id for this file chooser
     * @param id the id of the corresponding color chooser object
     */
    @Override
    public void setElementId(int id) {
        this.elementId = id;
    }

    /**
     * Get the element id for this chooser
     * @return id the id of the corresponding chooser object
     */
    @Override
    public int getElementId() {
        return this.elementId;
    }

    /**
     * Retrieve the selected color, or null
     * @return the selected color
     */
    @Override
    public Color getSelectedColor() {
        return selectedColor;
    }

    /**
     * Set the default color
     * @param color the default color
     */
    @Override
    public void setDefaultColor(Color color) {
        colorChooser.setColor(color);
    }

    /**
     * Display the font chooser and wait for a user input
     */
    @Override
    public void displayAndWait() {
        setVisible(true);
    }

}
