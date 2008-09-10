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

package org.scilab.modules.gui.bridge.fontchooser;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GraphicsEnvironment;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListSelectionModel;
import javax.swing.border.EtchedBorder;
import javax.swing.border.LineBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.scilab.modules.gui.fontchooser.SimpleFontChooser;

/**
 * Swing implementation of a Scilab FileChooser
 * @author Vincent COUVERT
 */
public class SwingScilabFontChooser extends JDialog implements SimpleFontChooser {
	
	private static final long serialVersionUID = 1L;

	private static final int HGAP = 10;
	private static final int VGAP = 2;
	private static final int PREVIEW_WIDTH = 120;
	private static final int PREVIEW_HEIGHT = 40;
	
	private int elementId;
	
	private JList fontNameList;
	private String[] availableFontNames;

	private JList fontSizeList;
	private String[] availableFontSizes = {"8", "10", "11", "12", 
										   "13", "14", "16", "18", "20",
										   "24", "30", "36", "40",
										    "48", "60", "72" };
	
	private JCheckBox boldCheckbox;
	private JCheckBox italicCheckbox;
	
	private JLabel preview;
	private String previewText = "The Quick Brown Fox Jumps Over The Lazy Dog";
	
	/* User choices */
	private Font selectedFont;
	
	/**
	 * Default constructor
	 * @param font the default font
	 */
	public SwingScilabFontChooser(Font font) {
		super(new JFrame(), "Font Chooser", true);
		((JFrame) getParent()).setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());

		getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
		
		/* Panel used to display available fonts and sizes */
		JPanel fontPanel = new JPanel(new GridLayout(1, 2, HGAP, VGAP));
		fontPanel.setBorder(new TitledBorder(new EtchedBorder(), "Font"));
		
		/* Font names list */
		fontNameList = new JList();
		fontNameList.setModel(new DefaultListModel());
		fontNameList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		fontPanel.add(new JScrollPane(fontNameList));
		
		/* Get the system fonts */
		availableFontNames = GraphicsEnvironment.getLocalGraphicsEnvironment().getAvailableFontFamilyNames();
		
