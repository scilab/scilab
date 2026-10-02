/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 *
 * JCEF-based Help Browser replacing the old JavaHelp implementation.
 */

package org.scilab.modules.gui.bridge.helpbrowser;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Locale;

import javax.swing.SwingUtilities;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.google.gson.Gson;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;
import org.cef.callback.CefQueryCallback;
import org.cef.handler.CefContextMenuHandlerAdapter;
import org.cef.handler.CefLifeSpanHandlerAdapter;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.cef.misc.BoolRef;
import org.cef.network.CefRequest.TransitionType;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.xml.ScilabXPathFactory;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import org.scilab.modules.gui.helpbrowser.ScilabHelpBrowser;

/**
 * JCEF-based Scilab Help Browser.
 *
 * Replaces the old JavaHelp-based SwingScilabHelpBrowser with a modern
 * Chromium-based browser embedded through a ScilabCefPanel, as the console.
 *
 * Architecture:
 * - Shell HTML (index.html) loaded from SCI/modules/helptools/src/web/
 * - toc.json, generated HTML pages and cached images stored in
 *   TMPDIR/help/&lt;lang&gt;/ (session cache: cheap to rebuild, images are
 *   embedded as data: URIs so the pages are self-contained)
 * - JS<->Java bridge via CefMessageRouter ("helpBrowserQuery"):
 *   - getHomePage / getToc: shell bootstrap
 *   - getScilabKeywords: syntax highlighting keyword lists, produced by
 *     getscilabkeywords() in the interpreter (commands, predefined
 *     variables, primitives, macros incl. loaded toolboxes)
 *   - getSyntaxColors: syntax highlighting colors from the Preferences
 *     (same items as SciNotes, see XConfiguration-colors.xml)
 *   - transformXml: runtime DocBook->HTML via XSLT, including lazy
 *     scilab:image and xcos diagram generation (PNG cache)
 *   - fullTextSearch: ranked search of the pages in an in-memory index
 *     (HelpSearchIndex), built in the background
 *   - execCode / editCode / copyCode / openXcos / openExternal: page actions
 */
public class SwingScilabHelpBrowserJCEF extends SwingScilabHelpBrowser {

    private static final long serialVersionUID = 1L;

    private static final String SCI = System.getenv("SCI");
    private static final String WEB_DIR = SCI + "/modules/helptools/src/web";
    private static final String XSLT_FILE = SCI + "/modules/helptools/etc/browser.xslt";
    private static final String JS_DIR = SCI + "/thirdparty/web";
    /** Pre-generated help images (source tree: plain dir; installed: zip). */
    private static final String IMAGES_DIR = SCI + "/modules/helptools/images";
    private static final String IMAGES_INDEX_NAME = "help_images.index";
    private static final String IMAGES_ZIP = SCI + "/modules/helptools/help_images.zip";
    /** Syntax highlighting colors shared with SciNotes (Preferences > Colors). */
    private static final String SYNTAX_COLORS_PATH = "//colors/body/syntax-highlighting-colors/item";

    private final String language;
    private final Path helpOutputDir; // TMPDIR/help/<lang>/
    /** Browser panel: shared JCEF focus handling, links, resize masking. */
    private final ScilabCefPanel cefPanel;
    private final CefBrowser browser;
    private javax.xml.transform.Templates xsltTemplates;
    private boolean pageLoaded = false;
    private String pendingKeyword = null;
    private String currentPageId = null;
    /** Notified when Preferences > Colors changes, to recolor the displayed code. */
    private final XConfigurationListener syntaxColorsListener;

    /**
     * Keyword lists for syntax highlighting: helpOutputDir/keywords.txt is
     * written by getscilabkeywords() in the helpbrowser() gateway
     * (sci_helpbrowser.cpp) before the browser is created or refreshed.
     * keywordsJson caches the JSON served to the pages for the file
     * modification time keywordsFileTime.
     */
    private static final String KEYWORDS_FILE_NAME = "keywords.txt";
    private String keywordsJson = null;
    private long keywordsFileTime = -1;

    /** Pre-generated images manifest: canonical code hash -> file name. */
    private java.util.Map<String, String> imagesManifest = null;
    private long imagesManifestTime = -1;
    private boolean imagesManifestFromZip = false;

    /**
     * Same manifest for the toolboxes, merged over every loaded one:
     * canonical code hash -> absolute image file. Filled from the
     * &lt;toolbox&gt;/help/images/help_images.index written at build time by
     * tbx_build_help(); empty when no toolbox pre-generated its images.
     */
    private java.util.Map<String, Path> toolboxImagesManifest = null;

    /** External help chapters registered via add_help_chapter() (toolboxes). */
    private volatile java.util.List<ExtChapter> externalChapters;

    /**
     * Full-text search. The index of the native chapters is built in the
     * background as soon as the browser is created, a toolbox chapter is
     * indexed on the first search after it is loaded. Indexing and searches
     * run on this single thread: a search waits for the native index without
     * blocking the CEF UI thread.
     */
    private final java.util.concurrent.ExecutorService searchExecutor =
        java.util.concurrent.Executors.newSingleThreadExecutor(r -> {
            Thread t = new Thread(r, "HelpBrowser-search");
            t.setDaemon(true);
            return t;
        });
    private static final int MAX_SEARCH_RESULTS = 50;
    /** Accessed on searchExecutor only. */
    private HelpSearchIndex nativeSearchIndex = null;
    /** Toolbox chapter indexes by chapter signature; accessed on searchExecutor only. */
    private final java.util.Map<String, HelpSearchIndex> toolboxSearchIndexes = new java.util.HashMap<>();

    public SwingScilabHelpBrowserJCEF(String[] helps, String language) {
        super(); // calls SwingScilabHelpBrowser() which sets BorderLayout
        this.language = (language != null && !language.isEmpty()) ? language : "en_US";
        this.helpOutputDir = Paths.get(ScilabConstants.TMPDIR.toString(), "help", this.language);

        // Ensure output directory exists
        try {
            Files.createDirectories(helpOutputDir);
        } catch (IOException e) {
            System.err.println("HelpBrowser: cannot create " + helpOutputDir + ": " + e.getMessage());
        }

        // Resolve external (toolbox) help chapters registered via add_help_chapter()
        this.externalChapters = resolveExternalChapters(helps);

        // Generate toc.json if needed
        generateTocIfNeeded();

        // Index the pages for the full-text search
        searchExecutor.execute(() -> nativeSearchIndex = new HelpSearchIndex(nativeSearchSources()));

        // Initialize XSLT transformer
        initTransformer();

        // Create JCEF browser. Its handlers are added through the panel: a
        // CefClient keeps a single handler of each kind, and the panel already
        // installed its own.
        String shellUrl = "file:///" + WEB_DIR.replace('\\', '/') + "/index.html";
        cefPanel = new ScilabCefPanel(shellUrl);
        browser = cefPanel.getBrowser();

        // Message router: JS -> Java bridge
        CefMessageRouterConfig routerConfig = new CefMessageRouterConfig("helpBrowserQuery", "helpBrowserCancel");
        CefMessageRouter router = CefMessageRouter.create(routerConfig, new HelpBrowserMessageHandler());
        cefPanel.getClient().addMessageRouter(router);

        // Ctrl+Shift+I toggles the DevTools
        cefPanel.setDebug(true);

        // Open external links in system browser
        cefPanel.addLifeSpanHandler(new CefLifeSpanHandlerAdapter() {
            @Override
            public boolean onBeforePopup(CefBrowser b, CefFrame frame, String targetUrl,
                    String targetFrameName) {
                if (targetUrl != null && (targetUrl.startsWith("http://") || targetUrl.startsWith("https://"))) {
                    try {
                        Desktop.getDesktop().browse(URI.create(targetUrl));
                    } catch (IOException e) {
                        // ignore
                    }
                }
                return true; // block popup, we handle it
            }
        });

        // Track page load to know when executeJavaScript is safe
        cefPanel.addLoadHandler(new CefLoadHandlerAdapter() {
            @Override
            public void onLoadEnd(CefBrowser b, CefFrame frame, int httpStatusCode) {
                if (b == browser && frame.isMain()) {
                    pageLoaded = true;
                    if (pendingKeyword != null) {
                        doNavigate(pendingKeyword);
                        pendingKeyword = null;
                    }
                }
            }
        });

        // Disable context menu
        cefPanel.addContextMenuHandler(new CefContextMenuHandlerAdapter() {
            @Override
            public void onBeforeContextMenu(CefBrowser b, CefFrame frame,
                    CefContextMenuParams params, CefMenuModel model) {
                model.clear();
            }
        });

        // Follow the syntax highlighting colors of the Preferences (shared
        // with SciNotes): the page re-fetches them through getSyntaxColors
        syntaxColorsListener = new XConfigurationListener() {
            @Override
            public void configurationChanged(XConfigurationEvent e) {
                if (!pageLoaded || !syntaxColorsChanged(e.getModifiedPaths())) return;
                browser.executeJavaScript(
                        "if(window.scilabHelp && window.scilabHelp.syntaxColorsChanged) window.scilabHelp.syntaxColorsChanged();",
                        "helpbrowser", 0);
            }
        };
        XConfiguration.addXConfigurationListener(syntaxColorsListener);

        add(cefPanel, BorderLayout.CENTER);

        // The help only lays out its browser: the focus given to its dock
        // goes to the browser panel, which hands it to the page.
        setFocusable(false);
    }

