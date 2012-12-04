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

package org.scilab.modules.uiwidget;

import java.awt.Color;
import java.awt.Container;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.font.TextAttribute;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.EventListener;
import java.util.Set;
import java.util.TreeMap;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JComponent;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.ScrollPaneConstants;

import org.xml.sax.Attributes;

import org.scilab.modules.uiwidget.components.NoLayout;
import org.scilab.modules.uiwidget.components.UIFocusListener;
import org.scilab.modules.uiwidget.components.UIMouseListener;
import org.scilab.modules.uiwidget.components.UITab;
import org.scilab.modules.uiwidget.components.UITools;

import org.scilab.modules.types.ScilabType;

public abstract class UIComponent {

    private static int UID = 0;

    private Object component;
    private Object modifiableComponent;
    private boolean thisOrComponent = true;
    private String id;
    private int uid;
    private UIComponent root;
    protected UIComponent parent;
    protected Map<String, ButtonGroup> buttonGroups;
    protected Map<String, UIComponent> children;
    protected List<UIComponent> childrenList;
    protected Map<String, Map<String, String>> style;
    protected UIMouseListener mouseListener;
    protected UIFocusListener focusListener;
    protected String path;
    protected Map<String, String> constraint;
    protected Rectangle position = new Rectangle();
    protected String tabTitle;

    protected UIComponent() { }

    public UIComponent(UIComponent parent) throws UIWidgetException {
        this.parent = parent;
        this.uid = UID++;

        if (parent != null) {
            this.root = parent.root;
        } else {
            this.root = this;
        }

        UILocator.add(this);
        registerToParent();
    }

    private final void registerToParent() {
        if (!isRoot()) {
            if (parent.childrenList == null) {
                parent.childrenList = new ArrayList<UIComponent>();
            }
            parent.childrenList.add(this);
        }
    }

    private final void registerIdToParent() {
        if (parent != null && parent.path != null && id != null) {
            if (parent.children == null) {
                parent.children = new HashMap<String, UIComponent>();
            }

            parent.children.put(id, this);
        }
    }

    public static final StringMap getMapFromAttributes(Attributes attributes) {
        final int len = attributes != null ? attributes.getLength() : 0;
        StringMap map = new StringMap(len);
        for (int i = 0; i < len; i++) {
            map.put(attributes.getLocalName(i), attributes.getValue(i));
        }

        return map;
    }

    public static final UIComponent getUIComponent(String pack, String name, String[] attributes, UIComponent parent) throws UIWidgetException {
        final int len = attributes != null ? (attributes.length % 2 == 0 ? attributes.length : attributes.length - 1) : 0;
        StringMap map = new StringMap(len);
        for (int i = 0; i < len; i += 2) {
            map.put(attributes[i], attributes[i + 1]);
        }

        return getUIComponent(pack, name, map, parent, parent == null ? null : parent.style);
    }

    public static final UIComponent getUIComponent(String pack, String name, Attributes attributes, UIComponent parent, Map<String, Map<String, String>> style) throws UIWidgetException {
        return getUIComponent(pack, name, getMapFromAttributes(attributes), parent, style);
    }

    public static final UIComponent getUIComponent(String pack, String name, ConvertableMap attributes, UIComponent parent, Map<String, Map<String, String>> style) throws UIWidgetException {
        try {
            Class clazz = Class.forName(pack + "." + name);
            Constructor constructor = clazz.getConstructor(UIComponent.class);
            UIComponent ui = (UIComponent) constructor.newInstance(parent);
            String id = (String) attributes.get(String.class, "id", null);
            ui.setId(id);
            attributes.remove("id");
            ui.setMapStyle(style);
            ui.createNewInstance(attributes);
            if (ui.component instanceof JComponent) {
                JComponent jc = (JComponent) ui.component;
                Dimension d = jc.getPreferredSize();
                ui.position = new Rectangle(0, 0, d.width, d.height);
            }
            String tabTitle = (String) attributes.get(String.class, "tab-title", null);
            if (tabTitle != null) {
                ui.setTabTitle(tabTitle);
                attributes.remove("tab-title");
            }
            return ui;
        } catch (ClassNotFoundException e) {
            throw new UIWidgetException("Cannot find the class " + pack + "." + name);
        } catch (NoSuchMethodException e) {
            throw new UIWidgetException("Cannot find a valid constructor in class " + pack + "." + name + ":\n" + e.getMessage());
        } catch (SecurityException e) {
            throw new UIWidgetException("Cannot find a valid constructor in class " + pack + "." + name + ":\n" + e.getMessage());
        } catch (InstantiationException e) {
            throw new UIWidgetException("Cannot instantiate the class " + pack + "." + name + ":\n" + e.getMessage());
        } catch (IllegalAccessException e) {
            throw new UIWidgetException("Cannot instantiate the class " + pack + "." + name + ":\n" + e.getMessage());
        } catch (IllegalArgumentException e) {
            throw new UIWidgetException("Cannot instantiate the class " + pack + "." + name + ":\n" + e.getMessage());
        } catch (InvocationTargetException e) {
            //System.err.println(e);
            e.getTargetException().printStackTrace();
            throw new UIWidgetException("Cannot instantiate the class " + pack + "." + name + ":\n" + e.getCause());
        }
    }

