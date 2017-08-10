/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
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

import java.awt.Dialog.ModalityType;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.text.Element;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * SetAnchorAction Class
 * @author Calixte DENIZET
 */
public class SetAnchorAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    private static final int GAP = 5;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SetAnchorAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    @Override
    public void doAction() {
        openAnchorWindow();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SetAnchorAction(label, editor), key);
    }

    /**
     * Open a dialog to get the anchor's name
     */
    public void openAnchorWindow() {
        final JDialog dialog;
        final JButton okButton;
        final JButton cancelButton;
        final JTextField textField;

        dialog = new JDialog();
        dialog.setModalityType(ModalityType.APPLICATION_MODAL);
        dialog.setIconImage(new ImageIcon(FindIconHelper.findIcon("scilab")).getImage());
        JLabel label = new JLabel(SciNotesMessages.ANCHOR_NAME);

        cancelButton = new JButton(SciNotesMessages.CANCEL);
        okButton = new JButton(SciNotesMessages.OK);
        okButton.setPreferredSize(cancelButton.getPreferredSize());

        JPanel panelButton = new JPanel();
        panelButton.setLayout(new GridLayout(1, 4, GAP, GAP));
        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, 0, 0, 0));
        panelButton.add(new JLabel());
        panelButton.add(new JLabel());
        panelButton.add(cancelButton);
        panelButton.add(okButton);

        textField = new JTextField();
        JPanel panelText = new JPanel();
        panelText.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelText.add(label);
        panelText.add(textField);

        JPanel panelDialog = new JPanel();
        panelDialog.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelDialog.setLayout(new BoxLayout(panelDialog, BoxLayout.PAGE_AXIS));
        panelDialog.add(panelText);
        panelDialog.add(panelButton);

        dialog.setContentPane(panelDialog);

        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                dialog.dispose();
            }
        });

        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                setAnchorName(textField.getText());
                dialog.dispose();
            }
        });

        textField.addKeyListener(new KeyListener() {
            @Override
            public void keyReleased(KeyEvent e) {
                int code = e.getKeyCode();
                if (code == KeyEvent.VK_ENTER) {
                    okButton.doClick();
                } else if (code == KeyEvent.VK_ESCAPE) {
                    cancelButton.doClick();
                }
            }

            @Override
            public void keyPressed(KeyEvent arg0) { }

            @Override
            public void keyTyped(KeyEvent arg0) { }
        });

        dialog.setDefaultCloseOperation(JDialog.DO_NOTHING_ON_CLOSE);
        dialog.setTitle(SciNotesMessages.SET_ANCHOR);
        dialog.pack();
        dialog.setLocationRelativeTo(getEditor());
        dialog.setVisible(true);
    }

    /**
     * Set the anchor's name
     * @param name the anchor's name
     */
    private void setAnchorName(String name) {
        if (name != null && name.length() != 0) {
            ScilabEditorPane sep = getEditor().getTextPane();
            sep.getXln().repaint();
            ScilabDocument doc = (ScilabDocument) sep.getDocument();
            Element root = doc.getDefaultRootElement();
            ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) root.getElement(root.getElementIndex(sep.getCaretPosition()));
            line.setAnchor(name);
            getEditor().getNavigator().update();
        }
    }
}
