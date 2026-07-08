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
public class ConsoleTests extends AbstractConsoleTest {

    @Test
    public void testConsoleHistoryNavigation() throws InterruptedException {
        // Test navigating through command history
        
        consoleRegion.type("a = 1 + 1" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("b = 2 * 2" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("c = 3 / 3" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Navigate up through history
        consoleRegion.type(Key.UP);  // Should show "c = 3 / 3"
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.UP);  // Should show "b = 2 * 2"
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.UP);  // Should show "a = 1 + 1"
        consoleRegion.wait(0.5);
        
        // Navigate down through history
        consoleRegion.type(Key.DOWN);  // Should show "b = 2 * 2"
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.DOWN);  // Should show "c = 3 / 3"
        consoleRegion.wait(0.5);
        consoleRegion.type(Key.DOWN);  // Should show empty prompt
        consoleRegion.wait(0.5);
        
        // Display a success message
        consoleRegion.type("// History navigation test completed\n");
    }

    @Test
    public void testConsoleModeCommands() throws InterruptedException {
        // Test console mode settings
        consoleRegion.type("mode()" + Key.ENTER);  // Display default mode
        consoleRegion.type("1+1  // Should show result" + Key.ENTER);

        consoleRegion.type("mode(0)" + Key.ENTER);  // Silent mode
        consoleRegion.type("1+1" + Key.ENTER);  // Should not show result
        
        consoleRegion.type("mode(1)" + Key.ENTER);  // Normal mode
        consoleRegion.type("1+1" + Key.ENTER);  // Should show result
        
        consoleRegion.type("mode(-1)" + Key.ENTER);  // Get current mode
        consoleRegion.type("1+1" + Key.ENTER);  // Should show result
        
        consoleRegion.type("mode(2)" + Key.ENTER);  // Get current mode
        consoleRegion.type("1+1  // Should show result" + Key.ENTER);
        
        consoleRegion.type("// Mode commands test completed\n");
    }

    @Test
    public void testConsoleInputFunction() throws InterruptedException {
        // Test the input() function
        consoleRegion.type("result = input('Enter a number: ')" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("42" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Test string input
        consoleRegion.type("text = input('Enter some text: ', 's')" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("Hello Scilab" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Display results
        consoleRegion.type("disp('Number entered: ' + string(result))" + Key.ENTER);
        consoleRegion.wait(0.5);
        consoleRegion.type("disp('Text entered: ' + text)" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        consoleRegion.type("// Input function test completed\n");
    }

    @Test
    public void testConsolePromptCustomization() throws InterruptedException {
        // Test prompt customization
        consoleRegion.type("original_prompt = prompt()" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Change prompt
        consoleRegion.type("prompt('SCILAB> ')" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Test with a command
        consoleRegion.type("disp('Testing custom prompt')" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        // Restore original prompt
        consoleRegion.type("prompt(original_prompt)" + Key.ENTER);
        consoleRegion.wait(0.5);
        
        consoleRegion.type("// Prompt customization test completed\n");
    }

    @Test
    public void testConsolePaste() throws InterruptedException {
        // Test paste functionality
        
        String testString = "disp('Pasting this text into console')";
        consoleRegion.paste(testString);
        
        consoleRegion.type(Key.ENTER);  // Paste the text
        consoleRegion.wait(0.5);
        consoleRegion.type("// Paste test completed\n");
    }

}