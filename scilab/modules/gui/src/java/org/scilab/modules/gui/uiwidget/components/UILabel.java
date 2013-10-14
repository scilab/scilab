/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.Color;
import java.awt.Cursor;
import java.awt.Font;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Map;

import javax.swing.Icon;
import javax.swing.JLabel;

import org.scilab.modules.gui.utils.WebBrowser;
import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * JLabel wrapper
 */
public class UILabel extends UIComponent {

    private final static int defaultAlignment = JLabel.LEFT;
    private final static Cursor HANDCURSOR = Cursor.getPredefinedCursor(Cursor.HAND_CURSOR);
    private final static Cursor DEFAULTCURSOR = Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR);

    private JLabel label;
    private String link;
    private MouseListener clicklistener;

    public enum Alignment {
        LEADING (JLabel.LEADING),
        CENTER (JLabel.CENTER),
        LEFT (JLabel.LEFT),
        RIGHT (JLabel.RIGHT),
        TRAILING (JLabel.TRAILING);

        private final int value;

        Alignment(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }

        public static Alignment get(int value) {
            switch (value) {
                case JLabel.LEADING:
                    return Alignment.LEADING;
                case JLabel.CENTER:
                    return Alignment.CENTER;
                case JLabel.LEFT:
                    return Alignment.LEFT;
                case JLabel.RIGHT:
                    return Alignment.RIGHT;
                case JLabel.TRAILING:
                    return Alignment.TRAILING;
                default:
                    return Alignment.LEADING;
            }
        }

        public static String getAsString(int value) {
            switch (value) {
                case JLabel.LEADING:
                    return "leading";
                case JLabel.CENTER:
                    return "center";
                case JLabel.LEFT:
                    return "left";
                case JLabel.RIGHT:
                    return "right";
                case JLabel.TRAILING:
                    return "trailing";
                default:
                    return "leading";
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public UILabel(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        label = new JLabel();

        return label;
    }

    @UIComponentAnnotation(attributes = {"text", "icon"})
    public Object newInstance(String text, Icon icon) {
        if (icon != null) {
            label = new JLabel(text, icon, defaultAlignment);
        } else {
            label = new JLabel(text);
        }

        return label;
    }

    /**
     * Set a link to open when the UILabel is clicked
     * @param link the link
     */
    public void setLink(String link) {
        if (link != this.link) {
            this.link = link;
            removeClickListener();
            if (link != null && !link.isEmpty()) {
                clicklistener = new MouseAdapter() {
                    public void mouseClicked(MouseEvent e) {
                        WebBrowser.openUrl(UILabel.this.link);
                    }

                    public void mouseEntered(MouseEvent e) {
                        label.setCursor(HANDCURSOR);
                    }

                    public void mouseExited(MouseEvent e) {
                        label.setCursor(DEFAULTCURSOR);
                    }
                };
                label.addMouseListener(clicklistener);
            }
        }
    }

    /**
     * Get the link to open when the UILabel is clicked
     * @return the link
     */
    public String getLink() {
        return link;
    }

    /**
     * Remove the onclick listener
     */
    protected void removeClickListener() {
        if (clicklistener != null) {
            label.removeMouseListener(clicklistener);
            clicklistener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeClickListener();
        super.remove();
    }

    /**
     * {@inheritDoc}
     */
    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        if (style != null) {
            String al = style.get("alignment");
            if (al != null && !al.isEmpty()) {
                Alignment a = StringConverters.getObjectFromValue(Alignment.class, al);
                label.setHorizontalAlignment(a.value());
                style.remove("alignment");
            }
            super.setUiStyle(style);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setBackground(Color color) {
        if (color != null) {
            label.setOpaque(true);
            label.setBackground(color);
        }
    }

    /**
     * Alias for setText
     */
    public void setString(String str) {
        label.setText(str);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return label.getText();
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setAlignment(Alignment a) {
        label.setHorizontalAlignment(a.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getAlignment() {
        return Alignment.getAsString(label.getHorizontalAlignment());
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setHorizontalAlignment(Alignment alignment) {
        label.setHorizontalAlignment(alignment.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getHorizontalAlignment() {
        return Alignment.getAsString(label.getHorizontalAlignment());
    }
}
