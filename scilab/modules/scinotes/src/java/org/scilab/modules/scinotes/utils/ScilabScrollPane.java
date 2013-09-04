/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.scinotes.utils;

import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.lang.reflect.Field;
import java.util.List;

import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.plaf.ScrollBarUI;
import javax.swing.plaf.basic.BasicScrollBarUI;

/**
 * A ScrollPane where the vertical scrollbar can contains marks to indicate particular positions in the document.
 * @author Calixte DENIZET
 */
@SuppressWarnings(value = { "serial" })
public class ScilabScrollPane extends JScrollPane {

    private static final Color MARKCOLOR = new Color(229, 192, 0);
    private List<Rectangle> marks;

    /**
     * Default constructor
     * @param view the view to put in the scrollpane
     */
    public ScilabScrollPane(Component view) {
        super(view);
    }

    /**
     * {@inheritDoc}
     */
    public JScrollBar createVerticalScrollBar() {
        return new ScilabVerticalScrollBar();
    }

    /**
     * Put marks in the vertical scrollbar
     * @param marks a list of the rectangle corresponding to the line in the document
     */
    public void putMarks(List<Rectangle> marks) {
        this.marks = marks;
        getVerticalScrollBar().repaint();
    }

    /**
     * Remove all the marks
     */
    public void removeMarks() {
        this.marks = null;
        getVerticalScrollBar().repaint();
    }

    /**
     * {@inheritDoc}
     */
    protected class ScilabVerticalScrollBar extends ScrollBar {

        private Field trackRect;
        private Field thumbRect;
        private ScrollBarUI ui;

        /**
         * Default constructor
         */
        public ScilabVerticalScrollBar() {
            super(JScrollBar.VERTICAL);
            ui = getUI();
            initFields();
        }

        /**
         * Init the protected fields in BasicScrollBarUI. These fields are used to get the dimension of the thumb and of the track.
         */
        private void initFields() {
            if (ui instanceof BasicScrollBarUI) {
                try {
                    trackRect = BasicScrollBarUI.class.getDeclaredField("trackRect");
                    trackRect.setAccessible(true);
                    thumbRect = BasicScrollBarUI.class.getDeclaredField("thumbRect");
                    thumbRect.setAccessible(true);
                } catch (Exception e) { }
            }
        }

        /**
         * {@inheritDoc}
         */
        public void paint(Graphics g) {
            super.paint(g);
            g.setColor(MARKCOLOR);
            ScrollBarUI ui = getUI();
            if (this.ui != ui) {
                this.ui = ui;
                initFields();
            }
            if (ui instanceof BasicScrollBarUI && trackRect != null && thumbRect != null) {
                BasicScrollBarUI basicUI = (BasicScrollBarUI) ui;
                try {
                    Rectangle track = (Rectangle) trackRect.get(basicUI);
                    Rectangle thumb = (Rectangle) thumbRect.get(basicUI);
                    double ratio = (double) track.height / (double) getMaximum();
                    if (marks != null) {
                        for (Rectangle mark : marks) {
                            Rectangle r = new Rectangle(track.x, track.y + (int) ((mark.y + mark.height) * ratio), track.width, 2);
                            if (!thumb.contains(r)) {
                                g.fillRect(r.x, r.y, r.width, r.height);
                            }
                        }
                    }
                } catch (Exception e) { }
            }
        }
    }
}
