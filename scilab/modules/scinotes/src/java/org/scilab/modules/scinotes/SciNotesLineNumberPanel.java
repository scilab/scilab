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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Toolkit;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseEvent;
import java.util.Map;
import java.util.Stack;

import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.MatteBorder;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Element;
import javax.swing.text.View;

import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * This class will display line numbers for a related text component. The text
 * component must use the same line height for each line. TextLineNumber
 * supports wrapped lines and will highlight the line number of the current
 * line in the text component.
 *
 * This class was designed to be used as a component added to the row header
 * of a JScrollPane.
 * @author Calixte DENIZET
 */
public class SciNotesLineNumberPanel extends JPanel implements CaretListener, DocumentListener, MouseMotionListener {

    private static final long serialVersionUID = -5302189665896954438L;
    private static final int PANELGAPSIZE = 10;
    private static final Border OUTER = new MatteBorder(0, 0, 0, 2, Color.GRAY);
    private static final int HEIGHT = Integer.MAX_VALUE - 1000000;
    private static Map desktopFontHints;

    private ScilabEditorPane textPane;

    private Color currentLineForeground;
    private Color foreground = Color.BLACK;
    private Color anchorColor = new Color(250, 251, 164);
    private Color alternColor1 = new Color(246, 191, 246);
    private Color alternColor2 = new Color(246, 101, 246);

    private int numbers = 1;
    private int lastLine;
    private int state;

    private ScilabDocument doc;
    private FontMetrics metrics;
    private int ascent;
    private int availableWidth;

    private int[] lineNumber;
    private byte[] lineLevel;
    private boolean whereami;
    private boolean display;

    /**
     *  Create a line number component for a text component.
     *  @param textPane the related text component
     */
    public SciNotesLineNumberPanel(ScilabEditorPane textPane) {
        this.textPane = textPane;
        this.doc = (ScilabDocument) textPane.getDocument();
        setOpaque(true);
        updateFont(textPane.getFont());
        setBorderGap(PANELGAPSIZE);
        setCurrentLineForeground(Color.RED);
        setAlternColors(ConfigSciNotesManager.getAlternColors());
        textPane.addCaretListener(this);
        addMouseMotionListener(this);
    }

    /**
     * Set a line numbering compatible with the whereami function
     * @param state 0 for nothing, 1 for normal and 2 for whereami
     */
    public void setWhereamiLineNumbering(int state) {
        if (state != this.state) {
            if (state != 0) {
                if (!display) {
                    textPane.getScrollPane().setRowHeaderView(this);
                }
                whereami = state == 2;
                display = true;
            } else {
                textPane.getScrollPane().setRowHeaderView(null);
                display = false;
            }
            updateLineNumber();
            this.state = state;
        }
    }

    public static int getState(boolean showLinesNumber, boolean whereami) {
        if (!showLinesNumber) {
            return 0;
        } else if (!whereami) {
            return 1;
        } else {
            return 2;
        }
    }

    public static boolean[] getState(int state) {
        switch (state) {
            case 0:
                return new boolean[] { false, false };
            case 1:
                return new boolean[] { true, false };
            case 2:
                return new boolean[] { true, true };
            default:
                return null;
        }
    }

    /**
     * Set a line numbering compatible with the whereami function
     * @param state 0 for nothing, 1 for normal and 2 for whereami
     */
    public void setWhereamiLineNumbering(boolean showLinesNumber, boolean whereami) {
        setWhereamiLineNumbering(getState(showLinesNumber, whereami));
    }

    /**
     * Get the number of the lines used in the panel
     * @return the lines number depending on whereami mode or none
     */
    public int[] getLineNumber() {
        int[] ret = null;
        if (display) {
            if (whereami) {
                updateLineNumber();
                ret = lineNumber;
            } else {
                ret = new int[doc.getDefaultRootElement().getElementCount()];
                for (int i = 0; i < ret.length; i++) {
                    ret[i] = i + 1;
                }
            }
        }

        return ret;
    }

    /**
     * @return the current state
     */
    public int getState() {
        return state;
    }

