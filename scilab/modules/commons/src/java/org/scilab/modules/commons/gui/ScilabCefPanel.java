/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.KeyboardFocusManager;
import java.io.File;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.logging.Logger;

import javax.swing.JPanel;
import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import org.cef.CefClient;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.handler.CefContextMenuHandler;
import org.cef.handler.CefContextMenuHandlerAdapter;
import org.cef.handler.CefFocusHandler;
import org.cef.handler.CefFocusHandlerAdapter;
import org.cef.handler.CefKeyboardHandler;
import org.cef.handler.CefKeyboardHandlerAdapter;
import org.cef.handler.CefLifeSpanHandler;
import org.cef.handler.CefLifeSpanHandlerAdapter;
import org.cef.handler.CefLoadHandler.ErrorCode;
import org.cef.browser.CefFrame;
import org.cef.callback.CefAuthCallback;
import org.cef.callback.CefCallback;
import org.cef.callback.CefContextMenuParams;
import org.cef.callback.CefMenuModel;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.cef.handler.CefRequestHandler;
import org.cef.handler.CefRequestHandlerAdapter;
import org.cef.handler.CefResourceRequestHandler;
import org.cef.handler.CefResourceRequestHandlerAdapter;
import org.cef.misc.BoolRef;
import org.cef.misc.EventFlags;
import org.cef.network.CefRequest;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.view.View;
import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.utils.ScilabBrowser;
import org.scilab.modules.commons.utils.WebBrowser;

/**
 * Reusable JPanel embedding a JCEF browser with proper focus management
 * for Scilab's FlexDock docking framework.
 *
 * <p>Handles:
 * <ul>
 *   <li>JCEF browser creation and lifecycle</li>
 *   <li>Focus transfer between Chromium's heavyweight HWND and Swing</li>
 *   <li>FlexDock panel activation on browser focus</li>
 *   <li>Context menu suppression (default JCEF menu)</li>
 *   <li>DevTools toggle (CtrlShiftI)</li>
 *   <li>Tab traversal out of the browser</li>
 * </ul>
 *
 * <p>Usage:
 * <pre>
 *   ScilabCefPanel panel = new ScilabCefPanel(url);
 *   panel.getClient().addLoadHandler(...);  // add custom handlers
 *   panel.getBrowser().executeJavaScript(...);
 *   // when done:
 *   panel.destroy();
 * </pre>
 */
public class ScilabCefPanel extends JPanel {

    private static final long serialVersionUID = 1L;
    private static final Logger log = Logger.getLogger("org.scilab.modules.commons.gui.ScilabCefPanel");
    public static final String DOCKABLE_FOCUS_FROM_BROWSER_PROPERTY = ScilabCefPanel.class.getName() + ".dockableFocusFromBrowser";
    public static final String CEF_NATIVE_FOCUS_PROPERTY = ScilabCefPanel.class.getName() + ".cefNativeFocus";

    private CefClient client;
    private CefBrowser browser;
    private boolean debug = false;

    /**
     * Browser backing the DevTools window, non-null only once CEF reported it
     * created and until it reports it closed. This is the only trustworthy
     * "DevTools are open" state: JCEF exposes none, and getDevToolsClient()
     * creates a CDP client on demand, so it always answers "open".
     */
    private volatile CefBrowser devToolsBrowser = null;

    /** Set between an open request and the DevTools browser showing up. */
    private volatile boolean devToolsRequested = false;

    /** True when Chromium has native focus. */
    private volatile boolean browserHasFocus = false;

    /** Incremented whenever a pending Swing focus transfer must be cancelled. */
    private volatile int focusTransferGeneration = 0;

    /** True while waiting for CEF to acknowledge a browser focus request. */
    private volatile boolean browserFocusRequested = false;

    /** FlexDock view currently marked as holding native CEF focus for this panel. */
    private View cefNativeFocusView = null;

    /** Single FlexDock view marked as holding native CEF focus across all CEF panels. */
    private static View currentCefNativeFocusView = null;

    /** >0 when a dock operation is in-progress */
    private static volatile int isDocking = 0;

    /** Global AWT listener to detect clicks outside the browser. */
    private java.awt.event.AWTEventListener clickOutsideListener;

    /**
     * Invisible heavyweight AWT Canvas whose sole purpose is to own a
     * native HWND that can steal OS keyboard focus from Chromium's HWND.
     * Lightweight Swing components cannot do this because they share
     * the JFrame's native window.
     */
    private final java.awt.Canvas nativeFocusSink = new java.awt.Canvas();

