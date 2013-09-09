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

import java.awt.Component;
import java.awt.Container;
import java.awt.Frame;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import javax.swing.JComponent;
import javax.swing.SwingUtilities;

import org.scilab.modules.graphic_objects.GraphicObjectBuilder;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabHandle;
import org.scilab.modules.types.ScilabStackPutter;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;

/**
 * Main interface between Scilab gateway and Java objects
 */
public class UIWidget {

    private static UIWidgetHandler handler;

    /**
     * Load a uiwidget XML file
     * @param filename the file name
     */
    public static int uiwidgetLoad(String filename) throws Exception {
        final XMLToUIComponentConverter converter = new XMLToUIComponentConverter(filename);
        converter.convert();

        return converter.rootUIComponent.getUid();
    }

    /**
     * Get the handler id to use to send Scilab data
     * @return the handler id
     */
    public static int getUIWidgetHandler() {
        if (handler == null) {
            handler = new UIWidgetHandler();
        }

        return handler.getHandlerId();
    }

    /**
     * Get the pairs property name -- property value
     * @param the UIComponent handle
     */
    public static String[][] uigetPropertyPairs(int uid) {
        final UIComponent comp = UILocator.get(uid);
        if (comp == null) {
            return new String[0][0];
        }

        return null;
    }

    /**
     * Get the handle of an UIComponent accordint to its path
     * @param path the UICompoent path
     * @return the corresponding handle
     */
    public static int getUidFromPath(final String path) {
        try {
            UIComponent comp = UILocator.get(path);
            if (comp == null) {
                return -1;
            }

            return comp.getUid();
        } catch (Exception e) {
            return -1;
        }
    }

    /**
     * Get the roots component
     * @param stackPos the position where to put the result
     */
    public static void getRoots(int stackPos) {
        UIComponent[] roots = UILocator.getRoots();
        ObjectToScilabConverters.putOnScilabStack(roots, stackPos);
    }

    /**
     * Get a property value of the UIComponent with the given handle and ut the result on the Scilab stack.
     * @param uid the handle
     * @param property the property name
     * @param stackPos the stack position
     */
    public static void uiget(int uid, String property, int stackPos) throws Exception {
        if (property == null || property.isEmpty()) {
            throw new Exception("Invalid argument: A valid property name expected");
        }

        final UIComponent comp = UILocator.get(uid);

        if (comp == null) {
            throw new Exception("Invalid first argument: A valid uiwidget identifier expected");
        }

        String p = property.toLowerCase();
        // The properties root, path & parent need to be quickly retrieve (no invokeAndWait)
        if (p.equals("root")) {
            ObjectToScilabConverters.putOnScilabStack(comp.getRoot(), stackPos);
        } else if (p.equals("path")) {
            ObjectToScilabConverters.putOnScilabStack(comp.getPath(), stackPos);
        } else if (p.equals("parent")) {
            ObjectToScilabConverters.putOnScilabStack(comp.getParent(), stackPos);
        } else if (p.equals("id") || p.equals("tag")) {
            ObjectToScilabConverters.putOnScilabStack(comp.getId(), stackPos);
        } else {
            Object o = comp.getProperty(property);
            ObjectToScilabConverters.putOnScilabStack(o, stackPos);
        }
    }

