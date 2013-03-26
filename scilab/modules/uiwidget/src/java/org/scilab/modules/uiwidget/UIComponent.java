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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.KeyboardFocusManager;
import java.awt.KeyEventDispatcher;
import java.awt.LayoutManager;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
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
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.RootPaneContainer;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingUtilities;

import org.xml.sax.Attributes;

import org.scilab.modules.uiwidget.components.NoLayout;
import org.scilab.modules.uiwidget.components.UIFocusListener;
import org.scilab.modules.uiwidget.components.UIMouseListener;
import org.scilab.modules.uiwidget.components.UITab;
import org.scilab.modules.uiwidget.components.UITools;

import org.scilab.modules.types.ScilabType;

/**
 * Main class to handle Java components.
 */
public abstract class UIComponent {

    private static final MouseAdapter NOMOUSE = new MouseAdapter() { };
    private static final KeyEventDispatcher NOKEY = new KeyEventDispatcher() {
        public boolean dispatchKeyEvent(KeyEvent e) {
            return true;
        }
    };

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
    protected boolean enableEvents = true;

    /**
     * Default empty constrructor
     */
    protected UIComponent() { }

    /**
     * Constructor
     * @param parent the parent UIComponent
     */
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

    /**
     * Register this component to its parent
     */
    private final void registerToParent() {
        if (!isRoot()) {
            if (parent.childrenList == null) {
                parent.childrenList = new ArrayList<UIComponent>();
            }
            parent.childrenList.add(this);
        }
    }

    /**
     * Register id to parent
     */
    private final void registerIdToParent() {
        if (parent != null && parent.path != null && id != null) {
            if (parent.children == null) {
                parent.children = new HashMap<String, UIComponent>();
            }

            parent.children.put(id, this);
        }
    }

    /**
     * Get a Map from Attributes object
     * @param attributes the attributes
     * @return a Map containing the pairs attribute-name -- attribute-value
     */
    public static final StringMap getMapFromAttributes(Attributes attributes) {
        final int len = attributes != null ? attributes.getLength() : 0;
        StringMap map = new StringMap(len);
        for (int i = 0; i < len; i++) {
            map.put(attributes.getLocalName(i), attributes.getValue(i));
        }

        return map;
    }

    /**
     * Construct an UIComponent
     * @param pack the Java package name containing the component to instantiate
     * @param name the UIComponent name
     * @param attributes an array of length 2xN containing attributes name and value
     * @param parent the parent UIComponent
     * @return the newly constructed UIComponent
     */
    public static final UIComponent getUIComponent(String pack, String name, String[] attributes, UIComponent parent) throws UIWidgetException {
        final int len = attributes != null ? (attributes.length % 2 == 0 ? attributes.length : attributes.length - 1) : 0;
        StringMap map = new StringMap(len);
        for (int i = 0; i < len; i += 2) {
            map.put(attributes[i], attributes[i + 1]);
        }

        return getUIComponent(pack, name, map, parent, parent == null ? null : parent.style);
    }

    /**
     * Construct an UIComponent
     * @param pack the Java package name containing the component to instantiate
     * @param name the UIComponent name
     * @param attributes the attributes
     * @param parent the parent UIComponent
     * @return the newly constructed UIComponent
     */
    public static final UIComponent getUIComponent(String pack, String name, Attributes attributes, UIComponent parent, Map<String, Map<String, String>> style) throws UIWidgetException {
        return getUIComponent(pack, name, getMapFromAttributes(attributes), parent, style);
    }

