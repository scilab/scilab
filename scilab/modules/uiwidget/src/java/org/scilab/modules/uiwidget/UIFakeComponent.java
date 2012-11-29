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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.xml.sax.Attributes;

public class UIFakeComponent implements Cloneable {

    private StringMap attributes;
    private UIFakeComponent parent;
    private List<UIFakeComponent> children;
    private String pack;
    private String name;

    public UIFakeComponent(String pack, String name, StringMap attributes) {
        this.pack = pack;
        this.name = name;
        this.attributes = attributes;
        this.children = new ArrayList<UIFakeComponent>();
    }

    public UIFakeComponent(String pack, String name, Attributes attributes) {
        this(pack, name, UIComponent.getMapFromAttributes(attributes));
    }

    public void addAttributes(Attributes attributes) {
        if (this.attributes == null) {
            this.attributes = UIComponent.getMapFromAttributes(attributes);
        } else {
            this.attributes.putAll(UIComponent.getMapFromAttributes(attributes));
        }
    }

    public void add(UIFakeComponent comp) {
        children.add(comp);
    }

    public UIComponent getUIComponent(final UIComponent parent, final StringMap attributes, final Map<String, Map<String, String>> style) throws UIWidgetException {
        final StringMap attrs;
        if (attributes != null) {
            attributes.putAll(this.attributes);
            attrs = attributes;
        } else {
            attrs = this.attributes;
        }

        final UIComponent ui = UIComponent.getUIComponent(pack, name, attrs, parent, style);
        UIAccessTools.execOnEDT(new Runnable() {
            public void run() {
                for (UIFakeComponent comp : children) {
                    try {
                        ui.add(comp.getUIComponent(ui, null, style));
                    } catch (UIWidgetException e) {
                        System.err.println(e);
                    }
                }
                ui.finish();
            }
        });

        return ui;
    }

    public Object clone() {
        UIFakeComponent ui = new UIFakeComponent(pack, name, (StringMap) null);
        if (attributes != null) {
            ui.attributes = (StringMap) attributes.clone();
        }

        ui.children = this.children;

        return ui;
    }
}