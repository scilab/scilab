/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.awt.BorderLayout;
import java.awt.Component;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;

/**
 * @author Calixte DENIZET
 */
public class EditorComponent extends JPanel {

    private static final long serialVersionUID = -1466505151043279504L;

    private ScilabEditorPane editor;
    private JScrollPane scroll;
    private JSplitPane split;
    private Component bottom;

    /**
     * Default constructor
     * @param the EditorPane to display
     */
    public EditorComponent(ScilabEditorPane editor) {
        super(new BorderLayout());
        this.editor = editor;
        scroll = new JScrollPane(editor);
        add(scroll);
    }

    /**
     * @return the scrollpane
     */
    public JScrollPane getScrollPane() {
        return scroll;
    }

    /**
     * @return the editorpane
     */
    public ScilabEditorPane getEditorPane() {
        return editor;
    }

    /**
     * @param split the split to use
     */
    public void setSplitPane(JSplitPane split) {
        this.split = split;
        remove(scroll);
        add(split, BorderLayout.CENTER);
        split.setVisible(true);
        setVisible(true);
        revalidate();
    }

    /**
     * @return the splitpane
     */
    public JSplitPane getSplitPane() {
        return split;
    }

    /**
     * @return true if the view is splited
     */
    public boolean isSplited() {
        return split != null;
    }

    /**
     * @param comp the component to insert at the bottom of this panel
     */
    public void insertBottomComponent(Component comp) {
        if (comp == null) {
            if (bottom != null) {
                remove(bottom);
            }
        } else {
            bottom = comp;
            add(comp, BorderLayout.PAGE_END);
        }
        revalidate();
    }

    /**
     * @return the component at the bottom
     */
    public Component getBottom() {
        return bottom;
    }
}
