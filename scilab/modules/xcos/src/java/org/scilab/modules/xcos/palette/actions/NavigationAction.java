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

import java.awt.event.ActionEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Navigation menu management.
 * Actions 'next' and 'previous' of the palette browser.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class NavigationAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String ICON_NEXT = FindIconHelper.findIcon("go-next");
    private static final String ICON_PREV = FindIconHelper.findIcon("go-previous");

    private static JButton btnNEXT;
    private static JButton btnPREV;
    private static MenuItem miNEXT;
    private static MenuItem miPREV;

    /**
     * Constructor
     */
    public NavigationAction() {
        super("");
    }

    /**
     * Create the button 'next'
     * @return the button
     */
    public static JButton createButtonNext() {
        btnNEXT = new JButton();
        ScilabLAF.setDefaultProperties(btnNEXT);
        btnNEXT.setName(XcosMessages.NEXT);
        btnNEXT.setIcon(new ImageIcon(ICON_NEXT));
        btnNEXT.setToolTipText(XcosMessages.NEXT);
        btnNEXT.addActionListener(getCallBack());
        btnNEXT.setFocusable(true);
        setEnabledNext(false);
        return btnNEXT;
    }

    /**
     * Create the button 'previous'
     * @return the button
     */
    public static JButton createButtonPrev() {
        btnPREV = new JButton();
        ScilabLAF.setDefaultProperties(btnPREV);
        btnPREV.setName(XcosMessages.PREVIOUS);
        btnPREV.setIcon(new ImageIcon(ICON_PREV));
        btnPREV.setToolTipText(XcosMessages.PREVIOUS);
        btnPREV.addActionListener(getCallBack());
        btnPREV.setFocusable(true);
        setEnabledPrev(false);
        return btnPREV;
    }

    /**
     * Creates a menu item associated with the 'next' action
     * @return the menuitem
     */
    public static MenuItem createMenuNext() {
        miNEXT = ScilabMenuItem.createMenuItem();
        miNEXT.setText(XcosMessages.NEXT);
        miNEXT.setMnemonic('N');
        miNEXT.setCallback(getCallBack());
        ((SwingScilabMenuItem) miNEXT.getAsSimpleMenuItem()).setIcon(new ImageIcon(ICON_NEXT));
        return miNEXT;
    }

    /**
     * Creates a menu item associated with the 'previous' action
     * @return the menuitem
     */
    public static MenuItem createMenuPrev() {
        miPREV = ScilabMenuItem.createMenuItem();
        miPREV.setText(XcosMessages.PREVIOUS);
        miPREV.setMnemonic('P');
        miPREV.setCallback(getCallBack());
        ((SwingScilabMenuItem) miPREV.getAsSimpleMenuItem()).setIcon(new ImageIcon(ICON_PREV));
        return miPREV;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = NavigationAction.class.getConstructor().newInstance();
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
        }

        if (cmd.equals(XcosMessages.NEXT)) {
            PaletteManagerView.get().getPanel().goNext();
        } else if (cmd.equals(XcosMessages.PREVIOUS)) {
            PaletteManagerView.get().getPanel().goPrevious();
        }
    }

    /**
     * setEnabled property of the button 'next'
     * @param enabled enabled
     */
    public static void setEnabledNext(boolean enabled) {
        btnNEXT.setEnabled(enabled);
        miNEXT.setEnabled(enabled);
    }

    /**
     * setEnabled property of the button 'previous'
     * @param enabled enabled
     */
    public static void setEnabledPrev(boolean enabled) {
        btnPREV.setEnabled(enabled);
        miPREV.setEnabled(enabled);
    }

    @Override
    public void callBack() {
    }
}
