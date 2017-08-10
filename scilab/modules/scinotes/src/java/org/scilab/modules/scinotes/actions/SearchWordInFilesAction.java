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

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;
import java.awt.event.WindowListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.bridge.filechooser.SwingScilabFileChooser;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.filechooser.ScilabFileChooser;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.SearchFile;

/**
 * SearchWordInFilesAction Class
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class SearchWordInFilesAction extends DefaultAction implements WindowFocusListener {

    private static final long serialVersionUID = 1L;

    private static final int GAP = 5;
    private static final String FILTERNEWLINES = "filterNewlines";
    private static final String ESCAPE = "ESCAPE";
    private static final String ENABLED = "enabled";
    private static final Color ERRORCOLOR = Color.RED;
    private static Color NORMALCOLOR;

    private static boolean windowAlreadyExist;
    private static JFrame mainFrame;
    private static Object searcher;
    private static SearchWordInFilesAction current;

    private JButton buttonFind;
    private JButton buttonStop;
    private JButton buttonClose;
    private JButton cancelButton;
    private JButton chooseBaseDirButton;
    private JComboBox comboBaseDir;
    private JComboBox comboFilePattern;
    private JComboBox comboWordPattern;
    private JCheckBox checkRecursive;
    private JCheckBox checkWordCase;
    private JCheckBox checkFileCase;
    private JCheckBox checkRegular;
    private JCheckBox checkLineByLine;
    private JCheckBox checkWhole;

    private boolean comboBaseDirCanceled;
    private boolean comboFilePatternCanceled;
    private boolean comboWordPatternCanceled;

    private String lastWordPattern;
    private String lastFilePattern;
    private String lastBaseDir;

    protected boolean searchFiles;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public SearchWordInFilesAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    @Override
    public void doAction() {
        current = this;
        openSearchWindow();
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, final SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new SearchWordInFilesAction(label, editor), key);
    }

    /**
     * Close the window
     */
    public static void closeWindow() {
        if (windowAlreadyExist) {
            stopSearch();
            SwingScilabWindow window = current.getEditor().getSwingParentWindow();
            if (window != null) {
                window.removeWindowFocusListener(current);
            }
            mainFrame.removeWindowFocusListener(current);
            mainFrame.dispose();
            windowAlreadyExist = false;
        }
    }

    /**
     * Start a search
     */
    public void startSearch() {
        if (SearchFile.isDone(searcher)) {
            buttonStop.setEnabled(true);
            buttonFind.setEnabled(false);
            String baseDir = getBaseDir((String) comboBaseDir.getEditor().getItem());
            boolean recursive = checkRecursive.isSelected();
            boolean lineByLine = checkLineByLine.isSelected();
            String filePattern = (String) comboFilePattern.getEditor().getItem();
            boolean fileCase = checkFileCase.isSelected();
            String wordPattern = null;
            if (!searchFiles) {
                wordPattern = (String) comboWordPattern.getEditor().getItem();
            }
            boolean wordCase = checkWordCase.isSelected();
            boolean wholeWord = checkWhole.isSelected();
            boolean regex = checkRegular.isSelected();
            searcher = SearchFile.getSearchResultsWindow(buttonStop, getEditor(), baseDir, recursive, !lineByLine, filePattern, fileCase, wordPattern, wordCase, wholeWord, regex);
        }
    }

    /**
     * Stop the current search if exists
     */
    public static void stopSearch() {
        if (searcher != null) {
            SearchFile.stopSearch(searcher);
            searcher = null;
        }
    }

    /**
     * {@inheritedDoc}
     */
    @Override
    public void windowGainedFocus(WindowEvent e) {
        if (e.getWindow() == getEditor().getSwingParentWindow()) {
            mainFrame.setAlwaysOnTop(true);
        }
    }

    /**
     * {@inheritedDoc}
     */
    @Override
    public void windowLostFocus(WindowEvent e) {
        if (e.getOppositeWindow() != mainFrame && e.getOppositeWindow() != getEditor().getSwingParentWindow()) {
            mainFrame.setAlwaysOnTop(false);
        }
    }

    /**
     * Open a window to get the name of the macro
     * @param name the name of a macro
     */
    public void openSearchWindow() {
        if (windowAlreadyExist) {
            mainFrame.setVisible(true);
            return;
        }

        mainFrame = new JFrame();
        mainFrame.setAlwaysOnTop(true);
        mainFrame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        mainFrame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE , 0), ESCAPE);
        mainFrame.getRootPane().getActionMap().put(ESCAPE, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                closeWindow();
            }
        });
        mainFrame.setTitle(SciNotesMessages.SEARCHINFILES);
        mainFrame.setIconImage(new ImageIcon(FindIconHelper.findIcon("system-search", "32x32")).getImage());

        getEditor().getSwingParentWindow().addWindowFocusListener(this);
        mainFrame.addWindowFocusListener(this);

        windowAlreadyExist = true;

        JLabel baseDirLabel = new JLabel(SciNotesMessages.BASEDIRECTORY);
        JLabel baseDirExpLabel = new JLabel(SciNotesMessages.BASEDIREXP);
        comboBaseDir = new JComboBox();
        comboBaseDir.setEditable(true);
        chooseBaseDirButton = new JButton(SciNotesMessages.CHOOSEDIR);
        JPanel panelBase = new JPanel();
        JLabel filePatternLabel = new JLabel(SciNotesMessages.FILEPATTERN + ":");
        JLabel filePatternExpLabel = new JLabel(SciNotesMessages.FILEPATTERNEXP);
        comboFilePattern = new JComboBox();
        comboFilePattern.setEditable(true);
        ((JTextField) comboFilePattern.getEditor().getEditorComponent()).setColumns(32);
        JLabel wordPatternLabel = new JLabel(SciNotesMessages.WORDPATTERN + ":");
        comboWordPattern = new JComboBox();
        comboWordPattern.setEditable(true);
        comboWordPattern.getEditor().addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                updateCombos();
                startSearch();
            }
        });
        ((JTextField) comboWordPattern.getEditor().getEditorComponent()).setColumns(32);
        NORMALCOLOR = ((JTextField) comboWordPattern.getEditor().getEditorComponent()).getForeground();

        JPanel panelOptions = new JPanel();
        panelOptions.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.OPTIONS));
        checkWordCase = new JCheckBox(SciNotesMessages.WORDCASESENSITIVE);
        checkFileCase = new JCheckBox(SciNotesMessages.FILECASESENSITIVE);
        checkWhole = new JCheckBox(SciNotesMessages.WHOLE_WORD);
        checkRegular = new JCheckBox(SciNotesMessages.REGULAR_EXPRESSIONS);
        checkLineByLine = new JCheckBox(SciNotesMessages.FILELINEBYLINE);
        checkRecursive = new JCheckBox(SciNotesMessages.RECURSIVESEARCH);
        panelOptions.setLayout(new GridLayout(3, 2, GAP, GAP));
        panelOptions.add(checkWordCase);
        panelOptions.add(checkFileCase);
        panelOptions.add(checkWhole);
        panelOptions.add(checkRegular);
        panelOptions.add(checkLineByLine);
        panelOptions.add(checkRecursive);

        buttonFind = new JButton(SciNotesMessages.FIND_BUTTON);
        buttonStop = new JButton(SciNotesMessages.STOPBUTTON);
        buttonStop.setEnabled(false);
        buttonClose = new JButton(SciNotesMessages.CLOSE);
        JPanel panelButton = new JPanel();
        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelButton.setLayout(new GridLayout(1, 4, GAP, GAP));
        panelButton.add(new JLabel());
        panelButton.add(buttonFind);
        panelButton.add(buttonStop);
        panelButton.add(buttonClose);

        restoreConfiguration();
        if (searchFiles) {
            comboWordPattern.setEnabled(false);
            checkWordCase.setEnabled(false);
            checkWhole.setEnabled(false);
            checkRegular.setEnabled(false);
            checkLineByLine.setEnabled(false);
            wordPatternLabel.setEnabled(false);
        }

        panelBase.setLayout(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();

        gbc.gridx = gbc.gridy = 0;
        gbc.gridwidth = gbc.gridheight = 1;
        gbc.weightx = gbc.weighty = 0;
        gbc.anchor = GridBagConstraints.LINE_START;
        panelBase.add(baseDirLabel, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.gridwidth = 3;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.BASELINE;
        gbc.insets = new Insets(0, 0, GAP, 0);
        panelBase.add(comboBaseDir, gbc);

        gbc.gridx = 1;
        gbc.gridy = 2;
        gbc.gridwidth = 2;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.EAST;
        gbc.insets = new Insets(0, 0, GAP, 0);
        panelBase.add(baseDirExpLabel, gbc);

        gbc.gridx = 3;
        gbc.gridy = 1;
        gbc.gridwidth = 1;
        gbc.weightx = 0;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.BASELINE;
        panelBase.add(chooseBaseDirButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.anchor = GridBagConstraints.LINE_START;
        panelBase.add(filePatternLabel, gbc);

        gbc.gridy = 4;
        gbc.gridwidth = 3;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.BASELINE;
        panelBase.add(comboFilePattern, gbc);

        gbc.gridx = 1;
        gbc.gridy = 5;
        gbc.gridwidth = 2;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.NONE;
        gbc.anchor = GridBagConstraints.EAST;
        gbc.insets = new Insets(0, 0, GAP, 0);
        panelBase.add(filePatternExpLabel, gbc);

        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.gridwidth = 1;
        gbc.weightx = 0;
        gbc.anchor = GridBagConstraints.LINE_START;
        panelBase.add(wordPatternLabel, gbc);

        gbc.gridy = 7;
        gbc.gridwidth = 3;
        gbc.weightx = 1;
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.anchor = GridBagConstraints.BASELINE;
        gbc.insets = new Insets(0, 0, GAP, 0);
        panelBase.add(comboWordPattern, gbc);

        JPanel panelFrame = new JPanel();
        panelFrame.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelFrame.setLayout(new BoxLayout(panelFrame, BoxLayout.PAGE_AXIS));
        panelFrame.add(panelBase);
        panelFrame.add(panelOptions);
        panelFrame.add(panelButton);

        mainFrame.setContentPane(panelFrame);

        mainFrame.addWindowListener(new WindowListener() {
            @Override
            public void windowClosed(WindowEvent arg0) { }
            @Override
            public void windowDeiconified(WindowEvent arg0) { }
            @Override
            public void windowActivated(WindowEvent arg0) { }

            @Override
            public void windowClosing(WindowEvent arg0) {
                closeWindow();
            }

            @Override
            public void windowDeactivated(WindowEvent arg0) { }
            @Override
            public void windowIconified(WindowEvent arg0) { };
            @Override
            public void windowOpened(WindowEvent arg0) { }
        });

        checkWordCase.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveCaseSensitive(checkWordCase.isSelected());
            }
        });

        checkFileCase.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveFileCase(checkFileCase.isSelected());
            }
        });

        checkWhole.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveWholeWord(checkWhole.isSelected());
            }
        });

        checkRecursive.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveRecursive(checkRecursive.isSelected());
            }
        });

        checkRegular.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveRegularExpression(checkRegular.isSelected());
            }
        });

        checkLineByLine.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveLineByLine(checkLineByLine.isSelected());
            }
        });

        chooseBaseDirButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                SwingScilabFileChooser fileChooser = ((SwingScilabFileChooser) ScilabFileChooser.createFileChooser().getAsSimpleFileChooser());
                fileChooser.setDialogTitle(SciNotesMessages.CHOOSEBASEDIR);
                fileChooser.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
                fileChooser.setAcceptAllFileFilterUsed(false);
                if (fileChooser.showOpenDialog(getEditor()) == JFileChooser.APPROVE_OPTION) {
                    File path = fileChooser.getSelectedFile();
                    if (path == null || path.isFile()) {
                        path = fileChooser.getCurrentDirectory();
                    }
                    comboBaseDir.getEditor().setItem(path.toString());
                    updateFindButtonStatus(true);
                }
            }
        });

        buttonFind.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateCombos();
                startSearch();
            }
        });

        buttonStop.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                stopSearch();
            }

        });

        buttonStop.addPropertyChangeListener(new PropertyChangeListener() {
            @Override
            public void propertyChange(PropertyChangeEvent e) {
                if (mainFrame.isVisible() && e.getPropertyName().equals(SearchFile.SEARCHDONE)) {
                    boolean newValue = (Boolean) e.getNewValue();
                    buttonFind.setEnabled(newValue);
                    buttonStop.setEnabled(!newValue);
                } else if (e.getPropertyName().equals(ENABLED)) {
                    boolean newValue = (Boolean) e.getNewValue();
                    comboBaseDir.setEnabled(!newValue);
                    comboFilePattern.setEnabled(!newValue);
                    chooseBaseDirButton.setEnabled(!newValue);
                }
            }
        });

        buttonClose.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                closeWindow();
            }

        });

        comboBaseDir.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                updateFindButtonStatus(true);
            }
        });

        comboBaseDir.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
                comboBaseDirCanceled = true;
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) { }

            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }
        });

        comboBaseDir.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) { }
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    if (comboBaseDirCanceled) {
                        comboBaseDirCanceled = false;
                    } else {
                        closeWindow();
                    }
                }
                updateFindButtonStatus(true);
            }

            @Override
            public void keyPressed(KeyEvent e) { }
        });

        comboFilePattern.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                updateFindButtonStatus(false);
            }
        });

        comboFilePattern.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
                comboFilePatternCanceled = true;
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) { }

            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }
        });

        comboFilePattern.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) { }
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    if (comboFilePatternCanceled) {
                        comboFilePatternCanceled = false;
                    } else {
                        closeWindow();
                    }
                }
                updateFindButtonStatus(false);
            }

            @Override
            public void keyPressed(KeyEvent e) { }
        });

        comboWordPattern.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
                comboWordPatternCanceled = true;
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) { }

            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }
        });

        comboWordPattern.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) { }
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    if (comboWordPatternCanceled) {
                        comboWordPatternCanceled = false;
                    } else {
                        closeWindow();
                    }
                }
                if (checkRegular.isSelected()) {
                    try {
                        Pattern.compile((String) comboWordPattern.getEditor().getItem());
                        ((JTextField) comboWordPattern.getEditor().getEditorComponent()).setForeground(NORMALCOLOR);
                        buttonFind.setEnabled(true);
                    } catch (PatternSyntaxException pse) {
                        ((JTextField) comboWordPattern.getEditor().getEditorComponent()).setForeground(ERRORCOLOR);
                        buttonFind.setEnabled(false);
                    }
                }
            }

            @Override
            public void keyPressed(KeyEvent e) { }
        });

        mainFrame.pack();
        mainFrame.setResizable(false);
        mainFrame.setLocationRelativeTo(getEditor());
        mainFrame.setVisible(true);
    }

    /**
     * Update status of buttons
     */
    protected void updateFindButtonStatus(boolean baseDirModified) {
        String baseDir = (String) comboBaseDir.getEditor().getItem();
        String filePattern = (String) comboFilePattern.getEditor().getItem();
        boolean goodBaseDir = !baseDirModified;
        if (baseDirModified && !baseDir.isEmpty()) {
            baseDir = getBaseDir(baseDir);
            File dir = new File(baseDir);
            if (dir.exists() && dir.isDirectory()) {
                goodBaseDir = true;
            }
        }
        if (goodBaseDir) {
            ((JTextField) comboBaseDir.getEditor().getEditorComponent()).setForeground(NORMALCOLOR);
            buttonFind.setEnabled(!filePattern.isEmpty());
        } else {
            ((JTextField) comboBaseDir.getEditor().getEditorComponent()).setForeground(ERRORCOLOR);
            buttonFind.setEnabled(false);
        }
    }

    /**
     * Replace ~ by user home
     * @param baseDir the base directory
     * @return correct base directory
     */
    private static String getBaseDir(String baseDir) {
        baseDir = baseDir.trim();
        if (baseDir != null && !baseDir.isEmpty()) {
            if (baseDir.startsWith("~" + File.separator) || baseDir.equals("~")) {
                return baseDir.replaceFirst("~", ScilabConstants.USERHOME);
            } else if (baseDir.startsWith("SCI" + File.separator) || baseDir.equals("SCI")) {
                try {
                    return baseDir.replaceFirst("SCI", ScilabConstants.SCI.getCanonicalPath());
                } catch (IOException e) { }
            }
        }

        return baseDir;
    }

    /**
     * Update the combos
     */
    public void updateCombos() {
        lastBaseDir = updateRecent(comboBaseDir, lastBaseDir, ConfigSciNotesManager.RECENTBASEDIR, ConfigSciNotesManager.BASEDIR);
        lastFilePattern = updateRecent(comboFilePattern, lastFilePattern, ConfigSciNotesManager.RECENTFILEPATTERN, ConfigSciNotesManager.FILEPATTERN);
        lastWordPattern = updateRecent(comboWordPattern, lastWordPattern, ConfigSciNotesManager.RECENTWORDPATTERN, ConfigSciNotesManager.WORDPATTERN);
    }

    /**
     * Restore configuration
     */
    private void restoreConfiguration() {
        checkRegular.setSelected(ConfigSciNotesManager.getRegularExpression());
        checkWhole.setSelected(ConfigSciNotesManager.getWholeWord());
        checkWordCase.setSelected(ConfigSciNotesManager.getCaseSensitive());
        checkFileCase.setSelected(ConfigSciNotesManager.getFileCase());
        checkRecursive.setSelected(ConfigSciNotesManager.getRecursive());
        checkLineByLine.setSelected(ConfigSciNotesManager.getLineByLine());
        fillCombo(comboBaseDir, ConfigSciNotesManager.RECENTBASEDIR, ConfigSciNotesManager.BASEDIR);
        fillCombo(comboFilePattern, ConfigSciNotesManager.RECENTFILEPATTERN, ConfigSciNotesManager.FILEPATTERN);
        fillCombo(comboWordPattern, ConfigSciNotesManager.RECENTWORDPATTERN, ConfigSciNotesManager.WORDPATTERN);
    }

    /**
     * fill comboBaseDir
     */
    private static void fillCombo(JComboBox combo, String nodeName, String childNodeName) {
        combo.removeAllItems();
        List<String> recent = ConfigSciNotesManager.getRecent(nodeName, childNodeName);
        for (String item : recent) {
            combo.addItem(item);
        }
    }

    /**
     * Update recent base directory
     */
    private static String updateRecent(JComboBox combo, String last, String nodeName, String childNodeName) {
        String str = (String) combo.getEditor().getItem();
        if (str != null && str.length() != 0 && !str.equals(last)) {
            List<String> recent = ConfigSciNotesManager.getRecent(nodeName, childNodeName);
            if (!recent.contains(str)) {
                combo.addItem(str);
                combo.setSelectedItem(str);
                ConfigSciNotesManager.saveRecent(str, nodeName, childNodeName);
                last = str;
            }
        }
        return last;
    }
}