    /**
     * @return true if whereami-compatible
     */
    public boolean getWhereamiLineNumbering() {
        return whereami;
    }

    /**
     * @param colors an array of size 2 containing the two alternative colors for inner function
     */
    public void setAlternColors(Color[] colors) {
        if (colors[0] == null) {
            this.alternColor1 = getBackground();
        } else {
            this.alternColor1 = colors[0];
        }
        if (colors[1] == null) {
            this.alternColor2 = getBackground();
        } else {
            this.alternColor2 = colors[1];
        }
    }

    /**
     * Update the font used in this component
     * @param font the font to use
     */
    public void updateFont(Font font) {
        setFont(font);
        metrics = textPane.getFontMetrics(font);
        ascent = metrics.getAscent();
        updateWidth(false);
    }

    /**
     *  Gets the current line rendering Color
     *
     *  @return the Color used to render the current line number
     */
    public Color getCurrentLineForeground() {
        if (currentLineForeground == null) {
            return getForeground();
        } else {
            return currentLineForeground;
        }
    }

    /**
     *  The Color used to render the current line numbers. Default is Coolor.RED.
     *
     *  @param currentLineForeground  the Color used to render the current line
     */
    public void setCurrentLineForeground(Color currentLineForeground) {
        this.currentLineForeground = currentLineForeground;
    }

    /**
     * Update the width of this component in using the number of digits used
     */
    public void updateWidth(boolean inc) {
        if (inc) {
            ++numbers;
        }
        Insets insets = getInsets();
        int width = metrics.charWidth('0') * numbers;
        availableWidth = width;
        Dimension d = getPreferredSize();
        d.setSize(availableWidth, HEIGHT);
        availableWidth = width;
        setPreferredSize(d);
        setSize(d);
    }

    /**
     * Draw the line numbers
     * @param g the graphics where to paint
     */
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        if (desktopFontHints == null) {
            desktopFontHints = (Map) (Toolkit.getDefaultToolkit().getDesktopProperty("awt.font.desktophints"));
        } else {
            ((Graphics2D) g).addRenderingHints(desktopFontHints);
        }

