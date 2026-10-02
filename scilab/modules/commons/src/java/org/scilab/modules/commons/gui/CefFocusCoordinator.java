/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.gui;

import java.awt.AWTEvent;
import java.awt.Canvas;
import java.awt.Component;
import java.awt.KeyboardFocusManager;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.MouseEvent;
import java.beans.PropertyChangeListener;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;

import javax.swing.SwingUtilities;
import javax.swing.Timer;
import javax.swing.plaf.basic.BasicSplitPaneDivider;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.props.DockablePropertySet;
import org.flexdock.view.View;
import org.scilab.modules.commons.OS;

/**
 * Single owner of the keyboard focus of the JCEF browsers (ScilabCefPanel).
 *
 * <p>Four states must agree: the Swing focus owner, the page Chromium gives
 * the keyboard to, the dock FlexDock shows active, and the title bar mark
 * ({@link ScilabCefPanel#CEF_NATIVE_FOCUS_PROPERTY}). Chromium reports a focus
 * gain (onGotFocus) but never a loss: a click in another browser, in a Swing
 * component or in another application leaves the previous page focused, and
 * on Windows CefBrowser.setFocus(false) does not blur it. The state is
 * therefore kept here for all the panels at once: at most one panel holds the
 * focus, and its dock is the one marked active.
 */
final class CefFocusCoordinator {

    /**
     * Delay (ms) before an application without active window drops the
     * browser focus: going from a Scilab window to another one also goes
     * through no active window for a moment.
     */
    private static final int APP_DEACTIVATION_MS = 200;

    /** Max attempts of the native focus sink before giving up a transfer. */
    private static final int SINK_ATTEMPTS = 10;

    /** Panels not disposed yet. */
    private static final Set<ScilabCefPanel> panels = ConcurrentHashMap.newKeySet();

    /** The panel whose browser holds the native focus, null when none. */
    private static volatile ScilabCefPanel focused = null;

    /**
     * Incremented on each focus change or request: a focus transfer or a
     * dock activation posted before it is then cancelled.
     */
    private static final AtomicInteger generation = new AtomicInteger();

    /** True between a focus gain and the dock activation it posted. */
    private static volatile boolean activationPending = false;

    /** The view painted active for the focused browser (written on the EDT). */
    private static volatile View markedView = null;

    /** True once the global listeners are installed. */
    private static boolean listening = false;

    /**
     * True on Windows. Elsewhere (X11), the Java window loses its activation
     * as soon as the native window of a browser holds the keyboard, and
     * FlexDock then deactivates the dock of that browser: neither can tell
     * the focus there, the pages report it themselves (pageFocusChanged).
     */
    static final boolean WINDOWS = OS.get() == OS.WINDOWS;

    /**
     * Listens to the marked view: once FlexDock deactivates it because
     * another dock became the active one (e.g. activated while Scilab
     * starts, without any browser getting the focus), keeping the mark
     * painted two title bars active.
     * On X11 FlexDock also deactivates the dock when its window loses the
     * activation, which happens as soon as the native window of the browser
     * holds the keyboard: that alone must not drop the focus.
     */
    private static final PropertyChangeListener MARKED_VIEW_DEACTIVATION = e -> {
        if (DockablePropertySet.ACTIVE.equals(e.getPropertyName()) && Boolean.FALSE.equals(e.getNewValue())) {
            final ScilabCefPanel panel = focused;
            final View view = markedView;
            if (WINDOWS) {
                if (panel != null) {
                    lost(panel);
                    handOverNativeFocus(panel);
                }
                return;
            }
            if (panel == null || view == null) {
                return;
            }
            // FlexDock deactivates the old dock before activating the new one
            SwingUtilities.invokeLater(() -> {
                Dockable active = ActiveDockableTracker.getActiveDockable(view);
                if (focused == panel && active != null && active != view) {
                    lost(panel);
                    handOverNativeFocus(panel);
                }
            });
        }
    };