    /**
     * Delete the UIComponent with the given handle
     * @param uid the handle
     */
    public static void uidelete(final int uid) {
        final UIComponent comp = UILocator.get(uid);
        if (comp != null) {
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    comp.remove();
                }
            });
        }
    }

    /**
     * Remove all the UIComponents
     */
    public static void uideleteAll() {
        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                UILocator.removeAll();
            }
        });

        UIClassFinder.clearCache();
        UIMethodFinder.clearCache();
        UILocator.clearCache();
    }

    /**
     * Show the window containing the UIComponent with the given handle
     * @param uid the handle
     */
    public static void uishowWindow(final int uid) {
        final UIComponent comp = UILocator.get(uid);
        if (comp != null) {
            Object o = comp.getComponent();
            if (o instanceof Component) {
                final Frame frame = (Frame) SwingUtilities.getAncestorOfClass(Frame.class, (Component) o);
                if (frame != null) {
                    UIAccessTools.execOnEDT(new Runnable() {
                        public void run() {
                            frame.setVisible(true);
                            frame.setState(Frame.NORMAL);
                            frame.toFront();
                        }
                    });
                }
            }
        }
    }

    /**
     * Check if a handle is valid
     * @param uid the handle
     */
    public static boolean uiisValid(final int uid) {
        return UILocator.isValid(uid);
    }

    /**
     * Set a property on the given UIComponent
     * Arguments have been previously retrieved (in the gateway)
     * @param uid the handle
     */
    public static void uiset(final String path) throws Exception {
        final int uid = getUidFromPath(path);
        if (uid == -1) {
            throw new Exception("Invalid path");
        }

        uiset(uid);
    }

    /**
     * Set a property on the given UIComponent
     * Arguments have been previously retrieved (in the gateway)
     * @param uid the handle
     */
    public static void uiset(final int uid) throws Exception {
        final UIComponent comp = UILocator.get(uid);

        if (comp == null) {
            throw new Exception("Unknown uid");
        }

        final List<ScilabType> args = handler.getArgumentList();
        final int size = args.size();
        final List<String> proprName = new ArrayList<String>(size / 2);
        final List<ScilabType> values = new ArrayList<ScilabType>(size / 2);

        for (int i = 0; i < size; i += 2) {
            ScilabType arg = args.get(i);
            if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                throw new Exception(String.format("Invalid argument: A property name expected"));
            }
            proprName.add(((ScilabString) arg).getData()[0][0]);
            values.add(args.get(i + 1));
        }

        comp.setProperty(proprName, values);
    }

    /**
     * Get an UIComponent corresponding to arg
     * @param arg the argument
     */
    public static UIComponent getUIComponent(ScilabType arg) {
        if (arg.isEmpty()) {
            return null;
        }

        if (arg.getType() == ScilabTypeEnum.sci_strings) {
            String path = ((ScilabString) arg).getData()[0][0];
            return UILocator.get(path);
        }

        if (arg.getType() == ScilabTypeEnum.sci_handles) {
            ScilabHandle hdl = (ScilabHandle) arg;
            long id = hdl.getData()[0][0];
            return getUIComponent(id);
        }

        return null;
    }

    /**
     * Get an UIComponent corresponding to arg
     * @param arg the argument
     */
    public static UIComponent getUIComponent(long hdl) {
        if (hdl < 0) {
            return UILocator.get((int) (-hdl - 1));
        }

        return null;
    }

    /**
     * Create a new UIComponent (arguments have been already retrieved in the gateway)
     * @return an handle
     */
    public static int uiwidget() throws Exception {
        final List<ScilabType> args = handler.getArgumentList();
        final ScilabTypeMap attributes = new ScilabTypeMap();
        UIComponent parent = null;
        ScilabType arg;
        int start = 0;
        final int size = args.size();

        if (size == 0) {
            throw new Exception("Invalid number of arguments");
        }

        if (size % 2 == 1) {
            start = 1;
            // First argument is the parent id
            arg = args.get(0);
            parent = getUIComponent(arg);
            if (parent == null) {
                throw new Exception("Invalid parent object");
            }
        }

        for (int i = start; i < size; i += 2) {
            arg = args.get(i);
            if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                throw new Exception(String.format("Invalid %d-th argument: A property name expected", i));
            }
            String propr = ((ScilabString) arg).getData()[0][0];
            if (propr != null && !propr.isEmpty()) {
                if (propr.equalsIgnoreCase("id") || propr.equalsIgnoreCase("tag") || propr.equalsIgnoreCase("style") || propr.equalsIgnoreCase("constraint")) {
                    arg = args.get(i + 1);
                    if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                        throw new Exception(String.format("Invalid %d-th argument: A String expected", i + 1));
                    }
                    attributes.put(propr.toLowerCase(), arg);
                } else if (propr.equalsIgnoreCase("parent")) {
                    if (parent == null) {
                        parent = getUIComponent(args.get(i + 1));
                    }
                } else {
                    attributes.put(propr, args.get(i + 1));
                }
            }
        }

        if (!attributes.containsKey("style")) {
            throw new Exception("Cannot create an UIWidget without Style");
        }

        if (!attributes.containsKey("id")) {
            //throw new Exception("Cannot create an UIWidget without an id");
        }

        String[] style = ScilabTypeConverters.getObjectFromValue(String[].class, attributes.get("style"));
        //String id = ((ScilabString) attributes.get("id")).getData()[0][0];

        attributes.remove("style");
        //attributes.remove("id");

        final UIComponent ui;
        if (style.length == 1) {
            ui = UIComponent.getUIComponent("org.scilab.modules.uiwidget.components", style[0], attributes, parent, null);
        } else {
            ui = null;
            // TODO: gerer les modeles correctement en ajoutant le ScilabTypeMap au StringMap deja defini
            //ui = UIModele.get(style[0], style[1], parent, id, null);
        }

        // Layout informations are prioritar so we handle them now
        if (parent != null) {
            final UIComponent _parent = parent;
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    ui.finish();
                    try {
                        _parent.add(ui);
                    } catch (Exception e) {
                        System.err.println(e);
                    }
                    _parent.finish();
                }
            });
        } else {
            UIAccessTools.execOnEDT(new Runnable() {
                public void run() {
                    ui.finish();
                }
            });
        }

        return ui.getUid();
    }
}
