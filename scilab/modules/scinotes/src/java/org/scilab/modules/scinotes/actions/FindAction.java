/* Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowFocusListener;
import java.awt.event.WindowListener;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import javax.swing.AbstractAction;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.Highlighter;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.SearchManager;
import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * FindAction
 * @author Sylvestre KOUMAR
 * @author Allan CORNET
 * @author Antoine ELIAS
 * @author Vincent COUVERT
 * @author Calixte DENIZET
 */
public final class FindAction extends DefaultAction implements WindowFocusListener {

    /**
     * serialVersionUID
     */
    private static final long serialVersionUID = -5499974793656106222L;

    private static final int GAP = 5;
    private static final int THREE = 3;

    private static final String FILTERNEWLINES = "filterNewlines";
    private static final String ESCAPE = "ESCAPE";

    private static final Color SELECTEDCOLOR = new Color(205, 183, 158);
    private static final Highlighter.HighlightPainter ACTIVEPAINTER = new DefaultHighlighter.DefaultHighlightPainter(Color.green);
    private static final Highlighter.HighlightPainter INACTIVEPAINTER = new DefaultHighlighter.DefaultHighlightPainter(Color.yellow);
    private static final Highlighter.HighlightPainter SELECTEDPAINTER = new DefaultHighlighter.DefaultHighlightPainter(SELECTEDCOLOR);

    private static String previousSearch;

    private static boolean windowAlreadyExist;
    private static FindAction current;

    private JFrame frame;
    private JButton buttonClose;
    private JButton buttonFind;
    private ButtonGroup buttonGroup1;
    private ButtonGroup buttonGroup2;
    private JButton buttonReplace;
    private JButton buttonReplaceAll;
    private JButton buttonReplaceFind;
    private JCheckBox checkCase;
    private JCheckBox checkRegular;
    private JCheckBox checkCircular;
    private JCheckBox checkWhole;
    private JComboBox comboFind;
    private JComboBox comboReplace;
    private JLabel labelFind;
    private JLabel labelReplace;
    private JPanel panelButton;
    private JPanel panelDirection;
    private JPanel panelFind;
    private JPanel panelFrame;
    private JPanel panelOptions;
    private JPanel panelScope;
    private SwingScilabTextBox statusBar;
    private JRadioButton radioAll;
    private JRadioButton radioFromCaret;
    private JRadioButton radioBackward;
    private JRadioButton radioForward;
    private JRadioButton radioSelection;

    private String oldWord;
    private String newWord;
    private String wordToFind;

    private String lastSearch = "";
    private String lastReplace = "";

    private int startSelectedLines;
    private int endSelectedLines;

    private int startFind;
    private int endFind;

    private boolean restarted;

    private Object[] highlighters;
    private Object selectedHighlight;

    private String previousRegexp;
    private int previousIndex;
    private List<Integer[]> foundOffsets;
    private final MyListener myListener = new MyListener();

    private boolean comboReplaceCanceled;
    private boolean comboFindCanceled;

    private Document previousDocument;

    /**
     * Constructor
     * @param name the name of the action
     * @param editor SciNotes
     */
    public FindAction(String name, SciNotes editor) {
        super(name, editor);
    }

