package org.scilab.modules.scinotes.assistant;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;
import java.util.stream.Stream;

import org.scilab.modules.commons.ScilabConstants;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

/**
 * Index and search Scilab's XML DocBook help files.
 *
 * Scans {SCI}/modules/{mod}/help/en_US/{func}.xml, builds an in-memory index,
 * and provides scored search + full help rendering to markdown.
 */
public final class ScilabHelpIndex {

    private static final Logger LOG = Logger.getLogger(ScilabHelpIndex.class.getName());

    private static final String NS_DOCBOOK = "http://docbook.org/ns/docbook";
    private static final Pattern COMPAT_PREFIX =
        Pattern.compile("^(mtlb_|mblt_|m2sci_|mfile2sci_).*");
    private static final Pattern TOKEN_SPLIT = Pattern.compile("[\\s,_\\-:]+");

    /** Single index entry. */
    static class Entry {
        final String name;
        final String xmlPath;
        String purpose = "";
        String syntax = "";
        String seeAlso = "";
        String keywords = "";
        boolean isTest = false;
        String testContent = "";   // raw .tst content (for test entries)
        String moduleName = "";    // parent module name

        Entry(String name, String xmlPath) {
            this.name = name;
            this.xmlPath = xmlPath;
        }
    }

    /** Search result with score. */
    public static class SearchResult {
        public final String name;
        public final String purpose;
        public final String syntax;
        public final int score;

        SearchResult(String name, String purpose, String syntax, int score) {
            this.name = name;
            this.purpose = purpose;
            this.syntax = syntax;
            this.score = score;
        }
    }

    // -- Singleton --

    private static ScilabHelpIndex instance;
    private final Map<String, Entry> index = new HashMap<>();
    private volatile boolean built = false;

    private ScilabHelpIndex() {}

    public static synchronized ScilabHelpIndex getInstance() {
        if (instance == null) {
            instance = new ScilabHelpIndex();
        }
        return instance;
    }

    public boolean isBuilt() { return built; }
    public int size() { return index.size(); }

    // -- Index building --

    /**
     * Build the index by scanning SCI/modules/{module}/help/en_US/{func}.xml.
     * @param sciPath the SCI root directory
     */
    public synchronized void buildIndex(String sciPath) {
        if (built) return;

        long t0 = System.currentTimeMillis();
        Path modulesDir = Paths.get(sciPath, "modules");
        if (!Files.isDirectory(modulesDir)) {
            LOG.warning("Modules dir not found: " + modulesDir);
            return;
        }

        try (Stream<Path> modules = Files.list(modulesDir)) {
            modules.filter(Files::isDirectory).forEach(modDir -> {
                Path helpDir = modDir.resolve("help").resolve("en_US");
                if (Files.isDirectory(helpDir)) {
                    scanHelpDir(helpDir);
                }
            });
        } catch (IOException e) {
            LOG.log(Level.WARNING, "Error scanning modules", e);
        }

        int helpCount = index.size();

        // Scan test files (unit_tests + nonreg_tests)
        try (Stream<Path> modules = Files.list(modulesDir)) {
            modules.filter(Files::isDirectory).forEach(modDir -> {
                String moduleName = modDir.getFileName().toString();
                Path unitTests = modDir.resolve("tests").resolve("unit_tests");
                if (Files.isDirectory(unitTests)) {
                    scanTestDir(unitTests, moduleName);
                }
                Path nonregTests = modDir.resolve("tests").resolve("nonreg_tests");
                if (Files.isDirectory(nonregTests)) {
                    scanTestDir(nonregTests, moduleName);
                }
            });
        } catch (IOException e) {
            LOG.log(Level.WARNING, "Error scanning test files", e);
        }

        built = true;
        LOG.info("Help index built: " + helpCount + " help entries + "
                 + (index.size() - helpCount) + " test entries in "
                 + (System.currentTimeMillis() - t0) + "ms");
    }

