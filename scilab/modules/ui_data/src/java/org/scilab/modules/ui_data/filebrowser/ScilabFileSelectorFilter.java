/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 * Copyright (C) 2024 - Dassault Systèmes S.E. - Vincent COUVERT
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

package org.scilab.modules.ui_data.filebrowser;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.ContainerOrderFocusTraversalPolicy;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.util.regex.Pattern;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JTextField;
import javax.swing.JToggleButton;
import javax.swing.KeyStroke;
import javax.swing.border.EmptyBorder;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.DefaultEditorKit;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.ui_data.utils.UiDataMessages;

/**
 * A panel with file filter and the combo to set the cwd.
 * @author Calixte DENIZET
 */
public final class ScilabFileSelectorFilter extends JPanel {
    private static final long serialVersionUID = 1L;

    private static final int GAP = 3;

    private MyJTextField textfield;
    private final SwingScilabTreeTable stt;
    private JToggleButton caseSensitive;
    private JToggleButton regexp;

    /**
     * Default constructor
     * @param stt the SwingScilabTree containing the cwd
     * @param comboPath the combobox where to set the path
    */
    public ScilabFileSelectorFilter(SwingScilabTreeTable stt) {
        super();
        this.stt = stt;
        initPanel();
    }

    /**
     * Init the panel
     */
    private void initPanel() {
        setBorder(new EmptyBorder(0, 0, GAP, 0));
        setLayout(new BorderLayout());
        textfield = new MyJTextField();
        setFocusCycleRoot(true);
        setFocusTraversalPolicy(new ContainerOrderFocusTraversalPolicy());

        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 1;
        gbc.gridheight = 1;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        add(textfield, BorderLayout.CENTER);
    }

    /**
     * Test the pattern and change the color of the text in textfield if the pattern is invalid
     */
    private void testPattern() {
        if (regexp.isSelected()) {
            if (getPattern() == null) {
                textfield.setForeground(Color.RED);
            } else {
                textfield.setForeground(textfield.defaultFg);
            }
        }
    }

    /**
     * Get the pattern according to the textfield content
     * @return the pattern
     */
    private Pattern getPattern() {
        String text = textfield.getText();
        if (text != null && !text.isEmpty()) {
            if (!regexp.isSelected()) {
                text = text.replaceAll("\\*", "\\\\E.*\\\\Q").replaceAll("\\?", "\\\\E.?\\\\Q");
                text = "\\Q" + text + "\\E";
            }
            if (!caseSensitive.isSelected()) {
                text = "(?i)" + text;
            }

            if (regexp.isSelected()) {
                text = "(?m)" + text;
            }

            try {
                return Pattern.compile(text);
            } catch (Exception e) { }
        }

        return null;
    }

    private class MyJTextField extends JPanel {
        private static final long serialVersionUID = 1L;

        JTextFieldWithPlaceHolder textField;
        Color defaultFg;

