/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - Calixte DENIZET
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

package org.scilab.modules.scinotes.actions;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * IncrementalSearchAction Class
 * @author Calixte DENIZET
 */
public final class IncrementalSearchAction extends DefaultAction {

    private static final long serialVersionUID = -3093932085002970876L;
    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String TAB = "tab";
    private static final ImageIcon CLOSEICON = new ImageIcon(SCI + "/modules/gui/images/icons/close-tab.png");
    private static final ImageIcon TOPICON = new ImageIcon(FindIconHelper.findIcon("go-top"));
    private static final ImageIcon BOTICON = new ImageIcon(FindIconHelper.findIcon("go-bottom"));
    private static final int BUTTONSIZE = 28;

    private static Map<SciNotes, SearchField> fields = new HashMap<SciNotes, SearchField>();

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
    @Override
    public void doAction() {
        if (!fields.containsKey(getEditor())) {
            SearchField field = new SearchField();
            fields.put(getEditor(), field);
            getEditor().insertBottomComponent(field);
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
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
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
     * @param editor the editor where to close the search bar
     * @return the word which is currently searching
     */
    public static String getWord(SciNotes editor) {
        if (fields.containsKey(editor)) {
            return fields.get(editor).getWord();
        }

        return null;
    }

    /**
     * @param editor the editor where to close the search bar
     * @return true if it is an exact search
     */
    public static boolean getExact(SciNotes editor) {
        if (fields.containsKey(editor)) {
            return fields.get(editor).getExact();
        }

        return false;
    }

    /**
     * Inner class to have an incremental search field.
     */
    @SuppressWarnings(value = { "serial" })
    class SearchField extends JPanel implements FocusListener, KeyListener {

        private String text;
        private final JTextField field;
        private boolean exact;

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
                @Override
                public void actionPerformed(ActionEvent e) {
                    getEditor().getTextPane().requestFocus();
                }
            });
            JPanel panelButtons = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
            panelButtons.add(new CloseButton());
            panelButtons.add(new TopBotButtons(true));
            panelButtons.add(new TopBotButtons(false));
            JCheckBox check = new JCheckBox(SciNotesMessages.EXACT);
            check.addItemListener(new ItemListener() {
                @Override
                public void itemStateChanged(ItemEvent e) {
                    exact = e.getStateChange() == ItemEvent.SELECTED;
                    changeText();
                }
            });
            panelButtons.add(check);
            add(panelButtons, BorderLayout.WEST);
            add(field, BorderLayout.CENTER);
            setVisible(true);
        }

        /**
         * @return the word which is currently searching
         */
        public String getWord() {
            return field.getText();
        }

        /**
         * @return true if it is an exact search
         */
        public boolean getExact() {
            return exact;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void requestFocus() {
            field.requestFocus();
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public void focusGained(FocusEvent e) {
            if (text == null) {
                changeText();
            }
            field.select(0, field.getText().length());
        }

        /**
         * {@inheritDoc}
         */
        @Override
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
            getEditor().insertBottomComponent(null);
        }

        /**
         * {@inheritDoc}
         */
        @Override
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
                    sep.highlightWords(str, exact);
                    if (!exact) {
                        str = str.toLowerCase();
                    }
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
        @Override
        public void keyTyped(KeyEvent e) { }

        /**
         * {@inheritDoc}
         */
        @Override
        public void keyPressed(KeyEvent e) { }

        /**
         * Change the text where to search
         */
        private void changeText() {
            if (getEditor().getTextPane() != null) {
                ScilabEditorPane sep = getEditor().getTextPane();
                ScilabDocument doc = (ScilabDocument) sep.getDocument();
                text = doc.getText();
                if (!exact) {
                    text = text.toLowerCase();
                }
                sep.highlightWords(field.getText(), exact);
            }
        }

        /**
         * Inner class for the close-buttons
         */
        @SuppressWarnings(value = { "serial" })
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
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        hideField();
                    }
                });
            }
        }

        /**
         * Inner class for the top-bot-buttons
        @SuppressWarnings(value = { "serial" })
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
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        ScilabEditorPane sep = getEditor().getTextPane();
                        int pos = sep.getSelectionStart();
                        int start;
                        String str = field.getText();

                        ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();
                        String txt = doc.getText();
                        if (!exact) {
                            txt = txt.toLowerCase();
                        }

                        if (str != null) {
                            if (!exact) {
                                str = str.toLowerCase();
                            }
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
