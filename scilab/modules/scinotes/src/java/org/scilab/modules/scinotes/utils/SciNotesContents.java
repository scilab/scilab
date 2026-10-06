/*
 * Scilab (https://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
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

package org.scilab.modules.scinotes.utils;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Rectangle;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;

import javax.swing.JLayeredPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.gui.ScilabCefPanel;
import org.scilab.modules.scinotes.SciNotes;

/**
 * Content pane for SciNotes.
 * @author Calixte DENIZET
 */
public final class SciNotesContents extends JPanel {
    private static final long serialVersionUID = 1L;

    private ScilabTabbedPane stp;
    private final JLayeredPane editorLayer;
    private Component bottom;
    private ScilabCefPanel browserOverlay;
    private Component browserTarget;
    private Component secondaryBrowserTarget;
    private transient SplitLayoutListener splitLayoutListener;

    /**
     * Notified with the browser overlay's local (overlay-relative) bounds
     * whenever a split layout is recomputed, so the single native browser can
     * position two DOM containers to match the two Swing placeholders it is
     * stretched across. {@code secondaryLocal} is null outside split mode.
     */
    public interface SplitLayoutListener {
        void onSplitLayout(Rectangle primaryLocal, Rectangle secondaryLocal);
    }

    /*
     * The Monaco browser is a single heavyweight window overlaid on the active
     * tab's editor area. When many files are opened at once, each addTab()
     * repositions the overlay before the JTabbedPane has relaid out its tab
     * strip, so the overlay is placed against a stale (one-row) strip height and
     * then covers the extra tab rows -- their clicks never reach the tabs.
     * Re-run the layout whenever the target or one of its ancestors is resized
     * or moved (i.e. once the strip settles).
     */
    private final transient java.awt.event.ComponentAdapter overlayTargetResized =
        new java.awt.event.ComponentAdapter() {
            public void componentResized(java.awt.event.ComponentEvent e) {
                layoutBrowserOverlay();
            }
            public void componentMoved(java.awt.event.ComponentEvent e) {
                layoutBrowserOverlay();
            }
        };
    private final transient java.awt.event.HierarchyBoundsListener overlayAncestorMoved =
        new java.awt.event.HierarchyBoundsListener() {
            public void ancestorMoved(java.awt.event.HierarchyEvent e) {
                layoutBrowserOverlay();
            }
            public void ancestorResized(java.awt.event.HierarchyEvent e) {
                layoutBrowserOverlay();
            }
        };

    /**
     * Default constructor
     * @param editor the editor where to put this pane
     */
    public SciNotesContents(final SciNotes editor) {
        super(new BorderLayout());
        stp = new ScilabTabbedPane(editor);
        editorLayer = new JLayeredPane() {
            @Override
            public void doLayout() {
                stp.setBounds(0, 0, getWidth(), getHeight());
                layoutBrowserOverlay();
            }

            @Override
            public Dimension getPreferredSize() {
                return stp.getPreferredSize();
            }

            @Override
            public Dimension getMinimumSize() {
                return stp.getMinimumSize();
            }
        };
        editorLayer.add(stp, JLayeredPane.DEFAULT_LAYER);
        add(editorLayer, BorderLayout.CENTER);
        addFocusListener(new FocusListener() {
            public void focusGained(FocusEvent e) {
                if (editor.getTextPane() != null) {
                    editor.getTextPane().requestFocus();
                }
            }
            public void focusLost(FocusEvent e) { }
        });
    }

    /**
     * @return the tabbed pane used in this content pane
     */
    public ScilabTabbedPane getScilabTabbedPane() {
        return stp;
    }

    /**
     * Display the window-owned native browser over the selected tab host.
     * The browser remains attached to this stable layer when tabs change;
     * only its bounds and active Monaco model are updated.
     *
     * @param browser the browser shared by this SciNotes window
     * @param target placeholder defining the editor bounds of the active tab
     */
    public void showBrowserOverlay(ScilabCefPanel browser, Component target) {
        showBrowserOverlaySplit(browser, target, null, null);
    }

