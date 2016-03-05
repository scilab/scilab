/* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO
 * Copyright (C) 2010 - Calixte DENIZET
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

package org.scilab.modules.scinotes;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JButton;
import javax.swing.JPopupMenu;
import javax.swing.KeyStroke;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.commons.gui.ScilabLAF;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.toolbar.SwingScilabToolBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.checkboxmenuitem.CheckBoxMenuItem;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.localization.Messages;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * Class SciNotesGUI handles menus, bar, ...
 */
public final class SciNotesGUI {

    private static final String MENU_CONF = System.getenv("SCI") + "/modules/scinotes/etc/scinotesGUIConfiguration.xml";
    private static final String ERROR_READ = "Could not load file: ";
    private static final String MENUBAR = "MenuBar";
    private static final String TOOLBAR = "ToolBar";
    private static final String RIGHTCLICKMENU = "RightClickMenu";
    private static final String MENU = "menu";
    private static final String MNEMONIC = "mnemonic";
    private static final String LABEL = "label";
    private static final String MENUITEM = "menuitem";
    private static final String ACTION = "action";
    private static final String SEPARATOR = "separator";
    private static final String TOOLTIP = "tooltip";
    private static final String ICON = "icon";
    private static final String BUTTON = "button";
    private static final String DOT = ".";

    private static final String DEFAULTACTIONPATH = "org.scilab.modules.scinotes.actions";

    private static Document menuConf;

    private static Map<SciNotes, MenuBar> mapMenuBar = new HashMap<SciNotes, MenuBar>();
    private static Map<SciNotes, ToolBar> mapToolBar = new HashMap<SciNotes, ToolBar>();
    private static Map<SciNotes, JPopupMenu> mapPopup = new HashMap<SciNotes, JPopupMenu>();
    private static Map<SciNotes, TextBox> mapInfoBar = new HashMap<SciNotes, TextBox>();

    /**
     * initialize the window
     * @param mainWindow Windows
     * @param editorInstance SciNotes
     * @param title the title
     */
    public static void init(SwingScilabWindow mainWindow, final SciNotes editorInstance, String title) {
        mainWindow.setTitle(title);
        mainWindow.addTab(editorInstance);

        // Set SciNotes Window position /size
        mainWindow.setTitle(title);
        mainWindow.setVisible(true);

        init(editorInstance);
    }

    /**
     * Initialize SciNotes
     * @param editorInstance SciNotes
     */
    public static void init(SciNotes editorInstance) {
        editorInstance.setMenuBar(generateMenuBar(editorInstance));
        editorInstance.setToolBar(generateToolBar(editorInstance));
        editorInstance.setInfoBar(generateInfoBar(editorInstance));
    }

    public static void clean(SciNotes editorInstance) {
        mapMenuBar.remove(editorInstance);
        mapToolBar.remove(editorInstance);
        mapPopup.remove(editorInstance);
        mapInfoBar.remove(editorInstance);
    }

    /**
     * Initialize SciNotes
     * @param editorInstance SciNotes
     */
    public static void reinitMenus(SciNotes editorInstance) {
        mapMenuBar.remove(editorInstance);
        mapPopup.remove(editorInstance);
        editorInstance.setMenuBar(generateMenuBar(editorInstance));
    }

    /**
     * @param editor the editor
     * @return the infobar of the editor
     */
    public static TextBox generateInfoBar(SciNotes editor) {
        if (mapInfoBar.containsKey(editor)) {
            return mapInfoBar.get(editor);
        }

        TextBox info = ScilabTextBox.createTextBox();
        mapInfoBar.put(editor, info);

        return info;
    }

