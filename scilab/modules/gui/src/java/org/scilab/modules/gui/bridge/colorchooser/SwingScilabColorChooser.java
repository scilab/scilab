/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JColorChooser;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;

import org.scilab.modules.gui.colorchooser.SimpleColorChooser;

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
	
	private JColorChooser colorChooser;
	
	/**
	 * Default constructor
	 * @param color the default Color
	 */
	public SwingScilabColorChooser(Color color) {
		super(new JFrame(), "Color Chooser", true);
		((JFrame) getParent()).setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());

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
	    
		/* Buttons panel */
	    JPanel buttonsContainer = new JPanel(new FlowLayout());
	    JPanel buttonsPanel = new JPanel(new GridLayout(1, 2, HGAP, VGAP));

	    JButton okButton = new JButton("Ok");
	    buttonsPanel.add(okButton);
	    getRootPane().setDefaultButton(okButton);
	    okButton.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
	        selectedColor = colorChooser.getColor();
	        dispose();
	        setVisible(false);
	      }
	    });

	    JButton canButton = new JButton("Cancel");
	    buttonsPanel.add(canButton);
	    canButton.addActionListener(new ActionListener() {
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
	public void setElementId(int id) {
		this.elementId = id;
	}
	
	/**
	 * Get the element id for this chooser
	 * @return id the id of the corresponding chooser object
	 */
	public int getElementId() {
		return this.elementId;
	}
	
	/** 
	 * Retrieve the selected color, or null 
	 * @return the selected color
	 */
	public Color getSelectedColor() {
		return selectedColor;
	}
	
	/** 
	 * Set the default color 
	 * @param color the default color
	 */
	public void setDefaultColor(Color color) {
		colorChooser.setColor(color);
	}
	
	/**
	 * Display the font chooser and wait for a user input
	 */
	public void displayAndWait() {
	    setVisible(true);
		
	}

}	