    /* Default implementations of handlers */

    // ---- Keyboard: CtrlShiftI for DevTools ----
    final CefKeyboardHandler keyboardHandler = new CefKeyboardHandlerAdapter() {
        @Override
        public boolean onKeyEvent(CefBrowser b, CefKeyEvent event) {
            if (b != browser) return false;
            boolean isMac = OS.get() == OS.MAC;
            boolean isCTRL = !isMac && (event.modifiers & EventFlags.EVENTFLAG_CONTROL_DOWN) != 0;
            boolean isCOMMAND = isMac && (event.modifiers & EventFlags.EVENTFLAG_COMMAND_DOWN) != 0;
            boolean isSHIFT = (event.modifiers & EventFlags.EVENTFLAG_SHIFT_DOWN) != 0;
            // Compare the virtual key code: on a RAWKEYDOWN, Chromium fills the
            // character fields with the key code itself ('I'), not with the
            // typed character
            if (event.type == CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN
                    && (isCTRL || isCOMMAND) && isSHIFT
                    && event.windows_key_code == KeyEvent.VK_I) {
                if (debug) {
                    toggleDevTools();
                }
                return true;
            }
            return false;
        }
    };
    
    // ---- Context menu: suppress default JCEF menu ----
   final CefContextMenuHandler contextMenuHandler = new CefContextMenuHandlerAdapter() {
        @Override
        public void onBeforeContextMenu(CefBrowser b, CefFrame frame,
                                        CefContextMenuParams params, CefMenuModel model) {
            if (b == browser && model.getCount() == 5
                    && model.getLabel(model.getCommandIdAt(0)).equals("&Back")
                    && model.getLabel(model.getCommandIdAt(1)).equals("&Forward")) {
                model.clear();
            }
        }
    };

    // ---- Focus management ----
    final CefFocusHandler focusHandler = new CefFocusHandlerAdapter() {
        @Override
        public boolean onSetFocus(CefBrowser b, CefFocusHandler.FocusSource source) {
            if (b != browser) return false;
            // Block navigation/JS-triggered focus grabs.
            // Allow FOCUS_SOURCE_SYSTEM so clicking on the browser
            // content naturally focuses the correct HTML element.
            return source == CefFocusHandler.FocusSource.FOCUS_SOURCE_NAVIGATION;
        }

        @Override
        public void onGotFocus(CefBrowser b) {
            if (b != browser) return;
            browserHasFocus = true;
            browserFocusRequested = false;
            final int focusTransfer = ++focusTransferGeneration;
            // Notify FlexDock so it updates title bars, menus, etc.
            SwingUtilities.invokeLater(() -> {
                if (!browserHasFocus || focusTransfer != focusTransferGeneration) {
                    return;
                }

                Component ancestor = SwingUtilities.getAncestorOfClass(View.class, ScilabCefPanel.this);
                if (ancestor != null) {
                    if (ancestor instanceof JComponent) {
                        ((JComponent) ancestor).putClientProperty(DOCKABLE_FOCUS_FROM_BROWSER_PROPERTY, Boolean.TRUE);
                    }
                    if (ancestor instanceof View) {
                        setCefNativeFocusView((View) ancestor);
                    }
                    ActiveDockableTracker.requestDockableActivation(ancestor);
                }
            });
        }

        @Override
        public void onTakeFocus(CefBrowser b, boolean next) {
            if (b != browser) return;
            browserHasFocus = false;
            browserFocusRequested = false;
            clearCefNativeFocusView();
            // Tab traversal out of the browser
            SwingUtilities.invokeLater(() -> {
                KeyboardFocusManager mgr = KeyboardFocusManager.getCurrentKeyboardFocusManager();
                if (next) {
                    mgr.focusNextComponent(ScilabCefPanel.this);
                } else {
                    mgr.focusPreviousComponent(ScilabCefPanel.this);
                }
            });
        }
    };

