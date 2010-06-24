/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes.actions;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Action to go to a given line
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
@SuppressWarnings("serial")
public final class GotoLineAction extends DefaultAction {

    private static boolean windowAlreadyExist;
    private static JFrame mainFrame;

    private JTextField enterLineNumberField;
    private int firstCaretPosition;
    private JButton okButton;

    /**
     * Constructor
     * @param editor SciNotes
     */
    private GotoLineAction(SciNotes editor) {
        super(SciNotesMessages.GOTO_LINE, editor);
    }

    /**
     * createMenu
     * @param editor SciNotes
     * @param key KeyStroke
     * @return createMenu
     */
    public static MenuItem createMenu(SciNotes editor, KeyStroke key) {
        return createMenu(SciNotesMessages.GOTO_LINE, null, new GotoLineAction(editor), key);
    }

    /**
     * Close this popup
     */
    public static void closeGotoLineWindow() {
        if (GotoLineAction.windowAlreadyExist) {
            mainFrame.dispose();
            GotoLineAction.windowAlreadyExist = false;
        }
    }

    /**
     * Action !!
     */
    public void doAction() {
        if (!GotoLineAction.windowAlreadyExist) {
            firstCaretPosition = getEditor().getTextPane().getCaretPosition();
            gotoLineBox();
            GotoLineAction.windowAlreadyExist = true;
        }
    }

    /**
     * A box to go to a given line
     */
    public void gotoLineBox() {
        final int dimX = 305;
        final int dimY = 105;

        mainFrame = new JFrame();
        mainFrame.setIconImage(new ImageIcon(System.getenv("SCI") + "/modules/gui/images/icons/scilab.png").getImage());
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setLayout(new GridBagLayout());

        mainFrame.setPreferredSize(new Dimension(dimX, dimY));
        mainFrame.setMinimumSize(new Dimension(dimX, dimY));
        mainFrame.setMaximumSize(new Dimension(dimX, dimY));

        JLabel label = new JLabel(SciNotesMessages.ENTER_LINE_NUMBER);

        enterLineNumberField = new JTextField();

        JButton cancelButton = new JButton(SciNotesMessages.CANCEL);
        okButton = new JButton(SciNotesMessages.OK);
        okButton.setPreferredSize(cancelButton.getPreferredSize());

        GridBagConstraints gbc = new GridBagConstraints();
        gbc.gridy = 0;
        gbc.gridx = 0;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.insets = new Insets(0, 10, 0, 0);

        gbc.gridx = 0;
        gbc.gridy = 4;
        gbc.gridheight = 1;
        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.insets = new Insets(0, 10, 0, 0);
        mainFrame.add(label, gbc);

        gbc.gridx = 1;
        gbc.gridwidth = GridBagConstraints.REMAINDER;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(5, 10, 0, 10);
        mainFrame.add(enterLineNumberField, gbc);

        if (isWindows()) {
            gbc.gridx = 1;
            gbc.gridy = 5;
            gbc.gridheight = 1;
            gbc.gridwidth = 1;
            gbc.weightx = 1.;
            gbc.fill = GridBagConstraints.NONE;
            gbc.insets = new Insets(5, 0, 10, 10);
            mainFrame.add(okButton, gbc);

            gbc.gridx = 2;
            gbc.weightx = 0.;
            gbc.insets = new Insets(5, 0, 10, 5);
            mainFrame.add(cancelButton, gbc);
        } else {
            gbc.gridx = 1;
            gbc.gridy = 5;
            gbc.gridheight = 1;
            gbc.gridwidth = 1;
            gbc.weightx = 1.;
            gbc.fill = GridBagConstraints.NONE;
            gbc.insets = new Insets(5, 0, 10, 5);
            mainFrame.add(cancelButton, gbc);

            gbc.gridx = 2;
            gbc.weightx = 0.;
            gbc.insets = new Insets(5, 0, 10, 10);
            mainFrame.add(okButton, gbc);
        }

        cancelButton.addActionListener(new ActionListener() {

                public void actionPerformed(ActionEvent e) {
                    getEditor().getTextPane().setCaretPosition(firstCaretPosition);
                    GotoLineAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }
            });

        okButton.addActionListener(new ActionListener() {

                public void actionPerformed(ActionEvent e) {
                    updateCaretPosition();
                    getEditor().getTextPane().setFocusable(true);
                    GotoLineAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }
            });

        enterLineNumberField.addKeyListener(new KeyListener() {
                public void keyTyped(KeyEvent arg0) { }
                public void keyPressed(KeyEvent arg0) { }

                public void keyReleased(KeyEvent arg0) {
                    updateCaretPosition();
                }
            });

        //display the frame and set some properties

        mainFrame.addWindowListener(new WindowListener() {
                public void windowClosed(WindowEvent arg0) { }
                public void windowDeiconified(WindowEvent arg0) { }
                public void windowActivated(WindowEvent arg0) { }

                public void windowClosing(WindowEvent arg0) {
                    GotoLineAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }

                public void windowDeactivated(WindowEvent arg0) { }
                public void windowIconified(WindowEvent arg0) { }
                public void windowOpened(WindowEvent arg0) { }
            });

        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(SciNotesMessages.GOTO_LINE);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(null);
        mainFrame.setVisible(true);
    }

    /**
     * check if it is Windows
     * @return true or false
     */
    private boolean isWindows() {
        return System.getProperty("os.name").toLowerCase().contains("windows");
    }

    /**
     * Update the caret position when a number is entered in the field
     */
    private void updateCaretPosition() {
        if (enterLineNumberField.getText().length() != 0) {

            // if the input is not a integer..
            Pattern p = Pattern.compile("\\D");
            Matcher m = p.matcher(enterLineNumberField.getText());

            if (m.find()) {
                //... we disable okbutton
                okButton.setEnabled(false);
            } else {
                okButton.setEnabled(true);
                ScilabDocument doc = (ScilabDocument) getEditor().getTextPane().getDocument();

                int lineNumber = Integer.decode(enterLineNumberField.getText()) - 1;
                int maxLineNumber = doc.getDefaultRootElement().getElementCount();

                // avoid too big or too small number
                if (lineNumber <= 0) {
                    lineNumber = 0;
                }
                if (lineNumber >= maxLineNumber) {
                    lineNumber = maxLineNumber - 1;
                }
                // we get the offset of the line we want
                int start = doc.getDefaultRootElement().getElement(lineNumber).getStartOffset();
                getEditor().getTextPane().scrollTextToPos(start);

            }
        } else {
            okButton.setEnabled(true);
        }
    }
}
