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

import java.util.Vector;
import java.util.ArrayList;
import java.util.List;

import javax.swing.text.GapContent;
import javax.swing.text.PlainDocument;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;
import javax.swing.text.AttributeSet;
import javax.swing.text.View;

import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * The class ScilabDocument is used to render a document .sci or .sce
 * @author Calixte DENIZET
 */
public class ScilabDocument extends PlainDocument implements DocumentListener {

    /**
     * The EOL in mac OS
     */
    public static final String EOLMAC = "\r";

    /**
     * The EOL in windows OS
     */
    public static final String EOLWIN = "\r\n";

    /**
     * The EOL in unix OS
     */
    public static final String EOLUNIX = "\n";

    private static final int GAPBUFFERCAPACITY = 2;

    private View view;
    private List<String> saved = new Vector();
    private FunctionScanner funScanner;

    private boolean contentModified;

    // Editor's default encoding is UTF-8
    private String encoding = "UTF-8";
    private boolean updater = true;
    private boolean binary;
    private boolean autoIndent;
    private boolean autoColorize;
    private boolean shouldMergeEdits;
    private boolean undoManagerEnabled;
    private CompoundUndoManager undo;

    private ScilabEditorPane pane;
    private boolean focused;

    private String LINE_SEPARATOR = "line.separator";
    private String eolStyle = System.getProperty(LINE_SEPARATOR);

    /**
     * Constructor
     */
    public ScilabDocument() {
        super(new GapContent(GAPBUFFERCAPACITY));
        setAsynchronousLoadPriority(2);

        autoIndent = ConfigXpadManager.getAutoIndent();
        autoColorize = ConfigXpadManager.getAutoColorize();
        encoding = ConfigXpadManager.getDefaultEncoding();

        undo = new CompoundUndoManager(this);
        addUndoableEditListener(undo);
        undoManagerEnabled = true;
        contentModified = false;

        funScanner = new FunctionScanner(this);
        addDocumentListener(this);
    }

    /**
     * Set the ScilabEditorPane associated with this doc
     * @param pane the ScilabEditorPane
     */
    public void setEditorPane(ScilabEditorPane pane) {
        this.pane = pane;
    }

    /**
     * Get the ScilabEditorPane associated with this doc
     * @return pane the ScilabEditorPane
     */
    public ScilabEditorPane getEditorPane() {
        return pane;
    }

    /**
     * Set to true of the document is focused in the EditorPane
     * @param b the boolean
     */
    public void setFocused(boolean b) {
        focused = b;
    }

    /**
     * Create a lexer used to colorize the text
     * @return ScilabLexer the lexer
     */
    public ScilabLexer createLexer() {
        return new ScilabLexer(this);
    }

    /**
     * Set the current view to render the code
     * @param view the used view
     */
    public void setView(View view) {
        this.view = view;
    }

    /**
     * @return the current used view
     */
    public View getView() {
        return view;
    }

    /**
     * Get encoding
     * @return String encoding
     */
    public String getEncoding() {
        return encoding;
    }

    /**
     * set Encoding
     * @param encoding encoding
     */
    public void setEncoding(String encoding) {
        this.encoding = encoding;
    }

    /**
     * Set to true if it's a binary doc
     * @param binary boolean
     */
    public void setBinary(boolean binary) {
        this.binary = binary;
    }

    /**
     * @return true if it's a binary file
     */
    public boolean getBinary() {
        return binary;
    }

    /**
     * set end of line value
     * @param eol String
     */
    public void setEOL(String eol) {
        this.eolStyle = eol;
    }

    /**
     * get end of line
     * @return end of line
     */
    public String getEOL() {
        return this.eolStyle;
    }

    /**
     * get end of line
     * @return end of line
     */
    public String getDefaultEOL() {
        return System.getProperty(LINE_SEPARATOR);
    }

    /**
     * isUpdater
     * @return boolean
     */
    public boolean isUpdater() {
        return updater;
    }

    /**
     * getAutoIndent
     * @return boolean
     */
    public boolean getAutoIndent() {
        return autoIndent;
    }

    /**
     * setAutoIndent
     * @param b boolean
     */
    public void setAutoIndent(boolean b) {
        autoIndent = b;
    }

