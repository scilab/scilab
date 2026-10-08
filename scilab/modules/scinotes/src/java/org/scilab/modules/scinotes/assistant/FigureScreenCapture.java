package org.scilab.modules.scinotes.assistant;

import java.awt.AWTException;
import java.awt.Component;
import java.awt.Frame;
import java.awt.GraphicsEnvironment;
import java.awt.Image;
import java.awt.KeyboardFocusManager;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Window;
import java.awt.image.BufferedImage;
import java.awt.image.MultiResolutionImage;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.Base64;
import java.util.List;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.SwingUtilities;

import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

/**
 * Screenshot of a figure window as it is displayed, uicontrols and embedded
 * browser pages included. FileExporter renders the graphic entities only,
 * and painting the Swing tree into an image does not work for the native
 * JCEF and JoGL surfaces: only a screen capture shows what the user sees.
 *
 * The window is made visible, de-iconified and put on top for the capture,
 * then its visibility, state, always-on-top flag and the previously active
 * window are restored.
 */
final class FigureScreenCapture {

    private static final Logger LOG = Logger.getLogger(FigureScreenCapture.class.getName());

    /** Time left to JCEF/JoGL to repaint once the window is on top. */
    private static final long REPAINT_DELAY_MS = 400;

    private FigureScreenCapture() {
    }

    /** Window state saved before the capture, restored afterwards. */
    private static final class Saved {
        SwingScilabWindow window;
        Component panel;
        boolean visible;
        int extendedState;
        boolean alwaysOnTop;
        Window previousActive;
    }

    /**
     * @param figUID the figure UID
     * @return "![figure](data:image/png;base64,...)", or null when the
     *         figure cannot be captured on screen (caller falls back)
     */
    static String capture(int figUID) {
        if (GraphicsEnvironment.isHeadless()) {
            return null;
        }
        Saved saved = null;
        try {
            saved = bringToFront(figUID);
            if (saved == null) {
                return null;
            }
            Thread.sleep(REPAINT_DELAY_MS);

            Rectangle area = onScreenArea(saved.panel);
            if (area == null) {
                return null;
            }
            BufferedImage img = grab(area);
            if (img == null || isBlank(img)) {
                // Locked session, minimized RDP...: the screen holds nothing useful
                return null;
            }
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            ImageIO.write(img, "png", out);
            return "![figure](data:image/png;base64,"
                + Base64.getEncoder().encodeToString(out.toByteArray()) + ")";
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        } catch (AWTException | IOException | InvocationTargetException | RuntimeException e) {
            LOG.warning("Figure screen capture failed: " + e);
            return null;
        } finally {
            if (saved != null) {
                restore(saved);
            }
        }
    }

    private static Saved bringToFront(int figUID) throws InterruptedException, InvocationTargetException {
        Saved[] result = new Saved[1];
        SwingUtilities.invokeAndWait(() -> {
            SwingViewObject view = SwingView.getFromId(figUID);
            if (!(view instanceof SwingScilabPanel) || !(view instanceof Component)) {
                return;
            }
            SwingScilabWindow window = ((SwingScilabPanel) view).getParentWindow();
            if (window == null) {
                return;
            }
            Saved s = new Saved();
            s.window = window;
            s.panel = (Component) view;
            s.visible = window.isVisible();
            s.extendedState = window.getExtendedState();
            s.alwaysOnTop = window.isAlwaysOnTop();
            s.previousActive = KeyboardFocusManager.getCurrentKeyboardFocusManager().getActiveWindow();

            if (!s.visible) {
                window.setVisible(true);
            }
            if ((s.extendedState & Frame.ICONIFIED) != 0) {
                window.setExtendedState(s.extendedState & ~Frame.ICONIFIED);
            }
            // toFront() alone does not beat other applications on Windows
            window.setAlwaysOnTop(true);
            window.toFront();
            result[0] = s;
        });
        return result[0];
    }

    /** Panel bounds on screen, clipped to the screens; null if not showing. */
    private static Rectangle onScreenArea(Component panel) throws InterruptedException, InvocationTargetException {
        Rectangle[] result = new Rectangle[1];
        SwingUtilities.invokeAndWait(() -> {
            // A figure docked in a non-selected tab is not showing
            if (!panel.isShowing() || panel.getWidth() <= 0 || panel.getHeight() <= 0) {
                return;
            }
            Point p = panel.getLocationOnScreen();
            Rectangle r = new Rectangle(p.x, p.y, panel.getWidth(), panel.getHeight());
            Rectangle screens = new Rectangle();
            for (java.awt.GraphicsDevice d : GraphicsEnvironment.getLocalGraphicsEnvironment().getScreenDevices()) {
                screens = screens.union(d.getDefaultConfiguration().getBounds());
            }
            r = r.intersection(screens);
            if (!r.isEmpty()) {
                result[0] = r;
            }
        });
        return result[0];
    }

    /** Full-resolution capture (HiDPI screens return several variants). */
    private static BufferedImage grab(Rectangle area) throws AWTException {
        MultiResolutionImage multi = new Robot().createMultiResolutionScreenCapture(area);
        List<Image> variants = multi.getResolutionVariants();
        Image best = variants.get(variants.size() - 1);
        if (best instanceof BufferedImage) {
            return (BufferedImage) best;
        }
        BufferedImage img = new BufferedImage(best.getWidth(null), best.getHeight(null), BufferedImage.TYPE_INT_RGB);
        img.getGraphics().drawImage(best, 0, 0, null);
        return img;
    }

    /** True when the capture is a single flat color (black screen...). */
    private static boolean isBlank(BufferedImage img) {
        int ref = img.getRGB(0, 0);
        int stepX = Math.max(1, img.getWidth() / 20);
        int stepY = Math.max(1, img.getHeight() / 20);
        for (int y = 0; y < img.getHeight(); y += stepY) {
            for (int x = 0; x < img.getWidth(); x += stepX) {
                if (img.getRGB(x, y) != ref) {
                    return false;
                }
            }
        }
        return true;
    }

    private static void restore(Saved s) {
        SwingUtilities.invokeLater(() -> {
            try {
                s.window.setAlwaysOnTop(s.alwaysOnTop);
                if ((s.extendedState & Frame.ICONIFIED) != 0) {
                    s.window.setExtendedState(s.extendedState);
                }
                if (!s.visible) {
                    s.window.setVisible(false);
                }
                if (s.previousActive != null && s.previousActive != s.window
                        && s.previousActive.isDisplayable()) {
                    s.previousActive.toFront();
                    s.previousActive.requestFocus();
                }
            } catch (RuntimeException e) {
                LOG.warning("Figure window restore failed: " + e);
            }
        });
    }
}
