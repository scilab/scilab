/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.uiwidget.components;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Map;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.Icon;
import javax.swing.JButton;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.KeyStroke;

import org.flexdock.view.Titlebar;

import org.scilab.modules.gui.uiwidget.StringConverters;
import org.scilab.modules.gui.uiwidget.UIComponent;
import org.scilab.modules.gui.uiwidget.UIComponentAnnotation;
import org.scilab.modules.gui.uiwidget.UIWidgetException;
import org.scilab.modules.gui.uiwidget.UIWidgetTools;
import org.scilab.modules.gui.uiwidget.callback.UICallback;

/**
 * JButton wrapper
 */
public class UIButton extends UIComponent {

    private static int action_id = 0;

    protected JButton button;
    protected JPopupMenu menu;
    protected Action menuAction;
    protected boolean hasRolloverIcon;
    protected ActionListener clicklistener;
    protected UICallback clickaction;
    protected boolean onclickEnable = true;
    protected boolean blueRollover = false;
    protected String text;
    protected AbstractAction actionForTab;

    public enum Relief {
        NONE,
        HALF,
        NORMAL;
    }

    public enum Alignment {
        LEADING (JButton.LEADING),
        CENTER (JButton.CENTER),
        LEFT (JButton.LEFT),
        RIGHT (JButton.RIGHT),
        TRAILING (JButton.TRAILING);

        private final int value;

        Alignment(int value) {
            this.value = value;
        }

        public int value() {
            return value;
        }

        public static Alignment get(int value) {
            switch (value) {
                case JButton.LEADING:
                    return Alignment.LEADING;
                case JButton.CENTER:
                    return Alignment.CENTER;
                case JButton.LEFT:
                    return Alignment.LEFT;
                case JButton.RIGHT:
                    return Alignment.RIGHT;
                case JButton.TRAILING:
                    return Alignment.TRAILING;
                default:
                    return Alignment.LEADING;
            }
        }

        public static String getAsString(int value) {
            switch (value) {
                case JButton.LEADING:
                    return "leading";
                case JButton.CENTER:
                    return "center";
                case JButton.LEFT:
                    return "left";
                case JButton.RIGHT:
                    return "right";
                case JButton.TRAILING:
                    return "trailing";
                default:
                    return "leading";
            }
        }
    }

    /**
     * {@inheritDoc}
     */
    public UIButton(UIComponent parent) throws UIWidgetException {
        super(parent);
    }

    /**
     * {@inheritDoc}
     */
    public Object newInstance() {
        button = new JButton();

        return button;
    }

    @UIComponentAnnotation(attributes = {"text", "icon", "mnemonic"})
    public Object newInstance(String text, Icon icon, KeyStroke mnemonic) {
        if (icon == null) {
            button = new JButton(text);
            //button.setVisible(true);
            //button.setBounds(10,10,100,100);
        } else {
            button = new JButton(text, icon);
        }

        if (mnemonic == null) {
            setText(text);
        } else {
            setMnemonic(mnemonic);
        }

        return button;
    }

    /**
     * Set the defaults
     */
    public void setDefaults() {
        button.setFocusable(false);
    }

    /**
     * Set the mnemonic
     * @param mnemonic the mnemonic
     */
    public void setMnemonic(KeyStroke mnemonic) {
        if (mnemonic != null) {
            button.setMnemonic(mnemonic.getKeyCode());
        }
    }

    /**
     * Get the mnemonic
     * @return the mnemonic
     */
    public KeyStroke getMnemonic() {
        int mnemonic = button.getMnemonic();
        if (mnemonic > 0) {
            return KeyStroke.getKeyStroke(mnemonic, 0);
        }

        return null;
    }

    /**
     * Set Rollover icon
     * @param icon an icon
     */
    public void setRolloverIcon(Icon icon) {
        button.setRolloverEnabled(icon != null);
        button.setRolloverIcon(icon);
        hasRolloverIcon = icon != null;
    }

    /**
     * Set a blue rollover
     * @param b if true enable the default rollover
     */
    public void setBlueRollover(boolean b) {
        Icon icon = button.getIcon();
        blueRollover = false;
        if (icon != null && !hasRolloverIcon && b) {
            button.setRolloverEnabled(true);
            button.setRolloverIcon(UITools.getRolloverIcon(icon));
            blueRollover = true;
        }
    }

    /**
     * @return true if the blue rollover is enabled
     */
    public boolean getBlueRollover() {
        return blueRollover;
    }