    public String getType() {
        return this.getClass().getSimpleName();
    }

    public int getUid() {
        return uid;
    }

    public void setParent(UIComponent parent) throws UIWidgetException {
        if (parent != this.parent) {
            if (parent != null && parent.children != null && id != null) {
                parent.children.remove(id);
            }
            if (isRoot() && buttonGroups != null) {
                if (parent.root.buttonGroups == null) {
                    parent.root.buttonGroups = buttonGroups;
                } else {
                    parent.root.buttonGroups.putAll(buttonGroups);
                }
                buttonGroups = null;
            }

            UILocator.removeFromCachedPaths(this);
            this.parent = parent;
            setRoot(parent == null ? this : parent.root);
            invalidateUIPath();
            registerToParent();
            registerIdToParent();
            if (isRoot()) {
                UILocator.addRoot(this);
            } else {
                parent.add(this);
            }
        }
    }

    private void setRoot(UIComponent root) {
        this.root = root;
        for (UIComponent c : childrenList) {
            c.setRoot(root);
        }
    }

    public void setParent(String parentId) throws UIWidgetException {
        UIComponent parent = UILocator.get(parentId);
        if (parent != null) {
            setParent(parent);
        }
    }

    public void setId(String id) {
        if ((id != null && !id.equals(this.id)) || (id == null && this.id != null)) {
            if (!isRoot() && this.id != null && parent.children != null) {
                parent.children.remove(this.id);
            }
            UILocator.removeFromCachedPaths(this);
            this.id = id;
            invalidateUIPath();
            if (isRoot()) {
                UILocator.addRoot(this);
            } else {
                registerIdToParent();
            }
        }
    }

    public String getId() {
        return this.id;
    }

    public void setTag(String tag) {
        setId(tag);
    }

    public String getTag() {
        return getId();
    }

    public String getMlistCode() {
        return "mlist([''UIWidget'',''_id'']," + Integer.toString(uid);
    }

    private void invalidateUIPath() {
        if (id != null) {
            if (isRoot()) {
                path = "/" + id;
            } else if (parent.path != null) {
                path = parent.path + "/" + id;
            }
            if (path != null && children != null) {
                for (UIComponent ui : children.values()) {
                    ui.invalidateUIPath();
                }
            }
        }
    }

    public String getUIPath() {
        return this.path;
    }

    public String getPath() {
        return getUIPath();
    }

    public String getRootId() {
        return root.id;
    }

    public UIComponent getRoot() {
        return root;
    }

    public void addToButtonGroup(String name, AbstractButton button) {
        if (isRoot()) {
            if (buttonGroups == null) {
                buttonGroups = new HashMap<String, ButtonGroup>();
            }
            ButtonGroup bg = buttonGroups.get(name);
            if (bg == null) {
                bg = new ButtonGroup();
                buttonGroups.put(name, bg);
            }
            bg.add(button);
        }
    }

    public void removeFromButtonGroup(String name, AbstractButton button) {
        if (isRoot() && buttonGroups != null) {
            ButtonGroup bg = buttonGroups.get(name);
            if (bg != null) {
                bg.remove(button);
            }
        }
    }

    public boolean isRoot() {
        return parent == null;
    }

    public UIComponent getUIComponent(final String id) {
        return children.get(id);
    }

    public void closeUIComponent() { }

