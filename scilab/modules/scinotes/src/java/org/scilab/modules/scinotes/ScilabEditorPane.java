/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.File;
import java.util.ArrayList;
import java.util.EventObject;
import java.util.List;
import java.util.UUID;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JEditorPane;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.Caret;
import javax.swing.text.DefaultHighlighter;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.Highlighter;
import javax.swing.text.JTextComponent;
import javax.swing.text.View;

import org.scilab.modules.commons.gui.ScilabCaret;
import org.scilab.modules.console.utils.ScilabLaTeXViewer;
import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.scinotes.actions.CopyAsHTMLAction;
import org.scilab.modules.scinotes.actions.OpenSourceFileOnKeywordAction;
import org.scilab.modules.scinotes.utils.NavigatorWindow;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Class ScilabEditorPane
 * @author Calixte DENIZET
 *
 */
public class ScilabEditorPane extends JEditorPane implements Highlighter.HighlightPainter,
                                                  CaretListener, MouseListener,
                                                  MouseMotionListener, Cloneable,
                                                  KeyListener {

    private static final long serialVersionUID = 4322071415211939097L;

    private static final String TIRET = " - ";
    private static final Cursor HANDCURSOR = Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
    private static final Cursor TEXTCURSOR = Cursor.getPredefinedCursor(Cursor.TEXT_CURSOR);
    private static final DefaultHighlighter.DefaultHighlightPainter HIGHLIGHTER = new DefaultHighlighter.DefaultHighlightPainter(Color.YELLOW);

    private static ScilabEditorPane focused;

    private Color highlightColor;
    private Color highlightContourColor;
    private boolean highlightEnable;
    private Object highlightCL;
    private boolean matchingEnable;
    private boolean overwriteMode;
    private ScilabLexer lexer;
    private SciNotes editor;
    private IndentManager indent;
    private TabManager tab;
    private CommentManager com;
    private HelpOnTypingManager helpOnTyping;
    private TrailingWhiteManager trailingWhite;
    private boolean readonly;
    private boolean binary;
    private String infoBar = "";
    private String shortName = "";
    private String title = "";

    private Point mousePoint;

    private long lastModified;

    /* matchLR matches Left to Right ... */
    private MatchingBlockManager matchLR;
    private MatchingBlockManager matchRL;

    private boolean suppressCom = true;

    private SciNotesLineNumberPanel xln;
    private ScilabEditorPane rightTextPane;
    private UUID uuid;

    private EditorComponent edComponent;

    private boolean hand;
    private boolean infoBarChanged;
    private boolean ctrlHit;

    private Color saveHighlightContourColor;
    private Color saveHighlightColor;
    private boolean hasBeenSaved;
    private boolean saveHighlightEnable;

    private List<KeywordListener> kwListeners = new ArrayList<KeywordListener>();
    private List<Object> highlightedWords = new ArrayList<Object>();
    private List<Integer> highlightedWordsBegin = new ArrayList<Integer>();

    /**
     * Constructor
     * @param editor which uses this pane
     */
    public ScilabEditorPane(SciNotes editor) {
        super();
        if (focused == null) {
            focused = this;
        }
        this.editor = editor;
        this.uuid = UUID.randomUUID();
        edComponent = new EditorComponent(this);

        addCaretListener(this);
        addMouseMotionListener(this);
        addMouseListener(this);
        enableMatchingKeywords(true);
        setFocusable(true);
        addFocusListener(new FocusListener() {
                public void focusGained(FocusEvent e) {
                    updateInfosWhenFocused();
                    NavigatorWindow.updateNavigator((ScilabDocument) getDocument());
                }

                public void focusLost(FocusEvent e) {
                    ((ScilabDocument) getDocument()).setFocused(false);
                }
            });

        addKeywordListener(new KeywordAdaptater.MouseOverAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    if (ScilabLexerConstants.isClickable(e.getType())) {
                        if (ctrlHit) {
                            setCursor(HANDCURSOR);
                            hand = true;
                            try {
                                String url = ((ScilabDocument) getDocument()).getText(e.getStart(), e.getLength());
                                if (ScilabLexerConstants.isClickable(e.getType())) {
                                    String text;
                                    switch (e.getType()) {
                                    case ScilabLexerConstants.URL :
                                        text = SciNotesMessages.OPENURL;
                                        break;
                                    case ScilabLexerConstants.MAIL :
                                        text = SciNotesMessages.MAILTO;
                                        break;
                                    case ScilabLexerConstants.MACROS :
                                        text = SciNotesMessages.OPENSOURCE;
                                        break;
                                    case ScilabLexerConstants.MACROINFILE :
                                        text = SciNotesMessages.SHOWSOURCE;
                                        break;
                                    default :
                                        text = null;
                                    }

                                    if (text != null) {
                                        ScilabEditorPane.this.editor.getInfoBar().setText(text + url);
                                        infoBarChanged = true;
                                    }
                                }
                            } catch (BadLocationException ex) { }
                        } else {
                            ScilabEditorPane.this.editor.getInfoBar().setText(SciNotesMessages.CLICKABLE_URL);
                            infoBarChanged = true;
                        }
                    } else {
                        if (hand) {
                            setCursor(TEXTCURSOR);
                            hand = false;
                        }
                        if (infoBarChanged) {
                            ScilabEditorPane.this.editor.getInfoBar().setText(infoBar);
                            infoBarChanged = false;
                        }
                        if (ScilabLexerConstants.isLaTeX(e.getType())) {
                            try {
                                int start = e.getStart();
                                int end = start + e.getLength();
                                String exp = ((ScilabDocument) getDocument()).getText(start, e.getLength());
                                int height = edComponent.getScrollPane().getHeight() + edComponent.getScrollPane().getVerticalScrollBar().getValue();
                                ScilabLaTeXViewer.displayExpression(ScilabEditorPane.this, height, exp, start, end);
                            } catch (BadLocationException ex) { }
                        } else {
                            ScilabLaTeXViewer.removeLaTeXViewer(ScilabEditorPane.this);
                        }
                    }
                }
            });

        addKeywordListener(new KeywordAdaptater.MouseClickedAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    if (ctrlHit && ScilabLexerConstants.isClickable(e.getType())) {
                        try {
                            hand = false;
                            ctrlHit = false;
                            infoBarChanged = false;
                            setCursor(TEXTCURSOR);
                            ScilabEditorPane.this.editor.getInfoBar().setText(infoBar);
                            String url = ((ScilabDocument) getDocument()).getText(e.getStart(), e.getLength());
                            if (ScilabLexerConstants.URL == e.getType() || ScilabLexerConstants.MAIL == e.getType()) {
                                WebBrowser.openUrl(url);
                            } else if (ScilabLexerConstants.isOpenable(e.getType())) {
                                OpenSourceFileOnKeywordAction.openSource(ScilabEditorPane.this, url);
                            }
                        } catch (BadLocationException ex) { }
                    }
                }
            });

        addKeyListener(this);
        setTransferHandler(new CopyAsHTMLAction.HTMLTransferHandler());
    }

    /**
     * @return the lexer
     */
    public ScilabLexer getLexer() {
        return lexer;
    }

    /**
     * {@inheritDoc}
     * When no split and in wrapped view , this method return true and the consequence is
     * that there is no horizontal scrollbar.
     */
    public boolean getScrollableTracksViewportWidth() {
        return ((ScilabDocument) getDocument()).getView() instanceof ScilabView && !edComponent.isSplited();
    }

    /**
     * @return true if the pane is in OverWrite mode (insert)
     */
    public boolean getOverwriteMode() {
        return this.overwriteMode;
    }

    /**
     * @param overwriteMode true if the pane is in OverWrite mode (insert)
     */
    public void setOverwriteMode(boolean overwriteMode) {
        this.overwriteMode = overwriteMode;
        ((ScilabCaret) getCaret()).setOverwriteMode(overwriteMode);
    }

    /**
     * {@inheritDoc}
     */
    public void replaceSelection(String content) {
        if (overwriteMode && getSelectionStart() == getSelectionEnd()) {
            int pos = getCaretPosition();
            select(pos, pos + content.length());
        }

        if (((SciNotesCaret) getCaret()).isEmptySelection()) {
            super.replaceSelection(content);
        } else {
            SciNotesCaret caret = (SciNotesCaret) getCaret();
            int[][] pos = caret.getSelectedPositions();
            List<Object> sels = caret.getSelections();
            int len = content.length();
            int res = 0;
            int sres;
            ScilabDocument doc = (ScilabDocument) getDocument();
            ((CompoundUndoManager) doc.getUndoManager()).enableOneShot(true);
            doc.mergeEditsBegin();
            caret.protectHighlights(true);
            for (int i = 0; i < pos.length; i++) {
                if (sels.get(i) != null) {
                    sres = pos[i][0] + res;
                    try {
                        doc.replace(sres, pos[i][1] - pos[i][0], content, null);
                    } catch (BadLocationException e) { }
                    res = sres + len - pos[i][1];
                    pos[i][0] = sres + len;
                    pos[i][1] = sres + len;
                }
            }
            doc.mergeEditsEnd();
            ((CompoundUndoManager) doc.getUndoManager()).enableOneShot(false);
            caret.protectHighlights(false);
            caret.updateHighlights();
        }
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyPressed(KeyEvent e) {
        // Workaround for bug 7238
        if (e.getKeyLocation() == KeyEvent.KEY_LOCATION_NUMPAD
            && e.getKeyCode() == KeyEvent.VK_DELETE
            && e.getKeyChar() != KeyEvent.VK_DELETE) {
            e.setKeyCode(KeyEvent.VK_DECIMAL);
            ctrlHit = false;
        } else if (e.getKeyCode() == KeyEvent.VK_CONTROL) {
            ctrlHit = true;
        } else {
            ctrlHit = false;
        }
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyReleased(KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_CONTROL) {
            ctrlHit = false;
        }
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void keyTyped(KeyEvent e) { }

    /**
     * @overload #setDocument
     * @param doc to set
     */
    public void setDocument(Document doc) {
        super.setDocument(doc);
        if (doc instanceof ScilabDocument) {
            ((ScilabDocument) doc).getUndoManager().discardAllEdits();
            initialize((ScilabDocument) doc);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setName(String name) {
        setNameInSuper(name);
        setShortNameAndTitle(name);
        ScilabEditorPane pane = getOtherPaneInSplit();
        if (pane != null) {
            // I don't call pane.setName since we will enter in an infinite loop
            pane.setNameInSuper(name);
            pane.setShortNameAndTitle(name);
        }
    }

    /**
     * @param name the name
     */
    private void setNameInSuper(String name) {
        super.setName(name);
    }

    /**
     * @param name the name
     */
    private void setShortNameAndTitle(String name) {
        if (name != null) {
            File f = new File(name);
            setShortName(f.getName());
            title =  shortName + " (" + f.getAbsolutePath() + ")" + TIRET + SciNotesMessages.SCILAB_EDITOR;
        }
    }

    /**
     * @param title the title
     */
    public void setTitle(String title) {
        this.title = title + TIRET + SciNotesMessages.SCILAB_EDITOR;
    }

    /**
     * @return the title
     */
    public String getTitle() {
        return title;
    }

    /**
     * @param name the short name
     */
    public void setShortName(String name) {
        this.shortName = name;
    }

    /**
     * @return the short name of the file (without the full path)
     */
    public String getShortName() {
        return shortName;
    }

    /**
     * Init the pane
     */
    public void init() {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    requestFocus();
                    setCaretPosition(0);
                }
            });
    }

    /**
     * Close this pane
     */
    public void close() {
        FocusListener[] l = getFocusListeners();
        for (int i = 0; i < l.length; i++) {
            removeFocusListener(l[i]);
        }
    }

    /**
     * Update infos
     */
    public void updateInfosWhenFocused() {
        ScilabDocument doc = (ScilabDocument) getDocument();
        doc.setFocused(true);
        SciNotes.setEditor(editor);
        focused = this;
        doc.getUndoManager().enableUndoRedoButtons();

        if (checkExternalModif()) {
            editor.getInfoBar().setText(SciNotesMessages.EXTERNAL_MODIFICATION_INFO);
        } else {
            editor.getInfoBar().setText(getInfoBarText());
        }
    }

    /**
     * @return true if an external modif occured
     */
    public boolean checkExternalModif() {
        String path = getName();
        if (path != null) {
            File f = new File(path);
            if (f != null && f.exists()) {
                return lastModified < f.lastModified();
            } else {
                return true;
            }
        }
        return false;
    }

    /**
     * @return the String which must be displayed in the infobar
     */
    public String getInfoBarText() {
        return infoBar;
    }

    /**
     * @param text String which must be displayed in the infobar
     */
    public void setInfoBarText(String text) {
        this.infoBar = text;
        editor.getInfoBar().setText(getInfoBarText());
    }

    /**
     * @param readonly true to set Read-Only mode
     */
    public void setReadOnly(boolean readonly) {
        this.readonly = readonly;
        setEditable(!readonly);
        setDragEnabled(!readonly);
        if (readonly) {
            infoBar = SciNotesMessages.READ_ONLY_MODE;
        } else {
            infoBar = "";
        }
    }

    /**
     * @param binary true to set binary mode
     */
    public void setBinary(boolean binary) {
        this.binary = binary;
        setEditable(!binary);
        setDragEnabled(!binary);
        if (binary) {
            infoBar = SciNotesMessages.BINARY_FILE_MODE;
            disableAll();
        } else {
            infoBar = "";
        }
    }

    /**
     * Copy the props of this textPane to pane
     * @param pane the pane which receives the same props as this
     */
    public void copyProps(ScilabEditorPane pane) {
        pane.lastModified = lastModified;
        pane.highlightEnable = highlightEnable;
        pane.matchingEnable = matchingEnable;
        pane.suppressCom = suppressCom;
        pane.setName(getName());
        pane.setShortName(getShortName());
        pane.setTitle(getTitle().substring(0, getTitle().lastIndexOf(TIRET)));
        pane.setEditable(isEditable());
    }

    /**
     * Get the UUID associated with the editor pane instance.
     * @return unique identifier
     */
    public UUID getUUID() {
        return uuid;
    }

    /**
     * Get the the time where the file associated with this pane
     * has been modified.
     * @return the last modified time or 0
     */
    public long getLastModified() {
        return lastModified;
    }

    /**
     * Set the last time where the file associated with this pane
     * has been modified.
     * @param time the time
     */
    public void setLastModified(long time) {
        this.lastModified = time;
        ScilabEditorPane pane = getOtherPaneInSplit();
        if (pane != null) {
            pane.lastModified = time;
        }
    }

    /**
     * @return the SciNotesLineNumberPanel used with this pane
     */
    public SciNotesLineNumberPanel getXln() {
        return xln;
    }

    /**
     * @return the editor
     */
    public SciNotes getEditor() {
        return editor;
    }

    /**
     * @param editor to set
     */
    public void setEditor(SciNotes editor) {
        this.editor = editor;
    }

    /**
     * Disable all
     */
    public void disableAll() {
        indent = null;
        tab = null;
        com = null;
        trailingWhite = null;
        enableMatchingKeywords(false);
        matchLR.desactivateMouseOver();
        matchLR = null;
        matchRL.desactivateMouseOver();
        matchRL = null;
    }

    /**
     * @return the scrollPane or the splitpane associated with this textPane
     */
    public EditorComponent getEditorComponent() {
        return edComponent;
    }

    /**
     * @return the scrollPane or the splitpane associated with this textPane
     */
    public void setEditorComponent(EditorComponent ed) {
        this.edComponent = ed;
    }

    /**
     * @param split the split used
     */
    public void setSplitPane(JSplitPane split) {
        edComponent.setSplitPane(split);
    }

    /**
     * @param split the split used
     */
    public JSplitPane getSplitPane() {
        return edComponent.getSplitPane();
    }

    /**
     * Update the title of current tab
     */
    public void updateTitle() {
        editor.updateTabTitle();
    }

    /**
     * Scroll the pane to have the line containing pos on the top of the pane
     * @param pos the position in the document
     */
    public void scrollTextToPos(int pos) {
        scrollTextToPos(pos, true, false);
    }

    /**
     * Scroll the pane to have the line containing pos on the top or centered on the pane
     * @param pos the position in the document
     * @param setCaret, if true the caret is set at the given position
     * @param centered, if true the line is centered
     */
    public void scrollTextToPos(final int pos, final boolean setCaret, final boolean centered) {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    try {
                        if (setCaret) {
                            setCaretPosition(pos);
                        }
                        JScrollBar scrollbar = edComponent.getScrollPane().getVerticalScrollBar();
                        Rectangle rect = modelToView(pos);
                        if (centered) {
                            int value = scrollbar.getValue();
                            int h = scrollbar.getHeight();
                            if (rect.y < value || rect.y > value + h) {
                                scrollbar.setValue(Math.max(0, rect.y - h / 2));
                            }
                        } else {
                            if (rect.y > scrollbar.getMaximum()) {
                                scrollbar.setMaximum(rect.y);
                            }
                            scrollbar.setValue(rect.y);
                        }
                    } catch (BadLocationException e) { }
                }
            });
    }

    /**
     * Scroll the pane to have the line lineNumber on the top of the pane
     * @param lineNumber the number of the line
     * @param highlight true to highlight the line
     */
    public void scrollTextToLineNumber(int lineNumber, boolean highlight) {
        scrollTextToLineNumber(lineNumber, highlight, true, false);
    }

    /**
     * Scroll the pane to have the line lineNumber on the top or centered on the pane
     * @param lineNumber the number of the line
     * @param highlight true to highlight the line
     * @param setCaret, if true the caret is set at the given line
     * @param centered, if true the line is centered
     */
    public void scrollTextToLineNumber(int lineNumber, final boolean highlight, final boolean setCaret, final boolean centered) {
        Element root = getDocument().getDefaultRootElement();
        if (lineNumber >= 1 && lineNumber <= root.getElementCount()) {
            final int pos = root.getElement(lineNumber - 1).getStartOffset();
            SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        scrollTextToPos(pos, setCaret, centered);
                        if (highlight) {
                            saveHighlightContourColor = highlightContourColor;
                            highlightContourColor = null;
                            saveHighlightColor = highlightColor;
                            highlightColor = Color.YELLOW;
                            saveHighlightEnable = highlightEnable;
                            hasBeenSaved = true;
                            enableHighlightedLine(false);
                            enableHighlightedLine(true);
                        }
                    }
                });
        }
    }

    /**
     * Scroll the pane to have the line lineNumber on the top of the pane in whereami mode
     * The line number is computed regarding the function named funname.
     * @param lineNumber the number of the line
     * @param funname the function name
     * @param highlight true to highlight the line
     */
    public void scrollTextToLineNumberInWhereami(int lineNumber, String funname, boolean highlight) {
        scrollTextToLineNumberInWhereami(lineNumber, funname, highlight, true, false);
    }

    /**
     * Scroll the pane to have the line lineNumber on the top of the pane in whereami mode
     * The line number is computed regarding the function named funname.
     * @param lineNumber the number of the line
     * @param funname the function name
     * @param highlight true to highlight the line
     * @param setCaret, if true the caret is set at the given line
     * @param centered, if true the line is centered
     */
    public void scrollTextToLineNumberInWhereami(int lineNumber, String funname, boolean highlight, boolean setCaret, boolean centered) {
        if (funname != null) {
            Element root = getDocument().getDefaultRootElement();
            int nlines = root.getElementCount();
            ScilabDocument.ScilabLeafElement elem;
            for (int i = 0; i < nlines; i++) {
                elem = (ScilabDocument.ScilabLeafElement) root.getElement(i);
                if (elem.getFunctionName().equals(funname)) {
                    lineNumber += i;
                    break;
                }
            }
        }
        scrollTextToLineNumber(lineNumber, highlight, setCaret, centered);
    }

    /**
     * @return the width of a white
     */
    public int getWhiteWidth() {
        View view = ((ScilabDocument) getDocument()).getView();
        if (view != null) {
            if (view instanceof ScilabView) {
                return ((ScilabView) view).getWhiteWidth();
            } else {
                return ((ScilabPlainView) view).getWhiteWidth();
            }
        }

        return 0;
    }

    /**
     * Set a new font
     * @param font to set
     */
    public void resetFont(Font font) {
        setFont(font);
        ((ScilabEditorKit) getEditorKit()).getStylePreferences().genFonts(font);
        View view = ((ScilabDocument) getDocument()).getView();
        if (view != null) {
            if (view instanceof ScilabView) {
                ((ScilabView) view).reinitialize();
            } else {
                ((ScilabPlainView) view).reinitialize();
            }
        }
        xln.updateFont(font);
    }

    /**
     * Set a new font
     * @param keyword the type of keyword
     * @param type an int : -2 to reset italic, -1 to reset bold, 1 to set bold and 2 to set italic
     */
    public void resetFont(String keyword, int type) {
        ((ScilabEditorKit) getEditorKit()).getStylePreferences().genFont(keyword, type);
    }

    /**
     * Set a new attribute
     * @param keyword the type of keyword
     * @param type an int : 0 nothing, 1 underline, 2 stroke, 3 underline and stroke
     */
    public void resetAttribute(String keyword, int type) {
        ((ScilabEditorKit) getEditorKit()).getStylePreferences().genAttribute(keyword, type);
    }

    /**
     * Set a new color
     * @param keyword the kind of the keyword
     * @param color the color
     */
    public void resetColor(String keyword, Color color) {
        ((ScilabEditorKit) getEditorKit()).getStylePreferences().genColors(keyword, color);
    }

    /**
     * Set to true if the comments must be suppressed when the code is executing in the console
     * @param b boolean
     */
    public void suppressCommentsInExecutingCode(boolean b) {
        suppressCom = b;
    }

    /**
     * Execute the code in the console, the code is the selected text if exists
     * or the text from beginning to actual position of the caret
     * Comments are removed if suppressCom is set to true
     * @return the code to be executed in the console.
     */
    public String getCodeToExecute() {
        String selection;
        int start, end;
        start = getSelectionStart();
        end = getSelectionEnd();

        if (((SciNotesCaret) getCaret()).isEmptySelection()) {
            try {
                if (start == end) {
                    selection = getDocument().getText(0, start);
                    start = 0;
                } else {
                    selection = getSelectedText();
                }
            } catch (BadLocationException e) {
                selection = "";
            }
        } else {
            selection = getSelectedText();
        }

        if (suppressCom) {
            StringBuffer buf = new StringBuffer(selection.length());
            int sstart = start;
            int tok = lexer.getKeyword(start, false);
            int len = selection.length();
            while (start < end) {
                int pos = lexer.start + lexer.yychar() + lexer.yylength();
                String str = selection.substring(start - sstart, Math.min(pos - sstart, len));
                if (tok != ScilabLexerConstants.COMMENT || str.equals("\n")) {
                    buf.append(str);
                }
                start = pos;
                tok = lexer.getKeyword(start, false);
            }

            return buf.toString();
        }

        return selection;
    }

    /**
     * Add a new KeywordListener
     * @param kw a KeywordListener
     */
    public void addKeywordListener(KeywordListener kw) {
        if (!kwListeners.contains(kw)) {
            kwListeners.add(kw);
        }
    }

    /**
     * Remove a new KeywordListener
     * @param kw a KeywordListener
     */
    public void removeKeywordListener(KeywordListener kw) {
        if (kwListeners.contains(kw)) {
            kwListeners.remove(kw);
        }
    }

    /**
     * @return an array of KeywordListener
     */
    public KeywordListener[] getKeywordListeners() {
        return kwListeners.toArray(new KeywordListener[0]);
    }

    /**
     * Set a new color for the highlighting
     * @param c the color, can be null (useful if setHighlightedContourColor is used with a
     * non-null value)
     */
    public void setHighlightedLineColor(Color c) {
        highlightColor = c;
    }

    /**
     * Set a new color for the contour of the highlighting
     * @param c the color, if null no contour is drawn
     */
    public void setHighlightedContourColor(Color c) {
        highlightContourColor = c;
    }

    /**
     * Activate or desactivate the help on typing
     */
    public void activateHelpOnTyping() {
        boolean isActive = HelpOnTypingManager.isActive();
        if (isActive && helpOnTyping == null) {
            helpOnTyping = HelpOnTypingManager.getInstance();
            addKeyListener(helpOnTyping);
        } else if (!isActive && helpOnTyping != null) {
            removeKeyListener(helpOnTyping);
            helpOnTyping = null;
        }
    }

    /**
     * Enable (active true) or disable (active false) the line-highlighting.
     * @param active true or false
     */
    public void enableHighlightedLine(boolean active) {
        if (active && !highlightEnable) {
            try {
                highlightCL = getHighlighter().addHighlight(0, 0, this);
            } catch (BadLocationException e) { }
            highlightEnable = true;
        }

        if (!active && highlightEnable) {
            getHighlighter().removeHighlight(highlightCL);
            highlightEnable = false;
        }

        repaint();
    }

    /**
     * Enable (active true) or disable (active false) the matching keywords.
     * @param active true or false
     */
    public void enableMatchingKeywords(boolean active) {
        matchingEnable = active;
    }

    /**
     * Get a matching manager
     * @param lr true if the LR matcher must be returned
     * @return the MatchingBlockManager
     */
    public MatchingBlockManager getMatchingBlockManager(boolean lr) {
        if (lr) {
            return matchLR;
        } else {
            return matchRL;
        }
    }

    /**
     * This class listens to the caret event
     * @param e event
     */
    public void caretUpdate(CaretEvent e) {
        if (hasBeenSaved) {
            removeHighlightForLine();
        }

        if (hasFocus()) {
            String str = getSelectedText();
            if (str != null && str.length() != 0) {
                highlightWords(str, true);
                removeHighlightOnPosition(getSelectionStart());
            } else {
                removeHighlightedWords();
            }
        }

        if (highlightEnable) {
            repaint();
        }

        int pos = getCaretPosition();

        if (matchingEnable) {
            int tok = lexer.getKeyword(pos, false);
            matchLR.searchMatchingBlock(tok, lexer.start + lexer.yychar());
            tok = lexer.getKeyword(pos, true);
            matchRL.searchMatchingBlock(tok, lexer.start + lexer.yychar() + lexer.yylength());
        }

        if (!readonly && !binary) {
            editor.getInfoBar().setText(((ScilabDocument) getDocument()).getCurrentFunction(pos));
        }
    }

    /**
     * Used to paint the highlighted line
     * @param g graphics to use
     * @param p0 start
     * @param p1 end
     * @param bounds the shape representing the area
     * @param c this pane
     */
    public void paint(Graphics g, int p0, int p1, Shape bounds, JTextComponent c) {
        if (highlightEnable) {
            try {
                Rectangle r = modelToView(getCaretPosition());
                if (highlightColor != null) {
                    g.setColor(highlightColor);
                    g.fillRect(0, r.y, getWidth(), r.height);
                }
                if (highlightContourColor != null) {
                    g.setColor(highlightContourColor);
                    g.drawLine(0, r.y - 1, getWidth(), r.y - 1);
                    g.drawLine(0, r.y + r.height, getWidth(), r.y + r.height);
                }

            } catch (BadLocationException e) { }
        }
    }

    /**
     * @return the current TrailingWhiteManager
     */
    public TrailingWhiteManager getTrailingWhiteManager() {
        return trailingWhite;
    }

    /**
     * @return the current IndentManager
     */
    public IndentManager getIndentManager() {
        return indent;
    }

    /**
     * @return the current TabManager
     */
    public TabManager getTabManager() {
        return tab;
    }

    /**
     * @return the current CommentManager
     */
    public CommentManager getCommentManager() {
        return com;
    }

    /**
     * Get a keyword at a position in the document.
     * @param position in the document
     * @return the KeywordEvent containing infos about keyword.
     */
    public KeywordEvent getKeywordEvent(int position) {
        int tok = lexer.getKeyword(position, true);
        return new KeywordEvent(this, null, tok, lexer.start + lexer.yychar(), lexer.yylength());
    }

    /**
     * Get a keyword at the current position in the document.
     * @return the KeywordEvent containing infos about keyword.
     */
    public KeywordEvent getKeywordEvent() {
        return getKeywordEvent(getCaretPosition());
    }

    /**
     * Get a keyword at the current position in the document.
     * @param caret if true the position is the current caret position in the doc else
     * the position is the mouse pointer position projected in the document.
     * @param strict if true the char just after the caret is ignored
     * @return the KeywordEvent containing infos about keyword.
     */
    public KeywordEvent getKeywordEvent(boolean caret, boolean strict) {
        int tok;
        if (caret) {
            tok = lexer.getKeyword(getCaretPosition(), strict);
        } else {
            tok = lexer.getKeyword(viewToModel(mousePoint), strict);
        }
        return new KeywordEvent(this, null, tok, lexer.start + lexer.yychar(), lexer.yylength());
    }

    /**
     * Get an helpable keyword at the current position in the document.
     * @param caret if true the position is the current caret position in the doc else
     * the position is the mouse pointer position projected in the document.
     * @return the helpable keyword.
     */
    public String getHelpableKeyword(boolean caret) {
        int tok;
        int pos;
        if (caret) {
            pos = getCaretPosition();
        } else {
            pos = viewToModel(mousePoint);
        }

        tok = lexer.getKeyword(pos, true);
        if (!ScilabLexerConstants.isHelpable(tok)) {
            tok = lexer.getKeyword(pos + 1, true);
        }

        if (ScilabLexerConstants.isHelpable(tok)) {
            try {
                return getDocument().getText(lexer.start + lexer.yychar(), lexer.yylength());
            } catch (BadLocationException e) { }
        }

        return null;
    }

    /**
     * Prevents the different KeywordListener that a MouseEvent occured
     * @param position of the mouse
     * @param ev the event which occured
     * @param type of the event : KeywordListener.ONMOUSECLICKED or KeywordListener.ONMOUSEOVER
     */
    protected void preventConcernedKeywordListener(int position, EventObject ev, int type) {
        int tok = lexer.getKeyword(position, true);
        KeywordEvent kev = new KeywordEvent(this, ev, tok, lexer.start + lexer.yychar(), lexer.yylength());
        for (KeywordListener listener : kwListeners) {
            if (type == listener.getType()) {
                listener.caughtKeyword(kev);
            }
        }
    }

    /**
     * Implements mouseClicked in MouseListener
     * @param e event
     */
    public void mouseClicked(MouseEvent e) {
        preventConcernedKeywordListener(getCaretPosition(), e, KeywordListener.ONMOUSECLICKED);
    }

    /**
     * Implements mouseEntered in MouseListener
     * @param e event
     */
    public void mouseEntered(MouseEvent e) {
        this.mousePoint = e.getPoint();
    }

    /**
     * Implements mouseExited in MouseListener
     * @param e event
     */
    public void mouseExited(MouseEvent e) { }

    /**
     * Implements mousePressed in MouseListener
     * @param e event
     */
    public void mousePressed(MouseEvent e) {
        if (hasBeenSaved) {
            removeHighlightForLine();
        }

        removeHighlightedWords();

        if (highlightEnable) {
            repaint();
        }
    }

    /**
     * Implements mouseReleseaed in MouseListener
     * @param e event
     */
    public void mouseReleased(MouseEvent e) { }

    /**
     * Implements mouseMoved in MouseMotionListener
     * @param e event
     */
    public void mouseMoved(MouseEvent e) {
        this.mousePoint = e.getPoint();
        preventConcernedKeywordListener(viewToModel(mousePoint), e, KeywordListener.ONMOUSEOVER);
    }

    /**
     * Implements mouseDragged in MouseMotionListener
     * @param e event
     */
    public void mouseDragged(MouseEvent e) {
        if (hasBeenSaved) {
            removeHighlightForLine();
        }

        removeHighlightedWords();

        if (highlightEnable) {
            repaint();
        }
    }

    /**
     * @return the current mouse poisition in this pane
     */
    public Point getMousePoint() {
        return mousePoint;
    }

    /**
     * @param pane the EditorPane associated with this EditorPane in a splitted view
     */
    public void setOtherPaneInSplit(ScilabEditorPane pane) {
        if (pane == null) {
            rightTextPane.rightTextPane = null;
        }
        rightTextPane = pane;
    }

    /**
     * @return the EditorPane associated with this EditorPane in a splitted view
     */
    public ScilabEditorPane getOtherPaneInSplit() {
        return rightTextPane;
    }

    /**
     * @return the scrollPane associated with this EditorPane
     */
    public JScrollPane getScrollPane() {
        return edComponent.getScrollPane();
    }

    /**
     * @return the current focused editorPane
     */
    public static ScilabEditorPane getFocusedPane() {
        return focused;
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        return shortName;
    }

    /**
     * @return true if something has been copied
     */
    public boolean copyColumnSelectionInClipBoard() {
        return ((SciNotesCaret) getCaret()).copyPositionsInClipboard();
    }

    /**
     * @return true if something has been removed
     */
    public boolean removeColumnSelection() {
        return ((SciNotesCaret) getCaret()).removePositions();
    }

    /**
     * {@inheritDoc}
     */
    public String getSelectedText() {
        String str = ((SciNotesCaret) getCaret()).getSelectedText();
        if (str == null) {
            return super.getSelectedText();
        } else {
            return str;
        }
    }

    /**
     * @param pos the position int the text
     * @return null if no column selection on the same line and an array of
     * integer (of size 2) containing the position of the selection
     */
    public int[] isNearColumnSelection(int pos) {
        if (!(getCaret() instanceof SciNotesCaret) || ((SciNotesCaret) getCaret()).isEmptySelection()) {
            return null;
        }

        Element root = getDocument().getDefaultRootElement();
        int[][] positions = ((SciNotesCaret) getCaret()).getSelectedPositions();
        int line = root.getElementIndex(pos);
        int min = root.getElementIndex(positions[0][0]);
        int max = root.getElementIndex(positions[positions.length - 1][0]);

        if (line >= min && line <= max) {
            return positions[line - min];
        }

        return null;
    }

    /**
     * {@inheritDoc}
     */
    public void setCaret(Caret c) {
        if (!(c instanceof ScilabCaret)) {
            final Caret caret = new SciNotesCaret(this);
            setCaretColor(getCaretColor());
            SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        int blinkRate = 500;
                        Object o = UIManager.get("TextComponent.caretBlinkRate");
                        if ((o != null) && (o instanceof Integer)) {
                            Integer rate = (Integer) o;
                            blinkRate = rate.intValue();
                        }
                        caret.setBlinkRate(blinkRate);
                        caret.setVisible(true);
                    }
                });
            super.setCaret(caret);
        } else {
            super.setCaret(c);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void select(int start, int end) {
        removeHighlightOnPosition(start);
        super.select(start, end);
    }

    /**
     * Remove the highlight putted to show the line (for editor('foo',123))
     */
    private void removeHighlightForLine() {
        highlightContourColor = saveHighlightContourColor;
        highlightColor = saveHighlightColor;
        enableHighlightedLine(false);
        if (saveHighlightEnable) {
            enableHighlightedLine(true);
        }
        hasBeenSaved = false;
    }

    /**
     * Remove the highlight at position start
     * @param start the beginning of the highlight
     */
    public void removeHighlightOnPosition(int start) {
        int pos = highlightedWordsBegin.indexOf(start);
        if (pos != -1) {
            getHighlighter().removeHighlight(highlightedWords.remove(pos));
            highlightedWordsBegin.remove(pos);
        }
    }

    /**
     * Highlight a word in this textpane.
     * @param word the word to highlight
     * @param exact if true the search is case sensitive
     */
    public void highlightWords(String word, boolean exact) {
        removeHighlightedWords();
        if (word != null && word.length() != 0) {
            String text = getText();
            if (!exact) {
                text = text.toLowerCase();
                word = word.toLowerCase();
            }

            int pos = text.indexOf(word, 0);
            int len = word.length();
            Highlighter highlighter = getHighlighter();
            while (pos != -1) {
                try {
                    highlightedWords.add(highlighter.addHighlight(pos, pos + len, HIGHLIGHTER));
                    highlightedWordsBegin.add(pos);
                } catch (BadLocationException e) { }
                pos = text.indexOf(word, pos + len);
            }
        }
    }

    /**
     * Highlight a word according to a pattern in this textpane.
     * @param pattern the pattern to highlight
     * @param centered, if true the pane is centered on the first occurence
     */
    public void highlightWords(Pattern pattern, boolean centered) {
        if (pattern != null) {
            removeHighlightedWords();
            int first = -1;
            String text = getText();
            Matcher matcher = pattern.matcher(text);

            Highlighter highlighter = getHighlighter();
            while (matcher.find()) {
                if (first == -1) {
                    first = matcher.start();
                }
                try {
                    highlightedWords.add(highlighter.addHighlight(matcher.start(), matcher.end(), HIGHLIGHTER));
                    highlightedWordsBegin.add(matcher.start());
                } catch (BadLocationException e) { }
            }

            if (centered && first != -1) {
                scrollTextToPos(first, false, true);
            }
        }
    }

    /**
     * Remove all the highlighted words
     */
    public void removeHighlightedWords() {
        Highlighter highlighter = getHighlighter();
        for (Object obj : highlightedWords) {
            highlighter.removeHighlight(obj);
        }
        highlightedWords.clear();
        highlightedWordsBegin.clear();
    }

    /**
     * Initialize the pane when the document is loaded
     * @param doc used with this pane
     */
    private void initialize(ScilabDocument doc) {
        indent = new IndentManager(doc);
        tab = new TabManager(doc, indent);
        tab.setDefaultTabulation();
        com = new CommentManager(doc);
        trailingWhite = new TrailingWhiteManager(doc);
        matchLR = new MatchingBlockManager(doc, this, true, getHighlighter());
        matchLR.setDefaults();
        matchRL = new MatchingBlockManager(doc, this, false, getHighlighter());
        matchRL.setDefaults();
        lexer = doc.createLexer();
        xln = new SciNotesLineNumberPanel(this);

        /* The order of the next two lines is important: the doc
           as listener will be called before xln so the resetTypeWhenBroken
           will be called before ! */
        doc.addDocumentListener(xln);
        doc.addDocumentListener(doc);

        edComponent.getScrollPane().setRowHeaderView(xln);
        doc.setEditorPane(this);
    }
}
