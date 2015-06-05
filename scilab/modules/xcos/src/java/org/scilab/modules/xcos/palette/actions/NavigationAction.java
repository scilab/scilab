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
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Navigation menu management.
 * Actions 'next' and 'previous' of the palette browser.
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class NavigationAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL_NEXT = XcosMessages.NEXT;
    private static final String LABEL_PREV = XcosMessages.PREVIOUS;
    private static final String ICON_NEXT = FindIconHelper.findIcon("go-next");
    private static final String ICON_PREV = FindIconHelper.findIcon("go-previous");

    private static JButton btnNEXT;
    private static JButton btnPREV;

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
        btnNEXT.setIcon(new ImageIcon(ICON_NEXT));
        btnNEXT.setToolTipText(LABEL_NEXT);
        btnNEXT.addActionListener(getCallBack());
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
        btnPREV.setIcon(new ImageIcon(ICON_PREV));
        btnPREV.setToolTipText(LABEL_PREV);
        btnPREV.addActionListener(getCallBack());
        setEnabledPrev(false);
        return btnPREV;
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
    public void actionPerformed(ActionEvent e) {
        Object src = e.getSource();
        if (btnNEXT != null && src.equals(btnNEXT)) {
            PaletteManagerView.get().getPanel().goNext();
        } else if (btnPREV != null && src.equals(btnPREV)) {
            PaletteManagerView.get().getPanel().goPrevious();
        }
    }

    /**
     * setEnabled property of the button 'next'
     * @param enabled enabled 
     */
    public static void setEnabledNext(boolean enabled) {
        btnNEXT.setEnabled(enabled);
    }

    /**
     * setEnabled property of the button 'previous'
     * @param enabled enabled 
     */
    public static void setEnabledPrev(boolean enabled) {
        btnPREV.setEnabled(enabled);
    }

    @Override
    public void callBack() {
    }
}