    public void remove() {
        UserData.removeUIWidgetUserData(uid);
        UILocator.remove(this);
        if (childrenList != null) {
            for (UIComponent ui : childrenList) {
                ui.remove();
            }
            childrenList = null;
            children = null;
        }
        closeUIComponent();
        parent = null;
        if (mouseListener != null) {
            mouseListener.addListenerToComponent(null);
            mouseListener = null;
        }
        if (focusListener != null) {
            focusListener.addListenerToComponent(null);
            focusListener = null;
        }
        if (component instanceof JComponent) {
            JComponent jc = (JComponent) component;
            if (jc.getParent() != null) {
                jc.getParent().remove(jc);
            }
            jc.removeAll();
        }
        if (getModifiableComponent() != component && getModifiableComponent() instanceof JComponent) {
            JComponent jc = (JComponent) getModifiableComponent();
            if (jc.getParent() != null) {
                jc.getParent().remove(jc);
            }
            jc.removeAll();
        }
        component = null;
        modifiableComponent = null;
        root = null;
    }

    public abstract Object newInstance();

    private void setMapStyle(Map<String, Map<String, String>> style) {
        this.style = style;
    }

    public void finish() {

    }

    public UIComponent getParent() {
        return parent;
    }

    public String getID() {
        return id;
    }

    public boolean isRootVisible() throws UIWidgetException {
        if (isRoot()) {
            if (component instanceof Component) {
                return ((Component) component).isVisible();
            }
            return false;
        } else {
            return root.isRootVisible();
        }
    }

    public UIComponent[] getChildren() {
        return childrenList.toArray(new UIComponent[childrenList.size()]);
    }

    public Object getComponent() {
        return component;
    }

    public void setComponent(Object o) {
        this.component = o;
    }

    public Object getModifiableComponent() {
        return modifiableComponent == null ? component : modifiableComponent;
    }


    public JComponent getModifiableJComponent() throws UIWidgetException {
        Object c = getModifiableComponent();
        if (c instanceof JComponent) {
            return (JComponent) c;
        }

        throw new UIWidgetException("Not a JComponent");
    }

    public JComponent getJComponent() throws UIWidgetException {
        Object c = getComponent();
        if (c instanceof JComponent) {
            return (JComponent) c;
        }

        throw new UIWidgetException("Not a JComponent");
    }

    public Map<String, String> getLayoutConstraint() {
        return this.constraint;
    }

    public Container getModifiableContainer() throws UIWidgetException {
        Object c = getModifiableComponent();
        if (c instanceof Container) {
            return (Container) c;
        }

        throw new UIWidgetException("Not a Container");
    }

    public Container getContainer() throws UIWidgetException {
        Object c = getComponent();
        if (c instanceof Container) {
            return (Container) c;
        }

        throw new UIWidgetException("Not a Container");
    }

    public void setUiStyle(String style) throws UIWidgetException {
        setUiStyle(StyleParser.parseLine(style));
    }

    public void setUiStyle(Map<String, String> style) throws UIWidgetException {
        Object c = getModifiableComponent();
        if (c instanceof JComponent) {
            ((JComponent) c).setFont(UITools.getFont(((JComponent) c).getFont(), style));
        }
        for (String key : style.keySet()) {
            try {
                setProperty(key, style.get(key));
            } catch (UIWidgetException e) { }
        }
    }

    public void setTabTitle(String title) throws UIWidgetException {
        if (parent instanceof UITab && title != null) {
            JComponent c = getJComponent();
            JTabbedPane tab = (JTabbedPane) ((UITab) parent).getJComponent();
            int index = tab.indexOfTabComponent(c);
            if (index != -1) {
                tab.setTitleAt(index, title);
            }
        }
        this.tabTitle = title;
    }

    public String getTabTitle() throws UIWidgetException {
        if (parent instanceof UITab) {
            JComponent c = getJComponent();
            JTabbedPane tab = (JTabbedPane) ((UITab) parent).getJComponent();
            int index = tab.indexOfTabComponent(c);
            if (index != -1) {
                return tab.getTitleAt(index);
            }
        }

        return tabTitle;
    }

    public void setTooltip(String text) throws UIWidgetException {
        getModifiableJComponent().setToolTipText(text);
    }

    public void setTooltipString(String text) throws UIWidgetException {
        setTooltip(text);
    }

