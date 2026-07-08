/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.tests.modules.console;

import org.junit.Before;
import org.junit.After;

import org.sikuli.script.App;
import org.sikuli.script.Key;
import org.sikuli.script.Region;

/**
 * Abstract base class for Console tests using Sikuli API, this will hand Scilab binary launching/exiting
 */
public abstract class AbstractConsoleTest {

    protected boolean appStarted = false;
    protected App scilabApp;
    protected Region consoleRegion;

    @Before
    public void setUp() throws InterruptedException {
        // Start Scilab application

        String sciBin = "scilab";
        if (System.getProperty("os.name").startsWith("Windows")) {
            sciBin = "WScilex.exe";
        } else {
            sciBin = "scilab";
        }

        // locate an existing Scilab instance or start a new one
        for (App app : App.getApps(sciBin)) {
            System.err.println("Found Scilab instance: " + app.getName());
            if (app.isRunning()) {
                scilabApp = app;
                break;
            }
        }
        if (scilabApp == null) {
            String scilabPath = System.getenv("SCI") + "/bin/" + sciBin;
            scilabApp = App.open(scilabPath);
            appStarted = true;
            System.err.println("Scilab started with path: " + scilabPath);
        }
        
        scilabApp.focus();

        // Wait for Scilab to start (passing the splashscreen) and focus on console
        while (consoleRegion == null || consoleRegion.w < 900) {
            consoleRegion = scilabApp.waitForWindow(10);
        }
        scilabApp.focus();
        
        // Alert the user by blinking
        consoleRegion.highlight(1);
        
        // Enable console box for better visibility (should not request focus)
        consoleRegion.type("consolebox on" + Key.ENTER);
    }

    @After
    public void tearDown() {
        if (appStarted) {
            consoleRegion.type("exit" + Key.ENTER);
            scilabApp.close();
        }
    }
}