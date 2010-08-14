/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.scinotes.actions;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.MouseEvent;
import java.util.Map;
import java.util.List;
import java.util.Iterator;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.KeyStroke;
import javax.swing.ListSelectionModel;
import javax.swing.text.DefaultCaret;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import org.scilab.modules.gui.bridge.colorchooser.SwingScilabColorChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.KeywordAdaptater;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Action called to customize SciNotes fonts & styles
 * @author Vincent COUVERT
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public final class SetColorsAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    private static final int GAP = 5;
    private static final int THREE = 3;
    private static final int FOUR = 4;
    private static final int FIVE = 5;

    private static boolean windowAlreadyExist;

    /* List of all components */
    private static JFrame frame;

    private JPanel contentPanel;
    private JPanel settingsPanel;
    private JPanel previewPanel;
    private JPanel buttonsPanel;
    private JButton cancelButton;
    private JButton defaultButton;
    private JButton okButton;
    private JScrollPane stylesScrollPane;
    private JList stylesList;
    private JLabel colorLabel;
    private JButton colorButton;
    private JButton bgColorButton;
    private JButton fgColorButton;
    private JCheckBox boldCheckBox;
    private JCheckBox italicCheckBox;
    private JCheckBox strikethroughCheckBox;
    private JCheckBox underlineCheckBox;
    private ScilabEditorPane previewEditorPane;

    private List<String> listStylesName;
    private int numberOfStyles;

    private Map<String, Color> allStylesColor;
    private Map<String, Boolean> allStylesIsBold;
    private Map<String, Boolean> allStylesIsItalic;
    private Map<String, Integer> allAttributes;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor Scilab editor instance
     */
    public SetColorsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Displays the customization window
     */
    public void doAction() {
        if (!windowAlreadyExist) {
            windowAlreadyExist = true;
            changeColorsBox();
        } else {
            frame.setVisible(true);
        }
    }

    /**
     * Create the associated menu
     * @param label label of the menu
     * @param editor Scilab editor instance
     * @param key KeyStroke
     * @return the menu
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SetColorsAction(label, editor), key);
    }

    /**
     * Create the customization window
     */
    private void changeColorsBox() {

        allStylesColor = ConfigSciNotesManager.getAllForegroundColors();
        allStylesIsBold = ConfigSciNotesManager.getAllisBold();
        allStylesIsItalic = ConfigSciNotesManager.getAllisItalic();
        allAttributes = ConfigSciNotesManager.getAllAttributes();

        /* Main frame = Window */
        frame = new JFrame();
        frame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setTitle(SciNotesMessages.SET_COLORS);
        frame.setResizable(false);

        /* Main content pane */
        contentPanel = new JPanel();
        contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.PAGE_AXIS));
        contentPanel.setBorder(BorderFactory.createEmptyBorder(2 * GAP, 2 * GAP, 2 * GAP, 2 * GAP));

        /* Settings */
        settingsPanel = new JPanel(new GridBagLayout());
        settingsPanel.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.SETTINGS));
        stylesList = new JList();
        stylesList.setModel(new DefaultListModel());
        listStylesName  =  ConfigSciNotesManager.getAllStyleName();
        numberOfStyles = listStylesName.size();
        for (int i = 0; i < numberOfStyles; i++) {
            ((DefaultListModel) stylesList.getModel()).addElement(listStylesName.get(i));
        }

        stylesScrollPane = new JScrollPane(stylesList);
        stylesList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        stylesList.addListSelectionListener(new ListSelectionListener() {

                public void valueChanged(ListSelectionEvent arg0) {
                    /* Update the GUI */
                    settingsUpdate();
                }

            });

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridx = 1;
        gbc.gridy = 1;
        gbc.insets = new Insets(GAP, GAP, GAP, GAP);
        gbc.gridheight = GridBagConstraints.REMAINDER;

        settingsPanel.add(stylesScrollPane, gbc);

        colorLabel = new JLabel(SciNotesMessages.COLOR);
        colorButton = new JButton(SciNotesMessages.CHANGE_COLOR);
        colorButton.setContentAreaFilled(true);
        colorButton.setOpaque(true);
        colorButton.addActionListener(new ActionListener() {

                public void actionPerformed(ActionEvent e) {
                    int selectedStyleIndex = 0;

                    /* Which style is selected in the list? */
                    selectedStyleIndex = stylesList.getSelectedIndex();

                    /* Launch a color chooser */
                    Color previousColor = allStylesColor.get(listStylesName.get(selectedStyleIndex));
                    SwingScilabColorChooser colorChooser = new SwingScilabColorChooser(previousColor);
                    colorChooser.displayAndWait();
                    Color newColor = colorChooser.getSelectedColor();

                    if (newColor != null) {
                        /* Update the styles */
                        allStylesColor.put(listStylesName.get(selectedStyleIndex), newColor);
                        /* Update the GUI */
                        settingsUpdate();
                    }

                    frame.setFocusable(true);
                }
            });

        gbc.gridx = 2;
        gbc.gridheight = 1;
        gbc.anchor = GridBagConstraints.LINE_START;
        gbc.insets = new Insets(GAP, THREE * GAP, GAP, GAP);
        settingsPanel.add(colorLabel, gbc);
        gbc.gridx = THREE;
        gbc.anchor = GridBagConstraints.CENTER;
        gbc.insets = new Insets(GAP, GAP, GAP, GAP);
        settingsPanel.add(colorButton, gbc);

        gbc.gridy = 2;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.insets = new Insets(GAP, GAP, GAP, GAP);
        bgColorButton = new JButton(SciNotesMessages.BGCOLOR);
        bgColorButton.setContentAreaFilled(true);
        bgColorButton.setOpaque(true);
        bgColorButton.setBackground(ConfigSciNotesManager.getSciNotesBackgroundColor());
        bgColorButton.addActionListener(new ActionListener() {

                public void actionPerformed(ActionEvent e) {
                    Color previousColor = bgColorButton.getBackground();
                    SwingScilabColorChooser colorChooser = new SwingScilabColorChooser(previousColor);
                    colorChooser.displayAndWait();
                    Color newColor = colorChooser.getSelectedColor();

                    if (newColor != null) {
                        previewEditorPane.setBackground(newColor);
                        bgColorButton.setBackground(newColor);
                    }

                    frame.setFocusable(true);
                }
            });

        settingsPanel.add(bgColorButton, gbc);

        gbc.gridy = 3;
        gbc.anchor = GridBagConstraints.WEST;
        gbc.insets = new Insets(GAP, GAP, GAP, GAP);
        fgColorButton = new JButton(SciNotesMessages.CARET_COLOR);
        fgColorButton.setContentAreaFilled(true);
        fgColorButton.setOpaque(true);
        fgColorButton.setBackground(ConfigSciNotesManager.getSciNotesForegroundColor());
        fgColorButton.addActionListener(new ActionListener() {

                public void actionPerformed(ActionEvent e) {
                    Color previousColor = fgColorButton.getBackground();
                    SwingScilabColorChooser colorChooser = new SwingScilabColorChooser(previousColor);
                    colorChooser.displayAndWait();
                    Color newColor = colorChooser.getSelectedColor();

                    if (newColor != null) {
                        previewEditorPane.setCaretColor(newColor);
                        previewEditorPane.getCaret().setVisible(true);
                        previewEditorPane.repaint();
                        fgColorButton.setBackground(newColor);
                    }

                    frame.setFocusable(true);
                }
            });

        settingsPanel.add(fgColorButton, gbc);

        gbc.gridx = 2;
        gbc.gridy = 2;
        gbc.insets = new Insets(GAP, THREE * GAP, GAP, GAP);
        gbc.anchor = GridBagConstraints.LINE_START;
        boldCheckBox = new JCheckBox(SciNotesMessages.BOLD);
        boldCheckBox.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int selectedStyleIndex = stylesList.getSelectedIndex();
                    String styleName = listStylesName.get(selectedStyleIndex);
                    allStylesIsBold.put(styleName, boldCheckBox.isSelected());
                    settingsUpdate();
                }
            });
        settingsPanel.add(boldCheckBox, gbc);

        gbc.gridy = THREE;
        italicCheckBox = new JCheckBox(SciNotesMessages.ITALIC);
        italicCheckBox.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int selectedStyleIndex = stylesList.getSelectedIndex();
                    String styleName = listStylesName.get(selectedStyleIndex);
                    allStylesIsItalic.put(styleName, italicCheckBox.isSelected());
                    settingsUpdate();
                }
            });
        settingsPanel.add(italicCheckBox, gbc);

        gbc.gridy = FOUR;
        strikethroughCheckBox = new JCheckBox(SciNotesMessages.STRIKETHROUGH);
        strikethroughCheckBox.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int selectedStyleIndex = stylesList.getSelectedIndex();
                    String styleName = listStylesName.get(selectedStyleIndex);
                    int stroke = 0;
                    if (strikethroughCheckBox.isSelected()) {
                        stroke = 2;
                    }
                    int actual = allAttributes.get(styleName);
                    allAttributes.put(styleName, (actual & 1) | stroke);
                    settingsUpdate();
                }
            });
        settingsPanel.add(strikethroughCheckBox, gbc);

        gbc.gridy = FIVE;
        underlineCheckBox = new JCheckBox(SciNotesMessages.UNDERLINE);
        underlineCheckBox.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    int selectedStyleIndex = stylesList.getSelectedIndex();
                    String styleName = listStylesName.get(selectedStyleIndex);
                    int under = 0;
                    if (underlineCheckBox.isSelected()) {
                        under = 1;
                    }
                    int actual = allAttributes.get(styleName);
                    allAttributes.put(styleName, (actual & 2) | under);
                    settingsUpdate();
                }
            });
        settingsPanel.add(underlineCheckBox, gbc);

        contentPanel.add(settingsPanel);

        /* Preview */
        previewPanel = new JPanel(new BorderLayout(GAP, GAP));
        previewPanel.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.PREVIEW));
        previewEditorPane = new ScilabEditorPane(getEditor());
        previewEditorPane.setEditorKit(new ScilabEditorKit());
        previewEditorPane.setText("// A comment with whites    and tabulations \t\t\n"
                                  + "// Email: <scilab.support@scilab.org>\n"
                                  + "// Scilab editor: http://www.scilab.org/\n"
                                  + "function [a, b] = myfunction(d, e, f)\n"
                                  + "\ta = 2.71828 + %pi + f($, :);\n"
                                  + "\tb = cos(a) + cosh(a);\n"
                                  + "\tif d == e then\n"
                                  + "\t\tb = 10 - e.field;\n"
                                  + "\telse\n"
                                  + "\t\tb = \"\t\ttest     \" + home\n"
                                  + "\t\treturn\n"
                                  + "\tend\n"
                                  + "\tmyvar = 1.23e-45;\n"
                                  + "endfunction");

        previewEditorPane.setBackground(bgColorButton.getBackground());
        previewEditorPane.setFont(ConfigSciNotesManager.getFont());
        previewEditorPane.setCaret(new DefaultCaret() {
                public void mouseDragged(MouseEvent e) {
                    e.consume();
                }
            });
        previewEditorPane.getCaret().setBlinkRate(500);
        previewEditorPane.setCaretColor(fgColorButton.getBackground());
        previewEditorPane.getCaret().setVisible(true);
        previewEditorPane.setEditable(false);
        previewEditorPane.addKeywordListener(new KeywordAdaptater.MouseOverAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    previewEditorPane.setToolTipText(ScilabLexerConstants.getStringRep(e.getType()));
                }
            });
        previewEditorPane.addKeywordListener(new KeywordAdaptater.MouseClickedAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    stylesList.setSelectedValue(ScilabLexerConstants.getStringRep(e.getType()), true);
                }
            });
        previewPanel.add(previewEditorPane, BorderLayout.CENTER);
        contentPanel.add(previewPanel);

        /* Buttons */
        buttonsPanel = new JPanel();
        buttonsPanel.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        buttonsPanel.setLayout(new GridLayout(1, FOUR, GAP, GAP));

        okButton  = new JButton(SciNotesMessages.OK);
        okButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    /* Apply all new settings */
                    int numberOfTab = getEditor().getTabPane().getTabCount();
                    for (int j = 0; j < numberOfTab; j++) {
                        ScilabEditorPane textPane = getEditor().getTextPane(j);
                        for (int i = 0; i < numberOfStyles; i++) {
                            String name = listStylesName.get(i);
                            int bold = -1;
                            if (allStylesIsBold.get(name)) {
                                bold = 1;
                            }
                            int italic = -2;
                            if (allStylesIsItalic.get(name)) {
                                italic = 2;
                            }
                            textPane.resetColor(name, allStylesColor.get(name));
                            textPane.resetFont(name, bold);
                            textPane.resetFont(name, italic);
                            textPane.resetAttribute(name, allAttributes.get(name));
                            textPane.setBackground(bgColorButton.getBackground());
                            textPane.setCaretColor(fgColorButton.getBackground());
                            if (textPane.getOtherPaneInSplit() != null) {
                                textPane.getOtherPaneInSplit().resetColor(name, allStylesColor.get(name));
                                textPane.getOtherPaneInSplit().resetFont(name, bold);
                                textPane.getOtherPaneInSplit().resetFont(name, italic);
                                textPane.getOtherPaneInSplit().resetAttribute(name, allAttributes.get(name));
                                textPane.getOtherPaneInSplit().setBackground(bgColorButton.getBackground());
                                textPane.getOtherPaneInSplit().setCaretColor(fgColorButton.getBackground());
                            }
                        }
                    }
                    /* Save the setting in the configuration file */
                    ConfigSciNotesManager.saveAllForegroundColors(allStylesColor);
                    ConfigSciNotesManager.saveAllFontStyle(allStylesIsBold, allStylesIsItalic);
                    ConfigSciNotesManager.saveAllAttributes(allAttributes);
                    ConfigSciNotesManager.saveSciNotesBackground(bgColorButton.getBackground());
                    ConfigSciNotesManager.saveSciNotesForeground(fgColorButton.getBackground());
                    windowAlreadyExist = false;
                    frame.dispose();
                }
            });

        cancelButton = new JButton(SciNotesMessages.CANCEL);
        cancelButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    windowAlreadyExist = false;
                    frame.dispose();
                }
            });

        defaultButton  = new JButton(SciNotesMessages.DEFAULT);
        defaultButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    settingsDefault();
                }
            });

        buttonsPanel.add(new JLabel()); /* Empty label used to move button to the right */
        buttonsPanel.add(okButton);
        buttonsPanel.add(cancelButton);
        buttonsPanel.add(defaultButton);

        contentPanel.add(buttonsPanel);

        frame.setContentPane(contentPanel);
        frame.addWindowListener(new WindowListener() {
                public void windowClosed(WindowEvent arg0) {
                }
                public void windowDeiconified(WindowEvent arg0) {
                }
                public void windowActivated(WindowEvent arg0) {
                }
                public void windowClosing(WindowEvent arg0) {
                    SetColorsAction.windowAlreadyExist = false;
                    frame.dispose();
                }
                public void windowDeactivated(WindowEvent arg0) {
                }
                public void windowIconified(WindowEvent arg0) {
                };
                public void windowOpened(WindowEvent arg0) {
                }
            });

        /* Select the default style */
        stylesList.setSelectedIndex(0);

        frame.pack();
        frame.setLocationRelativeTo(getEditor());
        frame.setVisible(true);

    }

    /**
     *  Close this window (used from SciNotes.java when closing SciNotes)
     */
    public static void closeSetColorsWindow() {
        SetColorsAction.windowAlreadyExist = false;
        if (frame != null) {
            frame.dispose();
        }
    }

    /**
     * Update the GUI according to the selected style
     */
    private void settingsUpdate() {
        /* Get the selected item */
        int selectedStyleIndex = stylesList.getSelectedIndex();
        String styleName = listStylesName.get(selectedStyleIndex);
        previewEditorPane.resetColor(styleName, allStylesColor.get(styleName));
        int bold = -1;
        if (allStylesIsBold.get(styleName)) {
            bold = 1;
        }
        previewEditorPane.resetFont(styleName, bold);
        int italic = -2;
        if (allStylesIsItalic.get(styleName)) {
            italic = 2;
        }
        previewEditorPane.resetFont(styleName, italic);
        previewEditorPane.resetAttribute(styleName, allAttributes.get(styleName));
        previewEditorPane.getCaret().setVisible(true);
        previewEditorPane.repaint();

        /* Update the GUI */
        colorButton.setBackground(allStylesColor.get(styleName));
        //colorButton.fireStateChanged();
        boldCheckBox.setSelected(allStylesIsBold.get(styleName));
        italicCheckBox.setSelected(allStylesIsItalic.get(styleName));
        underlineCheckBox.setSelected((allAttributes.get(styleName) & 1) == 1);
        strikethroughCheckBox.setSelected((allAttributes.get(styleName) & 2) == 2);
        /* TODO update other checkboxes */
    }

    /**
     * Reset the gui to set it to defaults
     */
    private void settingsDefault() {
        allStylesColor = ConfigSciNotesManager.getAllDefaultForegroundColors();
        allStylesIsBold = ConfigSciNotesManager.getDefaultAllisBold();
        allStylesIsItalic = ConfigSciNotesManager.getDefaultAllisItalic();
        allAttributes = ConfigSciNotesManager.getDefaultAllAttributes();

        Iterator<String> iter = allStylesColor.keySet().iterator();
        while (iter.hasNext()) {
            String name = iter.next();
            previewEditorPane.resetColor(name, allStylesColor.get(name));
            int bold = -1;
            if (allStylesIsBold.get(name)) {
                bold = 1;
            }
            previewEditorPane.resetFont(name, bold);
            int italic = -2;
            if (allStylesIsItalic.get(name)) {
                italic = 2;
            }
            previewEditorPane.resetFont(name, italic);
            previewEditorPane.resetAttribute(name, allAttributes.get(name));
        }

        int selectedStyleIndex = stylesList.getSelectedIndex();
        String styleName = listStylesName.get(selectedStyleIndex);

        colorButton.setBackground(allStylesColor.get(styleName));
        boldCheckBox.setSelected(allStylesIsBold.get(styleName));
        italicCheckBox.setSelected(allStylesIsItalic.get(styleName));
        underlineCheckBox.setSelected((allAttributes.get(styleName) & 1) == 1);
        strikethroughCheckBox.setSelected((allAttributes.get(styleName) & 2) == 2);
        bgColorButton.setBackground(ConfigSciNotesManager.getSciNotesDefaultBackgroundColor());
        fgColorButton.setBackground(ConfigSciNotesManager.getSciNotesDefaultForegroundColor());
        previewEditorPane.setBackground(bgColorButton.getBackground());
        previewEditorPane.setCaretColor(fgColorButton.getBackground());
        previewEditorPane.getCaret().setVisible(true);

        previewEditorPane.repaint();
    }
}
