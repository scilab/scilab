/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

import java.awt.Component;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollBar;
import javax.swing.JSpinner;
import javax.swing.KeyStroke;
import javax.swing.SpinnerNumberModel;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.IndentManager;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabView;
import org.scilab.modules.scinotes.TabManager;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * ConfigTabulationsAction Class
 * @author Calixte DENIZET
 */
public class ConfigTabulationsAction extends DefaultAction {

    private static final long serialVersionUID = 1L;
    private static final int GAP = 5;
    private static final String ESCAPE = "escape";
    private static final String SCI = ScilabConstants.SCI.getPath();

    private static boolean windowAlreadyExist;
    private static JFrame mainFrame;

    private JButton okButton;
    private JButton cancelButton;
    private JButton applyButton;
    private JRadioButton tabHRule;
    private JRadioButton tabVRule;
    private JRadioButton tabDbleChev;
    private JCheckBox useWhitesCheckBox;
    private JSpinner tabLengthSpinner;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public ConfigTabulationsAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    public void doAction() {
        ScilabEditorPane sep = (ScilabEditorPane) getEditor().getTextPane();
        openConfTabWindow();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new ConfigTabulationsAction(label, editor), key);
    }

    /**
     * Close the window
     */
    public static void closeConfTabWindow() {
        if (windowAlreadyExist) {
            mainFrame.dispose();
            mainFrame = null;
            windowAlreadyExist = false;
        }
    }

    /**
     * Open a window
     */
    public void openConfTabWindow() {
        if (windowAlreadyExist) {
            mainFrame.setVisible(true);
            return;
        }

        TabManager.Tabulation tabProps = ConfigSciNotesManager.getDefaultTabulation();

        mainFrame = new JFrame();
        mainFrame.setIconImage(new ImageIcon(SCI + "/modules/gui/images/icons/scilab.png").getImage());
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE , 0), ESCAPE);
        mainFrame.getRootPane().getActionMap().put(ESCAPE, new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    closeConfTabWindow();
                }
            });

        windowAlreadyExist = true;

        cancelButton = new JButton(SciNotesMessages.CANCEL);
        okButton = new JButton(SciNotesMessages.OK);
        okButton.setPreferredSize(cancelButton.getPreferredSize());
        applyButton = new JButton(SciNotesMessages.APPLYREINDENT);

        JPanel panelButton = new JPanel();
        panelButton.setLayout(new GridLayout(1, 4, GAP, GAP));
        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, 0, 0, 0));
        panelButton.add(new JLabel());
        panelButton.add(okButton);
        panelButton.add(applyButton);
        panelButton.add(cancelButton);

        /* Main content pane */
        JPanel contentPanel = new JPanel(new GridLayout(1, 2, GAP, GAP));
        contentPanel.setBorder(BorderFactory.createEmptyBorder(2 * GAP, 2 * GAP, 2 * GAP, 2 * GAP));

        /* Style */
        JPanel stylePanel = new JPanel(new GridLayout(2, 1, GAP, GAP));
        stylePanel.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.TABSTYLE));

        useWhitesCheckBox = new JCheckBox(SciNotesMessages.TABUSEWHITES, tabProps.tab == ' ');
        stylePanel.add(useWhitesCheckBox);

        JPanel tabLengthPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        tabLengthSpinner = new JSpinner(new SpinnerNumberModel(tabProps.number, 0, 16, 1));

        tabLengthPanel.add(new JLabel(SciNotesMessages.TABLENGTH));
        tabLengthPanel.add(tabLengthSpinner);

        stylePanel.add(tabLengthPanel);

        contentPanel.add(stylePanel);

        /* Representation */
        final JPanel panelRepresentation = new JPanel(new GridLayout(3, 1, GAP, GAP));
        panelRepresentation.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.TABREPRESENTATION));
        tabHRule = new JRadioButton(SciNotesMessages.TABHRULE, tabProps.type == ScilabView.TABHORIZONTAL);
        panelRepresentation.add(tabHRule);
        tabVRule = new JRadioButton(SciNotesMessages.TABVRULE, tabProps.type == ScilabView.TABVERTICAL);
        panelRepresentation.add(tabVRule);
        tabDbleChev = new JRadioButton(SciNotesMessages.TABDOUBLECHEVRONS, tabProps.type == ScilabView.TABDOUBLECHEVRONS);
        panelRepresentation.add(tabDbleChev);
        ButtonGroup repGroup = new ButtonGroup();
        repGroup.add(tabHRule);
        repGroup.add(tabVRule);
        repGroup.add(tabDbleChev);

        contentPanel.add(panelRepresentation);
        useWhitesCheckBox.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    boolean b = !useWhitesCheckBox.isSelected();
                    for (Component c : panelRepresentation.getComponents()) {
                        c.setEnabled(b);
                    }
                }
            });

        boolean b = !useWhitesCheckBox.isSelected();
        for (Component c : panelRepresentation.getComponents()) {
            c.setEnabled(b);
        }

        JPanel framePanel = new JPanel();
        framePanel.setLayout(new BoxLayout(framePanel, BoxLayout.PAGE_AXIS));
        framePanel.add(contentPanel);
        framePanel.add(panelButton);

        mainFrame.setContentPane(framePanel);

        cancelButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    ConfigTabulationsAction.windowAlreadyExist = false;
                    mainFrame.dispose();
                }
            });

        okButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    ConfigTabulationsAction.windowAlreadyExist = false;
                    refreshPane();
                    mainFrame.dispose();
                }
            });

        applyButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    ScilabEditorPane sep = getEditor().getTextPane();
                    IndentManager indent = sep.getIndentManager();
                    ScilabDocument doc = (ScilabDocument) sep.getDocument();
                    JScrollBar sb = sep.getScrollPane().getHorizontalScrollBar();
                    int val = -1;
                    if (sb != null) {
                        val = sb.getValue();
                    }
                    refreshPane();
                    indent.indentDoc(0, doc.getLength() - 1);
                    if (val != -1) {
                        sb.setValue(val);
                    }
                }
            });

        mainFrame.addWindowListener(new WindowListener() {
                public void windowClosed(WindowEvent arg0) { }
                public void windowDeiconified(WindowEvent arg0) { }
                public void windowActivated(WindowEvent arg0) { }

                public void windowClosing(WindowEvent arg0) {
                    closeConfTabWindow();
                }

                public void windowDeactivated(WindowEvent arg0) { }
                public void windowIconified(WindowEvent arg0) { };
                public void windowOpened(WindowEvent arg0) { }
            });

        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.setTitle(SciNotesMessages.CONFIGTAB);
        mainFrame.pack();
        mainFrame.setLocationRelativeTo(getEditor());
        mainFrame.setVisible(true);
    }

    /**
     * Refresh the pane
     */
    private void refreshPane() {
        char tab = ' ';
        if (!useWhitesCheckBox.isSelected()) {
            tab = '\t';
        }
        int len = ((Integer) tabLengthSpinner.getValue()).intValue();
        int type = ScilabView.TABNOTHING;
        if (tab == '\t' && tabVRule.isSelected()) {
            type = ScilabView.TABVERTICAL;
        } else if (tabHRule.isSelected()) {
            type = ScilabView.TABHORIZONTAL;
        } else if (tabDbleChev.isSelected()) {
            type = ScilabView.TABDOUBLECHEVRONS;
        }
        TabManager.Tabulation cfg = new TabManager.Tabulation(tab, len, type, ' ');
        ConfigSciNotesManager.saveDefaultTabulation(cfg);
        SciNotes.setDefaultTabulation(cfg);
    }
}
