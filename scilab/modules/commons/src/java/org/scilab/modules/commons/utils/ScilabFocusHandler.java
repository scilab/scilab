/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.commons.utils;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import javax.swing.SwingUtilities;

import org.cef.browser.CefBrowser;
import org.cef.handler.CefFocusHandler;

/**
 * Focus handler installed on every CefClient created by {@link ScilabBrowser}.
 *
 * <p>CefClient.onGotFocus() calls browser.setFocus(true) before delegating to
 * any handler, and CEF then grants focus again: left alone, the recursion
 * exhausts the AppKit thread stack on macOS. onSetFocus is the only cut point,
 * since CEF consults it before granting and returning true cancels the grant.
 *
 * <p>Handlers added through
 * {@link ScilabBrowser#addFocusHandler(org.cef.CefClient, CefFocusHandler)}
 * are chained behind that guard, so no caller can register one that bypasses
 * it. CefClient only accepts a single focus handler, hence the chaining.
 */
public final class ScilabFocusHandler implements CefFocusHandler {

    private final List<CefFocusHandler> delegates = new CopyOnWriteArrayList<>();

    /** True while a focus grant is already being delivered to this client. */
    private volatile boolean focusGrantInProgress = false;

    ScilabFocusHandler() { }

    void add(CefFocusHandler handler) {
        delegates.add(handler);
    }

    @Override
    public boolean onSetFocus(CefBrowser browser, FocusSource source) {
        if (focusGrantInProgress) {
            return true;
        }
        focusGrantInProgress = true;
        SwingUtilities.invokeLater(() -> focusGrantInProgress = false);

        for (CefFocusHandler handler : delegates) {
            if (handler.onSetFocus(browser, source)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public void onGotFocus(CefBrowser browser) {
        for (CefFocusHandler handler : delegates) {
            handler.onGotFocus(browser);
        }
    }

    @Override
    public void onTakeFocus(CefBrowser browser, boolean next) {
        for (CefFocusHandler handler : delegates) {
            handler.onTakeFocus(browser, next);
        }
    }
}
