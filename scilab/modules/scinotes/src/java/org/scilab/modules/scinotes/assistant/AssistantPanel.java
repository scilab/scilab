package org.scilab.modules.scinotes.assistant;

import org.cef.browser.CefBrowser;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.handler.CefContextMenuHandlerAdapter;
import org.cef.handler.CefLifeSpanHandlerAdapter;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;
import org.cef.browser.CefFrame;

import java.awt.BorderLayout;
import java.io.File;
import java.io.IOException;
import java.util.logging.Logger;

import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.utils.WebBrowser;

/**
 * Assistant chat panel using JCEF.
 * Each instance owns its own browser — multiple SciNotes windows
 * can each have their own assistant panel.
 *
 * The embedded ScilabCefPanel brings the focus management shared with the
 * console and Monaco, the resize masking, the external and "scilab:" links,
 * and a dispose() which waits for the browser to be closed by CEF. It is
 * wrapped rather than extended because ScilabCefPanel is final.
 *
 * Shared resources (MCP servers, help index) are managed statically.
 */
public final class AssistantPanel extends JPanel {

    private static final long serialVersionUID = 1L;
    private static final Logger LOG = Logger.getLogger(AssistantPanel.class.getName());

    private final ScilabCefPanel cef;

    /**
     * Create a new assistant panel with its own JCEF browser.
     * @throws IOException if GUI resources cannot be found
     */
    public AssistantPanel() throws IOException {
        super(new BorderLayout());
        cef = new ScilabCefPanel(startAndGetUrl());
        cef.setFocusTraversalKeysEnabled(false);
        // DevTools always available (CtrlShiftI)
        cef.setDebug(true);
        add(cef, BorderLayout.CENTER);

        // Message router
        // The JS function names are global to all CEF browsers: do not use
        // "toScilab", already defined by ScilabBrowser.js for browser uicontrols
        CefMessageRouterConfig config = new CefMessageRouterConfig("assistantQuery", "assistantQueryCancel");
        CefMessageRouter router = CefMessageRouter.create(config, new AssistantMessageHandler());
        cef.getClient().addMessageRouter(router);

        setupLifeSpanHandler();
        setupLoadHandler();
        setupContextMenuHandler();
    }

    /** Start the shared servers, then build the URL of the chat page. */
    private static String startAndGetUrl() throws IOException {
        ensureServersStarted();
        return new File(resolveGuiPath(), "index.html").toURI()
               + "?js=" + java.net.URLEncoder.encode(resolveJsUrl(), "UTF-8");
    }

    /** Listener called when the browser page is fully loaded. */
    private Runnable onReadyCallback;

    /** Set a callback to be invoked when the browser page is loaded and ready. */
    public void setOnReady(Runnable callback) {
        this.onReadyCallback = callback;
    }

    /**
     * Send a JSON event to the browser frontend.
     * @param jsonData a JSON string to pass to the JS fromScilab() function
     */
    public void sendEvent(String jsonData) {
        String escaped = jsonData
            .replace("\\", "\\\\")
            .replace("\"", "\\\"")
            .replace("\n", "\\n")
            .replace("\r", "\\r")
            .replace("\t", "\\t");
        cef.executeJS("if(typeof fromScilab==='function')fromScilab(JSON.parse(\"" + escaped + "\"));",
                  "assistant");
    }

    /** Give the keyboard focus to the chat page. */
    public void requestBrowserFocus() {
        cef.requestBrowserFocus();
    }

    /** Close the browser, waiting for CEF to release it. */
    public void dispose() {
        cef.dispose();
    }

    // -- Shared MCP servers (static, one per JVM) --

    private static boolean serversStarted = false;

    private static synchronized void ensureServersStarted() {
        if (serversStarted) return;

        CommandServer.start();
        McpSseServer.start();
        LOG.info("MCP servers started (SSE port " + McpSseServer.getPort() + ")");

        // Build help index in background
        Thread helpThread = new Thread(() -> {
            ScilabHelpIndex idx = ScilabHelpIndex.getInstance();
            if (!idx.isBuilt()) {
                idx.ensureBuilt();
            }
        }, "HelpIndexBuilder");
        helpThread.setDaemon(true);
        helpThread.start();

        serversStarted = true;
    }

