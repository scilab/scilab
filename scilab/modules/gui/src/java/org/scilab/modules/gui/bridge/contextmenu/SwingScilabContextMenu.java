/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent Couvert
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

package org.scilab.modules.gui.bridge.contextmenu;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;

import java.awt.Color;
import java.awt.MouseInfo;

import javax.swing.JPopupMenu;
import javax.swing.UIManager;
import javax.swing.border.Border;
import javax.swing.event.PopupMenuEvent;
import javax.swing.event.PopupMenuListener;

import org.scilab.modules.commons.utils.StringBlockingResult;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.contextmenu.SimpleContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.ScilabRelief;
import org.scilab.modules.gui.utils.Size;

/**
 * Swing implementation for Scilab Menus in GUIs
 * @author Vincent COUVERT
 */
public class SwingScilabContextMenu extends JPopupMenu implements SwingViewObject, SimpleContextMenu {

    private static final long serialVersionUID = 1L;

    private Integer uid;

    private boolean checkedState;

    private Border defaultBorder = null;

    /**
     * Constructor
     */
    public SwingScilabContextMenu() {
        super();
        setInvoker(this);
        addPopupMenuListener(new PopupMenuListener() {
            public void popupMenuWillBecomeVisible(PopupMenuEvent arg0) {
            }

            public void popupMenuWillBecomeInvisible(PopupMenuEvent arg0) {
                if (uid != null) {
                    if (GraphicController.getController().getObjectFromId(uid) != null) {
                        GraphicController.getController().removeRelationShipAndDelete(uid);
                    }
                }
            }

            public void popupMenuCanceled(PopupMenuEvent arg0) {
                if (uid != null) {
                    if (GraphicController.getController().getObjectFromId(uid) != null) {
                        GraphicController.getController().removeRelationShipAndDelete(uid);
                    }
                }
            }
        });
    }

    /**
     * Get the base text used for the Menu
     * @return the text
     */
    public String getBaseText() {
        return "";
    }

    /**
     * Makes the menu visible as the mouse pointer position
     */
    private void setVisibleAndLocation() {
        if (!isVisible()) {
            setVisible(true);
            setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo().getLocation().y);
        }
    }

    /**
     * Append a MenuItem to a Scilab Menu
     * @param newMenuItem the MenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
     */
    public void add(MenuItem newMenuItem) {
        super.add((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem());
    }

    /**
     * Append a CheckBoxMenuItem to a Scilab Menu
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.CheckBoxMenuItem)
     */
    public void add(CheckBoxMenuItem newCheckBoxMenuItem) {
        super.add((SwingScilabCheckBoxMenuItem) newCheckBoxMenuItem.getAsSimpleCheckBoxMenuItem());
    }

    /**
     * Display the ContextMenu
     * @return the label of the menu selected
     */
    public String displayAndWait() {
        return StringBlockingResult.getInstance().getResult();
    }

    /**
     * Add a Scilab MenuBar to a Scilab menu
     * @param menuBarToAdd the Scilab MenuBar to add to the Scilab menu
     * @see org.scilab.modules.gui.window.Window#setMenuBar(org.scilab.modules.gui.menubar.MenuBar)
     */
    public void addMenuBar(MenuBar menuBarToAdd) {
        throw new UnsupportedOperationException();
    }

    /**
     * Add a Scilab Toolbar to a Scilab menu
     * @param toolBarToAdd the Scilab ToolBar to add to the Scilab menu
     * @see org.scilab.modules.gui.window.Window#setToolBar(org.scilab.modules.gui.menubar.ToolBar)
     */
    public void addToolBar(ToolBar toolBarToAdd) {
        throw new UnsupportedOperationException();
    }

    /**
     * Draws a swing Scilab menu
     * @see org.scilab.modules.gui.UIElement#draw()
     */
    public void draw() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets the dimensions (width and height) of a swing Scilab menu
     * @return the dimensions of the menu
     * @see org.scilab.modules.gui.UIElement#getDims()
     */
    public Size getDims() {
        throw new UnsupportedOperationException();
    }

    /**
     * Gets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
     * @return the position of the menu
     * @see org.scilab.modules.gui.UIElement#getPosition()
     */
    public Position getPosition() {
        throw new UnsupportedOperationException();
    }

    /**
     * Sets the dimensions (width and height) of a swing Scilab menu
     * @param newSize the dimensions to set to the menu
     * @see org.scilab.modules.gui.UIElement#setDims(org.scilab.modules.gui.utils.Size)
     */
    public void setDims(Size newSize) {
        throw new UnsupportedOperationException();
    }

    /**
     * Sets the position (X-coordinate and Y-coordinate) of a swing Scilab menu
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
     * Add a callback to the menu, this callback is a Scilab command
     * @param callback the callback to set.
     */
    public void setCallback(CommonCallBack callback) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the horizontal alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setHorizontalAlignment(String alignment) {
        throw new UnsupportedOperationException();
    }

    /**
     * Set the vertical alignment for the Menu text
     * @param alignment the value for the alignment (See ScilabAlignment.java)
     */
    public void setVerticalAlignment(String alignment) {
        throw new UnsupportedOperationException();
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
     * Destroy the Menu
     * @see org.scilab.modules.gui.menuitem.SimpleMenuItem#destroy()
     */
    public void destroy() {
        getParent().remove(this);
    }

    /**
     * Set if the menu is enabled or not
     * @param status true if the menu is enabled
     */
    public void setEnabled(boolean status) {
        super.setEnabled(status);
    }

    /**
     * Not used
     * @param mnemonic not used
     */
    public void setMnemonic(char mnemonic) {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @param mnemonic not used
     */
    public void setMnemonic(int mnemonic) {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @return null
     */
    public String getText() {
        throw new UnsupportedOperationException();
    }

    /**
     * Not used
     * @param text not used
     */
    public void setText(String text) {
        throw new UnsupportedOperationException();
    }

    public void setEmptyText() {
        throw new UnsupportedOperationException();
    }

    /**
     * Add a Menu to this MenuItem
     * @param childMenu the Menu we want to add
     */
    public void add(Menu childMenu) {
        super.add((SwingScilabMenu) childMenu.getAsSimpleMenu());
    }

    /**
     * Default destruction action
     * @see javax.swing.JPopupMenu#firePopupMenuCanceled()
     */
    public void firePopupMenuCanceled() {
        StringBlockingResult.getInstance().setResult("");
        super.firePopupMenuCanceled();
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the ContextMenu.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for ContextMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the ContextMenu.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for ContextMenus */
        throw new UnsupportedOperationException();
    }

    /**
     * Retrieve the CallBack associated to this ContextMenu
     * @return the CallBack
     */
    public CommonCallBack getCallback() {
        throw new UnsupportedOperationException();
    }

    /**
     * Set if the Menu is checked or not
     * @param status true if the Menu is checked
     */
    public void setChecked(boolean status) {
        checkedState = status;
    }

    /**
     * Get if the Menu is checked or not
     * @return true if the Menu is checked
     */
    public boolean isChecked() {
        return checkedState;
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
        if (property == __GO_CHILDREN__) {
            setVisibleAndLocation();
            pack();
        }
    }

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("PopupMenu.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("PopupMenu.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
