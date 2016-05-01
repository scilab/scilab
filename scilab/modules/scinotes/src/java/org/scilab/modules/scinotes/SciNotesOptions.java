/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises -Calixte DENIZET
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

import java.awt.Color;
import java.text.DateFormat;
import java.util.Calendar;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.scilab.modules.commons.OS;

import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.commons.xml.XConfiguration;

/**
 * The class ScilabContext provides a context to render a Scilab's document.
 * @author Calixte DENIZET
 */
public class SciNotesOptions {

    public static final String PREFERENCESPATH = "//scinotes/body/scinotes-preferences";
    public static final String DISPLAYPATH = "//scinotes/display/body/scinotes-display";
    public static final String AUTOSAVEPATH = "//scinotes/autosave/body/scinotes-autosave";
    public static final String HEADERPATH = "//scinotes/header/body/scinotes-header";
    public static final String KEYMAPPATH = "//general/shortcuts/body/actions/action-folder[@xconf-uid=\"scinotes\"]/action";

    private static SciNotesOptions.Preferences prefs;
    private static SciNotesOptions.Display display;
    private static SciNotesOptions.Autosave autosave;
    private static SciNotesOptions.Header header;

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
        public boolean completeAtEOL;

        private Preferences() { }

        @XConfAttribute(tag = "scinotes-preferences", attributes = {"restart-reopen", "add-line-termination", "number-of-recently-open", "encoding", "eol", "scinotes", "cmd", "external-cmd", "complete-at-eol"})
        private void set(boolean restartOpen, boolean addLineTermination, int numberOfRecentlyOpen, String encoding, String eol, boolean useScinotes, String cmd, boolean externalCmd, boolean completeAtEOL) {
            this.restartOpen = restartOpen;
            this.addLineTermination = addLineTermination;
            this.numberOfRecentlyOpen = numberOfRecentlyOpen > 0 ? numberOfRecentlyOpen : 1;
            this.encoding = encoding.toLowerCase();

            if (eol.equals("")) {
                switch (OS.get()) {
                    case WINDOWS:
                        this.eol = ScilabDocument.EOLWIN;
                        break;
                    default:
                        this.eol = ScilabDocument.EOLUNIX;
                        break;
                }
            } else if (eol.startsWith("Windows")) {
                this.eol = ScilabDocument.EOLWIN;
            } else {
                this.eol = ScilabDocument.EOLUNIX;
            }

            this.useScinotes = useScinotes;
            this.externalCmd = externalCmd;
            this.cmd = cmd;
            this.completeAtEOL = completeAtEOL;
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
        public Color backgroundColor;
        public Color caretColor;

        private Display() { }

        @XConfAttribute(tag = "scinotes-display", attributes = {"highlight-current-line", "current-line-color", "show-line-numbers", "wrap-lines", "keywords-colorization", "highlight-brackets", "brackets-color", "brackets-highlightment", "brackets-onmouseover", "highlight-keywords", "keywords-color", "keywords-highlightment", "keywords-onmouseover", "whereami", "tab-size", "tab-representation", "use-spaces", "indent-size", "automatic-indent", "auto-complete-openers", "auto-complete-keywords", "background-color", "caret-color"})
        private void set(boolean highlightCurrentLine, Color currentLineColor, boolean showLineNumbers, boolean wrapLines, boolean keywordsColorization, boolean highlightBrackets, Color bracketsColor, String bracketsHighlightment, boolean bracketsOnmouseover, boolean highlightKeywords, Color keywordsColor, String keywordsHighlightment, boolean keywordsOnmouseover, boolean whereami, int tabSize, String tabRepresentation, boolean useSpaces, int indentSize, boolean automaticIndent, boolean autoCompleteOpeners, boolean autoCompleteKeywords, Color backgroundColor, Color caretColor) {
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
            this.backgroundColor = backgroundColor;
            this.caretColor = caretColor;
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

    /* scinotes-header */
    @XConfAttribute
    public static class Header {

        public String header;
        public boolean enable;

        @XConfAttribute(tag = "scinotes-header", attributes = {"enable"})
        private void set(boolean enable) {
            this.enable = enable;
            if (enable) {
                XPathFactory xpathFactory = ScilabXPathFactory.newInstance();
                XPath xp = xpathFactory.newXPath();
                try {
                    header = (String) xp.compile("string(" + HEADERPATH + ")").evaluate(doc, XPathConstants.STRING);
                } catch (XPathExpressionException e) {
                    System.err.println(e);
                }

                if (header != null) {
                    Calendar cal = Calendar.getInstance();
                    DateFormat dateFormat = DateFormat.getDateInstance();
                    header = header.replaceAll("\\{\\$current-year\\}", Integer.toString(cal.get(Calendar.YEAR)));
                    header = header.replaceAll("\\{\\$current-date\\}", dateFormat.format(cal.getTime()));
                }
            }
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
        if (conf.header) {
            header = null;
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

    public static final SciNotesOptions.Header getSciNotesHeader() {
        if (header == null) {
            if (doc == null) {
                doc = XConfiguration.getXConfigurationDocument();
            }

            header = XConfiguration.get(SciNotesOptions.Header.class, doc, HEADERPATH)[0];
        }


        return header;
    }
}