    // ================================================================
    // Syntax highlighting keywords (getscilabkeywords)
    // ================================================================

    private Path getKeywordsFile() {
        return helpOutputDir.resolve(KEYWORDS_FILE_NAME);
    }

    /**
     * JSON served to the pages: {"keyword": commands, "literal": predefined
     * variables, "built_in": primitives, "type": macros + scicos}, from
     * keywords.txt (one category per line: primitives, commands, predefined
     * variables, macros, scicos functions). Re-read when the file changes
     * (a toolbox load goes through helpbrowser() again, which rewrites it).
     * Without the file (getscilabkeywords failed), falls back to the
     * SciNotes JNI lists.
     */
    private synchronized String getKeywordsJson() {
        Path file = getKeywordsFile();
        long time = -1;
        try {
            if (Files.exists(file)) {
                time = Files.getLastModifiedTime(file).toMillis();
            }
        } catch (IOException e) {
            // treated as missing
        }
        if (time >= 0 && time != keywordsFileTime) {
            String fromFile = parseKeywordsFile(file);
            if (fromFile != null) {
                keywordsJson = fromFile;
                keywordsFileTime = time;
            }
        }
        if (keywordsJson == null) {
            keywordsJson = fallbackKeywordsJson();
        }
        return keywordsJson;
    }

    private static String parseKeywordsFile(Path file) {
        try {
            java.util.List<String> lines = Files.readAllLines(file, StandardCharsets.UTF_8);
            if (lines.size() < 4) return null;
            String primitives = cleanKeywords(lines.get(0));
            String commands = cleanKeywords(lines.get(1));
            String predef = cleanKeywords(lines.get(2));
            String macros = cleanKeywords(lines.get(3));
            if (lines.size() > 4) {
                String scicos = cleanKeywords(lines.get(4));
                if (!scicos.isEmpty()) macros = macros.isEmpty() ? scicos : macros + " " + scicos;
            }
            return keywordsJson(commands, predef, primitives, macros);
        } catch (IOException e) {
            return null;
        }
    }

    /** Keep only names highlight.js can match with its Scilab pattern (%?\w+). */
    private static String cleanKeywords(String line) {
        StringBuilder sb = new StringBuilder();
        for (String tok : line.trim().split("\\s+")) {
            if (tok.matches("%?\\w+")) {
                if (sb.length() > 0) sb.append(' ');
                sb.append(tok);
            }
        }
        return sb.toString();
    }

    private static String keywordsJson(String keyword, String literal, String builtIn, String type) {
        return "{\"keyword\":\"" + keyword + "\",\"literal\":\"" + literal
               + "\",\"built_in\":\"" + builtIn + "\",\"type\":\"" + type + "\"}";
    }

    /** SciNotes JNI lists (primitives + macros only), used while the interpreter is busy. */
    private static String fallbackKeywordsJson() {
        try {
            // Reflection avoids a compile-time gui -> scinotes dependency
            // (scinotes depends on gui; same reason as handleEditCode).
            Class<?> keywords = Class.forName("org.scilab.modules.scinotes.ScilabKeywords");
            String[] functions = (String[]) keywords.getMethod("GetFunctionsName").invoke(null);
            String[] macros = (String[]) keywords.getMethod("GetMacrosName").invoke(null);
            String builtIn = (functions != null) ? cleanKeywords(String.join(" ", functions)) : "";
            String type = (macros != null) ? cleanKeywords(String.join(" ", macros)) : "";
            return keywordsJson("", "", builtIn, type);
        } catch (Exception e) {
            return keywordsJson("", "", "", "");
        }
    }

    /** True if the modified XConfiguration paths touch the syntax colors ("ALL" included). */
    private static boolean syntaxColorsChanged(java.util.Set<String> paths) {
        if (paths == null) return false;
        for (String p : paths) {
            if ("ALL".equals(p) || p.contains("syntax-highlighting-colors")) return true;
        }
        return false;
    }

    /**
     * Read the syntax highlighting colors from the Preferences document and
     * serialize them as JSON: {"Primitive":{"color":"#32B9B9","bold":false,"italic":false}, ...}.
     * Keys are the item names of XConfiguration-colors.xml (SciNotes token names).
     */
    private static String buildSyntaxColorsJson() {
        StringBuilder json = new StringBuilder("{");
        try {
            Document doc = XConfiguration.getXConfigurationDocument();
            XPath xp = ScilabXPathFactory.newInstance().newXPath();
            NodeList items = (NodeList) xp.compile(SYNTAX_COLORS_PATH).evaluate(doc, XPathConstants.NODESET);
            boolean first = true;
            for (int i = 0; i < items.getLength(); i++) {
                if (!(items.item(i) instanceof Element)) continue;
                Element item = (Element) items.item(i);
                String name = item.getAttribute("name");
                String color = item.getAttribute("color");
                if (name.isEmpty() || !color.matches("#[0-9A-Fa-f]{6}")) continue;
                if (!first) json.append(',');
                first = false;
                json.append('"').append(name.replace("\"", "")).append("\":{")
                    .append("\"color\":\"").append(color).append("\",")
                    .append("\"bold\":").append("true".equalsIgnoreCase(item.getAttribute("bold"))).append(',')
                    .append("\"italic\":").append("true".equalsIgnoreCase(item.getAttribute("italic")))
                    .append('}');
            }
        } catch (Exception e) {
            // Preferences unavailable: the page keeps the built-in theme
        }
        return json.append('}').toString();
    }

    // ================================================================
    // TOC generation
    // ================================================================

    private Path getTocFile() {
        return helpOutputDir.resolve("toc.json");
    }

    /**
     * Generate the persistent toc.json. It contains ONLY the native Scilab
     * chapters (MAIN_CHAPTERS). Toolbox chapters registered via
     * add_help_chapter() are intentionally NOT written here: they are merged
     * in memory at serve time ({@link #buildExternalChapters()}), so that
     * restarting Scilab without loading a toolbox does not leave its help
     * lingering in the tree.
     */
    private void generateTocIfNeeded() {
        Path tocFile = getTocFile();
        if (Files.exists(tocFile)) return;

        try {
            java.util.List<String> modules = readMainChapters();

            JsonArray chapters = new JsonArray();

            for (String mod : modules) {
                // Always scan en_US for complete page list
                Path helpDirEnUS = Paths.get(SCI, "modules", mod, "help", "en_US");
                // Skip the modules with no page (arnoldi: addchapter.sce only)
                if (!hasXmlContent(helpDirEnUS.toFile())) continue;

                // Use localized dir for section titles (CHAPTER files) if available
                Path helpDirLocalized = Paths.get(SCI, "modules", mod, "help", language);
                if (!Files.isDirectory(helpDirLocalized)) {
                    helpDirLocalized = helpDirEnUS;
                }

                String chapterTitle = getChapterTitle(helpDirLocalized, mod);
                if (chapterTitle.equals(mod)) {
                    // Fallback to en_US title
                    chapterTitle = getChapterTitle(helpDirEnUS, mod);
                }

                JsonObject chapter = new JsonObject();
                chapter.addProperty("id", mod);
                chapter.addProperty("title", chapterTitle);
                String relPrefix = "modules/" + mod + "/help/en_US";
                addDirContents(chapter, helpDirEnUS, helpDirLocalized, relPrefix);
                chapters.add(chapter);
            }

            Files.createDirectories(tocFile.getParent());
            Files.write(tocFile, chapters.toString().getBytes(StandardCharsets.UTF_8));
        } catch (IOException e) {
            System.err.println("HelpBrowser: TOC generation failed: " + e.getMessage());
        }
    }