    /**
     * @param editor the editor
     * @return the JPopupMenu read in the conf file
     */
    public static JPopupMenu generateRightClickPopup(SciNotes editor) {
        if (mapPopup.containsKey(editor)) {
            return mapPopup.get(editor);
        }

        readMenusConf();
        JPopupMenu popup = new JPopupMenu();
        Element root = menuConf.getDocumentElement();
        Element rightmenu = (Element) root.getElementsByTagName(RIGHTCLICKMENU).item(0);
        NodeList menus = rightmenu.getChildNodes();
        for (int i = 0; i < menus.getLength(); i++) {
            Node item = menus.item(i);
            if (MENUITEM.equals(item.getNodeName())) {
                Element elem = (Element) item;
                Object menuitem = getMenuItem(elem.getAttribute(ACTION), elem.getAttribute(LABEL), editor);
                if (menuitem != null) {
                    if (menuitem instanceof CheckBoxMenuItem) {
                        SwingScilabCheckBoxMenuItem checkbox = (SwingScilabCheckBoxMenuItem) ((CheckBoxMenuItem) menuitem).getAsSimpleCheckBoxMenuItem();
                        popup.add(checkbox);
                    } else if (menuitem instanceof MenuItem) {
                        SwingScilabMenuItem smi = (SwingScilabMenuItem) ((MenuItem) menuitem).getAsSimpleMenuItem();
                        popup.add(smi);
                    } else if (menuitem instanceof Menu) {
                        SwingScilabMenu sm = (SwingScilabMenu) ((Menu) menuitem).getAsSimpleMenu();
                        popup.add(sm);
                    }
                }
            } else if (SEPARATOR.equals(item.getNodeName())) {
                popup.addSeparator();
            } else if (MENU.equals(item.getNodeName())) {
                SwingScilabMenu sm = (SwingScilabMenu) getMenu(editor, (Element) item).getAsSimpleMenu();
                popup.add(sm);
            }
        }
        mapPopup.put(editor, popup);

        return popup;
    }

    /**
     * @param editor the editor
     * @return the ToolBar read in the conf file
     */
    public static ToolBar generateToolBar(SciNotes editor) {
        if (mapToolBar.containsKey(editor)) {
            return mapToolBar.get(editor);
        }

        readMenusConf();
        ToolBar toolBar = ScilabToolBar.createToolBar();
        Element root = menuConf.getDocumentElement();
        Element toolbar = (Element) root.getElementsByTagName(TOOLBAR).item(0);
        NodeList buttons = toolbar.getChildNodes();
        for (int i = 0; i < buttons.getLength(); i++) {
            Node node = buttons.item(i);
            if (BUTTON.equals(node.getNodeName())) {
                Element elem = (Element) node;
                JButton button = getButton(elem.getAttribute(ACTION), elem.getAttribute(TOOLTIP), elem.getAttribute(ICON), editor);
                ((SwingScilabToolBar) toolBar.getAsSimpleToolBar()).add(button);
            } else if (SEPARATOR.equals(node.getNodeName())) {
                toolBar.addSeparator();
            }
        }
        mapToolBar.put(editor, toolBar);

        return toolBar;
    }

    /**
     * @param editor the editor
     * @return the MenuBar read in the conf file
     */
    public static MenuBar generateMenuBar(SciNotes editor) {
        if (mapMenuBar.containsKey(editor)) {
            return mapMenuBar.get(editor);
        }

        readMenusConf();
        MenuBar menuBar = ScilabMenuBar.createMenuBar();
        Element root = menuConf.getDocumentElement();
        Element menubar = (Element) root.getElementsByTagName(MENUBAR).item(0);
        NodeList menus = menubar.getChildNodes();
        for (int i = 0; i < menus.getLength(); i++) {
            Node node = menus.item(i);
            if (MENU.equals(node.getNodeName())) {
                menuBar.add(getMenu(editor, (Element) node));
            }
        }
        mapMenuBar.put(editor, menuBar);

        return menuBar;
    }

    /**
     * Read the conf file
     */
    private static void readMenusConf() {
        File xml = null;
        DocumentBuilder docBuilder = null;

        try {
            if (menuConf == null) {
                DocumentBuilderFactory factory = ScilabDocumentBuilderFactory.newInstance();
                docBuilder = factory.newDocumentBuilder();
                xml = new File(MENU_CONF);
                menuConf = docBuilder.parse(xml);
            }
        } catch (ParserConfigurationException pce) {
            System.err.println(ERROR_READ + MENU_CONF);
        } catch (SAXException se) {
            System.err.println(ERROR_READ + MENU_CONF);
        } catch (IOException ioe) {
            System.err.println(ERROR_READ + MENU_CONF);
        }
    }

