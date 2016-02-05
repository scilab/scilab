/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.SwingWorker;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.tree.DefaultMutableTreeNode;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.scinotes.utils.SciNotesMessages;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

/**
 * Class SearchManager
 * @author Sylvestre Koumar
 * @author Calixte DENIZET
 */
public class SearchManager {

    private static final ImageIcon FILEIMAGE = new ImageIcon(FindIconHelper.findIcon("stock_search"));
    private static final ImageIcon SCILABFILEIMAGE = new ImageIcon(FindIconHelper.findIcon("scilab_search"));
    private static final ImageIcon FOLDERIMAGE = new ImageIcon(FindIconHelper.findIcon("folder-saved-search"));
    private static final ImageIcon LINEICON = new ImageIcon(FindIconHelper.findIcon("line-found"));

    /**
     * FIND AND REPLACE START
     * @param scilabDocument document
     * @param word string
     * @param start int
     * @param end int
     * @param caseSensitive boolean
     * @param wholeWord boolean
     * @param useRegexp boolean
     * @return List
     */
    public static List<Integer[]> findWord(Document scilabDocument, String word,
                                           int start, int end,
                                           boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
        String fullText = "";
        try {
            fullText = scilabDocument.getText(start, end - start + 1);
        } catch (BadLocationException ex) {
            ex.printStackTrace();
        }

        if (fullText.length() == 0) {
            return null;
        }

        List<Integer[]> offsetList = new ArrayList<Integer[]>();

        //If we don't give any word to find
        if (word != null && !word.equals("")) {
            Pattern pattern = generatePattern(word, caseSensitive, wholeWord, useRegexp);
            Matcher matcher = pattern.matcher(fullText);

            while (matcher.find()) {
                if (matcher.start() != matcher.end()) {
                    offsetList.add(new Integer[] {matcher.start() + start, matcher.end() + start});
                }
            }
        }

        return offsetList;
    }

    /**
     * FIND AND REPLACE START
     * @param scilabDocument document
     * @param word string
     * @param start int
     * @param end int
     * @param caseSensitive boolean
     * @param wholeWord boolean
     * @param useRegexp boolean
     * @return List
     */
    public static List<Integer[]> findToken(ScilabDocument scilabDocument, int token, ScilabLexer lexer, Pattern pat) {
        String fullText = scilabDocument.getText();

        if (fullText.length() == 0) {
            return null;
        }

        List<Integer[]> offsetList = new ArrayList<Integer[]>();

        //If we don't give any word to find
        Matcher matcher = pat.matcher(fullText);

        while (matcher.find()) {
            int start = matcher.start();
            int end = matcher.end();
            if (token == -1 || token == lexer.getKeyword(start, false)) {
                offsetList.add(new Integer[] {start, end});
            }
        }

        return offsetList;
    }

    /**
     * Generate the good pattern according to the differents boolean
     * @param exp the searched expression
     * @param caseSensitive boolean
     * @param wholeWord boolean
     * @param useRegexp boolean
     * @return the pattern
     */
    public static Pattern generatePattern(String exp, boolean caseSensitive, boolean wholeWord, boolean useRegexp) {
        String word = exp;
        if (word != null && !word.equals("")) {
            if (!useRegexp) {
                word = word.replace("\\E", "\\E\\\\E\\Q");
                word = "\\Q" + word + "\\E";
                if (wholeWord && exp.matches("\\b.*\\b")) {
                    word = "\\b" + word + "\\b";
                }
            }

            if (!caseSensitive) {
                word = "(?i)" + word;
            }

            if (useRegexp) {
                word = "(?m)" + word;
            }

            return Pattern.compile(word);
        } else {
            return Pattern.compile("");
        }
    }

