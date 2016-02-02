/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Vincent Couvert
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

package org.scilab.modules.gui.utils;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
import java.util.Collection;
import java.util.TreeSet;

import javax.xml.parsers.ParserConfigurationException;

import org.scilab.modules.commons.gui.ScilabKeyStroke;
import org.scilab.modules.commons.xml.ScilabDocumentBuilderFactory;
import org.scilab.modules.graphic_objects.graphicObject.CallBack;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.localization.Messages;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * Create a menuBar from an XML file
 * @author Vincent COUVERT
 */
public final class MenuBarBuilder {

    private static final String FILE_NOT_FOUND = "Could not find file: ";

    private static final String CANNOT_CREATE_MENUBAR = "Cannot create MenuBar.\n"
            + "Check if file *_menubar.xml is available and valid.";

    private static int figureIndex;

    /**
     * Default constructor
     */
    private MenuBarBuilder() {
        throw new UnsupportedOperationException();
    }

    /**
     * Create a proxy to access a XML file
     * @param resultClass class for the return value
     * @param fileToLoad XML file to load
     * @return a proxy used to read the XML file
     * @throws SAXException can be thrown when an error occurs while reading the file
     * @throws IOException can be thrown when an error occurs while accessing the file
     * @throws ParserConfigurationException can be thrown when an error occurs while parsing the file
     */
    public static Object buildMenuBar(Class[] resultClass, String fileToLoad) throws SAXException, IOException, ParserConfigurationException {

        InvocationHandler invocationHandler = new MenuBarConfigurationHandler(fileToLoad);

        return Proxy.newProxyInstance(invocationHandler.getClass().getClassLoader(), resultClass, invocationHandler);
    }

    /**
     * Create a Scilab menubar from data in a XML file
     * @param fileToLoad XML file to load
     * @return the menubar created
     */
    public static MenuBar buildMenuBar(String fileToLoad) {
        return buildMenuBar(fileToLoad, 0);
    }

