/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

package org.scilab.tests.modules.console;

import org.junit.Ignore;
import org.junit.Test;

import org.sikuli.script.Key;
import org.sikuli.script.Region;

/**
 * Console feature tests for Scilab using Sikuli API
 * These tests verify various console functionalities including shortcuts, commands, and interactions
 */
public class ConsoleCompletionTests extends AbstractConsoleTest {

    @Test
    public void testConsoleCompletionSimple() throws InterruptedException {
        // Test to trigger completion
        consoleRegion.type("atomsG" + Key.TAB);
        consoleRegion.wait(0.5);
        
        // Hit 5 DOWN to have atomsGui selected, and select it
        consoleRegion.type(Key.DOWN.repeat(5));
        consoleRegion.type(Key.ENTER);

        // cancel to avoid execute it
        consoleRegion.type(Key.CTRL+"c");
        
        consoleRegion.type("// atomsGui should be selected\n");
    }

    @Test
    public void testConsoleCompletionPartialTyping() throws InterruptedException {
        // Test to trigger completion
        consoleRegion.type("atomsG" + Key.TAB);
        consoleRegion.wait(0.5);
        
        // Continue typing selected, and select it
        consoleRegion.type("ui");
        consoleRegion.type(Key.ENTER);

        // cancel to avoid execute it
        consoleRegion.type(Key.CTRL+"c");
        
        consoleRegion.type("// atomsGui should have been filtered on completion list and selected\n");
    }

    @Test
    public void testConsoleCompletionMiddle() throws InterruptedException {
        // Test to continue typing with completion visible
        consoleRegion.type("disp atomsG" + Key.TAB);
        consoleRegion.wait(0.5);
        
        // Hit 5 DOWN to have atomsGui selected, and select it
        consoleRegion.type(Key.DOWN.repeat(5));
        consoleRegion.type(Key.ENTER);

        // disp
        consoleRegion.type(Key.ENTER);
        
        consoleRegion.type("// \"atomsGui\" is displayed\n");
    }

    @Test
    public void testConsoleCompletion3Words() throws InterruptedException {
        // Test to trigger com
        consoleRegion.type("disp atomsG atomsGui3");
        consoleRegion.wait(0.5);

        // move back after atomsG
        consoleRegion.type(Key.LEFT.repeat(" atomsGui3".length()));
        consoleRegion.wait(0.5);
        
        // completion
        consoleRegion.type(Key.TAB);

        // Hit 5 DOWN to have atomsGui selected, and select it
        consoleRegion.type(Key.DOWN.repeat(5));
        consoleRegion.type(Key.ENTER);

        // disp
        consoleRegion.type(Key.ENTER);
        
        consoleRegion.type("// \"atomsG atomsGui3\" is displayed\n");
    }
}