    /**
     * Search a word (with a pattern) in files selected according to their name.
     * @param base the base directory
     * @param recursive, if true then a recursive search is made
     * @param ignoreCR, if true then the read file is considered as one line and regex pattern can include \n
     * @param filePattern the pattern to use to select the files. * is equivalent to .* and ? to .?
     * @param fileCaseSensitive, if true then the file pattern is case sensitive
     * @param wordPattern the pattern of the word to search
     * @param wordCaseSensitive, if true then the word pattern is case sensitive
     * @param wholeWord, if true only whole word will be matched, e.g. in "foobar foo bar", if the pattern is "foo", then only the second "foo" will be matched
     * @param regexp, if true the word pattern is considered as a regex
     * @return infos with the matching positions
     */
    public static MatchingPositions searchInFiles(final BackgroundSearch bgs, String base, final boolean recursive, final boolean ignoreCR,
            String filePattern, boolean fileCaseSensitive,
            String wordPattern, boolean wordCaseSensitive, boolean wholeWord, boolean regexp) {
        final File dir = new File(base);
        Pattern word = null;
        if (wordPattern != null && wordPattern.length() != 0) {
            word = generatePattern(wordPattern, wordCaseSensitive, wholeWord, regexp);
        }
        filePattern = filePattern.replace(".", "\\.").replace("*", ".*").replace("?", ".?");
        final Pattern file = generatePattern(filePattern, fileCaseSensitive, false, true);

        final boolean[] killed = new boolean[] {false};
        if (bgs == null) {
            return searchInFiles(killed, dir, recursive, ignoreCR, file, word);
        } else {
            final Pattern fword = word;
            SwingWorker worker = new SwingWorker<Object, Object>() {
                @Override
                public Object doInBackground() {
                    long begin = System.currentTimeMillis();
                    bgs.setResults(searchInFiles(killed, dir, recursive, ignoreCR, file, fword));
                    long end = System.currentTimeMillis();
                    bgs.setElapsedTime(end - begin);
                    return null;
                }

                @Override
                public void done() {
                    bgs.done();
                }
            };
            bgs.setKilled(killed);
            bgs.setSwingWorker(worker);
            worker.execute();
            return null;
        }
    }

    /**
     * Search a word (with a pattern) in files selected according to their name.
     * @param base the base directory
     * @param recursive, if true then a recursive search is made
     * @param ignoreCR, if true then the read file is considered as one line and regex pattern can include \n
     * @param filePattern the pattern to use to select the files. * is equivalent to .* and ? to .?
     * @param word the pattern of the word to search
     * @param killed a boolean array with more than one element. It is used as a reference on a boolean set to true if the process is killed.
     * @return infos with the matching positions
     */
    public static MatchingPositions searchInFiles(boolean[] killed, File base, boolean recursive, boolean ignoreCR, final Pattern file, final Pattern word) {
        MatchingPositions pos = null;
        if (base.exists() && base.isDirectory() && base.canRead() && !killed[0]) {
            List<MatchingPositions> list = new ArrayList<MatchingPositions>();
            pos = new MatchingPositions(base.getAbsolutePath(), list);
            int occurences = 0;
            File[] files = base.listFiles(new FilenameFilter() {
                @Override
                public boolean accept(File dir, String name) {
                    File f = new File(dir, name);
                    return f.isFile() && f.canRead() && file.matcher(name).matches();
                }
            });
            Arrays.sort(files);

            if (word != null) {
                for (int i = 0; i < files.length && !killed[0]; i++) {
                    MatchingPositions wpos;
                    if (!ignoreCR) {
                        wpos = searchWordInFile(files[i], word);
                    } else {
                        wpos = searchWordInFileIgnoringCR(files[i], word);
                    }
                    if (wpos != null && wpos.getOccurences() != 0) {
                        list.add(wpos);
                        occurences += wpos.getOccurences();
                    }
                }
            } else {
                for (int i = 0; i < files.length && !killed[0]; i++) {
                    list.add(new MatchingPositions(files[i].getAbsolutePath()));
                }
                occurences += files.length;
            }

            if (recursive) {
                files = base.listFiles(new FilenameFilter() {
                    @Override
                    public boolean accept(File dir, String name) {
                        File d = new File(dir, name);
                        return d.isDirectory() && d.canRead();
                    }
                });
                Arrays.sort(files);

                for (int i = 0; i < files.length && !killed[0]; i++) {
                    MatchingPositions rpos = searchInFiles(killed, files[i], true, ignoreCR, file, word);
                    if (rpos != null) {
                        list.add(rpos);
                        occurences += rpos.getOccurences();
                    }
                }
            }

            pos.setOccurences(occurences);

            if (list.isEmpty()) {
                return null;
            }
        }

        return pos;
    }