    /** Modules of the native help chapters, in the order of MAIN_CHAPTERS. */
    private static java.util.List<String> readMainChapters() throws IOException {
        Path mainChapters = Paths.get(SCI, "modules", "helptools", "etc", "MAIN_CHAPTERS");
        java.util.List<String> modules = new java.util.ArrayList<>();
        if (Files.exists(mainChapters)) {
            for (String line : Files.readAllLines(mainChapters, StandardCharsets.UTF_8)) {
                String mod = line.trim();
                if (!mod.isEmpty()) modules.add(mod);
            }
        }
        return modules;
    }

    // ================================================================
    // Full-text search index
    // ================================================================

    /**
     * Pages of the native chapters, the ones of toc.json, read in the
     * current language when translated (the text the browser displays).
     */
    private java.util.List<HelpSearchIndex.Source> nativeSearchSources() {
        java.util.List<HelpSearchIndex.Source> sources = new java.util.ArrayList<>();
        try {
            for (String mod : readMainChapters()) {
                Path helpDirEnUS = Paths.get(SCI, "modules", mod, "help", "en_US");
                if (Files.isDirectory(helpDirEnUS)) {
                    collectSearchSources(helpDirEnUS.toFile(), Paths.get(SCI, "modules", mod, "help", language),
                                         "modules/" + mod + "/help/en_US", sources);
                }
            }
        } catch (IOException e) {
            System.err.println("HelpBrowser: search index failed: " + e.getMessage());
        }
        return sources;
    }

    /** Pages of a help directory (en_US, as the TOC), with their TOC path. */
    private void collectSearchSources(File dir, Path localizedDir, String relPrefix,
                                      java.util.List<HelpSearchIndex.Source> sources) {
        java.util.List<File> xmlFiles = new java.util.ArrayList<>();
        java.util.List<File> subDirs = new java.util.ArrayList<>();
        listHelpDir(dir, xmlFiles, subDirs);
        for (File sub : subDirs) {
            if (hasXmlContent(sub)) {
                collectSearchSources(sub, localizedDir.resolve(sub.getName()), relPrefix + "/" + sub.getName(), sources);
            }
        }
        for (File xml : xmlFiles) {
            File localized = localizedDir.resolve(xml.getName()).toFile();
            sources.add(new HelpSearchIndex.Source(relPrefix + "/" + xml.getName(), localized.isFile() ? localized : xml));
        }
    }

    /**
     * Indexes to search: the native chapters and the toolbox chapters loaded
     * now (indexed on their first search). Runs on searchExecutor.
     */
    private java.util.List<HelpSearchIndex> searchIndexes() {
        java.util.List<HelpSearchIndex> indexes = new java.util.ArrayList<>();
        if (nativeSearchIndex != null) {
            indexes.add(nativeSearchIndex);
        }
        java.util.Set<String> loaded = new java.util.HashSet<>();
        for (ExtChapter ch : externalChapters) {
            String sig = chapterSignature(ch);
            loaded.add(sig);
            HelpSearchIndex index = toolboxSearchIndexes.get(sig);
            if (index == null) {
                java.util.List<HelpSearchIndex.Source> sources = new java.util.ArrayList<>();
                collectSearchSources(ch.enUS.toFile(), ch.localized, ch.enUS.toString().replace('\\', '/'), sources);
                index = new HelpSearchIndex(sources);
                toolboxSearchIndexes.put(sig, index);
            }
            indexes.add(index);
        }
        toolboxSearchIndexes.keySet().retainAll(loaded); // unloaded toolboxes
        return indexes;
    }

    /**
     * Build the chapter objects of the currently registered external (toolbox)
     * chapters (empty when no toolbox is loaded). Computed in memory on every
     * getToc and never persisted, so toolbox help only appears while the
     * toolbox is actually loaded.
     */
    private JsonArray buildExternalChapters() {
        JsonArray chapters = new JsonArray();
        for (ExtChapter ch : externalChapters) {
            JsonObject chapter = new JsonObject();
            chapter.addProperty("id", ch.id);
            chapter.addProperty("title", ch.title);
            String relPrefix = ch.enUS.toString().replace('\\', '/');
            try {
                addDirContents(chapter, ch.enUS, ch.localized, relPrefix);
            } catch (IOException e) {
                continue; // skip a chapter we cannot read
            }
            chapters.add(chapter);
        }
        return chapters;
    }

    /**
     * Append the in-memory external chapters to the native toc.json array.
     * Returns the native JSON unchanged when no toolbox is loaded or when it
     * is not the expected array.
     */
    private String mergeExternalChapters(String nativeJson) {
        JsonArray ext = buildExternalChapters();
        if (ext.size() == 0) {
            return nativeJson;
        }
        try {
            JsonElement toc = JsonParser.parseString(nativeJson);
            if (!toc.isJsonArray()) {
                return nativeJson;
            }
            toc.getAsJsonArray().addAll(ext);
            return toc.toString();
        } catch (JsonParseException e) {
            return nativeJson;
        }
    }

    // ================================================================
    // External (toolbox) chapters
    // ================================================================

    /** A toolbox help chapter resolved from an add_help_chapter() path. */
    private static class ExtChapter {
        final String id;
        final String title;
        final Path enUS;       // dir scanned for the complete page list
        final Path localized;  // dir used for localized section titles

        ExtChapter(String id, String title, Path enUS, Path localized) {
            this.id = id;
            this.title = title;
            this.enUS = enUS;
            this.localized = localized;
        }
    }

    /**
     * Resolve the toolbox help chapters from the paths passed by
     * add_help_chapter() (%helps). Native modules (under SCI/modules, handled
     * via MAIN_CHAPTERS) and entries with no DocBook XML source are skipped.
     */
    private java.util.List<ExtChapter> resolveExternalChapters(String[] helps) {
        java.util.List<ExtChapter> list = new java.util.ArrayList<>();
        if (helps == null) {
            return list;
        }
        String sciNorm = (SCI == null) ? "" : SCI.replace('\\', '/').toLowerCase();
        java.util.Set<String> seen = new java.util.HashSet<>();

        for (String h : helps) {
            if (h == null || h.isEmpty()) {
                continue;
            }
            Path enUS = resolveXmlDir(h, "en_US");
            Path localized = resolveXmlDir(h, language);
            Path scan = (enUS != null) ? enUS : localized;
            if (scan == null) {
                continue; // no XML source: nothing to render in this browser
            }

            String scanNorm = scan.toString().replace('\\', '/');
            // Native module chapters are already provided via MAIN_CHAPTERS.
            if (!sciNorm.isEmpty() && scanNorm.toLowerCase().startsWith(sciNorm + "/modules/")) {
                continue;
            }
            if (!seen.add(scanNorm.toLowerCase())) {
                continue;
            }

            Path chapterRoot = scan.getParent();              // <root>/help
            if (chapterRoot != null) {
                chapterRoot = chapterRoot.getParent();        // <root>
            }
            String id = (chapterRoot != null)
                    ? chapterRoot.getFileName().toString()
                    : scan.getFileName().toString();
            // Title: the one given to add_help_chapter() first, then the
            // <title> of master_help.xml, then the toolbox directory name.
            String title = ScilabHelpBrowser.getChapterTitle(h);
            if (title == null) {
                title = getExternalChapterTitle(scan, id);
            }

            list.add(new ExtChapter(id, title, scan, (localized != null) ? localized : scan));
        }
        return list;
    }

    /**
     * Resolve the DocBook XML source directory for a help path and a language.
     * Handles the path being the help/&lt;lang&gt; dir itself, the toolbox
     * "jar" dir (sibling of "help"), or the toolbox root.
     */
    private Path resolveXmlDir(String helpPath, String lang) {
        Path p = Paths.get(helpPath);
        // 1. path is already a help/<lang> source dir
        if (dirHasXml(p)) {
            return p;
        }
        // 2. path is the jar dir: <root>/jar -> <root>/help/<lang>
        Path parent = p.getParent();
        if (parent != null) {
            Path c = parent.resolve("help").resolve(lang);
            if (dirHasXml(c)) {
                return c;
            }
        }
        // 3. path is the toolbox root: <root> -> <root>/help/<lang>
        Path c = p.resolve("help").resolve(lang);
        if (dirHasXml(c)) {
            return c;
        }
        return null;
    }

