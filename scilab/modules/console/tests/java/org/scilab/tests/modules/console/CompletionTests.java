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
public class CompletionTests extends AbstractConsoleTest {

    @Test
    public void testConsoleCompletion() throws InterruptedException {
        // Test tab completion functionality
        consoleRegion.type("plot" + Key.TAB);
        consoleRegion.wait(2);  // Wait for completion list to appear
        
        // Cancel completion
        consoleRegion.type(Key.CTRL + "c");
        consoleRegion.wait(0.5);
        
        // Clear the line
        consoleRegion.type(Key.CTRL + "u");
        
        // Test completion with partial string
        consoleRegion.type("disp" + Key.TAB);
        consoleRegion.wait(2);
        consoleRegion.type(Key.CTRL + "c");
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.CTRL + "u");
        
        consoleRegion.type("// Completion test completed\n");
    }

}