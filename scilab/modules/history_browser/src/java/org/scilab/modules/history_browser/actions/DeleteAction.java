/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

package org.scilab.modules.history_browser.actions;

import java.lang.reflect.InvocationTargetException;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.KeyStroke;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.MutableTreeNode;
import javax.swing.tree.TreePath;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.history_browser.CommandHistory;
import org.scilab.modules.history_browser.CommandHistoryMessages;
import org.scilab.modules.history_manager.HistoryManagement;

/**
 * Manage Delete Actions
 * @author Vincent COUVERT
 */
public final class DeleteAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL = CommandHistoryMessages.DELETE;
    private static final String ICON = FindIconHelper.findIcon("edit-delete");
    private static final char MNEMONIC = 'D';

    private static final String KEY = "DELETE";
    private static final String SUPPR = "Suppr";

    /**
     * Constructor
     */
    public DeleteAction() {
        super("");
    }

    /**
     * Register the key for the action
     */
    public static void registerKeyAction() {
        CommandHistory.getTree().getActionMap().put(SUPPR, new DeleteAction());
        CommandHistory.getTree().getInputMap().put(KeyStroke.getKeyStroke(KEY), SUPPR);
    }

    /**
     * Create the associated menu
     * @return the menu
     */
    public static MenuItem createMenuItem() {
        MenuItem menuItem = ScilabMenuItem.createMenuItem();
        menuItem.setText(LABEL);
        menuItem.setMnemonic(MNEMONIC);
        menuItem.setCallback(getCallBack());
        ((SwingScilabMenuItem) menuItem.getAsSimpleMenuItem()).setIcon(new ImageIcon(ICON));
        return menuItem;
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
            callback = DeleteAction.class.getConstructor().newInstance();
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
        TreePath[] currentSelection = CommandHistory.getTree().getSelectionPaths();
        if (currentSelection != null) {
            for (int i = 0; i < currentSelection.length; i++) {
                DefaultMutableTreeNode currentNode = (DefaultMutableTreeNode) (currentSelection[i].getLastPathComponent());
                if (currentNode != null) {
                    MutableTreeNode parent = (MutableTreeNode) currentNode.getParent();
                    if (parent != null) {
                        if (parent.getParent() == null) {
                            /*
                             * A whole session has been selected
                             */

                            /* parent == rootNode */
                            int linePosition = 0;
                            for (int j = 0; j < parent.getIndex(currentNode); j++) {
                                linePosition++; /* Session beginning line */
                                linePosition += parent.getChildAt(j).getChildCount();
                            }

                            /* Have to remove all this session commands */
                            int childCount = currentNode.getChildCount(); /* Save child count because will change when removing line */
                            for (int j = childCount; j > 0; j--) {
                                HistoryManagement.deleteNthLineScilabHistory(linePosition + j);
                            }
                            HistoryManagement.deleteNthLineScilabHistory(linePosition);

                        } else {
                            /*
                             * A command has been selected
                             */

                            /* Compute absolute line index */
                            DefaultMutableTreeNode rootNode = (DefaultMutableTreeNode) CommandHistory.getTreeModel().getRoot();
                            int parentPosition = rootNode.getIndex(parent);
                            int linePosition = 0;
                            for (int j = 0; j < parentPosition; j++) {
                                linePosition++; /* Session beginning line */
                                linePosition += rootNode.getChildAt(j).getChildCount();
                            }
                            linePosition++; /* Parent session beginning line */
                            linePosition += parent.getIndex(currentNode);

                            /* Remove the line */
                            HistoryManagement.deleteNthLineScilabHistory(linePosition);
                        }
                    }
                }
            }
        }
    }
}
