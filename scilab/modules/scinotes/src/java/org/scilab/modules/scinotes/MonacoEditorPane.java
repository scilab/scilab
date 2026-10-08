/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.scinotes;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.KeyboardFocusManager;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.ActionEvent;
import java.awt.event.AWTEventListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowEvent;
import java.awt.geom.Rectangle2D;
import java.net.URI;
import java.net.MalformedURLException;
import java.net.URISyntaxException;
import java.net.URLDecoder;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.WeakHashMap;
import java.util.concurrent.CopyOnWriteArrayList;

import javax.swing.AbstractAction;
import javax.swing.JComponent;
import javax.swing.JPanel;
import javax.swing.JPopupMenu;
import javax.swing.JSplitPane;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;
import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import javax.swing.text.Element;

import org.cef.CefClient;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefFrame;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefMessageRouter.CefMessageRouterConfig;
import org.cef.callback.CefQueryCallback;
import org.cef.callback.CefDragData;
import org.cef.handler.CefDragHandler;
import org.cef.handler.CefLifeSpanHandlerAdapter;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.handler.CefMessageRouterHandlerAdapter;
import org.cef.handler.CefKeyboardHandlerAdapter;
import org.cef.handler.CefKeyboardHandler.CefKeyEvent;
import org.scilab.modules.scinotes.utils.SciNotesContents;
import org.cef.misc.BoolRef;
import org.cef.misc.EventFlags;
import org.cef.network.CefRequest.TransitionType;
import org.scilab.modules.commons.OS;
import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.commons.gui.ScilabGUIUtilities;
import org.scilab.modules.commons.utils.WebBrowser;
import org.scilab.modules.gui.utils.DebugModeIndicator;
import org.scilab.modules.gui.messagebox.ScilabModalDialog;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.AnswerOption;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.ButtonType;
import org.scilab.modules.gui.messagebox.ScilabModalDialog.IconType;
import org.scilab.modules.scinotes.actions.AboutAction;
import org.scilab.modules.scinotes.actions.CCloseTabInNewWindowAction;
import org.scilab.modules.scinotes.actions.CloseAction;
import org.scilab.modules.scinotes.actions.CopyAsHTMLAction;
import org.scilab.modules.scinotes.actions.CopyAsHTMLWithLineNumberAction;
import org.scilab.modules.scinotes.actions.DebugCommandAction;
import org.scilab.modules.scinotes.actions.DebugContinueAction;
import org.scilab.modules.scinotes.actions.EncodingAction;
import org.scilab.modules.scinotes.actions.EvaluateSelectionAction;
import org.scilab.modules.scinotes.actions.ExecuteFileIntoScilabAction;
import org.scilab.modules.scinotes.actions.ExitAction;
import org.scilab.modules.scinotes.actions.GenerateHelpFromFunctionAction;
import org.scilab.modules.scinotes.actions.HelpAction;
import org.scilab.modules.scinotes.actions.HelpOnKeywordAction;
import org.scilab.modules.scinotes.actions.LoadIntoScilabAction;
import org.scilab.modules.scinotes.actions.NewAction;
import org.scilab.modules.scinotes.actions.NextTabAction;
import org.scilab.modules.scinotes.actions.NextParagraphAction;
import org.scilab.modules.scinotes.actions.OpenAction;
import org.scilab.modules.scinotes.actions.OpenTabInNewWindowAction;
import org.scilab.modules.scinotes.actions.OpenURLAction;
import org.scilab.modules.scinotes.actions.PreviousTabAction;
import org.scilab.modules.scinotes.actions.PreviousParagraphAction;
import org.scilab.modules.scinotes.actions.PrintPreviewAction;
import org.scilab.modules.scinotes.actions.PrintAction;
import org.scilab.modules.scinotes.actions.ReloadAction;
import org.scilab.modules.scinotes.actions.SaveAndExecuteAllFilesAction;
import org.scilab.modules.scinotes.actions.SaveAndExecuteFileAction;
import org.scilab.modules.scinotes.actions.SearchFilesAction;
import org.scilab.modules.scinotes.actions.SearchWordInFilesAction;
import org.scilab.modules.scinotes.actions.SelectBlockAction;
import org.scilab.modules.scinotes.actions.SetAnchorAction;
import org.scilab.modules.scinotes.utils.SciNotesMessages;

/**
 * Monaco-based SciNotes editor surface.
 *
 * The ScilabDocument inherited from ScilabEditorPane remains the source used by
 * the existing SciNotes save/autosave/action code. Monaco is currently a view
 * synchronized with that document.
 */
public class MonacoEditorPane extends ScilabEditorPane {

    private static final long serialVersionUID = 3574418063043820291L;
    private static final Map<String, String> MONACO_BRIDGED_ACTIONS = new LinkedHashMap<String, String>();
    private static final String[][] MONACO_FIXED_BRIDGED_SHORTCUTS = new String[][] {
        {"F3", "false", "true", "false", "findPrevious"},
        {"KeyD", "true", "true", "true", "openDevTools"},
        {"Delete", "false", "true", "false", "cut"},
        {"Insert", "true", "false", "false", "copy"},
        {"Insert", "false", "true", "false", "paste"}
    };
    /*
     * Explicit Monaco-only shortcuts. Keep this list small and deliberate: each
     * entry is injected in JavaScript and mirrored as a Swing root-pane binding
     * so the command works with either focus owner.
     */
    private static final String[][] MONACO_ONLY_SHORTCUTS = new String[][] {
        // keyCode, ctrlCmd, shift, alt, Monaco command id
    };
    private static final int LEGACY_SHIFT_MASK = 1;
    private static final int LEGACY_CTRL_MASK = 2;
    private static final int LEGACY_META_MASK = 4;
    private static final int LEGACY_ALT_MASK = 8;
    private static final String[][] MONACO_DISABLED_SHORTCUTS = new String[][] {
        {"KeyA", "true", "false", "false"},
        {"KeyC", "true", "false", "false"},
        {"KeyF", "true", "false", "false"},
        {"KeyH", "true", "false", "false"},
        {"KeyD", "true", "false", "false"},
        {"KeyD", "true", "true", "false"},
        {"KeyI", "true", "false", "false"},
        {"KeyJ", "true", "false", "false"},
        {"KeyJ", "true", "true", "false"},
        {"KeyA", "true", "true", "false"},
        {"KeyG", "true", "true", "false"},
        {"KeyW", "true", "true", "false"},
        {"KeyV", "true", "false", "false"},
        {"KeyX", "true", "false", "false"},
        {"KeyY", "true", "false", "false"},
        {"KeyZ", "true", "false", "false"},
        {"KeyZ", "true", "true", "false"},
        {"F3", "false", "false", "false"},
        {"F3", "false", "true", "false"}
    };
    private static boolean externalReloadPromptActive;

    private static final Map<SciNotes, SharedBrowser> SHARED_BROWSERS =
        new WeakHashMap<SciNotes, SharedBrowser>();
    /*
     * For each file shown in several windows, the pane that sent the last
     * history message: its replica holds the text to repair a window that
     * diverged. Used on the EDT only.
     */
    private static final Map<String, MonacoEditorPane> SHARED_HISTORY_ORIGINS =
        new HashMap<String, MonacoEditorPane>();

    private final transient SharedBrowser sharedBrowser;
    private final JPanel browserHost;
    private final ScilabCefPanel cefPanel;
    private final transient CefClient client;
    private final transient CefBrowser browser;
    private transient DocumentListener documentListener;

    private boolean browserReady;
    private boolean paneConstructed;
    private boolean monacoDocumentCreated;
    private boolean focusRequested;
    private boolean updatingFromMonaco;
    private boolean browserDisposed;
    private String indexedDocumentUri;
    private String sharedDocumentName;
    private String sharedDocumentKey;
    private File watchedFile;
    private int indexedDocumentVersion;
    private ScilabEditorPane definitionOrigin;
    private String pendingText;
    private int pendingRevealLine;
    private int revealRequests;
    private Point lastContextMenuPoint = new Point(0, 0);
    private JPopupMenu activeContextMenu;
    private transient AWTEventListener contextMenuDismissListener;
    private long lastMonacoExternalModification;
    private int documentRevision;
    private int lastPreHandledKeyCode = -1;
    private int lastPreHandledModifiers = -1;
    private final transient Object droppedFilesLock = new Object();
    private transient List<String> draggedFilePaths = new ArrayList<String>();

    /**
     * Set only on the secondary half of a split Monaco tab: the primary
     * pane whose Monaco model this pane shows a second, independent view
     * of. Null on a non-split pane and on the primary half itself.
     * @see #enterSplitView(MonacoEditorPane)
     */
    private MonacoEditorPane splitPrimary;

    /** Owns the single native browser used by all Monaco tabs in one SciNotes window. */
    private static final class SharedBrowser {
        private static final String DOCUMENT_MESSAGE_PREFIX = "document:";
        private static final String DEBUG_STOP_ACTION = "scinotes-debug-disconnect";

        private final SciNotes editor;
        private final ScilabCefPanel cefPanel;
        private final CefClient client;
        private final CefBrowser browser;
        private final List<MonacoEditorPane> panes = new CopyOnWriteArrayList<MonacoEditorPane>();
        private volatile MonacoEditorPane activePane;
        private volatile boolean overlayAttached;
        /** True while the page has a secondary editor view mounted. */
        private volatile boolean splitActive;
        private volatile boolean ready;
        private volatile boolean monacoReady;
        private volatile boolean initialDocumentAdopted;
        private volatile boolean disposed;
        /*
         * The page runs one debug client for the whole window: its state
         * belongs here, not to the pane that happened to be active when it
         * was reported. A split or its removal replaces the panes of a tab.
         */
        private volatile boolean dapCanContinue;
        private volatile boolean dapCanExecute;
        private volatile boolean dapCanDebug;
        private volatile boolean dapDebugMode;
        private volatile boolean dapSessionReady;
        private volatile String dapSessionState = "";

        SharedBrowser(SciNotes editor) {
            this.editor = editor;
            cefPanel = new ScilabCefPanel(getMonacoUrl());
            client = cefPanel.getClient();
            browser = cefPanel.getBrowser();
        }

        boolean isEmpty() {
            return panes.isEmpty();
        }

        void register(MonacoEditorPane pane) {
            if (disposed) {
                return;
            }
            panes.add(pane);
            if (activePane == null) {
                activePane = pane;
            }
        }

        void paneConstructed(MonacoEditorPane pane) {
            if (!disposed) {
                pane.paneConstructed = true;
                if (monacoReady) {
                    pane.browserReady = true;
                    if (!initialDocumentAdopted && pane == activePane) {
                        pane.initializeSharedBrowser();
                    } else {
                        pane.createMonacoDocument();
                        if (pane == activePane) {
                            pane.selectMonacoDocument();
                        }
                    }
                }
            }
        }

        MonacoEditorPane getActivePane() {
            return activePane;
        }

        MonacoEditorPane getPane(String documentId) {
            if (documentId == null || documentId.isEmpty() || "initial".equals(documentId)) {
                return activePane;
            }
            for (MonacoEditorPane pane : panes) {
                if (!pane.browserDisposed && documentId.equals(pane.getMonacoDocumentId())) {
                    return pane;
                }
            }
            return null;
        }

        /**
         * The pane of this window whose Monaco document shows the file with
         * this key. Tabs on the same file share one model, so any of them
         * reaches all; the secondary half of a split owns no document.
         */
        MonacoEditorPane getSharedDocumentPane(String key) {
            if (disposed || !monacoReady) {
                return null;
            }
            for (MonacoEditorPane pane : panes) {
                if (!pane.browserDisposed && pane.browserReady && pane.monacoDocumentCreated
                        && pane.splitPrimary == null && key.equals(pane.getSharedDocumentKey())) {
                    return pane;
                }
            }
            return null;
        }

