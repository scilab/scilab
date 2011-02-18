/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - Calixte DENIZET
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
import java.awt.Component;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.pushbutton.PushButton;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;

/**
 * IncrementalSearchAction Class
 * @author Calixte DENIZET
 */
public final class IncrementalSearchAction extends DefaultAction {

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String TAB = "tab";
    private static final ImageIcon CLOSEICON = new ImageIcon(SCI + "/modules/gui/images/icons/close-tab.png");
    private static final ImageIcon TOPICON = new ImageIcon(SCI + "/modules/gui/images/icons/16x16/actions/go-top.png");
    private static final ImageIcon BOTICON = new ImageIcon(SCI + "/modules/gui/images/icons/16x16/actions/go-bottom.png");
    private static final int BUTTONSIZE = 28;

    private static Map<SciNotes, SearchField> fields = new HashMap();

    private TextBox info;
    private JPanel panelInfoSearch;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public IncrementalSearchAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        if (!fields.containsKey(getEditor())) {
            SearchField field = new SearchField();
            fields.put(getEditor(), field);
            getEditor().getTextPane().getEditorComponent().insertBottomComponent(field);
            field.requestFocus();
        } else {
            fields.get(getEditor()).requestFocus();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new IncrementalSearchAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static PushButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new IncrementalSearchAction(tooltip, editor));
    }

    /**
     * @param editor the editor where to close the search bar
     */
    public static void close(SciNotes editor) {
        if (fields.containsKey(editor)) {
            fields.get(editor).removeListeners();
            fields.remove(editor);
        }
    }

    /**
     * Inner class to have an incremental search field.
     */
    class SearchField extends JPanel implements FocusListener, KeyListener {

        private String text;
        private JTextField field;
        private TopBotButtons[] buttons = new TopBotButtons[2];

        /**
         * Default Constructor
         */
        public SearchField() {
            super(new BorderLayout());
            field = new JTextField();
            field.addKeyListener(this);
            field.addFocusListener(this);
            field.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, Collections.EMPTY_SET);
            field.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), TAB);
            field.getActionMap().put(TAB, new AbstractAction() {
                    public void actionPerformed(ActionEvent e) {
                        getEditor().getTextPane().requestFocus();
                    }
                });
            JPanel panelButtons = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
            panelButtons.add(new CloseButton());
            panelButtons.add(new TopBotButtons(true));
            panelButtons.add(new TopBotButtons(false));
            add(panelButtons, BorderLayout.WEST);
            add(field, BorderLayout.CENTER);
            setVisible(true);
        }

        /**
         * {@inheritDoc}
         */
        public void requestFocus() {
            field.requestFocus();
        }

        /**
         * {@inheritDoc}
         */
        public void focusGained(FocusEvent e) {
            if (text == null && getEditor().getTextPane() != null) {
                ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
                text = doc.getText().toLowerCase();
            }
            field.select(0, field.getText().length());
        }

        /**
         * {@inheritDoc}
         */
        public void focusLost(FocusEvent e) {
            text = null;
        }

        /**
         * Remove listeners
         */
        public void removeListeners() {
            field.removeFocusListener(field.getFocusListeners()[0]);
            field.removeKeyListener(field.getKeyListeners()[0]);
        }

        /**
         * Hide the search field
         */
        public void hideField() {
            removeListeners();
            fields.remove(getEditor());
            getEditor().getTextPane().getEditorComponent().insertBottomComponent(null);
        }

        /**
         * {@inheritDoc}
         */
        public void keyReleased(KeyEvent e) {
            int key = e.getKeyCode();
            if (key == KeyEvent.VK_ESCAPE) {
                hideField();
                return;
            }

            if (key != KeyEvent.VK_LEFT && key != KeyEvent.VK_RIGHT && key != KeyEvent.VK_UP && key != KeyEvent.VK_DOWN) {
                ScilabEditorPane sep = getEditor().getTextPane();
                int pos = sep.getSelectionStart();
                int start;
                String str = field.getText();
                if (str != null) {
                    str = str.toLowerCase();
                    if (key == KeyEvent.VK_ENTER && ((e.getModifiers() & KeyEvent.SHIFT_MASK) != 0)) {
                        start = text.lastIndexOf(str, pos - 1);
                        e.consume();
                    } else {
                        if (key == KeyEvent.VK_ENTER) {
                            pos++;
                        }
                        start = text.indexOf(str, pos);
                    }
                    if (start != -1) {
                        sep.select(start, start + str.length());
                        field.setForeground(Color.BLACK);
                    } else {
                        field.setForeground(Color.RED);
                    }
                }
            }
        }

        /**
         * {@inheritDoc}
         */
        public void keyTyped(KeyEvent e) { }

        /**
         * {@inheritDoc}
         */
        public void keyPressed(KeyEvent e) { }

        /**
         * Inner class for the close-buttons
         */
        class CloseButton extends JButton {

            /**
             * Constructor
             */
            CloseButton() {
                super(CLOSEICON);
                setFocusable(false);
                setContentAreaFilled(true);
                setOpaque(false);
                setRolloverEnabled(true);
                setBorderPainted(false);
                setPreferredSize(new Dimension(BUTTONSIZE, BUTTONSIZE));
                addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                            hideField();
                        }
                    });
            }
        }

        /**
         * Inner class for the top-bot-buttons
         */
        class TopBotButtons extends JButton {

            /**
             * Constructor
             * @param top true for the top-icon
             */
            TopBotButtons(final boolean top) {
                super(top ? TOPICON : BOTICON);
                setFocusable(false);
                setContentAreaFilled(true);
                setOpaque(false);
                setRolloverEnabled(true);
                setBorderPainted(false);
                setPreferredSize(new Dimension(BUTTONSIZE, BUTTONSIZE));
                addActionListener(new ActionListener() {
                        public void actionPerformed(ActionEvent e) {
                            ScilabEditorPane sep = getEditor().getTextPane();
                            int pos = sep.getSelectionStart();
                            int start;
                            String str = field.getText();

                            ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
                            String txt = doc.getText().toLowerCase();

                            if (str != null) {
                                str = str.toLowerCase();
                                if (top) {
                                    start = txt.lastIndexOf(str, pos - 1);
                                } else {
                                    start = txt.indexOf(str, pos + 1);
                                }
                                if (start != -1) {
                                    sep.select(start, start + str.length());
                                    field.setForeground(Color.BLACK);
                                } else {
                                    field.setForeground(Color.RED);
                                }
                            }
                        }
                    });
            }
        }
    }
}