    private void scanHelpDir(Path helpDir) {
        try (Stream<Path> files = Files.walk(helpDir)) {
            files.filter(p -> p.toString().endsWith(".xml")).forEach(xmlPath -> {
                String stem = xmlPath.getFileName().toString();
                stem = stem.substring(0, stem.length() - 4); // remove .xml

                if (COMPAT_PREFIX.matcher(stem).matches()) return;

                Entry entry = new Entry(stem, xmlPath.toString());
                try {
                    extractMetadata(entry, xmlPath);
                } catch (Exception e) {
                    // Skip malformed XML
                }
                index.put(stem.toLowerCase(), entry);
            });
        } catch (IOException e) {
            // Skip unreadable dirs
        }
    }

    private void scanTestDir(Path testDir, String moduleName) {
        try (Stream<Path> files = Files.list(testDir)) {
            files.filter(p -> p.toString().endsWith(".tst")).forEach(tstPath -> {
                String stem = tstPath.getFileName().toString();
                stem = stem.substring(0, stem.length() - 4); // remove .tst

                // Use a test-specific key to avoid overwriting help entries
                String key = "test:" + stem.toLowerCase();

                // Don't overwrite if already indexed (help entries take priority)
                if (index.containsKey(key)) return;

                try {
                    String content = new String(Files.readAllBytes(tstPath),
                                                java.nio.charset.StandardCharsets.UTF_8);
                    // Limit content to 4KB per test to avoid huge memory usage
                    if (content.length() > 4096) {
                        content = content.substring(0, 4096);
                    }

                    Entry entry = new Entry(stem, tstPath.toString());
                    entry.isTest = true;
                    entry.moduleName = moduleName;
                    entry.testContent = content;

                    // Extract first comment line as purpose
                    for (String line : content.split("\n")) {
                        line = line.trim();
                        if (line.startsWith("//") && !line.startsWith("// ==")
                            && !line.startsWith("// <--") && !line.startsWith("// Copyright")
                            && !line.startsWith("// Scilab")) {
                            entry.purpose = line.substring(2).trim();
                            break;
                        }
                    }

                    // Build keywords from content
                    entry.keywords = truncate(
                        (stem + " " + moduleName + " " + entry.purpose + " " + content).toLowerCase(),
                        500);

                    index.put(key, entry);
                } catch (IOException e) {
                    // Skip unreadable test files
                }
            });
        } catch (IOException e) {
            // Skip unreadable dirs
        }
    }

    private void extractMetadata(Entry entry, Path xmlPath) throws Exception {
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setNamespaceAware(true);
        // Disable DTD loading for speed
        dbf.setFeature("http://apache.org/xml/features/nonvalidating/load-external-dtd", false);
        dbf.setFeature("http://xml.org/sax/features/external-general-entities", false);
        DocumentBuilder db = dbf.newDocumentBuilder();
        Document doc = db.parse(xmlPath.toFile());
        Element root = doc.getDocumentElement();

        // Purpose
        entry.purpose = textOf(findFirst(root, "refpurpose"));

        // Syntax: first programlisting inside refsynopsisdiv
        Element synDiv = findFirst(root, "refsynopsisdiv");
        if (synDiv != null) {
            Element prog = findFirst(synDiv, "programlisting");
            if (prog != null) {
                String syn = textOf(prog);
                // Limit to first 3 lines, 280 chars
                String[] lines = syn.split("\n");
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < Math.min(3, lines.length); i++) {
                    if (sb.length() > 0) sb.append("\n");
                    sb.append(lines[i]);
                }
                entry.syntax = truncate(sb.toString(), 280);
            }
        }

        // See also
        List<Element> sections = findAll(root, "refsection");
        for (Element sec : sections) {
            String role = sec.getAttribute("role");
            String title = textOfFirst(sec, "title");
            if ("see also".equalsIgnoreCase(role)
                || (title != null && title.toLowerCase().contains("see also"))) {
                List<String> refs = new ArrayList<>();
                for (Element link : findAll(sec, "link")) {
                    String linkend = link.getAttribute("linkend");
                    if (linkend != null && !linkend.isEmpty()) {
                        refs.add(linkend);
                    } else {
                        String t = textOf(link).trim();
                        if (!t.isEmpty()) refs.add(t);
                    }
                }
                entry.seeAlso = String.join(", ", refs);
                break;
            }
        }