    /** Whether a directory directly contains at least one DocBook page XML. */
    private boolean dirHasXml(Path dir) {
        if (dir == null || !Files.isDirectory(dir)) {
            return false;
        }
        File[] files = dir.toFile().listFiles();
        if (files == null) {
            return false;
        }
        for (File f : files) {
            if (f.isFile() && f.getName().endsWith(".xml") && !isNonPageXml(f.getName())) {
                return true;
            }
        }
        return false;
    }

    /** Read the chapter title from master_help.xml, falling back to the id. */
    private String getExternalChapterTitle(Path xmlDir, String fallback) {
        Path master = xmlDir.resolve("master_help.xml");
        if (Files.exists(master)) {
            try {
                String content = new String(Files.readAllBytes(master), StandardCharsets.UTF_8);
                java.util.regex.Matcher m = java.util.regex.Pattern.compile(
                        "<title>([^<]*)</title>").matcher(content);
                if (m.find()) {
                    String t = decodeXmlEntities(m.group(1)).trim();
                    if (!t.isEmpty()) {
                        return t;
                    }
                }
            } catch (IOException e) { /* ignore */ }
        }
        return fallback;
    }

    /** Resolve the root directory of a loaded toolbox chapter by its id. */
    private String toolboxRootForId(String id) {
        for (ExtChapter ch : externalChapters) {
            if (ch.id.equals(id)) {
                Path root = ch.enUS.getParent();          // <root>/help
                if (root != null) {
                    root = root.getParent();              // <root>
                }
                if (root != null) {
                    return root.toString().replace('\\', '/');
                }
            }
        }
        return null;
    }

    /** XML files that are build artifacts, not documentation pages. */
    private static boolean isNonPageXml(String name) {
        return name.equals("master_help.xml")
                || name.equals("jhelptoc.xml")
                || name.equals("jhelpidx.xml")
                || name.equals("jhelpmap.xml");
    }

    private String getChapterTitle(Path helpDir, String moduleName) {
        Path addChapter = helpDir.resolve("addchapter.sce");
        if (Files.exists(addChapter)) {
            try {
                String content = new String(Files.readAllBytes(addChapter), StandardCharsets.UTF_8);
                java.util.regex.Matcher m = java.util.regex.Pattern.compile(
                        "add_help_chapter\\(\"([^\"]*)\"").matcher(content);
                if (m.find()) return m.group(1);
            } catch (IOException e) { /* ignore */ }
        }
        return moduleName;
    }

    private String getSectionTitle(Path dir) {
        Path chapterFile = dir.resolve("CHAPTER");
        if (Files.exists(chapterFile)) {
            try {
                String content = new String(Files.readAllBytes(chapterFile), StandardCharsets.UTF_8);
                java.util.regex.Matcher m = java.util.regex.Pattern.compile(
                        "title\\s*=\\s*(.*)").matcher(content);
                if (m.find()) return m.group(1).trim();
            } catch (IOException e) { /* ignore */ }
        }
        return dir.getFileName().toString().replace('_', ' ');
    }

    /** XML pages and sub-directories of a help directory, sorted by name. */
    private static void listHelpDir(File dir, java.util.List<File> xmlFiles, java.util.List<File> subDirs) {
        File[] entries = dir.listFiles();
        if (entries != null) {
            for (File f : entries) {
                if (f.isFile() && f.getName().endsWith(".xml") && !isNonPageXml(f.getName())) {
                    xmlFiles.add(f);
                } else if (f.isDirectory() && !f.getName().startsWith(".")
                        // Skip the built JavaHelp output (scilab_<lang>_help)
                        && !(f.getName().startsWith("scilab_") && f.getName().endsWith("_help"))) {
                    subDirs.add(f);
                }
            }
        }

        xmlFiles.sort((a, b) -> a.getName().compareToIgnoreCase(b.getName()));
        subDirs.sort((a, b) -> a.getName().compareToIgnoreCase(b.getName()));
    }

    /** Add the "sections" and "pages" arrays of a help directory to its TOC node. */
    private void addDirContents(JsonObject node, Path dir, Path localizedDir, String relPrefix) throws IOException {
        // Collect XML pages and subdirs from en_US (complete list)
        java.util.List<File> xmlFiles = new java.util.ArrayList<>();
        java.util.List<File> subDirs = new java.util.ArrayList<>();
        listHelpDir(dir.toFile(), xmlFiles, subDirs);

        // Sections
        JsonArray sections = new JsonArray();
        for (File sub : subDirs) {
            String subRel = relPrefix + "/" + sub.getName();
            if (!hasXmlContent(sub)) continue;

            // Use localized CHAPTER title if available
            Path localizedSub = localizedDir.resolve(sub.getName());
            String secTitle = Files.isDirectory(localizedSub)
                    ? getSectionTitle(localizedSub)
                    : getSectionTitle(sub.toPath());

            JsonObject section = new JsonObject();
            section.addProperty("id", sub.getName());
            section.addProperty("title", secTitle);
            addDirContents(section, sub.toPath(), localizedSub, subRel);
            sections.add(section);
        }
        node.add("sections", sections);

        // Pages
        JsonArray pages = new JsonArray();
        for (File xml : xmlFiles) {
            String[] info = parseXmlPage(xml);
            if (info == null) continue;

            JsonObject page = new JsonObject();
            page.addProperty("id", info[0]);
            page.addProperty("title", info[1]);
            page.addProperty("purpose", info[2]);
            page.addProperty("path", relPrefix + "/" + xml.getName());
            if (!info[3].isEmpty()) {
                // xml:ids are NCNames: no comma, split back safely
                JsonArray aliases = new JsonArray();
                for (String alias : info[3].split(",")) {
                    aliases.add(alias);
                }
                page.add("aliases", aliases);
            }
            pages.add(page);
        }
        node.add("pages", pages);
    }

    private boolean hasXmlContent(File dir) {
        File[] files = dir.listFiles();
        if (files == null) return false;
        for (File f : files) {
            if (f.isFile() && f.getName().endsWith(".xml") && !isNonPageXml(f.getName())) return true;
            if (f.isDirectory() && hasXmlContent(f)) return true;
        }
        return false;
    }

    /**
     * Parse an XML help file to extract id, refname, refpurpose.
     * @return [id, title, purpose, aliases] or null if not a refentry;
     *         aliases = the other xml:ids of the page, comma-separated
     */
    private String[] parseXmlPage(File xml) {
        try {
            // CRLF checkouts: keep the TOC text with LF line ends only
            String content = new String(Files.readAllBytes(xml.toPath()), StandardCharsets.UTF_8).replace("\r", "");
            java.util.regex.Matcher idM = java.util.regex.Pattern.compile("xml:id=\"([^\"]*)\"").matcher(content);
            java.util.regex.Matcher nameM = java.util.regex.Pattern.compile("<refname>([^<]*)</refname>").matcher(content);
            java.util.regex.Matcher purposeM = java.util.regex.Pattern.compile("<refpurpose>([^<]*)</refpurpose>").matcher(content);

            String id = idM.find() ? idM.group(1) : xml.getName().replace(".xml", "");
            String title = nameM.find() ? decodeXmlEntities(nameM.group(1)) : xml.getName().replace(".xml", "");
            String purpose = purposeM.find() ? decodeXmlEntities(purposeM.group(1)) : "";

            // Other xml:ids of the page are link targets too: functions
            // documented together (calmonths, calyears in caldays.xml),
            // sections...
            java.util.List<String> aliases = new java.util.ArrayList<>();
            while (idM.find()) {
                if (!idM.group(1).equals(id)) {
                    aliases.add(idM.group(1));
                }
            }

            return new String[] { id, title, purpose, String.join(",", aliases) };
        } catch (IOException e) {
            return null;
        }
    }

    private static final java.util.regex.Pattern XML_ENTITY =
        java.util.regex.Pattern.compile("&(amp|lt|gt|quot|apos|#[0-9]+|#x[0-9A-Fa-f]+);");