        void routeMessage(String request) {
            MonacoEditorPane pane = activePane;
            String paneRequest = request;
            if (request != null && request.startsWith(DOCUMENT_MESSAGE_PREFIX)) {
                int separator = request.indexOf(':', DOCUMENT_MESSAGE_PREFIX.length());
                if (separator < 0) {
                    return;
                }
                pane = getPane(request.substring(DOCUMENT_MESSAGE_PREFIX.length(), separator));
                paneRequest = request.substring(separator + 1);
            }
            // Addressed to the window whatever document it names, even none.
            if (paneRequest != null && paneRequest.startsWith("dapState:")) {
                final String payload = paneRequest.substring("dapState:".length());
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        updateDapState(payload);
                    }
                });
                return;
            }
            if (pane != null && !pane.browserDisposed) {
                pane.handleMessage(paneRequest);
            }
        }

        void activate(MonacoEditorPane pane) {
            if (disposed || pane == null || !panes.contains(pane)) {
                return;
            }
            /*
             * Being split belongs to the tab, not to the window: selecting
             * either half of a split tab must restore that tab's two views,
             * and selecting a tab that is not split (below) must take the
             * page back out of split mode -- otherwise the previous tab's
             * second view stays mounted over the newly selected one.
             */
            MonacoEditorPane splitOwner = pane.splitPrimaryPane();
            if (splitOwner != null) {
                activateSplit(splitOwner, pane.splitSecondaryPane());
                return;
            }
            /*
             * Monaco reports every text-focus event to Java ("focus:in").
             * Unconditionally repositioning/re-showing the heavyweight CEF
             * component on each of those notifications -- even when this
             * pane is already active and nothing changed -- provokes a
             * native focus event of its own, which Chromium propagates back
             * to Monaco's DOM node, which reports focus again: a
             * self-sustaining Java<->JS feedback loop that starves the CEF
             * renderer's IPC (page load never completes) and repeatedly
             * flips the FlexDock titlebar's focus indicator (visible
             * blink). Only touch the native component on an actual pane
             * change.
             *
             * register() already assigns activePane to the first pane of a
             * brand-new SharedBrowser (so paneConstructed()/loadFinished()
             * can tell it apart from later panes), which pre-empts the
             * activePane != pane comparison below on that pane's very first
             * activate() call. Without overlayAttached, showMonacoBrowser()
             * would then never run for it: the CEF component never gets
             * attached to the window's overlay layer, never becomes showing,
             * and JCEF never starts navigating it - the browser stays
             * permanently blank ("stuck loading" / empty tab).
             */
            boolean leftSplit = disablePageSplit(false);
            boolean changed = activePane != pane || !overlayAttached || leftSplit;
            activePane = pane;
            if (changed) {
                editor.showMonacoBrowser(cefPanel, pane.browserHost);
                overlayAttached = true;
            }
            if (monacoReady) {
                pane.selectMonacoDocument();
            }
        }

        /**
         * Drop the page's secondary editor view if one is mounted, without
         * touching the overlay bounds (the caller repositions them).
         *
         * @param forget true when the tab stops being split; false when
         *        another tab is only selected, so the page keeps the second
         *        view's caret, scroll and focus for when the tab comes back
         * @return true when a split was actually torn down
         */
        private boolean disablePageSplit(boolean forget) {
            if (!splitActive) {
                return false;
            }
            splitActive = false;
            if (monacoReady) {
                browser.executeJavaScript("window.SciNotesMonacoDisableSplit(" + forget + ");", browser.getURL(), 1);
            }
            return true;
        }

        /**
         * Stretch the single native browser over both halves of a split tab
         * and mount a second Monaco editor view -- bound to {@code primary}'s
         * model, not a model of its own -- in the page's secondary
         * container. There is only one CEF window per SciNotes window (see
         * the class comment), so "activating" a split does not mean picking
         * one pane over the other the way {@link #activate(MonacoEditorPane)}
         * does: both halves are shown at once.
         *
         * @param primary the pane owning the Monaco model (left/top half)
         * @param secondary the pane whose Swing placeholder positions the second view
         */
        void activateSplit(final MonacoEditorPane primary, MonacoEditorPane secondary) {
            if (disposed || primary == null || secondary == null
                    || !panes.contains(primary) || !panes.contains(secondary)) {
                return;
            }
            /*
             * Same reasoning as in activate(): "focus:in" fires on every
             * Monaco text focus, and re-showing the native component each
             * time feeds a Java<->JS focus loop. Only rebuild the split when
             * something actually changed.
             */
            boolean changed = activePane != primary || !overlayAttached || !splitActive;
            activePane = primary;
            if (changed) {
                splitActive = true;
                overlayAttached = true;
                if (monacoReady) {
                    browser.executeJavaScript("window.SciNotesMonacoEnableSplit("
                        + quoteJs(primary.getMonacoDocumentId()) + ");", browser.getURL(), 1);
                }
                editor.showMonacoBrowserSplit(cefPanel, primary.browserHost, secondary.browserHost,
                    new SciNotesContents.SplitLayoutListener() {
                        public void onSplitLayout(Rectangle primaryLocal, Rectangle secondaryLocal) {
                            if (secondaryLocal == null) {
                                return;
                            }
                            browser.executeJavaScript("window.SciNotesMonacoLayoutSplit("
                                + primaryLocal.x + ", " + primaryLocal.y + ", "
                                + primaryLocal.width + ", " + primaryLocal.height + ", "
                                + secondaryLocal.x + ", " + secondaryLocal.y + ", "
                                + secondaryLocal.width + ", " + secondaryLocal.height
                                + ");", browser.getURL(), 1);
                        }
                    });
            }
            if (monacoReady) {
                primary.selectMonacoDocument();
            }
        }

        /**
         * Undo {@link #activateSplit(MonacoEditorPane, MonacoEditorPane)}:
         * drop the secondary Monaco view and go back to covering only
         * {@code primary} with the native browser.
         */
        void deactivateSplit(MonacoEditorPane primary) {
            if (disposed) {
                return;
            }
            disablePageSplit(true);
            if (primary != null && panes.contains(primary)) {
                activePane = primary;
                overlayAttached = true;
                editor.showMonacoBrowser(cefPanel, primary.browserHost);
            }
        }

        /**
         * Apply the command availability computed by the page's debug client
         * to the controls of this window.
         * @param payload serialized command availability
         */
        private void updateDapState(String payload) {
            MonacoDapState state;
            try {
                state = new Gson().fromJson(payload, MonacoDapState.class);
            } catch (JsonSyntaxException e) {
                System.err.println("SciNotes DAP: invalid state payload: " + e.getMessage());
                return;
            }
            if (state == null || disposed) {
                return;
            }

            dapCanContinue = state.canContinue;
            dapCanExecute = state.canExecute;
            dapCanDebug = state.canDebug;
            dapSessionReady = state.sessionReady;
            dapSessionState = state.state == null ? "" : state.state;
            if (!"running".equals(dapSessionState) && !"stopped".equals(dapSessionState)
                    && editor.getInfoBar() != null
                    && SciNotesMessages.DEBUG_SESSION_BUSY.equals(editor.getInfoBar().getText())) {
                ScilabEditorPane pane = editor.getTextPane();
                editor.getInfoBar().setText(pane == null ? "" : pane.getInfoBarText());
            }
            DebugCommandAction.updateAvailability(editor, state.canContinue || state.canDebug,
                state.canPause, state.canStepOver, state.canStepIn, state.canStepOut,
                state.canDisconnect, state.canExecute);
            dapDebugMode = state.debugMode;
            // The status bar says Scilab is in debug mode, and what the
            // debugger says after its own text. It is a text area: a message
            // on several lines, as Scilab errors are, would make it grow and
            // push the editor.
            showDebuggerState(state.message == null ? "" : state.message.replaceAll("\\s+", " ").trim());
        }

        /**
         * @param message what the debugger says, empty for nothing
         */
        void showDebuggerState(String message) {
            String state = message;
            // In debug mode, the key that leaves it comes with the state.
            KeyStroke key = dapDebugMode ? SciNotes.getActionKeys().get(DEBUG_STOP_ACTION) : null;
            if (key != null) {
                String keyText = KeyEvent.getKeyText(key.getKeyCode());
                if (key.getModifiers() != 0) {
                    keyText = InputEvent.getModifiersExText(key.getModifiers()) + "+" + keyText;
                }
                String hint = String.format(SciNotesMessages.DEBUG_LEAVE_HINT, keyText);
                state = state.isEmpty() ? hint : state + " (" + hint + ")";
            }
            DebugModeIndicator.show(editor.getInfoBar(), dapDebugMode, state);
        }

        void loadStarted() {
            if (disposed) {
                return;
            }
            ready = false;
            monacoReady = false;
            dapCanContinue = false;
            dapCanExecute = false;
            dapCanDebug = false;
            dapSessionReady = false;
            dapSessionState = "";
            for (MonacoEditorPane pane : new ArrayList<MonacoEditorPane>(panes)) {
                pane.browserReady = false;
            }
        }

        void loadFinished() {
            if (disposed) {
                return;
            }
            ready = true;
        }

        /*
         * The Monaco editor module loads asynchronously, well after the page's
         * load-finished callback. Creating the per-tab models from load-finished
         * ran while the editor did not yet exist on the page: those calls did
         * nothing, but Java still recorded the models as created, so every
         * non-active tab kept showing the active tab's model. The page now
         * signals back once the editor is actually available.
         */
        void onMonacoReady() {
            if (disposed || monacoReady) {
                return;
            }
            monacoReady = true;
            registerScilabExtensions();
            for (MonacoEditorPane pane : new ArrayList<MonacoEditorPane>(panes)) {
                pane.browserReady = true;
            }
            MonacoEditorPane active = activePane;
            if (active != null && active.paneConstructed) {
                active.initializeSharedBrowser();
            }
            for (MonacoEditorPane pane : new ArrayList<MonacoEditorPane>(panes)) {
                if (pane != active && pane.paneConstructed) {
                    pane.createMonacoDocument();
                }
            }
            // A focus asked while the page was loading waited for the editor
            // (onLoadEnd comes before it is ready): give it now, otherwise
            // SciNotes is the active dock while the keyboard stays elsewhere.
            if (active != null) {
                SwingUtilities.invokeLater(() -> active.focusMonacoIfRequested());
            }
        }

        /*
         * SciNotes owns the list of Scilab extensions; the page gets it here,
         * before any document is created, so that a single list decides both
         * the language Monaco gives a model and the documents indexed for the
         * LSP. Monaco merges a second registration of an already known
         * language, so the page can register Scilab before this arrives.
         */
        private void registerScilabExtensions() {
            StringBuilder extensions = new StringBuilder("[");
            for (String extension : SciNotes.getScilabExtensions()) {
                if (extensions.length() > 1) {
                    extensions.append(',');
                }
                extensions.append(quoteJs(extension));
            }
            extensions.append(']');
            browser.executeJavaScript("window.SciNotesMonacoRegisterScilabExtensions("
                + extensions + ");", browser.getURL(), 1);
        }

        void unregister(MonacoEditorPane pane) {
            if (!panes.remove(pane)) {
                return;
            }
            /*
             * Closing the document is what takes this tab out of the page's
             * registry; skipping it left the buffer of a closed tab there,
             * fed by every later edit. The page ignores a document it does
             * not know, so there is no reason to guess here.
             */
            pane.closeMonacoDocument();
            if (activePane == pane) {
                /* The JTabbedPane change listener selects the actual
                 * successor immediately after the component is removed. Do
                 * not guess a pane here and trigger an intermediate model and
                 * focus transition. */
                activePane = null;
            }
            if (panes.isEmpty()) {
                ready = false;
                disposed = true;
                synchronized (SHARED_BROWSERS) {
                    SHARED_BROWSERS.remove(editor);
                }
                /*
                 * closeTabAt() removes the Swing tab immediately after
                 * MonacoEditorPane.close(). Let that tab-removal callback
                 * unwind, then mark JCEF as closing before detaching its
                 * stable overlay. This prevents CefBrowserWr.removeNotify()
                 * from posting a native reparent operation during teardown.
                 */
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        cefPanel.dispose();
                        /*
                         * cefPanel.dispose() above already closed the native
                         * browser explicitly. Removing the now-empty panel
                         * from its Swing parent would otherwise trigger
                         * ScilabCefPanel.removeNotify(), which harmlessly but
                         * needlessly calls dispose() a second time (it is
                         * idempotent, but the extra call is pure noise).
                         */
                        ScilabCefPanel.setDockInProgress(true);
                        try {
                            editor.removeMonacoBrowser(cefPanel);
                        } finally {
                            ScilabCefPanel.setDockInProgress(false);
                        }
                    }
                });
            }
        }
    }

    private static SharedBrowser getSharedBrowser(SciNotes editor) {
        synchronized (SHARED_BROWSERS) {
            SharedBrowser shared = SHARED_BROWSERS.get(editor);
            if (shared == null) {
                shared = new SharedBrowser(editor);
                SHARED_BROWSERS.put(editor, shared);
            }
            return shared;
        }
    }

    static {
        MONACO_BRIDGED_ACTIONS.put("scinotes-new", "new");
        MONACO_BRIDGED_ACTIONS.put("scinotes-open", "open");
        MONACO_BRIDGED_ACTIONS.put("scinotes-open-url", "openUrl");
        MONACO_BRIDGED_ACTIONS.put("scinotes-save", "save");
        MONACO_BRIDGED_ACTIONS.put("scinotes-save-as", "saveAs");
        MONACO_BRIDGED_ACTIONS.put("scinotes-reload", "reload");
        MONACO_BRIDGED_ACTIONS.put("scinotes-print-preview", "printPreview");
        MONACO_BRIDGED_ACTIONS.put("scinotes-print", "print");
        MONACO_BRIDGED_ACTIONS.put("scinotes-close", "close");
        MONACO_BRIDGED_ACTIONS.put("scinotes-exit", "exit");
        MONACO_BRIDGED_ACTIONS.put("scinotes-find", "find");
        MONACO_BRIDGED_ACTIONS.put("scinotes-autocomplete-openers", "toggleAutoCompleteOpeners");
        MONACO_BRIDGED_ACTIONS.put("scinotes-autocomplete-keyword", "toggleAutoCompleteKeywords");
        MONACO_BRIDGED_ACTIONS.put("scinotes-undo", "undo");
        MONACO_BRIDGED_ACTIONS.put("scinotes-redo", "redo");
        MONACO_BRIDGED_ACTIONS.put("scinotes-cut", "cut");
        MONACO_BRIDGED_ACTIONS.put("scinotes-copy", "copy");
        MONACO_BRIDGED_ACTIONS.put("scinotes-copy-with-lines", "copyHtmlWithLineNumbers");
        MONACO_BRIDGED_ACTIONS.put("scinotes-paste", "paste");
        MONACO_BRIDGED_ACTIONS.put("scinotes-paste-column", "pasteColumn");
        MONACO_BRIDGED_ACTIONS.put("scinotes-repeat-column", "repeatSelectionOnColumn");
        MONACO_BRIDGED_ACTIONS.put("scinotes-delete", "delete");
        MONACO_BRIDGED_ACTIONS.put("scinotes-select-all", "selectAll");
        MONACO_BRIDGED_ACTIONS.put("scinotes-select-block", "selectBlock");
        MONACO_BRIDGED_ACTIONS.put("scinotes-find-next", "findNext");
        MONACO_BRIDGED_ACTIONS.put("scinotes-find-previous", "findPrevious");
        MONACO_BRIDGED_ACTIONS.put("scinotes-incremental-search", "incrementalSearch");
        MONACO_BRIDGED_ACTIONS.put("scinotes-search-files", "searchFiles");
        MONACO_BRIDGED_ACTIONS.put("scinotes-word-file", "searchWordInFiles");
        MONACO_BRIDGED_ACTIONS.put("scinotes-completion", "completion");
        MONACO_BRIDGED_ACTIONS.put("scinotes-next-tab", "nextTab");
        MONACO_BRIDGED_ACTIONS.put("scinotes-previous-tab", "previousTab");
        MONACO_BRIDGED_ACTIONS.put("scinotes-next-paragraph", "nextParagraph");
        MONACO_BRIDGED_ACTIONS.put("scinotes-previous-paragraph", "previousParagraph");
        MONACO_BRIDGED_ACTIONS.put("scinotes-help", "help");
        MONACO_BRIDGED_ACTIONS.put("scinotes-help-keyword", "helpOnKeyword");
        MONACO_BRIDGED_ACTIONS.put("scinotes-about", "about");
        MONACO_BRIDGED_ACTIONS.put("scinotes-code-navigator", "codeNavigator");
        MONACO_BRIDGED_ACTIONS.put("scinotes-set-anchor", "setAnchor");
        MONACO_BRIDGED_ACTIONS.put("scinotes-remove-anchor", "removeAnchor");
        MONACO_BRIDGED_ACTIONS.put("scinotes-next-anchor", "nextAnchor");
        MONACO_BRIDGED_ACTIONS.put("scinotes-previous-anchor", "previousAnchor");
        MONACO_BRIDGED_ACTIONS.put("scinotes-clone-current", "cloneTabInNewWindow");
        MONACO_BRIDGED_ACTIONS.put("scinotes-export-current", "detachTabInNewWindow");
        MONACO_BRIDGED_ACTIONS.put("scinotes-tabify", "shiftRight");
        MONACO_BRIDGED_ACTIONS.put("scinotes-untabify", "shiftLeft");
        MONACO_BRIDGED_ACTIONS.put("scinotes-indent", "correctIndentation");
        MONACO_BRIDGED_ACTIONS.put("scinotes-remove-whites", "removeTrailingSpaces");
        MONACO_BRIDGED_ACTIONS.put("scinotes-help-prototype", "generateHelpComments");
        MONACO_BRIDGED_ACTIONS.put("scinotes-comment", "commentSelection");
        MONACO_BRIDGED_ACTIONS.put("scinotes-uncomment", "uncommentSelection");
        MONACO_BRIDGED_ACTIONS.put("scinotes-tolowercase", "toLowerCase");
        MONACO_BRIDGED_ACTIONS.put("scinotes-touppercase", "toUpperCase");
        MONACO_BRIDGED_ACTIONS.put("scinotes-capitalize", "capitalize");
        MONACO_BRIDGED_ACTIONS.put("scinotes-double-quote", "doubleQuoteStrings");
        MONACO_BRIDGED_ACTIONS.put("scinotes-load-scilab", "loadIntoScilab");
        MONACO_BRIDGED_ACTIONS.put("scinotes-execute-scilab", "executeFileIntoScilab");
        MONACO_BRIDGED_ACTIONS.put("scinotes-evaluate-selection", "evaluateSelection");
        MONACO_BRIDGED_ACTIONS.put("scinotes-save-execute", "saveAndExecuteFile");
        MONACO_BRIDGED_ACTIONS.put("scinotes-save-execute-all", "saveAndExecuteAllFiles");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-continue", "debugContinue");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-pause", "debugPause");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-step-over", "debugStepOver");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-step-in", "debugStepIn");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-step-out", "debugStepOut");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-disconnect", "debugDisconnect");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-toggle-breakpoint", "debugToggleBreakpoint");
        MONACO_BRIDGED_ACTIONS.put("scinotes-debug-run-to-cursor", "debugRunToCursor");
    }

    /**
     * Constructor.
     * @param editor the SciNotes editor
     */
    public MonacoEditorPane(final SciNotes editor) {
        super(editor);
        browserHost = new JPanel(new BorderLayout());
        sharedBrowser = getSharedBrowser(editor);
        cefPanel = sharedBrowser.cefPanel;
        client = sharedBrowser.client;
        browser = sharedBrowser.browser;
    }

    /**
     * Complete the initialization: must be called once, right after the
     * constructor returned (see ScilabEditorPane#init()).
     */
    @Override
    public void init() {
        super.init();
        /*
         * Unlike a classic ScilabEditorPane, this MonacoEditorPane ("this")
         * is never added to the visible Swing tree: EditorComponent's
         * "alternative frontend" constructor displays browserHost instead
         * (see MonacoEditorPane's use of EditorComponent(this, browserHost)).
         * super.requestFocus() below is therefore a no-op on "this" - not
         * showing, so the KeyboardFocusManager can never accept it as a
         * focus owner. browserHost is the real, showing stand-in; make it
         * focusable so a genuine Swing focus owner can be established.
         */
        browserHost.setFocusable(true);
        browserHost.addFocusListener(new FocusAdapter() {
            @Override
            public void focusGained(FocusEvent event) {
                /*
                 * browserHost only stands in for this pane in the Swing focus
                 * graph; the keystrokes belong to the native browser. Leaving
                 * it the focus owner is what makes a freshly opened tab show a
                 * blinking caret and swallow everything typed into it.
                 */
                if (browserReady && !browserDisposed && sharedBrowser.getActivePane() == MonacoEditorPane.this) {
                    cefPanel.reclaimSwingFocus();
                }
            }
        });
        boolean installBrowserHandlers = sharedBrowser.isEmpty();
        if (installBrowserHandlers) {
        browser.getUIComponent().addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent event) {
                if (!event.isPopupTrigger()) {
                    MonacoEditorPane pane = sharedBrowser.getActivePane();
                    if (pane != null) {
                        pane.hideActiveContextMenu();
                        pane.focusMonacoFromUserClick();
                    }
                }
            }
        });

        client.addMessageRouter(CefMessageRouter.create(new CefMessageRouterConfig("ScilabMonaco", "ScilabMonacoAbort"), new CefMessageRouterHandlerAdapter() {
            @Override
            public boolean onQuery(CefBrowser cefBrowser, CefFrame frame, long queryId, String request, boolean persistent, CefQueryCallback callback) {
                if (cefBrowser != browser) {
                    return false;
                }

                sharedBrowser.routeMessage(request);
                callback.success("");
                return true;
            }
        }));

        client.addDragHandler(new CefDragHandler() {
            @Override
            public boolean onDragEnter(CefBrowser cefBrowser, CefDragData dragData, int mask) {
                Vector<String> paths = new Vector<String>();
                if (cefBrowser == browser && dragData != null && dragData.isFile()) {
                    dragData.getFilePaths(paths);
                }

                MonacoEditorPane pane = sharedBrowser.getActivePane();
                if (pane != null) {
                    synchronized (pane.droppedFilesLock) {
                        pane.draggedFilePaths = new ArrayList<String>(paths);
                    }
                }

                // Let Chromium emit the DOM drop event. The page prevents
                // Monaco's default insertion and notifies Java on the actual drop.
                return false;
            }
        });

        cefPanel.addLifeSpanHandler(new CefLifeSpanHandlerAdapter() {
            @Override
            public boolean onBeforePopup(CefBrowser cefBrowser, CefFrame frame, String targetUrl,
                    String targetFrameName) {
                if (cefBrowser != browser) {
                    return false;
                }

                // A popup here would be a bare Chromium window: the page asks
                // for a browser, the user gets theirs.
                openInUserBrowser(targetUrl);
                return true;
            }
        });

        cefPanel.addKeyboardHandler(new CefKeyboardHandlerAdapter() {
            @Override
            public boolean onPreKeyEvent(CefBrowser cefBrowser, CefKeyEvent event, BoolRef isKeyboardShortcut) {
                if (cefBrowser != browser || event.type != CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN) {
                    return false;
                }

                MonacoEditorPane pane = sharedBrowser.getActivePane();
                if (pane != null && pane.handleBrowserShortcut(event)) {
                    pane.lastPreHandledKeyCode = event.windows_key_code;
                    pane.lastPreHandledModifiers = event.modifiers;
                    isKeyboardShortcut.set(true);
                    return true;
                }

                return false;
            }

            @Override
            public boolean onKeyEvent(CefBrowser cefBrowser, CefKeyEvent event) {
                if (cefBrowser != browser || event.type != CefKeyEvent.EventType.KEYEVENT_RAWKEYDOWN) {
                    return false;
                }

                MonacoEditorPane pane = sharedBrowser.getActivePane();
                if (pane == null) {
                    return false;
                }
                if (event.windows_key_code == pane.lastPreHandledKeyCode
                        && event.modifiers == pane.lastPreHandledModifiers) {
                    pane.lastPreHandledKeyCode = -1;
                    pane.lastPreHandledModifiers = -1;
                    return true;
                }

                return pane.handleBrowserShortcut(event);
            }
        });

        client.addLoadHandler(new CefLoadHandlerAdapter() {
            @Override
            public void onLoadEnd(CefBrowser cefBrowser, CefFrame frame, int httpStatusCode) {
                if (cefBrowser == browser && frame.isMain()) {
                    sharedBrowser.loadFinished();
                    SwingUtilities.invokeLater(new Runnable() {
                        public void run() {
                            MonacoEditorPane pane = sharedBrowser.getActivePane();
                            if (pane != null) {
                                pane.updateUndoRedoState();
                                pane.focusMonacoIfRequested();
                            }
                        }
                    });
                }
            }

            @Override
            public void onLoadStart(CefBrowser cefBrowser, CefFrame frame, TransitionType transitionType) {
                if (cefBrowser == browser && frame.isMain()) {
                    sharedBrowser.loadStarted();
                }
            }
        });
        }

        documentListener = new DocumentListener() {
            public void insertUpdate(DocumentEvent e) {
                onDocumentInsert(e.getOffset(), e.getLength());
                updateLineNumberOptions();
                indexDocument();
            }

            public void removeUpdate(DocumentEvent e) {
                onDocumentRemove(e.getOffset(), e.getLength());
                updateLineNumberOptions();
                indexDocument();
            }

            public void changedUpdate(DocumentEvent e) {
                // Attribute-only change (styling): no text impact on Monaco.
            }
        };

        setEditorComponent(new EditorComponent(this, browserHost));
        sharedBrowser.register(this);
        if (installBrowserHandlers) {
            sharedBrowser.activate(this);
        }
        attachDocumentListener(getDocument());
        disableReplicaUndoManager(getDocument());
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                installWindowShortcuts();
            }
        });
        indexDocument();
        sharedBrowser.paneConstructed(this);
    }

    private String getMonacoDocumentId() {
        return getUUID().toString();
    }

    private boolean isDocumentClean() {
        return !(getDocument() instanceof ScilabDocument)
            || !((ScilabDocument) getDocument()).isContentModified();
    }

    private void initializeSharedBrowser() {
        if (!browserReady || sharedBrowser.initialDocumentAdopted) {
            return;
        }
        syncDocumentToMonaco();
        applyDisplayOptions();
        configureShortcuts();
        configureLsp();
        configureDap();
        browser.executeJavaScript("window.SciNotesMonacoAdoptInitialDocument("
            + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(getDocumentUri()) + ", "
            + documentRevision + ", " + isDocumentClean() + ", " + (!isEditable())
            + ");", browser.getURL(), 1);
        monacoDocumentCreated = true;
        sharedBrowser.initialDocumentAdopted = true;
        joinSharedHistory(true);
        updateAnchorDecorations();
        updateReadOnlyState();
        revealPendingLine();
        syncDapDocument();
    }

    private void createMonacoDocument() {
        if (!browserReady || monacoDocumentCreated || getUUID() == null) {
            return;
        }
        browser.executeJavaScript("window.SciNotesMonacoCreateDocument("
            + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(getDocumentUri()) + ", "
            + quoteJs(getDocumentText()) + ", " + documentRevision + ", "
            + isDocumentClean() + ", " + (!isEditable()) + ");", browser.getURL(), 1);
        monacoDocumentCreated = true;
        joinSharedHistory(true);
        syncDapDocument();
    }

    private void selectMonacoDocument() {
        if (!browserReady) {
            return;
        }
        createMonacoDocument();
        browser.executeJavaScript("window.SciNotesMonacoSelectDocument("
            + quoteJs(getMonacoDocumentId()) + ", {\"options\":"
            + getDisplayOptionsConfiguration() + ",\"anchors\":"
            + getAnchorConfiguration() + "});", browser.getURL(), 1);
        revealPendingLine();
    }

    private void closeMonacoDocument() {
        if (getUUID() == null) {
            return;
        }
        removeDapDocument();
        browser.executeJavaScript("window.SciNotesMonacoCloseDocument("
            + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
        monacoDocumentCreated = false;
    }

    @Override
    public void setDocument(Document doc) {
        detachDocumentListener(getDocument());
        super.setDocument(doc);
        attachDocumentListener(doc);
        disableReplicaUndoManager(doc);
        syncDocumentToMonaco();
        indexDocument();
    }

    @Override
    public void setName(String name) {
        boolean wired = sharedBrowser != null && browserReady && monacoDocumentCreated
            && getUUID() != null;
        String previousSourcePath = wired ? getSavedSourcePath() : "";
        if (wired) {
            // Preserve Monaco's live decoration lines before Save As replaces
            // the text model and before the persisted path is migrated.
            prepareDapBreakpointsForSave();
        }
        super.setName(name);
        if (wired) {
            browser.executeJavaScript("window.SciNotesMonacoRenameDocument("
                + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(getDocumentUri())
                + ");", browser.getURL(), 1);
            /*
             * No adoption on a rename: Save As has just written this tab's
             * own text to that file, so it is not the one to be updated.
             */
            joinSharedHistory(false);
            String sourcePath = getSavedSourcePath();
            if (!previousSourcePath.isEmpty() && !sourcePath.isEmpty()
                    && !previousSourcePath.equals(sourcePath)
                    && SciNotesDapManager.getEndpoint() != null) {
                // The old file keeps its breakpoints: once no tab shows it,
                // the browser declares it from the store.
                SciNotesBreakpointStore.getInstance().copy(previousSourcePath, sourcePath);
            }
            syncDapDocument();
        }
        updateFileWatch();
        indexDocument();
    }

    /*
     * Follow the file this tab shows, so that a change made by another
     * program is seen without waiting for the tab to take the focus.
     */
    private void updateFileWatch() {
        File file = getName() == null || getName().isEmpty() ? null : new File(getName());
        if (watchedFile == null ? file == null : watchedFile.equals(file)) {
            return;
        }
        if (watchedFile != null) {
            SciNotesFileWatcher.unwatch(watchedFile);
        }
        watchedFile = file;
        if (watchedFile != null) {
            SciNotesFileWatcher.watch(watchedFile);
        }
    }

    /**
     * A file shown by SciNotes was written, created or removed by another
     * program. Reload it when nothing is at stake, and otherwise only say so:
     * the tab keeps what it holds, and the usual dialog asks what to do when
     * it takes the focus.
     *
     * @param file the file that changed
     */
    static void fileChangedOnDisk(File file) {
        String key;
        try {
            key = file.getCanonicalPath();
        } catch (IOException e) {
            key = file.getAbsolutePath();
        }

        List<MonacoEditorPane> panes = getPanesForFile(key);
        if (panes.isEmpty()) {
            return;
        }

        if (!file.exists()) {
            for (MonacoEditorPane pane : panes) {
                pane.reportFileRemoved();
            }
            return;
        }

        boolean changedOnDisk = false;
        boolean modified = false;
        for (MonacoEditorPane pane : panes) {
            changedOnDisk |= pane.checkExternalModif();
            Document doc = pane.getDocument();
            modified |= doc instanceof ScilabDocument && ((ScilabDocument) doc).isContentModified();
        }

        // Our own save, already acknowledged by acknowledgeSharedSave().
        if (!changedOnDisk) {
            return;
        }

        if (modified) {
            for (MonacoEditorPane pane : panes) {
                pane.reportExternalModification();
            }
            return;
        }

        reloadFromDisk(file, panes);
    }

    /*
     * Nothing was being edited, so the new contents replace what the tabs
     * show. It is sent as an ordinary edit -- the smallest one that turns the
     * old text into the new -- which keeps the carets, the scroll position
     * and the undo history, and lets Ctrl+Z undo the reload itself.
     */
    private static boolean reloadFromDisk(File file, List<MonacoEditorPane> panes) {
        MonacoEditorPane first = panes.get(0);
        ScilabDocument contents = new ScilabDocument(false);
        String text;
        try {
            ((ScilabEditorKit) first.getEditorKit()).read(first.getEditor(), file, contents, 0);
            text = contents.getText(0, contents.getLength());
        } catch (IOException | BadLocationException | RuntimeException e) {
            System.err.println(e);
            return false;
        }

        long modification = file.lastModified();
        boolean writable = file.canWrite();
        List<SharedBrowser> updated = new ArrayList<SharedBrowser>();
        for (MonacoEditorPane pane : panes) {
            ScilabDocument doc = pane.getDocument() instanceof ScilabDocument
                ? (ScilabDocument) pane.getDocument() : null;
            if (doc != null) {
                doc.setEncoding(contents.getEncoding());
                doc.setEOL(contents.getEOL());
                if (contents.getBinary()) {
                    pane.setBinary(true);
                }
            }
            pane.setLastModified(modification);
            pane.lastMonacoExternalModification = modification;
            pane.setReadOnly(!writable);
            /*
             * One tab per window applies the edit: the tabs of a window on
             * one file share a buffer, and each Java replica is updated from
             * that buffer, as for any other edit.
             */
            if (!updated.contains(pane.sharedBrowser) && pane.browserReady
                    && pane.monacoDocumentCreated) {
                updated.add(pane.sharedBrowser);
                pane.browser.executeJavaScript("window.SciNotesMonacoApplyContentAsEdit("
                    + quoteJs(pane.getMonacoDocumentId()) + ", " + quoteJs(text)
                    + ", true);", pane.browser.getURL(), 1);
            }
            SciNotes editor = pane.getEditor();
            if (editor != null && editor.getTextPane() == pane) {
                editor.getInfoBar().setText(pane.getInfoBarText());
            }
            pane.updateAnchorDecorations();
        }
        return true;
    }

    private void reportExternalModification() {
        SciNotes editor = getEditor();
        if (editor != null && editor.getTextPane() == this) {
            editor.getInfoBar().setText(SciNotesMessages.EXTERNAL_MODIFICATION_INFO);
        }
    }

    /*
     * The file is gone: keep the text, and let the tab say it is no longer
     * saved anywhere -- Monaco forgets which version was the saved one.
     */
    private void reportFileRemoved() {
        if (browserReady && monacoDocumentCreated) {
            browser.executeJavaScript("window.SciNotesMonacoForgetSavedVersion("
                + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
        }
        Document doc = getDocument();
        if (doc instanceof ScilabDocument && !((ScilabDocument) doc).isContentModified()) {
            ((ScilabDocument) doc).setContentModified(true);
            updateTitle();
        }
        SciNotes editor = getEditor();
        if (editor != null && editor.getTextPane() == this) {
            editor.getInfoBar().setText(SciNotesMessages.EXTERNAL_MODIFICATION_INFO);
        }
    }

    /*
     * One key per file, whatever path spelling each window opened it with.
     * An unnamed buffer belongs to its window only.
     */
    private String getSharedDocumentKey() {
        String name = getName();
        if (name == null || name.isEmpty()) {
            return null;
        }
        if (!name.equals(sharedDocumentName)) {
            File file = new File(name);
            try {
                sharedDocumentKey = file.getCanonicalPath();
            } catch (IOException e) {
                sharedDocumentKey = file.getAbsolutePath();
            }
            sharedDocumentName = name;
        }
        return sharedDocumentKey;
    }

    /** Every tab showing this file, in all windows; split secondaries excluded. */
    private static List<MonacoEditorPane> getPanesForFile(String key) {
        List<MonacoEditorPane> panes = new ArrayList<MonacoEditorPane>();
        if (key == null) {
            return panes;
        }
        List<SharedBrowser> browsers = new ArrayList<SharedBrowser>();
        synchronized (SHARED_BROWSERS) {
            browsers.addAll(SHARED_BROWSERS.values());
        }
        for (SharedBrowser browser : browsers) {
            for (MonacoEditorPane pane : browser.panes) {
                // The secondary half of a split shows the document of its
                // primary, which is in this list on its own.
                if (!pane.browserDisposed && pane.splitPrimary == null
                        && key.equals(pane.getSharedDocumentKey())) {
                    panes.add(pane);
                }
            }
        }
        return panes;
    }

    private List<SharedBrowser> getOtherSharedBrowsers() {
        List<SharedBrowser> others = new ArrayList<SharedBrowser>();
        synchronized (SHARED_BROWSERS) {
            for (SharedBrowser other : SHARED_BROWSERS.values()) {
                if (other != sharedBrowser) {
                    others.add(other);
                }
            }
        }
        return others;
    }

    /*
     * Relaying costs a message per edit and a digest per undo step, so a page
     * only relays a file another window also shows. This window starts
     * showing such a file: both sides start relaying, and the step open over
     * there is closed first -- this window's history is empty, and the next
     * edits must not extend a step it does not have.
     */
    private void joinSharedHistory(boolean adoptContent) {
        String key = getSharedDocumentKey();
        if (key == null) {
            return;
        }
        MonacoEditorPane source = null;
        for (SharedBrowser other : getOtherSharedBrowsers()) {
            MonacoEditorPane pane = other.getSharedDocumentPane(key);
            if (pane != null) {
                pane.setSharedDocument(true);
                if (source == null) {
                    source = pane;
                }
            }
        }
        if (source != null) {
            setSharedDocument(true);
            if (adoptContent) {
                adoptSharedContent(source);
            }
        }
    }

    /*
     * The file was read from disk, but another window is editing it: what
     * that window holds is the live version. Take it as an edit over the
     * contents just read -- so this tab shows what the others show, and says
     * it is modified -- and start this window's history from there, since the
     * steps that led to it belong to the other window.
     */
    private void adoptSharedContent(MonacoEditorPane source) {
        if (!browserReady || !monacoDocumentCreated) {
            return;
        }
        String text = source.getDocumentText();
        if (text == null || text.equals(getDocumentText())) {
            return;
        }
        browser.executeJavaScript("window.SciNotesMonacoAdoptSharedContent("
            + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(text)
            + ");", browser.getURL(), 1);
    }

    private void setSharedDocument(boolean shared) {
        if (browserReady && monacoDocumentCreated && !browserDisposed) {
            browser.executeJavaScript("window.SciNotesMonacoSetSharedDocument("
                + quoteJs(getMonacoDocumentId()) + ", " + shared + ");", browser.getURL(), 1);
        }
    }

    /**
     * Every tab of every window showing the file whose contents were just
     * written to disk: they hold the same text, so they are saved too. Their
     * Monaco buffer takes that version as its saved reference, their tab
     * loses its star, and their disk timestamp is acknowledged -- without
     * this, a save made by SciNotes itself looks like an external change and
     * each other window asks whether to reload.
     *
     * @param savedFile the file SciNotes has just written
     */
    public void acknowledgeSharedSave(File savedFile) {
        String key = getSharedDocumentKey();
        if (key == null || savedFile == null) {
            return;
        }
        long modification = savedFile.lastModified();
        String digest = contentDigest(getDocumentText());
        for (MonacoEditorPane pane : getPanesForFile(key)) {
            if (pane != this) {
                /*
                 * Immediately, not on the event queue: closing a window saves
                 * and closes its tabs in one go, without giving that queue a
                 * chance to run, and a tab still holding the old timestamp
                 * would be asked about reloading the file SciNotes has just
                 * written.
                 */
                pane.acknowledgeSave(modification, digest);
            }
        }
    }

    /**
     * Whether another tab still shows this file.
     *
     * @param otherWindowsOnly true to ignore the tabs of this window, when
     *        they are about to be closed with it
     */
    public boolean isOpenInAnotherPane(boolean otherWindowsOnly) {
        for (MonacoEditorPane pane : getPanesForFile(getSharedDocumentKey())) {
            if (pane != this && !(otherWindowsOnly && pane.sharedBrowser == sharedBrowser)) {
                return true;
            }
        }
        return false;
    }

    private void acknowledgeSave(long modification, String digest) {
        setLastModified(modification);
        lastMonacoExternalModification = modification;
        Document doc = getDocument();
        if (doc instanceof ScilabDocument && ((ScilabDocument) doc).isContentModified()) {
            // Clears the star, and tells Monaco through
            // contentMarkedUnmodified() -- with this tab's own replica, which
            // may be one edit behind the buffer that was written.
            ((ScilabDocument) doc).setContentModified(false);
        }
        // The contents actually written, whatever this tab has received so far.
        markMonacoSaved(digest);
    }

    /** Forward an undo history message of this window to the other windows showing the file. */
    private void relaySharedHistory(String message) {
        String key = getSharedDocumentKey();
        if (key == null || browserDisposed) {
            return;
        }
        boolean relayed = false;
        for (SharedBrowser other : getOtherSharedBrowsers()) {
            MonacoEditorPane pane = other.getSharedDocumentPane(key);
            if (pane != null) {
                pane.browser.executeJavaScript("window.SciNotesMonacoApplySharedHistory("
                    + quoteJs(pane.getMonacoDocumentId()) + ", " + quoteJs(message)
                    + ");", pane.browser.getURL(), 1);
                relayed = true;
            }
        }
        if (relayed) {
            SHARED_HISTORY_ORIGINS.put(key, this);
        } else {
            // The last other window showing this file is gone: stop paying
            // for messages and digests nobody reads.
            setSharedDocument(false);
            SHARED_HISTORY_ORIGINS.remove(key);
        }
    }

    /*
     * The text of this window no longer matches the window that sent the
     * last message, which is a bug. Put the sender's text back as a step of
     * this window's history rather than replacing the buffer, which would
     * lose that history.
     */
    private void repairSharedDocument(String detail) {
        String key = getSharedDocumentKey();
        System.err.println("SciNotes: undo history of " + key + " diverged between windows: " + detail);
        MonacoEditorPane origin = key == null ? null : SHARED_HISTORY_ORIGINS.get(key);
        if (origin == null || origin == this || origin.browserDisposed
                || !browserReady || !monacoDocumentCreated) {
            return;
        }
        browser.executeJavaScript("window.SciNotesMonacoRepairSharedDocument("
            + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(origin.getDocumentText())
            + ");", browser.getURL(), 1);
    }

    private void indexDocument() {
        // setDocument() is invoked by Swing while the superclass is still
        // being constructed, before ScilabEditorPane has assigned its UUID.
        if (getDocument() == null || getUUID() == null
                || SciNotesLspManager.getOrStartEndpoint() == null) {
            return;
        }
        String uri = isScilabDocument() ? getDocumentUri() : null;
        if (indexedDocumentUri != null && !indexedDocumentUri.equals(uri)) {
            SciNotesLspJNI.removeIndexedDocument(indexedDocumentUri);
        }
        indexedDocumentUri = uri;
        if (uri != null) {
            SciNotesLspJNI.indexDocument(uri, getDocumentText(), ++indexedDocumentVersion);
        }
    }

    /*
     * An unnamed buffer is Scilab; a named one only when Monaco gives its
     * model the Scilab language, which follows the extensions the page
     * received from SciNotes (see SharedBrowser.registerScilabExtensions()).
     */
    private boolean isScilabDocument() {
        String name = getName();
        if (name == null || name.isEmpty()) {
            return true;
        }

        String lowerCaseName = name.toLowerCase();
        for (String extension : SciNotes.getScilabExtensions()) {
            if (lowerCaseName.endsWith(extension)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Monaco owns the undo stack, so the read-replica must not record undoable
     * edits of its own (they would only accumulate and desync the toolbar
     * buttons). Button state is driven explicitly from {@link #updateInfosWhenFocused()}.
     */
    private void disableReplicaUndoManager(Document doc) {
        if (doc instanceof ScilabDocument) {
            ((ScilabDocument) doc).disableUndoManager();
        }
    }

    @Override
    public void updateInfosWhenFocused() {
        SciNotes ed = getEditor();
        if (browserDisposed || ed == null || ed.getInfoBar() == null) {
            return;
        }

        int selectedIndex = getSelectedEditorComponentIndex(ed);
        if (selectedIndex < 0) {
            return;
        }

        boolean wasActive = sharedBrowser.getActivePane() == this;
        sharedBrowser.activate(this);

        ScilabDocument doc = (ScilabDocument) getDocument();
        doc.setFocused(true);
        SciNotes.setEditor(ed);
        doc.getUndoManager().enableUndoRedoButtons();

        /*
         * Only force a repaint/relayout of the native browser surface on an
         * actual tab switch (see the matching comment in
         * SharedBrowser.activate()). This method also runs on every Monaco
         * "focus:in" notification while staying on the same tab; redoing
         * this work then contributed to a Java<->JS focus feedback loop.
         */
        if (!wasActive) {
            refreshMonacoView();
        }
        handleExternalModification(ed);
        updateUndoRedoState();
    }

    /** Refresh the native browser surface and Monaco's dimensions after a hidden tab is selected. */
    private void refreshMonacoView() {
        if (browserDisposed) {
            return;
        }

        browserHost.revalidate();
        browserHost.repaint();
        cefPanel.revalidate();
        cefPanel.repaint();
        browser.getUIComponent().revalidate();
        browser.getUIComponent().repaint();
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                if (browserReady && !browserDisposed && cefPanel.isShowing()) {
                    browser.executeJavaScript("window.SciNotesMonacoLayout();", browser.getURL(), 1);
                }
            }
        });
    }

    private int getSelectedEditorComponentIndex(SciNotes ed) {
        if (ed.getTabPane() == null || getEditorComponent() == null) {
            return -1;
        }

        int index = ed.getTabPane().indexOfComponent(getEditorComponent());
        if (index < 0 || ed.getTabPane().getSelectedComponent() != getEditorComponent()) {
            return -1;
        }

        return index;
    }

    private void handleExternalModification(SciNotes ed) {
        long modification = getLastModification();
        if (!checkExternalModif() || lastMonacoExternalModification >= modification) {
            ed.getInfoBar().setText(getInfoBarText());
            return;
        }

        ed.getInfoBar().setText(SciNotesMessages.EXTERNAL_MODIFICATION_INFO);
        if (externalReloadPromptActive) {
            return;
        }

        externalReloadPromptActive = true;
        try {
            AnswerOption answer = ScilabModalDialog.show(ed,
                    String.format(SciNotesMessages.ASK_TO_RELOAD, getShortName()),
                    SciNotesMessages.RELOAD,
                    IconType.QUESTION_ICON,
                    ButtonType.YES_NO);

            if (answer == AnswerOption.YES_OPTION) {
                int currentIndex = ed.getTabPane().indexOfComponent(getEditorComponent());
                if (currentIndex >= 0) {
                    ed.reload(currentIndex);
                }
            }

            lastMonacoExternalModification = modification;
        } finally {
            externalReloadPromptActive = false;
        }
    }

    private void updateUndoRedoButtons(boolean canUndo, boolean canRedo) {
        SciNotes ed = getEditor();
        if (ed != null) {
            ed.enableUndoButton(canUndo);
            ed.enableRedoButton(canRedo);
        }
    }

    @Override
    public void setReadOnly(boolean readonly) {
        super.setReadOnly(readonly);
        updateReadOnlyState();
    }

    /**
     * Reload this tab's file, without closing and reopening its tab: every
     * tab on that file, in every window, gets its new contents as one
     * ordinary edit, so carets, scroll positions and undo histories survive
     * and the reload itself can be undone.
     *
     * @return false when the file cannot be read, so that the caller falls
     *         back to reopening the tab
     */
    boolean reloadFileInPlace() {
        String name = getName();
        if (name == null) {
            return false;
        }

        File f = new File(name);
        if (!f.exists() || !f.canRead()) {
            return false;
        }

        List<MonacoEditorPane> panes = getPanesForFile(getSharedDocumentKey());
        if (panes.isEmpty()) {
            panes.add(this);
        }
        return reloadFromDisk(f, panes);
    }

    @Override
    public void resetFont() {
        super.resetFont();
        applyDisplayOptions();
    }

    @Override
    public void configurationChanged(SciNotesConfiguration.Conf conf) {
        super.configurationChanged(conf);

        if (conf.display || conf.font || conf.colors || conf.systemfont) {
            applyDisplayOptions();
        }

        if (conf.keymap) {
            configureShortcuts();
            installWindowShortcuts();
        }
    }

    @Override
    public void activateHelpOnTyping() {
        super.activateHelpOnTyping();
        applyDisplayOptions();
    }

    @Override
    public void init(final int pos) {
        super.init(pos);
        // Opening a file at a line (a debug stop, a definition) asks for it
        // before this runs: that line wins over the caret the tab starts with.
        final int requests = revealRequests;
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                if (revealRequests == requests) {
                    revealLine(pos);
                }
            }
        });
    }

    @Override
    public void requestFocus() {
        /*
         * Claim real Swing focus ownership too, not just native CEF focus
         * below. Without this, the KeyboardFocusManager never considers the
         * window's focus request satisfied (this override used to only ever
         * touch the separate heavyweight cefPanel), so it keeps retrying its
         * default focus restoration, which lands on SciNotesContents (the
         * window's content pane) and forwards right back here via its own
         * FocusListener - an unbounded same-EDT retry loop.
         *
         * super.requestFocus() targets "this", but "this" is never actually
         * added to the visible Swing tree for a Monaco pane (EditorComponent
         * displays browserHost instead - see init()). A
         * non-showing component can never become the real focus owner, so
         * that call alone is a no-op and does not break the loop.
         * browserHost is the real, showing stand-in: request focus there.
         * Harmless to also hold logical Swing focus on it: actual keystrokes
         * are still delivered to Monaco through cefPanel's own native OS
         * focus, entirely separate from this bookkeeping.
         */
        super.requestFocus();
        browserHost.requestFocus();
        updateInfosWhenFocused();
        focusRequested = true;
        focusMonacoIfRequested();
    }

    @Override
    public boolean requestFocusInWindow() {
        boolean result = super.requestFocusInWindow();
        result = browserHost.requestFocusInWindow() || result;
        updateInfosWhenFocused();
        focusRequested = true;
        focusMonacoIfRequested();
        return result;
    }

    @Override
    public void scrollTextToLineNumber(int lineNumber, boolean highlight) {
        revealLineNumber(lineNumber);
    }

    @Override
    public void scrollTextToLineNumber(int lineNumber, boolean highlight, boolean setCaret, boolean centered) {
        revealLineNumber(lineNumber);
    }

    @Override
    public void scrollTextToLineNumberInWhereami(int lineNumber, String funname, boolean highlight) {
        revealLineNumber(lineNumber);
    }

    @Override
    public void scrollTextToLineNumberInWhereami(int lineNumber, String funname, boolean highlight, boolean setCaret, boolean centered) {
        revealLineNumber(lineNumber);
    }

    @Override
    public void scrollTextToPos(int pos) {
        scrollTextToPos(pos, true, false);
    }

    @Override
    public void scrollTextToPos(int pos, boolean setCaret, boolean centered) {
        if (setCaret) {
            setCaretPosition(pos);
        }
        setMonacoPosition(pos);
    }

    @Override
    public KeywordEvent getKeywordEvent(int position) {
        KeywordEvent event = super.getKeywordEvent(position);
        if (event.getType() == ScilabLexerConstants.URL || event.getType() == ScilabLexerConstants.MAIL) {
            return event;
        }

        KeywordEvent urlEvent = getUrlKeywordEvent(position);
        if (urlEvent != null) {
            return urlEvent;
        }

        return event;
    }

    @Override
    public KeywordEvent getKeywordEvent(boolean caret, boolean strict) {
        return super.getKeywordEvent(true, strict);
    }

    @Override
    public String getHelpableKeyword(boolean caret) {
        return super.getHelpableKeyword(true);
    }

    @Override
    public String getCodeToExecute() {
        return trimBlankEdgeLines(super.getCodeToExecute());
    }

    @Override
    public Rectangle2D modelToView2D(int pos) throws BadLocationException {
        Rectangle2D rect = super.modelToView2D(pos);
        if (rect != null) {
            return rect;
        }

        Point point = lastContextMenuPoint == null ? new Point() : lastContextMenuPoint;
        return new Rectangle(point.x, point.y, 1, 16);
    }

    @Override
    public Point getMousePoint() {
        return new Point(lastContextMenuPoint);
    }

    @Override
    public void close() {
        final SciNotes currentEditor = getEditor();
        final ScilabEditorPane origin = definitionOrigin;
        definitionOrigin = null;
        if (indexedDocumentUri != null) {
            SciNotesLspJNI.removeIndexedDocument(indexedDocumentUri);
            indexedDocumentUri = null;
        }
        detachDocumentListener(getDocument());
        if (watchedFile != null) {
            SciNotesFileWatcher.unwatch(watchedFile);
            watchedFile = null;
        }
        for (Iterator<MonacoEditorPane> it = SHARED_HISTORY_ORIGINS.values().iterator(); it.hasNext();) {
            if (it.next() == this) {
                it.remove();
            }
        }
        disposeBrowser();
        super.close();
        if (currentEditor != null && origin != null) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    int index = currentEditor.getTabPane().indexOfComponent(origin.getEditorComponent());
                    if (index >= 0) {
                        currentEditor.getTabPane().setSelectedIndex(index);
                    }
                }
            });
        }
    }

    @Override
    public void destroy() {
        close();
    }

    private void disposeBrowser() {
        if (!browserDisposed) {
            browserDisposed = true;
            browserReady = false;
            if (splitPrimary != null) {
                exitSplitView();
            }
            sharedBrowser.unregister(this);
        }
    }

    /**
     * The pane owning the Monaco document of the split this pane belongs to,
     * or null when its tab is not split. Works from either half.
     */
    private MonacoEditorPane splitPrimaryPane() {
        if (splitPrimary != null) {
            return splitPrimary;
        }
        return splitSecondaryPane() != null ? this : null;
    }

    /**
     * The pane holding the split's second view, or null when its tab is not
     * split. Works from either half.
     */
    private MonacoEditorPane splitSecondaryPane() {
        if (splitPrimary != null) {
            return this;
        }
        ScilabEditorPane other = getOtherPaneInSplit();
        if (other instanceof MonacoEditorPane && ((MonacoEditorPane) other).splitPrimary == this) {
            return (MonacoEditorPane) other;
        }
        return null;
    }

    /**
     * Give the shared browser to this pane. For a pane that replaces the
     * panes of its tab (a split removed): the focus request that would do it
     * can land before the old panes are closed, and closing them hands the
     * browser back to one of them.
     */
    void activateInSharedBrowser() {
        if (!browserDisposed) {
            sharedBrowser.activate(this);
        }
    }

    /**
     * Turn this pane into a live secondary view of {@code primary}'s Monaco
     * model instead of an independent document. Called by
     * {@code SciNotes.splitTab()} on the "other" half of a split: the shared
     * native browser can only ever show one Monaco document as its own, so
     * that half cannot get one the normal way (see the SharedBrowser class
     * comment). Any document this pane's own construction already created
     * (see SharedBrowser#paneConstructed) is discarded first.
     *
     * @param primary the split's other pane, which owns the shared document
     */
    void enterSplitView(MonacoEditorPane primary) {
        splitPrimary = primary;
        if (monacoDocumentCreated) {
            closeMonacoDocument();
        }
        sharedBrowser.activateSplit(primary, this);
    }

    /**
     * Undo {@link #enterSplitView(MonacoEditorPane)}. Called before this pane
     * is closed (see {@link #disposeBrowser()}) so the native browser goes
     * back to covering only {@link #splitPrimary} instead of the pair.
     */
    private void exitSplitView() {
        sharedBrowser.deactivateSplit(splitPrimary);
        splitPrimary = null;
    }

    private void handleMessage(String request) {
        if (request == null) {
            return;
        }

        if (request.startsWith("edit:")) {
            final String payload = request.substring("edit:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    applyEditFromMonaco(payload);
                }
            });
        } else if (request.startsWith("history:")) {
            final String payload = request.substring("history:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    relaySharedHistory(payload);
                }
            });
        } else if (request.startsWith("historyMismatch:")) {
            final String payload = request.substring("historyMismatch:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    repairSharedDocument(payload);
                }
            });
        } else if (request.startsWith("undoState:")) {
            final String payload = request.substring("undoState:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    updateUndoRedoStateFromMonaco(payload);
                }
            });
        } else if (request.startsWith("modified:")) {
            final boolean modified = Boolean.parseBoolean(request.substring("modified:".length()));
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    Document doc = getDocument();
                    if (doc instanceof ScilabDocument
                            && ((ScilabDocument) doc).isContentModified() != modified) {
                        ((ScilabDocument) doc).setContentModified(modified);
                        /*
                         * setContentModified() only refreshes the tab title
                         * when it turns the document clean; a tab becoming
                         * modified through this message alone -- Monaco owns
                         * that state -- would keep a title without its star.
                         */
                        updateTitle();
                    }
                }
            });
        } else if (request.startsWith("dapBreakpoints:")) {
            final String payload = request.substring("dapBreakpoints:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    persistBreakpointsFromMonaco(payload);
                }
            });
        } else if ("dapExecutionEnded:".equals(request)) {
            ScilabInterpreterManagement.releaseAbortedScilabExecs();
        } else if (request.startsWith("contextMenu:")) {
            final String payload = request.substring("contextMenu:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    showContextMenuFromMonaco(payload);
                }
            });
        } else if (request.startsWith("splitSize:")) {
            final String payload = request.substring("splitSize:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    resizeSplitFromMonaco(payload);
                }
            });
        } else if ("contextMenuClose:".equals(request)) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    hideActiveContextMenu();
                }
            });
        } else if (request.startsWith("copyHtmlWithLineNumbers:")) {
            final String payload = request.substring("copyHtmlWithLineNumbers:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    copyAsHtmlWithLineNumbers(payload);
                }
            });
        } else if (request.startsWith("openUrl:")) {
            openInUserBrowser(request.substring("openUrl:".length()));
        } else if (request.startsWith("openUrlAt:")) {
            final String payload = request.substring("openUrlAt:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    openUrlAt(payload);
                }
            });
        } else if (request.startsWith("dapStopped:")) {
            final String payload = request.substring("dapStopped:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    revealStoppedSource(payload);
                }
            });
        } else if (request.startsWith("openDefinition:")) {
            final String payload = request.substring("openDefinition:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    openDefinition(payload);
                }
            });
        } else if ("dropFiles:".equals(request)) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    openDroppedFiles();
                }
            });
        } else if (request.startsWith("shortcut:")) {
            final String bridge = request.substring("shortcut:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    dispatchShortcut(bridge);
                }
            });
        } else if ("focus:in".equals(request)) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    focusBrowserFromMonaco();
                }
            });
        } else if (request.startsWith("selection:")) {
            final String payload = request.substring("selection:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    updateSelectionFromMonaco(payload);
                }
            });
        } else if (request.startsWith("zoom:")) {
            final int step;
            try {
                step = Integer.parseInt(request.substring("zoom:".length()));
            } catch (NumberFormatException e) {
                return;
            }
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    SciNotes.updateFontSize(step);
                }
            });
        } else if (request.startsWith("infoBar:")) {
            final String text = request.substring("infoBar:".length());
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    SciNotes ed = getEditor();
                    if (ed != null && ed.getInfoBar() != null) {
                        ed.getInfoBar().setText(text);
                    }
                }
            });
        } else if (request.startsWith("syncMismatch:")) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    syncDocumentToMonaco();
                }
            });
        } else if ("monacoReady".equals(request)) {
            sharedBrowser.onMonacoReady();
        }
    }

    private void openDroppedFiles() {
        List<String> paths;
        synchronized (droppedFilesLock) {
            paths = draggedFilePaths;
            draggedFilePaths = new ArrayList<String>();
        }

        SciNotes ed = getEditor();
        if (ed == null) {
            return;
        }

        for (String path : paths) {
            if (path != null && !path.isEmpty()) {
                File file = toDroppedFile(path);
                if (file.exists()) {
                    ed.openFile(file.getPath(), 0, null);
                }
            }
        }
    }

    private static File toDroppedFile(String path) {
        String normalizedPath = path.trim();
        try {
            URI uri = new URI(normalizedPath);
            if ("file".equalsIgnoreCase(uri.getScheme())) {
                String authority = uri.getAuthority();
                if (authority != null && !authority.isEmpty() && File.separatorChar == '\\') {
                    return new File("\\\\" + authority + uri.getPath().replace('/', '\\'));
                }
                return new File(uri);
            }
        } catch (IllegalArgumentException e) {
            // Not a file URI: handle it as a native path below.
        } catch (URISyntaxException e) {
            // Not a file URI: handle it as a native path below.
        }

        // Chromium can expose a Windows native path as /C:/path/to/file.
        if (File.separatorChar == '\\' && normalizedPath.length() >= 3
                && normalizedPath.charAt(0) == '/'
                && Character.isLetter(normalizedPath.charAt(1))
                && normalizedPath.charAt(2) == ':') {
            normalizedPath = normalizedPath.substring(1);
        }
        return new File(normalizedPath);
    }

    private void focusBrowserFromMonaco() {
        if (!browserReady || browserDisposed) {
            return;
        }

        /*
         * Chromium only takes the keyboard while the browser component is the
         * Swing focus owner. A click in the page gives Monaco its caret without
         * making it so -- Java never sees that click under Linux -- and the
         * keys then go to the tab's placeholder: this is the page telling us.
         */
        cefPanel.reclaimSwingFocus();
        updateInfosWhenFocused();
    }

    private void updateUndoRedoStateFromMonaco(String payload) {
        String[] parts = payload.split(":", 2);
        if (parts.length != 2) {
            return;
        }

        updateUndoRedoButtons(Boolean.parseBoolean(parts[0]), Boolean.parseBoolean(parts[1]));
    }

    private void newFile() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new NewAction(SciNotesMessages.NEW, ed).doAction();
        }
    }

    private void openFileChooser() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new OpenAction(SciNotesMessages.OPEN, ed).doAction();
        }
    }

    private void saveCurrentTab() {
        SciNotes ed = getEditor();
        if (ed == null) {
            return;
        }

        if (!ed.save(ed.getTabPane().getSelectedIndex(), true)) {
            ScilabModalDialog.show(ed, SciNotesMessages.COULD_NOT_SAVE_FILE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }

    private void printCurrentTab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new PrintAction(SciNotesMessages.PRINT, ed).doAction();
        }
    }

    private void openSearchFiles() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SearchFilesAction("", ed).doAction();
        }
    }

    private void openSearchWordInFiles() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SearchWordInFilesAction("", ed).doAction();
        }
    }

    private void openSciNotesHelp() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new HelpAction("", ed).doAction();
        }
    }

    private void openHelpOnKeyword() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new HelpOnKeywordAction("", ed).doAction();
        }
    }

    private void openAboutDialog() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new AboutAction("", ed).doAction();
        }
    }

    private void openCodeNavigator() {
        SciNotes ed = getEditor();
        if (ed != null) {
            ed.addNavigator();
        }
    }

    private void setAnchor() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SetAnchorAction("", ed).doAction();
            updateAnchorDecorations();
        }
    }

    public void removeAnchor() {
        ScilabDocument doc = getScilabDocument();
        if (doc == null) {
            return;
        }

        Element root = doc.getDefaultRootElement();
        int lineIndex = root.getElementIndex(getCaretPosition());
        ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) root.getElement(lineIndex);
        line.setAnchor(null);
        repaintLineNumbers();
        updateNavigator();
        updateAnchorDecorations();
    }

    public void nextAnchor() {
        navigateAnchor(true);
    }

    public void previousAnchor() {
        navigateAnchor(false);
    }

    private void navigateAnchor(boolean forward) {
        ScilabDocument doc = getScilabDocument();
        if (doc == null) {
            return;
        }

        Element root = doc.getDefaultRootElement();
        int currentLine = root.getElementIndex(getCaretPosition());
        int targetLine = forward ? doc.nextAnchorFrom(currentLine) : doc.previousAnchorFrom(currentLine);
        if (targetLine < 0) {
            return;
        }

        int offset = root.getElement(targetLine).getStartOffset();
        setCaretPosition(offset);
        setMonacoPosition(offset);
    }

    private ScilabDocument getScilabDocument() {
        Document doc = getDocument();
        if (doc instanceof ScilabDocument) {
            return (ScilabDocument) doc;
        }

        return null;
    }

    private void repaintLineNumbers() {
        if (getXln() != null) {
            getXln().repaint();
        }
    }

    private void updateNavigator() {
        SciNotes ed = getEditor();
        if (ed != null && ed.getNavigator() != null) {
            ed.getNavigator().update();
        }
    }

    private void cloneTabInNewWindow() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new OpenTabInNewWindowAction("", ed).doAction();
        }
    }

    private void detachTabInNewWindow() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new CCloseTabInNewWindowAction("", ed).doAction();
        }
    }

    private void nextTab() {
        SciNotes ed = getEditor();
        if (ed != null && ed.getTabPane().getTabCount() > 0) {
            new NextTabAction("", ed).doAction();
        }
    }

    private void previousTab() {
        SciNotes ed = getEditor();
        if (ed != null && ed.getTabPane().getTabCount() > 0) {
            new PreviousTabAction("", ed).doAction();
        }
    }

    private void nextParagraph() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new NextParagraphAction("", ed).doAction();
            syncCurrentMonacoCaret(ed);
        }
    }

    private void previousParagraph() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new PreviousParagraphAction("", ed).doAction();
            syncCurrentMonacoCaret(ed);
        }
    }

    private void syncCurrentMonacoCaret(SciNotes ed) {
        ScilabEditorPane pane = ed.getTextPane();
        if (pane instanceof MonacoEditorPane) {
            MonacoEditorPane monacoPane = (MonacoEditorPane) pane;
            monacoPane.setMonacoPosition(monacoPane.getCaretPosition());
        }
    }

    private void dispatchShortcut(final String bridge) {
        // CEF calls its keyboard handler on its own thread: the actions touch
        // Swing and the SciNotes tabs.
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    dispatchShortcut(bridge);
                }
            });
            return;
        }

        if ("new".equals(bridge)) {
            newFile();
        } else if ("open".equals(bridge)) {
            openFileChooser();
        } else if ("save".equals(bridge)) {
            saveCurrentTab();
        } else if ("saveAs".equals(bridge)) {
            saveCurrentTabAs();
        } else if ("reload".equals(bridge)) {
            reloadCurrentTab();
        } else if ("printPreview".equals(bridge)) {
            printPreviewCurrentTab();
        } else if ("print".equals(bridge)) {
            printCurrentTab();
        } else if ("close".equals(bridge)) {
            closeCurrentTab();
        } else if ("exit".equals(bridge)) {
            exitSciNotes();
        } else if ("find".equals(bridge)) {
            openReplace();
        } else if ("toggleAutoCompleteOpeners".equals(bridge)) {
            HelpOnTypingManager manager = HelpOnTypingManager.getInstance();
            manager.enableOpeners(!manager.isOpenersActive());
            SciNotes.activateHelpOnTyping();
        } else if ("toggleAutoCompleteKeywords".equals(bridge)) {
            HelpOnTypingManager manager = HelpOnTypingManager.getInstance();
            manager.enableKeywords(!manager.isKeywordsActive());
            SciNotes.activateHelpOnTyping();
        } else if ("undo".equals(bridge)) {
            undo();
        } else if ("redo".equals(bridge)) {
            redo();
        } else if ("cut".equals(bridge)) {
            cut();
        } else if ("copy".equals(bridge)) {
            copy();
        } else if ("copyHtmlWithLineNumbers".equals(bridge)) {
            copyAsHtmlWithLineNumbers();
        } else if ("paste".equals(bridge)) {
            paste();
        } else if ("pasteColumn".equals(bridge)) {
            pasteColumn();
        } else if ("repeatSelectionOnColumn".equals(bridge)) {
            repeatSelectionOnColumn();
        } else if ("delete".equals(bridge)) {
            delete();
        } else if ("selectAll".equals(bridge)) {
            selectAll();
        } else if ("selectBlock".equals(bridge)) {
            selectBlock();
        } else if ("findNext".equals(bridge)) {
            findNext();
        } else if ("findPrevious".equals(bridge)) {
            findPrevious();
        } else if ("incrementalSearch".equals(bridge)) {
            findNext();
        } else if ("searchFiles".equals(bridge)) {
            openSearchFiles();
        } else if ("searchWordInFiles".equals(bridge)) {
            openSearchWordInFiles();
        } else if ("nextTab".equals(bridge)) {
            nextTab();
        } else if ("previousTab".equals(bridge)) {
            previousTab();
        } else if ("nextParagraph".equals(bridge)) {
            nextParagraph();
        } else if ("previousParagraph".equals(bridge)) {
            previousParagraph();
        } else if ("help".equals(bridge)) {
            openSciNotesHelp();
        } else if ("helpOnKeyword".equals(bridge)) {
            openHelpOnKeyword();
        } else if ("about".equals(bridge)) {
            openAboutDialog();
        } else if ("openUrl".equals(bridge)) {
            openUrl();
        } else if ("codeNavigator".equals(bridge)) {
            openCodeNavigator();
        } else if ("setAnchor".equals(bridge)) {
            setAnchor();
        } else if ("removeAnchor".equals(bridge)) {
            removeAnchor();
        } else if ("nextAnchor".equals(bridge)) {
            nextAnchor();
        } else if ("previousAnchor".equals(bridge)) {
            previousAnchor();
        } else if ("cloneTabInNewWindow".equals(bridge)) {
            cloneTabInNewWindow();
        } else if ("detachTabInNewWindow".equals(bridge)) {
            detachTabInNewWindow();
        } else if ("shiftRight".equals(bridge)) {
            shiftRight();
        } else if ("shiftLeft".equals(bridge)) {
            shiftLeft();
        } else if ("correctIndentation".equals(bridge)) {
            correctIndentation();
        } else if ("removeTrailingSpaces".equals(bridge)) {
            removeTrailingSpaces();
        } else if ("generateHelpComments".equals(bridge)) {
            generateHelpComments();
        } else if ("commentSelection".equals(bridge)) {
            commentSelection();
        } else if ("uncommentSelection".equals(bridge)) {
            uncommentSelection();
        } else if ("toLowerCase".equals(bridge)) {
            toLowerCase();
        } else if ("toUpperCase".equals(bridge)) {
            toUpperCase();
        } else if ("capitalize".equals(bridge)) {
            capitalize();
        } else if ("doubleQuoteStrings".equals(bridge)) {
            doubleQuoteStrings();
        } else if ("loadIntoScilab".equals(bridge)) {
            loadIntoScilab();
        } else if ("executeFileIntoScilab".equals(bridge)) {
            executeFileIntoScilab();
        } else if ("evaluateSelection".equals(bridge)) {
            evaluateSelection();
        } else if ("saveAndExecuteFile".equals(bridge)) {
            saveAndExecuteFile();
        } else if ("saveAndExecuteAllFiles".equals(bridge)) {
            saveAndExecuteAllFiles();
        } else if ("debugContinue".equals(bridge)) {
            debugOrContinue();
        } else if ("debugPause".equals(bridge)) {
            runDebuggerCommand("pause");
        } else if ("debugStepOver".equals(bridge)) {
            runDebuggerCommand("next");
        } else if ("debugStepIn".equals(bridge)) {
            runDebuggerCommand("stepIn");
        } else if ("debugStepOut".equals(bridge)) {
            runDebuggerCommand("stepOut");
        } else if ("debugDisconnect".equals(bridge)) {
            runDebuggerCommand("disconnect");
        } else if ("debugToggleBreakpoint".equals(bridge)) {
            toggleBreakpointAtCaret();
        } else if ("debugRunToCursor".equals(bridge)) {
            runToCursor();
        } else if ("openDevTools".equals(bridge)) {
            openDevTools();
        }
    }

    private void installWindowShortcuts() {
        JComponent rootPane = SwingUtilities.getRootPane(getEditor());
        if (rootPane == null) {
            return;
        }

        for (final Map.Entry<String, String> entry : MONACO_BRIDGED_ACTIONS.entrySet()) {
            final String monacoCommand = getMonacoCommandForBridge(entry.getValue());
            installWindowShortcut(rootPane, entry.getKey(), "monaco-shortcut-" + entry.getValue(), new Runnable() {
                public void run() {
                    if (monacoCommand != null) {
                        runMonacoCommand(monacoCommand);
                    } else {
                        dispatchShortcut(entry.getValue());
                    }
                }
            });
        }

        for (int i = 0; i < MONACO_FIXED_BRIDGED_SHORTCUTS.length; i++) {
            final String[] shortcut = MONACO_FIXED_BRIDGED_SHORTCUTS[i];
            KeyStroke keyStroke = getKeyStrokeFromShortcut(shortcut);
            if (keyStroke != null) {
                installWindowShortcut(rootPane, keyStroke, "monaco-shortcut-" + shortcut[4], new Runnable() {
                    public void run() {
                        dispatchShortcut(shortcut[4]);
                    }
                });
            }
        }

        for (int i = 0; i < MONACO_ONLY_SHORTCUTS.length; i++) {
            final String[] shortcut = MONACO_ONLY_SHORTCUTS[i];
            KeyStroke keyStroke = getKeyStrokeFromShortcut(shortcut);
            if (keyStroke != null) {
                installWindowShortcut(rootPane, keyStroke, "monaco-command-" + shortcut[4], new Runnable() {
                    public void run() {
                        runMonacoCommand(shortcut[4]);
                    }
                });
            }
        }
    }

    private void installWindowShortcut(JComponent rootPane, String sciNotesAction, String actionKey, final Runnable action) {
        KeyStroke keyStroke = getKeyStrokeForAction(sciNotesAction);
        if (keyStroke == null) {
            return;
        }

        installWindowShortcut(rootPane, keyStroke, actionKey, action);
    }

    private void installWindowShortcut(JComponent rootPane, KeyStroke keyStroke, String actionKey, final Runnable action) {
        rootPane.getInputMap(JComponent.WHEN_IN_FOCUSED_WINDOW).put(keyStroke, actionKey);
        rootPane.getActionMap().put(actionKey, new AbstractAction() {
            public void actionPerformed(ActionEvent e) {
                action.run();
            }
        });
    }

    private boolean handleBrowserShortcut(CefKeyEvent event) {
        for (Map.Entry<String, String> entry : MONACO_BRIDGED_ACTIONS.entrySet()) {
            KeyStroke keyStroke = getKeyStrokeForAction(entry.getKey());
            if (matchesBrowserKeyStroke(event, keyStroke)) {
                if (isBrowserPassthroughBridge(entry.getValue()) || getMonacoCommandForBridge(entry.getValue()) != null) {
                    return false;
                }
                dispatchShortcut(entry.getValue());
                return true;
            }
        }

        for (int i = 0; i < MONACO_FIXED_BRIDGED_SHORTCUTS.length; i++) {
            String[] shortcut = MONACO_FIXED_BRIDGED_SHORTCUTS[i];
            if (matchesBrowserKeyStroke(event, getKeyStrokeFromShortcut(shortcut))) {
                dispatchShortcut(shortcut[4]);
                return true;
            }
        }

        for (int i = 0; i < MONACO_ONLY_SHORTCUTS.length; i++) {
            String[] shortcut = MONACO_ONLY_SHORTCUTS[i];
            if (matchesBrowserKeyStroke(event, getKeyStrokeFromShortcut(shortcut))) {
                runMonacoCommand(shortcut[4]);
                return true;
            }
        }

        return false;
    }

    private boolean matchesBrowserKeyStroke(CefKeyEvent event, KeyStroke keyStroke) {
        if (keyStroke == null || !matchesBrowserKeyCode(event, keyStroke.getKeyCode())) {
            return false;
        }

        int modifiers = keyStroke.getModifiers();
        boolean expectedCtrl = hasShortcutModifier(modifiers, InputEvent.CTRL_DOWN_MASK, LEGACY_CTRL_MASK)
                || hasShortcutModifier(modifiers, InputEvent.META_DOWN_MASK, LEGACY_META_MASK);
        boolean expectedShift = hasShortcutModifier(modifiers, InputEvent.SHIFT_DOWN_MASK, LEGACY_SHIFT_MASK);
        boolean expectedAlt = hasShortcutModifier(modifiers, InputEvent.ALT_DOWN_MASK, LEGACY_ALT_MASK);

        boolean actualCtrl = hasEventFlag(event, EventFlags.EVENTFLAG_CONTROL_DOWN)
                || hasEventFlag(event, EventFlags.EVENTFLAG_COMMAND_DOWN);
        boolean actualShift = hasEventFlag(event, EventFlags.EVENTFLAG_SHIFT_DOWN);
        boolean actualAlt = hasEventFlag(event, EventFlags.EVENTFLAG_ALT_DOWN);

        return expectedCtrl == actualCtrl
                && expectedShift == actualShift
                && expectedAlt == actualAlt;
    }

    private boolean matchesBrowserKeyCode(CefKeyEvent event, int keyCode) {
        if ((keyCode >= KeyEvent.VK_A && keyCode <= KeyEvent.VK_Z)
                || (keyCode >= KeyEvent.VK_0 && keyCode <= KeyEvent.VK_9)) {
            char expected = Character.toLowerCase((char) keyCode);
            char actual = Character.toLowerCase(event.unmodified_character);
            if (actual == expected) {
                return true;
            }
        }

        return event.windows_key_code == keyCode;
    }

    private static boolean hasEventFlag(CefKeyEvent event, int flag) {
        return (event.modifiers & flag) == flag;
    }

    private void saveCurrentTabAs() {
        SciNotes ed = getEditor();
        if (ed == null) {
            return;
        }

        if (!ed.saveAs(getName())) {
            ScilabModalDialog.show(ed, SciNotesMessages.COULD_NOT_SAVE_FILE, SciNotesMessages.SCINOTES_ERROR, IconType.ERROR_ICON);
        }
    }

    private void reloadCurrentTab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new ReloadAction("", ed).doAction();
        }
    }

    private void printPreviewCurrentTab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new PrintPreviewAction("", ed).doAction();
        }
    }

    private void closeCurrentTab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new CloseAction("", ed).doAction();
        }
    }

    private void exitSciNotes() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new ExitAction("", ed).doAction();
        }
    }

    /**
     * Keep the hidden Swing text component aligned with Monaco's caret and
     * selection while legacy actions still query ScilabEditorPane state.
     * Payload framing: "&lt;position&gt;:&lt;selectionStart&gt;:&lt;selectionEnd&gt;".
     */
    private void updateSelectionFromMonaco(String payload) {
        if (getCaret() == null || getDocument() == null) {
            return;
        }

        String[] parts = payload.split(":", 3);
        if (parts.length != 3) {
            return;
        }

        try {
            int position = Integer.parseInt(parts[0]);
            int selectionStart = Integer.parseInt(parts[1]);
            int selectionEnd = Integer.parseInt(parts[2]);
            int length = getDocument().getLength();
            position = clamp(position, 0, length);
            selectionStart = clamp(selectionStart, 0, length);
            selectionEnd = clamp(selectionEnd, 0, length);

            if (selectionStart == selectionEnd) {
                setCaretPosition(position);
            } else {
                setSelectionStart(Math.min(selectionStart, selectionEnd));
                setSelectionEnd(Math.max(selectionStart, selectionEnd));
            }
        } catch (NumberFormatException e) {
            // Ignore malformed bridge messages.
        }
    }

    private static int clamp(int value, int min, int max) {
        return Math.max(min, Math.min(max, value));
    }

    private static String trimBlankEdgeLines(String text) {
        if (text == null || text.isEmpty()) {
            return "";
        }

        String selection = text.replaceFirst("\\A(?:[ \\t]*(?:\\r\\n|\\n|\\r))+", "");
        return selection.replaceFirst("(?:(?:\\r\\n|\\n|\\r)[ \\t]*)+\\z", "");
    }

    /**
     * Show the regular SciNotes editor popup menu from a Monaco right-click.
     * Payload framing: "&lt;offset&gt;:&lt;selectionStart&gt;:&lt;selectionEnd&gt;:&lt;x&gt;:&lt;y&gt;"
     * where x/y are browser component coordinates.
     */
    private void showContextMenuFromMonaco(String payload) {
        String[] parts = payload.split(":");
        if ((parts.length != 3 && parts.length != 5) || getDocument() == null) {
            return;
        }

        try {
            int offset = clamp(Integer.parseInt(parts[0]), 0, getDocument().getLength());
            int selectionStart = offset;
            int selectionEnd = offset;
            int xIndex = 1;
            int yIndex = 2;
            if (parts.length == 5) {
                selectionStart = clamp(Integer.parseInt(parts[1]), 0, getDocument().getLength());
                selectionEnd = clamp(Integer.parseInt(parts[2]), 0, getDocument().getLength());
                xIndex = 3;
                yIndex = 4;
            }
            int x = Math.max(0, Integer.parseInt(parts[xIndex]));
            int y = Math.max(0, Integer.parseInt(parts[yIndex]));

            lastContextMenuPoint = new Point(x, y);
            if (selectionStart == selectionEnd) {
                setCaretPosition(offset);
            } else {
                setSelectionStart(Math.min(selectionStart, selectionEnd));
                setSelectionEnd(Math.max(selectionStart, selectionEnd));
            }
            updateInfosWhenFocused();

            SciNotes editor = getEditor();
            if (editor == null) {
                return;
            }

            JPopupMenu popup = SciNotesGUI.generateRightClickPopup(editor);
            if (popup == null) {
                return;
            }

            hideActiveContextMenu();
            activeContextMenu = popup;
            popup.setLightWeightPopupEnabled(false);
            popup.addPopupMenuListener(new PopupMenuListener() {
                public void popupMenuWillBecomeVisible(PopupMenuEvent e) { }

                public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
                    clearActiveContextMenu(popup);
                }

                public void popupMenuCanceled(PopupMenuEvent e) {
                    clearActiveContextMenu(popup);
                }
            });
            installContextMenuDismissListener();
            popup.show(browser.getUIComponent(), x, y);
        } catch (NumberFormatException e) {
            // Ignore malformed bridge messages.
        }
    }

    /**
     * Move the divider of this pane's split so the primary half gets the
     * size the user dragged the separator to in the page (see
     * configureSplitHandle() in monaco-split.js). Applied as a delta on the
     * current divider location, so the split pane's insets and the chrome
     * around the browser placeholder do not have to be known.
     *
     * @param payload new width (split by column) or height (split by row) of
     *        the primary half, in overlay pixels
     */
    private void resizeSplitFromMonaco(String payload) {
        MonacoEditorPane primary = splitPrimaryPane();
        JSplitPane split = primary == null ? null : primary.getSplitPane();
        if (split == null) {
            return;
        }

        try {
            int size = Integer.parseInt(payload);
            boolean byColumn = split.getOrientation() == JSplitPane.HORIZONTAL_SPLIT;
            int current = byColumn ? primary.browserHost.getWidth() : primary.browserHost.getHeight();
            if (size != current) {
                split.setDividerLocation(split.getDividerLocation() + size - current);
            }
        } catch (NumberFormatException e) {
            // Ignore malformed bridge messages.
        }
    }

    private void hideActiveContextMenu() {
        if (activeContextMenu != null) {
            activeContextMenu.setVisible(false);
            clearActiveContextMenu(activeContextMenu);
        }
    }

    private void clearActiveContextMenu(JPopupMenu popup) {
        if (activeContextMenu == popup) {
            activeContextMenu = null;
            uninstallContextMenuDismissListener();
        }
    }

    private void installContextMenuDismissListener() {
        if (contextMenuDismissListener != null) {
            return;
        }

        contextMenuDismissListener = new AWTEventListener() {
            public void eventDispatched(AWTEvent event) {
                if (activeContextMenu == null || !activeContextMenu.isVisible()) {
                    return;
                }

                if (event instanceof MouseEvent) {
                    MouseEvent mouseEvent = (MouseEvent) event;
                    if (mouseEvent.getID() != MouseEvent.MOUSE_PRESSED) {
                        return;
                    }

                    Object source = mouseEvent.getSource();
                    if (source instanceof Component && SwingUtilities.isDescendingFrom((Component) source, activeContextMenu)) {
                        return;
                    }

                    hideActiveContextMenu();
                } else if (event instanceof WindowEvent) {
                    WindowEvent windowEvent = (WindowEvent) event;
                    if (windowEvent.getID() == WindowEvent.WINDOW_LOST_FOCUS || windowEvent.getID() == WindowEvent.WINDOW_DEACTIVATED) {
                        hideActiveContextMenu();
                    }
                }
            }
        };

        Toolkit.getDefaultToolkit().addAWTEventListener(
            contextMenuDismissListener,
            AWTEvent.MOUSE_EVENT_MASK | AWTEvent.WINDOW_EVENT_MASK);
    }

    private void uninstallContextMenuDismissListener() {
        if (contextMenuDismissListener != null) {
            Toolkit.getDefaultToolkit().removeAWTEventListener(contextMenuDismissListener);
            contextMenuDismissListener = null;
        }
    }

    /**
     * Apply an incremental edit reported by Monaco onto the ScilabDocument
     * read-replica. Payload framing:
     * "&lt;expectedRevision&gt;:&lt;newRevision&gt;:&lt;offset&gt;:&lt;length&gt;:&lt;text&gt;"
     * where text is the unparsed remainder (may itself contain ':' or newlines).
     */
    private void applyEditFromMonaco(String payload) {
        int firstColon = payload.indexOf(':');
        int secondColon = payload.indexOf(':', firstColon + 1);
        int thirdColon = payload.indexOf(':', secondColon + 1);
        int fourthColon = payload.indexOf(':', thirdColon + 1);
        if (firstColon < 0 || secondColon < 0 || thirdColon < 0 || fourthColon < 0) {
            return;
        }

        int expectedRevision;
        int newRevision;
        int offset;
        int length;
        try {
            expectedRevision = Integer.parseInt(payload.substring(0, firstColon));
            newRevision = Integer.parseInt(payload.substring(firstColon + 1, secondColon));
            offset = Integer.parseInt(payload.substring(secondColon + 1, thirdColon));
            length = Integer.parseInt(payload.substring(thirdColon + 1, fourthColon));
        } catch (NumberFormatException e) {
            return;
        }
        String text = payload.substring(fourthColon + 1);

        Document doc = getDocument();
        if (doc == null) {
            return;
        }

        if (expectedRevision != documentRevision) {
            syncDocumentToMonaco();
            return;
        }

        boolean resync = false;
        updatingFromMonaco = true;
        try {
            if (length > 0) {
                doc.remove(offset, length);
            }
            if (!text.isEmpty()) {
                doc.insertString(offset, text, null);
            }
            if (doc instanceof ScilabDocument) {
                ((ScilabDocument) doc).setContentModified(true);
            }
        } catch (BadLocationException | RuntimeException e) {
            /*
             * A listener of the Swing replica can fail on content Monaco has
             * already accepted. Staying one revision behind would make every
             * later edit mismatch and resync, and each resync resets the
             * caret; take the revision anyway and push the whole document.
             */
            System.err.println(e);
            resync = true;
        } finally {
            documentRevision = newRevision;
            updatingFromMonaco = false;
        }
        updateAnchorDecorations();
        if (resync) {
            syncDocumentToMonaco();
        }
    }

    /** Forward a document insertion to Monaco as an incremental edit. */
    private void onDocumentInsert(int offset, int length) {
        // Skip our own mirror writes, the initial file load (pushed wholesale
        // once the browser is ready), and the secondary half of a split: it
        // shares its primary's document, which mirrors it already.
        if (updatingFromMonaco || !browserReady || splitPrimary != null) {
            return;
        }
        applyEditToMonaco(offset, 0, getDocumentText(offset, length));
        updateAnchorDecorations();
    }

    /** Forward a document removal to Monaco as an incremental edit. */
    private void onDocumentRemove(int offset, int length) {
        if (updatingFromMonaco || !browserReady || splitPrimary != null) {
            return;
        }
        applyEditToMonaco(offset, length, "");
        updateAnchorDecorations();
    }

    private void applyEditToMonaco(int offset, int length, String text) {
        int expectedRevision = documentRevision;
        int newRevision = expectedRevision + 1;
        documentRevision = newRevision;
        browser.executeJavaScript("window.SciNotesMonacoApplyDocumentEdit("
            + quoteJs(getMonacoDocumentId()) + ", " + expectedRevision + ", "
            + newRevision + ", " + offset + ", " + length + ", "
            + quoteJs(text) + ");", browser.getURL(), 1);
    }

    /**
     * Start a group of document edits that Monaco undoes in one step (an
     * edit of the assistant: removal plus insertion, or several
     * replacements). Must be paired with {@link #endEditGroup()}; the edits
     * are forwarded in order, so the group brackets them in Monaco too.
     */
    public void beginEditGroup() {
        if (splitPrimary != null) {
            // A split half shares its primary's document, which forwards the edits
            splitPrimary.beginEditGroup();
        } else if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoBeginEditGroup("
                + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
        }
    }

    /** End a group started by {@link #beginEditGroup()}. */
    public void endEditGroup() {
        if (splitPrimary != null) {
            splitPrimary.endEditGroup();
        } else if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoEndEditGroup("
                + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
        }
    }

    /** Route undo to Monaco, which owns the live text and the undo stack. */
    public void undo() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoUndo();", browser.getURL(), 1);
        }
    }

    /** Route redo to Monaco, which owns the live text and the undo stack. */
    public void redo() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoRedo();", browser.getURL(), 1);
        }
    }

    /** Route cut to Monaco's clipboard action. */
    public void cut() {
        runMonacoCommand("editor.action.clipboardCutAction");
    }

    /** Route copy to Monaco's clipboard action. */
    public void copy() {
        runMonacoCommand("editor.action.clipboardCopyAction");
    }

    /** Use the existing SciNotes HTML exporter, including line numbers. */
    public void copyAsHtmlWithLineNumbers() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoCopyAsHtmlWithLineNumbers();", browser.getURL(), 1);
        }
    }

    /** Use the existing SciNotes block scanner, then mirror the selection in Monaco. */
    public void selectBlock() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SelectBlockAction("", ed).doAction();
        }
    }

    public void finishSelectBlock(int previousStart, int previousEnd) {
        if (getSelectionStart() == previousStart && getSelectionEnd() == previousEnd) {
            selectEnclosingBlock();
        }
        setMonacoSelection(getSelectionStart(), getSelectionEnd());
    }

    private void selectEnclosingBlock() {
        Document doc = getDocument();
        if (doc == null) {
            return;
        }

        int caret = clamp(getCaretPosition(), 0, doc.getLength());
        MatchingBlockScanner scanner = getMatchingBlockManager(true).getScanner();
        int previousKeywordStart = -1;
        for (int pos = caret; pos >= 0; --pos) {
            KeywordEvent event = super.getKeywordEvent(pos);
            if (event.getStart() == previousKeywordStart) {
                continue;
            }
            previousKeywordStart = event.getStart();

            if (!ScilabLexerConstants.isMatchable(event.getType())) {
                continue;
            }

            MatchingBlockScanner.MatchingPositions match = scanner.getMatchingBlock(event.getStart(), true);
            if (match != null && match.firstB <= caret && caret <= match.secondE) {
                setSelectionStart(match.firstB);
                setSelectionEnd(match.secondE);
                return;
            }
        }
    }

    /** Use the existing SciNotes HTML exporter on Monaco's live selection. */
    private void copyAsHtmlWithLineNumbers(String selection) {
        if (selection == null || selection.isEmpty()) {
            return;
        }

        CopyAsHTMLAction.HTMLSelection transferable = new CopyAsHTMLAction.HTMLSelection(this, selection, true);
        Toolkit.getDefaultToolkit().getSystemClipboard().setContents(transferable, transferable);
    }

    /** Paste through Java's clipboard API, then insert into Monaco. */
    public void paste() {
        if (!browserReady || !isEditable()) {
            return;
        }

        try {
            Transferable contents = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this);
            if (contents == null || !contents.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                return;
            }

            Object data = contents.getTransferData(DataFlavor.stringFlavor);
            if (!(data instanceof String)) {
                return;
            }

            String text = (String) data;
            if (text.isEmpty()) {
                return;
            }

            text = ((ScilabEditorKit) getEditorKit()).read(new StringReader(text)).content;
            browser.executeJavaScript("window.SciNotesMonacoReplaceSelection(" + quoteJs(text) + ");", browser.getURL(), 1);
        } catch (UnsupportedFlavorException e) {
        } catch (IOException e) {
            System.err.println(e);
        }
    }

    /** Paste clipboard lines as a column at the Monaco caret. */
    public void pasteColumn() {
        if (!browserReady || !isEditable()) {
            return;
        }

        String text = getClipboardText();
        if (text == null || text.isEmpty()) {
            return;
        }

        browser.executeJavaScript("window.SciNotesMonacoPasteColumn(" + quoteJs(text) + ");", browser.getURL(), 1);
    }

    /** Repeat the current Monaco selection at the same column on the next line. */
    public void repeatSelectionOnColumn() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoRepeatSelectionOnColumn();", browser.getURL(), 1);
        }
    }

    /** Route delete to Monaco's editor command. */
    public void delete() {
        runMonacoCommand("deleteRight");
    }

    /** Route select-all to Monaco. */
    public void selectAll() {
        runMonacoCommand("editor.action.selectAll");
    }

    /** Open Monaco's find widget. */
    public void openFind() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoOpenFind();", browser.getURL(), 1);
        }
    }

    /** Open Monaco's find/replace widget. */
    public void openReplace() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoOpenReplace();", browser.getURL(), 1);
        }
    }

    /** Move to the next Monaco find match. */
    public void findNext() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoFindNext();", browser.getURL(), 1);
        }
    }

    /** Move to the previous Monaco find match. */
    public void findPrevious() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoFindPrevious();", browser.getURL(), 1);
        }
    }

    /** Indent the current Monaco selections. */
    public void shiftRight() {
        runMonacoCommand("editor.action.indentLines");
    }

    /** Outdent the current Monaco selections. */
    public void shiftLeft() {
        runMonacoCommand("editor.action.outdentLines");
    }

    /**
     * Correct the indentation of the selected lines, or of the whole document
     * when nothing is selected. The LSP refuses a document with syntax errors
     * (see infoBar:).
     */
    public void correctIndentation() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoCorrectIndentation();", browser.getURL(), 1);
        }
    }

    /** Remove trailing spaces from the whole Monaco model. */
    public void removeTrailingSpaces() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoRemoveTrailingSpaces();", browser.getURL(), 1);
        }
    }

    /** Use the Java SciNotes function scanner to insert the help_from_sci template. */
    public void generateHelpComments() {
        if (!isEditable()) {
            return;
        }

        ScilabDocument doc = getScilabDocument();
        if (doc == null) {
            return;
        }

        Element root = doc.getDefaultRootElement();
        Element element = root.getElement(root.getElementIndex(getCaretPosition()));
        if (!(element instanceof ScilabDocument.ScilabLeafElement)) {
            return;
        }

        ScilabDocument.ScilabLeafElement leaf = (ScilabDocument.ScilabLeafElement) element;
        if (!leaf.isFunction()) {
            return;
        }

        FunctionScanner.FunctionInfo info = leaf.getFunctionInfo();
        String help = GenerateHelpFromFunctionAction.TEMPLATE.replaceFirst("functionName", info.functionName);
        help = help.replaceFirst("returnValues", generateHelpList(info.returnValues, ", ", ""));
        help = help.replaceFirst("argsValues", generateHelpList(info.argsValues, ", ", ""));
        help = help.replaceFirst("returnValuesOnColumn", generateHelpList(info.returnValues, ": \n// ", ": "));
        help = help.replaceFirst("argsValuesOnColumn", generateHelpList(info.argsValues, ": \n// ", ": "));

        try {
            doc.insertString(element.getEndOffset(), help, null);
        } catch (BadLocationException e) {
            // Ignore stale function offsets.
        }
    }

    private String generateHelpList(java.util.List<String> list, String sep, String end) {
        String str = "";
        for (int i = 0; i < list.size() - 1; i++) {
            str += list.get(i) + sep;
        }
        if (list.size() >= 1) {
            str += list.get(list.size() - 1) + end;
        }

        return str;
    }

    /** Comment the current Monaco selections with Scilab line comments. */
    public void commentSelection() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoCommentSelection();", browser.getURL(), 1);
        }
    }

    /** Uncomment the current Monaco selections. */
    public void uncommentSelection() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoUncommentSelection();", browser.getURL(), 1);
        }
    }

    /** Lowercase the current Monaco selections. */
    public void toLowerCase() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoTransformSelection(\"lower\");", browser.getURL(), 1);
        }
    }

    /** Uppercase the current Monaco selections. */
    public void toUpperCase() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoTransformSelection(\"upper\");", browser.getURL(), 1);
        }
    }

    /** Capitalize the current Monaco selections, or toggle the current character. */
    public void capitalize() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoTransformSelection(\"capitalize\");", browser.getURL(), 1);
        }
    }

    /** Convert single-quoted Scilab strings to double-quoted strings. */
    public void doubleQuoteStrings() {
        if (browserReady && isEditable()) {
            browser.executeJavaScript("window.SciNotesMonacoDoubleQuoteStrings();", browser.getURL(), 1);
        }
    }

    /** Execute the whole current file with echo through the existing SciNotes action. */
    public void loadIntoScilab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new LoadIntoScilabAction("", ed).doAction();
        }
    }

    /** Execute the saved current file without echo through the existing SciNotes action. */
    public void executeFileIntoScilab() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new ExecuteFileIntoScilabAction("", ed).doAction();
        }
    }

    /** Execute Monaco's mirrored selection, or code up to the caret when no selection exists. */
    public void evaluateSelection() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new EvaluateSelectionAction("", ed).doAction();
        }
    }

    /** Save and execute the current file through the existing SciNotes action. */
    public void saveAndExecuteFile() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SaveAndExecuteFileAction("", ed).doAction();
        }
    }

    /** Continue the stopped execution, or save the current file and debug it. */
    public void debugOrContinue() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new DebugContinueAction("", ed).doAction();
        }
    }

    /** Save and execute all opened files through the existing SciNotes action. */
    public void saveAndExecuteAllFiles() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new SaveAndExecuteAllFilesAction("", ed).doAction();
        }
    }

    /** Open the URL/mail token under Monaco's mirrored caret with SciNotes' browser integration. */
    public void openUrl() {
        SciNotes ed = getEditor();
        if (ed != null) {
            new OpenURLAction("", ed).doAction();
        }
    }

    private static void openInUserBrowser(final String url) {
        if (url == null || url.isEmpty()) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                WebBrowser.openUrl(url);
            }
        });
    }

    private void openUrlAt(String payload) {
        if (getDocument() == null) {
            return;
        }

        try {
            int offset = clamp(Integer.parseInt(payload), 0, getDocument().getLength());
            setCaretPosition(offset);
            openUrl();
        } catch (NumberFormatException e) {
            // Ignore malformed bridge messages.
        }
    }

    /**
     * Open or activate the source the debugger stopped in. The browser owns
     * the stopped-line decoration; Java only has to make the right tab the
     * visible one, exactly as it does for a definition jump.
     */
    private void revealStoppedSource(String payload) {
        if (payload == null) {
            return;
        }

        int separator = payload.lastIndexOf(':');
        if (separator <= 0) {
            return;
        }

        try {
            String path = URLDecoder.decode(payload.substring(0, separator), "UTF-8");
            int line = Math.max(1, Integer.parseInt(payload.substring(separator + 1)));
            File source = new File(path);
            if (!source.isFile()) {
                return;
            }

            SciNotes ed = getEditor();
            if (ed != null) {
                // The stop happens while the console may hold the focus: the
                // step keys must reach the editor.
                Component owner = KeyboardFocusManager.getCurrentKeyboardFocusManager().getFocusOwner();
                if (owner == null || !SwingUtilities.isDescendingFrom(owner, ed)) {
                    ScilabGUIUtilities.toFront(ed, SciNotesMessages.SCILAB_EDITOR);
                }
                ed.openFile(source.getPath(), line, null);
            }
        } catch (NumberFormatException e) {
            // Ignore malformed bridge messages.
        } catch (UnsupportedEncodingException e) {
            // Ignore malformed bridge messages.
        }
    }

    /**
     * Send one validated execution command to the DAP client in the shared browser.
     * @param command DAP command name
     */
    public void runDapCommand(String command) {
        if (!browserDisposed && sharedBrowser.monacoReady && command != null) {
            browser.executeJavaScript("window.SciNotesMonacoDebugCommand("
                + quoteJs(command) + ");", browser.getURL(), 1);
        }
    }

    /** Add or remove a breakpoint on the line of the caret, as a click in the margin does. */
    public void toggleBreakpointAtCaret() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoToggleBreakpointAtCaret();", browser.getURL(), 1);
        }
    }

    /**
     * Run up to the line of the caret: continue a stopped execution, or save
     * the file and run it with the debugger when nothing runs. The page stops
     * there as on a breakpoint it sets for that one run. Nothing during a
     * run, as for the steps.
     */
    public void runToCursor() {
        if (browserDisposed || !browserReady || !sharedBrowser.monacoReady) {
            return;
        }

        if (sharedBrowser.dapCanContinue) {
            browser.executeJavaScript("window.SciNotesMonacoRunToCursor(true);", browser.getURL(), 1);
        } else if (sharedBrowser.dapCanDebug) {
            browser.executeJavaScript("window.SciNotesMonacoRunToCursor(false);", browser.getURL(), 1);
            SciNotes ed = getEditor();
            if (ed != null) {
                new SaveAndExecuteFileAction("", ed, true).doAction();
            }
        }
    }

    /**
     * Run a command of the debugger: pause, stop or a step. Outside debug mode
     * there is nothing for it to act on, and the status bar says so.
     * @param command DAP command name
     */
    public void runDebuggerCommand(String command) {
        if (!sharedBrowser.dapDebugMode) {
            sharedBrowser.showDebuggerState(SciNotesMessages.DEBUG_NOT_IN_DEBUG_MODE);
            return;
        }
        runDapCommand(command);
    }

    /**
     * Run one file through the debug session rather than handing it to the
     * interpreter from Java: the session sees the run begin, debugged or not.
     * @param path file to run
     * @param echo whether the file is executed with echo
     * @param debug whether the file is run in debug mode
     * @return true when the debug session took the run
     */
    public boolean executeFileThroughDap(String path, boolean echo, boolean debug) {
        boolean free = debug ? sharedBrowser.dapCanDebug : sharedBrowser.dapCanExecute;
        if (browserDisposed || !sharedBrowser.monacoReady || !sharedBrowser.dapSessionReady
                || !free || path == null || path.isEmpty()) {
            return false;
        }

        browser.executeJavaScript("window.SciNotesMonacoExecuteFile("
            + quoteJs(path) + ", " + echo + ", " + debug + ");", browser.getURL(), 1);
        return true;
    }

    /**
     * Continue the current execution when the debugger is stopped.
     * @return true when Continue was requested
     */
    /**
     * Whether the debug session holds the interpreter: a run in progress, or
     * stopped on a breakpoint. Another run handed to Scilab meanwhile would
     * wait behind the stopped one, or nest into it.
     * @return true while the session is running or stopped
     */
    public boolean isDebugSessionBusy() {
        String state = sharedBrowser.dapSessionState;
        return !browserDisposed && ("running".equals(state) || "stopped".equals(state));
    }

    public boolean continueDapExecution() {
        if (browserDisposed || !sharedBrowser.dapCanContinue) {
            return false;
        }
        runDapCommand("continue");
        return true;
    }

    private void openDefinition(String payload) {
        if (payload == null) {
            return;
        }

        String[] parts = payload.split(":", 3);
        if (parts.length < 3) {
            return;
        }

        try {
            URI uri = new URI(URLDecoder.decode(parts[0], "UTF-8"));
            if (!"file".equalsIgnoreCase(uri.getScheme())) {
                return;
            }

            int lineNumber = Math.max(1, Integer.parseInt(parts[1]));
            SciNotes ed = getEditor();
            if (ed != null) {
                ed.openFile(new File(uri).getPath(), lineNumber, null);
                ScilabEditorPane destination = ed.getTextPane();
                if (destination instanceof MonacoEditorPane && destination != this) {
                    ((MonacoEditorPane) destination).definitionOrigin = this;
                }
            }
        } catch (IllegalArgumentException e) {
            // Ignore malformed bridge messages.
        } catch (URISyntaxException e) {
            // Ignore malformed bridge messages.
        } catch (UnsupportedEncodingException e) {
            // Ignore malformed bridge messages.
        }
    }

    private void runMonacoCommand(String commandId) {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoRunCommand(" + quoteJs(commandId) + ");", browser.getURL(), 1);
        }
    }

    private void openDevTools() {
        if (!browserDisposed) {
            browser.openDevTools();
        }
    }

    private String getClipboardText() {
        try {
            Transferable contents = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(this);
            if (contents == null || !contents.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                return null;
            }

            Object data = contents.getTransferData(DataFlavor.stringFlavor);
            if (!(data instanceof String)) {
                return null;
            }

            return ((ScilabEditorKit) getEditorKit()).read(new StringReader((String) data)).content;
        } catch (UnsupportedFlavorException e) {
            return null;
        } catch (IOException e) {
            System.err.println(e);
            return null;
        }
    }

    private void setMonacoPosition(int offset) {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoSetPositionAtOffset(" + offset + ");", browser.getURL(), 1);
        }
    }

    private void setMonacoSelection(int selectionStart, int selectionEnd) {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoSetSelectionAtOffsets(" + selectionStart + ", " + selectionEnd + ");", browser.getURL(), 1);
        }
    }

    /** Restore the selection copied into the Java replica during a tab transfer. */
    public void restoreTransferredSelection() {
        setMonacoSelection(getSelectionStart(), getSelectionEnd());
    }

    /** Remove both DOM and native focus before transferring this pane. */
    public void releaseFocusForTransfer() {
        boolean willRelease = browserReady && !browserDisposed && sharedBrowser.getActivePane() == this;
        focusRequested = false;
        if (willRelease) {
            browser.executeJavaScript("window.SciNotesMonacoBlur();", browser.getURL(), 1);
            cefPanel.releaseBrowserFocus();
        }
    }

    private void updateUndoRedoState() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoNotifyUndoState();", browser.getURL(), 1);
        }
    }

    /**
     * Push the whole document into Monaco. Used only for the initial load and
     * file reload; live edits go through the incremental path.
     */
    private void syncDocumentToMonaco() {
        if (updatingFromMonaco) {
            return;
        }

        pendingText = getDocumentText();
        if (!browserReady) {
            return;
        }

        documentRevision++;
        boolean clean = isDocumentClean();
        if (monacoDocumentCreated) {
            browser.executeJavaScript("window.SciNotesMonacoSetDocumentValue("
                + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(pendingText) + ", "
                + documentRevision + ", " + clean + ", " + (!isEditable())
                + ");", browser.getURL(), 1);
        } else if (!sharedBrowser.initialDocumentAdopted && sharedBrowser.getActivePane() == this) {
            /*
             * The page's very first buffer, before it is adopted as this
             * pane's document. That call writes into whichever document is
             * active, so no other pane may use it: the secondary half of a
             * split has no document of its own -- its primary owns it -- and
             * pushing its text this way replaced the saved version of the
             * active tab with "nothing is saved", which no undo could then
             * clear.
             */
            browser.executeJavaScript("window.SciNotesMonacoSetValue(" + quoteJs(pendingText) + ", "
                + documentRevision + ", " + clean + ");", browser.getURL(), 1);
        }
        pendingText = null;
    }

    private void updateReadOnlyState() {
        if (browserReady && monacoDocumentCreated) {
            browser.executeJavaScript("window.SciNotesMonacoSetDocumentReadOnly("
                + quoteJs(getMonacoDocumentId()) + ", " + (!isEditable())
                + ");", browser.getURL(), 1);
        }
    }

    private void configureShortcuts() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoConfigureShortcuts(" + getShortcutConfiguration() + ");", browser.getURL(), 1);
        }
    }

    private void configureLsp() {
        if (!browserReady) {
            return;
        }

        SciNotesLspManager.Endpoint endpoint = SciNotesLspManager.getOrStartEndpoint();
        if (endpoint == null) {
            return;
        }

        browser.executeJavaScript("window.SciNotesMonacoConfigureLsp(" + getLspConfiguration(endpoint) + ");", browser.getURL(), 1);
    }

    private void configureDap() {
        if (!browserReady) {
            return;
        }

        SciNotesDapManager.Endpoint endpoint = SciNotesDapManager.getOrStartEndpoint();
        if (endpoint == null) {
            return;
        }

        browser.executeJavaScript("window.SciNotesMonacoConfigureDap(" + getDapConfiguration(endpoint) + ");", browser.getURL(), 1);
        syncDapStore();
    }

    /**
     * Push this tab's canonical source path and its persisted breakpoints to the
     * Monaco document registry. The browser keeps them per model and replays the
     * complete per-source list to DAP, so switching tabs never loses them.
     */
    private void syncDapDocument() {
        if (!browserReady || !monacoDocumentCreated || getUUID() == null
                || SciNotesDapManager.getEndpoint() == null) {
            return;
        }

        String sourcePath = getSavedSourcePath();
        browser.executeJavaScript("window.SciNotesMonacoSetDocumentBreakpoints("
            + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(getDocumentUri()) + ", "
            + quoteJs(sourcePath) + ", "
            + SciNotesBreakpointStore.getInstance().getBreakpointsJson(sourcePath)
            + ");", browser.getURL(), 1);
    }

    /**
     * Hand the stored list of one file to every other tab showing it. All
     * SciNotes windows of a Scilab share one durable store, so a breakpoint set
     * in one of them belongs to the others as much: without this they would
     * only learn it from the debug session, and draw it as somebody else's.
     * @param sourcePath canonical path of the file whose list just changed
     */
    private void refreshBreakpointsInOtherPanes(String sourcePath) {
        if (sourcePath == null || sourcePath.isEmpty()) {
            return;
        }

        for (SciNotes editor : SciNotes.getSciNotesList()) {
            for (int index = 0; index < editor.getTabPane().getTabCount(); index++) {
                ScilabEditorPane pane = editor.getTextPane(index);
                if (pane == this || !(pane instanceof MonacoEditorPane)) {
                    continue;
                }

                MonacoEditorPane monacoPane = (MonacoEditorPane) pane;
                if (!sourcePath.equals(monacoPane.getSavedSourcePath())) {
                    continue;
                }

                // A modified document owns its lines until it is saved: the
                // store still describes the file on disk, not what it shows.
                if (((ScilabDocument) monacoPane.getDocument()).isContentModified()) {
                    continue;
                }

                monacoPane.syncDapDocument();
            }
        }
    }

    /**
     * Whether some open tab holds that file at that saved path.
     * @param sourcePath canonical path to look for
     * @return true when a Monaco tab shows that file
     */
    private static boolean isSourcePathOpen(String sourcePath) {
        for (SciNotes editor : SciNotes.getSciNotesList()) {
            for (int index = 0; index < editor.getTabPane().getTabCount(); index++) {
                ScilabEditorPane pane = editor.getTextPane(index);
                if (pane instanceof MonacoEditorPane
                        && sourcePath.equals(((MonacoEditorPane) pane).getSavedSourcePath())) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Hand the whole durable store to this window's browser. Breakpoints belong
     * to their file, not to an open tab: the debug session must know every line
     * the store holds, so one still stops the execution when no editor shows
     * that file, and the stop then opens it.
     */
    private void syncDapStore() {
        if (!browserReady || getUUID() == null || SciNotesDapManager.getEndpoint() == null) {
            return;
        }

        browser.executeJavaScript("window.SciNotesMonacoSetStoredBreakpoints("
            + SciNotesBreakpointStore.getInstance().getAllBreakpointsJson()
            + ");", browser.getURL(), 1);
    }

    /**
     * Hand the store to every open SciNotes window: they share it, and each has
     * its own browser to tell.
     */
    private static void syncDapStoreEverywhere() {
        for (SciNotes editor : SciNotes.getSciNotesList()) {
            for (int index = 0; index < editor.getTabPane().getTabCount(); index++) {
                ScilabEditorPane pane = editor.getTextPane(index);
                if (pane instanceof MonacoEditorPane) {
                    // One browser per window: the first Monaco tab reaches it.
                    ((MonacoEditorPane) pane).syncDapStore();
                    break;
                }
            }
        }
    }

    /** Capture breakpoint decorations before a successful save refreshes the store. */
    private void prepareDapBreakpointsForSave() {
        if (browserReady && monacoDocumentCreated && getUUID() != null) {
            browser.executeJavaScript("window.SciNotesMonacoPrepareDocumentBreakpointsForSave("
                + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
        }
    }

    /**
     * Persist the breakpoints Monaco owns for a document that just obtained a
     * saved path. Until then the store has nothing under that path, so the
     * browser is the authority and hands its pending lines over here.
     */
    private void persistBreakpointsFromMonaco(String payload) {
        try {
            MonacoBreakpointPayload parsed = new Gson().fromJson(payload, MonacoBreakpointPayload.class);
            if (parsed == null || parsed.sourcePath == null || parsed.sourcePath.isEmpty()) {
                return;
            }
            /*
             * The bridge addresses a message with the browser's active
             * document, which is not always the document the payload is about:
             * a click in a split view or in a background tab publishes under
             * another document's name. The payload carries the file it
             * describes, and the store is keyed by that path, so persist what
             * it says provided some open tab still has that file saved there -
             * which is also what protects the store from a payload made
             * obsolete by a Save As.
             */
            if (!isSourcePathOpen(parsed.sourcePath)) {
                return;
            }

            List<SciNotesBreakpointStore.Breakpoint> breakpoints =
                new ArrayList<SciNotesBreakpointStore.Breakpoint>();
            if (parsed.breakpoints != null) {
                for (MonacoBreakpoint breakpoint : parsed.breakpoints) {
                    if (breakpoint != null && breakpoint.line > 0) {
                        breakpoints.add(new SciNotesBreakpointStore.Breakpoint(breakpoint.line,
                            breakpoint.enabled, breakpoint.condition, breakpoint.hitCondition));
                    }
                }
            }
            SciNotesBreakpointStore.getInstance().replaceBreakpoints(parsed.sourcePath, breakpoints);
            refreshBreakpointsInOtherPanes(parsed.sourcePath);
            syncDapStoreEverywhere();        } catch (JsonSyntaxException e) {
            System.err.println("SciNotes DAP: invalid breakpoint payload: " + e.getMessage());
        }
    }

    /** Wire format of the breakpoint list reported by monaco-dap.js. */
    private static final class MonacoBreakpointPayload {
        private String sourcePath;
        private List<MonacoBreakpoint> breakpoints;
    }

    private static final class MonacoBreakpoint {
        private int line;
        private boolean enabled;
        private String condition;
        private String hitCondition;
    }

    /** Transient command availability reported by the browser DAP client. */
    private static final class MonacoDapState {
        private boolean canContinue;
        private boolean canPause;
        private boolean canStepOver;
        private boolean canStepIn;
        private boolean canStepOut;
        private boolean canDisconnect;
        private boolean canExecute;
        private boolean canDebug;
        private boolean debugMode;
        private boolean sessionReady;
        private String state;
        private String message;
    }

    /** Drop this tab from the browser-side DAP registry; breakpoints stay persisted. */
    private void removeDapDocument() {
        // Not browserReady: a closing pane has already dropped it, and this
        // runs while it unregisters.
        if (!sharedBrowser.monacoReady || getUUID() == null || SciNotesDapManager.getEndpoint() == null) {
            return;
        }

        browser.executeJavaScript("window.SciNotesMonacoRemoveDocumentBreakpoints("
            + quoteJs(getMonacoDocumentId()) + ");", browser.getURL(), 1);
    }

    public void applyDisplayOptions() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoApplyOptions(" + getDisplayOptionsConfiguration() + ");", browser.getURL(), 1);
        }
    }

    @Override
    public void contentMarkedUnmodified() {
        markMonacoSaved(contentDigest(getDocumentText()));
    }

    /**
     * Tell Monaco that this text is the one on disk.
     *
     * The buffer is named by its contents, not by a revision number: a tab
     * can be one edit behind the page while the file was written from
     * another tab on the same buffer, and its revision would then reject a
     * version that is in fact the saved one. Comparing the text answers the
     * only question that matters -- is what Monaco shows what was written?
     *
     * @param digest the contents written to disk, as {@link #contentDigest}
     */
    private void markMonacoSaved(String digest) {
        if (splitPrimary != null) {
            // The document of a split names its secondary half as its pane,
            // but the Monaco document belongs to the primary one.
            splitPrimary.markMonacoSaved(digest);
            return;
        }
        if (browserReady && monacoDocumentCreated) {
            prepareDapBreakpointsForSave();
            browser.executeJavaScript("window.SciNotesMonacoMarkDocumentSaved("
                + quoteJs(getMonacoDocumentId()) + ", " + quoteJs(digest)
                + ");", browser.getURL(), 1);
            // The file now exists at its canonical path, so its breakpoints can
            // be verified and installed for real.
            syncDapDocument();
        }
    }

    /** Same formula as the page (monaco-shared-history.js). */
    private static String contentDigest(String text) {
        String value = text == null ? "" : text;
        int hash = 0;
        for (int i = 0; i < value.length(); i++) {
            hash = hash * 31 + value.charAt(i);
        }
        return value.length() + ":" + hash;
    }

    private void updateLineNumberOptions() {
        if (browserReady && sharedBrowser.getActivePane() == this
                && SciNotesOptions.getSciNotesDisplay().showLineNumbers
                && SciNotesOptions.getSciNotesDisplay().whereami) {
            browser.executeJavaScript("window.SciNotesMonacoApplyLineNumbers("
                + getLineNumberConfiguration() + ");", browser.getURL(), 1);
        }
    }

    public void updateAnchorDecorations() {
        if (browserReady && sharedBrowser.getActivePane() == this) {
            browser.executeJavaScript("window.SciNotesMonacoSetAnchors(" + getAnchorConfiguration() + ");", browser.getURL(), 1);
        }
    }

    private void focusMonacoFromUserClick() {
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoFocus();", browser.getURL(), 1);
        }
    }

    private void focusMonacoIfRequested() {
        if (!focusRequested || !browserReady || browserDisposed) {
            return;
        }

        SciNotes ed = getEditor();
        if (ed == null || getSelectedEditorComponentIndex(ed) < 0) {
            return;
        }

        if (SciNotes.isTabTransferInProgress()) {
            /*
             * Closing the source tab of a cross-window or drag-out transfer
             * can make JTabbedPane auto-select a sibling tab in the source
             * window. Without this guard that sibling would grab native CEF
             * focus here and immediately hand it back to the source window,
             * flashing both windows' titlebars right after the transfer.
             * Leave focusRequested set so a later, real focus event still
             * wins normally.
             */
            return;
        }

        focusRequested = false;
        cefPanel.requestBrowserFocus();
        browser.executeJavaScript("window.SciNotesMonacoFocus();", browser.getURL(), 1);
    }

    private void revealLine(int pos) {
        int line = Math.max(1, getDocument().getDefaultRootElement().getElementIndex(Math.max(0, pos)) + 1);
        revealLineNumber(line);
    }

    /*
     * The page shows one document at a time: revealing a line of a tab whose
     * model is not the displayed one would move the caret of another file.
     * That happens when a file is opened at a line (search in files, error
     * link) before its tab became the active pane or before Monaco is up.
     * Keep the line and reveal it when this document is selected.
     */
    private void revealLineNumber(int lineNumber) {
        if (lineNumber <= 0) {
            return;
        }
        revealRequests++;
        if (splitPrimary == null && (!browserReady || !monacoDocumentCreated
                || sharedBrowser.getActivePane() != this)) {
            pendingRevealLine = lineNumber;
            return;
        }
        pendingRevealLine = 0;
        if (browserReady) {
            browser.executeJavaScript("window.SciNotesMonacoRevealLine(" + lineNumber + ");", browser.getURL(), 1);
        }
    }

    private void revealPendingLine() {
        int line = pendingRevealLine;
        if (line > 0) {
            revealLineNumber(line);
        }
    }

    /**
     * The Swing lexer can miss URL tokens while the visible caret lives in
     * Monaco. Keep the legacy Open URL action working by detecting the token
     * around the mirrored caret in the read-replica.
     */
    private KeywordEvent getUrlKeywordEvent(int position) {
        Document doc = getDocument();
        if (doc == null) {
            return null;
        }

        String text = getDocumentText();
        if (text.isEmpty()) {
            return null;
        }

        int cursor = clamp(position, 0, text.length());
        int start = cursor;
        while (start > 0 && !isUrlBoundary(text.charAt(start - 1))) {
            start--;
        }

        int end = cursor;
        while (end < text.length() && !isUrlBoundary(text.charAt(end))) {
            end++;
        }

        while (end > start && isTrailingUrlPunctuation(text.charAt(end - 1))) {
            end--;
        }

        if (end <= start) {
            return null;
        }

        String token = text.substring(start, end);
        int type = getUrlTokenType(token);
        if (type == -1) {
            return null;
        }

        return new KeywordEvent(this, null, type, start, end - start);
    }

    private static boolean isUrlBoundary(char c) {
        return Character.isWhitespace(c) || c == '"' || c == '\'' || c == '<' || c == '>';
    }

    private static boolean isTrailingUrlPunctuation(char c) {
        return c == '.' || c == ',' || c == ';' || c == ':' || c == ')' || c == ']';
    }

    private static int getUrlTokenType(String token) {
        String lower = token.toLowerCase();
        if (lower.startsWith("http://") || lower.startsWith("https://") || lower.startsWith("ftp://")
                || lower.startsWith("file://") || lower.startsWith("www.")) {
            return ScilabLexerConstants.URL;
        }

        if (token.indexOf('@') > 0 && token.indexOf(' ', token.indexOf('@')) == -1) {
            return ScilabLexerConstants.MAIL;
        }

        return -1;
    }

    private void attachDocumentListener(Document doc) {
        if (doc != null && documentListener != null) {
            doc.addDocumentListener(documentListener);
        }
    }

    private void detachDocumentListener(Document doc) {
        if (doc != null && documentListener != null) {
            doc.removeDocumentListener(documentListener);
        }
    }

    private String getDocumentText(int offset, int length) {
        Document doc = getDocument();
        if (doc == null) {
            return "";
        }

        try {
            return doc.getText(offset, length);
        } catch (BadLocationException e) {
            return "";
        }
    }

    private String getDocumentText() {
        Document doc = getDocument();
        if (doc == null) {
            return "";
        }

        try {
            return doc.getText(0, doc.getLength());
        } catch (BadLocationException e) {
            return "";
        }
    }

    private String getShortcutConfiguration() {
        StringBuilder builder = new StringBuilder();
        builder.append("{\"shortcuts\":[");

        boolean first = true;
        for (int i = 0; i < MONACO_DISABLED_SHORTCUTS.length; i++) {
            if (!first) {
                builder.append(',');
            }
            String[] shortcut = MONACO_DISABLED_SHORTCUTS[i];
            appendShortcut(builder, shortcut[0], Boolean.parseBoolean(shortcut[1]), Boolean.parseBoolean(shortcut[2]), Boolean.parseBoolean(shortcut[3]), null, true);
            first = false;
        }

        for (int i = 0; i < MONACO_FIXED_BRIDGED_SHORTCUTS.length; i++) {
            if (!first) {
                builder.append(',');
            }
            String[] shortcut = MONACO_FIXED_BRIDGED_SHORTCUTS[i];
            appendShortcut(builder, shortcut[0], Boolean.parseBoolean(shortcut[1]), Boolean.parseBoolean(shortcut[2]), Boolean.parseBoolean(shortcut[3]), shortcut[4], false);
            first = false;
        }

        for (Map.Entry<String, String> entry : MONACO_BRIDGED_ACTIONS.entrySet()) {
            KeyStroke keyStroke = getKeyStrokeForAction(entry.getKey());
            String keyCode = getMonacoKeyCodeName(keyStroke);
            if (keyCode == null) {
                continue;
            }

            if (!first) {
                builder.append(',');
            }
            appendShortcut(builder, keyCode, keyStroke.getModifiers(), entry.getValue(), false, getMonacoCommandForBridge(entry.getValue()), isNativePassthroughBridge(entry.getValue()));
            first = false;
        }

        for (int i = 0; i < MONACO_ONLY_SHORTCUTS.length; i++) {
            if (!first) {
                builder.append(',');
            }
            String[] shortcut = MONACO_ONLY_SHORTCUTS[i];
            appendShortcut(builder, shortcut[0], Boolean.parseBoolean(shortcut[1]), Boolean.parseBoolean(shortcut[2]), Boolean.parseBoolean(shortcut[3]), null, false, shortcut[4]);
            first = false;
        }

        builder.append("]}");
        return builder.toString();
    }

    private String getDisplayOptionsConfiguration() {
        SciNotesOptions.Display display = SciNotesOptions.getSciNotesDisplay();
        ScilabContext context = ((ScilabEditorKit) getEditorKit()).getStylePreferences();
        Font font = context.getBaseFont();

        StringBuilder builder = new StringBuilder();
        builder.append('{');
        appendJsonProperty(builder, "fontFamily", font == null ? null : font.getFamily());
        builder.append(",\"fontSize\":").append(font == null ? 13 : font.getSize());
        builder.append(",\"highlightCurrentLine\":").append(display.highlightCurrentLine);
        builder.append(",\"lineNumbers\":").append(getLineNumberConfiguration());
        builder.append(",\"wordWrap\":").append(quoteJs(display.wrapLines ? "on" : "off"));
        builder.append(",\"tabSize\":").append(Math.max(1, display.tabSize));
        builder.append(",\"insertSpaces\":").append(display.useSpaces);
        builder.append(",\"showWhitespaces\":").append(display.showWhitespaces);
        builder.append(",\"autoClosingPairs\":").append(display.autoCompleteOpeners);
        builder.append(",\"snippets\":").append(display.autoCompleteKeywords);
        builder.append(",\"theme\":");
        appendThemeConfiguration(builder, display, context);
        builder.append('}');
        return builder.toString();
    }

    private String getLineNumberConfiguration() {
        SciNotesOptions.Display display = SciNotesOptions.getSciNotesDisplay();
        if (!display.showLineNumbers) {
            return "{\"mode\":\"off\"}";
        }
        if (!display.whereami || !(getDocument() instanceof ScilabDocument)) {
            return "{\"mode\":\"absolute\"}";
        }
        return "{\"mode\":\"local\"}";
    }

    private void appendThemeConfiguration(StringBuilder builder, SciNotesOptions.Display display, ScilabContext context) {
        builder.append('{');
        builder.append("\"syntaxHighlighting\":").append(display.keywordsColorization);
        builder.append(",\"semanticHighlighting\":").append(display.keywordsColorization);
        builder.append(",\"ui\":{");
        appendColorProperty(builder, "background", display.backgroundColor, true);
        appendColorProperty(builder, "caret", display.caretColor, false);
        builder.append('}');
        builder.append(",\"tokens\":{");
        appendTokenStyle(builder, "defaultText", context, ScilabLexerConstants.DEFAULT, true);
        appendTokenStyle(builder, "special", context, ScilabLexerConstants.SPECIAL, false);
        appendTokenStyle(builder, "operator", context, ScilabLexerConstants.OPERATOR, false);
        appendTokenStyle(builder, "primitive", context, ScilabLexerConstants.COMMANDS, false);
        appendTokenStyle(builder, "macro", context, ScilabLexerConstants.MACROS, false);
        appendTokenStyle(builder, "macroInFile", context, ScilabLexerConstants.MACROINFILE, false);
        appendTokenStyle(builder, "string", context, ScilabLexerConstants.STRING, false);
        appendTokenStyle(builder, "inputOutputArgs", context, ScilabLexerConstants.INPUTOUTPUTARGS, false);
        appendTokenStyle(builder, "identifier", context, ScilabLexerConstants.ID, false);
        appendTokenStyle(builder, "comment", context, ScilabLexerConstants.COMMENT, false);
        appendTokenStyle(builder, "field", context, ScilabLexerConstants.FIELD, false);
        appendTokenStyle(builder, "functionKeyword", context, ScilabLexerConstants.FKEYWORD, false);
        appendTokenStyle(builder, "url", context, ScilabLexerConstants.URL, false);
        appendTokenStyle(builder, "mail", context, ScilabLexerConstants.MAIL, false);
        appendTokenStyle(builder, "number", context, ScilabLexerConstants.NUMBER, false);
        appendTokenStyle(builder, "control", context, ScilabLexerConstants.CKEYWORD, false);
        appendTokenStyle(builder, "structure", context, ScilabLexerConstants.SKEYWORD, false);
        appendTokenStyle(builder, "constants", context, ScilabLexerConstants.CONSTANTES, false);
        appendTokenStyle(builder, "openClose", context, ScilabLexerConstants.OPENCLOSE, false);
        appendTokenStyle(builder, "todo", context, ScilabLexerConstants.TODO, false);
        appendTokenStyle(builder, "author", context, ScilabLexerConstants.AUTHORS, false);
        appendTokenStyle(builder, "latexComment", context, ScilabLexerConstants.LATEX, false);
        appendTokenStyle(builder, "latexString", context, ScilabLexerConstants.LATEXINSTRING, false);
        builder.append('}');
        builder.append('}');
    }

    private void appendColorProperty(StringBuilder builder, String name, Color color, boolean first) {
        if (!first) {
            builder.append(',');
        }
        builder.append('"').append(name).append("\":").append(quoteJs(toHex(color)));
    }

    private void appendTokenStyle(StringBuilder builder, String name, ScilabContext context, int token, boolean first) {
        if (!first) {
            builder.append(',');
        }

        builder.append('"').append(name).append("\":{");
        builder.append("\"foreground\":").append(quoteJs(toHex(context.tokenColors[token])));
        String fontStyle = getMonacoFontStyle(context.tokenFonts[token], context.tokenAttrib[token]);
        if (!fontStyle.isEmpty()) {
            builder.append(",\"fontStyle\":").append(quoteJs(fontStyle));
        }
        builder.append('}');
    }

    private static String toHex(Color color) {
        Color safeColor = color == null ? Color.BLACK : color;
        return String.format("%02X%02X%02X", safeColor.getRed(), safeColor.getGreen(), safeColor.getBlue());
    }

    private static String getMonacoFontStyle(Font font, int attributes) {
        StringBuilder builder = new StringBuilder();
        if (font != null && font.isBold()) {
            builder.append("bold");
        }
        if (font != null && font.isItalic()) {
            appendFontStyle(builder, "italic");
        }
        if ((attributes & 1) != 0) {
            appendFontStyle(builder, "underline");
        }
        if ((attributes & 2) != 0) {
            appendFontStyle(builder, "strikethrough");
        }
        return builder.toString();
    }

    private static void appendFontStyle(StringBuilder builder, String style) {
        if (builder.length() > 0) {
            builder.append(' ');
        }
        builder.append(style);
    }

    private static void appendJsonProperty(StringBuilder builder, String name, String value) {
        builder.append('"').append(name).append("\":");
        if (value == null) {
            builder.append("null");
        } else {
            builder.append(quoteJs(value));
        }
    }

    private static KeyStroke getKeyStrokeForAction(String actionName) {
        Map<String, KeyStroke> actionKeys = SciNotes.getActionKeys();
        KeyStroke keyStroke = actionKeys.get(actionName);
        if (keyStroke != null) {
            return keyStroke;
        }

        if ("scinotes-completion".equals(actionName)) {
            keyStroke = actionKeys.get("SciNotesCompletionAction");
            if (keyStroke != null) {
                return keyStroke;
            }
        }

        String javaAction = SciNotes.getActionName().get(actionName);
        if (javaAction == null) {
            return null;
        }

        keyStroke = actionKeys.get(javaAction);
        if (keyStroke != null) {
            return keyStroke;
        }

        int dot = javaAction.lastIndexOf('.');
        if (dot != -1 && dot + 1 < javaAction.length()) {
            return actionKeys.get(javaAction.substring(dot + 1));
        }

        return null;
    }

    private void appendShortcut(StringBuilder builder, String keyCode, int modifiers, String bridge, boolean disabled) {
        appendShortcut(builder, keyCode, modifiers, bridge, disabled, null, false);
    }

    private void appendShortcut(StringBuilder builder, String keyCode, int modifiers, String bridge, boolean disabled, String monacoCommand) {
        appendShortcut(builder, keyCode, modifiers, bridge, disabled, monacoCommand, false);
    }

    private void appendShortcut(StringBuilder builder, String keyCode, int modifiers, String bridge, boolean disabled, String monacoCommand, boolean nativePassthrough) {
        boolean ctrlCmd = hasShortcutModifier(modifiers, InputEvent.CTRL_DOWN_MASK, LEGACY_CTRL_MASK)
                || hasShortcutModifier(modifiers, InputEvent.META_DOWN_MASK, LEGACY_META_MASK);
        boolean shift = hasShortcutModifier(modifiers, InputEvent.SHIFT_DOWN_MASK, LEGACY_SHIFT_MASK);
        boolean alt = hasShortcutModifier(modifiers, InputEvent.ALT_DOWN_MASK, LEGACY_ALT_MASK);
        appendShortcut(builder, keyCode, getMonacoKeyName(keyCode), ctrlCmd, shift, alt, bridge, disabled, monacoCommand, nativePassthrough);
    }

    private static boolean hasShortcutModifier(int modifiers, int downMask, int legacyMask) {
        return (modifiers & downMask) != 0 || (modifiers & legacyMask) != 0;
    }

    private static String getMonacoCommandForBridge(String bridge) {
        if ("completion".equals(bridge)) {
            return "editor.action.triggerSuggest";
        }

        return null;
    }

    private static boolean isNativePassthroughBridge(String bridge) {
        return "paste".equals(bridge);
    }

    private static boolean isBrowserPassthroughBridge(String bridge) {
        // Tab must reach the web page so Monaco can accept a visible
        // completion before the normal SciNotes indentation action runs.
        return isNativePassthroughBridge(bridge)
                || "shiftRight".equals(bridge)
                || "shiftLeft".equals(bridge);
    }

    private void appendShortcut(StringBuilder builder, String keyCode, boolean ctrlCmd, boolean shift, boolean alt, String bridge, boolean disabled) {
        appendShortcut(builder, keyCode, ctrlCmd, shift, alt, bridge, disabled, null);
    }

    private void appendShortcut(StringBuilder builder, String keyCode, boolean ctrlCmd, boolean shift, boolean alt, String bridge, boolean disabled, String monacoCommand) {
        appendShortcut(builder, keyCode, null, ctrlCmd, shift, alt, bridge, disabled, monacoCommand, false);
    }

    private void appendShortcut(StringBuilder builder, String keyCode, String key, boolean ctrlCmd, boolean shift, boolean alt, String bridge, boolean disabled, String monacoCommand, boolean nativePassthrough) {
        builder.append('{');
        builder.append("\"keyCode\":\"").append(keyCode).append("\",");
        if (key != null) {
            builder.append("\"key\":").append(quoteJs(key)).append(',');
        }
        builder.append("\"ctrlCmd\":").append(ctrlCmd).append(',');
        builder.append("\"shift\":").append(shift).append(',');
        builder.append("\"alt\":").append(alt).append(',');
        builder.append("\"disabled\":").append(disabled);
        if (bridge != null) {
            builder.append(",\"bridge\":\"").append(bridge).append('"');
        }
        if (monacoCommand != null) {
            builder.append(",\"monacoCommand\":").append(quoteJs(monacoCommand));
        }
        if (nativePassthrough) {
            builder.append(",\"nativePassthrough\":true");
        }
        builder.append('}');
    }

    private static KeyStroke getKeyStrokeFromShortcut(String[] shortcut) {
        if (shortcut == null || shortcut.length < 5) {
            return null;
        }

        int keyCode = getJavaKeyCode(shortcut[0]);
        if (keyCode == -1) {
            return null;
        }

        int modifiers = 0;
        if (Boolean.parseBoolean(shortcut[1])) {
            modifiers |= InputEvent.CTRL_DOWN_MASK;
        }
        if (Boolean.parseBoolean(shortcut[2])) {
            modifiers |= InputEvent.SHIFT_DOWN_MASK;
        }
        if (Boolean.parseBoolean(shortcut[3])) {
            modifiers |= InputEvent.ALT_DOWN_MASK;
        }

        return KeyStroke.getKeyStroke(keyCode, modifiers);
    }

    private static int getJavaKeyCode(String keyCode) {
        if (keyCode == null) {
            return -1;
        }
        if (keyCode.startsWith("Key") && keyCode.length() == 4) {
            return keyCode.charAt(3);
        }
        if (keyCode.startsWith("Digit") && keyCode.length() == 6) {
            return keyCode.charAt(5);
        }
        if (keyCode.startsWith("F")) {
            try {
                int functionKey = Integer.parseInt(keyCode.substring(1));
                if (functionKey >= 1 && functionKey <= 12) {
                    return KeyEvent.VK_F1 + functionKey - 1;
                }
            } catch (NumberFormatException e) {
                return -1;
            }
        }

        if ("Backspace".equals(keyCode)) {
            return KeyEvent.VK_BACK_SPACE;
        } else if ("Tab".equals(keyCode)) {
            return KeyEvent.VK_TAB;
        } else if ("Enter".equals(keyCode)) {
            return KeyEvent.VK_ENTER;
        } else if ("Escape".equals(keyCode)) {
            return KeyEvent.VK_ESCAPE;
        } else if ("Space".equals(keyCode)) {
            return KeyEvent.VK_SPACE;
        } else if ("PageUp".equals(keyCode)) {
            return KeyEvent.VK_PAGE_UP;
        } else if ("PageDown".equals(keyCode)) {
            return KeyEvent.VK_PAGE_DOWN;
        } else if ("End".equals(keyCode)) {
            return KeyEvent.VK_END;
        } else if ("Home".equals(keyCode)) {
            return KeyEvent.VK_HOME;
        } else if ("ArrowLeft".equals(keyCode)) {
            return KeyEvent.VK_LEFT;
        } else if ("ArrowUp".equals(keyCode)) {
            return KeyEvent.VK_UP;
        } else if ("ArrowRight".equals(keyCode)) {
            return KeyEvent.VK_RIGHT;
        } else if ("ArrowDown".equals(keyCode)) {
            return KeyEvent.VK_DOWN;
        } else if ("Insert".equals(keyCode)) {
            return KeyEvent.VK_INSERT;
        } else if ("Delete".equals(keyCode)) {
            return KeyEvent.VK_DELETE;
        } else if ("Less".equals(keyCode)) {
            return KeyEvent.VK_LESS;
        }

        return -1;
    }

    private static String getMonacoKeyCodeName(KeyStroke keyStroke) {
        if (keyStroke == null) {
            return null;
        }

        int keyCode = keyStroke.getKeyCode();
        if (keyCode >= KeyEvent.VK_A && keyCode <= KeyEvent.VK_Z) {
            return "Key" + (char) keyCode;
        }
        if (keyCode >= KeyEvent.VK_0 && keyCode <= KeyEvent.VK_9) {
            return "Digit" + (char) keyCode;
        }
        if (keyCode >= KeyEvent.VK_F1 && keyCode <= KeyEvent.VK_F12) {
            return "F" + (keyCode - KeyEvent.VK_F1 + 1);
        }

        switch (keyCode) {
            case KeyEvent.VK_BACK_SPACE:
                return "Backspace";
            case KeyEvent.VK_TAB:
                return "Tab";
            case KeyEvent.VK_ENTER:
                return "Enter";
            case KeyEvent.VK_ESCAPE:
                return "Escape";
            case KeyEvent.VK_SPACE:
                return "Space";
            case KeyEvent.VK_PAGE_UP:
                return "PageUp";
            case KeyEvent.VK_PAGE_DOWN:
                return "PageDown";
            case KeyEvent.VK_END:
                return "End";
            case KeyEvent.VK_HOME:
                return "Home";
            case KeyEvent.VK_LEFT:
                return "ArrowLeft";
            case KeyEvent.VK_UP:
                return "ArrowUp";
            case KeyEvent.VK_RIGHT:
                return "ArrowRight";
            case KeyEvent.VK_DOWN:
                return "ArrowDown";
            case KeyEvent.VK_INSERT:
                return "Insert";
            case KeyEvent.VK_DELETE:
                return "Delete";
            case KeyEvent.VK_LESS:
                return "Less";
            default:
                return null;
        }
    }

    private static String getMonacoKeyName(String keyCode) {
        if (keyCode != null && keyCode.startsWith("Key") && keyCode.length() == 4) {
            return keyCode.substring(3).toLowerCase();
        }
        if (keyCode != null && keyCode.startsWith("Digit") && keyCode.length() == 6) {
            return keyCode.substring(5);
        }
        if ("Less".equals(keyCode)) {
            return "<";
        }

        return null;
    }

    private String getAnchorConfiguration() {
        Document document = getDocument();
        if (!(document instanceof ScilabDocument)) {
            return "[]";
        }

        StringBuilder builder = new StringBuilder();
        builder.append('[');
        Element root = document.getDefaultRootElement();
        boolean first = true;
        for (int i = 0; i < root.getElementCount(); i++) {
            Element element = root.getElement(i);
            if (element instanceof ScilabDocument.ScilabLeafElement) {
                ScilabDocument.ScilabLeafElement line = (ScilabDocument.ScilabLeafElement) element;
                if (line.isAnchor()) {
                    if (!first) {
                        builder.append(',');
                    }
                    builder.append("{\"line\":").append(i + 1).append(",\"name\":").append(quoteJs(line.getAnchorName())).append('}');
                    first = false;
                }
            }
        }
        builder.append(']');
        return builder.toString();
    }

    private String getLspConfiguration(SciNotesLspManager.Endpoint endpoint) {
        StringBuilder builder = new StringBuilder();
        builder.append('{');
        builder.append("\"url\":").append(quoteJs(endpoint.getUrl()));
        builder.append(",\"documentUri\":").append(quoteJs(getDocumentUri()));
        builder.append(",\"languageId\":\"scilab\"");
        builder.append(",\"enableSignatureHelp\":true");
        builder.append('}');
        return builder.toString();
    }

    /**
     * Connection-level DAP configuration only. It is sent once for the browser
     * shared by one SciNotes window; per-document breakpoints are pushed
     * separately by syncDapDocument() because one browser hosts every tab.
     */
    private String getDapConfiguration(SciNotesDapManager.Endpoint endpoint) {
        StringBuilder builder = new StringBuilder();
        builder.append('{');
        builder.append("\"url\":").append(quoteJs(endpoint.getUrl()));
        builder.append(",\"sessionId\":").append(quoteJs(endpoint.getSessionId()));
        // The page has no translations of its own: its texts come from here,
        // under the keys of DAP_TEXTS in monaco-dap.js and DAP_CLIENT_TEXTS
        // in dap-client.js.
        String[][] messages = {
            {"breakpoint", SciNotesMessages.DEBUG_BREAKPOINT},
            {"breakpointDisabled", SciNotesMessages.DEBUG_BREAKPOINT_DISABLED},
            {"breakpointForeign", SciNotesMessages.DEBUG_BREAKPOINT_FOREIGN},
            {"breakpointNoCode", SciNotesMessages.DEBUG_BREAKPOINT_NO_CODE},
            {"breakpointHitCount", SciNotesMessages.DEBUG_BREAKPOINT_HIT_COUNT},
            {"breakpointNoFile", SciNotesMessages.DEBUG_BREAKPOINT_NO_FILE},
            {"stoppedHere", SciNotesMessages.DEBUG_STOPPED_HERE},
            {"stoppedHereOnBreakpoint", SciNotesMessages.DEBUG_STOPPED_HERE_ON_BREAKPOINT},
            {"stopped", SciNotesMessages.DEBUG_STOPPED},
            {"stoppedOnError", SciNotesMessages.DEBUG_STOPPED_ON_ERROR},
            {"pausedOnBreakpoint", SciNotesMessages.DEBUG_PAUSED_ON_BREAKPOINT},
            {"paused", SciNotesMessages.DEBUG_PAUSED},
            {"running", SciNotesMessages.DEBUG_RUNNING},
            {"completed", SciNotesMessages.DEBUG_COMPLETED},
            {"connectionLost", SciNotesMessages.DEBUG_CONNECTION_LOST},
            {"connectionFailed", SciNotesMessages.DEBUG_CONNECTION_FAILED},
            {"noResponse", SciNotesMessages.DEBUG_NO_RESPONSE},
            {"fileChangedSinceRun", SciNotesMessages.DEBUG_FILE_CHANGED_SINCE_RUN}
        };
        builder.append(",\"messages\":{");
        for (int i = 0; i < messages.length; i++) {
            if (i > 0) {
                builder.append(',');
            }
            builder.append(quoteJs(messages[i][0])).append(':').append(quoteJs(messages[i][1]));
        }
        builder.append('}');
        builder.append('}');
        return builder.toString();
    }

    private String getSavedSourcePath() {
        String name = getName();
        if (name == null || name.isEmpty()) {
            return "";
        }

        File source = new File(name);
        if (!source.isFile()) {
            return "";
        }
        try {
            // SciNotesBreakpointStore keys on the canonical path. Sending
            // anything else would let one file appear as two browser-side
            // groups whenever a symlink or a relative segment is involved.
            return source.getCanonicalPath();
        } catch (IOException e) {
            return source.getAbsolutePath();
        }
    }

    private String getDocumentUri() {
        String name = getName();
        if (name != null && !name.isEmpty()) {
            return new File(name).toURI().toString();
        }

        return "untitled:scinotes/" + getUUID().toString();
    }

    // Monaco is shipped under Scilab thirdparty.
    private static final String MONACO_VS_PATH = "monaco-editor/vs";

    private static String getMonacoUrl() {
        try {
            String sci = ScilabConstants.SCI.getPath();
            String html = new File(sci, "modules/scinotes/src/web/monaco.html").toURI().toURL().toString();
            String jsDir = sci + "/thirdparty/web";
            if ((OS.get() != OS.WINDOWS) && (new File(jsDir).exists() == false)) {
                // Probably an installed Linux or macOS version where thirdparty/ is in SCI/../../
                jsDir = sci + "/../../thirdparty/web";
            }
            String vs = new File(jsDir, MONACO_VS_PATH).toURI().toURL().toString();
            // Pass the Monaco "vs" base to the page; it loads loader.js from there.
            return html + "?vs=" + URLEncoder.encode(vs, "UTF-8");
        } catch (MalformedURLException | UnsupportedEncodingException e) {
            return "about:blank";
        }
    }

    private static String quoteJs(String value) {
        StringBuilder builder = new StringBuilder(value.length() + 2);
        builder.append('"');
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            switch (c) {
                case '\\':
                    builder.append("\\\\");
                    break;
                case '"':
                    builder.append("\\\"");
                    break;
                case '\n':
                    builder.append("\\n");
                    break;
                case '\r':
                    builder.append("\\r");
                    break;
                case '\t':
                    builder.append("\\t");
                    break;
                default:
                    if (c < 0x20) {
                        builder.append(String.format("\\u%04x", (int) c));
                    } else {
                        builder.append(c);
                    }
            }
        }
        builder.append('"');
        return builder.toString();
    }
}
