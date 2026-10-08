/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
 */

package org.scilab.modules.scinotes.actions;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.WeakHashMap;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.KeyStroke;

import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.scinotes.MonacoEditorPane;
import org.scilab.modules.scinotes.SciNotes;
import org.scilab.modules.scinotes.ScilabEditorPane;

/** Shared implementation for the SciNotes debug execution actions. */
public abstract class DebugCommandAction extends DefaultAction {

    private static final long serialVersionUID = 8360651963901973344L;

    private static final Map<SciNotes, List<Binding>> BINDINGS =
        new WeakHashMap<SciNotes, List<Binding>>();
    private static final Map<SciNotes, Map<String, Boolean>> AVAILABILITY =
        new WeakHashMap<SciNotes, Map<String, Boolean>>();

    /** Availability key of the actions that start a run, not a debug step. */
    private static final String EXECUTE = "execute";

    private final String command;

    /**
     * @param name action label
     * @param editor associated SciNotes instance
     * @param command DAP command
     */
    protected DebugCommandAction(String name, SciNotes editor, String command) {
        super(name, editor);
        this.command = command;
    }

    @Override
    public void doAction() {
        ScilabEditorPane pane = getEditor().getTextPane();
        if (pane instanceof MonacoEditorPane) {
            ((MonacoEditorPane) pane).runDebuggerCommand(command);
        }
    }

    /**
     * Create and register a state-aware debug menu item.
     * @param label item label
     * @param editor associated SciNotes instance
     * @param key keyboard shortcut
     * @param action action callback
     * @return configured menu item
     */
    protected static MenuItem createDebugMenu(String label, SciNotes editor, KeyStroke key,
            DebugCommandAction action) {
        MenuItem item = createMenu(label, null, action, key);
        register(editor, action.command, (JComponent) item.getAsSimpleMenuItem());
        return item;
    }

    /**
     * Create and register a state-aware debug toolbar button.
     * @param tooltip button tooltip
     * @param icon button icon
     * @param editor associated SciNotes instance
     * @param action action callback
     * @return configured button
     */
    protected static JButton createDebugButton(String tooltip, String icon, SciNotes editor,
            DebugCommandAction action) {
        JButton button = createButton(tooltip, icon, action);
        register(editor, action.command, button);
        return button;
    }

    /**
     * Update the enabled state of every debug control belonging to one window.
     * @param editor associated SciNotes instance
     * @param canContinue whether Debug or resume is enabled
     * @param canPause whether Pause is enabled
     * @param canStepOver whether Step Over is enabled
     * @param canStepIn whether Step In is enabled
     * @param canStepOut whether Step Out is enabled
     * @param canDisconnect whether Stop is enabled
     * @param canExecute whether starting a run is possible
     */
    public static synchronized void updateAvailability(SciNotes editor, boolean canContinue,
            boolean canPause, boolean canStepOver, boolean canStepIn, boolean canStepOut,
            boolean canDisconnect, boolean canExecute) {
        Map<String, Boolean> values = new HashMap<String, Boolean>();
        // Continue also starts a debug run, when there is none to continue.
        values.put("continue", canContinue);
        values.put("pause", canPause);
        values.put("next", canStepOver);
        values.put("stepIn", canStepIn);
        values.put("stepOut", canStepOut);
        values.put("disconnect", canDisconnect);
        values.put(EXECUTE, canExecute);
        AVAILABILITY.put(editor, values);

        List<Binding> bindings = BINDINGS.get(editor);
        if (bindings == null) {
            return;
        }

        Iterator<Binding> iterator = bindings.iterator();
        while (iterator.hasNext()) {
            Binding binding = iterator.next();
            JComponent component = binding.component.get();
            if (component == null) {
                iterator.remove();
            } else {
                component.setEnabled(Boolean.TRUE.equals(values.get(binding.command)));
            }
        }
    }

    /**
     * Register a control that starts a run. Unlike a debug control it stays
     * enabled until a session says otherwise: without a debug session, running
     * a file is always possible.
     * @param editor associated SciNotes instance
     * @param component menu item or button to follow the execution state
     */
    public static synchronized void registerExecuteControl(SciNotes editor, JComponent component) {
        List<Binding> bindings = BINDINGS.get(editor);
        if (bindings == null) {
            bindings = new ArrayList<Binding>();
            BINDINGS.put(editor, bindings);
        }
        bindings.add(new Binding(EXECUTE, component));

        Map<String, Boolean> values = AVAILABILITY.get(editor);
        component.setEnabled(values == null || Boolean.TRUE.equals(values.get(EXECUTE)));
    }

    private static synchronized void register(SciNotes editor, String command, JComponent component) {
        List<Binding> bindings = BINDINGS.get(editor);
        if (bindings == null) {
            bindings = new ArrayList<Binding>();
            BINDINGS.put(editor, bindings);
        }
        bindings.add(new Binding(command, component));

        Map<String, Boolean> values = AVAILABILITY.get(editor);
        component.setEnabled(values == null ? "continue".equals(command) : Boolean.TRUE.equals(values.get(command)));
    }

    private static final class Binding {
        private final String command;
        private final WeakReference<JComponent> component;

        private Binding(String command, JComponent component) {
            this.command = command;
            this.component = new WeakReference<JComponent>(component);
        }
    }
}