		/* Add the font names to the list */
		for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
			((DefaultListModel) fontNameList.getModel()).addElement(availableFontNames[fontIndex]);
			if ((font != null) && (availableFontNames[fontIndex].equals(font.getFamily()))) {
				fontNameList.setSelectedIndex(fontIndex);
			}
		}
		if (fontNameList.getSelectedIndex() == -1) {
			fontNameList.setSelectedIndex(0);
		}
		
		
		/* Font size list */
		fontSizeList = new JList();
		fontSizeList.setModel(new DefaultListModel());
		fontSizeList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		fontPanel.add(new JScrollPane(fontSizeList));
		
		/* Add the font sizes to the list */
		for (int sizeIndex = 0; sizeIndex < availableFontSizes.length; sizeIndex++) {
			((DefaultListModel) fontSizeList.getModel()).addElement(availableFontSizes[sizeIndex]);
			if ((font != null) && (availableFontSizes[sizeIndex].equals(Integer.toString(font.getSize())))) {
				fontSizeList.setSelectedIndex(sizeIndex);
			}
		}
		if (fontSizeList.getSelectedIndex() == -1) {
			fontSizeList.setSelectedIndex(0);
		}

		
		getContentPane().add(fontPanel);

		/* Attributes panel */
		JPanel attributes = new JPanel(new GridLayout(1, 2, HGAP, VGAP));
		attributes.setBorder(new TitledBorder(new EtchedBorder(), "Attributes"));
	    boldCheckbox = new JCheckBox("Bold", (font != null) && font.isBold());
	    attributes.add(boldCheckbox);
	    italicCheckbox = new JCheckBox("Italic", (font != null) && font.isItalic());
	    attributes.add(italicCheckbox);
	    
	    getContentPane().add(attributes);
	    
	    /* Preview panel */
	    JPanel previewPanel = new JPanel(new BorderLayout());
	    previewPanel.setBorder(new TitledBorder(new EtchedBorder(), "Preview"));
	    preview = new JLabel(previewText, JLabel.CENTER);
	    preview.setBackground(Color.white);
	    preview.setForeground(Color.black);
	    preview.setOpaque(true);
	    preview.setBorder(new LineBorder(Color.black));
	    preview.setPreferredSize(new Dimension(PREVIEW_WIDTH, PREVIEW_HEIGHT));
	    previewPanel.add(preview, BorderLayout.CENTER);
	
		getContentPane().add(previewPanel);
		
		/* Buttons panel */
	    JPanel buttonsContainer = new JPanel(new FlowLayout());
	    JPanel buttonsPanel = new JPanel(new GridLayout(1, 2, HGAP, VGAP));

	    JButton okButton = new JButton("Ok");
	    buttonsPanel.add(okButton);
	    getRootPane().setDefaultButton(okButton);
	    okButton.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
	        previewFont();
	        dispose();
	        setVisible(false);
	      }
	    });

	    JButton canButton = new JButton("Cancel");
	    buttonsPanel.add(canButton);
	    canButton.addActionListener(new ActionListener() {
	      public void actionPerformed(ActionEvent e) {
	        selectedFont = null;
	        dispose();
	        setVisible(false);
	      }
	    });

	    buttonsContainer.add(buttonsPanel);
	    getContentPane().add(buttonsContainer);

		fontSizeList.addListSelectionListener(new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent arg0) {
				previewFont();
			}
		});
		fontNameList.addListSelectionListener(new ListSelectionListener() {
			public void valueChanged(ListSelectionEvent arg0) {
				previewFont();
			}
		});
		boldCheckbox.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				previewFont();
			}
		});
		italicCheckbox.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent arg0) {
				previewFont();
			}
		});

	    previewFont();

	    pack();
	    
	    setResizable(false);
	}

	/**
	 * Set the element id for this file chooser
	 * @param id the id of the corresponding file chooser object
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
	 * Retrieve the selected font, or null 
	 * @return the selected font
	 */
	public Font getSelectedFont() {
		return selectedFont;
	}

	/**
	 * Update the preview label
	 */
	protected void previewFont() {
		String selectedFontName = availableFontNames[fontNameList.getSelectedIndex()];
		String resultSizeName = availableFontSizes[fontSizeList.getSelectedIndex()];
		int selectedFontSize = Integer.parseInt(resultSizeName);
		boolean selectedBold = boldCheckbox.isSelected();
		boolean selectedItalic = italicCheckbox.isSelected();
		int attrs = Font.PLAIN;
		if (selectedBold) {
			attrs = Font.BOLD;
		}
		if (selectedItalic) {
		    attrs |= Font.ITALIC;
		}
		selectedFont = new Font(selectedFontName, attrs, selectedFontSize);
		preview.setFont(selectedFont);
		pack();
	}

	/** 
	 * Set the default font name 
	 * @param fontName the default font name
	 */
	public void setDefaultFontName(String fontName) {
		for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
			if (availableFontNames[fontIndex].equalsIgnoreCase(fontName)) {
				fontNameList.setSelectedIndex(fontIndex);
			}
		}
	}

	/** 
	 * Set the default font size 
	 * @param fontSize the default font size
	 */
	public void setDefaultFontSize(int fontSize) {
		for (int sizeIndex = 0; sizeIndex < availableFontSizes.length; sizeIndex++) {
			if (availableFontSizes[sizeIndex].equals(Integer.toString(fontSize))) {
				fontSizeList.setSelectedIndex(sizeIndex);
			}
		}
	}

	/** 
	 * Set the default font bold attribute 
	 * @param bold the default font bold attribute
	 */
	public void setDefaultBold(boolean bold) {
		boldCheckbox.setSelected(bold);
	}

	/** 
	 * Set the default font italic attribute 
	 * @param italic the default font italic attribute
	 */
	public void setDefaultItalic(boolean italic) {
		italicCheckbox.setSelected(italic);
	}
	
	/**
	 * Display the font chooser and wait for a user input
	 */
	public void displayAndWait() {
	    setVisible(true);
	}

}
