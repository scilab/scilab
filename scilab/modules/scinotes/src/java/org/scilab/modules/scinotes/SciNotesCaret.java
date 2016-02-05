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

import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.Highlighter;
import javax.swing.text.NavigationFilter;
import javax.swing.text.Position;

import org.scilab.modules.commons.gui.ScilabCaret;

/**
 * SciNotesCaret Class
 * @author Calixte DENIZET
 */
public class SciNotesCaret extends ScilabCaret {

    private static final long serialVersionUID = 2406847391415867003L;

    private ScilabEditorPane pane;
    private SciNotesNavigationFilter filter;
    private NavigationFilter saveFilter;

    private List<Object> selections = new ArrayList<Object>();
    private int[][] positions;
    private boolean protect;

    /**
     * Constructor
     * @param pane the pane where the caret is displayed
     */
    public SciNotesCaret(ScilabEditorPane pane) {
        super(pane);
        this.pane = pane;
    }

    public void clean() {
        pane = null;
        filter = null;
        saveFilter = null;
        selections = null;
        positions = null;
    }

    /**
     * @param protect true to avoid the highlight removal
     */
    public void protectHighlights(boolean protect) {
        this.protect = protect;
    }

    /**
     * Update the highlights considering the new positions
     */
    public void updateHighlights() {
        Highlighter h = pane.getHighlighter();
        try {
            for (int i = 0; i < positions.length; i++) {
                Object obj = selections.get(i);
                if (obj != null) {
                    h.changeHighlight(obj, positions[i][0], positions[i][1]);
                }
            }
        } catch (BadLocationException e) {
            System.err.println(e);
        }
    }

    /**
     * @return the positions of the column selection or null
     */
    public int[][] getSelectedPositions() {
        return positions;
    }

    /**
     * @return the positions of the column selection or null
     */
    public List<Object> getSelections() {
        return selections;
    }

    /**
     * @return true if something has been removed
     */
    public boolean removePositions() {
        int[][] pos = positions;
        if (pos != null) {
            Document doc = pane.getDocument();
            try {
                int slen = 0;
                int len;
                for (int i = 0; i < pos.length; i++) {
                    len = pos[i][1] - pos[i][0];
                    doc.remove(pos[i][0] - slen, len);
                    slen += len;
                }
            } catch (BadLocationException e) {
                System.err.println(e);
            }
        }

        return pos != null;
    }

    /**
     * @return the selected text in column mode
     */
    public String getSelectedText() {
        if (positions != null) {
            try {
                StringBuffer buf = new StringBuffer();
                for (int i = 0; i < positions.length; i++) {
                    buf.append(pane.getText(positions[i][0], positions[i][1] - positions[i][0]));
                    if (i < positions.length - 1) {
                        buf.append("\n");
                    }
                }

                return buf.toString();
            } catch (BadLocationException e) {
                System.err.println(e);
            }
        }

        return null;
    }

    /**
     * @return true if the column selection is empty
     */
    public boolean isEmptySelection() {
        return positions == null;
    }

    /**
     * @return true if something has been copied in the clipboard
     */
    public boolean copyPositionsInClipboard() {
        if (positions != null) {
            StringSelection sel = new StringSelection(getSelectedText());
            Toolkit.getDefaultToolkit().getSystemClipboard().setContents(sel, sel);
        }

        return positions != null;
    }

    /**
     * {@inheritDoc}
     */
    public void mouseReleased(MouseEvent e) {
        if (filter != null) {
            filter = null;
            pane.setNavigationFilter(saveFilter);
        }
        super.mouseReleased(e);
    }

    /**
     * {@inheritDoc}
     */
    public void mouseDragged(MouseEvent e) {
        if (!e.isConsumed() && SwingUtilities.isLeftMouseButton(e)) {
            if (e.isControlDown()) {
                if (filter == null) {
                    filter = new SciNotesNavigationFilter();
                    saveFilter = pane.getNavigationFilter();
                    pane.setNavigationFilter(filter);
                }
            } else if (filter != null) {
                filter = null;
                pane.setNavigationFilter(saveFilter);
            }

            super.mouseDragged(e);
        }
    }

    /**
     * Remove the highlights
     */
    public void removeHighlights() {
        if (!protect) {
            Highlighter h = pane.getHighlighter();
            for (Object obj : selections) {
                if (obj != null) {
                    h.removeHighlight(obj);
                }
            }
            selections.clear();
            positions = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setDot(int dot, Position.Bias dotBias) {
        removeHighlights();
        super.setDot(dot, dotBias);
    }

    /**
     * Inner class to specify the portion of the text to select in column mode.
     */
    class SciNotesNavigationFilter extends NavigationFilter {

        private int mark = -1;
        private Element root = pane.getDocument().getDefaultRootElement();

        /**
         * Constructor
         */
        public SciNotesNavigationFilter() {
            super();
        }

        /**
         * {@inheritDoc}
         */
        public void moveDot(NavigationFilter.FilterBypass fb, int sdot, Position.Bias bias) {
            int dot = sdot;
            if (mark == -1) {
                mark = --dot;
            }

            pane.setCaretPosition(dot);

            if (isSelectionVisible()) {
                Highlighter h = pane.getHighlighter();
                if (h != null) {
                    int p0 = Math.min(dot, mark);
                    int p1 = Math.max(dot, mark);

                    if (!selections.isEmpty()) {
                        removeHighlights();
                    }

                    int startL = root.getElementIndex(p0);
                    int endL = root.getElementIndex(p1);
                    int sC = p0 - root.getElement(startL).getStartOffset();
                    int eC = p1 - root.getElement(endL).getStartOffset();
                    int startC = Math.min(sC, eC);
                    int endC = Math.max(sC, eC);

                    Highlighter.HighlightPainter p = getSelectionPainter();
                    int delta = endC - startC;

                    if (p0 != p1) {
                        positions = new int[endL - startL + 1][2];

                        try {
                            for (int i = startL; i <= endL; i++) {
                                Element elem = root.getElement(i);
                                int end = elem.getEndOffset();
                                int index = i - startL;
                                p1 = Math.min(p0 + delta, end);
                                positions[index][0] = Math.min(elem.getStartOffset() + startC, end);
                                positions[index][1] = Math.min(positions[index][0] + delta, end);
                                if (positions[index][0] < positions[index][1]) {
                                    selections.add(h.addHighlight(positions[index][0], positions[index][1], p));
                                } else {
                                    selections.add(null);
                                }
                            }
                        } catch (BadLocationException e) {
                            System.err.println(e);
                        }
                    }
                }
            }
        }
    }
}
