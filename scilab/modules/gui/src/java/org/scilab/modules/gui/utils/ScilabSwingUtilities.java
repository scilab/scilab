/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - Calixte DENIZET
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
     * Look for the icon associated with the name.
     *
     * @param icon
     *            the name to look for
     * @return the image icon path
     */
    public static String findIcon(final String icon) {
        return findIcon(icon, "16x16");
    }

    /**
     * Look for the icon associated with the name for a specific module.
     *
     * @param name
     *            the name to look for
     * @param size
     *            the size to look for
     * @return the loaded image icon
     */
    public static String findIcon(final String icon, final String size) {
        if (icon == null || icon.isEmpty()) {
            return null;
        }

        final String filename = findIconHelper(icon, size, "Tango");
        if (filename != null) {
            if (LOG.isLoggable(Level.FINER)) {
                LOG.finer(icon + '[' + size + ']' + " i " + filename);
            }
            return filename;
        }

        final String fallback = lookupFallbackIcon(icon);
        if (fallback == null) {
            LOG.warning("Unable to found icon: " + icon + '[' + size + ']');
            return System.getenv("SCI") + "/modules/gui/images/icons/16x16/status/error.png";
        }

        if (LOG.isLoggable(Level.FINER)) {
            LOG.finer(icon + '[' + size + ']' + " f " + fallback);
        }
        return fallback;
    }

    private static String findIconHelper(final String icon, final String size, final String theme) {
        try {
            final String filename = lookupIcon(icon, size, theme);
            if (filename != null) {
                return filename;
            }
        } catch (IOException e) {
        }

        /*
         * always look for hicolor and then empty theme in case of invalid
         * theme.
         */
        if (!theme.isEmpty() && theme != HICOLOR) {
            return findIconHelper(icon, size, HICOLOR);
        } else if (!theme.isEmpty()) {
            return findIconHelper(icon, size, "");
        } else {
            return null;
        }
    }

    private static final Logger LOG = Logger.getLogger(ScilabSwingUtilities.class.getName());

    private static final String SCI = System.getenv("SCI");
    private static final String SEP = System.getProperty("file.separator");
    private static final String DOT = ".";
    private static final String HICOLOR = "hicolor";
    private static final List<String> ICONS_EXTENSIONS = Arrays.asList("png", "svg", "xpm");
    private static final HashMap<File, TreeSet<String>> THEME_SUBDIR_CACHE = new HashMap<File, TreeSet<String>>();

    private static final FileFilter DIR_FILTER;
    private static final List<String> THEME_BASENAME;
    static {
        DIR_FILTER = new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                return pathname.isDirectory();
            }
        };
        THEME_BASENAME = new ArrayList<String>();

        /*
         * Linux specific path
         */
        switch (OS.get()) {
            case UNIX:
                THEME_BASENAME.add("~/.icons");
                THEME_BASENAME.add("/usr/share/icons");
                THEME_BASENAME.add("/usr/share/pixmaps");

                THEME_BASENAME.add(SCI + "/../icons");
                break;

            case MAC:
                THEME_BASENAME.add(SCI + "/../icons");
                break;

            default:
                break;
        }

        /*
         * Scilab embedded icons
         */

        final ArrayList<File> dirs = new ArrayList<File>();

        // Append SCI/desktop and SCI/modules/xxx to the dirs
        dirs.add(new File(SCI + SEP + "desktop"));
        dirs.addAll(Arrays.asList(new File(SCI + SEP + "modules").listFiles(DIR_FILTER)));
        for (File m : dirs) {
            final File icons = new File(m, "images" + SEP + "icons");
            final boolean iconsIsDir = icons.isDirectory();

            // add dirs/images/icons/ to the base name
            if (iconsIsDir) {
                THEME_BASENAME.add(icons.getAbsolutePath());
            }

            // add dirs/images/icons/*/* to the base name
            if (iconsIsDir) {
                for (File s : icons.listFiles(DIR_FILTER)) {
                    for (File category : s.listFiles(DIR_FILTER)) {
                        THEME_BASENAME.add(category.getAbsolutePath());
                    }
                }
            }
        }
    }

    private static String lookupIcon(final String iconname, final String size, final String theme) throws IOException {
        for (String directory : THEME_BASENAME) {
            final File themeDir = new File(directory + SEP + theme);
            if (!themeDir.exists() || !themeDir.isDirectory()) {
                continue;
            }

            /*
             * FIXME: implement an index.theme reader, for now we are parsing
             * the file path to get the information
             */

            /*
             * Create the theme subdirs
             */
            final int themeDirLen = themeDir.getCanonicalPath().length();
            final TreeSet<String> themeSubdirs = findThemeSubdir(themeDir, themeDirLen);

            /*
             * Create a theme subdirs for a specific size
             */
            final TreeSet<String> sizedSubDirs = findSizedSubdirs(size, themeSubdirs);

            /*
             * Look for the icon
             */
            for (final String s : sizedSubDirs) {
                for (String extension : ICONS_EXTENSIONS) {
                    final File f = new File(themeDir, s + SEP + iconname + DOT + extension);

                    if (f.exists()) {
                        return f.getCanonicalPath();
                    }
                }
            }
        }

        return null;
    }

    private static TreeSet<String> findSizedSubdirs(final String size, final TreeSet<String> themeSubdirs) {
        final TreeSet<String> sizedSubDirs = new TreeSet<String>();
        for (String subdir : themeSubdirs) {
            if (subdir.startsWith(size)) {
                sizedSubDirs.add(subdir);
            }
        }
        return sizedSubDirs;
    }

    private static TreeSet<String> findThemeSubdir(final File themeDir, final int themeDirLen) throws IOException {
        final TreeSet<String> cache = THEME_SUBDIR_CACHE.get(themeDir);
        if (cache != null) {
            return cache;
        }

        final TreeSet<String> themeSubdirs = new TreeSet<String>();

        final LinkedList<File> dirs = new LinkedList<File>(Arrays.asList(themeDir.listFiles(DIR_FILTER)));
        while (!dirs.isEmpty()) {
            final File d = dirs.poll();

            final List<File> sub = Arrays.asList(d.listFiles(DIR_FILTER));
            if (sub.isEmpty()) {
                final String s = d.getCanonicalPath();

                themeSubdirs.add(s.substring(themeDirLen + 1));
            } else {
                dirs.addAll(sub);
            }
        }

        THEME_SUBDIR_CACHE.put(themeDir, themeSubdirs);
        return themeSubdirs;
    }

    private static String lookupFallbackIcon(final String icon) {
        for (String directory : THEME_BASENAME) {
            for (String extension : ICONS_EXTENSIONS) {
                final File f = new File(directory + SEP + icon + DOT + extension);
                if (f.exists()) {
                    return f.getAbsolutePath();
                }
            }
        }

        return null;
    }

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
