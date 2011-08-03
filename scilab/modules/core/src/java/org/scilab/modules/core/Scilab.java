/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 * Copyright (C) 2007-2008 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2007-2008 - INRIA - Jean-Baptiste SILVY
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.core;

import java.lang.reflect.InvocationTargetException;

import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;

import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;

import org.scilab.modules.commons.ScilabConstants;
import org.scilab.modules.jvm.LoadClassPath;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.gui.utils.LookAndFeelManager;
import org.scilab.modules.gui.utils.UIElementMapper;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.window.Window;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Main Class for Scilab
 * @author Allan CORNET
 * @author Jean-Baptiste SILVY
 * @author Vincent COUVERT
 * @author Sylvestre Ledru
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class Scilab {

    private static final String CLASS_NOT_FOUND = "Could not find class: ";

    private static final String SEE_DEFAULT_PATHS = "See SCI/etc/classpath.xml for default paths.";

    /** Index of windows vista version */
    private static final double VISTA_VERSION = 6.0;

    private static final String ENABLE_JAVA2D_OPENGL_PIPELINE = "sun.java2d.opengl";
    private static final String ENABLE = "true";
    private static final String DISABLE = "false";
    private static final String DISABLE_DDRAW = "sun.java2d.noddraw";
    private static final String OSNAME = "os.name";
    private static final String MACOS = "mac";

    private static String SCIDIR;

    private static boolean success;
    private static boolean finish;
    private static boolean exitCalled;
    private static int mode;

    private static List<Runnable> finalhooks = new ArrayList<Runnable>();
    private static List<Runnable> initialhooks = new ArrayList<Runnable>();

    private Window mainView;

    /**
     * Constructor Scilab Class.
     * @param mode Mode Scilab -NW -NWNI -STD -API
     */
    public Scilab(int mode) {
        this.mode = mode;
        DockingManager.setDockableFactory(ScilabTabFactory.getInstance());

        /*
         * Set Scilab directory. Note that it is done in the constructor
         * and not as directly when setting the member because we had some
         * race condition. See bug #4419
         */
        try {
            SCIDIR = System.getenv("SCI");

            /*
             * Set Java directories to Scilab ones
             */
            System.setProperty("java.io.tmpdir", ScilabConstants.TMPDIR.getCanonicalPath());
            System.setProperty("user.home", ScilabConstants.SCIHOME.getCanonicalPath());

        } catch (Exception e) {
            System.err.println("Cannot retrieve the variable SCI. Please report on http://bugzilla.scilab.org/");
            System.err.println(e.getLocalizedMessage());
            System.exit(-1);
        }

        /*
         * Set options for JOGL
         * they must be set before creating GUI
         */
        setJOGLFlags();

        /*
         * if not API mode
         * bug 3673 by default in API mode we dont modify look n feel
         * If SCI_JAVA_ENABLE_HEADLESS is set, do not set the look and feel.
         * (needed when building the documentation under *ux)
         */
        if (mode != 1 && System.getenv("SCI_JAVA_ENABLE_HEADLESS") == null) {
            /* http://java.sun.com/docs/books/tutorial/uiswing/lookandfeel/plaf.html */
            try {

                String scilabLookAndFeel = "javax.swing.plaf.metal.MetalLookAndFeel";

                if (isWindowsPlateform()) {
                    scilabLookAndFeel = "com.sun.java.swing.plaf.windows.WindowsLookAndFeel";
                } else if (System.getProperty(OSNAME).toLowerCase().indexOf(MACOS) != -1) {
                    /** OPTION ADDED TO ALLOW DOCKING UNDER MACOSX */
                    System.setProperty(DockingConstants.HEAVYWEIGHT_DOCKABLES, ENABLE);
                    scilabLookAndFeel = "apple.laf.AquaLookAndFeel";
                } else {
                    scilabLookAndFeel = "com.sun.java.swing.plaf.gtk.GTKLookAndFeel";
                }

                /* Init the LookAndFeelManager all the time since we can
                 * create windows in the NW mode */

                LookAndFeelManager lookAndFeel = new LookAndFeelManager();

                if (lookAndFeel.isSupportedLookAndFeel(scilabLookAndFeel)) {
                    lookAndFeel.setLookAndFeel(scilabLookAndFeel);
                } else {
                    lookAndFeel.setSystemLookAndFeel();
                }

            } catch (java.lang.NoClassDefFoundError exception) {
                System.err.println("Could not initialize graphics Environment");
                System.err.println("Scilab Graphical option may not be working correctly.");
                System.err.println("An error occurred: " + exception.getLocalizedMessage());
            }
        }

        if (mode == 2) { /* Mode GUI */
            // Create a user config file if not already exists
            ConfigManager.createUserCopy();

            WindowsConfigurationManager.restoreUUID(ConsoleTabFactory.NULLUUID);

            SwingScilabConsole sciConsole = ((SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole());
            SwingScilabTab consoleTab = (SwingScilabTab) sciConsole.getParent();
            mainView = (Window) UIElementMapper.getCorrespondingUIElement(consoleTab.getParentWindowId());
        }
    }

    /**
     * @return the current mode
     */
    public static int getMode() {
        return mode;
    }

    /**
     * Sets the prompt displayed in Scilab console
     * @param prompt the prompt to be displayed as a String
     */
    public void setPrompt(String prompt) {
        ScilabConsole.getConsole().setPrompt(prompt);
    }

    /**
     * Set the command line flags to the JVM
     */
    public static void setJOGLFlags() {

        // Must give popUpMenu heavy weight
        // in case we enable GLCanvas
        JPopupMenu.setDefaultLightWeightPopupEnabled(false);
        // Uneash OpenGL power
        // Not yet
        //System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, ENABLE_WITH_DEBUG);
        System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, DISABLE);

        if (isWindowsPlateform()) {
            if (findWindowsVersion() >= VISTA_VERSION) {
                // don't enable OpenGL because of aero
                System.setProperty(ENABLE_JAVA2D_OPENGL_PIPELINE, DISABLE);
            }
            // desactivate direct3d and direct draw under windows
            System.setProperty(DISABLE_DDRAW, ENABLE);
        }
    }

    /**
     * @return true if the os is windows, false otherwise
     */
    public static boolean isWindowsPlateform() {
        // get os name
        return System.getProperty(OSNAME).toLowerCase().contains("windows");
    }

    /**
     * Find the verion of windows used on the computer if one
     * @return negative value if the OS is not windows, the version of windows otherwise
     */
    public static double findWindowsVersion() {
        // default valu enot windows
        double windowsVersion = -1.0;

        if (isWindowsPlateform()) {
            // windows plateform
            return Double.valueOf(System.getProperty("os.version"));
        }

        return windowsVersion;
    }

    /**
     * Call from canCloseMainScilabObject (call itself from sci_exit)
     * @return true if the console is closed
     */
    public static boolean canClose() {
        SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    exitCalled = true;
                    success = ClosingOperationsManager.startClosingOperationOnRoot();
                    exitCalled = false;
                    finish = true;
                }
            });

        while (!finish) {
            try {
                Thread.sleep(10);
            } catch (InterruptedException e) {
                System.err.println(e);
            }
        }

        finish = false;

        return success;
    }

    /**
     * @return true if exit has been called from command line
     */
    public static boolean getExitCalled() {
        return exitCalled;
    }

    /**
     * Register a hook to execute just before the JVM shutdown.
     * A hook should not contain threads, there is no warranty that they will be fully executed.
     */
    public static void registerFinalHook(Runnable hook) {
        finalhooks.add(hook);
    }

    /**
     * Remove a hook
     */
    public static void removeFinalHook(Runnable hook) {
        finalhooks.remove(hook);
    }

    /**
     * This method should be called from jni (finishMainScilabObject())
     */
    public static void executeFinalHooks() {
        for (Runnable hook : finalhooks) {
            hook.run();
        }
    }

    /**
     * Register a hook to execute after the Scilab initialization.
     * A hook should not contain threads, there is no warranty that they will be fully executed.
     */
    public static void registerInitialHook(Runnable hook) {
        initialhooks.add(hook);
    }

    /**
     * Remove a hook
     */
    public static void removeInitialHook(Runnable hook) {
        initialhooks.remove(hook);
    }

    /**
     * This method should be called from C (realmain)
     */
    public static void executeInitialHooks() {
        for (final Runnable hook : initialhooks) {
            try {
                SwingUtilities.invokeAndWait(hook);
            } catch (InterruptedException e) {
                System.err.println(e);
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
