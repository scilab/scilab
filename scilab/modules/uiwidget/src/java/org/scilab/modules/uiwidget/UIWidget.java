/*
 * Uicontrol2 ( http://forge.scilab.org/index.php/p/uicontrol2/ ) - This file is a part of Uicontrol2
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

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.types.ScilabInteger;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabStackPutter;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.types.ScilabTypeEnum;

public class UIWidget {

    private static UIWidgetHandler handler;

    public static void uiwidgetLoad(String filename) throws Exception {
        final XMLToUIComponentConverter converter = new XMLToUIComponentConverter(filename);
        converter.convert();
    }

    public static int getUIWidgetHandler() {
        if (handler == null) {
            handler = new UIWidgetHandler();
        }

        return handler.getHandlerId();
    }

    public static String[][] uigetPropertyPairs(int uid) {
        final UIComponent comp = UILocator.get(uid);
        if (comp == null) {
            return new String[0][0];
        }

        return null;
    }

    public static int getUidFromPath(final String path) {
        UIComponent comp = UILocator.get(path);
        if (comp == null) {
            return -1;
        }

        return comp.getUid();
    }

    public static void uiget(int uid, String property, int stackPos) throws Exception {
        final UIComponent comp = UILocator.get(uid);

        if (comp == null) {
            throw new Exception("Invalid first argument: An uicontrol identifier expected");
        }

        Object o = comp.getProperty(property);
        ObjectToScilabConverters.putOnScilabStack(o, stackPos);
    }

    public static void uiset(final int uid) throws Exception {
        final List<ScilabType> args = handler.getArgumentList();
        final List<String> proprName = new ArrayList<String>();

        for (int i = 0; i < args.size(); i += 2) {
            ScilabType arg = args.get(i);
            if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                throw new Exception(String.format("Invalid argument: A property name expected"));
            }
            proprName.add(((ScilabString) arg).getData()[0][0]);
        }

        final UIComponent comp = UILocator.get(uid);

        if (comp == null) {
            throw new Exception("Unknown id");
        }

        UIComponent.execOnEDT(new Runnable() {
            public void run() {
                for (int i = 0; i < proprName.size(); i++) {
                    String name = proprName.get(i);
                    ScilabType value = args.get(2 * i + 1);
                    try {
                        comp.setProperty(name, value);
                    } catch (Exception e) {
                        System.err.println(e);
                        break;
                    }
                }
            }
        });
    }


    public static void uiwidget() throws Exception {
        final List<ScilabType> args = handler.getArgumentList();
        final List<String> proprName = new ArrayList<String>();
        final ScilabTypeMap attributes = new ScilabTypeMap();
        String parentId = null;
        ScilabType arg;
        int start = 0;

        if (args.size() == 0) {
            throw new Exception("Invalid number of arguments");
        }

        if (args.size() % 2 == 1) {
            start = 1;
            // First argument is the parent id
            arg = args.get(0);
            if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                throw new Exception("Invalid first argument: An uicontrol identifier expected");
            }
            parentId = ((ScilabString) arg).getData()[0][0];
        }

        for (int i = start; i < args.size(); i += 2) {
            arg = args.get(i);
            if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                throw new Exception(String.format("Invalid %d-th argument: A property name expected", i));
            }
            String propr = ((ScilabString) arg).getData()[0][0];
            if (propr != null && !propr.isEmpty()) {
                if (propr.equalsIgnoreCase("id") || propr.equalsIgnoreCase("style") || propr.equalsIgnoreCase("constraint")) {
                    arg = args.get(i + 1);
                    if (arg.getType() != ScilabTypeEnum.sci_strings || arg.isEmpty()) {
                        throw new Exception(String.format("Invalid %d-th argument: A String expected", i + 1));
                    }
                    attributes.put(propr.toLowerCase(), arg);
                } else {
                    attributes.put(propr, args.get(i + 1));
                }
            }
        }

        if (!attributes.containsKey("style")) {
            throw new Exception("Cannot create an UIWidget without Style");
        }

        if (!attributes.containsKey("id")) {
            throw new Exception("Cannot create an UIWidget without an id");
        }

        String[] style = ScilabTypeConverters.getObjectFromValue(String[].class, attributes.get("style"));
        String id = ((ScilabString) attributes.get("id")).getData()[0][0];

        attributes.remove("style");
        //attributes.remove("id");

        final UIComponent parent;
        if (parentId != null) {
            parent = UILocator.get(parentId);
            if (parent == null) {
                throw new Exception("Invalid parent object");
            }
        } else {
            parent = null;
        }

        final UIComponent ui;
        if (style.length == 1) {
            ui = UIComponent.getUIComponent("org.scilab.modules.uiwidget.components", style[0], attributes, parent, null);
        } else {
            ui = UIModele.get(style[0], style[1], parent, id, null);
        }

        /*if (attributes.containsKey("constraint")) {
            ui.setConstraint(((ScilabString) attributes.get("constraint")).getData()[0][0]);
            attributes.remove("constraint");
        }*/

        // Layout informations are prioritar so we handle them now
        if (parent != null) {
            UIComponent.execOnEDT(new Runnable() {
                public void run() {
                    try {
                        parent.add(ui);
                    } catch (Exception e) {
                        System.err.println(e);
                    }
                    parent.finish();
                }
            });
        }

        /*        final int s = start;
                UIComponent.execOnEDT(new Runnable() {
                        public void run() {
                            for (Map.Entry<String, ScilabType> entry : attributes.entrySet()) {
                                try {
                                    ui.setProperty(entry.getKey(), entry.getValue());
                                } catch (Exception e) {
                                    System.err.println(e);
                                    break;
                                }
                            }
                        }
                    });
        */
    }
}