    /**
     * Decode the predefined and numeric XML entities in a single pass:
     * sequential replace() calls would decode "&amp;lt;" (text "&lt;") to "<".
     */
    private static String decodeXmlEntities(String s) {
        if (s.indexOf('&') < 0) return s;
        java.util.regex.Matcher m = XML_ENTITY.matcher(s);
        StringBuffer sb = new StringBuffer();
        while (m.find()) {
            String name = m.group(1);
            String rep;
            switch (name) {
                case "amp": rep = "&"; break;
                case "lt": rep = "<"; break;
                case "gt": rep = ">"; break;
                case "quot": rep = "\""; break;
                case "apos": rep = "'"; break;
                default:
                    int cp;
                    try {
                        cp = name.charAt(1) == 'x'
                             ? Integer.parseInt(name.substring(2), 16)
                             : Integer.parseInt(name.substring(1));
                    } catch (NumberFormatException e) {
                        cp = -1;
                    }
                    rep = Character.isValidCodePoint(cp) ? new String(Character.toChars(cp)) : m.group();
                    break;
            }
            m.appendReplacement(sb, java.util.regex.Matcher.quoteReplacement(rep));
        }
        m.appendTail(sb);
        return sb.toString();
    }

    // ================================================================
    // XSLT transformation
    // ================================================================

    private void initTransformer() {
        try {
            TransformerFactory factory = TransformerFactory.newInstance();
            StreamSource xsltSource = new StreamSource(new File(XSLT_FILE));
            xsltTemplates = factory.newTemplates(xsltSource);
        } catch (Exception e) {
            System.err.println("HelpBrowser: XSLT init failed: " + e.getMessage());
        }
    }

    /**
     * Transform a DocBook XML file to HTML.
     *
     * @param xmlPath absolute path to the XML file
     * @return absolute path to the generated HTML file, or null on error
     */
    private synchronized String transformXml(String xmlPath, boolean external) {
        if (xsltTemplates == null) return null;

        try {
            File xmlFile = new File(xmlPath);
            if (!xmlFile.exists()) return null;

            Path htmlPath;
            if (external) {
                // External (toolbox) chapters live anywhere on disk, and two of
                // them may share a root directory name (e.g. ATOMS installs
                // under <name>/<version>/), so key the output on a hash of the
                // XML directory to avoid collisions between toolboxes.
                // Output: TMPDIR/help/<lang>/_external/<md5(dir)>/<file>.html
                String xmlDirPath = xmlFile.getAbsoluteFile().getParent().replace('\\', '/');
                String dirHash = md5Hex(xmlDirPath.getBytes(StandardCharsets.UTF_8));
                String htmlName = xmlFile.getName().replaceAll("\\.xml$", ".html");
                htmlPath = helpOutputDir.resolve("_external").resolve(dirHash).resolve(htmlName);
            } else {
                // Native module page.
                // Input:  SCI/modules/<module>/help/<lang>/<section>/<file>.xml
                // Output: TMPDIR/help/<lang>/<module>/<section>/<file>.html
                String normalizedPath = xmlPath.replace('\\', '/');
                String relativePath;
                java.util.regex.Matcher hm = java.util.regex.Pattern.compile(
                        "/([^/]+)/help/[^/]+/(.+)$").matcher(normalizedPath);
                if (hm.find()) {
                    relativePath = hm.group(1) + "/" + hm.group(2);
                } else {
                    relativePath = new File(normalizedPath).getName();
                }
                String htmlRelative = relativePath.replaceAll("\\.xml$", ".html");
                htmlPath = helpOutputDir.resolve(htmlRelative);
            }
            Files.createDirectories(htmlPath.getParent());

            // Skip if HTML is newer than both the XML source and the stylesheet
            if (Files.exists(htmlPath)) {
                java.nio.file.attribute.FileTime htmlTime = Files.getLastModifiedTime(htmlPath);
                File xsltFile = new File(XSLT_FILE);
                if (htmlTime.compareTo(Files.getLastModifiedTime(xmlFile.toPath())) > 0
                        && (!xsltFile.exists() || htmlTime.compareTo(Files.getLastModifiedTime(xsltFile.toPath())) > 0)) {
                    return htmlPath.toString();
                }
            }

            // Compute base-path as absolute file:// URL to WEB_DIR
            // (relative paths don't work across different drives on Windows)
            String basePath = "file:///" + WEB_DIR.replace('\\', '/');

            // Compute xml-dir as absolute file:// URL to the XML source directory
            String xmlDir = "file:///" + xmlFile.getParent().replace('\\', '/');

            // Transform (new Transformer each time to avoid Xalan state issues)
            Transformer transformer = xsltTemplates.newTransformer();
            transformer.setParameter("base-path", basePath);
            transformer.setParameter("xml-dir", xmlDir);
            String jsDir = JS_DIR;
            if ((OS.get() != OS.WINDOWS) && (new File(jsDir).exists() == false)) {
                // Probably an installed Linux or macOS version where thirdparty/ is in SCI/../../
                jsDir = SCI + "/../../thirdparty/web";
            }
            transformer.setParameter("js-dir", "file:///" + jsDir.replace('\\', '/'));
            StreamSource source = new StreamSource(xmlFile);
            java.io.StringWriter sw = new java.io.StringWriter();
            StreamResult result = new StreamResult(sw);
            transformer.transform(source, result);
            String html = sw.toString();

            // Pre-generate all images before writing the page
            html = generateAllImages(html);

            Files.write(htmlPath, html.getBytes(StandardCharsets.UTF_8));
            return htmlPath.toString();
        } catch (Exception e) {
            System.err.println("HelpBrowser: transform failed for " + xmlPath + ": " + e.getMessage());
            return null;
        }
    }

    // ================================================================
    // Image pre-generation
    // ================================================================

    /**
     * Scan HTML for scilab:image placeholders and xcos diagrams,
     * generate all images, and replace placeholders with img tags.
     */
    private String generateAllImages(String html) {
        // Process scilab:image containers
        html = generateScilabImages(html);
        // Process xcos diagram containers
        html = generateXcosImages(html);
        return html;
    }

    private String generateScilabImages(String html) {
        StringBuilder sb = new StringBuilder();
        String marker = "data-scilab-code=\"";
        int pos = 0;
        while (true) {
            int start = html.indexOf(marker, pos);
            if (start < 0) break;

            int codeStart = start + marker.length();
            int codeEnd = html.indexOf("\"", codeStart);
            if (codeEnd < 0) break;

            String code = decodeXmlEntities(html.substring(codeStart, codeEnd));

            // Find the whole container div and its closing tag
            int divStart = html.lastIndexOf("<div", start);
            int divEnd = html.indexOf("</div>", codeEnd);
            if (divEnd < 0) break;
            divEnd += "</div>".length();

            // Resolve the image and embed it as a data: URI so the generated
            // page is self-contained (no reference to cache files)
            byte[] png = resolveScilabImage(code);

            sb.append(html, pos, divStart);
            if (png != null) {
                sb.append("<div class=\"scilab-image-container\"><img src=\"data:image/png;base64,")
                  .append(java.util.Base64.getEncoder().encodeToString(png))
                  .append("\" alt=\"plot\"></div>");
            } else {
                sb.append("<div class=\"scilab-image-container\"><p>Image generation failed</p></div>");
            }
            pos = divEnd;
        }
        sb.append(html, pos, html.length());
        return sb.toString();
    }

    /**
     * Canonical key of a scilab:image code snippet, shared with the
     * pre-generation script (generate_help_images.sce): the language,
     * then the code with lines trimmed, empty lines dropped, joined by LF
     * (immune to whitespace differences between XML parsers). The language
     * is part of the key because an identical snippet can render
     * differently per locale (gettext/_()).
     */
    private String canonicalCodeKey(String code) {
        StringBuilder sb = new StringBuilder(language);
        for (String line : code.split("\n", -1)) {
            String trimmed = line.trim();
            if (trimmed.isEmpty()) continue;
            sb.append('\n').append(trimmed);
        }
        return sb.toString();
    }

    private static String md5Hex(byte[] data) throws java.security.NoSuchAlgorithmException {
        byte[] digest = java.security.MessageDigest.getInstance("MD5").digest(data);
        StringBuilder hex = new StringBuilder();
        for (byte b : digest) hex.append(String.format("%02x", b));
        return hex.toString();
    }

    /** Language-independent session cache for lazily generated images. */
    private Path sharedImagesCacheDir() {
        return Paths.get(ScilabConstants.TMPDIR.toString(), "help", "images");
    }

