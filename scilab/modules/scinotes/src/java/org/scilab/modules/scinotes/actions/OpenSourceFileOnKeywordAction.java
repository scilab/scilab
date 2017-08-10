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

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.Iterator;
import java.util.NavigableSet;
import java.util.Set;
import java.util.TreeSet;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.KeyStroke;
import javax.swing.text.BadLocationException;
import javax.swing.text.JTextComponent;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.KeywordEvent;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexer;
import org.scilab.modules.scinotes.ScilabLexerConstants;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * OpenSourceFileOnKeywordAction Class
 * @author Calixte DENIZET
 */
public class OpenSourceFileOnKeywordAction extends DefaultAction {

    private static final long serialVersionUID = 1L;

    private static final int GAP = 5;
    private static boolean windowAlreadyExist;
    private static JFrame mainFrame;

    private NavigableSet<String> macrosSet;
    private JButton okButton;
    private JButton cancelButton;
    private JComboBox comboComplete;
    private boolean onMenu;
    private int lastSize;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public OpenSourceFileOnKeywordAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     * @param onMenu if true the dialog will be open when a menu is actionned
     */
    public OpenSourceFileOnKeywordAction(String name, SciNotes editor, boolean onMenu) {
        this(name, editor);
        this.onMenu = onMenu;
    }

    /**
     * Clean
     */
    public static void close() {
        mainFrame = null;
    }

    /**
     * doAction
     */
    @Override
    public void doAction() {
        ScilabEditorPane sep = getEditor().getTextPane();
        KeywordEvent kwe = sep.getKeywordEvent(sep.getSelectionEnd());
        ScilabDocument doc = (ScilabDocument) sep.getDocument();
        if (ScilabLexerConstants.isOpenable(kwe.getType())) {
            String name = "";
            try {
                name = doc.getText(kwe.getStart(), kwe.getLength());
            } catch (BadLocationException e) { }
            if (onMenu) {
                openSourceBox(name);
            } else {
                openSource(name);
            }
        } else {
            openSourceBox("");
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new OpenSourceFileOnKeywordAction(label, editor, true), key);
    }

    /**
     * Show the source of a macro if it is present in the pane or open the source
     * @param pane the ScilabEditorPane
     * @param name the macro's name
     */
    public static void openSource(ScilabEditorPane pane, String name) {
        ScilabDocument doc = (ScilabDocument) pane.getDocument();
        int pos = doc.searchFunctionByName(name);
        if (pos != -1) {
            pane.scrollTextToPos(pos);
        } else {
            String path = "get_function_path('" + name + "')";
            InterpreterManagement.requestScilabExec("if " + path + " ~=[] then editor(" + path + ");end");
        }
    }

    /**
     * Close the window
     */
    public static void closeOpenSourceWindow() {
        if (windowAlreadyExist) {
            mainFrame.dispose();
            windowAlreadyExist = false;
        }
    }