    /**
     * setUpdater
     * @param updaterDisabled boolean
     */
    public void setUpdater(boolean updaterDisabled) {
        this.updater = updaterDisabled;
    }


    /**
     * Get document text
     * @return String
     */
    public String getText() {
        try {
            return getText(0, getLength());
        } catch (BadLocationException e) {
            return "";
        }
    }

    /**
     * Begins a compound edit (for the undo)
     */
    public void mergeEditsBegin() {
        undo.endCompoundEdit();
        undo.startCompoundEdit();
    }

    /**
     * Ends a compound edit (for the undo)
     */
    public void mergeEditsEnd() {
        undo.endCompoundEdit();
    }

    /**
     * getColorize
     * @return boolean
     */
    public boolean getColorize() {
        return autoColorize;
    }

    /**
     * setColorize
     * @param b boolean
     */
    public void setColorize(boolean b) {
        autoColorize = b;
    }

    /**
     * getUndoManager
     * @return CompoundUndoManager
     */
    public CompoundUndoManager getUndoManager() {
        return undo;
    }

    /**
     * disableUndoManager
     */
    public void disableUndoManager() {
        if (undoManagerEnabled) {
            this.removeUndoableEditListener(undo);
            undoManagerEnabled = false;
        }
    }

    /**
     * enableUndoManager
     */
    public void enableUndoManager() {
        if (!undoManagerEnabled) {
            this.addUndoableEditListener(undo);
            undoManagerEnabled = true;
        }
    }

    /**
     * isContentModified
     * @return boolean
     */
    public boolean isContentModified() {
        return contentModified;
    }

    /**
     * setContentModified
     * @param contentModified boolean
     */
    public void setContentModified(boolean contentModified) {
        this.contentModified = contentModified;
        if (!contentModified) {
            undo.setReference();
            pane.updateTitle();
        }
    }

    /**
     * dump document on stderr with line positions
     */
    public void dump() {
        readLock();
        try {
            Element root = getDefaultRootElement();
            for (int i = 0; i != root.getElementCount(); ++i) {
                Element e = root.getElement(i);
                int start = e.getStartOffset();
                int end = e.getEndOffset();
                System.err.println("line " + i + " from: " + start + " to: " + end + ":|" + getText(start, end - start) + "|");
            }
        } catch (BadLocationException e) {
            System.err.println(e);
        }
        readUnlock();
    }

    /**
     * Search the position of the function name in the Document
     * @param name the name of the function
     * @return the position where to go or -1 if not found
     */
    public int searchFunctionByName(String name) {
        Element root = getDefaultRootElement();
        for (int i = 0; i < root.getElementCount(); i++) {
            Element e = root.getElement(i);
            if (e instanceof ScilabLeafElement) {
                ScilabLeafElement se = (ScilabLeafElement) e;
                if (se.isFunction() && se.getFunctionInfo().functionName.equals(name)) {
                    return e.getStartOffset();
                }
            }
        }
        return -1;
    }

    /**
     * @return a list containing all the infos about functions available in this document
     */
    public List<FunctionScanner.FunctionInfo> getFunctionInfo() {
        List list = new ArrayList();
        Element root = getDefaultRootElement();
        for (int i = 0; i < root.getElementCount(); i++) {
            Element e = root.getElement(i);
            if (e instanceof ScilabLeafElement) {
                ScilabLeafElement se = (ScilabLeafElement) e;
                if (se.isFunction()) {
                    list.add(se.getFunctionInfo());
                }
            }
        }
        return list;
    }

    /**
     * @return the first function name which appears in this doc or null
     */
    public String getFirstFunctionName() {
        Element root = getDefaultRootElement();
        for (int i = 0; i < root.getElementCount(); i++) {
            Element e = root.getElement(i);
            if (e instanceof ScilabLeafElement) {
                ScilabLeafElement se = (ScilabLeafElement) e;
                if (se.isFunction()) {
                    return se.getFunctionInfo().functionName;
                }
            }
        }
        return null;
    }

