/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.utils;

import java.awt.Component;
import java.awt.event.HierarchyListener;
import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.channels.FileChannel;
import java.nio.channels.FileLock;
import java.nio.channels.OverlappingFileLockException;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardCopyOption;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;

import org.cef.CefApp;
import org.cef.CefClient;
import org.cef.CefSettings;
import org.cef.browser.CefBrowser;
import org.cef.handler.CefFocusHandler;
import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabConstants;
import org.cef.CefApp.CefAppState;

public final class ScilabBrowser {

    private static CefApp cefApp_;

    /** Active clients, tracked so they can be force-disposed at shutdown
     *  before cefApp_.dispose() (otherwise clients of browsers that never
     *  fully initialized can block the shutdown). */
    private static final Set<CefClient> activeClients = new LinkedHashSet<>();

    /** Focus guard installed on each client, keyed by that client. */
    private static final Map<CefClient, ScilabFocusHandler> focusGuards = new HashMap<>();

    /** Delay (ms) before the shutdown watchdog forces the process to exit
     *  if CefApp.N_Shutdown stays stuck in native code. */
    private static final long SHUTDOWN_WATCHDOG_DELAY_MS = 3000;

    /** Name of the lock file held in the master Chromium profile. */
    private static final String CACHE_LOCK_FILE = ".scilab.lock";
    /** Profile files copied from the master profile into a secondary one to
     *  share the cookies ("Local State" holds the key encrypting them). The
     *  SQLite journal is left out: a journal copied at another time than the
     *  database could corrupt it on rollback. */
    private static final String[] COOKIE_FILES = {
        "Local State",
        "Default/Network/Cookies"
    };

    /** Lock on the master Chromium profile, held until this process exits. */
    private static FileChannel cacheLockChannel;
    private static FileLock cacheLock;

    /** Profile private to this process (in TMPDIR), null when using the master one. */
    private static File privateProfile;

    /** Maximum delay (ms) spent at exit waiting for CEF to terminate before
     *  removing the private profile. */
    private static final long PRIVATE_PROFILE_TERMINATE_DELAY_MS = 5000;
    /** Maximum delay (ms) spent retrying the removal while the CEF
     *  subprocesses exit. */
    private static final long PRIVATE_PROFILE_DELETE_DELAY_MS = 1000;

    /** Prefix of the Scilab TMPDIR names: SCI_TMP_<pid>_<random>. */
    private static final String TMPDIR_PREFIX = "SCI_TMP_";

    private ScilabBrowser() { }

    /**
     * Chromium refuses to share a profile (cache_path) between processes:
     * with a single SCIHOME/jcef, the second Scilab ends in
     * INITIALIZATION_FAILED and every JCEF component of that instance is
     * dead (console, help browser, assistant, browser uicontrol).
     * The first Scilab locks SCIHOME/jcef, the master profile, for the whole
     * life of the process; the OS releases the lock if Scilab dies, so a
     * crash never leaves it unusable. The next instances use a profile in
     * their own TMPDIR, removed with it on exit, seeded with the cookies of
     * the master profile: they start logged in like the first one, but what
     * they change is not kept.
     *
     * @param base the master profile directory (SCIHOME/jcef)
     * @return a profile directory not used by another process
     */
    private static String lockCachePath(String base) {
        File dir = new File(base);
        if (cacheLock != null && cacheLock.isValid()) {
            return dir.getAbsolutePath(); // CEF restarted in this process
        }

        dir.mkdirs();
        FileChannel channel = null;
        try {
            channel = FileChannel.open(new File(dir, CACHE_LOCK_FILE).toPath(),
                                       StandardOpenOption.CREATE, StandardOpenOption.WRITE);
            FileLock lock = channel.tryLock();
            if (lock != null) {
                cacheLockChannel = channel;
                cacheLock = lock;
                return dir.getAbsolutePath();
            }
        } catch (IOException | OverlappingFileLockException e) {
            // master profile unusable: use a private one
        }
        if (channel != null) {
            try {
                channel.close();
            } catch (IOException ignored) {
                // nothing more to do
            }
        }

        File tmp = new File(ScilabConstants.TMPDIR, "jcef");
        if (!tmp.exists()) {
            copyCookies(dir, tmp);
        }
        privateProfile = tmp;
        return tmp.getAbsolutePath();
    }

