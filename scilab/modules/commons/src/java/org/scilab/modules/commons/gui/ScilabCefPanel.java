/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.KeyboardFocusManager;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.AWTEventListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyEvent;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Base64;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.JPanel;
import javax.swing.JComponent;
import javax.swing.JSplitPane;
import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.plaf.basic.BasicSplitPaneDivider;

import org.cef.CefClient;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefDevToolsClient;
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
import org.cef.handler.CefLoadHandler;
import org.cef.handler.CefLoadHandler.ErrorCode;
import org.cef.handler.CefLoadHandlerAdapter;
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
import org.scilab.modules.commons.utils.ScilabLink;
import org.scilab.modules.commons.utils.WebBrowser;

import com.google.gson.JsonElement;
import com.google.gson.JsonParser;

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

    /** Browsers closed by dispose() whose native close has not completed yet. */
    private static final Set<CefBrowser> closingBrowsers = new HashSet<>();

    /** Max time (ms) dispose() waits for closing browsers. */
    private static final long CLOSING_BROWSERS_TIMEOUT_MS = 500;

    /** Global AWT listener to detect clicks outside the browser. */
    private AWTEventListener clickOutsideListener;

    /**
     * Invisible heavyweight AWT Canvas whose sole purpose is to own a
     * native HWND that can steal OS keyboard focus from Chromium's HWND.
     * Lightweight Swing components cannot do this because they share
     * the JFrame's native window.
     */
    private final Canvas nativeFocusSink = new Canvas();

    /* ---- live-resize masking (see constructor) ---- */
    private static final String CARD_BROWSER = "browser";
    private static final String CARD_COVER = "cover";
    /**
     * Delay (ms) after the last resize step that ends a window-frame resize.
     * (longer on macOS, where the native view is only repositioned on release).
     */
    private static final int RESIZE_SETTLE_MS = (OS.get() == OS.MAC) ? 1500 : 150;
    /** Min quiet time (ms) after the last resize step for a MOUSE_MOVED to mean "drag released". */
    private static final int MOVE_END_QUIET_MS = 80;
    /** After a mouse release, skip masking this long: a divider/dock drag's single post-release resize needs none. */
    private static final int POST_RELEASE_NO_MASK_MS = 400;
    /** Quiet time (ms) after the last change before the snapshot is refreshed. */
    private static final int SNAPSHOT_REFRESH_MS = 250;
    /** Last time the page content changed. Written from any thread, cheaply. */
    private volatile long contentChangedMs = 0;
    /** contentChangedMs of the last capture, so we only capture on a change. */
    private long capturedChangeMs = 0;
    private CardLayout cardLayout;
    private JPanel cardPanel;
    private final ResizeCover resizeCover = new ResizeCover();
    private AWTEventListener resizeMouseListener;
    private Timer resizeSettleTimer;
    private Timer snapshotRefreshTimer;
    private long lastResizeStepMs = 0;
    /** Time of the last mouse release, to skip masking the single resize it triggers. */
    private volatile long lastMouseReleaseMs = 0;
    /** Last page snapshot, refreshed while idle so a gesture can mask instantly. */
    private volatile BufferedImage cachedSnapshot;
    /** True while a mouse button is held. */
    private volatile boolean mouseDown = false;
    /** True while the snapshot cover is masking the browser. */
    private boolean resizing = false;

    /* Default implementations of handlers */

    // ---- Keyboard: Ctrl+Shift+I for DevTools, Ctrl+Shift+R to reload ----
    final CefKeyboardHandler keyboardHandler = new CefKeyboardHandlerAdapter() {
        @Override
        public boolean onKeyEvent(CefBrowser b, CefKeyEvent event) {
            if (b != browser) return false;
            // Only handle key events when setDebug(true)
            if (!debug) return false;

            boolean isMac = OS.get() == OS.MAC;
            boolean isCTRL = !isMac && (event.modifiers & EventFlags.EVENTFLAG_CONTROL_DOWN) != 0;
            boolean isCOMMAND = isMac && (event.modifiers & EventFlags.EVENTFLAG_COMMAND_DOWN) != 0;
            boolean isSHIFT = (event.modifiers & EventFlags.EVENTFLAG_SHIFT_DOWN) != 0;
            // CefKeyEvent windows_key_code is a cross-platform virtual key code on a U.S. keyboard layout
            // Its value is the same as AWT KeyEvent.VK_*.
            if (event.type == CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN
                    && (isCTRL || isCOMMAND) && isSHIFT
                    && event.windows_key_code == KeyEvent.VK_I) {
                toggleDevTools();
                return true;
            }
            if (event.type == CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN
                    && (isCTRL || isCOMMAND) && isSHIFT
                    && event.windows_key_code == KeyEvent.VK_R) {
                b.reload();
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
            // Recursion is cut upstream, by ScilabFocusHandler.
            // Block navigation/JS-triggered focus grabs.
            // Allow FOCUS_SOURCE_SYSTEM so clicking on the browser
            // content naturally focuses the correct HTML element.
            return source == CefFocusHandler.FocusSource.FOCUS_SOURCE_NAVIGATION;
        }

        @Override
        public void onGotFocus(CefBrowser b) {
            if (b != browser) return;
            boolean alreadyFocused = browserHasFocus;
            browserHasFocus = true;
            browserFocusRequested = false;
            if (alreadyFocused) {
                /*
                 * Chromium can re-fire onGotFocus for a browser that already
                 * has focus (observed in bursts of 50+ calls within a few ms
                 * right after requestBrowserFocus(), one new native thread
                 * per call). Re-running the FlexDock activation below on
                 * every one of those calls requests focus again on this same
                 * component, which is what sustains the burst. Only the
                 * first, genuine gained-focus transition needs it.
                 */
                return;
            }
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

    // ---- Load state: gate resize masking until the browser has painted ----
    /** Set once the browser's main frame finished loading. */
    private volatile boolean browserLoaded = false;
    final CefLoadHandler loadHandler = new CefLoadHandlerAdapter() {
        @Override
        public void onLoadEnd(CefBrowser b, CefFrame frame, int httpStatusCode) {
            if (b == browser && frame != null && frame.isMain()) {
                browserLoaded = true;
                scheduleSnapshotRefresh();
            }
        }
    };

    // ---- External Request management ----
    // Open the default system browser for external links (http/https)
    // Run the Scilab command of "scilab:" links (see ScilabLink)
    // Resolve SCI paths to real files on disk (file://)
    public final static class ScilabResourceHandler extends CefResourceRequestHandlerAdapter {
        @Override
        public boolean onBeforeResourceLoad(CefBrowser browser, CefFrame frame, CefRequest request) {
            String url = request.getURL();

            if (url != null && url.startsWith("file:///SCI/")) {
                String relativePath = url.substring("file:///SCI/".length());

                File targetFile = new File(ScilabConstants.SCI.getPath(), relativePath);
                if ((OS.get() != OS.WINDOWS) && (targetFile.exists() == false) && url.startsWith("file:///SCI/thirdparty/")) {
                        // Probably an installed Linux or macOS version where thirdparty/ is in SCI/../../
                        targetFile = new File(ScilabConstants.SCI.getPath(), "/../../" + relativePath);
                    }
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
        public boolean onBeforeBrowse(CefBrowser browser, CefFrame frame, CefRequest request, boolean user_gesture, boolean is_redirect) {
            // "scilab:" links run a Scilab command, they never navigate.
            // Only a click of the user runs it: a page cannot open a link
            // by itself (script, redirection, ...).
            String url = request.getURL();
            if (ScilabLink.isScilabLink(url)) {
                if (user_gesture && !is_redirect) {
                    ScilabLink.open(url);
                }
                return true;
            }

            // Open external links in the default browser. The navigation is
            // canceled here rather than by disabling the default handling of
            // its request: the load then ends as aborted (ERR_ABORTED), which
            // is not an error to display, instead of as failed.
            if (url != null && (url.startsWith("https://") || url.startsWith("http://"))) {
                WebBrowser.openUrl(url);
                return true;
            }
            return false;
        }

        @Override
        public CefResourceRequestHandler getResourceRequestHandler(CefBrowser browser, CefFrame frame, CefRequest request, boolean isNavigation, boolean isDownload, String requestInitiator, BoolRef disableDefaultHandling) {
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
            synchronized (closingBrowsers) {
                if (closingBrowsers.remove(b)) {
                    closingBrowsers.notifyAll();
                }
            }
        }
    };

    /* Handler lists, to let the user add them */
    ArrayList<CefKeyboardHandler> keyboardHandlers = null;
    ArrayList<CefContextMenuHandler> contextMenuHandlers = null;
    ArrayList<CefRequestHandler> requestHandlers = null;
    ArrayList<CefLoadHandler> loadHandlers = null;
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
        ScilabBrowser.addFocusHandler(client, focusHandler);
        client.addRequestHandler(requestHandler);
        client.addLoadHandler(loadHandler);
        client.addLifeSpanHandler(lifeSpanHandler);

        // ---- Layout ----
        setBackground(Color.WHITE);

        // Mask JCEF's heavyweight native view on resize to avoid flickering
        cardLayout = new CardLayout();
        cardPanel = new JPanel(cardLayout);
        cardPanel.add(browser.getUIComponent(), CARD_BROWSER);
        cardPanel.add(resizeCover, CARD_COVER);
        add(cardPanel, BorderLayout.CENTER);

        // End of gesture tracker
        resizeSettleTimer = new Timer(RESIZE_SETTLE_MS, e -> {
            if (mouseDown) {
                resizeSettleTimer.restart();
            } else {
                endResize();
            }
        });
        resizeSettleTimer.setRepeats(false);

        // Single periodic tick instead of re-arming a timer on every change:
        // signalling a change must stay free (it happens on every console line).
        snapshotRefreshTimer = new Timer(SNAPSHOT_REFRESH_MS, e -> maybeRefreshSnapshot());
        snapshotRefreshTimer.setRepeats(true);
        snapshotRefreshTimer.start();

        resizeMouseListener = event -> {
            MouseEvent me = (MouseEvent) event;
            switch (me.getID()) {
                case MouseEvent.MOUSE_PRESSED:
                    mouseDown = true;
                    // Capture a fresh snapshot as a press may start a drag (divider or dockable).
                    refreshSnapshot();
                    break;
                case MouseEvent.MOUSE_RELEASED:
                    mouseDown = false;
                    lastMouseReleaseMs = System.currentTimeMillis();
                    endResize();
                    break;
                case MouseEvent.MOUSE_DRAGGED:
                    // Mask the console for the gesture; MOUSE_RELEASED ends it,
                    // and the post-release guard keeps the drop resize unmasked.
                    if (!resizing && isDividerDrag(me)) {
                        lastResizeStepMs = System.currentTimeMillis();
                        if (showCover()) {
                            resizing = true;
                            // Drop its CEF "focused" titlebar mark now the browser hidden
                            clearCefNativeFocusView();
                            resizeSettleTimer.restart();
                        }
                    }
                    break;
                case MouseEvent.MOUSE_MOVED:
                case MouseEvent.MOUSE_ENTERED:
                    // Cursor moving again after the steps went quiet = frame
                    // drag released. A MOUSE_MOVED still amid fresh steps is
                    // motion *during* the drag (Windows emits it) and is ignored
                    // to avoid re-capture thrashing.
                    if (resizing && !mouseDown
                            && System.currentTimeMillis() - lastResizeStepMs > MOVE_END_QUIET_MS) {
                        // Defer so a step queued just after this motion can
                        // refresh lastResizeStepMs and cancel a false end.
                        SwingUtilities.invokeLater(() -> {
                            if (resizing && !mouseDown
                                    && System.currentTimeMillis() - lastResizeStepMs > MOVE_END_QUIET_MS) {
                                endResize();
                            }
                        });
                    }
                    break;
                default:
                    break;
            }
        };
        Toolkit.getDefaultToolkit().addAWTEventListener(
            resizeMouseListener,
            AWTEvent.MOUSE_EVENT_MASK | AWTEvent.MOUSE_MOTION_EVENT_MASK);
        addComponentListener(new ComponentAdapter() {
            @Override
            public void componentResized(ComponentEvent e) {
                onResizeTick();
            }

            @Override
            public void componentMoved(ComponentEvent e) {
                onResizeTick();
            }
        });
        // Our own componentResized only fires once the layout cascade has
        // reached us - by then the stale native view has already been painted at
        // the old spot. An ancestor's resize fires earlier in that cascade, so
        // mask from there to catch the gesture before anything wrong shows.
        addHierarchyBoundsListener(new HierarchyBoundsListener() {
            @Override
            public void ancestorMoved(HierarchyEvent e) {
                // A move does not change our position inside the parent: the
                // native view stays correct, nothing to mask.
            }

            @Override
            public void ancestorResized(HierarchyEvent e) {
                onResizeTick();
            }
        });

        // Invisible heavyweight Canvas for native focus management
        nativeFocusSink.setFocusable(true);
        nativeFocusSink.setPreferredSize(new Dimension(0, 0));
        nativeFocusSink.setMinimumSize(new Dimension(0, 0));
        nativeFocusSink.setMaximumSize(new Dimension(0, 0));
        add(nativeFocusSink, BorderLayout.SOUTH);

        // ---- Click-outside detection ----
        // When the browser has focus and the user clicks on another
        // component, release Chromium focus and transfer to the target.
        clickOutsideListener = event -> {
            if (!browserHasFocus) return;
            if (event.getID() != MouseEvent.MOUSE_PRESSED) return;
            MouseEvent me = (MouseEvent) event;
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
            if (SwingUtilities.isDescendingFrom(target, ScilabCefPanel.this)) {
                return;
            }
            // Pressing a split divider starts a transient drag, not a real focus
            // change: keep the original focus owner so it is still focused after the
            // drag (the resize mask hides then restores the browser meanwhile).
            if (target instanceof BasicSplitPaneDivider) {
                return;
            }
            while (target != null && !(target instanceof Window) && !target.isFocusable()) {
                target = target.getParent();
            }
            if (target == null || target instanceof Window) return;

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
        Toolkit.getDefaultToolkit().addAWTEventListener(
            clickOutsideListener, AWTEvent.MOUSE_EVENT_MASK);
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
            // The page content is about to change: re-arm the snapshot used to
            // mask resizes and dock drags, otherwise it would keep showing the
            // page as it was.
            scheduleSnapshotRefresh();
        }
    }


    /**
     * Make JCEF reposition its heavyweight native view right away.
     */
    private void refreshBrowserGeometry() {
        if (browser == null) {
            return;
        }
        Component ui = browser.getUIComponent();
        if (ui != null) {
            ui.repaint(); // triggers CefBrowserWr.doUpdate() with the new geometry
        }
    }

    /**
     * Give native keyboard focus back to Chromium after a focus request coming
     * from the web page itself. This also cancels any pending delayed transfer
     * to a Swing component scheduled by the click-outside handler.
     */
    /**
     * Give the Swing focus owner back to the browser component. Chromium's
     * own focus state is left untouched: this only repairs the Java side,
     * when a lightweight component became the focus owner behind its back.
     */
    public void reclaimSwingFocus() {
        CefBrowser currentBrowser = browser;
        if (currentBrowser == null) {
            return;
        }
        final Component ui = currentBrowser.getUIComponent();
        final KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        final Component from = manager.getFocusOwner();
        // Already the owner: asking again can bounce the focus onto an
        // ancestor (see requestBrowserFocus()). No owner at all: the window
        // is inactive, or Chromium's own native window holds the keyboard.
        if (from == ui || from == null) {
            return;
        }
        ui.requestFocusInWindow();
        /*
         * The request can be dropped without a word while the browser is
         * being moved -- a tab being split lays the overlay out again -- and
         * the keyboard then stays with the placeholder. Ask again for a
         * moment, as long as nobody else took the focus meanwhile.
         */
        final Timer retry = new Timer(50, null);
        retry.addActionListener(new java.awt.event.ActionListener() {
            private int attempts = 0;

            @Override
            public void actionPerformed(java.awt.event.ActionEvent event) {
                Component owner = manager.getFocusOwner();
                if (owner != from || browser == null || ++attempts > 10) {
                    retry.stop();
                    return;
                }
                ui.requestFocusInWindow();
            }
        });
        retry.start();
    }

    public void requestBrowserFocus() {
        if (browserHasFocus) {
            // Already focused: skip. Calling requestFocusInWindow() again on
            // an already-focused heavyweight peer is redundant and was
            // observed to bounce focus onto a lightweight ancestor
            // container, whose own FocusListener re-requests focus on this
            // pane, sustaining a Java-side echo of the same loop.
            return;
        }
        focusTransferGeneration++;
        CefBrowser currentBrowser = browser;
        if (currentBrowser == null) {
            return;
        }
        currentBrowser.getUIComponent().requestFocusInWindow();

        if (!browserFocusRequested) {
            browserFocusRequested = true;
            currentBrowser.setFocus(true);
        }
    }

    /**
     * Relinquish Chromium's native keyboard focus without disposing it.
     * Used when ownership of an editor interaction moves to another browser.
     */
    public void releaseBrowserFocus() {
        focusTransferGeneration++;
        browserHasFocus = false;
        browserFocusRequested = false;
        clearCefNativeFocusView();
        CefBrowser currentBrowser = browser;
        if (currentBrowser != null) {
            currentBrowser.setFocus(false);
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
        // No local dispatcher here: removing the client's handler to install
        // one would drop the recursion guard ScilabBrowser installed.
        ScilabBrowser.addFocusHandler(client, h);
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

    public void addLoadHandler(CefLoadHandler h)
    {
        if (loadHandlers == null) {
            loadHandlers = new ArrayList<>();
            loadHandlers.add(loadHandler);
            client.removeLoadHandler();
            client.addLoadHandler(new CefLoadHandler() {
                @Override
                public void onLoadingStateChange(CefBrowser browser, boolean isLoading, boolean canGoBack,
                        boolean canGoForward) {
                    for (CefLoadHandler h : loadHandlers) {
                        h.onLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
                    }
                }
                @Override
                public void onLoadStart(CefBrowser browser, CefFrame frame, CefRequest.TransitionType transitionType) {
                    for (CefLoadHandler h : loadHandlers) {
                        h.onLoadStart(browser, frame, transitionType);
                    }
                }
                @Override
                public void onLoadEnd(CefBrowser browser, CefFrame frame, int httpStatusCode) {
                    for (CefLoadHandler h : loadHandlers) {
                        h.onLoadEnd(browser, frame, httpStatusCode);
                    }
                }
                @Override
                public void onLoadError(CefBrowser browser, CefFrame frame, ErrorCode errorCode, String errorText,
                        String failedUrl) {
                    for (CefLoadHandler h : loadHandlers) {
                        h.onLoadError(browser, frame, errorCode, errorText, failedUrl);
                    }
                }
            });
        }
        loadHandlers.add(h);
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

    // ====================================================================
    // live-resize masking
    // ====================================================================

    public BufferedImage getPageSnapshot() {
        return cachedSnapshot;
    }

    /** Swap in the cover. @return false if there is no snapshot to mask with. */
    private boolean showCover() {
        BufferedImage snapshot = cachedSnapshot;
        if (snapshot == null) {
            return false;
        }
        resizeCover.setSnapshot(snapshot);
        cardPanel.setBackground(resizeCover.getBackground());
        cardLayout.show(cardPanel, CARD_COVER);
        resizeCover.paintImmediately(0, 0, resizeCover.getWidth(), resizeCover.getHeight());
        return true;
    }

    /** Swap the live browser back in and let JCEF reposition its native view. */
    private void hideCover() {
        cardLayout.show(cardPanel, CARD_BROWSER);
        refreshBrowserGeometry();
        if (browserHasFocus && browser != null) {
            Component ui = browser.getUIComponent();
            if (ui != null) {
                ui.requestFocusInWindow();
            }
        }
        // Re-arm a snapshot for the next gesture, once the browser has settled.
        scheduleSnapshotRefresh();
    }

    /** @return true if the event is dragging a split divider that contains this panel. */
    private boolean isDividerDrag(MouseEvent me) {
        Component src = me.getComponent();
        if (!(src instanceof BasicSplitPaneDivider)) {
            return false;
        }
        // A divider's parent is its JSplitPane; mask only if we live inside it.
        Component split = src.getParent();
        return split instanceof JSplitPane
                && SwingUtilities.isDescendingFrom(this, split);
    }

    /** On the first resize step show the cached snapshot; on every step re-arm the fallback timer. */
    private void onResizeTick() {
        if (!browserLoaded) {
            return;
        }
        // A divider/dock drag ends on a mouse release and, with continuous
        // layout off, its single resize lands just after. That one-shot needs no
        // masking: with no ongoing gesture the cover would only linger over the
        // freshly laid-out panes until the fallback timer fires. Only a frame
        // resize (continuous, no AWT mouse events) still needs the mask.
        if (!resizing
                && System.currentTimeMillis() - lastMouseReleaseMs < POST_RELEASE_NO_MASK_MS) {
            return;
        }
        lastResizeStepMs = System.currentTimeMillis();
        if (!resizing) {
            if (!showCover()) {
                return;
            }
            resizing = true;
        }
        resizeSettleTimer.restart();
    }

    /** End of a resize gesture: restore the live browser. */
    private void endResize() {
        if (!resizing) {
            return;
        }
        resizing = false;
        if (resizeSettleTimer != null) {
            resizeSettleTimer.stop();
        }
        hideCover();
    }

    /**
     * Signal that the page content changed, so the next resize/drag masks with
     * an up-to-date image. Called on every console line, so it must stay free:
     * it only stamps a timestamp, the periodic tick does the work. Safe to call
     * from any thread.
     */
    private void scheduleSnapshotRefresh() {
        contentChangedMs = System.currentTimeMillis();
    }

    /**
     * Periodic tick: capture once the content has settled. Bursts of changes
     * (a command printing thousands of lines) keep pushing contentChangedMs, so
     * nothing is captured until they stop - a single capture per burst.
     */
    private void maybeRefreshSnapshot() {
        if (resizing) {
            return;
        }
        long changed = contentChangedMs;
        if (changed == capturedChangeMs) {
            return; // nothing new since the last capture
        }
        if (System.currentTimeMillis() - changed < SNAPSHOT_REFRESH_MS) {
            return; // still changing, let it settle
        }
        capturedChangeMs = changed;
        refreshSnapshot();
    }

    /**
     * Cache a fresh screenshot of the page, taken by Chromium itself.
     * Note: Using java.awt.Robot needs the macOS "Screen Recording" permission.
     */
    private void refreshSnapshot() {
        if (browser == null || !browserLoaded || resizing) {
            return;
        }
        Component ui = browser.getUIComponent();
        if (ui == null || !ui.isShowing() || ui.getWidth() <= 0 || ui.getHeight() <= 0) {
            return;
        }
        CefDevToolsClient devTools = browser.getDevToolsClient();
        if (devTools == null || devTools.isClosed()) {
            return;
        }
        devTools.executeDevToolsMethod("Page.captureScreenshot", "{\"format\":\"png\"}")
            .thenAccept(shotJson -> {
                BufferedImage img = decodeScreenshot(shotJson);
                if (img != null) {
                    cachedSnapshot = img;
                    // A grab issued on mouse-press can land after the cover is
                    // already up (dockable/divider drag, where the console is
                    // not resizing). Repaint it so the fresh image shows instead
                    // of the previous one painted by showCover().
                    SwingUtilities.invokeLater(() -> {
                        if (resizing) {
                            resizeCover.setSnapshot(img);
                            resizeCover.repaint();
                        }
                    });
                }
            });
    }

    /** Extract the base64 PNG out of a Page.captureScreenshot response. */
    private static BufferedImage decodeScreenshot(String json) {
        if (json == null) {
            return null;
        }
        try {
            JsonElement data = JsonParser.parseString(json).getAsJsonObject().get("data");
            if (data == null) {
                return null;
            }
            return ImageIO.read(new ByteArrayInputStream(Base64.getDecoder().decode(data.getAsString())));
        } catch (Exception e) {
            log.warning("Resize snapshot decode failed: " + e.getMessage());
            return null;
        }
    }

    /**
     * Lightweight stand-in shown while the browser resizes
     * Draws the last capture at its original (logical) size.
     */
    private static class ResizeCover extends JPanel {
        private static final long serialVersionUID = 1L;
        private transient BufferedImage image;

        ResizeCover() {
            setBackground(Color.WHITE);
        }

        void setSnapshot(BufferedImage image) {
            this.image = image;
        }

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g); // fills with the background
            if (image == null) {
                return;
            }
            double scale = (g instanceof Graphics2D) ? ((Graphics2D) g).getTransform().getScaleX() : 1.0;
            if (scale <= 0) {
                scale = 1.0;
            }
            int imageW = (int) Math.round(image.getWidth() / scale);
            int imageH = (int) Math.round(image.getHeight() / scale);
            g.drawImage(image, 0, 0, imageW, imageH, null);
        }
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

    /**
     * Wait for the browsers being closed to be destroyed by CEF.
     * Otherwise, removeNotify() goes on and destroys the native parent of a
     * browser still closing: Chromium then loses the compositor of another
     * browser moved at the same time (e.g. closing SciNotes docked next to
     * the console, which is reparented into the freed room). That page stays
     * at its former size, never repainted again.
     * CEF reports the end of a close on its own thread, so waiting here on the
     * EDT cannot dead-lock; the wait is bounded anyway.
     */
    private static void waitForClosingBrowsers()
    {
        synchronized (closingBrowsers) {
            long end = System.currentTimeMillis() + CLOSING_BROWSERS_TIMEOUT_MS;
            long remaining = CLOSING_BROWSERS_TIMEOUT_MS;
            while (!closingBrowsers.isEmpty() && remaining > 0) {
                try {
                    closingBrowsers.wait(remaining);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
                remaining = end - System.currentTimeMillis();
            }
            if (!closingBrowsers.isEmpty()) {
                // A browser that never completed its close (e.g. never fully
                // created) must not delay every later dispose.
                log.warning(closingBrowsers.size() + " browser(s) not closed in time");
                closingBrowsers.clear();
            }
        }
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
        // A docking move must let JCEF temporarily detach and reparent its
        // native child without closing the browser.
        if (isDocking > 0) {
            super.removeNotify();
            return;
        }

        /*
         * Mark the browser as closing before its child component receives
         * removeNotify(). CefBrowserWr otherwise posts an asynchronous native
         * setParent(0), which can race CloseBrowser while an owning SciNotes
         * window is being destroyed. CefBrowser_N.setParent() deliberately
         * ignores that detach once close(true) set its isClosing flag.
         */
        dispose();
        super.removeNotify();
    }

    /**
     * Explicitly dispose the native browser before its Swing hierarchy is
     * detached. This method is idempotent and may also be called by owners
     * which suppress removeNotify() while moving a browser component.
     */
    public void dispose() {
        focusTransferGeneration++;
        browserHasFocus = false;
        browserFocusRequested = false;

        if (clickOutsideListener != null) {
            Toolkit.getDefaultToolkit().removeAWTEventListener(clickOutsideListener);
            clickOutsideListener = null;
        }

        clearCefNativeFocusView();

        if (resizeMouseListener != null) {
            Toolkit.getDefaultToolkit().removeAWTEventListener(resizeMouseListener);
            resizeMouseListener = null;
        }

        if (snapshotRefreshTimer != null) {
            snapshotRefreshTimer.stop();
        }
        if (resizeSettleTimer != null) {
            resizeSettleTimer.stop();
        }

        CefBrowser browserToClose = browser;
        browser = null;
        if (browserToClose != null) {
            devToolsRequested = false;
            devToolsBrowser = null;
            browserToClose.closeDevTools();
            synchronized (closingBrowsers) {
                closingBrowsers.add(browserToClose);
            }
            browserToClose.close(true);
        }

        CefClient clientToRelease = client;
        client = null;
        if (clientToRelease != null) {
            ScilabBrowser.release(clientToRelease);
        }

        if (browserToClose != null) {
            waitForClosingBrowsers();
        }
    }
}