        synchronized (doc) {
            Element root = doc.getDefaultRootElement();
            View view = doc.getView();
            Rectangle clip = g.getClipBounds();
            Point pt = new Point(0, clip.y);
            int rowStartOffset = textPane.viewToModel(pt);
            pt.y += clip.height;
            int endOffset = textPane.viewToModel(pt);
            int lineEnd = root.getElementIndex(endOffset);
            boolean colorChanged;

            for (int line = root.getElementIndex(rowStartOffset); line <= lineEnd; line++) {
                String str;
                colorChanged = false;
                if (whereami && lineNumber != null) {
                    if (lineNumber[line] != -1) {
                        str = Integer.toString(lineNumber[line]);
                        if ((lineLevel[line] % 3) == 1) {
                            g.setColor(alternColor1);
                            colorChanged = true;
                        } else if ((lineLevel[line] % 3) == 2) {
                            g.setColor(alternColor2);
                            colorChanged = true;
                        }
                    } else {
                        str = "";
                    }
                } else {
                    str = Integer.toString(line + 1);
                }

                Element elem = root.getElement(line);
                if (((ScilabDocument.ScilabLeafElement) elem).isAnchor()) {
                    g.setColor(anchorColor);
                    colorChanged = true;
                }

                if (colorChanged) {
                    if (view instanceof ScilabView) {
                        g.fillRect(0, ((ScilabView) view).getLineAllocation(line), availableWidth, metrics.getHeight());
                    } else {
                        g.fillRect(0, ((ScilabPlainView) view).getLineAllocation(line), availableWidth, metrics.getHeight());
                    }
                }

                if (line != lastLine) {
                    g.setColor(foreground);
                } else {
                    g.setColor(currentLineForeground);
                }

                int diff = (availableWidth - metrics.stringWidth(str)) / 2;
                if (diff <= 0) {
                    updateWidth(true);
                    diff = (availableWidth - metrics.stringWidth(str)) / 2;
                }

                if (view instanceof ScilabView) {
                    g.drawString(str, diff, ((ScilabView) view).getLineAllocation(line) + ascent);
                } else {
                    g.drawString(str, diff, ((ScilabPlainView) view).getLineAllocation(line) + ascent);
                }
            }
        }
    }

    /**
     * Update this component if the caret changed of line
     * @param e the event
     */
    public void caretUpdate(CaretEvent e) {
        Element root = doc.getDefaultRootElement();
        int currentLine = root.getElementIndex(textPane.getCaretPosition());

        if (lastLine != currentLine) {
            lastLine = currentLine;
            repaint();
        }
    }

    /**
     * Useful method to determinate the number of the lines in being compatible
     * with the whereami function
     */
    private void updateLineNumber() {
        synchronized (doc) {
            Stack<Integer> stk = new Stack<Integer>();
            Element root = doc.getDefaultRootElement();
            int nlines = root.getElementCount();
            lineNumber = new int[nlines + 1];
            lineLevel = new byte[nlines + 1];
            lineNumber[0] = 1;
            int current = 1;
            boolean prevBroken = false;
            ScilabDocument.ScilabLeafElement elem;
            for (int i = 0; i < nlines; i++) {
                elem = (ScilabDocument.ScilabLeafElement) root.getElement(i);
                int type = elem.getType();
                switch (type) {
                    case ScilabDocument.ScilabLeafElement.NOTHING :
                        if (prevBroken) {
                            lineNumber[i] = -1;
                            if (!elem.isBroken()) {
                                prevBroken = false;
                            }
                        } else {
                            lineNumber[i] = current++;
                        }
                        lineLevel[i] = (byte) stk.size();
                        break;
                    case ScilabDocument.ScilabLeafElement.FUN :
                        stk.push(new Integer(current));
                        lineLevel[i] = (byte) stk.size();
                        current = 2;
                        lineNumber[i] = 1;
                        if (elem.isBroken()) {
                            prevBroken = true;
                        }
                        break;
                    case ScilabDocument.ScilabLeafElement.ENDFUN :
                        lineNumber[i] = current++;
                        lineLevel[i] = (byte) stk.size();
                        if (!stk.empty()) {
                            current = stk.pop().intValue() + lineNumber[i];
                        }
                        break;
                    default :
                        break;
                }
            }
        }
    }

    /**
     * Implements mouseMoved in MouseMotionListener
     * @param e event
     */
    public void mouseMoved(MouseEvent e) {
        int pos = textPane.viewToModel(e.getPoint());
        Element root = doc.getDefaultRootElement();
        ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) root.getElement(root.getElementIndex(pos));
        if (line.isAnchor()) {
            setToolTipText(SciNotesMessages.ANCHOR + line.toString());
        } else {
            setToolTipText(null);
        }
    }

    /**
     * Nothing !
     * @param e event
     */
    public void mouseDragged(MouseEvent e) { }

    /**
     * Nothing !
     * @param e the event
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * Called when an insertion is made in the doc
     * @param e the event
     */
    public void insertUpdate(DocumentEvent e) {
        handleEvent(e);
    }

    /**
     * Called when a remove is made in the doc
     * @param e the event
     */
    public void removeUpdate(DocumentEvent e) {
        handleEvent(e);
    }

    /**
     * Update the line numbering on a change in the document
     * @param e the document event
     */
    private void handleEvent(DocumentEvent e) {
        if (whereami) {
            Element root = doc.getDefaultRootElement();
            DocumentEvent.ElementChange chg = e.getChange(root);
            if (chg == null) {
                // change occurred only in one line
                ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) root.getElement(root.getElementIndex(e.getOffset()));
                if (line.needLineNumberRepaint()) {
                    updateLineNumber();
                    repaint();
                }
            } else {
                updateLineNumber();
            }
        }
    }

    /**
     *  The border gap is used in calculating the left and right insets of the
     *  border. Default value is 5.
     *  @param borderGap  the gap in pixels
     */
    private void setBorderGap(int borderGap) {
        Border inner = new EmptyBorder(0, borderGap, 0, borderGap);
        setBorder(new CompoundBorder(OUTER, inner));
    }
}