        // Keywords blob
        String blob = (entry.purpose + " " + entry.syntax + " " + entry.seeAlso).toLowerCase();
        entry.keywords = truncate(blob, 500);
    }

    // -- Search --

    /**
     * Search the index with scored ranking.
     * @param query search query
     * @param limit max results
     * @return sorted list of results
     */
    public List<SearchResult> search(String query, int limit) {
        ensureBuilt();
        if (query == null || query.trim().isEmpty()) return Collections.emptyList();

        String pattern = query.trim().toLowerCase();
        String[] tokens = TOKEN_SPLIT.split(pattern);

        List<SearchResult> results = new ArrayList<>();
        for (Entry entry : index.values()) {
            String name = entry.name.toLowerCase();
            int score = 0;

            // Name matching
            if (name.equals(pattern)) {
                score += 120;
            } else if (name.startsWith(pattern)) {
                score += 80;
            } else if (name.contains(pattern)) {
                score += 50;
            }

            // Metadata matching
            String meta = (entry.purpose + " " + entry.syntax + " "
                           + entry.seeAlso + " " + entry.keywords).toLowerCase();
            if (meta.contains(pattern)) {
                score += 20;
            }

            // Token matching
            for (String token : tokens) {
                if (token.isEmpty()) continue;
                if (name.contains(token)) score += 20;
                if (meta.contains(token)) score += 8;
            }

            if (score > 0) {
                results.add(new SearchResult(entry.name, entry.purpose, entry.syntax, score));
            }
        }

        // Sort by score desc, then name asc
        results.sort((a, b) -> {
            int cmp = Integer.compare(b.score, a.score);
            return cmp != 0 ? cmp : a.name.compareToIgnoreCase(b.name);
        });

        return results.subList(0, Math.min(limit, results.size()));
    }

    // -- Full help rendering --

    /**
     * Get full help for a function, rendered as markdown.
     * @param functionName the function name
     * @return markdown string, or null if not found
     */
    public String getHelp(String functionName) {
        ensureBuilt();
        Entry entry = index.get(functionName.toLowerCase());
        // Also try test entries
        if (entry == null) {
            entry = index.get("test:" + functionName.toLowerCase());
        }
        if (entry == null) return null;

        if (entry.isTest) {
            return renderTestHelp(entry);
        }

        try {
            return parseFullHelp(entry);
        } catch (Exception e) {
            LOG.log(Level.WARNING, "Error parsing help for " + functionName, e);
            return "Error reading help: " + e.getMessage();
        }
    }

    private String renderTestHelp(Entry entry) {
        StringBuilder md = new StringBuilder();
        md.append("# Test: ").append(entry.name).append("\n");
        md.append("Module: ").append(entry.moduleName).append("\n\n");
        if (!entry.purpose.isEmpty()) {
            md.append("**").append(entry.purpose).append("**\n\n");
        }
        md.append("```scilab\n").append(entry.testContent).append("\n```\n");
        return md.toString();
    }

    private String parseFullHelp(Entry entry) throws Exception {
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setNamespaceAware(true);
        dbf.setFeature("http://apache.org/xml/features/nonvalidating/load-external-dtd", false);
        dbf.setFeature("http://xml.org/sax/features/external-general-entities", false);
        DocumentBuilder db = dbf.newDocumentBuilder();
        Document doc = db.parse(new File(entry.xmlPath));
        Element root = doc.getDocumentElement();

        StringBuilder md = new StringBuilder();

        // Name + purpose
        String name = textOfFirst(root, "refname");
        if (name == null || name.isEmpty()) name = entry.name;
        String purpose = textOfFirst(root, "refpurpose");
        md.append("# ").append(name).append("\n\n");
        if (purpose != null && !purpose.isEmpty()) {
            md.append("**").append(purpose.trim()).append("**\n\n");
        }

        // Syntax
        Element synDiv = findFirst(root, "refsynopsisdiv");
        if (synDiv != null) {
            Element prog = findFirst(synDiv, "programlisting");
            if (prog != null) {
                md.append("## Syntax\n```scilab\n").append(textOf(prog).trim()).append("\n```\n\n");
            }
        }

        // Sections (arguments, description, examples, see also)
        List<Element> sections = findAll(root, "refsection");
        for (Element sec : sections) {
            String role = sec.getAttribute("role");
            String title = textOfFirst(sec, "title");
            if (title == null) title = role != null ? role : "";
            String titleLower = title.toLowerCase();

            if (titleLower.contains("argument")) {
                md.append("## Arguments\n");
                for (Element varEntry : findAll(sec, "varlistentry")) {
                    String term = textOfFirst(varEntry, "term");
                    String desc = textOfFirst(varEntry, "listitem");
                    if (term != null) {
                        md.append("- **").append(term.trim()).append("**");
                        if (desc != null && !desc.isEmpty()) {
                            md.append(": ").append(desc.trim());
                        }
                        md.append("\n");
                    }
                }
                md.append("\n");

            } else if (titleLower.contains("description")) {
                md.append("## Description\n");
                for (Element para : findAll(sec, "para")) {
                    String text = textOf(para).trim();
                    if (!text.isEmpty()) {
                        md.append(text).append("\n\n");
                    }
                }

            } else if (titleLower.contains("example")) {
                md.append("## Examples\n");
                for (Element prog : findAll(sec, "programlisting")) {
                    md.append("```scilab\n").append(textOf(prog).trim()).append("\n```\n\n");
                }

            } else if ("see also".equalsIgnoreCase(role) || titleLower.contains("see also")) {
                md.append("## See also\n");
                List<String> refs = new ArrayList<>();
                for (Element link : findAll(sec, "link")) {
                    String linkend = link.getAttribute("linkend");
                    String t = (linkend != null && !linkend.isEmpty()) ? linkend : textOf(link).trim();
                    if (!t.isEmpty()) refs.add(t);
                }
                md.append(String.join(", ", refs)).append("\n\n");
            }
        }

        return md.toString();
    }

    // -- XML helpers (namespace-aware with fallback) --

    void ensureBuilt() {
        if (!built) {
            if (ScilabConstants.SCI != null) {
                buildIndex(ScilabConstants.SCI.getAbsolutePath());
            } else {
                LOG.warning("Cannot build help index: ScilabConstants.SCI is null");
            }
        }
    }

    /** Find first element by local name (ignoring namespace). */
    private static Element findFirst(Element parent, String localName) {
        // Try with namespace first
        NodeList nl = parent.getElementsByTagNameNS(NS_DOCBOOK, localName);
        if (nl.getLength() > 0) return (Element) nl.item(0);
        // Fallback without namespace
        nl = parent.getElementsByTagName(localName);
        if (nl.getLength() > 0) return (Element) nl.item(0);
        return null;
    }

    /** Find all elements by local name (ignoring namespace). */
    private static List<Element> findAll(Element parent, String localName) {
        List<Element> result = new ArrayList<>();
        // Try namespace first
        NodeList nl = parent.getElementsByTagNameNS(NS_DOCBOOK, localName);
        if (nl.getLength() == 0) {
            nl = parent.getElementsByTagName(localName);
        }
        for (int i = 0; i < nl.getLength(); i++) {
            Node n = nl.item(i);
            if (n instanceof Element) {
                result.add((Element) n);
            }
        }
        return result;
    }

    /** Get text content of an element, or empty string. */
    private static String textOf(Element el) {
        return el != null ? el.getTextContent() : "";
    }

    /** Get text of the first child element matching localName. */
    private static String textOfFirst(Element parent, String localName) {
        Element el = findFirst(parent, localName);
        return el != null ? el.getTextContent().trim() : null;
    }

    private static String truncate(String s, int max) {
        return s.length() > max ? s.substring(0, max) : s;
    }
}