    /**
     * Manifest of the pre-generated images (canonical code hash -> file
     * name), read from SCI/modules/helptools/images/help_images.index in a
     * source tree, or from help_images.zip in an installed Scilab. Reloaded
     * when the index file changes (images regenerated during development).
     */
    private synchronized java.util.Map<String, String> imagesManifest() {
        File index = new File(IMAGES_DIR, IMAGES_INDEX_NAME);
        if (index.exists()) {
            if (imagesManifest != null && !imagesManifestFromZip
                    && index.lastModified() == imagesManifestTime) {
                return imagesManifest;
            }
            imagesManifest = new java.util.HashMap<>();
            imagesManifestFromZip = false;
            imagesManifestTime = index.lastModified();
            try {
                for (String line : Files.readAllLines(index.toPath(), StandardCharsets.UTF_8)) {
                    String[] tok = line.trim().split("\\s+");
                    if (tok.length == 2) {
                        imagesManifest.put(tok[0], tok[1]);
                    }
                }
            } catch (IOException e) {
                System.err.println("HelpBrowser: cannot read " + index + ": " + e.getMessage());
            }
            return imagesManifest;
        }
        if (imagesManifest != null && imagesManifestFromZip) {
            return imagesManifest;
        }
        imagesManifest = new java.util.HashMap<>();
        imagesManifestFromZip = true;
        File zip = new File(IMAGES_ZIP);
        if (zip.exists()) {
            try (java.util.zip.ZipFile zf = new java.util.zip.ZipFile(zip)) {
                java.util.zip.ZipEntry entry = zf.getEntry(IMAGES_INDEX_NAME);
                if (entry != null) {
                    try (java.io.BufferedReader reader = new java.io.BufferedReader(
                            new java.io.InputStreamReader(zf.getInputStream(entry), StandardCharsets.UTF_8))) {
                        String line;
                        while ((line = reader.readLine()) != null) {
                            String[] tok = line.trim().split("\\s+");
                            if (tok.length == 2) {
                                imagesManifest.put(tok[0], tok[1]);
                            }
                        }
                    }
                }
            } catch (IOException e) {
                System.err.println("HelpBrowser: cannot read " + IMAGES_ZIP + ": " + e.getMessage());
            }
        }
        return imagesManifest;
    }

    /**
     * Manifests of the images pre-generated by the loaded toolboxes, merged
     * into a single hash -> file map. Each entry comes from
     * &lt;toolbox&gt;/help/images/help_images.index, written by
     * tbx_build_help() at toolbox build time. Rebuilt whenever the set of
     * chapters changes, i.e. when a toolbox is loaded or unloaded.
     */
    private synchronized java.util.Map<String, Path> toolboxImagesManifest() {
        if (toolboxImagesManifest != null) {
            return toolboxImagesManifest;
        }
        toolboxImagesManifest = new java.util.HashMap<>();
        java.util.Set<Path> done = new java.util.HashSet<>();
        for (ExtChapter ch : externalChapters) {
            // <toolbox>/help/<lang> -> <toolbox>/help/images
            for (Path langDir : new Path[] {ch.enUS, ch.localized}) {
                if (langDir == null || langDir.getParent() == null) {
                    continue;
                }
                Path dir = langDir.getParent().resolve("images");
                if (!done.add(dir)) {
                    continue;
                }
                File index = dir.resolve(IMAGES_INDEX_NAME).toFile();
                if (!index.isFile()) {
                    continue;
                }
                try {
                    for (String line : Files.readAllLines(index.toPath(), StandardCharsets.UTF_8)) {
                        String[] tok = line.trim().split("\\s+");
                        if (tok.length == 2) {
                            toolboxImagesManifest.put(tok[0], dir.resolve(tok[1]));
                        }
                    }
                } catch (IOException e) {
                    System.err.println("HelpBrowser: cannot read " + index + ": " + e.getMessage());
                }
            }
        }
        return toolboxImagesManifest;
    }