    /**
     * @param editor the editor
     * @param node the node containing the information
     * @return the Menu
     */
    private static Menu getMenu(SciNotes editor, Element node) {
        Menu menu = ScilabMenu.createMenu();
        menu.setText(Messages.gettext(node.getAttribute(LABEL)));
        String mnemonic = node.getAttribute(MNEMONIC);
        if (mnemonic != null && mnemonic.length() != 0) {
            menu.setMnemonic(mnemonic.charAt(0));
        }
        NodeList elements = node.getChildNodes();
        for (int i = 0; i < elements.getLength(); i++) {
            Node item = elements.item(i);
            if (MENUITEM.equals(item.getNodeName())) {
                Element elem = (Element) item;
                Object menuitem = getMenuItem(elem.getAttribute(ACTION), elem.getAttribute(LABEL), editor);
                if (menuitem != null) {
                    if (menuitem instanceof CheckBoxMenuItem) {
                        menu.add((CheckBoxMenuItem) menuitem);
                    } else if (menuitem instanceof MenuItem) {
                        menu.add((MenuItem) menuitem);
                    } else if (menuitem instanceof Menu) {
                        menu.add((Menu) menuitem);
                    }
                }
            } else if (SEPARATOR.equals(item.getNodeName())) {
                menu.addSeparator();
            } else if (MENU.equals(item.getNodeName())) {
                menu.add(getMenu(editor, (Element) item));
            }
        }

        return menu;
    }

    /**
     * @param action a string with the action associated with the button
     * @param label the title of the menu
     * @param editor the editor
     * @return a MenuItem or a CheckBoxMenuItem or a Menu
     */
    private static Object getMenuItem(String action, String label, SciNotes editor) {
        ClassLoader loader = ClassLoader.getSystemClassLoader();
        String className = "";
        Method method = null;
        String actionClass = SciNotes.getActionName().get(action);
        try {
            if (action.lastIndexOf(DOT) != -1)  {
                className = actionClass;
            } else {
                className = DEFAULTACTIONPATH + DOT + actionClass;
            }
            Class clazz = loader.loadClass(className);
            method = clazz.getMethod("createMenu", new Class[] {String.class, SciNotes.class, KeyStroke.class});
        } catch (ClassNotFoundException e) {
            System.err.println("No action: " + className);
        } catch (NoSuchMethodException e) {
            System.err.println("No valid method createMenu in action: " + className);
        }

        if (method == null) {
            return null;
        }

        try {
            return method.invoke(null, new Object[] {Messages.gettext(label), editor, SciNotes.getActionKeys().get(action)});
        } catch (InvocationTargetException e) {
            System.err.println("Warning: problem to create the menu for action: " + className);
            System.err.println("The menu label is: " + Messages.gettext(label));
            System.err.println("English version will be used instead.");
            System.err.println("Please report a bug at: http://bugzilla.scilab.org");
            try {
                return method.invoke(null, new Object[] {label, editor, SciNotes.getActionKeys().get(action)});
            } catch (InvocationTargetException ex) {
                System.err.println("Problem to create menu of the action: " + className);
            } catch (IllegalAccessException ex) {
                System.err.println("The method createMenu must be public: " + className);
            }
        } catch (IllegalAccessException e) {
            System.err.println("The method createMenu must be public: " + className);
        }

        return null;
    }

    /**
     * @param action a string with the action associated with the button
     * @param tooltip a string with the tooltip
     * @param icon the icon name
     * @param editor the editor
     * @return the button
     */
    private static JButton getButton(String action, String tooltip, String icon, SciNotes editor) {
        ClassLoader loader = ClassLoader.getSystemClassLoader();
        String className = "";
        String actionClass = SciNotes.getActionName().get(action);
        try {
            if (action.lastIndexOf(DOT) != -1)  {
                className = actionClass;
            } else {
                className = DEFAULTACTIONPATH + DOT + actionClass;
            }
            Class clazz = loader.loadClass(className);
            Method method = clazz.getMethod("createButton", new Class[] {String.class, String.class, SciNotes.class});
            JButton button = (JButton) method.invoke(null, new Object[] {Messages.gettext(tooltip), icon, editor});
            ScilabLAF.setDefaultProperties(button);

            return button;
        } catch (ClassNotFoundException e) {
            System.err.println("No action: " + className);
        } catch (NoSuchMethodException e) {
            System.err.println("No valid method createButton in action: " + className);
        } catch (IllegalAccessException e) {
            System.err.println("The method createButton must be public: " + className);
        } catch (InvocationTargetException e) {
            System.err.println("The method createButton in " + className + " threw an exception :");
            e.printStackTrace();
        }

        return null;
    }
}