    /**
     * Delay (ms) a page may stay blurred before its browser is no longer seen
     * focused: on X11 the native windows of Java and Chromium exchange the
     * keyboard several times in a row while a browser takes it.
     */
    private static final int PAGE_BLUR_MS = 200;

    /** Drops the focus of a browser whose page stayed blurred (EDT only). */
    private static Timer pageBlurCheck;

    private CefFocusCoordinator() { }

    static void register(ScilabCefPanel panel) {
        panels.add(panel);
        listen();
    }

    static void unregister(ScilabCefPanel panel) {
        panels.remove(panel);
        lost(panel);
    }

    /** @return true if the browser of this panel holds the native focus */
    static boolean hasFocus(ScilabCefPanel panel) {
        return focused == panel;
    }

    /** A new focus request: cancels a transfer or an activation still pending. */
    static void newRequest() {
        generation.incrementAndGet();
    }

    /**
     * The browser of this panel got the native focus: the other browsers lost
     * it without being told (a click goes straight to Chromium), and its dock
     * becomes the active one (title bar, menus, ...).
     * Called from the CEF threads (onGotFocus) and the EDT (clicks).
     */
    static void gained(ScilabCefPanel panel) {
        for (ScilabCefPanel other : panels) {
            if (other != panel) {
                other.focusRequestDone();
            }
        }
        ScilabCefPanel previous = focused;
        focused = panel;
        panel.focusRequestDone();

        View view = markedView;
        boolean activated = WINDOWS ? view != null && view.isActive() : view != null;
        if (previous == panel && (activationPending || activated)) {
            /*
             * Chromium can re-fire onGotFocus for a browser that already
             * has focus (observed in bursts of 50+ calls within a few ms
             * right after requestBrowserFocus(), one new native thread
             * per call). Re-running the FlexDock activation below on
             * every one of those calls requests focus again on this same
             * component, which is what sustains the burst. Only the
             * first, genuine gained-focus transition needs it.
             * On Windows being focused is not enough: the state can outlive
             * the activation of its dock (another dock activated without any
             * browser taking the focus, e.g. at startup), and the next click
             * in the browser then left its dock inactive.
             * On X11 whether its dock is FlexDock's active one does not tell:
             * FlexDock deactivates it as soon as Chromium holds the keyboard,
             * and activating it again on each call started an activation
             * ping-pong between Java and Chromium. A dock activated meanwhile
             * drops the focus there (MARKED_VIEW_DEACTIVATION).
             */
            return;
        }
        if (previous != panel) {
            unmark();
        }

        activationPending = true;
        final int expected = generation.incrementAndGet();
        // Notify FlexDock so it updates title bars, menus, etc.
        SwingUtilities.invokeLater(() -> {
            activationPending = false;
            if (focused != panel || expected != generation.get()) {
                return;
            }

            View dock = (View) SwingUtilities.getAncestorOfClass(View.class, panel);
            if (dock != null) {
                mark(dock);
                // On X11, asking the Swing focus for the dock makes the Java
                // window take the keyboard back from Chromium, and restore its
                // former focus owner, possibly in another browser dock: only
                // the FlexDock state is changed there (below).
                if (OS.get() == OS.WINDOWS) {
                    ActiveDockableTracker.requestDockableActivation(dock);
                }
                // FlexDock activates the dock once the component it focuses
                // gains the Swing focus: never when that component has it
                // already (the focus stayed there while another dock was
                // activated, e.g. at startup).
                if (!dock.isActive() && dock.isShowing()) {
                    ActiveDockableTracker.getTracker(dock).setActive(dock);
                }
            }
        });
    }

    /**
     * The browser of this panel lost the focus (Tab out of the page, focus
     * released, panel disposed, ...): drop its state and cancel a pending
     * transfer, so that no stale state makes it take the focus back later.
     */
    static void lost(ScilabCefPanel panel) {
        panel.focusRequestDone();
        if (focused == panel) {
            focused = null;
            generation.incrementAndGet();
            unmark();
        }
    }

