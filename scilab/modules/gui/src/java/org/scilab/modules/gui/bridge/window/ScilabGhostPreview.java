/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.gui.bridge.window;

import java.awt.AlphaComposite;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.util.Map;

import javax.swing.JComponent;
import javax.swing.JWindow;
import javax.swing.SwingUtilities;
import javax.swing.Timer;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingPort;
import org.flexdock.docking.drag.DragManager;
import org.flexdock.docking.drag.effects.DefaultPreview;
import org.scilab.modules.commons.gui.ScilabCefPanel;

/**
 * Drop preview showing the dragged panel laid out at the size of the drop
 * region, like FlexDock's GhostPreview, with two changes forced by the console
 * being a heavyweight JCEF browser.
 *
 * GhostPreview resizes the live component to the preview polygon and calls
 * paintAll(), which validates it; its restoring setSize() only invalidates, so
 * the panel stays laid out at the drop-region size until some later pass. The
 * freed area then shows the docking port's background. Restoring the layout
 * synchronously keeps that off screen.
 *
 * A JCEF browser paints nothing through paintAll() and its native window
 * cannot be re-laid out for a preview, so a panel holding one is captured at
 * its own size from its cached page snapshot and drawn unscaled: a console
 * that changes size shows more or fewer lines, it does not magnify them.
 *
 * The preview is painted by an always-on-top window rather than on
 * FlexDock's glasspane, which is lightweight and would be hidden by the
 * browser's native window over the console area.
 */
public class ScilabGhostPreview extends DefaultPreview {

    private static final String PREVIEW_IMG = ScilabGhostPreview.class.getName() + ".previewImage";
    private static final String PREVIOUS_BOUNDS = ScilabGhostPreview.class.getName() + ".previousBounds";
    private static final String PAINT_TARGET = ScilabGhostPreview.class.getName() + ".paintTarget";
    private static final float PREVIEW_ALPHA = 0.55f;
    /** How often the end of a drag is checked: DragPreview has no such callback. */
    private static final int WATCHDOG_MS = 120;

    private JWindow previewWindow;
    private GhostPane ghostPane;
    private Timer watchdog;
    private boolean perPixelAlpha;

    /** The capture is redone only when the drop region changes, as GhostPreview does. */
    @Override
    public Polygon createPreviewPolygon(Component dragged, DockingPort port, Dockable dockable,
                                        String targetRegion, Component paintingTarget, Map dragInfo) {
        Polygon poly = super.createPreviewPolygon(dragged, port, dockable, targetRegion, paintingTarget, dragInfo);
        if (poly == null) {
            hidePreview();
            return null;
        }
        if (dragInfo != null) {
            dragInfo.put(PAINT_TARGET, paintingTarget);
            Rectangle bounds = poly.getBounds();
            if (!bounds.equals(dragInfo.get(PREVIOUS_BOUNDS))) {
                dragInfo.put(PREVIOUS_BOUNDS, bounds);
                dragInfo.put(PREVIEW_IMG, capture(dragged, bounds.getSize()));
            }
        }
        return poly;
    }

    /** The glasspane graphics is left untouched: everything goes to the window. */
    @Override
    public void drawPreview(Graphics2D g, Polygon poly, Dockable dockable, Map dragInfo) {
        Object img = dragInfo == null ? null : dragInfo.get(PREVIEW_IMG);
        Object target = dragInfo == null ? null : dragInfo.get(PAINT_TARGET);
        if (!(target instanceof Component)) {
            return;
        }
        showPreview(poly.getBounds(), (Component) target,
                    img instanceof BufferedImage ? (BufferedImage) img : null);
    }

    /** Same tab-region geometry as GhostPreview. */
    @Override
    protected Rectangle createTabbedPaneRect(DockingPort port, Component comp) {
        return comp != null ? comp.getBounds() : super.createTabbedPaneRect(port, comp);
    }

    // ====================================================================
    // capture
    // ====================================================================

    /** @return the dragged panel rendered at {@code size}, or null. */
    private static BufferedImage capture(Component component, Dimension size) {
        if (component == null || size.width <= 0 || size.height <= 0
                || component.getWidth() <= 0 || component.getHeight() <= 0) {
            return null;
        }
        return containsBrowser(component) ? captureCropped(component, size)
                                          : captureRelaidOut(component, size);
    }

    /**
     * Lay the panel out at the drop-region size, paint it, and put its layout
     * back before returning: the live component must never be left mis-sized.
     */
    private static BufferedImage captureRelaidOut(Component component, Dimension size) {
        BufferedImage img = new BufferedImage(size.width, size.height, BufferedImage.TYPE_INT_RGB);
        Dimension original = component.getSize();
        Graphics2D g = img.createGraphics();
        try {
            component.setSize(size);
            component.validate();
            component.paintAll(g);
        } finally {
            g.dispose();
            component.setSize(original);
            // setSize only invalidates; without this the panel would stay laid
            // out at the drop-region size for the rest of the gesture.
            component.validate();
        }
        return img;
    }

    /**
     * A browser cannot be re-laid out for a preview, so paint the panel at its
     * own size and crop it into the region, top left, without scaling. This is
     * what resizing a console actually does: the text keeps its size and the
     * region shows more or less of it, over the page background.
     */
    private static BufferedImage captureCropped(Component component, Dimension size) {
        BufferedImage natural = new BufferedImage(component.getWidth(), component.getHeight(),
                                                  BufferedImage.TYPE_INT_RGB);
        Graphics2D g = natural.createGraphics();
        try {
            component.paintAll(g);
            paintBrowserSnapshots(component, component, g);
        } finally {
            g.dispose();
        }

        BufferedImage img = new BufferedImage(size.width, size.height, BufferedImage.TYPE_INT_RGB);
        Graphics2D out = img.createGraphics();
        try {
            out.setColor(pageBackground(component));
            out.fillRect(0, 0, size.width, size.height);
            out.drawImage(natural, 0, 0, null);
        } finally {
            out.dispose();
        }
        return img;
    }

