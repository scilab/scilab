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
import java.nio.file.FileSystems;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardWatchEventKinds;
import java.nio.file.WatchEvent;
import java.nio.file.WatchKey;
import java.nio.file.WatchService;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.SwingUtilities;

/**
 * Tells SciNotes when a file it shows is written, created or removed by
 * something else.
 *
 * The watch service reports directories, so one watch is kept per directory
 * holding open files, and released when its last file is gone. Events come in
 * bursts -- editors write through a temporary file and rename it -- so they
 * are gathered for a short while before SciNotes is told, once per file.
 *
 * Some file systems report nothing: a Windows directory seen from WSL, or a
 * network share. SciNotes still checks the file when a tab takes the focus,
 * which is what it did before this watcher existed.
 */
public final class SciNotesFileWatcher {

    /** Editors write in several steps; wait for the burst to end. */
    private static final long QUIET_PERIOD_MS = 200;

    private static final Map<Path, Integer> WATCHED_DIRECTORIES = new HashMap<Path, Integer>();
    private static final Map<Path, WatchKey> KEYS = new HashMap<Path, WatchKey>();

    private static WatchService service;
    private static Thread thread;

    private SciNotesFileWatcher() { }

    /**
     * Watch the directory of this file, if it is not watched already.
     *
     * @param file the file to follow
     */
    public static synchronized void watch(File file) {
        Path directory = directoryOf(file);
        if (directory == null || !start()) {
            return;
        }

        Integer count = WATCHED_DIRECTORIES.get(directory);
        if (count != null) {
            WATCHED_DIRECTORIES.put(directory, count + 1);
            return;
        }

        try {
            KEYS.put(directory, directory.register(service,
                StandardWatchEventKinds.ENTRY_CREATE,
                StandardWatchEventKinds.ENTRY_DELETE,
                StandardWatchEventKinds.ENTRY_MODIFY));
            WATCHED_DIRECTORIES.put(directory, 1);
        } catch (IOException | RuntimeException e) {
            // An unwatchable directory (removable media, network share, a
            // file system without notifications): the focus check remains.
            System.err.println("SciNotes: cannot watch " + directory + ": " + e);
        }
    }

    /**
     * Release one watch on this file's directory.
     *
     * @param file a file passed to {@link #watch(File)} before
     */
    public static synchronized void unwatch(File file) {
        Path directory = directoryOf(file);
        Integer count = directory == null ? null : WATCHED_DIRECTORIES.get(directory);
        if (count == null) {
            return;
        }

        if (count > 1) {
            WATCHED_DIRECTORIES.put(directory, count - 1);
            return;
        }

        WATCHED_DIRECTORIES.remove(directory);
        WatchKey key = KEYS.remove(directory);
        if (key != null) {
            key.cancel();
        }
    }

    private static Path directoryOf(File file) {
        if (file == null) {
            return null;
        }
        try {
            File parent = file.getCanonicalFile().getParentFile();
            return parent == null ? null : Paths.get(parent.getPath());
        } catch (IOException | RuntimeException e) {
            return null;
        }
    }

    private static boolean start() {
        if (thread != null) {
            return true;
        }

        try {
            service = FileSystems.getDefault().newWatchService();
        } catch (IOException | RuntimeException e) {
            System.err.println("SciNotes: no file watching available: " + e);
            return false;
        }

        thread = new Thread(new Runnable() {
            public void run() {
                loop();
            }
        }, "SciNotes file watcher");
        thread.setDaemon(true);
        thread.start();
        return true;
    }

    private static void loop() {
        while (true) {
            WatchKey key;
            try {
                key = service.take();
            } catch (InterruptedException | RuntimeException e) {
                return;
            }

            Set<File> changed = new HashSet<File>();
            // Gather the whole burst: a save through a temporary file shows
            // up as a removal followed by a creation.
            while (key != null) {
                collect(key, changed);
                key.reset();
                try {
                    key = service.poll(QUIET_PERIOD_MS, java.util.concurrent.TimeUnit.MILLISECONDS);
                } catch (InterruptedException e) {
                    key = null;
                }
            }

            if (!changed.isEmpty()) {
                final Set<File> files = changed;
                SwingUtilities.invokeLater(new Runnable() {
                    public void run() {
                        for (File file : files) {
                            MonacoEditorPane.fileChangedOnDisk(file);
                        }
                    }
                });
            }
        }
    }

    private static void collect(WatchKey key, Set<File> changed) {
        Object directory = key.watchable();
        if (!(directory instanceof Path)) {
            return;
        }
        for (WatchEvent<?> event : key.pollEvents()) {
            Object context = event.context();
            if (event.kind() != StandardWatchEventKinds.OVERFLOW && context instanceof Path) {
                changed.add(((Path) directory).resolve((Path) context).toFile());
            }
        }
    }
}