    // ---- External Request management ----
    // Open the default system browser for external links (http/https)
    // Resolve SCI paths to real files on disk (file://)
    public final static class ScilabResourceHandler extends CefResourceRequestHandlerAdapter {
        @Override
        public boolean onBeforeResourceLoad(CefBrowser browser, CefFrame frame, CefRequest request) {
            String url = request.getURL();

            if (url != null && url.startsWith("file:///SCI/")) {
                String relativePath = url.substring("file:///SCI/".length());

                File targetFile = new File(ScilabConstants.SCI.getPath(), relativePath);
                String realFileUrl = targetFile.toURI().toString();

                if (log.isLoggable(java.util.logging.Level.FINE)) {
                    log.fine("Redirecting SCI path: " + url + " -> " + realFileUrl);
                }

                // JCEF / CEF internal redirect
                request.setURL(realFileUrl);
            }

            // continue with modified URL
            return false;
        }
    }
    final CefRequestHandler requestHandler = new CefRequestHandlerAdapter() {
        @Override
        public CefResourceRequestHandler getResourceRequestHandler(CefBrowser browser, CefFrame frame, CefRequest request, boolean isNavigation, boolean isDownload, String requestInitiator, BoolRef disableDefaultHandling) {
            String url = request.getURL();
            // Open external links in the default browser
            if (isNavigation && (url.startsWith("https://") || url.startsWith("http://"))) {
                WebBrowser.openUrl(request.getURL());
                disableDefaultHandling.set(true); // Prevent JCEF from handling the URL
            }
            return new ScilabResourceHandler();
        }
    };

    // ---- Life span: track the DevTools window ----
    final CefLifeSpanHandler lifeSpanHandler = new CefLifeSpanHandlerAdapter() {
        @Override
        public void onAfterCreated(CefBrowser b) {
            // The DevTools browser is created on this very client, so its
            // life span events land here. Claim the first browser created
            // while an open is pending: this is what makes the toggle able
            // to tell an open DevTools window from a failed open.
            if (b != browser && devToolsRequested && devToolsBrowser == null) {
                devToolsRequested = false;
                devToolsBrowser = b;
            }
        }

        @Override
        public void onBeforeClose(CefBrowser b) {
            // Closing the window from its own UI comes through here too, so
            // the toggle stays in sync with what the user sees
            if (b == devToolsBrowser) {
                devToolsBrowser = null;
                devToolsRequested = false;
            }
        }
    };

    /* Handler lists, to let the user add them */
    ArrayList<CefKeyboardHandler> keyboardHandlers = null;
    ArrayList<CefContextMenuHandler> contextMenuHandlers = null;
    ArrayList<CefFocusHandler> focusHandlers = null;
    ArrayList<CefRequestHandler> requestHandlers = null;
    ArrayList<CefLifeSpanHandler> lifeSpanHandlers = null;

    /**
     * Create a new panel embedding a JCEF browser.
     * @param url initial URL to load, or empty string for about:blank
     */
    public ScilabCefPanel(String url) {
        super(new BorderLayout(0, 0));
        client = ScilabBrowser.get();
        browser = client.createBrowser(url, false, false);

        client.addKeyboardHandler(keyboardHandler);
        client.addContextMenuHandler(contextMenuHandler);
        client.addFocusHandler(focusHandler);
        client.addRequestHandler(requestHandler);
        client.addLifeSpanHandler(lifeSpanHandler);

        // ---- Layout ----
        setBackground(Color.WHITE);
        add(browser.getUIComponent(), BorderLayout.CENTER);

        // Invisible heavyweight Canvas for native focus management
        nativeFocusSink.setFocusable(true);
        nativeFocusSink.setPreferredSize(new java.awt.Dimension(0, 0));
        nativeFocusSink.setMinimumSize(new java.awt.Dimension(0, 0));
        nativeFocusSink.setMaximumSize(new java.awt.Dimension(0, 0));
        add(nativeFocusSink, BorderLayout.SOUTH);

        // ---- Click-outside detection ----
        // When the browser has focus and the user clicks on another
        // component, release Chromium focus and transfer to the target.
        clickOutsideListener = event -> {
            if (!browserHasFocus) return;
            if (event.getID() != java.awt.event.MouseEvent.MOUSE_PRESSED) return;
            java.awt.event.MouseEvent me = (java.awt.event.MouseEvent) event;
            Component src = me.getComponent();
            if (src == null || !isShowing()) return;

            // The toolkit reports the same click twice: first targeted at the
            // heavyweight frame, then retargeted at the deep lightweight
            // component. Resolve the deepest component ourselves so the first
            // event is already conclusive, then walk up to a focusable target.
            // Never aim at the Window itself: Window.requestFocusInWindow()
            // hands focus back to its most recent owner - the browser.
            Component target = SwingUtilities.getDeepestComponentAt(src, me.getX(), me.getY());
            if (target == null) {
                target = src;
            }
            if (SwingUtilities.isDescendingFrom(target, ScilabCefPanel.this)) return;
            while (target != null && !(target instanceof java.awt.Window) && !target.isFocusable()) {
                target = target.getParent();
            }
            if (target == null || target instanceof java.awt.Window) return;

            browserHasFocus = false;
            browserFocusRequested = false;
            final int focusTransfer = ++focusTransferGeneration;
            clearCefNativeFocusView();
            browser.setFocus(false);
            // Only steal native focus when both components are in the
            // same window.  For separate windows the OS handles it.
            if (SwingUtilities.getWindowAncestor(target) == SwingUtilities.getWindowAncestor(ScilabCefPanel.this)) {
                final Component focusTarget = target;
                final class Sink implements Runnable {
                    private int attempts = 0;
                    @Override
                    public void run() {
                        if (browserHasFocus || focusTransfer != focusTransferGeneration) {
                            return;
                        }
                        if (nativeFocusSink.hasFocus()) {
                            focusTarget.requestFocusInWindow();
                        } else if (++attempts < 10) {
                            // If the focus sink failed, try again after the next repaint cycle.
                            nativeFocusSink.requestFocus();
                            SwingUtilities.invokeLater(this);
                        }
                    }
                }
                nativeFocusSink.requestFocus();
                SwingUtilities.invokeLater(new Sink());
            }
        };
        java.awt.Toolkit.getDefaultToolkit().addAWTEventListener(
            clickOutsideListener, java.awt.AWTEvent.MOUSE_EVENT_MASK);
    }

