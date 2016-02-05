package org.scilab.modules.commons.gui;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.TreeSet;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.imageio.ImageIO;
import javax.swing.Icon;
import javax.swing.ImageIcon;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabCommonsUtils;

public final class FindIconHelper {
    /**
     * Look for the icon associated with the name.
     *
     * @param icon
     *            the name to look for
     * @return the image icon path
     */
    public static String findIcon(final String icon) {
        return findIcon(icon, "16x16", true);
    }

    public static String findIcon(final String icon, boolean defaultValue) {
        return findIcon(icon, "16x16", defaultValue);
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
        return findIcon(icon, size, true);
    }

    public static String findIcon(final String icon, final String size, boolean defaultValue) {
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
        if (defaultValue && fallback == null) {
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

    private static final Logger LOG = Logger.getLogger(FindIconHelper.class.getName());

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
        if(iconname.contains("<html>")) {
            return null;
        }

        for (String directory : THEME_BASENAME) {
            final File themeDir = new File(directory + SEP + theme);
            if (!themeDir.exists() || !themeDir.isDirectory()) {
                continue;
            }

            /*
             * FIXME: implement an index.theme reader, for now we are parsing
             * the file path to get the information
             */
            for (String extension : ICONS_EXTENSIONS) {
                final File f = new File(themeDir, iconname + DOT + extension);

                if (f.exists()) {
                    return f.getCanonicalPath();
                }
            }
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

            File[] allDirs = d.listFiles(DIR_FILTER);
            if (allDirs == null) {
                continue;
            }

            final List<File> sub = Arrays.asList(allDirs);
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

    public static String findImage(final String image) {
        return findImage(image, true);
    }

    public static String findImage(final String image, boolean defaulValue) {

        File file = new File(ScilabCommonsUtils.getCorrectedPath(image));
        if (file.exists()) {
            return file.getAbsolutePath();
        }

        for (String directory : THEME_BASENAME) {
            final File f = new File(directory + SEP + image);
            if (f.exists()) {
                return f.getAbsolutePath();
            }
        }

        return findIcon(image, defaulValue);
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

    public static void addThemePath(String path) {
        if (THEME_BASENAME.contains(path)) {
            return;
        }

        THEME_BASENAME.add(path);
    }


    public static Icon loadIcon(String icon) throws IOException {
        String iconFile = findIcon(icon, false);
        if (iconFile == null) {
            iconFile = "";
        }

        //add item in listbox
        File file = new File(iconFile);
        if (file.exists() == false) {
            String filename = findImage(iconFile, false);
            if (filename == null) {
                filename = "";
            }

            file = new File(filename);
        }

        return new ImageIcon(ImageIO.read(file));
    }
}