    /**
     * doAction
     */
    @Override
    public void doAction() {
        if (windowAlreadyExist) {
            if (current != this) {
                current.closeFindReplaceWindow();
                current = this;
                findReplaceBox();
            } else {
                frame.setVisible(true);
                buttonFind.requestFocus();
                frame.toFront();
            }
        } else {
            current = this;
            findReplaceBox();
        }

        previousRegexp = "";
        previousIndex = -1;
        getEditor().getTextPane().removeCaretListener(myListener);
        startSelectedLines = 0;
        endSelectedLines = 0;

        try {
            // If some text is selected, it is used in find.
            //if more than one line is selected set radio button "selected lines" at true
            // else find and replace action is applied to the entire document
            JEditorPane scinotesTextPane = getEditor().getTextPane();
            int startPos = scinotesTextPane.getSelectionStart();
            int endPos = scinotesTextPane.getSelectionEnd();
            Element root = scinotesTextPane.getDocument().getDefaultRootElement();
            int startLine = root.getElementIndex(startPos);
            int endLine = root.getElementIndex(endPos);

            if (startPos != endPos) {
                if (startLine != endLine) {
                    radioSelection.doClick();
                    comboFind.setSelectedIndex(-1);
                    comboReplace.setSelectedIndex(-1);
                } else {
                    radioAll.doClick();
                    comboFind.getEditor().setItem(scinotesTextPane.getDocument().getText(startPos, endPos - startPos));
                    comboFind.getEditor().selectAll();
                }
            } else {
                radioAll.doClick();
                comboFind.setSelectedIndex(-1);
                comboReplace.setSelectedIndex(-1);
            }
            windowAlreadyExist = true;
            updateFindReplaceButtonStatus();
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }

    /**
     * createMenu
     * @param label label of the menu
     * @param editor SciNotes
     * @param key Keystroke
     * @return MenuItem
     */
    public static MenuItem createMenu(String label, SciNotes editor, KeyStroke key) {
        return createMenu(label, null, new FindAction(label, editor), key);
    }

    /**
     * createButton
     * @param tooltip the tooltip
     * @param icon an icon name searched in SCI/modules/gui/images/icons/
     * @param editor SciNotes
     * @return PushButton
     */
    public static JButton createButton(String tooltip, String icon, SciNotes editor) {
        return createButton(tooltip, icon, new FindAction(tooltip, editor));
    }

    /**
     * Close the eventually opened FindReplaceWindow
     */
    public static void close() {
        if (current != null) {
            current.closeFindReplaceWindow();
        }
    }

    /**
     * {@inheritedDoc}
     */
    @Override
    public void windowGainedFocus(WindowEvent e) {
        if (e.getWindow() == getEditor().getSwingParentWindow()) {
            frame.setAlwaysOnTop(true);
        }
    }

    /**
     * {@inheritedDoc}
     */
    @Override
    public void windowLostFocus(WindowEvent e) {
        if (e.getOppositeWindow() != frame && e.getOppositeWindow() != getEditor().getSwingParentWindow()) {
            frame.setAlwaysOnTop(false);
        }
    }

    /**
     * findReplaceBox
     */
    public void findReplaceBox() {

        //Find & Replace Frame
        frame = new JFrame();
        frame.setAlwaysOnTop(true);
        frame.getRootPane().getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE , 0), ESCAPE);
        frame.getRootPane().getActionMap().put(ESCAPE, new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                closeFindReplaceWindow();
            }
        });

        frame.setIconImage(new ImageIcon(FindIconHelper.findIcon("scilab")).getImage());
        frame.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        frame.setTitle(SciNotesMessages.FIND_REPLACE);
        frame.setResizable(false);

        getEditor().getSwingParentWindow().addWindowFocusListener(this);
        frame.addWindowFocusListener(this);

        buttonGroup1 = new ButtonGroup();
        buttonGroup2 = new ButtonGroup();
        panelFrame = new JPanel();
        panelDirection = new JPanel();
        radioForward = new JRadioButton();
        radioBackward = new JRadioButton();
        panelScope = new JPanel();
        radioAll = new JRadioButton();
        radioFromCaret = new JRadioButton();
        radioSelection = new JRadioButton();
        panelOptions = new JPanel();
        checkCase = new JCheckBox();
        checkWhole = new JCheckBox();
        checkRegular = new JCheckBox();
        checkCircular = new JCheckBox();
        panelFind = new JPanel();
        labelFind = new JLabel();
        labelReplace = new JLabel();
        comboFind = new JComboBox();
        comboReplace = new JComboBox();
        panelButton = new JPanel();
        buttonFind = new JButton();
        buttonReplaceFind = new JButton();
        buttonReplace = new JButton();
        buttonReplaceAll = new JButton();
        buttonClose = new JButton();
        statusBar = new SwingScilabTextBox();

        panelFrame.setLayout(new BoxLayout(panelFrame, BoxLayout.PAGE_AXIS));
        panelFrame.setBorder(BorderFactory.createEmptyBorder(2 * GAP, 2 * GAP, 2 * GAP, 2 * GAP));

        /* Pattern to search and replace*/
        labelFind.setText(SciNotesMessages.FIND);
        labelReplace.setText(SciNotesMessages.REPLACE);

        comboFind.setEditable(true);
        comboReplace.setEditable(true);

        panelFind.setLayout(new GridLayout(2, 2, GAP, GAP));
        panelFind.add(labelFind);
        panelFind.add(comboFind);
        panelFind.add(labelReplace);
        panelFind.add(comboReplace);
        panelFrame.add(panelFind);

        /* Search direction selection*/
        panelDirection.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.DIRECTION));

        buttonGroup1.add(radioForward);
        radioForward.setText(SciNotesMessages.FORWARD);
        buttonGroup1.add(radioBackward);
        radioBackward.setText(SciNotesMessages.BACKWARD);

        panelDirection.setLayout(new GridLayout(3, 1, GAP, GAP));
        panelDirection.add(radioForward);
        panelDirection.add(radioBackward);

        /* Scope for search */
        panelScope.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.SCOPE));

        buttonGroup2.add(radioAll);
        radioAll.setText(SciNotesMessages.SELECT_ALL);

        buttonGroup2.add(radioFromCaret);
        radioFromCaret.setText(SciNotesMessages.SELECT_FROM_CARET);

        buttonGroup2.add(radioSelection);
        radioSelection.setText(SciNotesMessages.SELECTED_LINES);

        panelScope.setLayout(new GridLayout(3, 1, GAP, GAP));
        panelScope.add(radioAll);
        panelScope.add(radioFromCaret);
        panelScope.add(radioSelection);

        JPanel panelDirectionScope = new JPanel();
        panelDirectionScope.setLayout(new GridLayout(1, 2, GAP, GAP));
        panelDirectionScope.add(panelDirection);
        panelDirectionScope.add(panelScope);
        panelFrame.add(panelDirectionScope);

        /* Misc Options */
        panelOptions.setBorder(BorderFactory.createTitledBorder(SciNotesMessages.OPTIONS));

        checkCase.setText(SciNotesMessages.CASE_SENSITIVE);
        checkWhole.setText(SciNotesMessages.WHOLE_WORD);
        checkRegular.setText(SciNotesMessages.REGULAR_EXPRESSIONS);
        checkCircular.setText(SciNotesMessages.CIRCULAR_SEARCH);

        checkCircular.setSelected(true);
        panelOptions.setLayout(new GridLayout(2, 2, GAP, GAP));
        panelOptions.add(checkCase);
        panelOptions.add(checkRegular);
        panelOptions.add(checkWhole);
        panelOptions.add(checkCircular);
        panelFrame.add(panelOptions);

        buttonFind.setText(SciNotesMessages.FIND_BUTTON);
        buttonReplaceFind.setText(SciNotesMessages.REPLACE_FIND);
        buttonReplace.setText(SciNotesMessages.REPLACE);
        buttonReplaceAll.setText(SciNotesMessages.REPLACE_ALL);
        buttonClose.setText(SciNotesMessages.CLOSE);
        statusBar.setText("");

        panelButton.setBorder(BorderFactory.createEmptyBorder(GAP, GAP, GAP, GAP));
        panelButton.setLayout(new GridLayout(THREE, THREE, GAP, GAP));
        panelButton.add(new JLabel());
        panelButton.add(buttonFind);
        panelButton.add(buttonReplaceFind);
        panelButton.add(new JLabel());
        panelButton.add(buttonReplace);
        panelButton.add(buttonReplaceAll);
        panelButton.add(new JLabel());
        panelButton.add(new JLabel());
        panelButton.add(buttonClose);

        panelFrame.add(panelButton);
        panelFrame.add(statusBar);

        frame.setContentPane(panelFrame);

        frame.pack();
        frame.setLocationRelativeTo(getEditor());
        frame.setVisible(true);

        buttonReplaceFind.setEnabled(false);
        buttonReplace.setEnabled(false);
        buttonFind.setEnabled(false);
        buttonReplaceAll.setEnabled(false);

        radioForward.setSelected(true);
        radioAll.doClick();

        fillSearch();
        fillReplace();

        restoreConfiguration();

        ((JTextField) comboReplace.getEditor().getEditorComponent()).getDocument().putProperty(FILTERNEWLINES, Boolean.FALSE);
        ((JTextField) comboFind.getEditor().getEditorComponent()).getDocument().putProperty(FILTERNEWLINES, Boolean.FALSE);
        checkCase.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveCaseSensitive(checkCase.isSelected());
                updateFindReplaceButtonStatus();
            }
        });

        checkWhole.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveWholeWord(checkWhole.isSelected());
                updateFindReplaceButtonStatus();
            }
        });

        checkCircular.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveCircularSearch(checkCircular.isSelected());
                updateFindReplaceButtonStatus();
            }
        });

        checkRegular.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                ConfigSciNotesManager.saveRegularExpression(checkRegular.isSelected());
                updateFindReplaceButtonStatus();
            }
        });

        /*behaviour of buttons*/
        radioSelection.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JEditorPane scinotesTextPane = getEditor().getTextPane();
                Element root = scinotesTextPane.getDocument().getDefaultRootElement();
                int startPos = scinotesTextPane.getSelectionStart();
                int endPos = scinotesTextPane.getSelectionEnd();
                int startLine = root.getElementIndex(startPos);
                int endLine = root.getElementIndex(endPos);
                startSelectedLines = root.getElement(startLine).getStartOffset();
                endSelectedLines = root.getElement(endLine).getEndOffset() - 1;

                scinotesTextPane.setCaretPosition(startSelectedLines);

                removeAllHighlights();
                setSelectedHighlight();

                scinotesTextPane.addFocusListener(new FocusListener() {
                    @Override
                    public void focusGained(FocusEvent e) {
                        JFrame opposite = (JFrame) SwingUtilities.getAncestorOfClass(JFrame.class, e.getOppositeComponent());
                        if (opposite == frame) {
                            removeAllHighlights();
                            previousRegexp = "";
                            radioAll.setSelected(true);
                            getEditor().getTextPane().removeFocusListener(this);
                        }
                    }

                    @Override
                    public void focusLost(FocusEvent e) { }
                });

                updateFindReplaceButtonStatus();
            }
        });

        radioAll.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                removeAllHighlights();
                previousRegexp = "";
                updateFindReplaceButtonStatus();
            }
        });

        radioFromCaret.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                removeAllHighlights();
                previousRegexp = "";
                updateFindReplaceButtonStatus();
            }
        });

        radioBackward.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateFindReplaceButtonStatus();
            }
        });

        buttonFind.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateRecentSearch();
                findText();
            }
        });

        buttonReplace.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateRecentSearch();
                updateRecentReplace();

                JEditorPane scinotesTextPane = getEditor().getTextPane();
                ScilabDocument doc = (ScilabDocument) scinotesTextPane.getDocument();

                doc.mergeEditsBegin();
                replaceOnlyText();
                doc.mergeEditsEnd();
            }

        });

        buttonReplaceFind.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateRecentSearch();
                updateRecentReplace();

                JEditorPane scinotesTextPane = getEditor().getTextPane();
                ScilabDocument doc = (ScilabDocument) scinotesTextPane.getDocument();

                doc.mergeEditsBegin();
                if (buttonReplace.isEnabled()) {
                    replaceText();
                }
                doc.mergeEditsEnd();
                findText();
            }
        });

        buttonReplaceAll.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                updateRecentSearch();
                updateRecentReplace();

                JEditorPane scinotesTextPane = getEditor().getTextPane();
                ScilabDocument doc = (ScilabDocument) scinotesTextPane.getDocument();
                int start = 0;
                int end = doc.getLength();
                String text = "";

                boolean wholeWordSelected = checkWhole.isSelected() && checkWhole.isEnabled();
                boolean regexpSelected = checkRegular.isSelected();
                boolean caseSelected = checkCase.isSelected();

                // save current caret position to restore it at the end
                int currentCaretPos = scinotesTextPane.getCaretPosition();

                if (radioSelection.isSelected()) {
                    start = startSelectedLines;
                    end = endSelectedLines;
                    try {
                        text = doc.getText(startSelectedLines, endSelectedLines - startSelectedLines);
                    } catch (BadLocationException ex) { }
                } else if (radioFromCaret.isSelected()) {
                    if (radioForward.isSelected()) {
                        start = currentCaretPos;
                        end = doc.getLength();
                    } else {
                        start = 0;
                        end = currentCaretPos;
                    }
                    try {
                        text = doc.getText(start, end - start);
                    } catch (BadLocationException ex) { }
                } else {
                    text = doc.getText();
                }

                oldWord = (String) comboFind.getEditor().getItem();
                newWord = (String) comboReplace.getEditor().getItem();
                setPreviousSearch(oldWord);

                Pattern pattern = SearchManager.generatePattern(oldWord, caseSelected, wholeWordSelected, regexpSelected);
                Matcher matcher = pattern.matcher(text);
                String replacedText;
                if (regexpSelected) {
                    newWord = newWord.replaceAll("\\\\r\\\\n|\\\\n", "\n").replaceAll("\\\\t", "\t");
                    replacedText = matcher.replaceAll(newWord);
                } else {
                    replacedText = matcher.replaceAll(Matcher.quoteReplacement(newWord));
                }
                if (replacedText.compareTo(text) != 0 && text.length() > 0) {
                    // only touch document if any replacement took place
                    try {
                        List<ScilabDocument.Anchor> anchors = doc.getAnchorsBetween(start, end);
                        doc.mergeEditsBegin();
                        doc.setFocused(true);
                        doc.replace(start, text.length(), replacedText, null);
                        doc.mergeEditsEnd();
                        Element root = doc.getDefaultRootElement();
                        for (ScilabDocument.Anchor anchor : anchors) {
                            ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) root.getElement(anchor.getLine());
                            line.setAnchor(anchor.toString());
                        }
                        if (getEditor().getNavigator() != null) {
                            getEditor().getNavigator().update();
                        }
                        previousRegexp = "";
                        previousIndex = -1;
                        buttonReplace.setEnabled(false);
                        buttonReplaceFind.setEnabled(false);
                        buttonReplaceAll.setEnabled(false);
                        scinotesTextPane.setCaretPosition(Math.min(currentCaretPos, doc.getLength()));
                    } catch (BadLocationException e1) {
                        e1.printStackTrace();
                    }
                }
                if (radioSelection.isSelected()) {
                    removeAllHighlights();
                    endSelectedLines = startSelectedLines + replacedText.length();
                    setSelectedHighlight();
                }
            }
        });

        buttonClose.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                closeFindReplaceWindow();
            }
        });

        comboReplace.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
                comboReplaceCanceled = true;
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) { }

            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }
        });

        comboReplace.getEditor().getEditorComponent().addMouseListener(new MouseListener() {
            @Override
            public void mouseReleased(MouseEvent e) { }
            @Override
            public void mousePressed(MouseEvent e) {
                closeComboPopUp();
                updateFindReplaceButtonStatus();
            }
            @Override
            public void mouseExited(MouseEvent e) { }
            @Override
            public void mouseEntered(MouseEvent e) { }
            @Override
            public void mouseClicked(MouseEvent e) { }
        });

        comboReplace.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) { }
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    if (comboReplaceCanceled) {
                        comboReplaceCanceled = false;
                    } else {
                        closeFindReplaceWindow();
                    }
                }

                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    replaceText();
                    findText();
                    buttonReplaceFind.requestFocus();
                }

                updateFindReplaceButtonStatus();
            }
            @Override
            public void keyPressed(KeyEvent e) { }
        });

        comboFind.addPopupMenuListener(new PopupMenuListener() {
            @Override
            public void popupMenuCanceled(PopupMenuEvent e) {
                comboFindCanceled = true;
            }

            @Override
            public void popupMenuWillBecomeInvisible(PopupMenuEvent e) { }

            @Override
            public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }
        });

        comboFind.getEditor().getEditorComponent().addMouseListener(new MouseListener() {
            @Override
            public void mouseReleased(MouseEvent arg0) { }
            @Override
            public void mousePressed(MouseEvent arg0) {
                closeComboPopUp();
            }
            @Override
            public void mouseExited(MouseEvent arg0) { }
            @Override
            public void mouseEntered(MouseEvent arg0) { }
            @Override
            public void mouseClicked(MouseEvent arg0) { }
        });

        comboFind.addItemListener(new ItemListener() {
            @Override
            public void itemStateChanged(ItemEvent e) {
                updateFindReplaceButtonStatus();
            }
        });

        comboFind.getEditor().getEditorComponent().addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) { }
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
                    if (comboFindCanceled) {
                        comboFindCanceled = false;
                    } else {
                        closeFindReplaceWindow();
                    }
                }

                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    buttonFind.requestFocus();
                    findText();
                }

                updateFindReplaceButtonStatus();
            }

            @Override
            public void keyPressed(KeyEvent e) { }
        });

        frame.addWindowListener(new WindowListener() {
            @Override
            public void windowClosed(WindowEvent e) { }
            @Override
            public void windowDeiconified(WindowEvent e) { }
            @Override
            public void windowActivated(WindowEvent e) { }

            @Override
            public void windowClosing(WindowEvent e) {
                closeFindReplaceWindow();
            }

            @Override
            public void windowDeactivated(WindowEvent e) { }
            @Override
            public void windowIconified(WindowEvent e) { }
            @Override
            public void windowOpened(WindowEvent e) { }
        });
    }

    /**
     * Close combo pop-up
     */
    private void closeComboPopUp() {
        if (comboFind.isPopupVisible()) {
            comboFind.hidePopup();
        }

        if (comboReplace.isPopupVisible()) {
            comboReplace.hidePopup();
        }
    }

    /**
     * fill comboFind
     */
    private void fillSearch() {
        comboFind.removeAllItems();
        List<String> recentFind = ConfigSciNotesManager.getRecentSearch();
        for (String item : recentFind) {
            comboFind.addItem(item);
        }
    }

    /**
     * fill comboReplace
     */
    private void fillReplace() {
        comboReplace.removeAllItems();
        List<String> recentReplace = ConfigSciNotesManager.getRecentReplace();
        for (String item : recentReplace) {
            comboReplace.addItem(item);
        }
    }

    /**
     * Update recent search
     */
    private void updateRecentSearch() {
        String word = (String) comboFind.getEditor().getItem();
        if (word != null && word.length() != 0 && !word.equals(lastSearch)) {
            List<String> recentFind = ConfigSciNotesManager.getRecentSearch();
            if (!recentFind.contains(word)) {
                comboFind.addItem(word);
                ConfigSciNotesManager.saveRecentSearch(word);
                lastSearch = word;
            }
        }
    }

    /**
     * Update recent replace
     */
    private void updateRecentReplace() {
        String word = (String) comboReplace.getEditor().getItem();
        if (word != null && word.length() != 0 && !word.equals(lastReplace)) {
            List<String> recentReplace = ConfigSciNotesManager.getRecentReplace();
            if (!recentReplace.contains(word)) {
                comboReplace.addItem(word);
                ConfigSciNotesManager.saveRecentReplace(word);
                lastReplace = word;
            }
        }
    }

    /**
     * Restore configuration
     */
    private void restoreConfiguration() {
        checkRegular.setSelected(ConfigSciNotesManager.getRegularExpression());
        checkCircular.setSelected(ConfigSciNotesManager.getCircularSearch());
        checkWhole.setSelected(ConfigSciNotesManager.getWholeWord());
        checkCase.setSelected(ConfigSciNotesManager.getCaseSensitive());
    }

    /**
     * Update status of buttons
     */
    protected void updateFindReplaceButtonStatus() {
        String textFind = (String) comboFind.getEditor().getItem();
        if (textFind.compareTo("") != 0) {
            buttonFind.setEnabled(true);
            buttonReplace.setEnabled(true);
            buttonReplaceAll.setEnabled(true);
            buttonReplaceFind.setEnabled(true);
            statusBar.setText("");
        } else {
            buttonFind.setEnabled(false);
            buttonReplace.setEnabled(false);
            buttonReplaceAll.setEnabled(false);
            buttonReplaceFind.setEnabled(false);
        }

        // permit to choose "whole word" only if the input is a single word
        Pattern patternWholeWord = Pattern.compile("\\w*");
        Matcher matcherWholeWord = patternWholeWord.matcher(textFind);

        checkWhole.setEnabled(false);

        if (matcherWholeWord.find()) {
            if ((matcherWholeWord.end() - matcherWholeWord.start()) == textFind.length()) {
                checkWhole.setEnabled(true);
            }
        }

        // if we search a regexp, we first need to know if the regexp is valid or not
        if (checkRegular.isSelected()) {
            try {
                Pattern.compile(textFind);
            } catch (PatternSyntaxException pse) {
                statusBar.setText(String.format(SciNotesMessages.INVALID_REGEXP, textFind));
                buttonFind.setEnabled(false);
                buttonReplaceAll.setEnabled(false);
            }
        }

        if (buttonReplace.isEnabled() && oldWord != null && oldWord.compareTo(textFind) != 0) {
            buttonReplace.setEnabled(false);
            buttonReplaceFind.setEnabled(false);
        }
    }

    /**
     * Add highlights on the view to see the searched word
     * @param active to highlight an active word
     */
    public void addHighlighters(boolean active) {
        Highlighter hl = getEditor().getTextPane().getHighlighter();
        removeAllHighlights();
        highlighters = new Object[foundOffsets.size()];
        for (int i = 0; i < foundOffsets.size(); i++) {
            try {
                if (active) {
                    highlighters[i] = hl.addHighlight(foundOffsets.get(i)[0], foundOffsets.get(i)[1], ACTIVEPAINTER);
                } else {
                    highlighters[i] = hl.addHighlight(foundOffsets.get(i)[0], foundOffsets.get(i)[1], INACTIVEPAINTER);
                }
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
        }

        if (radioSelection.isSelected()) {
            setSelectedHighlight();
        }
    }

    /**
     * Change the highlight with index n to active (or not) the word
     * @param n the index
     * @param active to highlight an active word
     */
    public void changeHighlighter(int n, boolean active) {
        if (highlighters != null) {
            Highlighter hl = getEditor().getTextPane().getHighlighter();
            hl.removeHighlight(highlighters[n]);
            try {
                if (active) {
                    highlighters[n] = hl.addHighlight(foundOffsets.get(n)[0], foundOffsets.get(n)[1], ACTIVEPAINTER);
                } else {
                    highlighters[n] = hl.addHighlight(foundOffsets.get(n)[0], foundOffsets.get(n)[1], INACTIVEPAINTER);
                }
            } catch (BadLocationException e) {
                e.printStackTrace();
            }

            if (radioSelection.isSelected()) {
                setSelectedHighlight();
            }
        }
    }

    /**
     * Set the selection where to search
     */
    public void setSelectedHighlight() {
        Highlighter hl = getEditor().getTextPane().getHighlighter();
        try {
            if (selectedHighlight != null) {
                hl.removeHighlight(selectedHighlight);
            }
            selectedHighlight = hl.addHighlight(startSelectedLines, endSelectedLines, SELECTEDPAINTER);
        } catch (BadLocationException e) {
            e.printStackTrace();
        }
    }

    /**
     * Remove an highlight
     * @param n th index of the word
     **/
    public void removeHighlighter(int n) {
        if (highlighters != null) {
            Highlighter hl = getEditor().getTextPane().getHighlighter();
            hl.removeHighlight(highlighters[n]);
        }
    }

    /**
     * Remove all Highlights
     */
    public void removeAllHighlights() {
        Highlighter hl = getEditor().getTextPane().getHighlighter();
        if (highlighters != null) {
            for (int i = 0; i < highlighters.length; i++) {
                hl.removeHighlight(highlighters[i]);
            }
        }
        if (selectedHighlight != null) {
            hl.removeHighlight(selectedHighlight);
            selectedHighlight = null;
        }
    }

    /**
     * Get the next keyword after the position pos
     * @param pos the position of the caret
     * @param forward for a forward search
     * @param circular for a circular search
     * @return the index of the word
     */
    private int getSearched(int pos, boolean forward, boolean circular) {
        int index = -1;
        for (int i = 0; i < foundOffsets.size(); i++) {
            if (forward && foundOffsets.get(i)[0] >= pos) {
                index = i;
                break;
            } else if (!forward && foundOffsets.get(i)[0] >= pos) {
                if (i == 0) {
                    return -1;
                }
                index = i - 1;
                break;
            }
        }

        if (index == -1 && circular) {
            if (forward) {
                return 0;
            } else {
                return foundOffsets.size() - 1;
            }
        }

        return index;
    }

    /**
     * Generate the offsets of the searched word
     * @return a boolean if a job has been done
     */
    public boolean generateOffsets() {
        boolean caseSensitive = checkCase.isSelected();
        boolean wholeWord = checkWhole.isSelected() && checkWhole.isEnabled();
        boolean useRegexp = checkRegular.isSelected();
        boolean onlySelectedLines = radioSelection.isSelected();
        Document doc = getEditor().getTextPane().getDocument();
        wordToFind = (String) comboFind.getEditor().getItem();

        String strregexp = SearchManager.generatePattern(wordToFind, caseSensitive, wholeWord, useRegexp).toString();
        if (doc != previousDocument || !previousRegexp.equals(strregexp)) {
            previousRegexp = strregexp;
            if (doc != previousDocument) {
                previousDocument = doc;
            }
            if (onlySelectedLines) {
                foundOffsets = SearchManager.findWord(doc, wordToFind, startSelectedLines, endSelectedLines, caseSensitive, wholeWord, useRegexp);
            } else {
                foundOffsets = SearchManager.findWord(doc, wordToFind, 0, doc.getLength(), caseSensitive, wholeWord, useRegexp);
            }
            return true;
        }
        return false;
    }

    /**
     * findText
     */
    private void findText() {
        boolean circularSearchSelected = checkCircular.isSelected();
        boolean forwardSearch = radioForward.isSelected();
        boolean backwardSearch = radioBackward.isSelected();

        String exp = (String) comboFind.getEditor().getItem();
        if (exp.compareTo("") == 0) {
            return;
        }

        setPreviousSearch(wordToFind);

        JEditorPane scinotesTextPane = getEditor().getTextPane();

        if (generateOffsets()) {
            addHighlighters(false);
            previousIndex = -1;
        }

        int currentCaretPos = scinotesTextPane.getSelectionStart();

        if (forwardSearch) {
            currentCaretPos = scinotesTextPane.getSelectionEnd();
        } else {
            currentCaretPos = scinotesTextPane.getSelectionStart() - 1;
        }

        statusBar.setText("");

        int size = foundOffsets.size();
        if (size > 0) {
            int nextIndex = -1;
            if (previousIndex == -1) {
                nextIndex = getSearched(currentCaretPos, !backwardSearch, circularSearchSelected);
            } else {
                if (backwardSearch) {
                    if (circularSearchSelected) {
                        nextIndex = (size + previousIndex - 1) % size;
                    } else {
                        nextIndex = previousIndex - 1;
                    }
                } else {
                    if (circularSearchSelected) {
                        nextIndex = (previousIndex + 1) % size;
                    } else {
                        nextIndex = previousIndex + 1;
                    }
                }
                changeHighlighter(previousIndex, false);
            }

            if (nextIndex == size) {
                nextIndex = -1;
            }

            if (nextIndex != -1) {
                if (restarted) {
                    statusBar.setText("");
                    restarted = false;
                } else if (circularSearchSelected && size >= 2 && nextIndex <= previousIndex) {
                    statusBar.setText(SciNotesMessages.RESTART_FROM_BEGINNING);
                    restarted = true;
                }

                changeHighlighter(nextIndex, true);
                previousIndex = nextIndex;
                if (backwardSearch) {
                    scinotesTextPane.setCaretPosition(foundOffsets.get(nextIndex)[0]);
                } else {
                    scinotesTextPane.setCaretPosition(foundOffsets.get(nextIndex)[1]);
                }

                scinotesTextPane.addFocusListener(myListener);
                buttonReplace.setEnabled(true);
                buttonReplaceFind.setEnabled(true);

                startFind = foundOffsets.get(nextIndex)[0];
                endFind = foundOffsets.get(nextIndex)[1];
            } else {
                startFind = 0;
                endFind = 0;
                buttonFind.setEnabled(false);
                buttonReplace.setEnabled(false);
                buttonReplaceFind.setEnabled(false);
                statusBar.setText(String.format(SciNotesMessages.STRING_NOT_FOUND, wordToFind));
            }
        } else { // nothing has been found
            statusBar.setText(String.format(SciNotesMessages.STRING_NOT_FOUND, wordToFind));
            buttonFind.setEnabled(false);
            buttonReplace.setEnabled(false);
            buttonReplaceFind.setEnabled(false);
            buttonReplaceAll.setEnabled(false);
        }
    }

    /**
     * replaceOnlyText
     */
    private void replaceOnlyText() {
        replaceText();
        buttonReplace.setEnabled(false);
        buttonReplaceFind.setEnabled(false);
    }

    /**
     * replaceText
     */
    private void replaceText() {
        boolean caseSensitive = checkCase.isSelected();
        boolean wholeWord = checkWhole.isSelected() && checkWhole.isEnabled();
        boolean useRegexp = checkRegular.isSelected();

        String find = (String) comboFind.getEditor().getItem();
        String replace = (String) comboReplace.getEditor().getItem();
        if (useRegexp) {
            replace = replace.replaceAll("\\\\r\\\\n|\\\\n", "\n").replaceAll("\\\\t", "\t");
        } else {
            replace = Matcher.quoteReplacement(replace);
        }

        if (find.compareTo("") == 0 || (startFind == endFind)) {
            return;
        }

        setPreviousSearch(find);

        JEditorPane scinotesTextPane = getEditor().getTextPane();

        /*
         * we replace only the current result and then disable replace and replace find button
         * same behaviour as find and replace in eclipse
         */

        try {
            Pattern pattern = SearchManager.generatePattern(find, caseSensitive, wholeWord, useRegexp);
            Matcher matcher = pattern.matcher(scinotesTextPane.getText(startFind, endFind - startFind + 1));
            String replacedText = matcher.replaceAll(replace);
            ScilabDocument doc = (ScilabDocument) scinotesTextPane.getDocument();
            doc.setFocused(true);
            doc.replace(startFind, endFind - startFind + 1, replacedText, null);
            scinotesTextPane.setCaretPosition(startFind + replacedText.length());
            endSelectedLines += replacedText.length() - (endFind - startFind + 1);
            previousRegexp = "";
            previousIndex = -1;
            if (generateOffsets()) {
                addHighlighters(false);
            }
        } catch (BadLocationException ex) {
            ex.printStackTrace();
        }
    }

    /**
     * closeFindReplaceWindow
     */
    public void closeFindReplaceWindow() {
        if (windowAlreadyExist) {
            JEditorPane scinotesTextPane = getEditor().getTextPane();
            if (scinotesTextPane != null) {
                scinotesTextPane.getHighlighter().removeAllHighlights();
                int start = scinotesTextPane.getSelectionStart();
                int end = scinotesTextPane.getSelectionEnd();
                scinotesTextPane.select(start, end);
            }
            getEditor().getSwingParentWindow().removeWindowFocusListener(this);
            frame.removeWindowFocusListener(this);
            frame.dispose();
            windowAlreadyExist = false;
        }
    }

    /**
     * Get the previous search
     * @return the previuos search
     */
    public static String getPreviousSearch() {
        return previousSearch;
    }

    /**
     * Set the previous search
     * @param previousSearch String
     */
    public static void setPreviousSearch(String previousSearch) {
        FindAction.previousSearch = previousSearch;
    }

    /**
     * Inner class to handle events (used as a singleton)
     */
    private class MyListener implements CaretListener, FocusListener {

        /**
         * Constructor
         */
        public MyListener() {
            super();
        }

        /**
         * focusGained in interface FocusListener
         * @param e FocusEvent
         */
        @Override
        public void focusGained(FocusEvent e) {
            if (previousIndex != -1) {
                int start = foundOffsets.get(previousIndex)[0];
                int end = foundOffsets.get(previousIndex)[1];
                getEditor().getTextPane().select(start, end);
            }
            getEditor().getTextPane().addCaretListener(this);
        }

        /**
         * focusLost in interface FocusListener
         * @param e FocusEvent
         */
        @Override
        public void focusLost(FocusEvent e) { }

        /**
         * caretUpdate in interface CaretListener
         * @param e FocusEvent
         */
        @Override
        public void caretUpdate(CaretEvent e) {
            removeAllHighlights();
            getEditor().getTextPane().removeCaretListener(this);
            getEditor().getTextPane().removeFocusListener(this);
            previousRegexp = "";
        }
    }
}