    /**
     * The page of this panel reports its own focus (window focus/blur): the
     * keyboard left it for another application, a Swing component or another
     * page. This is Chromium's own view, the only reliable one on X11 where
     * the Java window is inactive while a browser holds the keyboard.
     * Not on Windows (see ScilabCefPanel, which installs the reporting).
     * Called from the CEF threads.
     */
    static void pageFocusChanged(ScilabCefPanel panel, boolean pageFocused) {
        panel.setPageFocused(pageFocused);
        if (pageFocused && focused != panel) {
            // On X11 Chromium does not always report the focus going from
            // one of its pages to another (onGotFocus): the page does
            gained(panel);
            return;
        }
        if (!pageFocused && focused == panel) {
            SwingUtilities.invokeLater(() -> {
                if (pageBlurCheck == null) {
                    pageBlurCheck = new Timer(PAGE_BLUR_MS, e -> {
                        ScilabCefPanel current = focused;
                        if (current != null && !current.isPageFocused()) {
                            lost(current);
                        }
                    });
                    pageBlurCheck.setRepeats(false);
                }
                pageBlurCheck.restart();
            });
        }
    }

    /**
     * Remove the title bar mark of this panel while its browser is hidden
     * (resize masking); its next focus gain marks it again.
     */
    static void hidden(ScilabCefPanel panel) {
        if (focused == panel) {
            unmark();
        }
    }

    // ====================================================================
    // Global listeners
    // ====================================================================

    private static synchronized void listen() {
        if (listening) {
            return;
        }
        listening = true;

        Toolkit.getDefaultToolkit().addAWTEventListener(event -> {
            if (event.getID() == MouseEvent.MOUSE_PRESSED) {
                mousePressed((MouseEvent) event);
            }
        }, AWTEvent.MOUSE_EVENT_MASK);

        // Chromium does not report losing the focus to another application:
        // the browser which had it kept its dock title bar painted active, and
        // a second dock became active when clicking back into Scilab. Once no
        // Scilab window is active, no browser holds the focus anymore: coming
        // back, Chromium reports the focus of the browser which gets it.
        // On Windows only: on X11 the Java window is inactive whenever a
        // browser holds the keyboard; the page blur (pageFocusChanged) tells
        // there, and on Windows too when the page does blur.
        if (OS.get() != OS.WINDOWS) {
            return;
        }
        final KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        final Timer check = new Timer(APP_DEACTIVATION_MS, e -> {
            ScilabCefPanel panel = focused;
            if (manager.getActiveWindow() == null && panel != null) {
                lost(panel);
            }
        });
        check.setRepeats(false);
        manager.addPropertyChangeListener("activeWindow", e -> {
            if (e.getNewValue() == null) {
                check.restart();
            }
        });
    }

