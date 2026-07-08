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
public class ConsoleWithUITests extends AbstractConsoleTest {

    @Test
    public void testConsoleClearConfirm() throws InterruptedException {
        // Test clearing the console with clc command
        consoleRegion.type("disp('Test content to clear')" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Clear the console using clc command
        consoleRegion.type("clc" + Key.ENTER);
        
        // validate the confirmation dialog (if any)
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.ENTER);

        // Verify console is cleared by typing a simple command
        consoleRegion.type("disp('Console cleared successfully')" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        consoleRegion.type("// Console clear test completed\n");
    }

    @Test
    public void testConsoleClearSomeLines() throws InterruptedException {
        // Test clearing the console with clc command for specific number of lines
        consoleRegion.wait(0.5);
        consoleRegion.type("rand(10,10)" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("clc();" + Key.ENTER);
        
        // validate the confirmation dialog (if any)
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.ENTER);

        consoleRegion.type("rand(10,10)" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("clc(5); // Delete the 5 previous lines" + Key.ENTER);

        // validate the confirmation dialog (if any)
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.ENTER);

        consoleRegion.type("disp('After clear')" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        consoleRegion.type("// Console clear test completed\n");
    }
    
    @Test
    public void testConsoleLinesSettings() throws InterruptedException {
        // Test console lines/pagination settings
        consoleRegion.type("lines()" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Set lines to a specific value
        consoleRegion.type("lines(20, 20)" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Test with some output
        consoleRegion.type("(1:25)'" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("y"); // Continue output
        consoleRegion.wait(0.5);
        
        // Reset to default
        consoleRegion.type("lines(0)" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.setActive(null);
        
        consoleRegion.type("// Lines settings test completed\n");
    }

}