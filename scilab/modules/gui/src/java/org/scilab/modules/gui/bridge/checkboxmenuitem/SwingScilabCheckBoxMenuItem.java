/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Vincent Couvert
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

package org.scilab.modules.gui.bridge.checkboxmenuitem;

import java.awt.Color;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.UIManager;
import javax.swing.JToggleButton.ToggleButtonModel;
import javax.swing.border.Border;

import org.scilab.modules.commons.utils.StringBlockingResult;
import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.gui.SwingViewMenu;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.SimpleCheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabAlignment;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.ScilabSwingUtilities;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab MenuBars in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabCheckBoxMenuItem extends JCheckBoxMenuItem implements SwingViewObject, SimpleCheckBoxMenuItem {

    private static final long serialVersionUID = 1L;
    private CommonCallBack callback;
    private Menu meAsAMenu;
    private boolean autoCheckedMode = true;
    private String text = "";

    private Integer uid;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabCheckBoxMenuItem() {
        this(true);
    }

    /**
     * Constructor
     * @param autoCheckedMode if false, menu checking is managed by the user (and not automatically by Java)
     */
    public SwingScilabCheckBoxMenuItem(boolean autoCheckedMode) {
        super();
        addActionListener(new ActionListener() {
            /**
             * Action performed ? What do I have to do ?
             * @param arg0 the action
             */
            public void actionPerformed(ActionEvent arg0) {
                StringBlockingResult.getInstance().setResult(((SwingScilabCheckBoxMenuItem) arg0.getSource()).getText());
            }
        });
        this.autoCheckedMode = autoCheckedMode;
        if (!autoCheckedMode) {
            setModel(new ScilabCheckBoxMenuItemModel());
        }
    }

    /**
     * Get the base text used for the Menu
     * @return the text
     */
    public String getBaseText() {
        return this.text;
    }

    /**
     * @param text to use for the menu, if it's enclosed between '$' then it's interpreted as
     * a LaTeX string, in this case the setIcon method of this object is used.
     */
    public void setText(String text) {
        this.text = text;
        if (ScilabSpecialTextUtilities.setText(this, text)) {
            super.setText("");
        } else {
            super.setText(text);
        }
    }

    public void setEmptyText() {
        this.text = null;
        this.setText(null);
    }

    /**
     * Add a callback to the MenuItem, this callback is a Scilab command
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        if (this.callback != null) {
            removeActionListener(this.callback);
        }
        this.callback = callback;
        addActionListener(this.callback);
    }

    /**
     * set a mnemonic to a MenuItem
     * @param mnemonic the mnemonic to add to the MenuItem
     * @see org.scilab.modules.gui.widget.MenuItem#setMnemonic(org.scilab.modules.gui.widget.int)
     */
    public void setMnemonic(int mnemonic) {
        super.setMnemonic(mnemonic);
    }

    /**
     * Add a Scilab MenuItemBar to a Scilab MenuItem
     * @param menuBarToAdd the Scilab MenuBar to add to the Scilab MenuItem
     * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        throw new UnsupportedOperationException();
    }

    /**
     * Add a Scilab Toolbar to a Scilab MenuItem
     * @param toolBarToAdd the Scilab ToolBar to add to the Scilab MenuItem
     * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        throw new UnsupportedOperationException();
    }

    /**
     * Draws a swing Scilab MenuItem
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab MenuItem
     * @return the dimensions of the menu
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab MenuItem
     * @return the position of the menu
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab MenuItem
     * @param newSize the dimensions to set to the menu
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab MenuItem
     * @param newPosition the position to set to the menu
     * @see org.scilab.modules.gui.UIElement#setPosition(org.scilab.modules.gui.utils.Position)
     */
    public void setPosition(Position newPosition) {
        throw new UnsupportedOperationException();
    }

    /**
     * Get the id of the menu bar associated to the menu (not the parent menubar)
     * @return the menubar
     * @see org.scilab.modules.gui.uielement.UIElement#getMenuBarId()
     */
    public MenuBar getMenuBar() {
        throw new UnsupportedOperationException();
    }

    /**
     * Get the id of the tool bar associated to the menu
     * @return the toolbar
     * @see org.scilab.modules.gui.uielement.UIElement#getToolBarId()
     */
    public ToolBar getToolBar() {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        setHorizontalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        setVerticalAlignment(ScilabAlignment.toSwingAlignment(alignment));
    }

    /**
     * Set the Relief of the Menu
     * @param reliefType the type of the relief to set (See ScilabRelief.java)
     */
    public void setRelief(String reliefType) {
        if (defaultBorder == null) {
            defaultBorder = getBorder();
        }
        setBorder(ScilabRelief.getBorderFromRelief(reliefType, defaultBorder));
    }

    /**
     * Add a MenuItem to this MenuItem
     * @param childMenuItem the MenuItem we want to add
     */
    public void add(MenuItem childMenuItem) {
        if (meAsAMenu == null) {
            meAsAMenu = ScilabMenu.createMenu();
            meAsAMenu.setText(getText());
            meAsAMenu.add(childMenuItem);
            Container parent = getParent();
            int index = parent.getComponentZOrder(this);
            parent.remove(this.getComponent());
            parent.add((SwingScilabMenu) meAsAMenu.getAsSimpleMenu(), index);
        } else {
            meAsAMenu.add(childMenuItem);
        }
    }

    /**
     * Destroy the MenuItem
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Set if the menu item is enabled or not
     * @param status true if the menu item is enabled
     */
    public void setEnabled(boolean status) {
        super.setEnabled(status);
        /* (Des)Activate the callback */
        if (callback != null) {
            if (status) {
                removeActionListener(callback); /* To be sure the callback is not added two times */
                addActionListener(callback);
            } else {
                removeActionListener(callback);
            }
        }
    }

    /**
     * Add a Menu to this MenuItem
     * @param childMenu the Menu we want to add
     */
    public void add(Menu childMenu) {
        if (meAsAMenu == null) {
            meAsAMenu = ScilabMenu.createMenu();
            meAsAMenu.setText(getText());
            meAsAMenu.add(childMenu);
            Container parent = getParent();
            int index = parent.getComponentZOrder(this);
            parent.remove(this.getComponent());
            parent.add((SwingScilabMenu) meAsAMenu.getAsSimpleMenu(), index);
        } else {
            meAsAMenu.add(childMenu);
        }
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the MenuItem.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for MenuItems */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the MenuItem.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for MenuItems */
        throw new UnsupportedOperationException();
    }

    /**
     * Retrieve the CallBack associated to this MenuItem
     * @return the CallBack
     */
    public CommonCallBack getCallback() {
        return callback;
    }
    /**
     * Set if the menu item is checked or not
     * @param status true if the menu item is checked
     */
    public void setChecked(boolean status) {
        if (autoCheckedMode) {
            setSelected(status);
        } else {
            ((ScilabCheckBoxMenuItemModel) getModel()).forceSelected(status);
        }
    }

    /**
     * Get if the menu item is checked or not
     * @return true if the menu item is checked
     */
    public boolean isChecked() {
        return isSelected();
    }

    /**
     * Append a CheckBoxMenuItem to a Scilab CheckBoxMenuItem
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the CheckBoxMenuItem
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.CheckBoxMenuItem)
     */
    public void add(CheckBoxMenuItem newCheckBoxMenuItem) {
        if (meAsAMenu == null) {
            meAsAMenu = ScilabMenu.createMenu();
            meAsAMenu.setText(getText());
            meAsAMenu.add(newCheckBoxMenuItem);
            Container parent = getParent();
            int index = parent.getComponentZOrder(this);
            parent.remove(this.getComponent());
            parent.add((SwingScilabMenu) meAsAMenu.getAsSimpleMenu(), index);
        } else {
            meAsAMenu.add(newCheckBoxMenuItem);
        }
    }

    /**
     * Add a separator
     * @see org.scilab.modules.gui.menu.SimpleMenu#addSeparator()
     */
    public void addSeparator() {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the UID
     * @param id the UID
     */
    public void setId(Integer id) {
        uid = id;
    }

    /**
     * Get the UID
     * @return the UID
     */
    public Integer getId() {
        return uid;
    }

    /**
     * Generic update method
     * @param property property name
     * @param value property value
     */
    public void update(int property, Object value) {
        SwingViewMenu.update(this, property, value);
    }

    /**
     * Overload default Model so that automatic check/uncheck is disabled
     * Checking is only managed by the user
     * See bug #7364
     * @author Vincent COUVERT
     */
    private class ScilabCheckBoxMenuItemModel extends ToggleButtonModel {

        private static final long serialVersionUID = 1L;

        /**
         * Constructor
         */
        public ScilabCheckBoxMenuItemModel() {
            super();
        }

        /**
         * Using this method is forbidden
         * Use forceSelected instead
         * @param status the new selection status
         * @see javax.swing.JToggleButton.ToggleButtonModel#setSelected(boolean)
         */
        public void setSelected(boolean status) {
            // Does nothing
        }

        /**
         * Set checked status
         * @param status the new selection status
         */
        public void forceSelected(boolean status) {
            super.setSelected(status);
        }

    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("CheckBoxMenuItem.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("CheckBoxMenuItem.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