    private static void mousePressed(MouseEvent me) {
        Component src = me.getComponent();
        if (src == null) {
            return;
        }

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

        ScilabCefPanel current = focused;
        ScilabCefPanel clicked = (ScilabCefPanel) (target instanceof ScilabCefPanel
                                 ? target : SwingUtilities.getAncestorOfClass(ScilabCefPanel.class, target));
        if (clicked != null && panels.contains(clicked) && clicked.isShowing()) {
            if (current != null && current != clicked) {
                // Chromium moves the native focus to the clicked browser by
                // itself. Taking it back through the sink to hand it over
                // leaves Chromium unable to ever blur the page it lands on
                // (two pages then show a caret), so only drop ours.
                current.blurBrowser();
            }
            // A click in a browser gives it the focus. Chromium does not
            // report it when its page kept the focus while our state was
            // dropped (e.g. another dock activated at startup).
            View view = markedView;
            boolean unmarked = WINDOWS ? view == null || !view.isActive() : view == null;
            if (current != clicked || unmarked) {
                gained(clicked);
            }
            return;
        }

        if (current == null || !current.isShowing()) {
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
        if (target == null || target instanceof Window) {
            return;
        }

        // A click on another component: release Chromium focus and give it
        // to the target.
        lost(current);
        current.blurBrowser();
        // Only steal native focus when both components are in the
        // same window. For separate windows the OS handles it.
        if (SwingUtilities.getWindowAncestor(target) == SwingUtilities.getWindowAncestor(current)) {
            transfer(current.getNativeFocusSink(), target, generation.get());
        }
    }

    /**
     * FlexDock activated another dock while the browser of this panel kept
     * the native focus: the dock became active without a click (e.g.
     * browsevar() shows the variable browser), but Chromium still gets the
     * keyboard and shows its caret. Give the focus to that dock: a Swing dock
     * gets the native focus as for a click on it, a dock with a browser gets
     * the Swing focus, which its content hands over to its browser.
     */
    private static void handOverNativeFocus(final ScilabCefPanel panel) {
        final int expected = generation.get();
        SwingUtilities.invokeLater(() -> {
            if (focused != null || expected != generation.get()) {
                return;
            }
            KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
            // No active window: Scilab lost the focus, nothing to hand over
            if (manager.getActiveWindow() == null) {
                return;
            }
            // The dock FlexDock activated: the Swing focus is still moving
            // into it, the focus owner can be null or the dock itself.
            Dockable active = ActiveDockableTracker.getActiveDockable(panel);
            if (!(active instanceof View)) {
                return;
            }
            View dock = (View) active;
            if (SwingUtilities.getWindowAncestor(dock) != SwingUtilities.getWindowAncestor(panel)) {
                return;
            }
            if (holdsBrowser(dock)) {
                // Its content hands the focus over to its browser, which takes
                // the native focus from Chromium by itself
                dock.requestFocusInWindow();
                return;
            }
            Component owner = manager.getFocusOwner();
            Component target = owner != null && SwingUtilities.isDescendingFrom(owner, dock) ? owner : dock;
            transfer(panel.getNativeFocusSink(), target, expected);
        });
    }

    /** @return true if a browser panel is shown in this dock */
    private static boolean holdsBrowser(View dock) {
        for (ScilabCefPanel panel : panels) {
            if (SwingUtilities.isDescendingFrom(panel, dock)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Give the native focus to a Swing component of the window: Chromium's
     * native window keeps it otherwise, a lightweight component cannot take
     * it. The heavyweight sink takes it first, then hands it over.
     */
    private static void transfer(final Canvas sink, final Component target, final int expected) {
        final class Sink implements Runnable {
            private int attempts = 0;

            @Override
            public void run() {
                if (focused != null || expected != generation.get()) {
                    return;
                }
                if (sink.hasFocus()) {
                    target.requestFocusInWindow();
                } else if (++attempts < SINK_ATTEMPTS) {
                    // If the focus sink failed, try again after the next repaint cycle.
                    sink.requestFocus();
                    SwingUtilities.invokeLater(this);
                }
            }
        }
        sink.requestFocus();
        SwingUtilities.invokeLater(new Sink());
    }

    // ====================================================================
    // Title bar mark
    // ====================================================================

    private static void mark(View view) {
        if (markedView == view) {
            repaintTitlebar(view);
            return;
        }
        unmark();
        markedView = view;
        view.putClientProperty(ScilabCefPanel.CEF_NATIVE_FOCUS_PROPERTY, Boolean.TRUE);
        if (view.getDockingProperties() != null) {
            view.getDockingProperties().addPropertyChangeListener(MARKED_VIEW_DEACTIVATION);
        }
        repaintTitlebar(view);
    }

    private static void unmark() {
        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(CefFocusCoordinator::unmark);
            return;
        }
        View view = markedView;
        if (view == null) {
            return;
        }
        markedView = null;
        view.putClientProperty(ScilabCefPanel.CEF_NATIVE_FOCUS_PROPERTY, null);
        if (view.getDockingProperties() != null) {
            view.getDockingProperties().removePropertyChangeListener(MARKED_VIEW_DEACTIVATION);
        }
        repaintTitlebar(view);
    }

    private static void repaintTitlebar(View view) {
        if (view.getTitlebar() != null) {
            view.getTitlebar().repaint();
        }
    }
}