    /**
     * Get the local variables used as arguments or returned valuesof a function
     * @param pos the position in the document
     * @return the two lists containing args and returned values or null if we are not
     * in a function
     */
    public List<String>[] getLocalVariables(int pos) {
        Element root = getDefaultRootElement();
        int index = root.getElementIndex(pos);
        int compt = 0;
        while (index != -1) {
            Element e = root.getElement(index--);
            if (e instanceof ScilabLeafElement) {
                ScilabLeafElement se = (ScilabLeafElement) e;
                int type = se.getType();
                switch (type) {
                case ScilabLeafElement.NOTHING :
                    break;
                case ScilabLeafElement.FUN :
                    if (compt == 0) {
                        FunctionScanner.FunctionInfo info = se.getFunctionInfo();
                        return new List[]{info.returnValues, info.argsValues};
                    } else {
                        compt++;
                    }
                    break;
                case ScilabLeafElement.ENDFUN :
                    compt--;
                    break;
                default :
                }
            }
        }
        return null;
    }

    /**
     * lock
     */
    public void lock() {
        super.writeLock();
    }

    /**
     * unlock
     */
    public void unlock() {
        super.writeUnlock();
    }

    /**
     * Implements DocumentListener
     * @param documentEvent DocumentEvent
     */
    public void changedUpdate(DocumentEvent documentEvent) { }

    /**
     * Implements DocumentListener
     * @param documentEvent DocumentEvent
     */
    public void insertUpdate(DocumentEvent documentEvent) {
        if (!contentModified && pane != null) {
            contentModified = true;
            pane.updateTitle();
        }
    }

    /**
     * Implements DocumentListener
     * @param documentEvent DocumentEvent
     */
    public void removeUpdate(DocumentEvent documentEvent) {
        if (!contentModified && pane != null) {
            contentModified = true;
            pane.updateTitle();
        }
    }

    /**
     * @overload #createDefaultRoot
     * @return the element base
     */
    protected AbstractElement createDefaultRoot() {
        BranchElement map = (BranchElement) createBranchElement(null, null);
        Element line = super.createLeafElement(map, null, 0, 1);
        Element[] lines = new Element[1];
        lines[0] = line;
        map.replace(0, 0, lines);
        return map;
    }

    /**
     * @overload #createLeafElement
     * @param parent the parent Element
     * @param a an AttributeSet
     * @param p0 start in the doc
     * @param p1 end in the doc
     * @return the created LeafElement
     */
    protected Element createLeafElement(Element parent, AttributeSet a, int p0, int p1) {
        return new ScilabLeafElement(parent, a, p0, p1);
    }

    /**
     * Inner class to consider the type of a line :
     *  - FUN : function y=foo(x)
     *  - ENDFUN : endfunction
     *  - NOTHING : bla bla bla
     * This inner class is useful to make a line numbering compatible with the whereami macro.
     */
    public class ScilabLeafElement extends LeafElement {

        /**
         * Nothing in this line
         */
        public static final int NOTHING = 0;

        /**
         * function ... in this line
         */
        public static final int FUN = 1;

        /**
         * endfunction in this line
         */
        public static final int ENDFUN = 2;

        private boolean visible = true;
        private int type;
        private FunctionScanner.FunctionInfo info;

        /**
         * The same constructor as in LeafElement.
         * @param parent the parent Element
         * @param a an AttributeSet
         * @param p0 start in the doc
         * @param p1 end in the doc
         */
        public ScilabLeafElement(Element parent, AttributeSet a, int p0, int p1) {
            super(parent, a, p0, p1);
            type = funScanner.getLineType(p0, p1);
            if (type == FUN) {
                info = funScanner.getFunctionInfo();
            }
        }

        /**
         * Reset type (normally called on a change in the document)
         */
        public void resetType() {
            type = funScanner.getLineType(getStartOffset(), getEndOffset());
            if (type == FUN) {
                info = funScanner.getFunctionInfo();
            }
        }

        /**
         * @return the type of this line (FUN,...)
         */
        public int getType() {
            return type;
        }

        /**
         * @return the info about this line containing a function def
         */
        public FunctionScanner.FunctionInfo getFunctionInfo() {
            return info;
        }

        /**
         * @return if this line begins with function
         */
        public boolean isFunction() {
            return type == FUN;
        }

        /**
         * @return if this line begins with endfunction
         */
        public boolean isEndfunction() {
            return type == ENDFUN;
        }

        /**
         * @return if this line is visible
         */
        public boolean isVisible() {
            return visible;
        }

        /**
         * @param b true if this line is visible
         */
        public void setVisible(boolean b) {
            visible = b;
        }
    }
}
