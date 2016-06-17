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

import java.nio.charset.Charset;

import java.util.Vector;
import java.util.ArrayList;
import java.util.List;
import java.util.TreeSet;
import java.util.Set;
import java.util.HashSet;
import java.util.Comparator;
import java.util.Iterator;

import javax.swing.text.GapContent;
import javax.swing.text.PlainDocument;
import javax.swing.text.BadLocationException;
import javax.swing.text.Element;
import javax.swing.text.AttributeSet;
import javax.swing.text.View;

import javax.swing.tree.DefaultMutableTreeNode;

import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

import org.scilab.modules.scinotes.utils.ConfigSciNotesManager;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.scilab.modules.console.utils.ScilabLaTeXViewer;

/**
 * The class ScilabDocument is used to render a document .sci or .sce
 * @author Calixte DENIZET
 */
public class ScilabDocument extends PlainDocument implements DocumentListener {

    private static final long serialVersionUID = -1227880612912063687L;

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
    private static final String LINE_SEPARATOR = "line.separator";
    private static final int INITFUNCTIONSNUMBER = 128;

    private View view;
    private List<String> saved = new Vector<String>();
    private FunctionScanner funScanner;
    private ScilabLexer lexerBlockComment;

    private Set<String> functions = new HashSet<String>(INITFUNCTIONSNUMBER);

    private boolean contentModified;
    private boolean contentModifiedSinceBackup;
    private boolean alphaOrder;

    // Editor's default encoding is UTF-8
    private String encoding;
    private boolean updater = true;
    private boolean binary;
    private boolean autoIndent;
    private boolean shouldMergeEdits;
    private boolean undoManagerEnabled;
    private CompoundUndoManager undo;

    private ScilabEditorPane pane;
    private boolean focused;

    private String eolStyle;

    /**
     * Constructor
     */
    public ScilabDocument() {
        this(true);
    }

