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

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.lang.reflect.InvocationTargetException;

import javax.swing.ComboBoxEditor;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;
import javax.swing.plaf.basic.BasicComboBoxEditor;
import javax.swing.plaf.metal.MetalComboBoxUI;

import org.scilab.modules.commons.gui.FindIconHelper;
import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.XcosMessages;

/**
 * Search Palettes Action
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
public class SearchAction extends CommonCallBack {

    private static final long serialVersionUID = 1L;

    private static final String LABEL_SEARCH = XcosMessages.SEARCH;
    private static final String ICON_SEARCH = FindIconHelper.findIcon("system-search");

    private static JButton btnSearch;
    private static JComboBox txtSearch;
    private static JPanel searchBar;

    /**
     * Constructor
     */
    public SearchAction() {
        super("");
    }

    /**
     * Create the search bar
     * @return JPanel
     */
    public static JPanel createSearchBar() {
        searchBar = new JPanel();
        searchBar.setLayout(new FlowLayout(FlowLayout.LEFT, 0, 0));

        txtSearch = new JComboBox();
        txtSearch.setUI(new SearchUI());
        searchBar.add(txtSearch);

        btnSearch = new JButton();
        ScilabLAF.setDefaultProperties(btnSearch);
        btnSearch.setIcon(new ImageIcon(ICON_SEARCH));
        btnSearch.setToolTipText(LABEL_SEARCH);
        btnSearch.addActionListener(getCallBack());
        btnSearch.setFocusable(true);
        btnSearch.setBorderPainted(false);
        searchBar.add(btnSearch);

        return searchBar;
    }

    /**
     * Create a new class instance
     * @return the instance
     */
    private static CommonCallBack getCallBack() {
        CommonCallBack callback = null;
        try {
            callback = SearchAction.class.getConstructor().newInstance();
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
        search();
    }

    @Override
    public void callBack() {
    }

    /**
     * Search
     */
    protected static void search() {
        String query = txtSearch.getSelectedItem().toString();
        if (query.isEmpty()) {
            return;
        }

        PaletteManagerView.get().getSearchManager().search(query);
        Object lastItem = txtSearch.getItemAt(0);
        if (lastItem == null || !lastItem.toString().equals(query)) {
            txtSearch.insertItemAt(query, 0); // insert on top
        }
    }
}

/**
 * Implements a comboboxui
 * @author Marcos CARDINOT <mcardinot@gmail.com>
 */
class SearchUI extends MetalComboBoxUI {

    /**
     * Default constructor
     */
    public SearchUI() {
        super();
    }

    /**
     * Hides the arrow button
     * @return JButton
     */
    @Override
    @SuppressWarnings("serial")
    protected JButton createArrowButton() {
        JButton btn = new JButton() {
            @Override
            public int getWidth() {
                return 0;
            }
        };
        btn.setVisible(false);
        return btn;
    }

    /**
     * Create the box editor
     * @return ComboBoxEditor
     */
    @Override
    protected ComboBoxEditor createEditor() {
        BasicComboBoxEditor editor = (BasicComboBoxEditor) super.createEditor();
        JTextField txtField = (JTextField) editor.getEditorComponent();

        comboBox.setEditable(true);

        final int height = txtField.getPreferredSize().height - 2;
        final int width = 150;
        Dimension dim = new Dimension(width, height);
        comboBox.setPreferredSize(dim);
        txtField.setPreferredSize(dim);

        final int tb = 0;
        final int lr = 5;
        txtField.setBorder(new EmptyBorder(tb, lr, tb, lr));

        txtField.addKeyListener(new KeyAdapter() {
            @Override
            public void keyReleased(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    SearchAction.search();
                } else if (e.getKeyCode() != KeyEvent.VK_UP
                           && e.getKeyCode() != KeyEvent.VK_DOWN) {
                    comboBox.hidePopup();
                }
            }
        });
        return editor;
    }
}