    /**
     * Search a word (with a pattern) in a file. The search is made line by line.
     * @param file the file where to search
     * @param pat the pattern of the word to search
     * @return infos with the matching positions
     */
    public static MatchingPositions searchWordInFile(File f, Pattern pat) {
        if (f.exists() && f.canRead()) {
            MatchingPositions pos = new MatchingPositions(f.getAbsolutePath());
            String charset;
            try {
                charset = ScilabEditorKit.tryToGuessEncoding(f).name();
            } catch (Exception e) {
                charset = Charset.defaultCharset().name();
            }

            try {
                Scanner scanner = new Scanner(f, charset);
                int occ = 0;
                int line = 0;
                while (scanner.hasNextLine()) {
                    line++;
                    String str = scanner.nextLine();
                    Matcher matcher = pat.matcher(str);
                    int socc = occ;
                    while (matcher.find()) {
                        occ++;
                    }
                    if (occ != socc) {
                        pos.addLine(line, str, pat);
                    }
                }
                scanner.close();

                pos.setOccurences(occ);
                return pos;
            } catch (Exception e) { }
        }

        return null;
    }

    /**
     * Search a word (with a pattern) in a file. The file content is considered as one line (useful to search "...\n...")
     * @param file the file where to search
     * @param pat the pattern of the word to search
     * @return infos with the matching positions
     */
    public static MatchingPositions searchWordInFileIgnoringCR(File f, Pattern pat) {
        if (f.exists() && f.canRead()) {
            MatchingPositions pos = new MatchingPositions(f.getAbsolutePath());
            String charset;
            try {
                charset = ScilabEditorKit.tryToGuessEncoding(f).name();
            } catch (Exception e) {
                charset = Charset.defaultCharset().name();
            }

            try {
                Scanner scanner = new Scanner(f, charset);
                int occ = 0;
                while (scanner.findWithinHorizon(pat, 0) != null) {
                    occ++;
                }
                pos.setOccurences(occ);
                scanner.close();

                return pos;
            } catch (Exception e) { }
        }

        return null;
    }

    /**
     * Count the file having a name corresponding to a pattern
     * @param base the base directory
     * @param pat the file name pattern
     * @return the number of files
     */
    public static int countFiles(File base, Pattern pat) {
        if (!base.isDirectory() || !base.canRead()) {
            return -1;
        }

        int[] count = new int[] {0};
        countFiles(base, pat, count);

        return count[0];
    }