    /**
     * Open a window to get the name of the macro
     * @param name the name of a macro
     */
    public void openSourceBox(String name) {
        if (windowAlreadyExist) {
            mainFrame.setVisible(true);
            if (name.length() != 0) {
                comboComplete.removeAllItems();
                comboComplete.getEditor().setItem(name);
                comboComplete.hidePopup();
            }
            return;
        }

        macrosSet = new TreeSet(ScilabLexer.macros);

        mainFrame = new JFrame();
        mainFrame.setAlwaysOnTop(true);
        mainFrame.setIconImage(new ImageIcon(FindIconHelper.findIcon("scilab")).getImage());
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        windowAlreadyExist = true;

        JLabel label = new JLabel(SciNotesMessages.OPEN_SOURCE_FILE_OF);

        comboComplete = new JComboBox();
        comboComplete.setEditable(true);
        if (name.length() != 0) {
            comboComplete.removeAllItems();
            comboComplete.getEditor().setItem(name);
        } else {
            updateComboComplete(" ");
        }

        comboComplete.hidePopup();

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

        JPanel panelCombo = new JPanel();
        panelCombo.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelCombo.add(label);
        panelCombo.add(comboComplete);

        JPanel panelFrame = new JPanel();
        panelFrame.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelFrame.setLayout(new BoxLayout(panelFrame, BoxLayout.PAGE_AXIS));
        panelFrame.add(panelCombo);
        panelFrame.add(panelButton);

        mainFrame.setContentPane(panelFrame);

        cancelButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                mainFrame.dispose();
            }
        });

        okButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                mainFrame.dispose();
                openSource((String) comboComplete.getEditor().getItem());
            }
        });

        comboComplete.getEditor().getEditorComponent().addKeyListener(new KeyListener() {

            private String old = "";

            @Override
            public void keyReleased(KeyEvent e) {
                int code = e.getKeyCode();
                if (code == KeyEvent.VK_ENTER) {
                    if (comboComplete.getSelectedIndex() == -1 && comboComplete.getItemCount() > 0) {
                        comboComplete.setSelectedIndex(0);
                    }
                    comboComplete.hidePopup();
                    okButton.requestFocus();
                } else if (code == KeyEvent.VK_ESCAPE) {
                    comboComplete.hidePopup();
                } else if (code != KeyEvent.VK_UP && code != KeyEvent.VK_DOWN) {
                    updateComboComplete(old);
                    comboComplete.setPopupVisible(true);
                }
            }

            @Override
            public void keyPressed(KeyEvent arg0) {
                old = (String) comboComplete.getEditor().getItem();
            }

            @Override
            public void keyTyped(KeyEvent arg0) { }
        });

        mainFrame.addWindowListener(new WindowListener() {
            @Override
            public void windowClosed(WindowEvent arg0) { }
            @Override
            public void windowDeiconified(WindowEvent arg0) { }
            @Override
            public void windowActivated(WindowEvent arg0) { }

            @Override
            public void windowClosing(WindowEvent arg0) {
                OpenSourceFileOnKeywordAction.windowAlreadyExist = false;
                mainFrame.dispose();
            }

            @Override
            public void windowDeactivated(WindowEvent arg0) { }
            @Override
            public void windowIconified(WindowEvent arg0) { };
            @Override
            public void windowOpened(WindowEvent arg0) { }
        });

        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(SciNotesMessages.OPEN_SOURCE);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(getEditor());
        mainFrame.setVisible(true);
    }

    /**
     * Open the source of a macro
     * @param name of the macro
     */
    private void openSource(String name) {
        openSource(getEditor().getTextPane(), name);
    }

    /**
     * Update the comboBox with the possible completion of the name
     * entered in the comboBox.
     * @param old the previous name
     */
    private void updateComboComplete(String old) {
        String name = (String) comboComplete.getEditor().getItem();
        Set subset = null;
        if (name != null && !name.equals(old)) {
            String bound = "";
            Iterator<String> iter;
            int len = name.length();
            if (len != 0) {
                if (len == 1) {
                    bound = new String(new char[] {(char) (name.charAt(0) + 1)});
                } else {
                    bound = name.substring(0, len - 1);
                    bound += (char) (name.charAt(len - 1) + 1);
                }
                /* If "abc" is entered, we built "abd" (last letter + 1) and we
                   retrieve all the names between "abc" and "abd" (excluded) */
                subset = macrosSet.subSet(name, true, bound, false);
                iter = subset.iterator();
            } else {
                iter = macrosSet.iterator();
            }

            if (subset != null && subset.size() != lastSize) {
                comboComplete.removeAllItems();
                while (iter.hasNext()) {
                    comboComplete.addItem(iter.next());
                }
                lastSize = subset.size();
                comboComplete.setPopupVisible(false);
            }

            comboComplete.getEditor().setItem(name);
            JTextComponent c = ((JTextComponent) comboComplete.getEditor().getEditorComponent());
            c.setSelectionStart(c.getSelectionEnd());
        }
    }
}