    /**
     * PNG bytes of a pre-generated image, from the source tree (manifest +
     * plain dir), from the installed help_images.zip, or from the images a
     * loaded toolbox pre-generated at build time; null if the key is unknown
     * or the file is missing.
     */
    private byte[] preGeneratedImage(String manifestKey) {
        try {
            String name = imagesManifest().get(manifestKey);
            if (name == null) {
                Path tbx = toolboxImagesManifest().get(manifestKey);
                if (tbx != null && Files.isRegularFile(tbx) && Files.size(tbx) > 0) {
                    return Files.readAllBytes(tbx);
                }
                return null;
            }
            if (!imagesManifestFromZip) {
                File pre = new File(IMAGES_DIR, name);
                if (pre.exists() && pre.length() > 0) {
                    return Files.readAllBytes(pre.toPath());
                }
            } else {
                try (java.util.zip.ZipFile zf = new java.util.zip.ZipFile(IMAGES_ZIP)) {
                    java.util.zip.ZipEntry entry = zf.getEntry(name);
                    if (entry != null) {
                        return zf.getInputStream(entry).readAllBytes();
                    }
                }
            }
            return null;
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * Resolve the PNG bytes for a scilab:image code snippet:
     * 1. session cache (lazily generated earlier in this session)
     * 2. pre-generated image from the source tree (manifest + plain dir)
     * 3. pre-generated image from the installed help_images.zip
     * 4. pre-generated image shipped by a loaded toolbox
     * 5. lazy generation (typically an example being edited)
     */
    private byte[] resolveScilabImage(String code) {
        try {
            String hash = md5Hex(canonicalCodeKey(code).getBytes(StandardCharsets.UTF_8));

            Path imageDir = sharedImagesCacheDir();
            Files.createDirectories(imageDir);
            Path pngFile = imageDir.resolve(hash + ".png");

            // 1. session cache
            if (Files.exists(pngFile) && Files.size(pngFile) > 0) {
                return Files.readAllBytes(pngFile);
            }

            // 2/3/4. pre-generated image: plain dir (source tree), zip
            // (installed), or the pool built with a loaded toolbox
            byte[] pre = preGeneratedImage(hash);
            if (pre != null) {
                return pre;
            }

            // 5. lazy generation (typically an example being edited)
            String pngPath = pngFile.toString().replace('\\', '/');
            String scilabCode =
                    "function _generate_help_image()\n"
                    + "  __oldwins__ = winsid();\n"
                    + "  __olddrv__ = driver();\n"
                    + "  __oldseed__ = rand(\"seed\");\n"
                    + "  __oldgrand__ = grand(\"getsd\");\n"
                    + "  __err__ = %f;\n"
                    + "  try\n"
                    + "    driver(\"png\");\n"
                    + "    xinit(\"" + pngPath + "\");\n"
                    + "    scf();\n"
                    + "    rand(\"seed\", 0);\n"
                    + "    grand(\"setsd\", 0);\n"
                    + code + "\n"
                    + "    gcf().anti_aliasing = \"2x\";\n"
                    + "  catch\n"
                    + "    __err__ = %t;\n"
                    + "    mprintf(\"Help image generation error: %s\\n\", lasterror());\n"
                    + "  end\n"
                    + "  try xend(); catch end\n"
                    + "  try driver(__olddrv__); catch end\n"
                    + "  rand(\"seed\", __oldseed__);\n"
                    + "  try grand(\"setsd\", __oldgrand__); catch end\n"
                    + "  __newwins__ = setdiff(winsid(), __oldwins__);\n"
                    + "  for __i__ = __newwins__, delete(scf(__i__)); end\n"
                    + "  if __err__ then\n"
                    + "    error(\"Help image generation failed\");\n"
                    + "  end\n"
                    + "endfunction\n"
                    + "_generate_help_image();\n"
                    + "clear _generate_help_image;\n";

            org.scilab.modules.action_binding.InterpreterManagement.requestScilabExec(scilabCode);

            long timeout = System.currentTimeMillis() + 10000;
            while (!Files.exists(pngFile) || Files.size(pngFile) == 0) {
                if (System.currentTimeMillis() > timeout) return null;
                Thread.sleep(100);
            }

            return Files.readAllBytes(pngFile);
        } catch (Exception e) {
            return null;
        }
    }

    private String generateXcosImages(String html) {
        StringBuilder sb = new StringBuilder();
        String marker = "data-xcos-file=\"";
        int pos = 0;
        while (true) {
            int start = html.indexOf(marker, pos);
            if (start < 0) break;

            int fileStart = start + marker.length();
            int fileEnd = html.indexOf("\"", fileStart);
            if (fileEnd < 0) break;

            String xcosFileUrl = html.substring(fileStart, fileEnd);

            int divStart = html.lastIndexOf("<div", start);
            int divEnd = html.indexOf("</div>", fileEnd);
            if (divEnd < 0) break;
            divEnd += "</div>".length();

            byte[] png = resolveXcosImage(xcosFileUrl);

            sb.append(html, pos, divStart);
            if (png != null) {
                sb.append("<div class=\"xcos-image-container\" data-xcos-file=\"")
                  .append(xcosFileUrl)
                  .append("\"><img src=\"data:image/png;base64,")
                  .append(java.util.Base64.getEncoder().encodeToString(png))
                  .append("\" alt=\"diagram\"></div>");
            } else {
                sb.append("<div class=\"xcos-image-container\" data-xcos-file=\"")
                  .append(xcosFileUrl)
                  .append("\"><p>Diagram generation failed</p></div>");
            }
            pos = divEnd;
        }
        sb.append(html, pos, html.length());
        return sb.toString();
    }

    private byte[] resolveXcosImage(String xcosFileUrl) {
        try {
            String xcosPath = xcosFileUrl.replace("file:///", "");

            // key on the diagram content: portable across machines, and the
            // image is regenerated when the diagram changes
            String hash = md5Hex(Files.readAllBytes(Paths.get(xcosPath)));

            Path imageDir = sharedImagesCacheDir();
            Files.createDirectories(imageDir);
            Path pngFile = imageDir.resolve("xcos_" + hash + ".png");

            if (Files.exists(pngFile) && Files.size(pngFile) > 0) {
                return Files.readAllBytes(pngFile);
            }

            byte[] pre = preGeneratedImage("xcos_" + hash);
            if (pre != null) {
                return pre;
            }

            String pngPath = pngFile.toString().replace('\\', '/');
            String scilabCode =
                    "loadXcosLibs();\n"
                    + "importXcosDiagram(\"" + xcosPath + "\");\n"
                    + "xcosDiagramExport(scs_m, \"" + pngPath + "\");\n";

            org.scilab.modules.action_binding.InterpreterManagement.requestScilabExec(scilabCode);

            long timeout = System.currentTimeMillis() + 30000;
            while (!Files.exists(pngFile) || Files.size(pngFile) == 0) {
                if (System.currentTimeMillis() > timeout) return null;
                Thread.sleep(200);
            }

            return Files.readAllBytes(pngFile);
        } catch (Exception e) {
            return null;
        }
    }

    // ================================================================
    // JS -> Java message handler
    // ================================================================

    private class HelpBrowserMessageHandler extends CefMessageRouterHandlerAdapter {

        @Override
        public boolean onQuery(CefBrowser b, CefFrame frame, long queryId,
                String request, boolean persistent, CefQueryCallback callback) {
            if (b != browser) return false;

            try {
                // Simple JSON parsing: {"type":"...", ...}
                String type = extractJsonString(request, "type");
                if (type == null) {
                    callback.failure(-1, "Missing type");
                    return true;
                }

                switch (type) {
                    case "getHomePage":
                        handleGetHomePage(callback);
                        return true;

                    case "getToc":
                        handleGetToc(callback);
                        return true;

                    case "getScilabKeywords":
                        handleGetKeywords(callback);
                        return true;

                    case "getSyntaxColors":
                        callback.success(buildSyntaxColorsJson());
                        return true;

                    case "transformXml":
                        handleTransformXml(request, callback);
                        return true;

                    case "fullTextSearch":
                        handleFullTextSearch(request, callback);
                        return true;

                    case "openXcos":
                        handleOpenXcos(request, callback);
                        return true;

                    case "scilabLink":
                        handleScilabLink(request, callback);
                        return true;

                    case "openExternal":
                        handleOpenExternal(request, callback);
                        return true;

                    case "execCode":
                        handleExecCode(request, callback);
                        return true;

                    case "editCode":
                        handleEditCode(request, callback);
                        return true;

                    case "copyCode":
                        handleCopyCode(request, callback);
                        return true;

                    case "navigateById":
                        String id = extractJsonString(request, "id");
                        handleNavigateById(id, callback);
                        return true;

                    default:
                        callback.failure(-1, "Unknown type: " + type);
                        return true;
                }
            } catch (Exception e) {
                callback.failure(-1, e.getMessage());
                return true;
            }
        }

        private void handleGetHomePage(CefQueryCallback callback) {
            String pagesDir = SCI + "/modules/helptools/data/pages";
            File localized = new File(pagesDir, "homepage-" + language + ".html");
            if (!localized.exists()) {
                localized = new File(pagesDir, "homepage-en_US.html");
            }
            if (localized.exists()) {
                callback.success("file:///" + localized.getAbsolutePath().replace('\\', '/'));
            } else {
                callback.failure(-1, "Homepage not found");
            }
        }

        private void handleGetToc(CefQueryCallback callback) {
            try {
                Path tocFile = getTocFile();
                if (!Files.exists(tocFile)) {
                    callback.failure(-1, "toc.json not found at " + tocFile);
                    return;
                }
                // Persistent file: native chapters only. Merge the in-memory
                // external (toolbox) chapters just before serving, so toolbox
                // help is present only while the toolbox is loaded.
                String nativeJson = new String(Files.readAllBytes(tocFile), StandardCharsets.UTF_8);
                callback.success(mergeExternalChapters(nativeJson));
            } catch (IOException e) {
                callback.failure(-1, "Failed to read toc.json: " + e.getMessage());
            }
        }

        private void handleGetKeywords(CefQueryCallback callback) {
            callback.success(getKeywordsJson());
        }

        private void handleTransformXml(String request, CefQueryCallback callback) {
            String xmlPath = extractJsonString(request, "path");
            if (xmlPath == null) {
                callback.failure(-1, "Missing path");
                return;
            }

            // Try the current language first, fallback to en_US
            String localizedPath = xmlPath.replace("/help/en_US/", "/help/" + language + "/");
            String absolutePath;
            boolean external = new File(localizedPath).isAbsolute();
            if (external) {
                // External toolbox page: toc.json stores an absolute path
                absolutePath = new File(localizedPath).exists() ? localizedPath : xmlPath;
            } else {
                // Native module page: path is relative to SCI
                absolutePath = SCI + "/" + localizedPath;
                if (!new File(absolutePath).exists()) {
                    absolutePath = SCI + "/" + xmlPath;
                }
            }

            // Extract page ID from XML filename
            String fileName = new File(xmlPath).getName().replace(".xml", "");
            currentPageId = fileName;

            String htmlPath = transformXml(absolutePath, external);
            if (htmlPath != null) {
                String htmlUrl = "file:///" + htmlPath.replace('\\', '/');
                callback.success(htmlUrl);
            } else {
                callback.failure(-1, "Transform failed for: " + xmlPath);
            }
        }


        private void handleFullTextSearch(String request, CefQueryCallback callback) {
            String query = extractJsonString(request, "query");
            if (query == null || query.trim().isEmpty()) {
                callback.success("{\"total\":0,\"terms\":[],\"results\":[]}");
                return;
            }
            // Off the CEF UI thread: the first search may wait for the index
            searchExecutor.execute(() -> {
                try {
                    callback.success(HelpSearchIndex.search(searchIndexes(), query, MAX_SEARCH_RESULTS));
                } catch (RuntimeException e) {
                    callback.failure(-1, String.valueOf(e.getMessage()));
                }
            });
        }

        private void handleOpenXcos(String request, CefQueryCallback callback) {
            String xcosFileUrl = extractJsonString(request, "file");
            if (xcosFileUrl != null) {
                String path = xcosFileUrl.replace("file:///", "");
                String sciPath = SCI.replace("\\", "/");
                if (path.startsWith(sciPath)) {
                    path = "SCI" + path.substring(sciPath.length());
                }
                org.scilab.modules.action_binding.InterpreterManagement.putCommandInScilabQueue(
                        "xcos(\"" + path + "\");");
            }
            callback.success("");
        }

        /**
         * Resolve a Scilab DocBook link of the form
         * {@code <main>.<sub>/<path>} (e.g. "scilab.xcos/xcos/examples/x.zcos",
         * "<toolbox>.exec/macros/foo.sce") and perform the matching action.
         * Help links ("*.help/id") are handled in JS; they never reach here.
         */
        private void handleScilabLink(String request, CefQueryCallback callback) {
            String linkend = extractJsonString(request, "linkend");
            String linkType = extractJsonString(request, "linkType");
            if (linkend == null || linkend.isEmpty()) {
                callback.success("");
                return;
            }

            String addr = linkend.replace('\\', '/');
            int slash = addr.indexOf('/');
            if (slash < 0) {
                callback.success("");
                return;
            }
            String location = addr.substring(0, slash);
            String path = addr.substring(slash + 1);
            int dot = location.indexOf('.');
            String mainLoc = (dot >= 0) ? location.substring(0, dot) : location;
            String subLoc = (dot >= 0) ? location.substring(dot + 1) : "";

            // Base dir: SCI/modules for core ("scilab.*"), toolbox root otherwise.
            String base = "scilab".equals(mainLoc) ? (SCI + "/modules") : toolboxRootForId(mainLoc);

            String command = null;
            if (base != null) {
                String file = base + "/" + path;
                switch (subLoc) {
                    case "xcos":
                        command = "xcos(\"" + file + "\");";
                        break;
                    case "scinotes":
                        command = "scinotes(\"" + file + "\");";
                        break;
                    case "demos":
                        command = "exec(\"" + file + ".sce\");";
                        break;
                    case "exec":
                        command = "exec(\"" + file + "\");";
                        break;
                    default:
                        break;
                }
            }

            if (command != null) {
                org.scilab.modules.action_binding.InterpreterManagement.putCommandInScilabQueue(command);
            } else if ("remote".equals(linkType) && (path.startsWith("http://") || path.startsWith("https://"))) {
                try {
                    Desktop.getDesktop().browse(URI.create(path));
                } catch (IOException e) {
                    // ignore
                }
            }
            callback.success("");
        }

        private void handleOpenExternal(String request, CefQueryCallback callback) {
            String url = extractJsonString(request, "url");
            if (url != null && (url.startsWith("http://") || url.startsWith("https://"))) {
                try {
                    Desktop.getDesktop().browse(URI.create(url));
                } catch (IOException e) {
                    // ignore
                }
            }
            callback.success("");
        }

        private void handleExecCode(String request, CefQueryCallback callback) {
            String code = extractJsonString(request, "code");
            if (code != null && !code.isEmpty()) {
                org.scilab.modules.action_binding.InterpreterManagement.putCommandInScilabQueue(code);
            }
            callback.success("");
        }

        private void handleEditCode(String request, CefQueryCallback callback) {
            String code = extractJsonString(request, "code");
            if (code != null && !code.isEmpty()) {
                final String finalCode = code;
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        try {
                            Class<?> scinotesClass = Class.forName("org.scilab.modules.scinotes.SciNotes");
                            java.lang.reflect.Method method = scinotesClass.getMethod("scinotesWithText", String.class);
                            method.invoke(null, finalCode);
                        } catch (Exception e) {
                            System.err.println("HelpBrowser: Failed to open SciNotes: " + e.getMessage());
                        }
                    }
                });
            }
            callback.success("");
        }