    /**
     * Constructor
     */
    public ScilabDocument(boolean paned) {
        super(new GapContent(GAPBUFFERCAPACITY));
        contentModified = false;
        lexerBlockComment = createLexer();

        if (paned) {
            setAsynchronousLoadPriority(2);

            autoIndent = SciNotesOptions.getSciNotesDisplay().automaticIndent;
            encoding = Charset.forName(SciNotesOptions.getSciNotesPreferences().encoding).toString();
            eolStyle = SciNotesOptions.getSciNotesPreferences().eol;

            undo = new CompoundUndoManager(this);
            addUndoableEditListener(undo);
            undoManagerEnabled = true;

            contentModifiedSinceBackup = false;
        }
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
    public ScilabLexer createLexer(boolean update) {
        return new ScilabLexer(this, update);
    }

    /**
     * Create a lexer used to colorize the text
     * @param update true if the scilab vars must be updated
     * @return ScilabLexer the lexer
     */
    public ScilabLexer createLexer() {
        return new ScilabLexer(this, true);
    }

    /**
     * @return the Set containing the functions name
     */
    public Set<String> getFunctionsInDoc() {
        return functions;
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

    public void addEOL() {
        if (SciNotesOptions.getSciNotesPreferences().addLineTermination) {
            int len = getLength();
            if (getLength() >= 1) {
                try {
                    String end = getText(len - 1, 1);
                    if (!end.equals("\n")) {
                        insertString(len, "\n", null);
                    }
                } catch (BadLocationException e) {
                    System.err.println(e);
                }
            } else {
                try {
                    insertString(len, "\n", null);
                } catch (BadLocationException e) {
                    System.err.println(e);
                }
            }
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
     * isContentModifiedSinceBackup
     * @return boolean
     */
    public boolean isContentModifiedSinceBackup() {
        return contentModifiedSinceBackup;
    }

    /**
     * setContentModified
     * @param contentModified boolean
     */
    public void setContentModifiedSinceBackup(boolean contentModified) {
        this.contentModifiedSinceBackup = contentModified;
    }

    /**
     * setContentModified
     * @param contentModified boolean
     */
    public void setContentModified(boolean contentModified) {
        this.contentModified = contentModified;
        if (pane != null && !contentModified) {
            undo.setReference();
            pane.updateTitle();
        } else {
            this.contentModifiedSinceBackup = true;
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
        List<FunctionScanner.FunctionInfo> list = new ArrayList<FunctionScanner.FunctionInfo>();
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
     * @param alphaOrder is true if names must be sorted with alphabetic order
     */
    public void setAlphaOrderInTree(boolean alphaOrder) {
        this.alphaOrder = alphaOrder;
    }

    /**
     * Fill a tree with function's name according to alphabetic order or not
     * @param base to fill
     */
    public synchronized void fillTreeFuns(DefaultMutableTreeNode base) {
        Element root = getDefaultRootElement();
        int nlines = root.getElementCount();
        if (!alphaOrder) {
            for (int i = 0; i < nlines; i++) {
                Element elem = root.getElement(i);
                if (elem instanceof ScilabDocument.ScilabLeafElement) {
                    int type = ((ScilabDocument.ScilabLeafElement) elem).getType();
                    switch (type) {
                        case ScilabDocument.ScilabLeafElement.NOTHING :
                            break;
                        case ScilabDocument.ScilabLeafElement.FUN :
                            base.add(new DefaultMutableTreeNode(elem));
                            break;
                        case ScilabDocument.ScilabLeafElement.ENDFUN :
                            break;
                        default :
                            break;
                    }
                }
            }
        } else {
            Set<DefaultMutableTreeNode> set = new TreeSet<DefaultMutableTreeNode>(new Comparator<DefaultMutableTreeNode>() {
                    public int compare(DefaultMutableTreeNode o1, DefaultMutableTreeNode o2) {
                        ScilabLeafElement l1 = (ScilabLeafElement) o1.getUserObject();
                        ScilabLeafElement l2 = (ScilabLeafElement) o2.getUserObject();
                        int n = l1.getFunctionName().compareTo(l2.getFunctionName());
                        if (n != 0) {
                            return n;
                    }
                    return l1.getStartOffset() - l2.getStartOffset();
                }

                public boolean equals(DefaultMutableTreeNode o1, DefaultMutableTreeNode o2) {
                    return false;
                }
            });
            for (int i = 0; i < nlines; i++) {
                Element elem = root.getElement(i);
                if (elem instanceof ScilabDocument.ScilabLeafElement) {
                    int type = ((ScilabDocument.ScilabLeafElement) elem).getType();
                    switch (type) {
                        case ScilabDocument.ScilabLeafElement.NOTHING :
                            break;
                        case ScilabDocument.ScilabLeafElement.FUN :
                            set.add(new DefaultMutableTreeNode(elem));
                            break;
                        case ScilabDocument.ScilabLeafElement.ENDFUN :
                            break;
                        default :
                            break;
                    }
                }
            }
            Iterator<DefaultMutableTreeNode> iter = set.iterator();
            while (iter.hasNext()) {
                base.add(iter.next());
            }
        }
    }

    /**
     * Fill a tree with anchor's name according to alphabetic order or not
     * @param base to fill
     */
    public synchronized void fillTreeAnchors(DefaultMutableTreeNode base) {
        Element root = getDefaultRootElement();
        int nlines = root.getElementCount();
        if (!alphaOrder) {
            for (int i = 0; i < nlines; i++) {
                ScilabLeafElement elem = (ScilabLeafElement) root.getElement(i);
                if (elem.isAnchor()) {
                    base.add(new DefaultMutableTreeNode(elem));
                }
            }
        } else {
            Set<DefaultMutableTreeNode> set = new TreeSet<DefaultMutableTreeNode>(new Comparator<DefaultMutableTreeNode>() {
                    public int compare(DefaultMutableTreeNode o1, DefaultMutableTreeNode o2) {
                        ScilabLeafElement l1 = (ScilabLeafElement) o1.getUserObject();
                        ScilabLeafElement l2 = (ScilabLeafElement) o2.getUserObject();
                        int n = l1.getAnchorName().compareTo(l2.getAnchorName());
                        if (n != 0) {
                            return n;
                        }
                    return l1.getStartOffset() - l2.getStartOffset();
                }

                public boolean equals(DefaultMutableTreeNode o1, DefaultMutableTreeNode o2) {
                    return false;
                }
            });
            for (int i = 0; i < nlines; i++) {
                ScilabLeafElement elem = (ScilabLeafElement) root.getElement(i);
                if (elem.isAnchor()) {
                    set.add(new DefaultMutableTreeNode(elem));
                }
            }
            Iterator<DefaultMutableTreeNode> iter = set.iterator();
            while (iter.hasNext()) {
                base.add(iter.next());
            }
        }
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
     * @param line the number of the line where to begin the search
     * @return the next anchor
     */
    public int nextAnchorFrom(int line) {
        Element root = getDefaultRootElement();
        for (int i = line + 1; i < root.getElementCount(); i++) {
            ScilabLeafElement se = (ScilabLeafElement) root.getElement(i);
            if (se.isAnchor()) {
                return i;
            }
        }
        for (int i = 0; i < line; i++) {
            ScilabLeafElement se = (ScilabLeafElement) root.getElement(i);
            if (se.isAnchor()) {
                return i;
            }
        }

        return -1;
    }

    /**
     * @param line the number of the line where to begin the search
     * @return the previous anchor
     */
    public int previousAnchorFrom(int line) {
        Element root = getDefaultRootElement();
        for (int i = line - 1; i >= 0; i--) {
            ScilabLeafElement se = (ScilabLeafElement) root.getElement(i);
            if (se.isAnchor()) {
                return i;
            }
        }
        for (int i = root.getElementCount() - 1; i > line; i--) {
            ScilabLeafElement se = (ScilabLeafElement) root.getElement(i);
            if (se.isAnchor()) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Get the anchors between two positions
     * @param start the beginning
     * @param end the end
     * @return a list of the anchors
     */
    public List<Anchor> getAnchorsBetween(int start, int end) {
        Element root = getDefaultRootElement();
        int lineS = root.getElementIndex(start);
        int lineE = root.getElementIndex(end);
        List<Anchor> list = new ArrayList<Anchor>();
        for (int i = lineS; i <= lineE; i++) {
            final ScilabLeafElement se = (ScilabLeafElement) root.getElement(i);
            if (se.isAnchor()) {
                list.add(new Anchor(i, se.getAnchorName()));
            }
        }

        return list;
    }

    /**
     * Get the lhs/rhs args used in a function declaration
     * @param pos the position in the document
     * @return the two lists containing args and returned values or null if we are not
     * in a function
     */
    public List<String>[] getInOutArgs(int pos) {
        Element root = getDefaultRootElement();
        int index = root.getElementIndex(pos);
        int compt = 0;
        while (index != -1) {
            ScilabLeafElement e = (ScilabLeafElement) root.getElement(index--);
            switch (e.getType()) {
                case ScilabLeafElement.NOTHING :
                    break;
                case ScilabLeafElement.FUN :
                    if (compt == 0) {
                        FunctionScanner.FunctionInfo info = e.getFunctionInfo();
                        return new List[] {info.returnValues, info.argsValues};
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
        return null;
    }

    /**
     * Get the function name where the caret is
     * @param pos the position in the document
     * @return the nearest function name
     */
    public String getCurrentFunction(int pos) {
        Element root = getDefaultRootElement();
        int index = root.getElementIndex(pos);
        int line = index;
        int compt = 0;
        while (index != -1) {
            ScilabLeafElement e = (ScilabLeafElement) root.getElement(index--);
            switch (e.getType()) {
                case ScilabLeafElement.NOTHING :
                    break;
                case ScilabLeafElement.FUN :
                    if (compt == 0) {
                        String str = e.getFunctionInfo().functionName;
                        if (str == null) {
                            str = SciNotesMessages.UNKNOWN_FUNCTION;
                        }
                        return String.format(SciNotesMessages.POSFUN_IN_DOC, line + 1, pos - root.getElement(line).getStartOffset(), str, line - index);
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
        return String.format(SciNotesMessages.POS_IN_DOC, line + 1, pos - root.getElement(line).getStartOffset());
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
     * {@inheritDoc}
     */
    protected void insertUpdate(DefaultDocumentEvent chng, AttributeSet attr) {
        // Fix bug 8277 in putting attr=null
        // Java, by default, highlights the chinese chars when entered on keyboard
        super.insertUpdate(chng, null);
    }

    /**
     * @param ev the DocumentEvent to handle
     */
    private void handleEvent(DocumentEvent ev) {
        if (!contentModified) {
            contentModified = true;
            if (pane != null) {
                pane.updateTitle();
            }
        }

        contentModifiedSinceBackup = true;

        DocumentEvent.ElementChange chg = ev.getChange(getDefaultRootElement());
        if (chg != null) {
            Element[] added = chg.getChildrenAdded();
            Element[] removed = chg.getChildrenRemoved();
            if ((added != null && added.length > 0) || (removed != null && removed.length > 0)) {
                for (int i = 0; i < removed.length; i++) {
                    String name = ((ScilabLeafElement) removed[i]).getFunctionName();
                    if (name != null && name.length() != 0) {
                        functions.remove(name);
                    }
                }
                for (int i = 0; i < added.length; i++) {
                    ((ScilabLeafElement) added[i]).resetType();
                    ((ScilabLeafElement) added[i]).resetTypeWhenBroken();
                    String name = ((ScilabLeafElement) added[i]).getFunctionName();
                    if (name != null && name.length() != 0) {
                        functions.add(name);
                    }
                }
            }
        } else {
            // change occurred only on one line
            Element root = getDefaultRootElement();
            int index = root.getElementIndex(ev.getOffset());
            ScilabLeafElement line = (ScilabLeafElement) root.getElement(index);
            boolean broken = line.isBroken();

            boolean brokenString = line.isBrokenString();
            boolean blockComment = line.isBlockComment();
            if (line.resetType() == ScilabLeafElement.FUN || brokenString != line.isBrokenString()
                || blockComment != line.isBlockComment()
                || broken != line.isBroken()
                || (index > 0 && ((ScilabLeafElement) root.getElement(index - 1)).isBroken())) {
                pane.repaint();
            }
        }

        if (pane != null) {
            KeywordEvent e = pane.getKeywordEvent();

            if (ScilabLexerConstants.isLaTeX(e.getType())) {
                try {
                    int start = e.getStart();
                    int end = start + e.getLength();
                    String exp = getText(start, e.getLength());
                    int height = pane.getScrollPane().getHeight() + pane.getScrollPane().getVerticalScrollBar().getValue();
                    ScilabLaTeXViewer.displayExpressionIfVisible(pane, height, exp, start, end);
                } catch (BadLocationException ex) { }
            }
        }
    }

    /**
     * @overload #createDefaultRoot
     * @return the element base
     */
    protected AbstractElement createDefaultRoot() {
        funScanner = new FunctionScanner(this);
        BranchElement map = (BranchElement) createBranchElement(null, null);
        Element line = createLeafElement(map, null, 0, 1);
        map.replace(0, 0, new Element[] {line});
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

        private static final long serialVersionUID = 4389590345677765643L;

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

        /**
         * broken line
         */
        public static final int BROKEN = 4;

        private boolean visible = true;
        private int previousType;
        private int type;
        private FunctionScanner.FunctionInfo info;
        private boolean broken;
        private boolean brokenString;
        private boolean blockComment;

        private boolean anchor;
        private String anchorName;

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
            if ((type & BROKEN) == BROKEN) {
                broken = true;
                type -= BROKEN;
            }

            if (type == FUN) {
                info = funScanner.getFunctionInfo();
            }
        }

        /**
         * Reset type (normally called on a change in the document)
         * @return the new type
         */
        public int resetType() {
            String oldName = "";
            if (type == FUN) {
                oldName = info.functionName;
            }

            previousType = type;
            type = funScanner.getLineType(getStartOffset(), getEndOffset());

            if ((type & BROKEN) == BROKEN) {
                broken = true;
                type -= BROKEN;
            } else {
                broken = false;
            }

            if (lexerBlockComment.isLineFinishedByBlockComment(getStartOffset(), getEndOffset())) {
                broken = true;
            } else {
                broken = false;
            }

            if (type == FUN) {
                info = funScanner.getFunctionInfo();
                if (info.functionName != null) {
                    if (!info.functionName.equals(oldName)) {
                        functions.remove(oldName);
                        functions.add(info.functionName);
                    }
                } else {
                    functions.remove(oldName);
                }
            }

            resetTypeWhenBroken();

            return type;
        }

        /**
         * @return true if the Line number panel need to be refreshed (useful in whereami mode)
         */
        public boolean needLineNumberRepaint() {
            return type == FUN || type == ENDFUN || type != previousType;
        }

        /**
         * If the previous line is broken, then this line is a part of it
         * so we need to resetType of the previous.
         */
        public void resetTypeWhenBroken() {
            int p0 = getStartOffset();
            if (p0 != 0) {
                Element parent = getParentElement();
                ScilabLeafElement elem = (ScilabLeafElement) parent.getElement(parent.getElementIndex(p0 - 1));
                if (elem.broken) {
                    elem.resetType();
                }
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

        /**
         * @return if this line is broken
         */
        public boolean isBroken() {
            return broken;
        }

        /**
         * @param b true if this line is broken
         */
        public void setBroken(boolean b) {
            broken = b;
        }

        /**
         * @return if this line is broken
         */
        public boolean isBrokenString() {
            return brokenString;
        }

        /**
         * @param b true if this line is broken in a string
         */
        public void setBrokenString(boolean b) {
            brokenString = b;
            if (b) {
                broken = true;
            }
        }

        /**
         * @return if this line is broken
         */
        public boolean isBlockComment() {
            return blockComment;
        }

        /**
         * @param b true if this line is broken in a string
         */
        public void setBlockComment(boolean b) {
            blockComment = b;
            if (b) {
                broken = true;
            }
        }

        /**
         * @return the function's name
         */
        public String getFunctionName() {
            if (type == FUN) {
                return info.functionName;
            }
            return "";
        }

        /**
         * @return if this line is an anchor
         */
        public boolean isAnchor() {
            return anchor;
        }

        /**
         * @param name the name of the anchor, if null remove
         * the anchor.
         */
        public void setAnchor(String name) {
            if (name == null) {
                anchor = false;
                return;
            }

            anchor = true;
            anchorName = name;
        }

        /**
         * @return the name of the anchor if exists
         */
        public String getAnchorName() {
            if (anchor) {
                return anchorName;
            } else {
                return "";
            }
        }

        /**
         * @return String representation
         */
        public String toString() {
            if (anchor) {
                if (type == FUN) {
                    return "function: " + info.functionName + " & anchor: " + anchorName;
                } else {
                    return anchorName;
                }
            }
            return info.functionName;
        }
    }

    /**
     * Inner class to get infos on anchor
     */
    public class Anchor {

        private int line;
        private String name;

        /**
         * Default constructor
         * @param line the line where the anchor is
         * @param name the anchor's name
         */
        public Anchor(int line, String name) {
            this.line = line;
            this.name = name;
        }

        /**
         * {@inheritDoc}
         */
        public String toString() {
            return name;
        }

        /**
         * @return the line number
         */
        public int getLine() {
            return line;
        }
    }
}
