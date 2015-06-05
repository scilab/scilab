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
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Zoom Out Management
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class ZoomOutAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL = XcosMessages.ZOOM_OUT;
    private static final String ICON = FindIconHelper.findIcon("zoom-out");

    private static final int ACCELERATOR_KEY = Toolkit.getDefaultToolkit().getMenuShortcutKeyMask();
    private static final String ZOOM_OUT = "ZoomOut";

    /**
     * Constructor
     */
    public ZoomOutAction() {
        super("");
    }

    /**
     * Register the key for the action
     * @param view PaletteManagerPanel instance
     */
    public static void registerKeyAction(PaletteManagerView view) {
        // Multi-shortcut action
        ActionMap[] ams = new ActionMap[] {
                view.getTree().getActionMap(),
                view.getPanel().getActionMap()
        };
        InputMap[] maps = new InputMap[] {
                view.getTree().getInputMap(),
                view.getPanel().getInputMap()
        };

        // register the action to a unique action keyword
        for (ActionMap am : ams) {
            am.put(ZOOM_OUT, new ZoomOutAction());
        }

        // add custom key stroke for this action
        final KeyStroke[] keystrokes;
        if (OS.get() == MAC) {
            // AZERTY for Mac has a non-supported classic layout
            keystrokes = new KeyStroke[] {
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('=', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK)
            };
        } else {
            keystrokes = new KeyStroke[] {
                KeyStroke.getKeyStroke('-', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('-', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke('_', ACCELERATOR_KEY),
                KeyStroke.getKeyStroke('_', ACCELERATOR_KEY | KeyEvent.SHIFT_DOWN_MASK),
                KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, ACCELERATOR_KEY)
            };
        }

        for (InputMap map : maps) {
            for (KeyStroke k : keystrokes) {
                map.put(k, ZOOM_OUT);
            }
        }
    }

    /**
     * Create the associated button
     * @return the button
     */
    public static JButton createPushButton() {
        JButton pushButton = new JButton();
        ScilabLAF.setDefaultProperties(pushButton);
        pushButton.setIcon(new ImageIcon(ICON));
        pushButton.setToolTipText(LABEL);
        pushButton.addActionListener(getCallBack());
        return pushButton;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = ZoomOutAction.class.getConstructor().newInstance();
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
     * Action!
     * @see org.scilab.modules.gui.events.callback.CallBack#callBack()
     */
    @Override
    public void callBack() {
        PaletteManagerView.get().getPanel().zoomOut();
    }
}
