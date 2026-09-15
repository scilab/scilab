/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 */

package org.scilab.modules.gui.bridge.helpbrowser;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * In-memory full-text index of help pages (DocBook XML) for the search field
 * of the help browser.
 *
 * Each page keeps its xml:ids, names (refname), purpose (refpurpose) and the
 * plain text of the rest of the page, lower-cased. A query is split into
 * terms: blank separated words, or "quoted phrases". A page matches when it
 * contains every term, anywhere (id, name, purpose or body, inside a longer
 * word too). Pages are ranked by where the terms are found: page id, then
 * name, then purpose, then body; whole words above occurrences inside a
 * word; then the number of occurrences in the body.
 */
final class HelpSearchIndex {

    /** A page to index: its path as written in the TOC and the XML file to read. */
    static final class Source {
        final String path;
        final File file;

        Source(String path, File file) {
            this.path = path;
            this.file = file;
        }
    }

    private static final Pattern XML_ID = Pattern.compile("xml:id\\s*=\\s*\"([^\"]*)\"");
    private static final Pattern REFNAME = Pattern.compile("<refname>(.*?)</refname>", Pattern.DOTALL);
    private static final Pattern REFPURPOSE = Pattern.compile("<refpurpose>(.*?)</refpurpose>", Pattern.DOTALL);
    private static final Pattern QUERY_TERM = Pattern.compile("\"([^\"]*)\"?|(\\S+)");

    /** Occurrences counted per term in a text: bounds the cost of common words. */
    private static final int MAX_COUNT = 30;
    private static final int SNIPPET_BEFORE = 60;
    private static final int SNIPPET_AFTER = 120;

    private final List<Page> pages;

    HelpSearchIndex(List<Source> sources) {
        pages = new ArrayList<>(sources.size());
        for (Source s : sources) {
            try {
                pages.add(new Page(s.path, s.file, read(s.file)));
            } catch (IOException e) {
                // unreadable page: not searchable
            }
        }
    }

    int size() {
        return pages.size();
    }

    /**
     * Search several indexes at once (native chapters, toolboxes).
     *
     * @return JSON {"total": number of matching pages, "terms": [the query
     *         terms, lower-cased], "results": [the max best pages: id, title,
     *         purpose, snippet, path]}
     */
    static String search(List<HelpSearchIndex> indexes, String query, int max) {
        Query q = new Query(query);
        List<Hit> hits = new ArrayList<>();
        if (!q.terms.isEmpty()) {
            for (HelpSearchIndex index : indexes) {
                for (Page p : index.pages) {
                    int score = p.score(q);
                    if (score > 0) {
                        hits.add(new Hit(p, score));
                    }
                }
            }
        }
        hits.sort((a, b) -> a.score != b.score
                  ? Integer.compare(b.score, a.score)
                  : a.page.title.compareToIgnoreCase(b.page.title));

        StringBuilder json = new StringBuilder("{\"total\":").append(hits.size()).append(",\"terms\":[");
        for (int i = 0; i < q.terms.size(); i++) {
            if (i > 0) {
                json.append(',');
            }
            json.append(quote(q.terms.get(i)));
        }
        json.append("],\"results\":[");
        for (int i = 0; i < Math.min(max, hits.size()); i++) {
            Page p = hits.get(i).page;
            if (i > 0) {
                json.append(',');
            }
            json.append("{\"id\":").append(quote(p.id))
                .append(",\"title\":").append(quote(p.title))
                .append(",\"purpose\":").append(quote(p.purpose))
                .append(",\"snippet\":").append(quote(p.snippet(q.terms)))
                .append(",\"path\":").append(quote(p.path))
                .append('}');
        }
        return json.append("]}").toString();
    }

    private static final class Hit {
        final Page page;
        final int score;

        Hit(Page page, int score) {
            this.page = page;
            this.score = score;
        }
    }

    private static final class Query {
        final List<String> terms = new ArrayList<>();
        /** Several unquoted terms joined: bonus for the pages where they follow each other. */
        final String phrase;

        Query(String query) {
            String q = query.toLowerCase(Locale.ROOT);
            Matcher m = QUERY_TERM.matcher(q);
            while (m.find()) {
                String t = (m.group(1) != null ? m.group(1) : m.group(2)).replaceAll("\\s+", " ").trim();
                if (!t.isEmpty() && !terms.contains(t)) {
                    terms.add(t);
                }
            }
            phrase = (q.indexOf('"') < 0 && terms.size() > 1) ? String.join(" ", terms) : null;
        }
    }

    private static final class Page {
        final String path;
        final File file;
        final String id;
        final String title;
        final String purpose;
        /** Lower-cased xml:ids, the page id first (the others are link targets). */
        final String[] ids;
        /** Lower-cased refnames (functions documented together). */
        final String[] names;
        final String purposeLower;
        final String bodyLower;

