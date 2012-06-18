/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.scinotes;

import java.awt.Color;

import org.w3c.dom.Document;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.commons.xml.XConfiguration;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class SciNotesOptions {

    public static final String PREFERENCESPATH = "//scinotes/body/scinotes-preferences";
    public static final String DISPLAYPATH = "//scinotes/display/body/scinotes-display";
    public static final String AUTOSAVEPATH = "//scinotes/autosave/body/scinotes-autosave";
    public static final String KEYMAPPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"scinotes\"]/action";

    private static SciNotesOptions.Preferences prefs;
    private static SciNotesOptions.Display display;
    private static SciNotesOptions.Autosave autosave;

    private static Document doc;

    /* scinotes-preferences */
    @XConfAttribute
    public static class Preferences {

        public boolean restartOpen;
        public boolean addLineTermination;
        public int numberOfRecentlyOpen;
        public String encoding;
        public String eol;
        public boolean useScinotes;
        public boolean externalCmd;
        public String cmd;

        private Preferences() { }

        @XConfAttribute(tag = "scinotes-preferences", attributes = {"restart-reopen", "add-line-termination", "number-of-recently-open", "encoding", "eol", "scinotes", "cmd", "external-cmd"})
        private void set(boolean restartOpen, boolean addLineTermination, int numberOfRecentlyOpen, String encoding, String eol, boolean useScinotes, boolean externalCmd, String cmd) {
            this.restartOpen = restartOpen;
            this.addLineTermination = addLineTermination;
            this.numberOfRecentlyOpen = numberOfRecentlyOpen;
            this.encoding = encoding.toLowerCase();

            if (eol.startsWith("Windows")) {
                this.eol = ScilabDocument.EOLWIN;
            } else if (eol.startsWith("Mac")) {
                this.eol = ScilabDocument.EOLMAC;
            } else {
                this.eol = ScilabDocument.EOLUNIX;
            }

            this.useScinotes = useScinotes;
            this.externalCmd = externalCmd;
            this.cmd = cmd;
        }
    }

    /* scinotes-display */
    @XConfAttribute
    public static class Display {

        public boolean highlightCurrentLine;
        public Color currentLineColor;
        public boolean showLineNumbers;
        public boolean wrapLines;
        public boolean keywordsColorization;
        public boolean highlightBrackets;
        public Color bracketsColor;
        public int bracketsHighlightment;
        public boolean bracketsOnmouseover;
        public boolean highlightKeywords;
        public Color keywordsColor;
        public int keywordsHighlightment;
        public boolean keywordsOnmouseover;
        public boolean whereami;
        public int tabSize;
        public int tabRepresentation;
        public boolean useSpaces;
        public int indentSize;
        public boolean automaticIndent;
        public boolean autoCompleteOpeners;
        public boolean autoCompleteKeywords;

        private Display() { }

        @XConfAttribute(tag = "scinotes-display", attributes = {"highlight-current-line", "current-line-color", "show-line-numbers", "wrap-lines", "keywords-colorization", "highlight-brackets", "brackets-color", "brackets-highlightment", "brackets-onmouseover", "highlight-keywords", "keywords-color", "keywords-highlightment", "keywords-onmouseover", "whereami", "tab-size", "tab-representation", "use-spaces", "indent-size", "automatic-indent", "auto-complete-openers", "auto-complete-keywords"})
        private void set(boolean highlightCurrentLine, Color currentLineColor, boolean showLineNumbers, boolean wrapLines, boolean keywordsColorization, boolean highlightBrackets, Color bracketsColor, String bracketsHighlightment, boolean bracketsOnmouseover, boolean highlightKeywords, Color keywordsColor, String keywordsHighlightment, boolean keywordsOnmouseover, boolean whereami, int tabSize, String tabRepresentation, boolean useSpaces, int indentSize, boolean automaticIndent, boolean autoCompleteOpeners, boolean autoCompleteKeywords) {
            this.highlightCurrentLine = highlightCurrentLine;
            this.currentLineColor = currentLineColor;
            this.showLineNumbers = showLineNumbers;
            this.wrapLines = wrapLines;
            this.keywordsColorization = keywordsColorization;

            this.highlightBrackets = highlightBrackets;
            this.bracketsColor = bracketsColor;
            this.bracketsOnmouseover = bracketsOnmouseover;
            this.highlightKeywords = highlightKeywords;
            this.keywordsColor = keywordsColor;
            this.keywordsOnmouseover = keywordsOnmouseover;
            this.whereami = whereami;
            this.tabSize = tabSize;
            if (tabRepresentation.equalsIgnoreCase("chevrons")) {
                this.tabRepresentation = ScilabView.TABDOUBLECHEVRONS;
            } else if (tabRepresentation.equalsIgnoreCase("hrule")) {
                this.tabRepresentation = ScilabView.TABHORIZONTAL;
            } else if (tabRepresentation.equalsIgnoreCase("vrule")) {
                this.tabRepresentation = ScilabView.TABVERTICAL;
            } else {
                this.tabRepresentation = ScilabView.TABNOTHING;
            }
            this.useSpaces = useSpaces;
            this.indentSize = indentSize;
            this.automaticIndent = automaticIndent;

            if (bracketsHighlightment.equalsIgnoreCase("filled")) {
                this.bracketsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.FILLED;
            } else if (bracketsHighlightment.equalsIgnoreCase("framed")) {
                this.bracketsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.FRAMED;
            } else {
                this.bracketsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.UNDERLINED;
            }

            if (keywordsHighlightment.equalsIgnoreCase("filled")) {
                this.keywordsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.FILLED;
            } else if (keywordsHighlightment.equalsIgnoreCase("framed")) {
                this.keywordsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.FRAMED;
            } else {
                this.keywordsHighlightment = MatchingBlockManager.ScilabKeywordsPainter.UNDERLINED;
            }

            this.autoCompleteOpeners = autoCompleteOpeners;
            this.autoCompleteKeywords = autoCompleteKeywords;
        }
    }

    /* scinotes-autosave */
    @XConfAttribute
    public static class Autosave {

        public boolean autoSave;
        public int saveEvery;
        public boolean automaticDelete;
        public boolean appendFilename;
        public String appendWith;
        public String replaceWith;
        public boolean sourceFlag;
        public String singleDirectory;

        private Autosave() { }

        @XConfAttribute(tag = "scinotes-autosave", attributes = {"enable", "save-every", "automatic-delete", "append-filename", "append-with", "replace-with", "source-flag", "single-directory"})
        private void set(boolean autoSave, int saveEvery, boolean automaticDelete, boolean appendFilename, String appendWith, String replaceWith, boolean sourceFlag, String singleDirectory) {
            this.autoSave = autoSave;
            this.saveEvery = saveEvery;
            this.automaticDelete = automaticDelete;
            this.appendFilename = appendFilename;
            this.appendWith = appendWith;
            this.replaceWith = replaceWith;
            this.sourceFlag = sourceFlag;
            this.singleDirectory = singleDirectory;
        }
    }

    public static void invalidate(SciNotesConfiguration.Conf conf) {
        if (conf.preferences) {
            prefs = null;
            doc = null;
        }
        if (conf.display) {
            display = null;
            doc = null;
        }
        if (conf.autosave) {
            autosave = null;
            doc = null;
        }
    }

    public static final SciNotesOptions.Preferences getSciNotesPreferences() {
        if (prefs == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            prefs = XConfiguration.get(SciNotesOptions.Preferences.class, doc, PREFERENCESPATH)[0];
        }

        return prefs;
    }

    public static final SciNotesOptions.Display getSciNotesDisplay() {
        if (display == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            display = XConfiguration.get(SciNotesOptions.Display.class, doc, DISPLAYPATH)[0];
        }

        return display;
    }

    public static final SciNotesOptions.Autosave getSciNotesAutosave() {
        if (autosave == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }
            autosave = XConfiguration.get(SciNotesOptions.Autosave.class, doc, AUTOSAVEPATH)[0];
        }

        return autosave;
    }
}
