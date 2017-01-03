/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Marcos CARDINOT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.palette.actions;

import static org.scilab.modules.commons.OS.MAC;

import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.ActionMap;
import javax.swing.ImageIcon;
import javax.swing.InputMap;
import javax.swing.JButton;
import javax.swing.KeyStroke;

import org.scilab.modules.commons.OS;
import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Zoom Management
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class ZoomAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String ICON_ZOOMIN = FindIconHelper.findIcon("zoom-in");
    private static final String ICON_ZOOMOUT = FindIconHelper.findIcon("zoom-out");

    private static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
    private static final String ZOOM_IN = "ZoomIn";
    private static final String ZOOM_OUT = "ZoomOut";

    private static JButton btnZoomIn;
    private static JButton btnZoomOut;
    private static MenuItem miZoomIn;
    private static MenuItem miZoomOut;

    /**
     * Constructor
     */
    public ZoomAction() {
        super("");
    }

    /**
     * Register the key for the action
     * @param am the ActionMap
     * @param im the InputMap
     */
    public static void registerKeyAction(ActionMap am, InputMap im) {
        // register the actions to a unique action keyword
        am.put(ZOOM_IN, new ZoomAction());
        am.put(ZOOM_OUT, new ZoomAction());

        // add custom key stroke for these action
        final KeyStroke[] keystrokesIn;
        final KeyStroke[] keystrokesOut;
        if (OS.get() == MAC) {
            // AZERTY for Mac has a non-supported classic layout
            keystrokesIn = new KeyStroke[] {
                KeyStroke.getKeyStroke('/', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('/', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK)
            };
            keystrokesOut = new KeyStroke[] {
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK)
            };
        } else {
            keystrokesIn = new KeyStroke[] {
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke('+', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('+', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke(KeyEvent.VK_ADD, ACCELERATOR_KEY)
            };
            keystrokesOut = new KeyStroke[] {
                KeyStroke.getKeyStroke('-', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('-', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke('_', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('_', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, ACCELERATOR_KEY)
            };
        }

        for (KeyStroke k : keystrokesIn) {
            im.put(k, ZOOM_IN);
        }
        for (KeyStroke k : keystrokesOut) {
            im.put(k, ZOOM_OUT);
        }
    }

    /**
     * Create the button 'zoom in'
     * @return the button
     */
    public static JButton createButtonZoomIn() {
        btnZoomIn = new JButton();
        ScilabLAF.setDefaultProperties(btnZoomIn);
        btnZoomIn.setName(XcosMessages.ZOOM_IN);
        btnZoomIn.setIcon(new ImageIcon(ICON_ZOOMIN));
        btnZoomIn.setToolTipText(XcosMessages.ZOOM_IN);
        btnZoomIn.addActionListener(getCallBack());
        btnZoomIn.setFocusable(true);
        setEnabledZoomIn(true);
        return btnZoomIn;
    }

    /**
     * Create the button 'zoom out'
     * @return the button
     */
    public static JButton createButtonZoomOut() {
        btnZoomOut = new JButton();
        ScilabLAF.setDefaultProperties(btnZoomOut);
        btnZoomOut.setName(XcosMessages.ZOOM_OUT);
        btnZoomOut.setIcon(new ImageIcon(ICON_ZOOMOUT));
        btnZoomOut.setToolTipText(XcosMessages.ZOOM_OUT);
        btnZoomOut.addActionListener(getCallBack());
        btnZoomOut.setFocusable(true);
        setEnabledZoomOut(true);
        return btnZoomOut;
    }

    /**
     * Creates a menu item associated with the 'zoom in' action
     * @return the menuitem
     */
    public static MenuItem createMenuZoomIn() {
        miZoomIn = ScilabMenuItem.createMenuItem();
        miZoomIn.setText(XcosMessages.ZOOM_IN);
        miZoomIn.setCallback(getCallBack());
        SwingScilabMenuItem menu = ((SwingScilabMenuItem) miZoomIn.getAsSimpleMenuItem());
        menu.setIcon(new ImageIcon(ICON_ZOOMIN));
        menu.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_ADD, ACCELERATOR_KEY));
        return miZoomIn;
    }

    /**
     * Creates a menu item associated with the 'zoom out' action
     * @return the menuitem
     */
    public static MenuItem createMenuZoomOut() {
        miZoomOut = ScilabMenuItem.createMenuItem();
        miZoomOut.setText(XcosMessages.ZOOM_OUT);
        miZoomOut.setCallback(getCallBack());
        SwingScilabMenuItem menu = ((SwingScilabMenuItem) miZoomOut.getAsSimpleMenuItem());
        menu.setIcon(new ImageIcon(ICON_ZOOMOUT));
        menu.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, ACCELERATOR_KEY));
        return miZoomOut;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = ZoomAction.class.getConstructor().newInstance();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return callback;
    }

    /**
     * Action
     * @param e ActionEvent
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        String cmd = e.getActionCommand();
        if (cmd.isEmpty()) {
            cmd = ((JButton) e.getSource()).getName();
        } else if ((OS.get() == MAC && cmd.equals("=")) || cmd.equals("-") || cmd.equals("_")) {
            cmd = XcosMessages.ZOOM_OUT;
        } else if (cmd.equals("/") || cmd.equals("+") || cmd.equals("=")) {
            cmd = XcosMessages.ZOOM_IN;
        }

        if (cmd.equals(XcosMessages.ZOOM_IN)) {
            PaletteManagerView.get().getPanel().zoomIn();
        } else if (cmd.equals(XcosMessages.ZOOM_OUT)) {
            PaletteManagerView.get().getPanel().zoomOut();
        }
    }

    /**
     * setEnabled property of the button 'zoomIn'
     * @param enabled enabled
     */
    public static void setEnabledZoomIn(boolean enabled) {
        btnZoomIn.setEnabled(enabled);
        miZoomIn.setEnabled(enabled);
    }

    /**
     * setEnabled property of the button 'zoomOut'
     * @param enabled enabled
     */
    public static void setEnabledZoomOut(boolean enabled) {
        btnZoomOut.setEnabled(enabled);
        miZoomOut.setEnabled(enabled);
    }

    @Override
    public void callBack() {
    }
}