    /**
     * Construct an UIComponent
     * @param pack the Java package name containing the component to instantiate
     * @param name the UIComponent name
     * @param attributes the attributes
     * @param parent the parent UIComponent
     * @return the newly constructed UIComponent
     */
    public static final UIComponent getUIComponent(String pack, String name, ConvertableMap attributes, final UIComponent parent, Map<String, Map<String, String>> style) throws UIWidgetException {
        try {
            Class clazz = Class.forName(pack + "." + name);
            final Constructor constructor = clazz.getConstructor(UIComponent.class);
            final UIComponent[] arr = new UIComponent[1];

            if (SwingUtilities.isEventDispatchThread()) {
                arr[0] = (UIComponent) constructor.newInstance(parent);
            } else {
                try {
                    SwingUtilities.invokeAndWait(new Runnable() {
                        public void run() {
                            try {
                                arr[0] = (UIComponent) constructor.newInstance(parent);
                            } catch (Exception e) {
                                System.err.println(e);
                            }
                        }
                    });
                } catch (Exception e) {
                    System.err.println(e);
                    return null;
                }
            }
            UIComponent ui = arr[0];
            String id = (String) attributes.get(String.class, "id", null);
            ui.setId(id);
            attributes.remove("id");
            ui.setMapStyle(style);
            String tabTitle = (String) attributes.get(String.class, "tab-title", null);
            if (tabTitle != null) {
                ui.setTabTitle(tabTitle);
                attributes.remove("tab-title");
            }
            ui.createNewInstance(attributes);
            if (ui.component instanceof JComponent) {
                JComponent jc = (JComponent) ui.component;
                Dimension d = jc.getPreferredSize();
                ui.position = new Rectangle(0, 0, d.width, d.height);
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

    /**
     * Create a new instance of this UIComponent
     * @return the created instance
     */
    public abstract Object newInstance();

    /**
     * Get the backed component
     * @return the component
     */
    public Object getComponent() {
        return component;
    }

    /**
     * Set the component
     * @param o the component
     */
    public void setComponent(Object o) {
        this.component = o;
    }

    /**
     * Set the cursor
     * @param cursor the cursor
     */
    public void setEnableEvents(boolean b) {
        if (b != this.enableEvents) {
            RootPaneContainer root = null;
            if (component instanceof JComponent) {
                root = (RootPaneContainer) ((JComponent) component).getTopLevelAncestor();
            } else if (component instanceof RootPaneContainer) {
                root = (RootPaneContainer) component;
            }
            if (root != null) {
                root.getGlassPane().setVisible(!b);
                if (b) {
                    KeyboardFocusManager.getCurrentKeyboardFocusManager().removeKeyEventDispatcher(NOKEY);
                    root.getGlassPane().removeMouseListener(NOMOUSE);
                } else {
                    KeyboardFocusManager.getCurrentKeyboardFocusManager().addKeyEventDispatcher(NOKEY);
                    root.getGlassPane().addMouseListener(NOMOUSE);
                }
            }
            this.enableEvents = b;
        }
    }

    /**
     * Set the cursor
     * @param cursor the cursor
     */
    public void setCursor(Cursor cursor) {
        if (component instanceof Component) {
            ((Component) component).setCursor(cursor);
        }
    }

    /**
     * Get the cursor
     * @return the cursor
     */
    public Cursor getCursor() {
        if (component instanceof Component) {
            return ((Component) component).getCursor();
        }

        return null;
    }

    /**
     * Get the modifiable component (can be different of component, e.g. with a JScrollPane containing the component)xs
     * @return the modfifiable component
     */
    public Object getModifiableComponent() {
        return modifiableComponent == null ? component : modifiableComponent;
    }

    /**
     * Get the modifiable component as a JComponent
     * @return modifiable JComponent
     */
    public JComponent getModifiableJComponent() throws UIWidgetException {
        Object c = getModifiableComponent();
        if (c instanceof JComponent) {
            return (JComponent) c;
        }

        throw new UIWidgetException("Not a JComponent");
    }

    /**
     * Get the component as a JComponent
     * @return the JComponent
     */
    public JComponent getJComponent() throws UIWidgetException {
        Object c = getComponent();
        if (c instanceof JComponent) {
            return (JComponent) c;
        }

        throw new UIWidgetException("Not a JComponent");
    }

    /**
     * Get the modifiable component as a Container
     * @return the Container
     */
    public Container getModifiableContainer() throws UIWidgetException {
        Object c = getModifiableComponent();
        if (c instanceof Container) {
            return (Container) c;
        }

        throw new UIWidgetException("Not a Container");
    }

    /**
     * Get the component as a Container
     * @return the Container
     */
    public Container getContainer() throws UIWidgetException {
        Object c = getComponent();
        if (c instanceof Container) {
            return (Container) c;
        }

        throw new UIWidgetException("Not a Container");
    }

    /**
     * Get the layout constraint
     * @return the constraint
     */
    public Map<String, String> getLayoutConstraint() {
        return this.constraint;
    }

    /**
     * @return the UIComponent name
     */
    public String getType() {
        return this.getClass().getSimpleName();
    }

    /**
     * @return the uid
     */
    public int getUid() {
        return uid;
    }

    /**
     * Set the preferred size
     * @param dim the preferred dimension
     */
    public void setPreferredSize(Dimension dim) throws UIWidgetException {
        getContainer().setPreferredSize(dim);
    }

    /**
     * Get the preferred size
     * @return the preferred dimension
     */
    public Dimension getPreferredSize() throws UIWidgetException {
        return getContainer().getPreferredSize();
    }

    /**
     * Set the size
     * @param d the size
     */
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

    /**
     * Get the size
     * @return the dimension
     */
    public Dimension getSize() throws UIWidgetException {
        return getContainer().getSize();
    }

    /**
     * Set the minimum size
     * @param dim the minimum dimension
     */
    public void setMinimumSize(Dimension dim) throws UIWidgetException {
        getContainer().setMinimumSize(dim);
    }

    /**
     * Get the minimum size
     * @return the minimum dimension
     */
    public Dimension getMinimumSize() throws UIWidgetException {
        return getContainer().getMinimumSize();
    }

    /**
     * Set the maximum size
     * @param dim the maximum dimension
     */
    public void setMaximumSize(Dimension dim) throws UIWidgetException {
        getContainer().setMaximumSize(dim);
    }

    /**
     * Get the maximum size
     * @return the maximum dimension
     */
    public Dimension getMaximumSize() throws UIWidgetException {
        return getContainer().getMaximumSize();
    }

    /**
     * Set the parent
     * @param parent the parent
     */
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

    /**
     * Set the root element
     * @param root the root element
     */
    private void setRoot(UIComponent root) {
        this.root = root;
        for (UIComponent c : childrenList) {
            c.setRoot(root);
        }
    }

    /**
     * Set the parent with the given id
     * @param parentId the id of the parent
     */
    public void setParent(String parentId) throws UIWidgetException {
        UIComponent parent = UILocator.get(parentId);
        if (parent != null) {
            setParent(parent);
        }
    }

    /**
     * Set the id
     * @param id the id to set
     */
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

    /**
     * Get the id of this UIComponent
     *@return the id
     */
    public String getId() {
        return this.id;
    }

    /**
     * Set the tag (Scilab's UIControl compatibility)
     * @param tag the tag
     */
    public void setTag(String tag) {
        setId(tag);
    }

    /**
     * Get the tag (id) of this UIComponent
     *@return the tag
     */
    public String getTag() {
        return getId();
    }

    /**
     * Invalidate the path to this UIComponent
     */
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

    /**
     * Get the path to this UIComponent
     * @return the path
     */
    public String getUIPath() {
        return this.path;
    }

    /**
     * Get the path to this UIComponent
     * @return the path
     */
    public String getPath() {
        return getUIPath();
    }

    /**
     * Get the root id of this UIComponent
     * @return the root id
     */
    public String getRootId() {
        return getRoot().id;
    }

    /**
     * Get the root element
     * @return the root
     */
    public UIComponent getRoot() {
        return root;
    }

    /**
     * Add a button to a button-group
     * @param name the name of the button-group
     * @param button the button to add
     */
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

    /**
     * Remove a button from a button-group
     * @param name the name of the button-group
     * @param button the button to remove
     */
    public void removeFromButtonGroup(String name, AbstractButton button) {
        if (isRoot() && buttonGroups != null) {
            ButtonGroup bg = buttonGroups.get(name);
            if (bg != null) {
                bg.remove(button);
            }
        }
    }

    /**
     * Check if this UIComponent is a root element (no parent)
     * @return true if it is a root element
     */
    public boolean isRoot() {
        return parent == null;
    }

    /**
     * Get the child with the given id
     * @param if the child id
     * @return the corresponding UIComponent
     */
    public UIComponent getUIComponent(final String id) {
        return children.get(id);
    }

    /**
     * Close this UIComponent
     */
    public void closeUIComponent() { }

    /**
     * Remove the UIComponent from differents cache and delete its children
     */
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

    /**
     * Set the map containing style element
     */
    private void setMapStyle(Map<String, Map<String, String>> style) {
        this.style = style;
    }

    /**
     * Finish the UIComponent creation
     */
    public void finish() {

    }

    /**
     * Replace the component by another one
     * @param c the new component
     */
    public void replaceBy(Component c) {
        if (component instanceof Component) {
            Component comp = (Component) component;
            Container parent = comp.getParent();
            if (parent != null) {
                LayoutManager layout = parent.getLayout();
                Object constraint = null;
                if (layout instanceof GridBagLayout) {
                    constraint = ((GridBagLayout) layout).getConstraints(comp);
                } else if (layout instanceof BorderLayout) {
                    constraint = ((BorderLayout) layout).getConstraints(comp);
                } else if (layout instanceof NoLayout) {
                    constraint = ((NoLayout) layout).getConstraints(comp);
                }

                if (constraint != null) {
                    parent.remove(comp);
                    parent.add(c, constraint);
                } else {
                    int pos = parent.getComponentZOrder(comp);
                    parent.remove(comp);
                    parent.add(c, pos);
                }

                parent.doLayout();
                parent.repaint();
            }
            component = c;
        }
    }

    /**
     * Get the parent UIComponent
     * @return the parent
     */
    public UIComponent getParent() {
        return parent;
    }

    /**
     * Get the id of this UIComponent
     * @return the id
     */
    public String getID() {
        return id;
    }

    /**
     * Check if the root element is visible
     * @return true if the root element is visible
     */
    public boolean isRootVisible() {
        if (isRoot()) {
            if (component instanceof Component) {
                return ((Component) component).isVisible();
            }
            return false;
        } else {
            return root.isRootVisible();
        }
    }

    /**
     * Get the children
     * @return the children as an array
     */
    public UIComponent[] getChildren() {
        return childrenList.toArray(new UIComponent[childrenList.size()]);
    }

    /**
     * Set the uistyle
     * @param style a String containing style definition as a CSS string
     */
    public void setUiStyle(String style) throws UIWidgetException {
        setUiStyle(StyleParser.parseLine(style));
    }

    /**
     * Set the uistyle
     * @param style a map
     */
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

    /**
     * Set the tab title if the component is in a JTabbedPane
     * @param title the tab title
     */
    public void setTabTitle(String title) throws UIWidgetException {
        if (parent instanceof UITab && title != null && component != null) {
            JComponent c = getJComponent();
            JTabbedPane tab = (JTabbedPane) ((UITab) parent).getJComponent();
            int index = tab.indexOfTabComponent(c);
            if (index != -1) {
                tab.setTitleAt(index, title);
            }
        }
        this.tabTitle = title;
    }

    /**
     * Get the tab title if the component is in a JTabbedPane
     * @return the tab title
     */
    public String getTabTitle() throws UIWidgetException {
        return tabTitle;
    }

    /**
     * Set the tooltip text
     * @param text the tooltip text
     */
    public void setTooltip(String text) throws UIWidgetException {
        getModifiableJComponent().setToolTipText(text);
    }

    /**
     * Get the tooltip text
     * @return the tooltip text
     */
    public String getTooltip() throws UIWidgetException {
        return getModifiableJComponent().getToolTipText();
    }

    /**
     * Set the tooltip text
     * @param text the tooltip text
     */
    public void setTooltipString(String text) throws UIWidgetException {
        setTooltip(text);
    }

    /**
     * Get the tooltip text
     * @return the tooltip text
     */
    public String getTooltipString() throws UIWidgetException {
        return getTooltip();
    }

    /**
     * Set the foreground color
     * @param c the color
     */
    public void setColor(Color c) throws UIWidgetException {
        getModifiableJComponent().setForeground(c);
    }

    /**
     * Get the foreground color
     * @return the foreground color
     */
    public Color getColor() throws UIWidgetException {
        return getModifiableJComponent().getForeground();
    }

    /**
     * Set the font name
     * @param name the font name
     */
    public void setFontName(String name) throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            map.put(TextAttribute.FAMILY, name);
            setFont(new Font(map));
        }
    }

    /**
     * Get the font name
     * @return name the font name
     */
    public String getFontName() throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            return f.getFontName();
        }