        Page(String path, File file, String xml) {
            this.path = path;
            this.file = file;
            String baseName = file.getName().replaceFirst("\\.xml$", "");

            List<String> idList = new ArrayList<>();
            Matcher m = XML_ID.matcher(xml);
            while (m.find()) {
                if (!idList.contains(m.group(1))) {
                    idList.add(m.group(1));
                }
            }
            if (idList.isEmpty()) {
                idList.add(baseName);
            }
            id = idList.get(0);
            ids = lower(idList);

            List<String> nameList = new ArrayList<>();
            m = REFNAME.matcher(xml);
            while (m.find()) {
                nameList.add(plainText(m.group(1)));
            }
            title = nameList.isEmpty() ? baseName : nameList.get(0);
            names = lower(nameList);

            m = REFPURPOSE.matcher(xml);
            purpose = m.find() ? plainText(m.group(1)) : "";
            purposeLower = purpose.toLowerCase(Locale.ROOT);
            bodyLower = bodyText(xml).toLowerCase(Locale.ROOT);
        }

        /** 0 when a term is missing. */
        int score(Query q) {
            int total = 0;
            for (String t : q.terms) {
                int s = termScore(t);
                if (s == 0) {
                    return 0;
                }
                total += s;
            }
            if (q.phrase != null) {
                if (purposeLower.contains(q.phrase)) {
                    total += 100;
                } else if (bodyLower.contains(q.phrase)) {
                    total += 50;
                }
            }
            return total;
        }

        private int termScore(String t) {
            // Identity: the best match among the ids and names
            int s = 0;
            for (int k = 0; k < ids.length; k++) {
                s = Math.max(s, identityScore(ids[k], t, k == 0 ? 1000 : 700));
            }
            for (String n : names) {
                s = Math.max(s, identityScore(n, t, 900));
            }

            int[] c = count(purposeLower, t);
            if (c[0] > 0) {
                s += 150;
            } else if (c[1] > 0) {
                s += 40;
            }

            c = count(bodyLower, t);
            if (c[0] > 0) {
                s += 20 + 5 * c[0];
            }
            if (c[1] > 0) {
                s += 5 + c[1];
            }
            return s;
        }

        /**
         * Text around the first occurrence of a term in the body (whole
         * words first), in its original case; empty when the terms are only
         * in the name or purpose. The page is read again: only the displayed
         * results need it.
         */
        String snippet(List<String> terms) {
            String body;
            try {
                body = bodyText(read(file));
            } catch (IOException e) {
                return "";
            }
            int pos = -1;
            int len = 0;
            for (int pass = 0; pass < 2 && pos < 0; pass++) {
                for (String t : terms) {
                    int i = find(body, t, pass == 0);
                    if (i >= 0 && (pos < 0 || i < pos)) {
                        pos = i;
                        len = t.length();
                    }
                }
            }
            if (pos < 0) {
                return "";
            }
            // Cut on blanks, not inside words
            int start = Math.max(0, pos - SNIPPET_BEFORE);
            if (start > 0) {
                int sp = body.indexOf(' ', start);
                if (sp >= 0 && sp < pos) {
                    start = sp + 1;
                }
            }
            int end = Math.min(body.length(), pos + len + SNIPPET_AFTER);
            if (end < body.length()) {
                int sp = body.lastIndexOf(' ', end);
                if (sp > pos + len) {
                    end = sp;
                }
            }
            return (start > 0 ? "\u2026" : "") + body.substring(start, end) + (end < body.length() ? "\u2026" : "");
        }
    }

    private static int identityScore(String name, String t, int exact) {
        if (name.equals(t)) {
            return exact;
        }
        if (name.startsWith(t)) {
            return 300;
        }
        int i = name.indexOf(t);
        if (i < 0) {
            return 0;
        }
        return isWholeAt(name, i, t) ? 250 : 80;
    }

    /** {whole-word occurrences, occurrences inside a word}, MAX_COUNT at most. */
    private static int[] count(String text, String t) {
        int whole = 0;
        int partial = 0;
        int i = text.indexOf(t);
        while (i >= 0 && whole + partial < MAX_COUNT) {
            if (isWholeAt(text, i, t)) {
                whole++;
            } else {
                partial++;
            }
            i = text.indexOf(t, i + t.length());
        }
        return new int[] {whole, partial};
    }

    /** Case-insensitive search in the original text (for the snippets). */
    private static int find(String text, String t, boolean wholeWord) {
        for (int i = 0, n = text.length() - t.length(); i <= n; i++) {
            if (text.regionMatches(true, i, t, 0, t.length()) && (!wholeWord || isWholeAt(text, i, t))) {
                return i;
            }
        }
        return -1;
    }

