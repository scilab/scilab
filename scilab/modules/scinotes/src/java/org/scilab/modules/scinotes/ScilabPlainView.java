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

package org.scilab.modules.scinotes;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Color;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.font.FontRenderContext;
import java.awt.font.TextLayout;
import java.awt.Shape;
import java.awt.Rectangle;
import java.awt.geom.Rectangle2D;
import java.awt.Toolkit;
import java.awt.Component;
import java.util.Map;

import java.io.IOException;

import javax.swing.text.Utilities;
import javax.swing.text.PlainView;
import javax.swing.text.Segment;
import javax.swing.text.Element;
import javax.swing.text.BadLocationException;

import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;

/**
 * This important class is mainly used to render a document in using a PlainView (no line-wrapping)
 * @author Calixte DENIZET
 */
public class ScilabPlainView extends PlainView {

    private static final String DESKTOPHINTS = "awt.font.desktophints";

    private ScilabContext context;
    private ScilabLexer lexer;
    private ScilabDocument doc;
    private Segment text = new Segment();
    private boolean isTabViewable = true;
    private boolean isWhiteViewable = true;
    private boolean enable = SciNotesOptions.getSciNotesDisplay().keywordsColorization;

    private int tabType;
    private String tabCharacter = " ";
    private int tabLength = 4;

    private int numOfColumns = 80;
    private Color lineColor = new Color(220, 220, 220);

    private final Rectangle rect = new Rectangle();
    private Map desktopFontHints;
    private boolean enableDesktopFontHints = true;

    private int whiteHeight;
    private int whiteWidth;

    private boolean unselected = true;

    /**
     * The constructor to set this view for an element with a context (containing infos
     * such as colors or fonts of the keywords).
     * @param elem the element to view
     * @param context used to view the element
     */
    ScilabPlainView(Element elem, ScilabContext context) {
        super(elem);
        this.context = context;
        doc = (ScilabDocument) getDocument();
        if (doc.getBinary()) {
            disable();
        }
        doc.setView(this);
        lexer = doc.createLexer();
        setTabRepresentation(ScilabView.TABVERTICAL);
    }

    /**
     * A tabulation can be drawn with a mark
     * @param b true if viewable or not
     */
    public void setTabViewable(boolean b) {
        isTabViewable = b;
    }

    /**
     * A white can be drawn with a mark
     * @param b true if viewable or not
     */
    public void setWhiteViewable(boolean b) {
        isWhiteViewable = b;
    }

    /**
     * Disable this view
     */
    public void disable() {
        enable = false;
    }

    /**
     * Enable this view
     */
    public void enable() {
        enable = true;
    }

    /**
     * Enable this view
     */
    public void enable(boolean b) {
        enable = b;
    }

    /**
     * If n > 0, then a line will be drawn to see the maximum of chars recommanded in a line
     * (80 by default).
     * @param n the maximum of column recommanded in this view
     */
    public void setMaxColumns(int n) {
        numOfColumns = n;
    }

    /**
     * @return the width of a white
     */
    public int getWhiteWidth() {
        return whiteWidth;
    }

    /**
     * This method can be used to draw anything you want in the editor (such as
     * the line of maximum recommanded chars).
     * @param g the graphics where to draw
     * @param a the shape bounding the visible area
     * @overload paint method in WrappedPlainView
     */
    public void paint(Graphics g, Shape a) {
        if (numOfColumns > 0) {
            g.setColor(lineColor);
            Component c = getContainer();
            g.drawLine(numOfColumns * whiteWidth, 0, numOfColumns * whiteWidth, c.getHeight());
        }
        super.paint(g, a);
    }

    /**
     * A trick to easily determine the y-coordinate of the line n (useful in SciNotesLineNumberPanel)
     * @param n the line number
     * @return the y-coordinate of the line
     */
    public int getLineAllocation(int n) {
        rect.setLocation(0, 4); // Why 4 ?? Because it works with 4 !
        try {
            return lineToRect(rect, n).y;
        } catch (ArrayIndexOutOfBoundsException e) {
            return 0;
        }
    }

    /**
     * Used when the font is changed in the pane
     */
    public void reinitialize() {
        desktopFontHints = null;
        enableDesktopFontHints = true;
    }

