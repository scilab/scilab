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

package org.scilab.modules.scinotes;

import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Graphics;
import java.awt.Font;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.File;
import java.util.List;
import java.util.ArrayList;
import java.util.EventObject;

import javax.swing.JEditorPane;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JScrollBar;
import javax.swing.ScrollPaneConstants;
import javax.swing.JComponent;
import javax.swing.SwingUtilities;
import javax.swing.text.JTextComponent;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.View;
import javax.swing.text.BadLocationException;
import javax.swing.event.CaretListener;
import javax.swing.event.CaretEvent;
import javax.swing.text.Highlighter;

import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.scinotes.utils.NavigatorWindow;

/**
 * Class ScilabEditorPane
 * @author Calixte DENIZET
 *
 */
public class ScilabEditorPane extends JEditorPane implements Highlighter.HighlightPainter,
                                                             CaretListener, MouseListener,
                                                             MouseMotionListener, Cloneable,
                                                             KeyListener {

    private static ScilabEditorPane focused;

    private Color highlightColor;
    private Color highlightContourColor;
    private boolean highlightEnable;
    private Object highlightCL;
    private boolean matchingEnable;
    private ScilabLexer lexer;
    private SciNotes editor;
    private NavigatorWindow navigator;
    private IndentManager indent;
    private TabManager tab;
    private CommentManager com;
    private HelpOnTypingManager helpOnTyping;
    private TrailingWhiteManager trailingWhite;
    private boolean readonly;
    private String infoBar = "";

    private long lastModified;

    /* matchLR matches Left to Right ... */
    private MatchingBlockManager matchLR;
    private MatchingBlockManager matchRL;

    private boolean suppressCom = true;

    private SciNotesLineNumberPanel xln;
    private JScrollPane scroll;
    private JSplitPane split;
    private ScilabEditorPane rightTextPane;

    private static final Cursor handCursor = Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
    private static final Cursor textCursor = Cursor.getPredefinedCursor(Cursor.TEXT_CURSOR);
    private boolean hand;
    private boolean infoBarChanged = false;
    private boolean ctrlHit;

    private List<KeywordListener> kwListeners = new ArrayList();

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
        scroll = new JScrollPane(this);
        scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
        scroll.addComponentListener(new ComponentAdapter() {
                public void componentResized(ComponentEvent e) {
                    setSize(scroll.getViewport().getSize());
                    validate();
                    SwingUtilities.invokeLater(new Runnable() {
                            public void run() {
                                scrollTextToPos(getCaretPosition());
                            }
                        });
                }
            });

        addCaretListener(this);
        addMouseMotionListener(this);
        addMouseListener(this);
        enableMatchingKeywords(true);
        setFocusable(true);
        addFocusListener(new FocusListener() {
                public void focusGained(FocusEvent e) {
                    updateInfosWhenFocused();
                }

                public void focusLost(FocusEvent e) {
                    ((ScilabDocument) getDocument()).setFocused(false);
                }
            });

        addKeywordListener(new KeywordAdaptater.MouseOverAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    if (ScilabLexerConstants.URL == e.getType() || ScilabLexerConstants.MAIL == e.getType()) {
                        if (ctrlHit) {
                            setCursor(handCursor);
                            hand = true;
                        } else {
                            ScilabEditorPane.this.editor.getInfoBar().setText(SciNotesMessages.CLICKABLE_URL);
                            infoBarChanged = true;
                        }
                    } else {
                        if (hand) {
                            setCursor(textCursor);
                            hand = false;
                        }
                        if (infoBarChanged) {
                            ScilabEditorPane.this.editor.getInfoBar().setText(infoBar);
                            infoBarChanged = false;
                        }
                    }
                }
            });

        addKeywordListener(new KeywordAdaptater.MouseClickedAdaptater() {
                public void caughtKeyword(KeywordEvent e) {
                    if (ctrlHit && (ScilabLexerConstants.URL == e.getType() || ScilabLexerConstants.MAIL == e.getType())) {
                        try {
                            hand = false;
                            ctrlHit = false;
                            infoBarChanged = false;
                            setCursor(textCursor);
                            ScilabEditorPane.this.editor.getInfoBar().setText(infoBar);
                            String url = ((ScilabDocument) getDocument()).getText(e.getStart(), e.getLength());
                            WebBrowser.openUrl(url);
                        } catch (BadLocationException ex) { }
                    }
                }
            });

        addKeyListener(this);
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
     * @param navigator the NavigatorWindow associated with this pane
     */
    public void setNavigator(NavigatorWindow navigator) {
        this.navigator = navigator;
    }

    /**
     * @return true if this pane has a code navigator
     */
    public boolean hasNavigator() {
        return navigator != null;
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
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    if (navigator != null) {
                        navigator.closeNavigator();
                    }
                }
            });
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
            if (f != null) {
                return lastModified < f.lastModified();
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
        pane.setEditable(isEditable());
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
    public JComponent getParentComponent() {
        if (split == null) {
            return scroll;
        }
        return split;
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
        try {
            setCaretPosition(pos);
            JScrollBar scrollbar = scroll.getVerticalScrollBar();
            int value = modelToView(pos).y;
            if (modelToView(pos).y > scrollbar.getMaximum()) {
                scrollbar.setMaximum(value);
            }
            scrollbar.setValue(value);
        } catch (BadLocationException e) { }
    }

    /**
     * Scroll the pane to have the line lineNumber on the top of the pane
     * @param lineNumber the number of the line
     */
    public void scrollTextToLineNumber(int lineNumber) {
        Element root = getDocument().getDefaultRootElement();
        if (lineNumber >= 1 && lineNumber <= root.getElementCount()) {
            int pos = root.getElement(lineNumber - 1).getStartOffset();
            setCaretPosition(pos);
        }
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
        int start = getSelectionStart();
        int end = getSelectionEnd();
        try {
            if (start == end) {
                selection = getDocument().getText(0, start + 1);
            } else {
                selection = getDocument().getText(start, end - start + 1);
            }
            if (suppressCom) {
                selection = selection.replaceAll("[ \t]*//[^\n]*\n", "");
            }
        } catch (BadLocationException e) {
            selection = "";
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
     * @param isActive a boolean, true to activate.
     */
    public void activateHelpOnTyping(boolean isActive) {
        if (isActive && helpOnTyping == null) {
            helpOnTyping = new HelpOnTypingManager(this);
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
        if (highlightEnable) {
            repaint();
        }

        if (matchingEnable) {
            int pos = getCaretPosition();
            int tok = lexer.getKeyword(pos, false);
            matchLR.searchMatchingBlock(tok, lexer.start + lexer.yychar());
            tok = lexer.getKeyword(pos, true);
            matchRL.searchMatchingBlock(tok, lexer.start + lexer.yychar() + lexer.yylength());
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
    public void mouseEntered(MouseEvent e) { }

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
        preventConcernedKeywordListener(viewToModel(e.getPoint()), e, KeywordListener.ONMOUSEOVER);
    }

    /**
     * Implements mouseDragged in MouseMotionListener
     * @param e event
     */
    public void mouseDragged(MouseEvent e) {
        if (highlightEnable) {
            repaint();
        }
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
        return scroll;
    }

    /**
     * @return the current focused editorPane
     */
    public static ScilabEditorPane getFocusedPane() {
        return focused;
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
        scroll.setRowHeaderView(xln);
        doc.setEditorPane(this);
    }
}