    public void setColor(Color c) throws UIWidgetException {
        getModifiableJComponent().setForeground(c);
    }

    public void setFontName(String name) throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            map.put(TextAttribute.FAMILY, name);
            setFont(new Font(map));
        }
    }

    public void setFontSize(double size) throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            map.put(TextAttribute.SIZE, Double.isNaN(size) ? new Double(12.0) : new Double(size));
            setFont(new Font(map));
        }
    }

    public void setFontWeight(UITools.FontWeight weight) throws UIWidgetException {
        if (weight != null) {
            Font f = getFont();
            if (f != null) {
                Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
                map.put(TextAttribute.WEIGHT, weight.value());
                setFont(new Font(map));
            }
        }
    }

    public void setFontAngle(String angle) throws UIWidgetException {
        if (angle != null && !angle.isEmpty()) {
            angle = angle.toLowerCase();
            boolean italic = angle.equals("italic") || angle.equals("oblique");
            Font f = getModifiableJComponent().getFont();
            if (f != null) {
                Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
                map.put(TextAttribute.POSTURE, italic ? TextAttribute.POSTURE_OBLIQUE : TextAttribute.POSTURE_REGULAR);
                setFont(new Font(map));
            }
        }
    }

    public void setFont(Font f) throws UIWidgetException {
        getModifiableJComponent().setFont(f);
    }

    public Font getFont() throws UIWidgetException {
        return getModifiableJComponent().getFont();
    }

    public void setPosition(Rectangle r) throws UIWidgetException {
        Container p = getJComponent().getParent();
        if (p == null || p.getLayout() instanceof NoLayout) {
            position.x = r.x;
            position.y = r.y;
            if (position.width != r.width || position.height != r.height) {
                position.width = r.width;
                position.height = r.height;
                getJComponent().setSize(position.width, position.height);
                getJComponent().revalidate();
            }
            if (p != null) {
                p.invalidate();
                p.doLayout();
                p.repaint();
            }
        }
    }

    public Rectangle getPosition() throws UIWidgetException {
        Container p = getJComponent().getParent();
        if (p == null || p.getLayout() instanceof NoLayout) {
            return position;
        }

        Dimension d = getJComponent().getSize();
        Point pt = getJComponent().getLocation();

        return new Rectangle(pt.x, pt.y, d.width, d.height);
    }

    public void setSize(Dimension d) throws UIWidgetException {
        position.width = d.width;
        position.height = d.height;
        Container p = getJComponent().getParent();
        if (p != null) {
            p.invalidate();
            p.doLayout();
            p.repaint();
        }
    }

    public void setForegroundColor(Color c) throws UIWidgetException {
        if (c != null) {
            getModifiableJComponent().setForeground(c);
        }
    }

    public void setBackgroundColor(Color c) throws UIWidgetException {
        if (c != null) {
            getModifiableJComponent().setBackground(c);
        }
    }

    public void setScrollable(boolean b) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            if (b) {
                scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
                scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED);
            } else {
                scroll.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
                scroll.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_NEVER);
            }
        } else if (component instanceof JComponent && b) {
            JComponent c = (JComponent) component;
            Container parent = c.getParent();
            int pos = parent.getComponentZOrder(c);
            parent.remove(c);
            JScrollPane scroll = new JScrollPane(c);
            parent.add(scroll, pos);
            parent.doLayout();
            parent.repaint();
            component = scroll;
            modifiableComponent = c;
        }
    }

    public boolean getScrollable() {
        return component instanceof JScrollPane && ((JScrollPane) component).getHorizontalScrollBarPolicy() == ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
    }

    public void setConstraint(String constraint) {
        setConstraint(StyleParser.parseLine(constraint));
    }

    public void setConstraint(Map<String, String> constraint) {
        this.constraint = constraint;
    }

    public void setLayout(String layout) throws UIWidgetException {
        getModifiableContainer().setLayout(UILayoutFactory.getLayout(getModifiableJComponent(), layout));
    }

    public void setEnable(String enable) throws UIWidgetException {
        getJComponent().setEnabled("on".equalsIgnoreCase(enable));
    }

    public String getEnable() throws UIWidgetException {
        return getJComponent().isEnabled() ? "on" : "off";
    }

    private final void createMouseListener() throws UIWidgetException {
        if (mouseListener == null) {
            mouseListener = new UIMouseListener(this);
            mouseListener.newInstance();
            mouseListener.addListenerToComponent(getModifiableJComponent());
        }
    }

    public void setOnmouseclick(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseclick(command);
        }
    }

    public void setOnmouseover(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseover(command);
        }
    }

    public void setOnmouseenter(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseenter(command);
        }
    }

    public void setOnmouseexit(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseexit(command);
        }
    }

    public void setOnmousepress(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousepress(command);
        }
    }

    public void setOnmouserelease(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouserelease(command);
        }
    }

    public void setOnmousewheel(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousewheel(command);
        }
    }

    public void setOnmousedrag(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousedrag(command);
        }
    }

    public String getOnmouseclick() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseclick();
        }

        return null;
    }

    public String getOnmouseover() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseover();
        }

        return null;
    }

    public String getOnmouseenter() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseenter();
        }

        return null;
    }

    public String getOnmouseexit() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseexit();
        }

        return null;
    }

    public String getOnmousepress() {
        if (mouseListener != null) {
            return mouseListener.getOnmousepress();
        }

        return null;
    }

    public String getOnmouserelease() {
        if (mouseListener != null) {
            return mouseListener.getOnmouserelease();
        }

        return null;
    }

    public String getOnmousedrag() {
        if (mouseListener != null) {
            return mouseListener.getOnmousedrag();
        }

        return null;
    }

    public String getOnmousewheel() {
        if (mouseListener != null) {
            return mouseListener.getOnmousewheel();
        }

        return null;
    }

    public void setOnmouseclickEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseclickEnable(b);
        }
    }

    public void setOnmouseoverEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseoverEnable(b);
        }
    }

    public void setOnmouseenterEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseenterEnable(b);
        }
    }

    public void setOnmouseexitEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseexitEnable(b);
        }
    }

    public void setOnmousepressEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousepressEnable(b);
        }
    }

    public void setOnmousereleaseEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousereleaseEnable(b);
        }
    }

    public void setOnmousewheelEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousewheelEnable(b);
        }
    }

    public void setOnmousedragEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousedragEnable(b);
        }
    }

    private final void createFocusListener() throws UIWidgetException {
        if (focusListener == null) {
            focusListener = new UIFocusListener(this);
            focusListener.newInstance();
            focusListener.addListenerToComponent(getModifiableJComponent());
        }
    }

    public void setOnfocusgain(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createFocusListener();
            focusListener.setOnfocusgain(command);
        }
    }

    public void setOnfocusloss(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createFocusListener();
            focusListener.setOnfocusloss(command);
        }
    }

    public String getOnfocusgain() {
        if (focusListener != null) {
            return focusListener.getOnfocusgain();
        }

        return null;
    }

    public String getOnfocusloss() {
        if (focusListener != null) {
            return focusListener.getOnfocusloss();
        }

        return null;
    }

    public void setOnfocusgainEnable(boolean b) {
        if (focusListener != null) {
            focusListener.setOnfocusgainEnable(b);
        }
    }

    public void setOnfocuslossEnable(boolean b) {
        if (focusListener != null) {
            focusListener.setOnfocuslossEnable(b);
        }
    }

    public void updateDependencies(UIComponent parent) throws UIWidgetException {
        setParent(parent);
    }

    public void add(List<UIComponent> list) throws UIWidgetException {
        for (UIComponent uicomp : list) {
            add(uicomp);
        }
    }

    public void add(final UIComponent uicomp) throws UIWidgetException {
        UIAccessTools.add(this, uicomp);
    }

    public void addListener(final UIListener uicomp) throws UIWidgetException {
        uicomp.addListenerToComponent(getModifiableJComponent());
    }

    public void addPopupMenu(final JPopupMenu popup) {
        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                try {
                    getModifiableJComponent().setComponentPopupMenu(popup);
                } catch (Exception e) {

                }
            }
        });
    }

    public void setProperty(final String name, final String value) throws UIWidgetException {
        try {
            if (thisOrComponent) {
                setPropertyViaReflectionInThis(name, value);
            } else {
                setPropertyViaReflectionInComponent(name, value);
            }
        } catch (Exception e) {
            if (thisOrComponent) {
                setPropertyViaReflectionInComponent(name, value);
            } else {
                setPropertyViaReflectionInThis(name, value);
            }
        }
    }

    protected final void setPropertyViaReflectionInThis(final String name, final String value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(this, name, value);
    }

    protected final void setPropertyViaReflectionInComponent(final String name, final String value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(getModifiableComponent(), name, value);
    }

    public void setProperty(final String name, final ScilabType value) throws UIWidgetException {
        try {
            if (thisOrComponent) {
                setPropertyViaReflectionInThis(name, value);
            } else {
                setPropertyViaReflectionInComponent(name, value);
            }
        } catch (Exception e) {
            if (thisOrComponent) {
                setPropertyViaReflectionInComponent(name, value);
            } else {
                setPropertyViaReflectionInThis(name, value);
            }
        }
    }

    protected final void setPropertyViaReflectionInThis(final String name, final ScilabType value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(this, name, value);
    }

    protected final void setPropertyViaReflectionInComponent(final String name, final ScilabType value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(getModifiableComponent(), name, value);
    }

    public String[][] getPropertiesPairs() {
        Map<String, Method> map = new TreeMap<String, Method>();
        if (thisOrComponent) {
            UIMethodFinder.getSetter(getModifiableComponent().getClass(), map);
            UIMethodFinder.getSetter(this.getClass(), map);
        } else {
            UIMethodFinder.getSetter(this.getClass(), map);
            UIMethodFinder.getSetter(getModifiableComponent().getClass(), map);
        }

        System.out.println(map);

        return null;
    }

    public Object getProperty(final String name) throws UIWidgetException {
        try {
            if (thisOrComponent) {
                return getPropertyViaReflectionInThis(name);
            } else {
                return getPropertyViaReflectionInComponent(name);
            }
        } catch (Exception e) {
            if (thisOrComponent) {
                return getPropertyViaReflectionInComponent(name);
            } else {
                return getPropertyViaReflectionInThis(name);
            }
        }
    }

    protected Object getPropertyViaReflectionInThis(final String name) throws UIWidgetException {
        return UIAccessTools.getPropertyViaReflection(this, name);
    }

    protected Object getPropertyViaReflectionInComponent(final String name) throws UIWidgetException {
        return UIAccessTools.getPropertyViaReflection(getComponent(), name);
    }

    private void createNewInstance(final ConvertableMap attributes) throws UIWidgetException {
        Set<String> uselessAttrs = UIAccessTools.createNewInstance(this, attributes);

        if (uselessAttrs.contains("scrollable")) {
            boolean scrollable = (Boolean) attributes.get(boolean.class, "scrollable", false);
            if (scrollable && component instanceof JComponent) {
                modifiableComponent = component;
                component = new JScrollPane((JComponent) modifiableComponent);
                uselessAttrs.remove("scrollable");
            }
        }

        setAttributesAndStyle(attributes, uselessAttrs);
    }

    private void setAttributesAndStyle(final ConvertableMap attributes, final Set<String> uselessAttrs) {
        if (!uselessAttrs.isEmpty()) {
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    if (attributes instanceof StringMap) {
                        for (String attr : uselessAttrs) {
                            try {
                                setProperty(attr, (String) attributes.get(attr));
                            } catch (UIWidgetException e) { }
                        }
                    } else {
                        for (String attr : uselessAttrs) {
                            try {
                                setProperty(attr, (ScilabType) attributes.get(attr));
                            } catch (UIWidgetException e) { }
                        }
                    }
                }
            });
        }

        if (style != null) {
            Map<String, String> elementStyle = null;
            if (id != null) {
                elementStyle = style.get("#" + id);
            }
            if (elementStyle == null) {
                String styleClass = (String) attributes.get(String.class, "class", null);
                if (styleClass != null) {
                    elementStyle = style.get(styleClass);
                }
            }
            if (elementStyle == null) {
                elementStyle = style.get("." + this.getClass().getSimpleName());
            }

            final Map<String, String> es = elementStyle;
            if (elementStyle != null) {
                UIAccessTools.execOnEDT(new Runnable() {
                    public void run() {
                        try {
                            setUiStyle(new HashMap<String, String>(es));
                        } catch (UIWidgetException e) {
                            System.err.println(e);
                        }
                    }
                });
            }
        }

    }

    protected void finalize() throws Throwable {
        UserData.removeUIWidgetUserData(uid);
        super.finalize();
    }
}