    /**
     * Remove the private profile at exit. clearTMPDIR() cannot be relied on:
     * CEF and its subprocesses (GPU, network) may still hold some of its
     * files and removedir gives up at the first file it cannot delete.
     * Wait for CEF to terminate, then retry while the subprocesses exit.
     * On Windows, a few files stay open until this process exits
     * (chrome_debug.log, first_party_sets.db, ukm_db): the next Scilab
     * removes them, see deleteOrphanProfiles().
     */
    private static void deletePrivateProfile() {
        long deadline = System.currentTimeMillis() + PRIVATE_PROFILE_TERMINATE_DELAY_MS;
        while (CefApp.getState() != CefAppState.TERMINATED) {
            if (System.currentTimeMillis() > deadline || !sleep(100)) {
                return;
            }
        }

        deadline = System.currentTimeMillis() + PRIVATE_PROFILE_DELETE_DELAY_MS;
        while (true) {
            deleteTree(privateProfile.toPath());
            if (!privateProfile.exists() || System.currentTimeMillis() > deadline || !sleep(100)) {
                return;
            }
        }
    }

    /**
     * Remove the private profiles left by Scilab processes that are gone
     * (files kept open until exit on Windows, crash, ...). Only the jcef
     * directory is removed, then its TMPDIR if nothing else is left in it
     * (it survived clearTMPDIR() only because of jcef). A profile whose
     * process id has been reused by another process is kept until a next run.
     */
    private static void deleteOrphanProfiles() {
        File parent = ScilabConstants.TMPDIR.getAbsoluteFile().getParentFile();
        File[] dirs = parent == null ? null : parent.listFiles();
        if (dirs == null) {
            return;
        }

        long self = ProcessHandle.current().pid();
        for (File dir : dirs) {
            String name = dir.getName();
            File profile = new File(dir, "jcef");
            if (!name.startsWith(TMPDIR_PREFIX) || !profile.isDirectory()) {
                continue;
            }
            String[] parts = name.substring(TMPDIR_PREFIX.length()).split("_", 2);
            long pid;
            try {
                pid = Long.parseLong(parts[0]);
            } catch (NumberFormatException e) {
                continue;
            }
            if (pid != self && !ProcessHandle.of(pid).isPresent()) {
                deleteTree(profile.toPath());
                dir.delete(); // only if empty
            }
        }
    }

    /**
     * @param ms the delay to sleep
     * @return false if the thread was interrupted
     */
    private static boolean sleep(long ms) {
        try {
            Thread.sleep(ms);
            return true;
        } catch (InterruptedException e) {
            return false;
        }
    }

