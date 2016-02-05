/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
 * Copyright (C) 2010 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.bridge.fontchooser;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GraphicsEnvironment;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

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

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.fontchooser.SimpleFontChooser;
import org.scilab.modules.gui.utils.ScilabFontUtils;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.localization.Messages;

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
    private static final int DEFAULT_FONT_SIZE = 12;

    private int elementId;

    private final JList fontNameList;
    private final String[] availableFontNames;

    private final JList fontSizeList;
    private final String[] availableFontSizes = {"8", "10", "11", "12",
                                                 "13", "14", "16", "18", "20",
                                                 "24", "30", "36", "40",
                                                 "48", "60", "72"
                                                };

    private final boolean calledFromScinotes;
    private final JCheckBox boldCheckbox;
    private final JCheckBox italicCheckbox;
    private final JCheckBox monospacedCheckbox;
    private JCheckBox scinotesCompatibleCheckbox;

    private final JLabel preview;
    private final String previewText = "The Quick Brown Fox Jumps Over The Lazy Dog";

    /* User choices */
    private Font selectedFont;

    /**
     * Default constructor
     * @param font the default font
     */
    public SwingScilabFontChooser(Font font) {
        this(new JFrame(), font, false);
    }

    /**
     * Default constructor
     * @param owner the Dialog owner
     * @param font the default font
     * @param calledFromScinotes true if called from SciNotes (specific options to display)
     */
    public SwingScilabFontChooser(JFrame owner, Font font, boolean calledFromScinotes) {
        super(owner, Messages.gettext("Font Chooser"), true);
        this.calledFromScinotes = calledFromScinotes;
        ((JFrame) getParent()).setIconImage(new ImageIcon(FindIconHelper.findIcon("scilab", "256x256")).getImage());

        getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));

        /* Panel used to display available fonts and sizes */
        JPanel fontPanel = new JPanel(new GridLayout(1, 2, HGAP, VGAP));
        fontPanel.setBorder(new TitledBorder(new EtchedBorder(), Messages.gettext("Font")));

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


        ScilabSwingUtilities.closeOnEscape(this);

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
        JPanel attributes = new JPanel(new GridLayout(2, 2, HGAP, VGAP));
        attributes.setBorder(new TitledBorder(new EtchedBorder(), Messages.gettext("Attributes")));
        boldCheckbox = new JCheckBox(Messages.gettext("Bold"), (font != null) && font.isBold());
        attributes.add(boldCheckbox);
        italicCheckbox = new JCheckBox(Messages.gettext("Italic"), (font != null) && font.isItalic());
        attributes.add(italicCheckbox);
        monospacedCheckbox = new JCheckBox(Messages.gettext("Monospaced"), false);
        attributes.add(monospacedCheckbox);
        monospacedCheckbox.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                updateFontNameList();
            }
        });

        if (calledFromScinotes) {
            boldCheckbox.setEnabled(false);
            italicCheckbox.setEnabled(false);
            scinotesCompatibleCheckbox = new JCheckBox(Messages.gettext("SciNotes compatible"), false);
            attributes.add(scinotesCompatibleCheckbox);
            scinotesCompatibleCheckbox.addItemListener(new ItemListener() {
                @Override
                public void itemStateChanged(ItemEvent e) {
                    updateFontNameList();
                }
            });
        }
        getContentPane().add(attributes);

        /* Preview panel */
        JPanel previewPanel = new JPanel(new BorderLayout());
        previewPanel.setBorder(new TitledBorder(new EtchedBorder(), Messages.gettext("Preview")));
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

        JButton okButton = new JButton(Messages.gettext("Ok"));
        buttonsPanel.add(okButton);
        getRootPane().setDefaultButton(okButton);
        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                previewFont();
                dispose();
                setVisible(false);
            }
        });

        JButton canButton = new JButton(Messages.gettext("Cancel"));
        buttonsPanel.add(canButton);
        canButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                selectedFont = null;
                dispose();
                setVisible(false);
            }
        });

        buttonsContainer.add(buttonsPanel);
        getContentPane().add(buttonsContainer);

        fontSizeList.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent arg0) {
                previewFont();
            }
        });
        fontNameList.addListSelectionListener(new ListSelectionListener() {
            @Override
            public void valueChanged(ListSelectionEvent arg0) {
                previewFont();
            }
        });
        boldCheckbox.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent arg0) {
                previewFont();
            }
        });
        italicCheckbox.addChangeListener(new ChangeListener() {
            @Override
            public void stateChanged(ChangeEvent arg0) {
                previewFont();
            }
        });

        previewFont();

        pack();

        setPreferredSize(getSize());

        setResizable(false);
    }

    /**
     * Set the element id for this file chooser
     * @param id the id of the corresponding file chooser object
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
     * Retrieve the selected font, or null
     * @return the selected font
     */
    @Override
    public Font getSelectedFont() {
        return selectedFont;
    }

    /**
     * Update the preview label
     */
    protected void previewFont() {
        if (fontNameList.getSelectedIndex() == -1 || fontSizeList.getSelectedIndex() == -1) {
            return;
        }
        String selectedFontName = (String) fontNameList.getSelectedValue();
        String resultSizeName = (String) fontSizeList.getSelectedValue();
        int selectedFontSize = Integer.parseInt(resultSizeName);
        int attrs = Font.PLAIN;

        boolean selectedBold = boldCheckbox.isSelected();
        boolean selectedItalic = italicCheckbox.isSelected();
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
    @Override
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
    @Override
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
    @Override
    public void setDefaultBold(boolean bold) {
        boldCheckbox.setSelected(bold);
    }

    /**
     * Set the default font italic attribute
     * @param italic the default font italic attribute
     */
    @Override
    public void setDefaultItalic(boolean italic) {
        italicCheckbox.setSelected(italic);
    }

    /**
     * Display the font chooser and wait for a user input
     */
    @Override
    public void displayAndWait() {
        setVisible(true);
    }

    /**
     * Update font names list according to the checkboxes status
     */
    private void updateFontNameList() {
        boolean monospacedMode = monospacedCheckbox.isSelected();
        boolean scinotesMode = calledFromScinotes && scinotesCompatibleCheckbox.isSelected();

        setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

        ((DefaultListModel) fontNameList.getModel()).removeAllElements();

        if (!monospacedMode & !scinotesMode) {
            /* Display all fonts */
            for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
                ((DefaultListModel) fontNameList.getModel()).addElement(availableFontNames[fontIndex]);
            }
        }

        if (monospacedMode & !scinotesMode) {
            /* Display only monospaced fonts */
            for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
                if (ScilabFontUtils.isMonospaced(new Font(availableFontNames[fontIndex], Font.PLAIN, DEFAULT_FONT_SIZE))) {
                    ((DefaultListModel) fontNameList.getModel()).addElement(availableFontNames[fontIndex]);
                }
            }
        }

        if (!monospacedMode & scinotesMode) {
            /* Display only SciNotes compatibles fonts */
            for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
                if (ScilabFontUtils.isAllStylesSameWidths(availableFontNames[fontIndex])) {
                    ((DefaultListModel) fontNameList.getModel()).addElement(availableFontNames[fontIndex]);
                }
            }
        }

        if (monospacedMode & scinotesMode) {
            /* Display only SciNotes compatibles fonts which are monospaced */
            for (int fontIndex = 0; fontIndex < availableFontNames.length; fontIndex++) {
                if (ScilabFontUtils.isMonospaced(new Font(availableFontNames[fontIndex], Font.PLAIN, DEFAULT_FONT_SIZE))
                        & ScilabFontUtils.isAllStylesSameWidths(availableFontNames[fontIndex])) {
                    ((DefaultListModel) fontNameList.getModel()).addElement(availableFontNames[fontIndex]);
                }
            }
        }

        setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
    }
}