        MyJTextField() {
            // https://stackoverflow.com/questions/22777780/how-to-add-button-in-text-field
            super();
            setLayout(new GridBagLayout());

            textField = new JTextFieldWithPlaceHolder();
            defaultFg = textField.defaultFg;

            this.setBackground(textField.getBackground());
            this.setBorder(textField.getBorder());
            textField.setBorder(null);

            GridBagConstraints gbc = new GridBagConstraints();

            gbc.insets = new Insets(2, 2, 2, 2);

            gbc.gridx = 0;
            gbc.gridy = 0;
            gbc.weightx = 1.0;
            gbc.fill = GridBagConstraints.HORIZONTAL;
            this.add(textField, gbc);

            // TODO uncomment if using FlatLaf
            // UIManager.put("ToggleButton.hoverBackground", ((Color)UIManager.get("Button.default.hoverBackground")));

            gbc.weightx = 0.0;
            gbc.fill = GridBagConstraints.NONE;

            caseSensitive = new JToggleButton(new ImageIcon(FindIconHelper.findIcon("case-sensitive")));
            caseSensitive.setToolTipText(UiDataMessages.CASESENSITIVE);
            caseSensitive.setMargin(new Insets(0, 0, 0, 0));
            gbc.gridx = 1;
            gbc.gridy = 0;
            this.add(caseSensitive, gbc);

            regexp = new JToggleButton(new ImageIcon(FindIconHelper.findIcon("regex")));
            regexp.setToolTipText(UiDataMessages.REGEXP);
            regexp.setMargin(new Insets(0, 0, 0, 0));
            gbc.gridx = 2;
            gbc.gridy = 0;
            this.add(regexp, gbc);

            JButton clear = new JButton(new CommonCallBack(null) {
                @Override
                public void callBack() {
                    textField.setText("");
                    stt.setFilter(getPattern());
                    textField.toggleContents();
                }
            });
            clear.setIcon(new ImageIcon(FindIconHelper.findIcon("clear")));
            clear.setToolTipText(UiDataMessages.CLEAR);
            clear.setMargin(new Insets(0, 0, 0, 0));
            gbc.gridx = 3;
            gbc.gridy = 0;
            this.add(clear, gbc);

            JButton filter = new JButton(new CommonCallBack(null) {
                @Override
                public void callBack() {
                    stt.setFilter(getPattern());
                }
            });
            filter.setIcon(new ImageIcon(FindIconHelper.findIcon("filter")));
            filter.setToolTipText(UiDataMessages.RUNFILTER);
            filter.setMargin(new Insets(0, 0, 0, 0));
            gbc.gridx = 4;
            gbc.gridy = 0;
            this.add(filter, gbc);
        }

        String getText() {
            return textField.getText();
        }
    }

    /**
     * Inner class to have the possibility to add a default text in the textfield but in this case the textfield is empty !
     * It is a workaround for a GTK bug which avoids me to paint directly in the field.
     */
    private class JTextFieldWithPlaceHolder extends JTextField implements DocumentListener, FocusListener {
        private static final long serialVersionUID = 1L;

        boolean isEmpty = true;
        Color defaultFg;

        /**
         * {@inheritDoc}
         */
        JTextFieldWithPlaceHolder() {
            defaultFg = getForeground();
            setEditable(true);
            setComponentPopupMenu(createPopup());
            addFocusListener(this);
            getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ENTER, 0), "ENTER");
            getActionMap().put("ENTER", new CommonCallBack(null) {
                @Override
                public void callBack() {
                    stt.setFilter(getPattern());
                }
            });
            toggleContents();
        }

        /**
         * Toggle the contents
         */
        private void toggleContents() {
            if (isEmpty) {
                setForeground(Color.LIGHT_GRAY);
                setText(UiDataMessages.FILEFILTER);
            }
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public String getText() {
            if (isEmpty) {
                return "";
            }

            return super.getText();
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void focusGained(FocusEvent e) {
            getDocument().addDocumentListener(this);
            if (isEmpty) {
                setForeground(defaultFg);
                setText("");
            }
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void focusLost(FocusEvent e) {
            getDocument().removeDocumentListener(this);
            isEmpty = super.getText().isEmpty();
            toggleContents();
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void insertUpdate(DocumentEvent e) {
            isEmpty = super.getText().isEmpty();
            testPattern();
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void removeUpdate(DocumentEvent e) {
            isEmpty = super.getText().isEmpty();
            testPattern();
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void changedUpdate(DocumentEvent e) { }

        /**
         * @return the popup menu used in this textfield
         */
        private JPopupMenu createPopup() {
            JPopupMenu popup = new JPopupMenu();

            JMenuItem item = new JMenuItem(UiDataMessages.CUT);
            item.addActionListener(new DefaultEditorKit.CutAction());
            popup.add(item);

            item = new JMenuItem(UiDataMessages.COPY);
            item.addActionListener(new DefaultEditorKit.CopyAction());
            popup.add(item);

            item = new JMenuItem(UiDataMessages.PASTE);
            item.addActionListener(new DefaultEditorKit.PasteAction());
            popup.add(item);

            item = new JMenuItem(UiDataMessages.CLEAR);
            item.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    JTextFieldWithPlaceHolder.this.setText("");
                }
            });
            popup.add(item);

            return popup;
        }
    }
}
