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

package org.scilab.modules.xpad;

import java.awt.Rectangle;
import java.awt.Shape;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Font;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;

import java.util.List;
import java.util.ArrayList;
import java.util.EventObject;

import javax.swing.JEditorPane;
import javax.swing.text.JTextComponent;
import javax.swing.text.Document;
import javax.swing.text.BadLocationException;
import javax.swing.event.CaretListener;
import javax.swing.event.CaretEvent;
import javax.swing.text.Highlighter;

/**
 * Class ScilabEditorPane
 * @author Calixte DENIZET
 *
 */
public class ScilabEditorPane extends JEditorPane implements Highlighter.HighlightPainter,
							     CaretListener, MouseListener,
							     MouseMotionListener {

    private Color highlightColor = new Color(228, 233, 244);
    private Color highlightContourColor = new Color(50, 50, 50);
    private boolean highlightEnable;
    private boolean matchingEnable;
    private ScilabLexer lexer;
    private Xpad editor;
    private IndentManager indent;
    private TabManager tab;
    private CommentManager com;
    
    /* matchLR matches Left to Right ... */
    private MatchingBlockManager matchLR;
    private MatchingBlockManager matchRL;

    private boolean suppressCom = true;

    private List<KeywordListener> kwListeners = new ArrayList();

    /**
     * Constructor
     * @param editor which uses this pane
     */
    public ScilabEditorPane(Xpad editor) {
	super();
	this.editor = editor;
	addCaretListener(this);
	addMouseMotionListener(this);
	addMouseListener(this);
	enableMatchingKeywords(true);
	setFocusable(true);
    }
    
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
     * Set a new font
     * @param font to set
     */
    public void resetFont(Font font) {
	setFont(font);
	((ScilabEditorKit) getEditorKit()).getStylePreferences().genFonts(font);
	editor.getXln().updateFont(font);
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
	kwListeners.add(kw);
    }
    
    /**
     * Remove a new KeywordListener
     * @param kw a KeywordListener
     */
    public void removeKeywordListener(KeywordListener kw) {
	kwListeners.remove(kw);
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
     * Enable (active true) or disable (active false) the line-highlighting.
     * @param active true or false
     */
    public void enableHighlightedLine(boolean active) {
	if (active && !highlightEnable) {
	    try {
		getHighlighter().addHighlight(0, 0, this);
	    } catch (BadLocationException e) { }
	    highlightEnable = true;
	}

	if (!active && highlightEnable) {
	    getHighlighter().removeAllHighlights();
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
     * Initialize the pane when the document is loaded
     * @param doc used with this pane
     */ 
    private void initialize(ScilabDocument doc) {
	indent = new IndentManager(doc);
	tab = new TabManager(doc, indent);
	tab.setDefaultTabulation();
	com = new CommentManager(doc);
	matchLR = new MatchingBlockManager(doc, true, getHighlighter(), Color.PINK);
	matchRL = new MatchingBlockManager(doc, false, getHighlighter(), Color.ORANGE);
	lexer = doc.createLexer();
    }
}