    /**
     * Set the button icon
     * @param icon the icon
     */
    public void setIcon(Icon icon) {
        button.setIcon(icon);
        if (blueRollover && icon != null) {
            button.setRolloverIcon(UITools.getRolloverIcon(icon));
        }
        if (actionForTab != null) {
            actionForTab.putValue(Action.SMALL_ICON, button.getIcon());
        }
    }

    public void setEnabled(boolean b) {
        button.setEnabled(b);
        if (actionForTab != null) {
            actionForTab.setEnabled(b);
        }
    }

    public void setEnable(boolean b) {
        setEnabled(b);
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setAlignment(Alignment a) {
        button.setHorizontalAlignment(a.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getAlignment() {
        return Alignment.getAsString(button.getHorizontalAlignment());
    }

    /**
     * Set the alignment
     * @param a the alignment
     */
    public void setHorizontalAlignment(Alignment alignment) {
        button.setHorizontalAlignment(alignment.value());
    }

    /**
     * Get the alignment
     * @return he alignment
     */
    public String getHorizontalAlignment() {
        return Alignment.getAsString(button.getHorizontalAlignment());
    }

    /**
     * {@inheritDoc}
     */
    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        String al = style.get("alignment");
        if (al != null && !al.isEmpty()) {
            Alignment a = StringConverters.getObjectFromValue(Alignment.class, al);
            button.setHorizontalAlignment(a.value());
            style.remove("alignment");
        }
        super.setUiStyle(style);
    }

    /**
     * Add a JMenuitem
     * @param menuitem the menu item
     */
    public void add(JMenuItem menuitem) {
        if (menu == null) {
            menu = new JPopupMenu();
            menu.setBorderPainted(true);
            menuAction = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    menu.show(button, 0, button.getBounds(null).height);
                }
            };
            button.addActionListener(menuAction);
        }
        menu.add(menuitem);
    }

    /**
     * Remove the onclick listener
     */
    protected void removeClickListener() {
        if (clicklistener != null) {
            button.removeActionListener(clicklistener);
            clicklistener = null;
        }
    }

    /**
     * {@inheritDoc}
     */
    public void remove() {
        removeClickListener();
        if (actionForTab != null) {
            Titlebar tb = (Titlebar) actionForTab.getValue("TITLEBAR");
            if (tb != null) {
                tb.removeAction((String) actionForTab.getValue(Action.NAME));
            }
        }
        super.remove();
    }

    /**
     * Get the onclick action
     * @return the action
     */
    public UICallback getOnclick() {
        return this.clickaction;
    }

