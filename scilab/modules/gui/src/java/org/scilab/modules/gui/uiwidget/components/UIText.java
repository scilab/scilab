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
import javax.swing.text.html.HTMLDocument;
import javax.swing.text.html.HTMLEditorKit;

import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JEditorPane wrapper
 */
public class UIText extends UIComponent {

    // TODO: add callback pr l'edition genre onedit, onselection...

    protected JEditorPane editor;
    protected boolean onurlclickEnable = true;
    protected UICallback action;
    protected HyperlinkListener hyperlinkListener;
    protected boolean horizontalScroll;

    /**
     * {@inheritDoc}
     */
    public UIText(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        editor = new JEditorPane();

        return editor;
    }

    @UIComponentAnnotation(attributes = {"text", "url", "rows", "columns", "color", "font", "underline", "bold", "italic", "strike-through", "size", "weight", "horizontal-scroll"})
    public Object newInstance(String text, String url, int rows, int columns, Color color, String font, boolean underline, boolean bold, boolean italic, boolean strikethrough, double size, UITools.FontWeight weight, boolean horizontalScroll) {
        this.horizontalScroll = horizontalScroll;

        if (url != null && !url.isEmpty()) {
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
        }

        if (editor == null) {
            editor = new JEditorPane() {
                public boolean getScrollableTracksViewportWidth() {
                    return !UIText.this.horizontalScroll;
                }
            };
            if (text != null && !text.isEmpty()) {
                setText(new String[] {text});
            }
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

    /**
     * Enable or not an horizontal scrollbar
     * @param b if true it could have an horizontal scrollbar
     */
    public void setHorizontalScroll(boolean b) {
        this.horizontalScroll = b;
    }

    /**
     * Check if it could have an horizontal scrollbar
     * @return true it could have an horizontal scrollbar
     */
    public boolean getHorizontalScroll() {
        return this.horizontalScroll;
    }

    /**
     * Select or not all the contents
     * @param all if true, all is selected
     */
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

    /**
     * Remove a listener
     */
    protected void removeHyperlinkListener() {
        if (hyperlinkListener != null) {
            editor.removeHyperlinkListener(hyperlinkListener);
            hyperlinkListener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeHyperlinkListener();
        super.remove();
    }

    public void setOnurlclick(final String action) {
        if (this.action == null) {
            removeHyperlinkListener();
            hyperlinkListener = new HyperlinkListener() {
                public void hyperlinkUpdate(HyperlinkEvent e) {
                    if (onurlclickEnable && e.getEventType() == HyperlinkEvent.EventType.ACTIVATED) {
                        UIWidgetTools.execAction(UIText.this.action, "\"" + e.getURL() + "\"");
                    }
                }
            };
            editor.addHyperlinkListener(hyperlinkListener);
        }
        this.action = UICallback.newInstance(this, action);
    }

    public UICallback getOnurlclick() {
        return action;
    }

    public void setOnurlclikEnable(boolean b) {
        onurlclickEnable = b;
    }

    public boolean setOnurlclikEnable() {
        return onurlclickEnable;
    }

    /**
     * Append text in the pane
     * @param str the string to append
     */
    protected void append(String str) {
        if (str != null) {
            Document doc = editor.getDocument();
            if (doc != null) {
                if (doc instanceof HTMLDocument) {
                    HTMLEditorKit kit = (HTMLEditorKit) editor.getEditorKit();
                    HTMLDocument htmlDoc = (HTMLDocument) doc;
                    try {
                        kit.insertHTML(htmlDoc, doc.getLength(), str, 0, 0, null);
                    } catch (Exception e) {
                        try {
                            doc.insertString(doc.getLength(), str, null);
                        } catch (BadLocationException be) { }
                    }
                } else {
                    try {
                        doc.insertString(doc.getLength(), str, null);
                    } catch (BadLocationException e) { }
                }
            }
        }
    }

    /**
     * Set the text
     * @param strs the text
     */
    public void setText(String[] strs) {
        if (!isRootVisible()) {
            editor.setSize(Short.MAX_VALUE, Short.MAX_VALUE);
        }

        Document doc = editor.getDocument();
        try {
            doc.remove(0, doc.getLength());
        } catch (BadLocationException e) { }

        if (strs != null) {
            for (int i = 0; i < strs.length; i++) {
                append(strs[i]);
                if (i != strs.length - 1) {
                    append("\n");
                }
            }
        }
    }
}