        return null;
    }

    /**
     * Set the font size
     * @param size the font size
     */
    public void setFontSize(double size) throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            map.put(TextAttribute.SIZE, Double.isNaN(size) ? new Double(12.0) : new Double(size));
            setFont(new Font(map));
        }
    }

    /**
     * Get the font size
     * @return the font size
     */
    public double getFontSize() throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            return f.getSize2D();
        }

        return -1;
    }

    /**
     * Set the font weight
     * @param weight the font weight
     */
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

    /**
     * Get the font weight
     * @return the font weight
     */
    public String getFontWeight() throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            Float fl = (Float) map.get(TextAttribute.WEIGHT);
            return UITools.mapTextAttribute.get(fl);
        }

        return null;
    }

    /**
     * Set the font angle
     * @param angle the font angle
     */
    public void setFontAngle(String angle) throws UIWidgetException {
        if (angle != null && !angle.isEmpty()) {
            angle = angle.toLowerCase();
            boolean italic = angle.equals("italic") || angle.equals("oblique") || angle.equals("it");
            Font f = getFont();
            if (f != null) {
                Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
                map.put(TextAttribute.POSTURE, italic ? TextAttribute.POSTURE_OBLIQUE : TextAttribute.POSTURE_REGULAR);
                setFont(new Font(map));
            }
        }
    }

    /**
     * Get the font angle
     * @return the font angle
     */
    public String getFontAngle() throws UIWidgetException {
        Font f = getFont();
        if (f != null) {
            Map<TextAttribute, Object> map = (Map<TextAttribute, Object>) f.getAttributes();
            Float fl = (Float) map.get(TextAttribute.POSTURE);
            return UITools.mapTextAttribute.get(fl);
        }

        return null;
    }

    /**
     * Set the font
     * @param f the font
     */
    public void setFont(Font f) throws UIWidgetException {
        getModifiableJComponent().setFont(f);
    }

    /**
     * Get the font
     * @return the font
     */
    public Font getFont() throws UIWidgetException {
        return getModifiableJComponent().getFont();
    }

    /**
     * Set the position (when NoLayout has been set)
     * @param r the position
     */
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

    /**
     * Get the position
     * @return the position
     */
    public Rectangle getPosition() throws UIWidgetException {
        Container p = getJComponent().getParent();
        if (p == null || p.getLayout() instanceof NoLayout) {
            return position;
        }

        Dimension d = getJComponent().getSize();
        Point pt = getJComponent().getLocation();

        return new Rectangle(pt.x, pt.y, d.width, d.height);
    }

    /**
     * Set the foreground color
     * @param c the color
     */
    public void setForegroundColor(Color c) throws UIWidgetException {
        if (c != null) {
            getModifiableJComponent().setForeground(c);
        }
    }

    /**
     * Get the foreground color
     * @return the foreground color
     */
    public Color getForegroundColor() throws UIWidgetException {
        return getModifiableJComponent().getForeground();
    }

    /**
     * Set the background color
     * @param c the color
     */
    public void setBackgroundColor(Color c) throws UIWidgetException {
        if (c != null) {
            getModifiableJComponent().setBackground(c);
        }
    }

    /**
     * Get the background color
     * @return the background color
     */
    public Color getBackgroundColor() throws UIWidgetException {
        return getModifiableJComponent().getBackground();
    }

    /**
     * Set the component scrollable or not
     * @param b if true the component will be scrollable
     */
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

    /**
     * Check if the component is scrollable
     * @return true if the component is scrollable
     */
    public boolean getScrollable() {
        return component instanceof JScrollPane && ((JScrollPane) component).getHorizontalScrollBarPolicy() == ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED;
    }

    /**
     * Set the horizontal block increment (pgup, pgdown in scroll)
     * @param blockIncrement the block increment
     */
    public void setHorizontalBlockIncrement(int blockIncrement) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                bar.setBlockIncrement(blockIncrement);
            }
        }
    }

    /**
     * Get the horizontal block increment
     * @return block increment
     */
    public int getHorizontalBlockIncrement() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                return bar.getBlockIncrement();
            }
        }

        return -1;
    }

    /**
     * Set the vertical block increment (pgup, pgdown in scroll)
     * @param blockIncrement the block increment
     */
    public void setVerticalBlockIncrement(int blockIncrement) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                bar.setBlockIncrement(blockIncrement);
            }
        }
    }

    /**
     * Get the vertical block increment
     * @return block increment
     */
    public int getVerticalBlockIncrement() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                return bar.getBlockIncrement();
            }
        }

        return -1;
    }

    /**
     * Set the horizontal unit increment (wheel in scroll)
     * @param unitIncrement the unit increment
     */
    public void setHorizontalUnitIncrement(int unitIncrement) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                bar.setUnitIncrement(unitIncrement);
            }
        }
    }

    /**
     * Get the horizontal unit increment
     * @return unit increment
     */
    public int getHorizontalUnitIncrement() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                return bar.getUnitIncrement();
            }
        }

        return -1;
    }

    /**
     * Set the vertical unit increment (wheel in scroll)
     * @param unitIncrement the unit increment
     */
    public void setVerticalUnitIncrement(int unitIncrement) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                bar.setUnitIncrement(unitIncrement);
            }
        }
    }

    /**
     * Get the vertical unit increment
     * @return unit increment
     */
    public int getVerticalUnitIncrement() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                return bar.getUnitIncrement();
            }
        }

        return -1;
    }

    /**
     * Set the horizontal scroll value
     * @param value the value
     */
    public void setHorizontalScrollValue(int value) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                bar.setValue(value);
            }
        }
    }

    /**
     * Get the horizontal scroll value
     * @return the scroll value
     */
    public int getHorizontalScrollValue() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getHorizontalScrollBar();
            if (bar != null) {
                return bar.getValue();
            }
        }

        return -1;
    }

    /**
     * Set the vertical scroll value
     * @param value the value
     */
    public void setVerticalScrollValue(int value) {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                bar.setValue(value);
            }
        }
    }

    /**
     * Get the vertical scroll value
     * @return the scroll value
     */
    public int getVerticalScrollValue() {
        if (component instanceof JScrollPane) {
            JScrollPane scroll = (JScrollPane) component;
            JScrollBar bar = scroll.getVerticalScrollBar();
            if (bar != null) {
                return bar.getValue();
            }
        }

        return -1;
    }

    /**
     * Set the component constraints
     * @param constraint a string containing layout constraints (CSS style)
     */
    public void setConstraint(String constraint) {
        setConstraint(StyleParser.parseLine(constraint));
    }

    /**
     * Set the component constraints
     * @param constraint a map containing layout constraints
     */
    public void setConstraint(Map<String, String> constraint) {
        this.constraint = constraint;
    }

    /**
     * Set the layout
     * @param layout the layout informations (CSS style)
     */
    public void setLayout(String layout) throws UIWidgetException {
        getModifiableContainer().setLayout(UILayoutFactory.getLayout(getModifiableJComponent(), layout));
    }

    /**
     * Enable or not the component
     * @param enable if true, the component is enabled
     */
    public void setEnable(boolean enable) throws UIWidgetException {
        getJComponent().setEnabled(enable);
    }

    /**
     * Check if the component is enabled
     * @return true if the component is enabled
     */
    public boolean getEnable() throws UIWidgetException {
        return getJComponent().isEnabled();
    }

    /**
     * Create a MouseListener on this component
     */
    private final void createMouseListener() throws UIWidgetException {
        if (mouseListener == null) {
            mouseListener = new UIMouseListener(this);
            mouseListener.newInstance();
            mouseListener.addListenerToComponent(getModifiableJComponent());
        }
    }

    /**
     * Set onmouseclick action
     * @param command the command to execute
     */
    public void setOnmouseclick(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseclick(command);
        }
    }

    /**
     * Set onmouseover action
     * @param command the command to execute
     */
    public void setOnmouseover(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseover(command);
        }
    }

    /**
     * Set onmouseenter action
     * @param command the command to execute
     */
    public void setOnmouseenter(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseenter(command);
        }
    }

    /**
     * Set onmouseexit action
     * @param command the command to execute
     */
    public void setOnmouseexit(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouseexit(command);
        }
    }

    /**
     * Set onmousepress action
     * @param command the command to execute
     */
    public void setOnmousepress(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousepress(command);
        }
    }

    /**
     * Set onmouserelease action
     * @param command the command to execute
     */
    public void setOnmouserelease(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmouserelease(command);
        }
    }

    /**
     * Set onmousewheel action
     * @param command the command to execute
     */
    public void setOnmousewheel(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousewheel(command);
        }
    }

    /**
     * Set onmousedrag action
     * @param command the command to execute
     */
    public void setOnmousedrag(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createMouseListener();
            mouseListener.setOnmousedrag(command);
        }
    }

    /**
     * Get onmouseclick action
     * @return the command to execute
     */
    public String getOnmouseclick() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseclick();
        }

        return null;
    }

    /**
     * Get onmouseover action
     * @return the command to execute
     */
    public String getOnmouseover() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseover();
        }

        return null;
    }

    /**
     * Get onmouseenter action
     * @return the command to execute
     */
    public String getOnmouseenter() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseenter();
        }

        return null;
    }

    /**
     * Get onmouseexit action
     * @return the command to execute
     */
    public String getOnmouseexit() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseexit();
        }

        return null;
    }

    /**
     * Get onmousepress action
     * @return the command to execute
     */
    public String getOnmousepress() {
        if (mouseListener != null) {
            return mouseListener.getOnmousepress();
        }

        return null;
    }

    /**
     * Get onmouserelease action
     * @return the command to execute
     */
    public String getOnmouserelease() {
        if (mouseListener != null) {
            return mouseListener.getOnmouserelease();
        }

        return null;
    }

    /**
     * Get onmousedrag action
     * @return the command to execute
     */
    public String getOnmousedrag() {
        if (mouseListener != null) {
            return mouseListener.getOnmousedrag();
        }

        return null;
    }

    /**
     * Get onmousewheel action
     * @return the command to execute
     */
    public String getOnmousewheel() {
        if (mouseListener != null) {
            return mouseListener.getOnmousewheel();
        }

        return null;
    }

    /**
     * Enable onmouseclick
     * @param b, if true the action is enabled
     */
    public void setOnmouseclickEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseclickEnable(b);
        }
    }

    /**
     * Check if onmouseclick is enabled
     * @return true the action is enabled
     */
    public boolean getOnmouseclickEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseclickEnable();
        }

        return false;
    }

    /**
     * Enable onmouseover
     * @param b, if true the action is enabled
     */
    public void setOnmouseoverEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseoverEnable(b);
        }
    }

    /**
     * Check if onmouseover is enabled
     * @return true the action is enabled
     */
    public boolean getOnmouseoverEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseoverEnable();
        }

        return false;
    }

    /**
     * Enable onmouseenter
     * @param b, if true the action is enabled
     */
    public void setOnmouseenterEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseenterEnable(b);
        }
    }

    /**
     * Check if onmouseenter is enabled
     * @return true the action is enabled
     */
    public boolean getOnmouseenterEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseenterEnable();
        }

        return false;
    }

    /**
     * Enable onmouseexit
     * @param b, if true the action is enabled
     */
    public void setOnmouseexitEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmouseexitEnable(b);
        }
    }

    /**
     * Check if onmouseexit is enabled
     * @return true the action is enabled
     */
    public boolean getOnmouseexitEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmouseexitEnable();
        }

        return false;
    }

    /**
     * Enable onmousepress
     * @param b, if true the action is enabled
     */
    public void setOnmousepressEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousepressEnable(b);
        }
    }

    /**
     * Check if onmousepress is enabled
     * @return true the action is enabled
     */
    public boolean getOnmousepressEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmousepressEnable();
        }

        return false;
    }

    /**
     * Enable onmouserelease
     * @param b, if true the action is enabled
     */
    public void setOnmousereleaseEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousereleaseEnable(b);
        }
    }

    /**
     * Check if onmouserelease is enabled
     * @return true the action is enabled
     */
    public boolean getOnmousereleaseEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmousereleaseEnable();
        }

        return false;
    }

    /**
     * Enable onmousewheel
     * @param b, if true the action is enabled
     */
    public void setOnmousewheelEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousewheelEnable(b);
        }
    }

    /**
     * Check if onmousewheel is enabled
     * @return true the action is enabled
     */
    public boolean getOnmousewheelEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmousewheelEnable();
        }

        return false;
    }

    /**
     * Enable onmousedrag
     * @param b, if true the action is enabled
     */
    public void setOnmousedragEnable(boolean b) {
        if (mouseListener != null) {
            mouseListener.setOnmousedragEnable(b);
        }
    }

    /**
     * Check if onmousedrag is enabled
     * @return true the action is enabled
     */
    public boolean getOnmousedragEnable() {
        if (mouseListener != null) {
            return mouseListener.getOnmousedragEnable();
        }

        return false;
    }

    /**
     * Create a FocusListener on this component
     */
    private final void createFocusListener() throws UIWidgetException {
        if (focusListener == null) {
            focusListener = new UIFocusListener(this);
            focusListener.newInstance();
            focusListener.addListenerToComponent(getModifiableJComponent());
        }
    }

    /**
     * Set onfocusgain action
     * @param command the command to execute
     */
    public void setOnfocusgain(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createFocusListener();
            focusListener.setOnfocusgain(command);
        }
    }

    /**
     * Set onfocusloss action
     * @param command the command to execute
     */
    public void setOnfocusloss(String command) throws UIWidgetException {
        if (command != null && !command.isEmpty()) {
            createFocusListener();
            focusListener.setOnfocusloss(command);
        }
    }

    /**
     * Get onfocusgain action
     * @return the command to execute
     */
    public String getOnfocusgain() {
        if (focusListener != null) {
            return focusListener.getOnfocusgain();
        }

        return null;
    }

    /**
     * Get onfocusloss action
     * @return the command to execute
     */
    public String getOnfocusloss() {
        if (focusListener != null) {
            return focusListener.getOnfocusloss();
        }

        return null;
    }

    /**
     * Enable onfocusgain
     * @param b, if true the action is enabled
     */
    public void setOnfocusgainEnable(boolean b) {
        if (focusListener != null) {
            focusListener.setOnfocusgainEnable(b);
        }
    }

    /**
     * Check if onfocusgain is enabled
     * @return true the action is enabled
     */
    public boolean getOnfocusgainEnable() {
        if (focusListener != null) {
            return focusListener.getOnfocusgainEnable();
        }

        return false;
    }

    /**
     * Enable onfocusloss
     * @param b, if true the action is enabled
     */
    public void setOnfocuslossEnable(boolean b) {
        if (focusListener != null) {
            focusListener.setOnfocuslossEnable(b);
        }
    }

    /**
     * Check if onfocusloss is enabled
     * @return true the action is enabled
     */
    public boolean getOnfocuslossEnable() {
        if (focusListener != null) {
            return focusListener.getOnfocuslossEnable();
        }

        return false;
    }

    /**
     * Change the parent and update the dependencies
     * @param parent the parent
     */
    public void updateDependencies(UIComponent parent) throws UIWidgetException {
        setParent(parent);
    }

    /**
     * Add a list of children
     * @param list the children
     */
    public void add(List<UIComponent> list) throws UIWidgetException {
        for (UIComponent uicomp : list) {
            add(uicomp);
        }
    }

    /**
     * Add an UIComponent to the children list
     * @parent uicomp the child to add
     */
    public void add(final UIComponent uicomp) throws UIWidgetException {
        UIAccessTools.add(this, uicomp);
    }

    /**
     * Add an UIListener
     * @parent uicomp the listener to add
     */
    public void addListener(final UIListener uicomp) throws UIWidgetException {
        uicomp.addListenerToComponent(getModifiableJComponent());
    }

    /**
     * Add a popup menu
     * @param popup the popup menu
     */
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

    /**
     * Set a property of this component
     * @param name the property name
     * @param value the property value
     */
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

    /**
     * Set a property (via relection) of this component
     * @param name the property name
     * @param value the property value
     */
    protected final void setPropertyViaReflectionInThis(final String name, final String value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(this, name, value);
    }

    /**
     * Set a property (via relection) of the modifiable component
     * @param name the property name
     * @param value the property value
     */
    protected final void setPropertyViaReflectionInComponent(final String name, final String value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(getModifiableComponent(), name, value);
    }

    /**
     * Set a property of this component
     * @param name the property name
     * @param value the property value
     */
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

    /**
     * Set a property (via relection) of this component
     * @param name the property name
     * @param value the property value
     */
    protected final void setPropertyViaReflectionInThis(final String name, final ScilabType value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(this, name, value);
    }

    /**
     * Set a property (via relection) of the modifiable component
     * @param name the property name
     * @param value the property value
     */
    protected final void setPropertyViaReflectionInComponent(final String name, final ScilabType value) throws UIWidgetException {
        UIAccessTools.setPropertyViaReflection(getModifiableComponent(), name, value);
    }

    /**
     * Get the pairs property name -- method name
     * @return the pairs
     */
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

    /**
     * Get a property value
     * @param name the property name
     */
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

    /**
     * Get a property value (via reflection)
     * @param name the property name
     */
    protected Object getPropertyViaReflectionInThis(final String name) throws UIWidgetException {
        return UIAccessTools.getPropertyViaReflection(this, name);
    }

    /**
     * Get a property value (via reflection) in the modifiable component
     * @param name the property name
     */
    protected Object getPropertyViaReflectionInComponent(final String name) throws UIWidgetException {
        return UIAccessTools.getPropertyViaReflection(getModifiableComponent(), name);
    }

    /**
     * Create a new instance
     * @param attributes the attributes
     */
    private void createNewInstance(final ConvertableMap attributes) throws UIWidgetException {
        Set<String> uselessAttrs = UIAccessTools.createNewInstance(this, attributes);
        if (uselessAttrs.contains("scrollable")) {
            boolean scrollable = (Boolean) attributes.get(boolean.class, "scrollable", false);
            if (scrollable && component instanceof JComponent) {
                modifiableComponent = component;
                if (SwingUtilities.isEventDispatchThread()) {
                    component = new JScrollPane((JComponent) modifiableComponent);
                } else {
                    try {
                        SwingUtilities.invokeAndWait(new Runnable() {
                            public void run() {
                                try {
                                    component = new JScrollPane((JComponent) modifiableComponent);
                                } catch (Exception e) {
                                    System.err.println(e);
                                }
                            }
                        });
                    } catch (Exception e) {
                        System.err.println(e);
                    }
                }
                uselessAttrs.remove("scrollable");
            }
        }

        setAttributesAndStyle(attributes, uselessAttrs);
    }

    /**
     * Set the attributes and style
     * @param attributes the attributes
     * @param uselessAttrs the useless attributes
     */
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

    /**
     * {@inheritdoc}
     */
    protected void finalize() throws Throwable {
        UserData.removeUIWidgetUserData(uid);
        super.finalize();
    }
}