    /**
     * Create a Scilab menubar from data in a XML file
     * @param fileToLoad XML file to load
     * @param figureIndex the index of the figure in Scilab (for graphics figures only)
     * @return the menubar created
     */
    public static MenuBar buildMenuBar(String fileToLoad, int figureIndex) {

        MenuBarBuilder.figureIndex = figureIndex;

        MenuBar menubar = ScilabMenuBar.createMenuBar();

        try {
            MenuBarConfiguration menuBarConfig =
                (MenuBarConfiguration) buildMenuBar(new Class[] {MenuBarConfiguration.class}, fileToLoad);
            menuBarConfig.addMenus(menubar);
        } catch (IllegalArgumentException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (SAXException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (IOException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        } catch (ParserConfigurationException e) {
            System.err.println(CANNOT_CREATE_MENUBAR);
            System.err.println(FILE_NOT_FOUND + e.getLocalizedMessage());
        }

        return menubar;
    }

    /**
     * Class used to read the XMl file
     */
    private static class MenuBarConfigurationHandler implements InvocationHandler {
        protected static final String LABEL = "label";
        protected static final String MENU = "menu";
        protected static final String MNEMONIC = "mnemonic";
        protected static final String SUBMENU = "submenu";
        protected static final String SEPARATOR = "separator";
        protected static final String ENABLED = "enabled";
        protected static final String ACCELERATOR = "accelerator";
        protected static final String CALLBACK = "callback";
        protected static final String TYPE = "type";
        protected static final String INSTRUCTION = "instruction";
        protected static final String TRUE = "true";

        private Document dom;
        private Collection<String> internalMethodNames;


        /**
         * Constructor
         * @param xmlFile XML file to load
         * @throws SAXException can be thrown when an error occurs while reading the file
         * @throws IOException can be thrown when an error occurs while accessing the file
         * @throws ParserConfigurationException can be thrown when an error occurs while parsing the file
         */
        public MenuBarConfigurationHandler(String xmlFile) throws SAXException, IOException, ParserConfigurationException {

            if (!new File(xmlFile).exists()) {
                throw new java.io.IOException();
            }

            // Build dictionary for internal method
            internalMethodNames = new TreeSet<String>();
            Method[] internalMethodes = this.getClass().getMethods();
            for (Method method : internalMethodes) {
                internalMethodNames.add(method.getName());
            }

            // Build xml document for request
            dom = ScilabDocumentBuilderFactory.newInstance().newDocumentBuilder().parse(new File(xmlFile));
        }

        /**
         * Invoke a proxy to get data
         * @param proxy the proxy to call
         * @param method the method to call
         * @param args the arguments for the method
         * @return the object read
         * @throws IllegalAccessException thrown when the method called is inaccessible
         * @throws InvocationTargetException thorwn when the method called threw an exception
         * @throws NoSuchMethodException thrown when invoking a non-existing method
         * @see java.lang.reflect.InvocationHandler#invoke(java.lang.Object, java.lang.reflect.Method, java.lang.Object[])
         */
        public Object invoke(Object proxy, Method method, Object[] args)
        throws IllegalAccessException, InvocationTargetException, NoSuchMethodException {
            if (internalMethodNames.contains(method.getName())) {
                return getClass().getMethod(method.getName(), method.getParameterTypes()).invoke(this, args);
            } else {
                return null;
            }
        }

        /**
         * Add menus to a menubar
         * @param mb the menubar which the menus will be added to
         * @see org.scilab.modules.gui.utils.MenuBarConfiguration#addMenus(org.scilab.modules.gui.menubar.MenuBar)
         */
        public void addMenus(MenuBar mb) {

            NodeList menus = dom.getElementsByTagName(MENU);
            Menu menu = ScilabMenu.createMenu();
            for (int i = 0; i < menus.getLength(); i++) {
                menu = ScilabMenu.createMenu();
                menu.setText(Messages.gettext(menus.item(i).getAttributes().getNamedItem(LABEL).getNodeValue()));
                if (menus.item(i).getAttributes().getNamedItem(MNEMONIC) != null) {
                    menu.setMnemonic(menus.item(i).getAttributes().getNamedItem(MNEMONIC).getNodeValue().charAt(0));
                }
                if (menus.item(i).getAttributes().getNamedItem(ENABLED) != null) {
                    menu.setEnabled(menus.item(i).getAttributes().getNamedItem(ENABLED).getNodeValue().equals(TRUE));
                }
                addSubMenus(menu, i);
                mb.add(menu);
            }

        }

        /**
         * Read submenus data in the XML file
         * @param menu the parent menu for submenus
         * @param index the index of the parent in menu list
         */
        public void addSubMenus(Menu menu, int index) {
            Node submenu = dom.getElementsByTagName(MENU).item(index).getFirstChild();
            //Menu menuChild = ScilabMenu.createMenu();

            while (submenu != null) {
                if (submenu.getNodeName() == SEPARATOR) {
                    // Add a separator
                    menu.addSeparator();
                } else if (submenu.getNodeName() == SUBMENU) {
                    // Add a submenu
                    MenuItem menuItem = ScilabMenuItem.createMenuItem();
                    // Add the submenu to the parent menu
                    menu.add(menuItem);

                    // First we have to read its attributes
                    NamedNodeMap attributes = submenu.getAttributes();

                    for (int i = 0; i < attributes.getLength(); i++) {
                        if (attributes.item(i).getNodeName() == LABEL) {
                            menuItem.setText(Messages.gettext(attributes.item(i).getNodeValue()));
                        } else if (attributes.item(i).getNodeName() == MNEMONIC) {
                            menuItem.setMnemonic(attributes.item(i).getNodeValue().charAt(0));
                        } else if (attributes.item(i).getNodeName() == ENABLED) {
                            menuItem.setEnabled(attributes.item(i).getNodeValue().equals(TRUE));
                        } else if (attributes.item(i).getNodeName() == ACCELERATOR) {
                            SwingScilabMenuItem smenuitem = (SwingScilabMenuItem) menuItem.getAsSimpleMenuItem();
                            smenuitem.setAccelerator(ScilabKeyStroke.getKeyStroke(attributes.item(i).getNodeValue()));
                        }
                    }

                    // Then we get its callback (if exists)
                    Node callback = submenu.getFirstChild();
                    while (callback != null) {
                        if (callback.getNodeName() == CALLBACK) {
                            NamedNodeMap cbAttributes = callback.getAttributes();
                            String command = null;
                            int commandType = CallBack.UNTYPED;
                            for (int j = 0; j < cbAttributes.getLength(); j++) {
                                if (cbAttributes.item(j).getNodeName() == INSTRUCTION) {
                                    command = cbAttributes.item(j).getNodeValue();
                                } else if (cbAttributes.item(j).getNodeName() == TYPE) {
                                    commandType = Integer.parseInt(cbAttributes.item(j).getNodeValue());
                                }
                            }
                            if (command != null && commandType != CallBack.UNTYPED) {
                                menuItem.setCallback(CommonCallBack.createCallback(replaceFigureID(command), commandType));
                            }
                        } else if (callback.getNodeName() == SUBMENU) {
                            addSubMenuItem(menuItem, callback);
                        }
                        // Read next child
                        callback = callback.getNextSibling();
                    }

                }
                // Read next child
                submenu = submenu.getNextSibling();
            }
        }

        /**
         * Add submenu for menu
         * @param menuItem will become a menu with subMenuItems
         * @param node to get attributes of the menu
         */
        public void addSubMenuItem(MenuItem menuItem, Node node) {

            NamedNodeMap attributes = node.getAttributes();
            MenuItem subMenuItem = ScilabMenuItem.createMenuItem();

            for (int i = 0; i < attributes.getLength(); i++) {
                if (attributes.item(i).getNodeName() == LABEL) {
                    subMenuItem.setText(Messages.gettext(attributes.item(i).getNodeValue()));
                    subMenuItem.setText(Messages.gettext(attributes.item(i).getNodeValue()));
                } else if (attributes.item(i).getNodeName() == MNEMONIC) {
                    subMenuItem.setMnemonic(attributes.item(i).getNodeValue().charAt(0));
                } else if (attributes.item(i).getNodeName() == ENABLED) {
                    subMenuItem.setEnabled(attributes.item(i).getNodeValue().equals(TRUE));
                }
            }

            // Then we get its callback (if exists)
            Node callback = node.getFirstChild();
            while (callback != null) {
                if (callback.getNodeName() == CALLBACK) {
                    NamedNodeMap cbAttributes = callback.getAttributes();
                    String command = null;
                    int commandType = CallBack.UNTYPED;
                    for (int j = 0; j < cbAttributes.getLength(); j++) {
                        if (cbAttributes.item(j).getNodeName() == INSTRUCTION) {
                            command = cbAttributes.item(j).getNodeValue();
                        } else if (cbAttributes.item(j).getNodeName() == TYPE) {
                            commandType = Integer.parseInt(cbAttributes.item(j).getNodeValue());
                        }
                    }
                    if (command != null && commandType != CallBack.UNTYPED) {
                        subMenuItem.setCallback(CommonCallBack.createCallback(replaceFigureID(command), commandType));
                    }
                } else if (callback.getNodeName() == SUBMENU) {
                    addSubMenuItem(subMenuItem, callback);
                }
                // Read next child
                callback = callback.getNextSibling();
            }
            menuItem.add(subMenuItem);

        }


        /**
         * Replace pattern [SCILAB_FIGURE_ID] by the figure index
         * @param initialString string read in XML file
         * @return callback string
         */
        private String replaceFigureID(String initialString) {
            return initialString.replaceAll("\\[SCILAB_FIGURE_ID\\]", Integer.toString(MenuBarBuilder.figureIndex));
        }

    }
}
