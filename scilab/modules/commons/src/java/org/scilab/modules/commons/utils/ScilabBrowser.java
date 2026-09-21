/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2025 - Dassault Systèmes S.E. - Antoine ELIAS
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.utils;

import java.io.File;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.cef.CefApp;
import org.cef.CefClient;
import org.cef.CefSettings;
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

    private ScilabBrowser() { }

    private static void init() {
        if (cefApp_ == null || CefApp.getState() == CefAppState.TERMINATED) {
            CefSettings settings = new CefSettings();
            settings.windowless_rendering_enabled = false;
            settings.log_severity = CefSettings.LogSeverity.LOGSEVERITY_DISABLE;
            List<String> cefArgs = new ArrayList<>();
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

            settings.cache_path = ScilabCommons.getSCIHOME() + "/jcef";
            settings.persist_session_cookies = true;

            CefApp.startup(cefArgs.toArray(new String[0]));
            cefApp_ = CefApp.getInstance(cefArgs.toArray(new String[0]), settings);

            try {
                Class scilab = ClassLoader.getSystemClassLoader().loadClass("org.scilab.modules.core.Scilab");
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
        Thread watchdog = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(SHUTDOWN_WATCHDOG_DELAY_MS);
                } catch (InterruptedException ie) {
                    return;
                }
                System.err.println("[ScilabBrowser] CEF shutdown watchdog: forcing JVM halt (N_Shutdown stuck)");
                Runtime.getRuntime().halt(0);
            }
        }, "ScilabBrowser-shutdown-watchdog");
        watchdog.setDaemon(true);
        watchdog.start();

        cefApp_.dispose();
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

    public static void release(CefClient client) {
        synchronized (activeClients) {
            activeClients.remove(client);
            focusGuards.remove(client);
        }
        client.dispose();
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