    private static final String OPEN_WITH_SCINOTES = "open_with_scinotes";
    private static final String PANEL_WIDTH = "panel_width";

    /** Stop shared servers (called on Scilab exit). */
    public static void stopServers() {
        McpSseServer.stop();
        CommandServer.stop();
        serversStarted = false;
    }

    /**
     * Called when the last SciNotes closes: kill the AI CLIs still running
     * (they would survive Scilab as orphan processes) and stop the servers.
     */
    public static void shutdown() {
        AgentProcess.cancelAll();
        if (serversStarted) {
            stopServers();
        }
    }

    /**
     * Whether SciNotes opens the assistant with its first window: true
     * until the user closes the panel, to make it known, then whatever
     * state the user left it in.
     */
    public static boolean isOpenWithSciNotes() {
        return AssistantSettings.getBool(OPEN_WITH_SCINOTES, true);
    }

    /** Remember whether the user left the assistant panel open or closed. */
    public static void setOpenWithSciNotes(boolean open) {
        if (isOpenWithSciNotes() != open) {
            AssistantSettings.setBool(OPEN_WITH_SCINOTES, open);
        }
    }

    /** Width of the panel last set by the user, or -1 when never set. */
    public static int getSavedWidth() {
        return AssistantSettings.getInt(PANEL_WIDTH, -1);
    }

    /** Remember the width of the panel set by the user. */
    public static void saveWidth(int width) {
        if (getSavedWidth() != width) {
            AssistantSettings.setInt(PANEL_WIDTH, width);
        }
    }

    // -- Setup helpers --

    private void setupLoadHandler() {
        cef.addLoadHandler(new CefLoadHandlerAdapter() {
            @Override
            public void onLoadEnd(CefBrowser b, CefFrame frame, int httpStatusCode) {
                if (b == cef.getBrowser() && onReadyCallback != null) {
                    SwingUtilities.invokeLater(onReadyCallback);
                }
            }
        });
    }

    private void setupLifeSpanHandler() {
        cef.addLifeSpanHandler(new CefLifeSpanHandlerAdapter() {
            @Override
            public boolean onBeforePopup(CefBrowser b, CefFrame f, String url, String name) {
                // Links opening a new window (target="_blank") go to the
                // default browser
                if (b == cef.getBrowser() && url != null && !url.isEmpty()) {
                    WebBrowser.openUrl(url);
                    return true;
                }
                return false;
            }
        });
    }

    private void setupContextMenuHandler() {
        cef.addContextMenuHandler(new CefContextMenuHandlerAdapter() {
            @Override
            public void onBeforeContextMenu(CefBrowser b, CefFrame f,
                                             CefContextMenuParams params, CefMenuModel model) {
                if (b == cef.getBrowser()) {
                    model.clear();
                }
            }
        });
    }

    // -- Resolve GUI resources path --

    private static String resolveGuiPath() throws IOException {
        String sciDir = null;
        if (ScilabConstants.SCI != null) {
            sciDir = ScilabConstants.SCI.getAbsolutePath();
        }
        if (sciDir == null) {
            throw new IOException("SCI directory not available");
        }

        String guiDir = (sciDir + "/modules/scinotes/gui/assistant").replace("\\", "/");

        if (!new File(guiDir).isDirectory()) {
            throw new IOException("GUI resources not found at: " + guiDir);
        }
        return guiDir;
    }

    /**
     * URL of the thirdparty JS libraries (marked, highlight, purify):
     * SCI/thirdparty/web/js in a source tree or on Windows, SCI/../../thirdparty
     * in an installed Linux/macOS build (same rule as the help browser).
     */
    private static String resolveJsUrl() {
        File js = new File(ScilabConstants.SCI, "thirdparty/web");
        if (!js.isDirectory()) {
            File installed = new File(ScilabConstants.SCI, "../../thirdparty/web");
            if (installed.isDirectory()) {
                js = installed;
            }
        }
        try {
            js = js.getCanonicalFile();
        } catch (IOException e) {
            // keep the non-canonical path
        }
        return js.toURI().toString().replaceAll("/$", "");
    }
}