    /**
     * Delete a directory tree, going on when a file cannot be deleted.
     *
     * @param root the directory to delete
     */
    private static void deleteTree(Path root) {
        try {
            Files.walkFileTree(root, new SimpleFileVisitor<Path>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) {
                    file.toFile().delete();
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public FileVisitResult visitFileFailed(Path file, IOException e) {
                    return FileVisitResult.CONTINUE;
                }

                @Override
                public FileVisitResult postVisitDirectory(Path dir, IOException e) {
                    dir.toFile().delete();
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (IOException e) {
            // retried by the caller
        }
    }

    /**
     * Copy the cookies of a profile into another one. Chromium may be writing
     * them in the master profile: at worst the copy is a few seconds old or
     * unreadable, and Chromium then drops it without touching the master.
     *
     * @param from the master profile directory
     * @param to the profile directory to seed
     */
    private static void copyCookies(File from, File to) {
        for (String name : COOKIE_FILES) {
            File src = new File(from, name);
            if (!src.isFile()) {
                continue;
            }
            File dst = new File(to, name);
            dst.getParentFile().mkdirs();
            try {
                Files.copy(src.toPath(), dst.toPath(), StandardCopyOption.REPLACE_EXISTING);
            } catch (IOException e) {
                // no cookies: the instance works anyway
            }
        }
    }

    private static void init() {
        if (cefApp_ == null || CefApp.getState() == CefAppState.TERMINATED) {
            CefSettings settings = new CefSettings();
            settings.windowless_rendering_enabled = false;
            settings.log_severity = CefSettings.LogSeverity.LOGSEVERITY_DISABLE;
            List<String> cefArgs = new ArrayList<>();
            
            // On Unix (Linux), force CEF to use X11 backend to avoid Wayland/GTK4 issues for now
            if (OS.get() == OS.UNIX) {
                cefArgs.add("--ozone-platform=x11");  // Explicitly use X11 platform
            } 
            if (OS.get() == OS.MAC) {
                // Development version: JCEF is in SCI/lib/thirdparty/jcef/
                // Packaged version:  JCEF is in SCI/../../lib/thirdparty/jcef/
                String pathPrefix = ""; // Development version
                if (new File(ScilabConstants.SCI.getPath() + "/lib/thirdparty/jcef/Chromium Embedded Framework.framework").exists() == false) {
                    pathPrefix = "/../.."; // Packaged version
                }
                cefArgs.add(0, "--framework-dir-path=" + ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/Chromium Embedded Framework.framework");
                cefArgs.add(0, "--main-bundle-path=" + ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/jcef Helper.app");
                cefArgs.add(0, "--browser-subprocess-path=" + ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/jcef Helper.app/Contents/MacOS/jcef Helper");
                // The following settings are mandatory for packaged version
                settings.resources_dir_path = ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/Chromium Embedded Framework.framework/Resources/";
                settings.locales_dir_path = ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/Chromium Embedded Framework.framework/Resources/";
                settings.browser_subprocess_path = ScilabConstants.SCI.getPath() + pathPrefix + "/lib/thirdparty/jcef/jcef Helper.app";
            }

            // Set by bin/scilab on Linux when the desktop keyring is locked/absent. This avoid the "unlock keyring" dialog at startup.
            // User can set to enforce a specific keyring.
            String passwordStore = System.getenv("SCILAB_CEF_PASSWORD_STORE");
            if (passwordStore != null && !passwordStore.isEmpty()) {
                cefArgs.add("--password-store=" + passwordStore);
            }

            settings.cache_path = lockCachePath(ScilabCommons.getSCIHOME() + "/jcef");
            settings.persist_session_cookies = true;

            Thread cleanup = new Thread(new Runnable() {
                @Override
                public void run() {
                    deleteOrphanProfiles();
                }
            }, "ScilabBrowser-profile-cleanup");
            cleanup.setDaemon(true);
            cleanup.start();

            CefApp.startup(cefArgs.toArray(new String[0]));

            // macOS: CEF intercepts the app-menu Quit / CMD+Q before AWT's
            // Desktop.setQuitHandler is never invoked, so Scilab never exits via the menu.
            if (OS.get() == OS.MAC) {
                CefApp.addAppHandler(new org.cef.handler.CefAppHandlerAdapter(cefArgs.toArray(new String[0])) {
                    @Override
                    public boolean onBeforeTerminate() {
                        // commons cannot depend on action_binding: reflectively
                        // call InterpreterManagement.requestScilabExec("exit();").
                        try {
                            Class<?> im = ClassLoader.getSystemClassLoader()
                                .loadClass("org.scilab.modules.action_binding.InterpreterManagement");
                            im.getMethod("requestScilabExec", String.class).invoke(null, "exit();");
                        } catch (Throwable t) {
                            t.printStackTrace();
                        }
                        return true; // Scilab's exit() will perform the real termination
                    }
                });
            }

            cefApp_ = CefApp.getInstance(cefArgs.toArray(new String[0]), settings);

            try {
                Class<?> scilab = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.core.Scilab");
                Method registerFinalHook = scilab.getDeclaredMethod("registerFinalHook", Runnable.class);
                registerFinalHook.invoke(null, new Runnable() {
                    public void run() {
                        shutdown();
                    }
                });
            } catch (ClassNotFoundException|
                IllegalAccessException|
                IllegalArgumentException|
                InvocationTargetException|
                NoSuchMethodException|
                SecurityException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * JCEF shutdown hook, run just before the JVM shuts down.
     * Force-releases any still-referenced CefClient, then starts a daemon
     * watchdog that will halt the process if CefApp.N_Shutdown stays stuck
     * in native code (case of a browser that was created but never reached
     * onAfterCreated).
     */
    private static void shutdown() {
        List<CefClient> pending;
        synchronized (activeClients) {
            pending = new ArrayList<>(activeClients);
            activeClients.clear();
            focusGuards.clear();
        }
        for (CefClient c : pending) {
            try {
                c.dispose();
            } catch (Throwable t) {
                // ignore: we are just trying to unblock the shutdown
            }
        }

        // Daemon watchdog: CefApp.dispose posts N_Shutdown on the EDT,
        // which can stay stuck indefinitely in native code. Force exit
        // after a delay if the JVM could not die on its own.
        final AtomicBoolean shutdownReturned = new AtomicBoolean(false);
        Thread watchdog = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(SHUTDOWN_WATCHDOG_DELAY_MS);
                } catch (InterruptedException ie) {
                    return;
                }
                if (shutdownReturned.get()) {
                    return;
                }
                System.err.println("[ScilabBrowser] CEF shutdown watchdog: forcing JVM halt (N_Shutdown stuck)");
                Runtime.getRuntime().halt(0);
            }
        }, "ScilabBrowser-shutdown-watchdog");
        watchdog.setDaemon(true);
        watchdog.start();

        try {
            cefApp_.dispose();
        } finally {
            shutdownReturned.set(true);
        }

        if (privateProfile != null) {
            deletePrivateProfile();
        }
    }

    public static CefClient get() {
        init();
        CefClient c = cefApp_.createClient();
        // Installed here rather than by each caller: a client left without it
        // recurses on focus until the AppKit thread dies, and CefClient only
        // accepts one focus handler, so a late one would be a silent no-op.
        ScilabFocusHandler guard = new ScilabFocusHandler();
        c.addFocusHandler(guard);
        synchronized (activeClients) {
            activeClients.add(c);
            focusGuards.put(c, guard);
        }
        return c;
    }

    /**
     * Chain a focus handler behind the client's recursion guard.
     * Use this instead of CefClient.addFocusHandler, which would either be
     * ignored or replace the guard.
     *
     * @param client a client returned by {@link #get()}
     * @param handler the handler to chain
     */
    public static void addFocusHandler(CefClient client, CefFocusHandler handler) {
        ScilabFocusHandler guard;
        synchronized (activeClients) {
            guard = focusGuards.get(client);
        }
        if (guard != null) {
            guard.add(handler);
        } else {
            client.addFocusHandler(handler);
        }
    }

    /**
     * Force-close a browser.
     * On macOS, CefBrowserWr forwards every visibility change of its component
     * to the browser NSView asynchronously, even while closing: one landing
     * after CEF freed that view crashes the main thread. Drop that listener first.
     */
    public static void close(CefBrowser browser) {
        Component ui = browser.getUIComponent();
        if (ui != null) {
            for (HierarchyListener l : ui.getHierarchyListeners()) {
                ui.removeHierarchyListener(l);
            }
        }
        browser.close(true);
    }

    public static void release(CefClient client) {
        synchronized (activeClients) {
            activeClients.remove(client);
            focusGuards.remove(client);
        }
        // Exit race: the shutdown final hook may have disposed CefApp before a window's removeNotify runs.
        // Disposing the client then throws IllegalStateException "CefApp was terminated"
        if (isShutdown()) {
            return;
        }
        client.dispose();
    }

    /** @return true if CefApp is not initialized or already terminated. */
    public static boolean isShutdown() {
        return cefApp_ == null || CefApp.getState() == CefAppState.TERMINATED;
    }

    public static String getJcefVersion() {
        init();
        return cefApp_.getVersion().getJcefVersion();
    }

    public static String getCefVersion() {
        init();
        return cefApp_.getVersion().getCefVersion();
    }

    public static String getChromeVersion() {
        init();
        return cefApp_.getVersion().getChromeVersion();
    }
}
