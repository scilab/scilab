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

package org.scilab.modules.uiwidget.components;

import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.io.File;
import java.net.URL;
import java.util.Map;

import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;
import javax.swing.JEditorPane;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.uiwidget.UIComponent;
import org.scilab.modules.uiwidget.UIComponentAnnotation;
import org.scilab.modules.uiwidget.UIWidgetException;
import org.scilab.modules.uiwidget.UIWidgetTools;

public class UIText extends UIComponent {

    private JEditorPane editor;
    private boolean onurlclickEnable = true;
    private String action;
    private boolean horizontalScroll;

    public UIText(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    public Object newInstance() {
        editor = new JEditorPane();

        return editor;
    }

    @UIComponentAnnotation(attributes = {"text", "url", "rows", "columns", "color", "font", "underline", "bold", "italic", "strike-through", "size", "weight", "horizontal-scroll"})
    public Object newInstance(String text, String url, int rows, int columns, Color color, String font, boolean underline, boolean bold, boolean italic, boolean strikethrough, double size, UITools.FontWeight weight, boolean horizontalScroll) {
        this.horizontalScroll = horizontalScroll;
        URL u = null;

        try {
            File f = new File(url);
            if (f.exists()) {
                u = f.toURI().toURL();
            } else {
                u = new URL(url);
            }
        } catch (Exception e) { }

        if (u != null) {
            try {
                editor = new JEditorPane(u) {
                    public boolean getScrollableTracksViewportWidth() {
                        return !UIText.this.horizontalScroll;
                    }
                };
            } catch (Exception e) { }
        }

        if (editor == null) {
            editor = new JEditorPane()  {
                public boolean getScrollableTracksViewportWidth() {
                    return !UIText.this.horizontalScroll;
                }
            };
            editor.setText(text);
        }

        if (color != null) {
            editor.setForeground(color);
        }

        if (font != null && !font.isEmpty()) {
            editor.setFont(UITools.getFont(null, null, font, size, underline, bold, italic, strikethrough, weight));
        } else {
            editor.setFont(UITools.getFont(null, null, editor.getFont(), size, underline, bold, italic, strikethrough, weight));
        }

        editor.setCaretPosition(0);
        editor.setMinimumSize(new Dimension(0, 0));

        return editor;
    }

    public void setHorizontalScroll(boolean b) {
        this.horizontalScroll = b;
    }

    public boolean getHorizontalScroll() {
        return this.horizontalScroll;
    }

    public void setSelectAll(boolean all) {
        if (all) {
            editor.selectAll();
        } else {
            editor.select(0, 0);
        }
    }

    public void setUrl(String url) {
        URL u = null;

        try {
            File f = new File(url);
            if (f.exists()) {
                u = f.toURI().toURL();
            } else {
                u = new URL(url);
            }
        } catch (Exception e) { }

        if (u != null) {
            try {
                editor.setPage(u);
            } catch (Exception e) { }
        }
    }

    public void setOnurlclick(final String action) {
        if (this.action == null) {
            editor.addHyperlinkListener(new HyperlinkListener() {
                public void hyperlinkUpdate(HyperlinkEvent e) {
                    if (onurlclickEnable && e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                        UIWidgetTools.execAction(UIText.this, UIText.this.action, e.getURL());
                    }
                }
            });
        }
        this.action = action;
    }

    public void setOnurlclikEnable(boolean b) {
        onurlclickEnable = b;
    }

    public void append(String str) {
        Document doc = editor.getDocument();
        if (doc != null) {
            try {
                doc.insertString(doc.getLength(), str, null);
            } catch (BadLocationException e) { }
        }
    }

    /*    public void setText(String[] strs) {
            editor.setText("");
            if (strs != null) {
                for (int i = 0; i < strs.length; i++) {
                    append(strs[i]);
                    if (i != strs.length - 1) {
                        append("\n");
                    }
                }
            }
    	}*/
}