    /**
     * Set the onclick action
     * @param the action
     */
    public void setOnclick(final String action) {
        if (this.clickaction == null) {
            removeClickListener();
            clicklistener = new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    if (onclickEnable) {
                        UIWidgetTools.execAction(UIButton.this.clickaction);
                    }
                }
            };
            button.addActionListener(clicklistener);
        }
        this.clickaction = UICallback.newInstance(this, action);
    }

    /**
     * Check if the onclick is enabled
     * @return true if enabled
     */
    public boolean getOnclickEnable() {
        return onclickEnable;
    }

    /**
     * Set if the onclick is enabled
     * @param b true if enabled
     */
    public void setOnclickEnable(boolean b) {
        onclickEnable = b;
    }

    /**
     * Alias for setText
     */
    public void setLabel(String label) {
        setText(label);
    }

    /**
     * Alias for getText
     */
    public String getLabel() {
        return getText();
    }

    /**
     * Alias for setText
     */
    public void setString(String label) {
        setText(label);
    }

    /**
     * Alias for getText
     */
    public String getString() {
        return getText();
    }

    /**
     * Set the button text
     * @param text the button text
     */
    public void setText(String text) {
        boolean containsMnemonic = UITools.setTextAndMnemonic(text, button);
        if (containsMnemonic) {
            this.text = text;
        } else {
            if (this.text != null) {
                button.setMnemonic(0);
            }
            this.text = null;
        }
    }

    /**
     * Get the button text
     * @return the button text
     */
    public String getText() {
        if (this.text == null) {
            return button.getText();
        }

        return this.text;
    }

    /**
     * Get the action
     * @return the action
     */
    public Action getAction() {
        if (actionForTab == null) {
            actionForTab = new AbstractAction() {
                public void actionPerformed(ActionEvent e) {
                    if (clicklistener != null) {
                        clicklistener.actionPerformed(e);
                    }
                }
            };
            String str = getId();
            if (str == null || str.isEmpty()) {
                str = button.getText();
                if (str == null || str.isEmpty()) {
                    str = "button_" + (action_id++);
                }
            }
            actionForTab.putValue(Action.NAME, str);
            actionForTab.putValue(Action.SMALL_ICON, button.getIcon());
        }

        return actionForTab;
    }

    /**
     * {@inheritDoc}
     */
    public String[] getScilabRepresentation() {
        return new String[] {
                   "\"Alignment = \"+h.alignment",
                   "\"BackgroundColor = \"+sci2exp(h.backgroundColor, 0)",
                   "\"BlueRollover = \"+sci2exp(h.blueRollover)",
                   "\"Border = \"+fmtuser_data(h.border)",
                   "\"BorderPainted = \"+sci2exp(h.borderPainted)",
                   "\"Bounds = \"+sci2exp(h.bounds,0)",
                   "\"ForegroundColor = \"+sci2exp(h.foreground,0)",
                   "\"Constraint = \"+fmtuser_data(h.constraint)",
                   "\"ContentAreaFilled = \"+sci2exp(h.contentAreaFilled)",
                   "\"Cursor = \"+fmtuser_data(h.cursor)",
                   "\"DisabledIcon = \"+fmtuser_data(h.disabledIcon)",
                   "\"DisabledSelectedIcon = \"+fmtuser_data(h.disabledSelectedIcon)",
                   "\"DoubleBuffered = \"+sci2exp(h.doubleBuffered)",
                   "\"EnableEvents = \"+sci2exp(h.enableEvents)",
                   "\"Enabled = \"+sci2exp(h.enabled)",
                   "\"Focusable = \"+sci2exp(h.focusable)",
                   "\"Font = \"+h.font",
                   "\"FontAngle = \"+h.fontAngle",
                   "\"FontSize = \"+sci2exp(h.fontSize)",
                   "\"FontWeight = \"+h.fontWeight",
                   "\"HorizontalAlignment = \"+h.horizontalAlignment",
                   "\"Icon = \"+fmtuser_data(h.icon)",
                   "\"IconTextGap = \"+sci2exp(h.iconTextGap)",
                   "\"Id = \"+sci2exp(h.id)",
                   "\"Location = \"+sci2exp(h.location,0)",
                   "\"Margin = \"+sci2exp(h.margin,0)",
                   "\"MaximumSize = \"+sci2exp(h.maximumSize,0)",
                   "\"MinimumSize = \"+sci2exp(h.minimumSize,0)",
                   "\"Mnemonic = \"+h.mnemonic",
                   "\"Onclick = \"+fmtuser_data(h.onclick)",
                   "\"OnclickEnable = \"+sci2exp(h.onclickEnable)",
                   "\"Onfocusgain = \"+fmtuser_data(h.onfocusgain)",
                   "\"OnfocusgainEnable = \"+sci2exp(h.onfocusgainEnable)",
                   "\"Onfocusloss = \"+fmtuser_data(h.onfocusloss)",
                   "\"OnfocuslossEnable = \"+sci2exp(h.onfocuslossEnable)",
                   "\"Onmouseenter = \"+fmtuser_data(h.onmouseenter)",
                   "\"OnmouseenterEnable = \"+sci2exp(h.onmouseenterEnable)",
                   "\"Onmouseexit = \"+fmtuser_data(h.onmouseexit)",
                   "\"OnmouseexitEnable = \"+sci2exp(h.onmouseexitEnable)",
                   "\"Onmouseover = \"+fmtuser_data(h.onmouseover)",
                   "\"OnmouseoverEnable = \"+sci2exp(h.onmouseoverEnable)",
                   "\"Opaque = \"+sci2exp(h.opaque)",
                   "\"Parent = \"+h.parent.type",
                   "\"Position = \"+sci2exp(h.position,0)",
                   "\"PreferredSize = \"+sci2exp(h.preferredSize,0)",
                   "\"PressedIcon = \"+fmtuser_data(h.pressedIcon)",
                   "\"Relief = \"+h.relief",
                   "\"RolloverEnabled = \"+sci2exp(h.rolloverEnabled)",
                   "\"RolloverIcon = \"+fmtuser_data(h.rolloverIcon)",
                   "\"Selected = \"+sci2exp(h.selected)",
                   "\"SelectedIcon = \"+fmtuser_data(h.selectedIcon)",
                   "\"Style = \"+h.style",
                   "\"Tag = \"+sci2exp(h.tag)",
                   "\"Text = \"+fmtuser_data(h.text)",
                   "\"TooltipText = \"+fmtuser_data(h.tooltipText)",
                   "\"Uistyle = \"+fmtuser_data(h.uistyle)",
                   "\"Units = \"+sci2exp(h.units,0)",
                   "\"Visible = \"+sci2exp(h.visible)"
               };
    }
}