    /** Whether t found at i in text is not part of a longer word ("table" in "unstable"). */
    private static boolean isWholeAt(String text, int i, String t) {
        int j = i + t.length();
        return (i == 0 || !isWordChar(text.charAt(i - 1)) || !isWordChar(t.charAt(0)))
               && (j >= text.length() || !isWordChar(text.charAt(j)) || !isWordChar(t.charAt(t.length() - 1)));
    }

    /** Characters of a Scilab identifier (%pi, my_var). */
    private static boolean isWordChar(char c) {
        return Character.isLetterOrDigit(c) || c == '_' || c == '%';
    }

    private static String[] lower(List<String> list) {
        String[] out = new String[list.size()];
        for (int i = 0; i < out.length; i++) {
            out[i] = list.get(i).toLowerCase(Locale.ROOT);
        }
        return out;
    }

    private static String read(File f) throws IOException {
        return new String(Files.readAllBytes(f.toPath()), StandardCharsets.UTF_8);
    }

    /** Plain text of a page without its refnamediv (name and purpose, indexed apart). */
    private static String bodyText(String xml) {
        int a = xml.indexOf("<refnamediv");
        int b = xml.indexOf("</refnamediv>");
        if (a >= 0 && b > a) {
            return plainText(xml.substring(0, a) + " " + xml.substring(b + "</refnamediv>".length()));
        }
        return plainText(xml);
    }

    /**
     * Text of an XML fragment: tags and comments dropped (a tag separates
     * words), CDATA sections kept verbatim (code examples contain '<'),
     * character entities decoded, blanks collapsed to single spaces.
     */
    static String plainText(String xml) {
        StringBuilder sb = new StringBuilder(xml.length() / 2);
        int n = xml.length();
        int i = 0;
        while (i < n) {
            char c = xml.charAt(i);
            if (c == '<') {
                if (xml.startsWith("<![CDATA[", i)) {
                    int e = xml.indexOf("]]>", i + 9);
                    int end = e < 0 ? n : e;
                    for (int k = i + 9; k < end; k++) {
                        put(sb, xml.charAt(k));
                    }
                    i = e < 0 ? n : e + 3;
                } else if (xml.startsWith("<!--", i)) {
                    int e = xml.indexOf("-->", i + 4);
                    i = e < 0 ? n : e + 3;
                } else {
                    int e = xml.indexOf('>', i + 1);
                    i = e < 0 ? n : e + 1;
                }
                put(sb, ' ');
                continue;
            }
            if (c == '&') {
                int semi = xml.indexOf(';', i + 1);
                if (semi > i + 1 && semi - i <= 10) {
                    int cp = entity(xml.substring(i + 1, semi));
                    if (cp >= 0) {
                        for (char ch : Character.toChars(cp)) {
                            put(sb, ch);
                        }
                        i = semi + 1;
                        continue;
                    }
                }
            }
            put(sb, c);
            i++;
        }
        int len = sb.length();
        if (len > 0 && sb.charAt(len - 1) == ' ') {
            sb.setLength(len - 1);
        }
        return sb.toString();
    }

    /** Appends c, blanks collapsed (none at the start). */
    private static void put(StringBuilder sb, char c) {
        if (c == '\u00A0' || Character.isWhitespace(c)) {
            int len = sb.length();
            if (len > 0 && sb.charAt(len - 1) != ' ') {
                sb.append(' ');
            }
        } else {
            sb.append(c);
        }
    }

    /** Code point of a character entity name ("lt", "#233", "#xE9"), -1 if unknown. */
    private static int entity(String name) {
        switch (name) {
            case "lt":
                return '<';
            case "gt":
                return '>';
            case "amp":
                return '&';
            case "quot":
                return '"';
            case "apos":
                return '\'';
            case "nbsp":
                return ' ';
            default:
                break;
        }
        if (name.startsWith("#")) {
            try {
                int cp = (name.startsWith("#x") || name.startsWith("#X"))
                         ? Integer.parseInt(name.substring(2), 16)
                         : Integer.parseInt(name.substring(1));
                return Character.isValidCodePoint(cp) ? cp : -1;
            } catch (NumberFormatException e) {
                return -1;
            }
        }
        return -1;
    }

    /** JSON string literal. */
    private static String quote(String s) {
        StringBuilder sb = new StringBuilder(s.length() + 2).append('"');
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '"':
                    sb.append("\\\"");
                    break;
                case '\\':
                    sb.append("\\\\");
                    break;
                default:
                    if (c < 0x20 || c == '\u2028' || c == '\u2029') {
                        sb.append(String.format("\\u%04x", (int) c));
                    } else {
                        sb.append(c);
                    }
            }
        }
        return sb.append('"').toString();
    }
}