        private void handleCopyCode(String request, CefQueryCallback callback) {
            String code = extractJsonString(request, "code");
            if (code != null && !code.isEmpty()) {
                java.awt.datatransfer.StringSelection selection = new java.awt.datatransfer.StringSelection(code);
                java.awt.Toolkit.getDefaultToolkit().getSystemClipboard().setContents(selection, null);
            }
            callback.success("");
        }

        private void handleNavigateById(String id, CefQueryCallback callback) {
            // Navigate the iframe to the page matching this xml:id
            // The JS side will handle this via the TOC index
            callback.success(id != null ? id : "");
        }
    }

    // ================================================================
    // SimpleHelpBrowser interface
    // ================================================================

    @Override
    public void display() {
        // Nothing to do, the browser is already visible
    }

    @Override
    public void searchKeywork(String keyword) {
        if (keyword == null || keyword.isEmpty()) return;
        if (pageLoaded) {
            doNavigate(keyword);
        } else {
            pendingKeyword = keyword;
        }
    }

    /**
     * Re-evaluate the registered help chapters and, if the set of toolbox
     * chapters changed since the browser was created, reload the TOC tree in
     * the running browser. Called when add_help_chapter() refreshes an
     * already-open browser (e.g. toolboxes loaded by atomsAutoload after
     * session restoration reopened the help). The persistent toc.json is left
     * untouched; only the in-memory external chapters change, and reloadToc
     * re-fetches the merged TOC.
     */
    public void refresh(String[] helps) {
        java.util.List<ExtChapter> updated = resolveExternalChapters(helps);
        if (sameChapters(updated, this.externalChapters)) {
            return; // nothing changed, avoid a needless reload
        }
        this.externalChapters = updated;
        synchronized (this) {
            toolboxImagesManifest = null; // rebuilt from the new chapter set
        }
        if (pageLoaded) {
            browser.executeJavaScript(
                    "if(window.scilabHelp && window.scilabHelp.reloadToc) window.scilabHelp.reloadToc();",
                    "helpbrowser", 0);
        }
    }

    /** Compare two external-chapter lists by their (id, title, source dir) signature. */
    private static boolean sameChapters(java.util.List<ExtChapter> a, java.util.List<ExtChapter> b) {
        if (a.size() != b.size()) {
            return false;
        }
        java.util.Set<String> sigA = new java.util.HashSet<>();
        for (ExtChapter ch : a) {
            sigA.add(chapterSignature(ch));
        }
        for (ExtChapter ch : b) {
            if (!sigA.contains(chapterSignature(ch))) {
                return false;
            }
        }
        return true;
    }

    private static String chapterSignature(ExtChapter ch) {
        return ch.id + "\n" + ch.title + "\n" + ch.enUS.toString().replace('\\', '/').toLowerCase();
    }

    private void doNavigate(String keyword) {
        browser.executeJavaScript(
                "(function retry(n) {"
                + "  if(window.scilabHelp) { window.scilabHelp.navigateById(" + jsString(keyword) + "); }"
                + "  else if(n > 0) { setTimeout(function(){ retry(n-1); }, 100); }"
                + "})(50);",
                "helpbrowser", 0);
    }

    @Override
    public void fullTextSearch(String keyword) {
        if (keyword == null || keyword.isEmpty()) return;

        // Tell the JS shell to perform a search
        browser.executeJavaScript(
                "if(window.scilabHelp) window.scilabHelp.search(" + jsString(keyword) + ");",
                "helpbrowser", 0);
    }

    @Override
    public void close() {
        searchExecutor.shutdownNow();
        XConfiguration.removeXConfigurationListener(syntaxColorsListener);
        cefPanel.dispose();
    }

    // ================================================================
    // Additional methods for ScilabHelpBrowser integration
    // ================================================================

    /** Zoom the displayed document (not the shell); see shell.js / doczoom.js. */
    public void increaseFont() {
        browser.executeJavaScript(
                "if(window.scilabHelp) window.scilabHelp.zoomIn();",
                "helpbrowser", 0);
    }

    public void decreaseFont() {
        browser.executeJavaScript(
                "if(window.scilabHelp) window.scilabHelp.zoomOut();",
                "helpbrowser", 0);
    }

    public void resetFont() {
        browser.executeJavaScript(
                "if(window.scilabHelp) window.scilabHelp.resetZoom();",
                "helpbrowser", 0);
    }

    /** Open the find-in-page bar of the displayed document (Tools > Incremental Search). */
    public void showSearchField() {
        browser.executeJavaScript(
                "if(window.scilabHelp) window.scilabHelp.showFind();",
                "helpbrowser", 0);
    }

    public String getCurrentURL() {
        return browser.getURL();
    }

    public String getCurrentID() {
        return currentPageId;
    }

    // ================================================================
    // Utility
    // ================================================================

    /** Quoted JS/JSON string literal of s, safe to splice into executed JavaScript. */
    private static String jsString(String s) {
        return new Gson().toJson(s);
    }

    /**
     * Extract a string value from a JSON object, or null if the request is
     * not valid JSON or has no such string member.
     */
    private static String extractJsonString(String json, String key) {
        try {
            JsonElement root = JsonParser.parseString(json);
            if (!root.isJsonObject()) return null;
            JsonElement value = root.getAsJsonObject().get(key);
            if (value == null || !value.isJsonPrimitive() || !value.getAsJsonPrimitive().isString()) return null;
            return value.getAsString();
        } catch (JsonParseException e) {
            return null;
        }
    }
}
