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
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.xml.sax.Attributes;

public class UIFakeComponent {

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

    public void setId(String id) {
        if (attributes != null) {
            attributes.put("id", id);
        }
    }

    public void add(UIFakeComponent comp) {
        children.add(comp);
    }

    public UIComponent getUIComponent(final UIComponent parent, final Map<String, Map<String, String>> style) throws UIWidgetException {
        final UIComponent ui = UIComponent.getUIComponent(pack, name, attributes, parent, style);

        UIComponent.execOnEDT(new Runnable() {
            public void run() {
                for (UIFakeComponent comp : children) {
                    try {
                        ui.add(comp.getUIComponent(ui, style));
                    } catch (UIWidgetException e) {
                        System.err.println(e);
                    }
                }
                ui.finish();
            }
        });

        return ui;
    }
}