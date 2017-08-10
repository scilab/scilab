/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

package org.scilab.modules.gui.utils;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.KeyboardFocusManager;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Collections;

import javax.swing.AbstractAction;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Document;
import javax.swing.text.JTextComponent;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabKeyStroke;

/**
 * Class to have an incremental search field.
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class HelpSearchField extends JPanel implements FocusListener, KeyListener {

    private static final String SCI = ScilabConstants.SCI.getPath();
    private static final String TAB = "tab";
    private static final String F3 = "F3";
    private static final String KEY = "help-search-field";
    private static final ImageIcon CLOSEICON = new ImageIcon(SCI + "/modules/gui/images/icons/close-tab.png");
    private static final ImageIcon TOPICON = new ImageIcon(FindIconHelper.findIcon("go-top"));
    private static final ImageIcon BOTICON = new ImageIcon(FindIconHelper.findIcon("go-bottom"));
    private static final int BUTTONSIZE = 28;

    private String text;
    private final JTextField field;
    private final JPanel parent;
    private JTextComponent textcomp;
    private final TopBotButtons[] buttons = new TopBotButtons[2];

    private final DefaultHighlighter.DefaultHighlightPainter highlighter = new DefaultHighlighter.DefaultHighlightPainter(Color.YELLOW);
    private int currentPos;
    private String currentWord;

    /**
     * Default Constructor
     */
    public HelpSearchField(JPanel parent, JTextComponent textcomp) {
        this(parent, textcomp, F3);
    }

    /**
     * Default Constructor
     */
    public HelpSearchField(JPanel parent, JTextComponent textcomp, String key) {
        super(new BorderLayout());
        this.parent = parent;
        setTextComponent(textcomp, key);
        field = new JTextField();
        field.setFocusTraversalKeys(KeyboardFocusManager.FORWARD_TRAVERSAL_KEYS, Collections.EMPTY_SET);
        field.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_TAB, 0), TAB);
        field.getActionMap().put(TAB, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                HelpSearchField.this.textcomp.requestFocus();
            }
        });
        JPanel panelButtons = new JPanel(new FlowLayout(FlowLayout.CENTER, 0, 0));
        panelButtons.add(new CloseButton());
        panelButtons.add(new TopBotButtons(true));
        panelButtons.add(new TopBotButtons(false));
        add(panelButtons, BorderLayout.WEST);
        add(field, BorderLayout.CENTER);
        setVisible(false);
    }

    /**
     * Change the textcomponent
     * @param textcomp the new textcomponent
     */
    public void setTextComponent(JTextComponent textcomp) {
        setTextComponent(textcomp, F3);
    }

    /**
     * Change the textcomponent
     * @param textcomp the new textcomponent
     */
    public void setTextComponent(JTextComponent textcomp, String key) {
        this.textcomp = textcomp;
        if (textcomp != null) {
            textcomp.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(ScilabKeyStroke.getKeyStroke(key), KEY);
            textcomp.getActionMap().put(KEY, new AbstractAction() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    HelpSearchField.this.showField();
                }
            });
        }

        text = null;
        currentPos = 0;
    }

    public void setKeyStroke(KeyStroke key) {
        if (textcomp != null) {
            textcomp.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(key, KEY);
        }
    }

    /**
     * Get the text in the text component to search in
     */
    private void getText() {
        if (text == null) {
            Document doc = textcomp.getDocument();
            try {
                text = doc.getText(0, doc.getLength()).toLowerCase();
            } catch (BadLocationException ex) {
                System.err.println(ex);
            }
        }
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
        getText();
        field.select(0, field.getText().length());
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void focusLost(FocusEvent e) {
        text = null;
        textcomp.getHighlighter().removeAllHighlights();
    }

    /**
     * Remove listeners
     */
    public void removeListeners() {
        field.removeFocusListener(this);
        field.removeKeyListener(this);
    }

    /**
     * Hide the search field
     */
    public void hideField() {
        parent.requestFocus();
        removeListeners();
        textcomp.getHighlighter().removeAllHighlights();
        setVisible(false);
        parent.remove(this);
    }

    /**
     * Show the search field
     */
    public void showField() {
        if (!isVisible()) {
            currentPos = 0;
            field.addKeyListener(this);
            field.addFocusListener(this);
            parent.add(this, BorderLayout.PAGE_END);
            setVisible(true);
        }
        requestFocus();
    }

    /**
     * Highlights the searched text
     * @param start the start position
     * @param end the end position
     */
    public void highlightText(int start, int end) {
        if (start != -1) {
            try {
                JScrollPane scrollPane = (JScrollPane) SwingUtilities.getAncestorOfClass(JScrollPane.class, textcomp);
                if (scrollPane != null) {
                    final Rectangle rect = textcomp.modelToView(start);
                    final JScrollBar sb = scrollPane.getVerticalScrollBar();
                    final int value = sb.getValue();
                    final int h = sb.getHeight();
                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            if (rect.y < value || rect.y > value + h) {
                                sb.setValue(Math.max(0, rect.y - h / 2));
                            }
                        }
                    });
                }
                textcomp.getHighlighter().removeAllHighlights();
                textcomp.getHighlighter().addHighlight(start, end, highlighter);
                currentPos = start;
            } catch (BadLocationException ex) {
                System.err.println(ex);
            }
            field.setForeground(Color.BLACK);
        } else {
            field.setForeground(Color.RED);
        }
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

        if (key != KeyEvent.VK_LEFT && key != KeyEvent.VK_RIGHT && key != KeyEvent.VK_UP && key != KeyEvent.VK_DOWN && key != KeyEvent.VK_SHIFT) {
            String str = field.getText();
            if (str != null) {
                int start;
                str = str.toLowerCase();
                if (!str.equals(currentWord)) {
                    currentWord = str;
                    currentPos = 0;
                    textcomp.getHighlighter().removeAllHighlights();
                }
                getText();
                if ((key == KeyEvent.VK_ENTER || key == KeyEvent.VK_F3) && ((e.getModifiers() & KeyEvent.SHIFT_MASK) != 0)) {
                    currentPos = Math.max(0, currentPos - 1);
                    start = text.lastIndexOf(str, currentPos);
                    if (start == -1) {
                        currentPos = text.length() - 1;
                    }
                    e.consume();
                } else {
                    if (key == KeyEvent.VK_ENTER || key == KeyEvent.VK_F3) {
                        currentPos++;
                    }
                    start = text.indexOf(str, currentPos);
                    if (start == -1) {
                        currentPos = 0;
                    }
                }

                int end = start + str.length();
                if (start != end) {
                    highlightText(start, end);
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
                @Override
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
                @Override
                public void actionPerformed(ActionEvent e) {
                    String str = field.getText();
                    String txt = "";
                    Document doc = textcomp.getDocument();
                    try {
                        txt = doc.getText(0, doc.getLength()).toLowerCase();
                    } catch (BadLocationException ex) {
                        System.err.println(ex);
                    }

                    if (str != null && !str.isEmpty()) {
                        int start;
                        str = str.toLowerCase();
                        if (top) {
                            currentPos = Math.max(0, currentPos - 1);;
                            start = txt.lastIndexOf(str, currentPos);
                            if (start == -1) {
                                currentPos = text.length() - 1;
                            }
                        } else {
                            currentPos++;
                            start = txt.indexOf(str, currentPos);
                            if (start == -1) {
                                currentPos = 0;
                            }
                        }

                        highlightText(start, start + str.length());
                    }
                }
            });
        }
    }
}