    /**
     * Very important method since we draw the text in this method !!
     * @param g the graphics where to draw
     * @param sx the x-coordinate where to draw
     * @param sy the y-coordinate ... (guess the end pf the sentence)
     * @param p0 the start of the text in the doc
     * @param p1 the end of the text in the doc
     * @return the x-coordinate where to draw the next piece of text
     * @throws BadLocationException if p0 and p1 are bad positions in the text
     */
    protected int drawUnselectedText(Graphics g, int sx, int sy, int p0, int p1) throws BadLocationException {
        if (!enable) {
            return super.drawUnselectedText(g, sx, sy, p0, p1);
        }

        if (enableDesktopFontHints && desktopFontHints == null) {
            /* This hint is used to have antialiased fonts in the view in using
               the same method (differents way to antialias with LCD screen) as the desktop. */
            desktopFontHints = (Map) Toolkit.getDefaultToolkit().getDesktopProperty(DESKTOPHINTS);
            calculateHeight(((Graphics2D) g).getFontRenderContext(), context.tokenFonts[0]);
            enableDesktopFontHints = desktopFontHints != null;
        }

        if (enableDesktopFontHints) {
            ((Graphics2D) g).addRenderingHints(desktopFontHints);
        }

        /* The lexer returns all tokens between the pos p0 and p1.
           The value of the returned token determinates the color and the font.
           The lines can be broken by the Pane so we must look at previous
           and next chars to know if p0 or p1 is "inside" a token. */

        Element elem = doc.getDefaultRootElement();
        Element line = elem.getElement(elem.getElementIndex(p0));

        int prevTok = -1;
        int tok = -1;
        int mark = p0;
        int start = p0;
        int x = sx;
        int y = sy;
        boolean isBroken = false;

        int startL = line.getStartOffset();
        int endL = line.getEndOffset();

        if (startL != start) {
            //we are drawing a broken line
            try {
                lexer.setRange(startL, endL);
                while (startL < start) {
                    tok = lexer.scan();
                    startL = lexer.start + lexer.yychar() + lexer.yylength();
                }
                isBroken = true;
            } catch (IOException e) { }
        }

        if (!isBroken) {
            lexer.setRange(start, endL);
        }

        while (start < p1 && tok != ScilabLexerConstants.EOF) {

            try {
                if (!isBroken) {
                    tok = lexer.scan();
                } else {
                    isBroken = false;
                }
            } catch (IOException e) { }

            start = lexer.start + lexer.yychar();

            int end = Math.min(p1, start + lexer.yylength());

            if (end != mark) {
                if (tok != prevTok) {
                    if (unselected) {
                        g.setColor(context.tokenColors[tok]);
                    } else {
                        g.setColor(Color.WHITE);
                    }
                    g.setFont(context.tokenFonts[tok]);
                    prevTok = tok;
                }

                doc.getText(mark, end - mark, text);

                int w;

                if ((context.tokenAttrib[tok] & 1) != 0) {
                    w = Utilities.getTabbedTextWidth(text, g.getFontMetrics(), x, this, mark);
                    g.drawLine(x, y + 1, x + w, y + 1);
                }

                if ((context.tokenAttrib[tok] & 2) != 0) {
                    w = Utilities.getTabbedTextWidth(text, g.getFontMetrics(), x, this, mark);
                    g.drawLine(x, y - whiteHeight, x + w, y - whiteHeight);
                }

                switch (tok) {
                    case ScilabLexerConstants.WHITE :
                    case ScilabLexerConstants.WHITE_COMMENT :
                    case ScilabLexerConstants.WHITE_STRING :
                        if (isWhiteViewable) {
                            w = Utilities.getTabbedTextWidth(text, g.getFontMetrics(), x, this, mark);
                            g.drawLine(x + (w - 1) / 2, y - whiteHeight, x + (w + 1) / 2, y - whiteHeight);
                        }
                        break;
                    case ScilabLexerConstants.TAB :
                    case ScilabLexerConstants.TAB_COMMENT :
                    case ScilabLexerConstants.TAB_STRING :
                        if (isTabViewable) {
                            paintTab(text, x, y, g, mark);
                        }
                        break;
                    case ScilabLexerConstants.ERROR :
                        if (unselected) {
                            g.setColor(Color.RED);
                        } else {
                            g.setColor(Color.WHITE);
                        }
                        w = Utilities.getTabbedTextWidth(text, g.getFontMetrics(), x, this, mark);
                        for (int i = 0; i < w; i += 4) {
                            g.drawLine(x + i, y + 2, x + i + 1, y + 2);
                        }
                        for (int i = 2; i < w; i += 4) {
                            g.drawLine(x + i, y + 1, x + i + 1, y + 1);
                        }
                        break;
                    default :
                        break;
                }

                x = Utilities.drawTabbedText(text, x, y, g, this, mark);
                mark = end;
            }

            start = end;
        }

        return x;
    }

