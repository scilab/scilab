/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - Calixte DENIZET
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
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

package org.scilab.modules.gui.utils;

import java.awt.Component;
import java.awt.Container;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JWindow;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.gui.ScilabKeyStroke;

/**
 * Add some utilities related to swing event dispatch thread.
 * @author Jean-Baptiste Silvy
 */
public final class ScilabSwingUtilities {

    /**
     * Should not be called
     */
    protected ScilabSwingUtilities() {
        throw new UnsupportedOperationException();
    }

    /**
     * Add a component to its new parent.
     * This method is thread safe.
     * @param component component to add
     * @param parent parent in which the component will be added
     */

    /*
      public static void addToParent(JComponent component, Container parent) {
      final JComponent componentF = component;
      final Container parentF = parent;
      try {
      SwingUtilities.invokeAndWait(new Runnable() {
      public void run() {
      parentF.add(componentF);
      // repaint to see the changes
      parentF.repaint();
      }
      });
      } catch (InterruptedException e) {
      e.printStackTrace();
      } catch (InvocationTargetException e) {
      e.getCause().printStackTrace();
      }
      }
    */
    /**
     * Remove a component from its parent.
     * This method is thread safe.
     * @param component component to remove
     */
    public static void removeFromParent(Component component) {
        final Component componentF = component;
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    componentF.setVisible(false);
                    Container parent = componentF.getParent();
                    if (parent != null) {
                        parent.remove(componentF);
                        // repaint to see the changes
                        //parent.repaint();
                    }
                }
            });
        } catch (InterruptedException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.getCause().printStackTrace();
        }
    }

    /**
     * Add a component to its new parent.
     * This method is thread safe.
     * @param component component to add
     * @param parent parent in which the component will be added
     * @param constraints an object expressing layout constraints for the parent
     * @param index the position in the container's list at which to insert the component; -1 means insert at the end component
     */
    /*
      public static void addToParent(Component component, Container parent, Object constraints, int index) {
      final Component componentF = component;
      final Container parentF = parent;
      final Object constraintF = constraints;
      final int indexF = index;
      try {
      SwingUtilities.invokeAndWait(new Runnable() {
      public void run() {
      parentF.add(componentF, constraintF, indexF);
      // repaint to see the changes
      parentF.repaint();
      }
      });
      } catch (InterruptedException e) {
      e.printStackTrace();
      } catch (InvocationTargetException e) {
      e.getCause().printStackTrace();
      }
      }
    */
    /**
     * Create a cursor than can be used with any component from an icon.
     * The icon can be any kind of image that can be read with ImageIO (jpg, png, gif, bmp)
     * and is specified by its path.
     * @param imagePath Name of the image file to read
     * @param cursorName name of the cursor for later use
     * @return the Cursor that can be applied to a component.
     */
    public static Cursor createCursorFromIcon(String imagePath, String cursorName) {

        // Open the file specified by the path
        File iconFile = new File(imagePath);

        // create a buffered image from the file
        BufferedImage icon = null;
        try {
            icon = ImageIO.read(iconFile);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }

        // The window system may not be able to use the specified image.
        // Some window system are not able to display cursors with certain size.
        // We then need to resize the image to the compatible dims.
        Dimension compatibleDims = Toolkit.getDefaultToolkit().getBestCursorSize(icon.getWidth(), icon.getHeight());

        // create a new image with a compatible dimentions
        Image compatibleIcon = null;
        if (compatibleDims.width == icon.getWidth() || compatibleDims.height == icon.getHeight()) {
            // no need for resize just use the icon
            compatibleIcon = icon;
        } else if (icon.getWidth() < compatibleDims.width && icon.getHeight() < compatibleDims.height) {
            // the icon is smaller than the compatible size
            // We create a new image with a compatible size and copy the icon on the top left corner.
            // The remaining part of the icon is kept transparent, so the cursor rendering
            // is actually unchanged
            BufferedImage compatibleIconB = new BufferedImage(compatibleDims.width, compatibleDims.height, BufferedImage.TYPE_INT_ARGB);

            // by default set the image to transparent
            for (int i = 0; i < compatibleIconB.getWidth(); i++) {
                for (int j = 0; j < compatibleIconB.getHeight(); j++) {
                    compatibleIconB.setRGB(i, j, 0);
                }
            }

            // then copy the icon in the top left part
            for (int i = 0; i < icon.getWidth(); i++) {
                for (int j = 0; j < icon.getHeight(); j++) {
                    // we're lucky since getRGB return the value as ARGB
                    compatibleIconB.setRGB(i, j, icon.getRGB(i, j));
                }
            }

            compatibleIcon = compatibleIconB;
        } else {
            // the icon is larger than the compatible size
            // we then need to reduce it
            compatibleIcon = icon.getScaledInstance(compatibleDims.width, compatibleDims.height, Image.SCALE_SMOOTH);
        }

        // finally create the Cursor
        // the second parameter is quite obscure, the java doc does not help very much. But using
        // (0, 0) as value works fine so let's use this.
        return Toolkit.getDefaultToolkit().createCustomCursor(compatibleIcon, new Point(0, 0), cursorName);
    }

    /*
     * freedesktop icon lookup mechanism
     * http://standards.freedesktop.org/icon-theme
     * -spec/icon-theme-spec-latest.html#icon_lookup
     */

    /**
     * Add an action to close the window when the ESCAPE key is hit.
     * @param window the window where to add the action, it must be an instance of
     * JFrame or JWindow or JDialog
     */
    public static void closeOnEscape(final Window window) {
        KeyStroke esc = KeyStroke.getKeyStroke(KeyEvent.VK_ESCAPE, 0);
        KeyStroke ctrlw = ScilabKeyStroke.getKeyStroke("OSSCKEY W");

        ActionListener listener = new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                WindowListener[] listeners = window.getWindowListeners();
                for (int i = 0; i < listeners.length; i++) {
                    listeners[i].windowClosing(new WindowEvent(window, WindowEvent.WINDOW_CLOSING));
                }
                window.setVisible(false);
                window.dispose();
            }
        };
        if (window instanceof JFrame) {
            ((JFrame) window).getRootPane().registerKeyboardAction(listener, esc, JComponent.WHEN_IN_FOCUSED_WINDOW);
            ((JFrame) window).getRootPane().registerKeyboardAction(listener, ctrlw, JComponent.WHEN_IN_FOCUSED_WINDOW);
        } else if (window instanceof JWindow) {
            ((JWindow) window).getRootPane().registerKeyboardAction(listener, esc, JComponent.WHEN_IN_FOCUSED_WINDOW);
            ((JWindow) window).getRootPane().registerKeyboardAction(listener, ctrlw, JComponent.WHEN_IN_FOCUSED_WINDOW);
        } else if (window instanceof JDialog) {
            ((JDialog) window).getRootPane().registerKeyboardAction(listener, esc, JComponent.WHEN_IN_FOCUSED_WINDOW);
            ((JDialog) window).getRootPane().registerKeyboardAction(listener, ctrlw, JComponent.WHEN_IN_FOCUSED_WINDOW);
        }
    }
}
