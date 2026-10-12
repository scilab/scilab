/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.gui.utils;

import java.awt.Color;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.bridge.textbox.SwingScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.localization.Messages;

/**
 * Shows in a status bar that Scilab is in debug mode: one colour for the
 * console and for the editor, and what the debugger says after the text of
 * the bar.
 */
public final class DebugModeIndicator {

    private static final Color BACKGROUND = new Color(255, 204, 102);
    private static final Color ERROR_BACKGROUND = new Color(255, 153, 153);

    // the states InterpreterManagement.getScilabDebugState() returns
    private static final int AT_REST = 1;
    private static final int RUNNING = 2;
    private static final int STOPPED = 3;

    private DebugModeIndicator() {
    }

    /**
     * @return what the debugger of Scilab is doing, empty outside debug mode
     */
    public static String getState() {
        switch (InterpreterManagement.getScilabDebugState()) {
            case RUNNING:
                return Messages.gettext("Debug mode: execution running");
            case STOPPED:
                return Messages.gettext("Debug mode: execution paused");
            case AT_REST:
                return Messages.gettext("Debug mode");
            default:
                return "";
        }
    }

    /**
     * Give a status bar the colour of debug mode, or its own back, and keep
     * what the debugger says after its text.
     * @param infoBar the status bar, may be null
     * @param debugMode whether Scilab is in debug mode
     * @param state what the debugger says, empty for nothing
     */
    public static void show(final TextBox infoBar, final boolean debugMode, final String state) {
        show(infoBar, debugMode, false, state);
    }

    /**
     * Same, in the colour of an error when the execution is stopped on one.
     * @param infoBar the status bar, may be null
     * @param debugMode whether Scilab is in debug mode
     * @param error whether the execution is stopped on an error
     * @param state what the debugger says, empty for nothing
     */
    public static void show(final TextBox infoBar, final boolean debugMode, final boolean error,
            final String state) {
        if (infoBar == null) {
            return;
        }

        if (!SwingUtilities.isEventDispatchThread()) {
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    show(infoBar, debugMode, error, state);
                }
            });
            return;
        }

        if (infoBar.getAsSimpleTextBox() instanceof SwingScilabTextBox) {
            ((SwingScilabTextBox) infoBar.getAsSimpleTextBox()).setTrailer(state);
        }

        // A status bar is not opaque by default: its background would not be painted.
        Object component = infoBar.getAsSimpleTextBox();
        if (component instanceof JComponent) {
            ((JComponent) component).setOpaque(debugMode);
        }
        if (debugMode) {
            infoBar.setBackground(error ? ERROR_BACKGROUND : BACKGROUND);
        } else {
            infoBar.resetBackground();
        }
        if (component instanceof JComponent) {
            ((JComponent) component).repaint();
        }
    }
}