    /**
     * {@inheritDoc}
     */
    protected void drawLine(int lineIndex, Graphics g, int x, int y) {
        Element elem = doc.getDefaultRootElement().getElement(lineIndex);
        int p0 = elem.getStartOffset();
        int p1 = elem.getEndOffset();
        ScilabEditorPane pane = (ScilabEditorPane) getContainer();
        int sel0 = pane.getSelectionStart();
        int sel1 = pane.getSelectionEnd();
        int[] selC = pane.isNearColumnSelection(p0);

        try {
            if (sel0 == sel1) {
                if (selC == null) {
                    drawUnselectedText(g, x, y, p0, p1);
                    return;
                }
                sel0 = selC[0];
                sel1 = selC[1];
                if (sel0 == sel1) {
                    drawUnselectedText(g, x, y, p0, p1);
                    return;
                }
            }

            if ((p0 >= sel0 && p0 <= sel1) && (p1 >= sel0 && p1 <= sel1)) {
                drawSelectedText(g, x, y, p0, p1);
            } else if (sel0 >= p0 && sel0 <= p1) {
                if (sel1 >= p0 && sel1 <= p1) {
                    x = drawUnselectedText(g, x, y, p0, sel0);
                    x = drawSelectedText(g, x, y, sel0, sel1);
                    drawUnselectedText(g, x, y, sel1, p1);
                } else {
                    x = drawUnselectedText(g, x, y, p0, sel0);
                    drawSelectedText(g, x, y, sel0, p1);
                }
            } else if (sel1 >= p0 && sel1 <= p1) {
                x = drawSelectedText(g, x, y, p0, sel1);
                drawUnselectedText(g, x, y, sel1, p1);
            } else {
                drawUnselectedText(g, x, y, p0, p1);
            }
        } catch (BadLocationException e) { }
    }

    /**
     * Draw the selected text.
     * @param g the graphics where to draw
     * @param sx the x-coordinate where to draw
     * @param sy the y-coordinate ... (guess the end pf the sentence)
     * @param p0 the start of the text in the doc
     * @param p1 the end of the text in the doc
     * @return the x-coordinate where to draw the next piece of text
     * @throws BadLocationException if p0 and p1 are bad positions in the text
     */
    protected int drawSelectedText(Graphics g, int x, int y, int p0, int p1) throws BadLocationException {
        unselected = false;
        int z = drawUnselectedText(g, x, y, p0, p1);
        unselected = true;
        return z;
    }

    /**
     * Used to give the way to represent a tabulation. By default TABVERTICAL is used.
     * @param type must be TABVERTICAL or TABDOUBLECHEVRONS or TABHORIZONTAL
     * If a bad value is given, then nothing will be drawn
     */
    public void setTabRepresentation(int type) {
        this.tabType = type;
    }

    /**
     * Used to represent a tabulation with the given character ('|' or '#' or...)
     * @param rep the char representing a tab
     */
    public void setTabRepresentation(char rep) {
        setTabRepresentation(ScilabView.TABCHARACTER);
        this.tabCharacter = Character.toString(rep);
    }

    /**
     * Used to represent a tabulation
     * @param tabulation a Tabulation
     */
    public void setTabRepresentation(TabManager.Tabulation tabulation) {
        if (tabulation.type == ScilabView.TABCHARACTER) {
            setTabRepresentation(tabulation.rep);
        } else {
            setTabRepresentation(tabulation.type);
        }
        tabLength = tabulation.number;
    }

    /**
     * Used to represent the default tabulation got with ConfigSciNotesManager
     */
    public void setDefaultTabRepresentation() {
        setTabRepresentation(new TabManager.Tabulation());
    }

    /**
     * Method to paint a tabulation according to the setTabRepresentation.
     * @param text the segment of text representing a tabulation
     * @param x the x-coordinate where to draw
     * @param y the y-coordinate where to draw
     * @param g the graphics ... (yeah ! once again)
     * @param start the position in the document
     */
    protected void paintTab(Segment text, int x, int y, Graphics g, int start) {
        FontMetrics fm = g.getFontMetrics();
        int w = Utilities.getTabbedTextWidth(text, fm, x, this, start);
        switch (tabType) {
            case ScilabView.TABVERTICAL :
                g.drawLine(x, y + 4, x, y + 4 - fm.getHeight());
                break;
            case ScilabView.TABDOUBLECHEVRONS :
                g.drawString("\u00BB", x, y);
                break;
            case ScilabView.TABHORIZONTAL :
                g.drawLine(x, y - whiteHeight, x + w - 1, y - whiteHeight);
                break;
            case ScilabView.TABCHARACTER :
                g.drawString(tabCharacter, x, y);
                break;
            default :
        }
    }

    /**
     * Determinates the height of a '+' to have the vertical shift
     * to draw a line which strokes the text or to draw the mark
     * let by a white.
     * @param frc a font context
     * @param f the font where to take the '+'
     */
    private void calculateHeight(FontRenderContext frc, Font f) {
        TextLayout layout = new TextLayout("+", f, frc);
        Rectangle2D rectangle = layout.getBounds();
        whiteHeight = (int) Math.round(-rectangle.getY() / 2);
        layout = new TextLayout("w", f, frc);
        rectangle = layout.getBounds();
        whiteWidth = (int) Math.round(rectangle.getWidth());
    }
}