    /**
     * Display the window-owned native browser stretched over two tab hosts
     * side by side (or stacked), e.g. the two halves of a split Monaco tab.
     * There is only one native browser per window, so it cannot literally
     * occupy two disjoint areas: its bounds become the smallest rectangle
     * covering both targets, and {@code listener} is told the two targets'
     * position within that rectangle so the page can place two DOM
     * containers to match.
     *
     * @param browser the browser shared by this SciNotes window
     * @param target placeholder defining the editor bounds of the primary half
     * @param secondaryTarget placeholder for the other half, or null outside split mode
     * @param listener notified with the local bounds of both halves on every relayout
     */
    public void showBrowserOverlaySplit(ScilabCefPanel browser, Component target,
            Component secondaryTarget, SplitLayoutListener listener) {
        if (browserOverlay != browser) {
            /*
             * A stale overlay here belongs to a SharedBrowser whose last
             * pane was just closed (e.g. closing a window's last named tab,
             * which immediately reopens a blank one and constructs a new
             * SharedBrowser before the old one's own deferred cleanup runs).
             * Removing it outside setDockInProgress would let
             * ScilabCefPanel.removeNotify() close the still-disposing native
             * browser a second time, racing its owner's own teardown.
             */
            ScilabCefPanel.setDockInProgress(true);
            try {
                if (browserOverlay != null && browserOverlay.getParent() == editorLayer) {
                    editorLayer.remove(browserOverlay);
                }
                browserOverlay = browser;
                if (browser.getParent() != editorLayer) {
                    if (browser.getParent() != null) {
                        browser.getParent().remove(browser);
                    }
                    editorLayer.add(browser, JLayeredPane.PALETTE_LAYER);
                }
            } finally {
                ScilabCefPanel.setDockInProgress(false);
            }
        }
        if (browserTarget != target) {
            if (browserTarget != null) {
                browserTarget.removeComponentListener(overlayTargetResized);
                browserTarget.removeHierarchyBoundsListener(overlayAncestorMoved);
            }
            browserTarget = target;
            if (browserTarget != null) {
                browserTarget.addComponentListener(overlayTargetResized);
                browserTarget.addHierarchyBoundsListener(overlayAncestorMoved);
            }
        }
        if (secondaryBrowserTarget != secondaryTarget) {
            if (secondaryBrowserTarget != null) {
                secondaryBrowserTarget.removeComponentListener(overlayTargetResized);
                secondaryBrowserTarget.removeHierarchyBoundsListener(overlayAncestorMoved);
            }
            secondaryBrowserTarget = secondaryTarget;
            if (secondaryBrowserTarget != null) {
                secondaryBrowserTarget.addComponentListener(overlayTargetResized);
                secondaryBrowserTarget.addHierarchyBoundsListener(overlayAncestorMoved);
            }
        }
        splitLayoutListener = listener;
        editorLayer.revalidate();
        editorLayer.repaint();
        layoutBrowserOverlay();
        SwingUtilities.invokeLater(this::layoutBrowserOverlay);
    }

    /**
     * Remove a browser which has already been disposed by its owner.
     *
     * @param browser browser to detach from the stable layer
     */
    public void removeBrowserOverlay(ScilabCefPanel browser) {
        if (browserOverlay != browser) {
            return;
        }
        if (browserTarget != null) {
            browserTarget.removeComponentListener(overlayTargetResized);
            browserTarget.removeHierarchyBoundsListener(overlayAncestorMoved);
        }
        if (secondaryBrowserTarget != null) {
            secondaryBrowserTarget.removeComponentListener(overlayTargetResized);
            secondaryBrowserTarget.removeHierarchyBoundsListener(overlayAncestorMoved);
        }
        browserTarget = null;
        secondaryBrowserTarget = null;
        splitLayoutListener = null;
        browserOverlay = null;
        if (browser.getParent() == editorLayer) {
            editorLayer.remove(browser);
        }
        editorLayer.revalidate();
        editorLayer.repaint();
    }

    private void layoutBrowserOverlay() {
        // A newly selected JTabbedPane component becomes showing during the
        // next layout pass. Until then, retain Chromium at its previous bounds
        // instead of hiding and showing its native peer.
        if (browserOverlay == null || browserTarget == null
                || browserTarget.getParent() == null || !browserTarget.isShowing()) {
            return;
        }
        Rectangle primary = SwingUtilities.convertRectangle(browserTarget.getParent(),
                browserTarget.getBounds(), editorLayer);
        Rectangle bounds = primary;
        Rectangle primaryLocal = new Rectangle(0, 0, primary.width, primary.height);
        Rectangle secondaryLocal = null;
        if (secondaryBrowserTarget != null && secondaryBrowserTarget.getParent() != null
                && secondaryBrowserTarget.isShowing()) {
            Rectangle secondary = SwingUtilities.convertRectangle(secondaryBrowserTarget.getParent(),
                    secondaryBrowserTarget.getBounds(), editorLayer);
            bounds = primary.union(secondary);
            primaryLocal = new Rectangle(primary.x - bounds.x, primary.y - bounds.y, primary.width, primary.height);
            secondaryLocal = new Rectangle(secondary.x - bounds.x, secondary.y - bounds.y, secondary.width, secondary.height);
        }
        boolean visible = bounds.width > 0 && bounds.height > 0;
        /*
         * A tab just inserted is showing but not laid out yet: empty. Hiding
         * Chromium for that one pass makes it show its last frame, the
         * previous document, once visible again.
         */
        if (!visible && !browserTarget.isValid()) {
            return;
        }
        browserOverlay.setBounds(bounds);
        browserOverlay.setVisible(visible);
        if (visible) {
            browserOverlay.revalidate();
            browserOverlay.repaint();
        }
        if (splitLayoutListener != null) {
            splitLayoutListener.onSplitLayout(primaryLocal, secondaryLocal);
        }
    }

    /**
     * @param comp the component to insert at the bottom of this panel
     */
    public void insertBottomComponent(Component comp) {
        if (comp == null) {
            if (bottom != null) {
                remove(bottom);
            }
        } else {
            bottom = comp;
            add(comp, BorderLayout.PAGE_END);
        }
        revalidate();
    }

    /**
     * @return the component at the bottom
     */
    public Component getBottom() {
        return bottom;
    }
}