    /** @return the browser page's own background, so a wider region blends in. */
    private static Color pageBackground(Component c) {
        BufferedImage snapshot = firstBrowserSnapshot(c);
        return snapshot == null ? Color.WHITE : new Color(snapshot.getRGB(0, 0));
    }

    private static BufferedImage firstBrowserSnapshot(Component c) {
        if (c instanceof ScilabCefPanel) {
            BufferedImage snapshot = ((ScilabCefPanel) c).getPageSnapshot();
            return snapshot != null && snapshot.getWidth() > 0 ? snapshot : null;
        }
        if (c instanceof Container) {
            for (Component child : ((Container) c).getComponents()) {
                BufferedImage snapshot = firstBrowserSnapshot(child);
                if (snapshot != null) {
                    return snapshot;
                }
            }
        }
        return null;
    }

    /** Fill in the areas paintAll() leaves empty: the heavyweight browsers. */
    private static void paintBrowserSnapshots(Component root, Component c, Graphics2D g) {
        if (c instanceof ScilabCefPanel) {
            BufferedImage snapshot = ((ScilabCefPanel) c).getPageSnapshot();
            if (snapshot != null && c.getWidth() > 0 && c.getHeight() > 0) {
                Point origin = SwingUtilities.convertPoint(c, 0, 0, root);
                g.drawImage(snapshot, origin.x, origin.y, c.getWidth(), c.getHeight(), null);
            }
            return;
        }
        if (c instanceof Container) {
            for (Component child : ((Container) c).getComponents()) {
                if (child.isVisible()) {
                    paintBrowserSnapshots(root, child, g);
                }
            }
        }
    }

    private static boolean containsBrowser(Component c) {
        if (c instanceof ScilabCefPanel) {
            return true;
        }
        if (c instanceof Container) {
            for (Component child : ((Container) c).getComponents()) {
                if (containsBrowser(child)) {
                    return true;
                }
            }
        }
        return false;
    }

    // ====================================================================
    // preview window
    // ====================================================================

    private void showPreview(Rectangle bounds, Component target, BufferedImage image) {
        if (!target.isShowing() || bounds.width <= 0 || bounds.height <= 0) {
            hidePreview();
            return;
        }
        Point origin = new Point(bounds.x, bounds.y);
        SwingUtilities.convertPointToScreen(origin, target);

        JWindow window = previewWindow();
        ghostPane.setImage(image);
        window.setBounds(origin.x, origin.y, bounds.width, bounds.height);
        if (!window.isVisible()) {
            window.setVisible(true);
            watchdog().start();
        }
        ghostPane.repaint();
    }

    private void hidePreview() {
        if (watchdog != null) {
            watchdog.stop();
        }
        if (previewWindow != null && previewWindow.isVisible()) {
            previewWindow.setVisible(false);
        }
    }

    private JWindow previewWindow() {
        if (previewWindow == null) {
            perPixelAlpha = isTranslucencySupported();
            ghostPane = new GhostPane();
            previewWindow = new JWindow();
            previewWindow.setFocusableWindowState(false);
            previewWindow.setContentPane(ghostPane);
            try {
                previewWindow.setAlwaysOnTop(true);
            } catch (SecurityException e) {
                // stays behind the browser, no worse than the glasspane
            }
            if (perPixelAlpha) {
                previewWindow.setBackground(new Color(0, 0, 0, 0));
            } else {
                previewWindow.setOpacity(PREVIEW_ALPHA);
            }
        }
        return previewWindow;
    }

    /**
     * DragPreview has no end-of-drag callback, and the window must not outlive
     * the gesture, so the drag state is polled while it is up.
     */
    private Timer watchdog() {
        if (watchdog == null) {
            watchdog = new Timer(WATCHDOG_MS, e -> {
                if (DragManager.getCurrentDragOperation() == null) {
                    hidePreview();
                }
            });
            watchdog.setRepeats(true);
        }
        return watchdog;
    }

    private static boolean isTranslucencySupported() {
        try {
            GraphicsDevice device = GraphicsEnvironment.getLocalGraphicsEnvironment().getDefaultScreenDevice();
            return device.isWindowTranslucencySupported(GraphicsDevice.WindowTranslucency.PERPIXEL_TRANSLUCENT);
        } catch (Exception e) {
            return false;
        }
    }

    /** Content of the preview window: the ghost, already at the region size. */
    private class GhostPane extends JComponent {
        private static final long serialVersionUID = 1L;
        private transient BufferedImage image;

        GhostPane() {
            setOpaque(false);
        }

        void setImage(BufferedImage image) {
            this.image = image;
        }

        @Override
        protected void paintComponent(Graphics g) {
            Graphics2D g2 = (Graphics2D) g.create();
            try {
                if (image != null) {
                    if (perPixelAlpha) {
                        g2.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER, PREVIEW_ALPHA));
                    }
                    g2.drawImage(image, 0, 0, getWidth(), getHeight(), null);
                    g2.setComposite(AlphaComposite.SrcOver);
                }
                g2.setColor(Color.BLACK);
                g2.drawRect(0, 0, getWidth() - 1, getHeight() - 1);
            } finally {
                g2.dispose();
            }
        }
    }
}