    /**
     * Count files in a recursive way
     */
    private static void countFiles(File base, final Pattern pat, final int[] count) {
        File[] files = base.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File dir, String name) {
                File f = new File(dir, name);
                if (f.isFile() && f.canRead() && pat.matcher(name).matches()) {
                    count[0]++;
                } else if (f.isDirectory() && f.canRead()) {
                    countFiles(f, pat, count);
                }
                return false;
            }
        });
    }

    /**
     * @param file the file to test
     * @return true if it is a binary file
     */
    public static boolean isBinaryFile(File f) {
        try {
            BufferedReader reader = new BufferedReader(new FileReader(f));
            char[] buffer = new char[8192];
            int len = reader.read(buffer, 0, 8192);
            reader.close();
            int i = 0;
            if (len != -1) {
                for (; i < len && buffer[i] != '\0'; i++) {
                    ;
                }
            }

            return len != -1 && i != len;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }

    private static void indent(BufferedWriter buffer, int level) throws IOException {
        for (int i = 0; i < level; i++) {
            buffer.append("    ");
        }
    }

    /**
     * MatchingPositions: inner class to store the results of a search in a file or in a directory
     */
    public static class MatchingPositions implements Iconable {

        private final String file;
        private boolean isRoot;
        private Icon icon;
        private int occurences;
        private List<MatchingPositions> children;
        private final List<Line> lines = new ArrayList<Line>();

        /**
         * Constructor
         * @param file the file where to search is made
         * @param children the list of the file in the directory
         */
        public MatchingPositions(String file, List<MatchingPositions> children) {
            this.file = file;
            this.children = children;
            setIcon();
        }

        /**
         * Constructor
         * @param file the file where to search is made
         */
        public MatchingPositions(String file) {
            this(file, null);
        }

        /**
         * @param root the xml Element representing a MatchingPositions
         */
        public MatchingPositions(Element root) {
            this.file = root.getAttribute("file");
            this.isRoot = Boolean.parseBoolean(root.getAttribute("isRoot"));
            this.occurences = Integer.parseInt(root.getAttribute("occurences"));
            if (root.hasChildNodes()) {
                NodeList nodeList = root.getChildNodes();
                Element e = (Element) nodeList.item(1);
                if (e.getTagName().equals("Position")) {
                    this.children = new ArrayList<MatchingPositions>();
                    for (int i = 0; i < nodeList.getLength(); i++) {
                        if (nodeList.item(i) instanceof Element) {
                            e = (Element) nodeList.item(i);
                            this.children.add(new MatchingPositions(e));
                        }
                    }
                } else {
                    for (int i = 0; i < nodeList.getLength(); i++) {
                        if (nodeList.item(i) instanceof Element) {
                            e = (Element) nodeList.item(i);
                            this.lines.add(new Line(e));
                        }
                    }
                }
            }
            setIcon();
        }

        private void setIcon() {
            if (children != null) {
                this.icon = FOLDERIMAGE;
            } else if (file.endsWith(".sce") || file.endsWith(".sci")) {
                this.icon = SCILABFILEIMAGE;
            } else {
                this.icon = FILEIMAGE;
            }
        }

        /**
         * Set this as the root directory
         */
        public void setRoot() {
            isRoot = true;
        }

        /**
         * Set the number of matches in the file or in the directory
         * @param occ the number of matches
         */
        public void setOccurences(int occ) {
            occurences = occ;
        }

        /**
         * @return the number of matches
         */
        public int getOccurences() {
            return occurences;
        }

        /**
         * Add a line where the searched word has been found
         * @param number the line number
         * @param line the line content
         * @param pat the pattern used
         */
        public void addLine(int number, String line, Pattern pat) {
            this.lines.add(new Line(number, line, pat));
        }

        /**
         * @return the file name
         */
        public String getFileName() {
            return file;
        }

        /**
         * @return true if we are in a directory
         */
        public boolean isDirectory() {
            return children != null;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public Icon getIcon() {
            return icon;
        }

        /**
         * @return true if the file have matching lines
         */
        public boolean hasLines() {
            return !lines.isEmpty();
        }

        /**
         * @return the file present in this directory
         */
        public List<MatchingPositions> getChildren() {
            return children;
        }

        /**
         * Convert this MatchingPositions and its children (if they are) in a DefaultMutableTreeNode
         * @return the coirresponding DefaultMutableTreeNode
         */
        public DefaultMutableTreeNode toDefaultMutableTreeNode() {
            DefaultMutableTreeNode root = new DefaultMutableTreeNode(this);
            if (children != null && !children.isEmpty()) {
                for (int i = 0; i < children.size(); i++) {
                    root.add(children.get(i).toDefaultMutableTreeNode());
                }
            } else if (!lines.isEmpty()) {
                for (Line l : lines) {
                    root.add(new DefaultMutableTreeNode(l));
                }
            }

            return root;
        }

        /**
         * @param buffer the buffer where to write the XML
         * @param level the indentation level
         */
        public void toXML(BufferedWriter buffer, int level) throws IOException {
            indent(buffer, level);
            buffer.append("<Position file=\"" + ScilabXMLUtilities.getXMLString(file) + "\" isRoot=\"" + isRoot + "\" occurences=\"" + occurences + "\"");
            if (children != null && !children.isEmpty()) {
                buffer.append(">\n");
                for (int i = 0; i < children.size(); i++) {
                    children.get(i).toXML(buffer, level + 1);
                }
                indent(buffer, level);
                buffer.append("</Position>\n");
            } else if (!lines.isEmpty()) {
                buffer.append(">\n");
                for (Line l : lines) {
                    l.toXML(buffer, level + 1);
                }
                indent(buffer, level);
                buffer.append("</Position>\n");
            } else {
                buffer.append("/>\n");
            }
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public String toString() {
            String occ = SciNotesMessages.MATCHES;
            if (occurences <= 1) {
                occ = SciNotesMessages.MATCH;
            }

            String filename;
            if ((!isDirectory() && occurences != 0)) {
                filename = new File(getFileName()).getName();
            } else if (isRoot) {
                filename = getFileName();
            } else {
                filename = new File(getFileName()).getName();
            }

            if (occurences == 0 && !isRoot) {
                return filename;
            }

            filename = filename.replace("&", "&amp;").replace("/", "&#47;").replace("\\", "&#92;").replace("<", "&lt;").replace(">", "&gt;");
            return String.format(occ, filename, occurences);
        }
    }

    /**
     * Line: inner class to store a line number and line content
     */
    public static class Line implements Iconable {

        private final int number;
        private String content;

        /**
         * Constructor
         * @param number the line number
         * @param content the line content
         * @param pattern the used pattern
         */
        public Line(int number, String content, Pattern pattern) {
            this.number = number;
            Matcher matcher = pattern.matcher(content);
            if (content.length() > 128) {
                content = content.substring(0, 128) + "...";
            }
            StringBuffer sb = new StringBuffer();
            while (matcher.find()) {
                /*
                  TODO: Find a better way to handle <b> and </b> around the pattern.
                  If HTML entities are put before the loop the pattern should be updated (not a funky task...).
                  Actually, it could have a bad rendering on binary files (which probably contains \0...)
                */
                matcher.appendReplacement(sb, "\0");
                sb.append(matcher.group());
                sb.append("\0\0");
            }
            matcher.appendTail(sb);
            this.content = sb.toString();
            this.content = this.content.replace("&", "&amp;").replace("/", "&#47;").replace("\\", "&#92;").replace("<", "&lt;").replace(">", "&gt;").replace("\0\0", "</b>").replace("\0", "<b>");
        }

        /**
         * @param root the xml Element representing a Line
         */
        public Line(Element root) {
            this.number = Integer.parseInt(root.getAttribute("number"));
            this.content = root.getAttribute("content");
        }

        /**
         * @return the line number
         */
        public int getNumber() {
            return number;
        }

        /**
         * @return the line content as HTML
         */
        public String getContent() {
            return content;
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public Icon getIcon() {
            return LINEICON;
        }

        /**
         * @param buffer the buffer where to write the XML
         * @param level the indentation level
         */
        public void toXML(BufferedWriter buffer, int level) throws IOException {
            indent(buffer, level);
            buffer.append("<Line content=\"" + ScilabXMLUtilities.getXMLString(content) + "\" number=\"" + number + "\"/>\n");
        }

        /**
         * {@inheritDoc}
         */
        @Override
        public String toString() {
            return "<html><u>line " + number + "</u>&thinsp;: " + content + "</html>";
        }
    }

    /**
     * Inner interface for the JTree representation
     */
    public static interface Iconable {

        /**
         * @return the icon used in the JTree representation
         */
        public Icon getIcon();
    }

    /**
     * Inner class to allow a background search
     */
    public static abstract class BackgroundSearch {

        private MatchingPositions pos;
        private SwingWorker worker;
        private long time;
        private boolean[] killed;

        /**
         * Default Constructor
         */
        public BackgroundSearch() { }

        /**
         * Stop this search
         */
        public void stop() {
            if (worker != null && !isDone()) {
                worker.cancel(true);
                worker = null;
                if (killed != null && killed.length >= 1) {
                    killed[0] = true;
                }
            }
        }

        /**
         * Called when the work is done
         */
        public abstract void done();

        /**
         * @return true if the search is finished
         */
        public boolean isDone() {
            if (worker != null) {
                return worker.isDone();
            }
            return true;
        }

        /**
         * Get the results
         * @return the results
         */
        public MatchingPositions getResults() {
            if (isDone()) {
                worker = null;
                return pos;
            }
            return null;
        }

        /**
         * Set the elapsed time for this search
         */
        public void setElapsedTime(long t) {
            this.time = t;
        }

        /**
         * @return the elapsed time of this search
         */
        public long getElapsedTime() {
            return time;
        }

        /**
         * Set the SwingWorker we work with
         * @param worker the SwingWorker
         */
        private void setSwingWorker(SwingWorker worker) {
            this.worker = worker;
        }

        /**
         * @param killed a reference on a boolean to inform the main loop that the process has been killed
         */
        private void setKilled(boolean[] killed) {
            this.killed = killed;
        }

        /**
         * @param pos the results to set
         */
        private void setResults(MatchingPositions pos) {
            this.pos = pos;
        }
    }
}
