/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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

package org.scilab.modules.gui.bridge.menu;

import java.awt.Color;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JMenu;
import javax.swing.UIManager;
import javax.swing.border.Border;

import org.scilab.modules.console.utils.ScilabSpecialTextUtilities;
import org.scilab.modules.gui.SwingViewMenu;
import org.scilab.modules.gui.SwingViewObject;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.SimpleMenu;
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
 * Swing implementation for Scilab Menus in GUIs
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 */
public class SwingScilabMenu extends JMenu implements SwingViewObject, SimpleMenu {

    private static final long serialVersionUID = 1L;

    private CommonCallBack callback;
    private MouseListener[] nativeMouseListeners;
    private MouseListener customedMouseListener;
    private boolean checkedState;
    private String text = "";
    private Border defaultBorder = null;

    private Integer uid;

    /**
     * Constructor
     */
    public SwingScilabMenu() {
        super();
        this.setFocusable(false);
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
        setText("");
    }

    /**
     * Append a CheckBoxMenuItem to a Scilab Menu
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.CheckBoxMenuItem)
     */
    public void add(CheckBoxMenuItem newCheckBoxMenuItem) {
        /* Back to Java Mouse Listeners */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }
        if (nativeMouseListeners != null) {
            for (int i = 0; i < nativeMouseListeners.length; i++) {
                addMouseListener(nativeMouseListeners[i]);
            }
            nativeMouseListeners = null;
        }

        super.add((SwingScilabCheckBoxMenuItem) newCheckBoxMenuItem.getAsSimpleCheckBoxMenuItem());
        super.repaint();
    }

    /**
     * Append a CheckBoxMenuItem to a Scilab Menu
     * @param newCheckBoxMenuItem the CheckBoxMenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.CheckBoxMenuItem)
     */
    public void add(SwingScilabCheckBoxMenuItem newCheckBoxMenuItem) {
        /* Back to Java Mouse Listeners */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }
        if (nativeMouseListeners != null) {
            for (int i = 0; i < nativeMouseListeners.length; i++) {
                addMouseListener(nativeMouseListeners[i]);
            }
            nativeMouseListeners = null;
        }

        super.add(newCheckBoxMenuItem);
        super.repaint();
    }

    /**
     * Append a MenuItem to a Scilab Menu
     * @param newMenuItem the MenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
     */
    public void add(MenuItem newMenuItem) {
        /* Back to Java Mouse Listeners */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }
        if (nativeMouseListeners != null) {
            for (int i = 0; i < nativeMouseListeners.length; i++) {
                addMouseListener(nativeMouseListeners[i]);
            }
            nativeMouseListeners = null;
        }

        super.add((SwingScilabMenuItem) newMenuItem.getAsSimpleMenuItem());
        super.repaint();
    }

    /**
     * Append a MenuItem to a Scilab Menu
     * @param newMenuItem the MenuItem to add to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.MenuItem)
     */
    public void add(SwingScilabMenuItem newMenuItem) {
        /* Back to Java Mouse Listeners */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }
        if (nativeMouseListeners != null) {
            for (int i = 0; i < nativeMouseListeners.length; i++) {
                addMouseListener(nativeMouseListeners[i]);
            }
            nativeMouseListeners = null;
        }

        super.add(newMenuItem);
        super.repaint();
    }

    /**
     * Append a subMenu to a Scilab Menu
     * @param newSubMenu the subMenu to append to the Menu
     * @see org.scilab.modules.gui.menu.Menu#add(org.scilab.modules.gui.ContextMenu)
     */
    public void add(Menu newSubMenu) {
        /* Back to Java Mouse Listeners */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }
        if (nativeMouseListeners != null) {
            for (int i = 0; i < nativeMouseListeners.length; i++) {
                addMouseListener(nativeMouseListeners[i]);
            }
            nativeMouseListeners = null;
        }

        super.add((SwingScilabMenu) newSubMenu.getAsSimpleMenu());
        super.repaint();
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
     * @param cb the callback to set.
     */
    public void setCallback(CommonCallBack cb) {
        this.callback = cb;

        // Manage callback removing (callback string == "")
        if (cb.getCommand().equals("")) {
            // Back to native listeners
            if (nativeMouseListeners != null) {
                for (int i = 0; i < nativeMouseListeners.length; i++) {
                    addMouseListener(nativeMouseListeners[i]);
                }
            }
            return;
        }

        if (getItemCount() == 0) {
            /* No children: managed as a button for callbacks */
            /* Remove Java MouseListeners if not already done */
            /* Save them so that they can be put back */
            if (nativeMouseListeners == null) {
                nativeMouseListeners = getMouseListeners();
                for (int i = 0; i < nativeMouseListeners.length; i++) {
                    removeMouseListener(nativeMouseListeners[i]);
                }
            }
        } else {
            /* With children */
            /* Back to native listeners if were already removed */
            if (nativeMouseListeners != null) {
                for (int i = 0; i < nativeMouseListeners.length; i++) {
                    addMouseListener(nativeMouseListeners[i]);
                }
            }
        }

        /* Remove previous listener */
        if (customedMouseListener != null) {
            removeMouseListener(customedMouseListener);
        }

        /* Create a customed MouseListener */
        customedMouseListener = new MouseListener() {

            public void mouseClicked(MouseEvent arg0) {
                /* Mouse button released over the menu */
                /* Deselect the menu and execute the callback */
                setSelected(false);
                if (callback != null) {
                    callback.actionPerformed(null);
                }
            }

            public void mouseEntered(MouseEvent arg0) {
                /* Nothing to do */
            }

            public void mouseExited(MouseEvent arg0) {
                /* Nothing to do */
            }

            public void mousePressed(MouseEvent arg0) {
                /* Select the menu */
                setSelected(true);
            }

            public void mouseReleased(MouseEvent arg0) {
                /* Mouse button released out of the menu */
                /* Deselect the menu */
                setSelected(false);
            }

        };

        /* Add the mouse listener */
        addMouseListener(customedMouseListener);
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
     * Destroy the Menu
     * @see org.scilab.modules.gui.menuitem.SimpleMenuItem#destroy()
     */
    public void destroy() {
        ScilabSwingUtilities.removeFromParent(this);
    }

    /**
     * Set if the menu is enabled or not
     * @param status true if the menu is enabled
     */
    public void setEnabled(boolean status) {
        if (status == isEnabled()) {
            return;
        }

        super.setEnabled(status);
        /* (Des)Activate the callback */
        if (callback != null) {
            if (status) {
                removeMouseListener(customedMouseListener); /* To be sure the callback is not added two times */
                addMouseListener(customedMouseListener);
            } else {
                removeMouseListener(customedMouseListener);
            }
        }
    }

    /**
     * Setter for InfoBar
     * @param infoBarToAdd the InfoBar associated to the Menu.
     */
    public void addInfoBar(TextBox infoBarToAdd) {
        /* Unimplemented for Menus */
        throw new UnsupportedOperationException();
    }

    /**
     * Getter for InfoBar
     * @return the InfoBar associated to the Menu.
     */
    public TextBox getInfoBar() {
        /* Unimplemented for Menus */
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
     * Retrieve the CallBack associated to this MenuItem
     * @return the CallBack
     */
    public CommonCallBack getCallback() {
        return callback;
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

    public void resetBackground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Menu.background");
        if (color != null) {
            setBackground(color);
        }
    }

    public void resetForeground() {
        Color color = (Color)UIManager.getLookAndFeelDefaults().get("Menu.foreground");
        if (color != null) {
            setForeground(color);
        }
    }
}
