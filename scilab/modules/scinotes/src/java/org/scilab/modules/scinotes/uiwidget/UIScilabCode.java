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

package org.scilab.modules.gui.uiwidget;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Image;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JMenuBar;

import javax.swing.text.BadLocationException;
import javax.swing.text.Document;

import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.scinotes.ScilabDocument;
import org.scilab.modules.scinotes.ScilabEditorKit;
import org.scilab.modules.scinotes.ScilabEditorPane;
import org.scilab.modules.scinotes.ScilabLexer;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;

/**
 * ScilabEditorPane wrapper
 */
public class UIScilabCode extends UIComponent {

    private ScilabEditorPane pane;
    private File file;

    static {
        ScilabCommonsUtils.loadOnUse("SciNotes");
    }

    /**
     * {@inheritDoc}
     */
    public UIScilabCode(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        pane = new MyClass().pane;

        return pane.getEditorComponent();
    }

    /**
     * {@inheritDoc}
     */
    public Object getModifiableComponent() {
        return pane;
    }

    public void setFile(File f) {
        if (f != null && f.exists() && f.canRead()) {
            ScilabEditorKit kit = (ScilabEditorKit) pane.getEditorKit();
            ScilabDocument doc = (ScilabDocument) pane.getDocument();
            try {
                kit.read(null, f, doc, 0);
                pane.setName(f.getAbsolutePath());
                this.file = f;
            } catch (BadLocationException e) {
                System.err.println(e);
            } catch (IOException e) {
                System.err.println(e);
            }
        }
    }

    public File getFile() {
        return file;
    }

    public void setLineNumbering(String type) {
        if (type != null && !type.isEmpty()) {
            type = type.toLowerCase();
            if (type.equals("none")) {
                pane.getXln().setWhereamiLineNumbering(0);
            } else if (type.equals("normal")) {
                pane.getXln().setWhereamiLineNumbering(1);
            } else if (type.equals("whereami")) {
                pane.getXln().setWhereamiLineNumbering(2);
            }
        }
    }

    public String getLineNumbering() {
        int state = pane.getXln().getState();
        switch (state) {
            case 0:
                return "none";
            case 1:
                return "normal";
            case 2:
                return "whereami";
            default:
                return "unknown";
        }
    }

    /**
     * Select or not all the contents
     * @param all if true, all is selected
     */
    public void setSelectAll(boolean all) {
        if (all) {
            pane.selectAll();
        } else {
            pane.select(0, 0);
        }
    }

    /**
     * {@inheritDoc}
     */
    public void setScrollable() {

    }

    /**
     * {@inheritDoc}
     */
    public boolean getScrollable() {
        return true;
    }

    /**
     * Append text in the pane
     * @param str the string to append
     */
    protected void append(String str) {
        if (str != null) {
            Document doc = pane.getDocument();
            if (doc != null) {
                try {
                    doc.insertString(doc.getLength(), str, null);
                } catch (BadLocationException e) { }
            }
        }
    }

    /**
     * Set the text
     * @param strs the text
     */
    public void setText(String[] strs) {
        if (!isRootVisible()) {
            pane.setSize(Short.MAX_VALUE, Short.MAX_VALUE);
        }

        Document doc = pane.getDocument();
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

    /**
     * Trick to avoid SciNotes jar loading on UIScilabCode loading
     */
    private static class MyClass {

        ScilabEditorPane pane;

        MyClass() {
            ScilabLexer.update();
            pane = new ScilabEditorPane(null);
            ScilabEditorKit kit = new ScilabEditorKit();
            pane.setEditorKit(kit);
            pane.setFont(kit.getStylePreferences().tokenFonts[0]);
        }
    }
}