    private void setCefNativeFocusView(View view) {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(() -> setCefNativeFocusView(view));
            return;
        }

        cefNativeFocusView = view;

        synchronized (ScilabCefPanel.class) {
            if (currentCefNativeFocusView == view) {
                repaintTitlebar(view);
                return;
            }

            clearCefNativeFocusView(currentCefNativeFocusView);
            currentCefNativeFocusView = view;
            view.putClientProperty(CEF_NATIVE_FOCUS_PROPERTY, Boolean.TRUE);
            repaintTitlebar(view);
        }
    }

    private void clearCefNativeFocusView() {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(() -> clearCefNativeFocusView());
            return;
        }

        View view = cefNativeFocusView;
        cefNativeFocusView = null;

        synchronized (ScilabCefPanel.class) {
            if (currentCefNativeFocusView == view) {
                clearCefNativeFocusView(view);
                currentCefNativeFocusView = null;
            }
        }
    }

    private static void clearCefNativeFocusView(View view) {
        if (view == null) {
            return;
        }

        view.putClientProperty(CEF_NATIVE_FOCUS_PROPERTY, null);
        repaintTitlebar(view);
    }

    private static void repaintTitlebar(View view) {
        if (view != null && view.getTitlebar() != null) {
            view.getTitlebar().repaint();
        }
    }

    // ====================================================================
    // Public API
    // ====================================================================

    /** @return the CefClient for adding custom handlers */
    public CefClient getClient() {
        return client;
    }

    /** @return the CefBrowser instance */
    public CefBrowser getBrowser() {
        return browser;
    }

    /** Load a URL in the browser */
    public void loadURL(String url) {
        browser.loadURL(url);
    }

    /** Execute JavaScript in the browser */
    public void executeJS(String code, String sourceUrl) {
        if (browser != null) {
            browser.executeJavaScript(code, sourceUrl, 0);
        }
    }

    /**
     * Give native keyboard focus back to Chromium after a focus request coming
     * from the web page itself. This also cancels any pending delayed transfer
     * to a Swing component scheduled by the click-outside handler.
     */
    public void requestBrowserFocus() {
        focusTransferGeneration++;
        browser.getUIComponent().requestFocusInWindow();

        if (!browserHasFocus && !browserFocusRequested) {
            browserFocusRequested = true;
            browser.setFocus(true);
        }
    }

    /** Enable or disable DevTools (CtrlShiftI) */
    public void setDebug(boolean enabled) {
        this.debug = enabled;
    }

    /** @return true if DevTools are currently open */
    public boolean isDebug() {
        return debug;
    }

    /** Add a message router with the standard Scilab/ScilabAbort prefix */
    public CefMessageRouter addMessageRouter(CefMessageRouterHandlerAdapter handler) {
        CefMessageRouterConfig config = new CefMessageRouterConfig("Scilab", "ScilabAbort");
        CefMessageRouter router = CefMessageRouter.create(config, handler);
        client.addMessageRouter(router);
        return router;
    }

    public void addKeyboardHandler(CefKeyboardHandler h)
    {
        if (keyboardHandlers == null)
        {
            keyboardHandlers = new ArrayList<>();
            keyboardHandlers.add(keyboardHandler);

            // CefClient.addXHandler is a no-op when a handler is already
            // registered: drop the constructor one before installing the
            // dispatcher, otherwise the handlers added here never fire
            client.removeKeyboardHandler();
            client.addKeyboardHandler(new CefKeyboardHandler() {
                @Override
                public boolean onPreKeyEvent(CefBrowser browser, CefKeyEvent event, BoolRef is_keyboard_shortcut) {
                    for (CefKeyboardHandler h : keyboardHandlers)
                    {
                        if (h.onPreKeyEvent(browser, event, is_keyboard_shortcut))
                            return true;
                    }
                    return false;
                }
                @Override
                public boolean onKeyEvent(CefBrowser browser, CefKeyEvent event) {
                    for (CefKeyboardHandler h : keyboardHandlers)
                    {
                        if (h.onKeyEvent(browser, event))
                            return true;
                    }
                    return false;
                }
            });
        }
        keyboardHandlers.add(h);
    }

    public void addContextMenuHandler(CefContextMenuHandler h)
    {
        if (contextMenuHandlers == null)
        {
            contextMenuHandlers = new ArrayList<>();
            contextMenuHandlers.add(contextMenuHandler);

            client.removeContextMenuHandler();
            client.addContextMenuHandler(new CefContextMenuHandler() {
                @Override
                public void onBeforeContextMenu(CefBrowser browser, CefFrame frame, CefContextMenuParams params,
                        CefMenuModel model) {
                    for (CefContextMenuHandler h : contextMenuHandlers)
                    {
                        h.onBeforeContextMenu(browser, frame, params, model);
                    }
                }
                @Override
                public boolean onContextMenuCommand(CefBrowser browser, CefFrame frame, CefContextMenuParams params,
                        int commandId, int eventFlags) {
                    for (CefContextMenuHandler h : contextMenuHandlers)
                    {
                        if(h.onContextMenuCommand(browser, frame, params, commandId, eventFlags))
                            return true;
                    }
                    return false;
                }
                @Override
                public void onContextMenuDismissed(CefBrowser browser, CefFrame frame) {
                    for (CefContextMenuHandler h : contextMenuHandlers)
                    {
                        h.onContextMenuDismissed(browser, frame);
                    }
                }
            });
        }
        contextMenuHandlers.add(h);
    }

    public void addFocusHandler(CefFocusHandler h)
    {
        if (focusHandlers == null)
        {
            focusHandlers = new ArrayList<>();
            focusHandlers.add(focusHandler);
            client.removeFocusHandler();
            client.addFocusHandler(new CefFocusHandler() {
                @Override
                public void onTakeFocus(CefBrowser browser, boolean next) {
                    for (CefFocusHandler h : focusHandlers)
                    {
                        h.onTakeFocus(browser, next);
                    }
                }
                @Override
                public boolean onSetFocus(CefBrowser browser, FocusSource source) {
                    for (CefFocusHandler h : focusHandlers)
                    {
                        if (h.onSetFocus(browser, source))
                            return true;
                    }
                    return false;
                }
                @Override
                public void onGotFocus(CefBrowser browser) {
                    for (CefFocusHandler h : focusHandlers) {
                        h.onGotFocus(browser);
                    }
                }
            });
        }
        focusHandlers.add(h);
    }

    public void addRequestHandler(CefRequestHandler h)
    {
        if (requestHandlers == null) {
            requestHandlers = new ArrayList<>();
            requestHandlers.add(requestHandler);
            client.removeRequestHandler();
            client.addRequestHandler(new CefRequestHandler() {
                @Override
                public boolean onBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request,
                        boolean user_gesture, boolean is_redirect) {
                    for (CefRequestHandler h : requestHandlers) {
                        if (h.onBeforeBrowse(browser, frame, request, user_gesture, is_redirect))
                            return true;
                    }
                    return false;
                }
                @Override
                public boolean onOpenURLFromTab(CefBrowser browser, CefFrame frame, String target_url,
                        boolean user_gesture) {
                    for (CefRequestHandler h : requestHandlers) {
                        if (h.onOpenURLFromTab(browser, frame, target_url, user_gesture))
                            return true;
                    }
                    return false;
                }
                @Override
                public CefResourceRequestHandler getResourceRequestHandler(CefBrowser browser, CefFrame frame,
                        CefRequest request, boolean isNavigation, boolean isDownload, String requestInitiator,
                        BoolRef disableDefaultHandling) {
                    for (CefRequestHandler h : requestHandlers) {
                        CefResourceRequestHandler handler = h.getResourceRequestHandler(browser, frame, request,
                                isNavigation, isDownload, requestInitiator, disableDefaultHandling);
                        if (handler != null)
                            return handler;
                    }
                    return null;
                }
                @Override
                public boolean getAuthCredentials(CefBrowser browser, String origin_url, boolean isProxy, String host,
                        int port, String realm, String scheme, CefAuthCallback callback) {
                    for (CefRequestHandler h : requestHandlers) {
                        if (h.getAuthCredentials(browser, origin_url, isProxy, host, port, realm, scheme, callback))
                            return true;
                    }
                    return false;
                }

                @Override
                public boolean onCertificateError(CefBrowser browser, ErrorCode cert_error, String request_url,
                        CefCallback callback) {
                    for (CefRequestHandler h : requestHandlers) {
                        if (h.onCertificateError(browser, cert_error, request_url, callback))
                            return true;
                    }
                    return false;
                }

                @Override
                public void onRenderProcessTerminated(CefBrowser browser, TerminationStatus status, int error_code,
                        String error_string) {
                    for (CefRequestHandler h : requestHandlers) {
                        h.onRenderProcessTerminated(browser, status, error_code, error_string);
                    }
                }
            });
        }
        requestHandlers.add(h);
    }

    public void addLifeSpanHandler(CefLifeSpanHandler h)
    {
        if (lifeSpanHandlers == null) {
            lifeSpanHandlers = new ArrayList<>();
            lifeSpanHandlers.add(lifeSpanHandler);
            client.removeLifeSpanHandler();
            client.addLifeSpanHandler(new CefLifeSpanHandler() {
                @Override
                public boolean onBeforePopup(CefBrowser browser, CefFrame frame, String target_url,
                        String target_frame_name) {
                    for (CefLifeSpanHandler h : lifeSpanHandlers) {
                        if (h.onBeforePopup(browser, frame, target_url, target_frame_name))
                            return true;
                    }
                    return false;
                }
                @Override
                public void onAfterCreated(CefBrowser browser) {
                    for (CefLifeSpanHandler h : lifeSpanHandlers) {
                        h.onAfterCreated(browser);
                    }
                }
                @Override
                public void onAfterParentChanged(CefBrowser browser) {
                    for (CefLifeSpanHandler h : lifeSpanHandlers) {
                        h.onAfterParentChanged(browser);
                    }
                }
                @Override
                public boolean doClose(CefBrowser browser) {
                    for (CefLifeSpanHandler h : lifeSpanHandlers) {
                        if (h.doClose(browser))
                            return true;
                    }
                    return false;
                }
                @Override
                public void onBeforeClose(CefBrowser browser) {
                    for (CefLifeSpanHandler h : lifeSpanHandlers) {
                        h.onBeforeClose(browser);
                    }
                }
            });
        }
        lifeSpanHandlers.add(h);
    }

    /** Toggle DevTools window */
    public void toggleDevTools() {
        // Decide on the browser CEF actually created, never on a request we
        // merely sent: an open that silently failed would otherwise leave the
        // toggle inverted, and the next keystroke would close nothing
        if (devToolsBrowser != null) {
            closeDevTools();
        } else {
            openDevTools();
        }
    }

    /** Open DevTools (if not already open) */
    public void openDevTools() {
        devToolsRequested = true;
        browser.openDevTools();
    }

    /** Close DevTools (if open) */
    public void closeDevTools() {
        devToolsRequested = false;
        devToolsBrowser = null;
        browser.closeDevTools();
    }

    public static void setDockInProgress(boolean inProgress)
    {
        if (inProgress) {
            isDocking++;
        } else {
            isDocking--;
        }
    }

    /**
     * Dispose JCEF resources and remove global listeners.
     * will be called when the panel is no longer needed.
     */
    @Override
    public void removeNotify() {
        super.removeNotify();

        // do not release while docking
        if (isDocking > 0) {
            return;
        }


        if (clickOutsideListener != null) {
            java.awt.Toolkit.getDefaultToolkit().removeAWTEventListener(clickOutsideListener);
            clickOutsideListener = null;
        }

        clearCefNativeFocusView();

        if (browser != null) {
            closeDevTools();
            browser.close(true);
            browser = null;
        }

        if (client != null) {
            ScilabBrowser.release(client);
            client = null;
        }
    }
}
