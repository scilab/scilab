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

package org.scilab.modules.gui.uiwidget;

import java.util.HashMap;
import java.util.Map;

import javax.swing.SwingUtilities;

import org.xml.sax.Attributes;

/**

   <modele namespace="foo">

   <UIFrame modele-name="Machin">
   ...
   </UIFrame>

   <UIButton modele-name="Truc">
   ...
   </UIButton>
   </modele>

**/

/**
 * Class to handle modele
 */
public class UIModele {

    private static final Map<String, UIModele> modeles = new HashMap<String, UIModele>();

    private Map<String, UIFakeComponent> m;

    /**
     * Default constructor
     */
    public UIModele() {
        m = new HashMap<String, UIFakeComponent>();
    }

    /**
     * Register an uri for a model name and the given fake UIComponent
     * @param uri the URI
     * @param modeleName the model name
     * @param component the fake UIComponent
     */
    public static void add(String uri, String modeleName, UIFakeComponent component) {
        UIModele modele = modeles.get(uri);
        if (modele == null) {
            modele = new UIModele();
            modeles.put(uri, modele);
        }

        modele.m.put(modeleName, component);
    }

    /**
     * Get a fake UIComponent
     * @param uri the URI
     * @param modeleName the modele name
     * @param attrs the XML attributes
     * @return the corresponding fake UIComponent
     */
    public static UIFakeComponent get(String uri, String modeleName, Attributes attrs) {
        UIModele modele = modeles.get(uri);
        if (modele == null) {
            return null;
        }

        UIFakeComponent ui = modele.m.get(modeleName);
        if (attrs != null && attrs.getLength() != 0) {
            UIFakeComponent uic = (UIFakeComponent) ui.clone();
            uic.addAttributes(attrs);

            return uic;
        }

        return ui;
    }

    /**
     * Get a real UIComponent (not fake)
     * @param uri the URI
     * @param modeleName the modele name
     * @param parent the parent UIComponent
     * @param style the CSS map style
     * @return the corresponding UIComponent
     */
    public static UIComponent get(String uri, String modeleName, final UIComponent parent, final Map<String, Map<String, String>> style) throws UIWidgetException {
        return get(uri, modeleName, parent, null, style);
    }

    /**
     * Get a real UIComponent (not fake)
     * @param uri the URI
     * @param modeleName the modele name
     * @param parent the parent UIComponent
     * @param attributes the XML attributes
     * @param style the CSS map style
     * @return the corresponding UIComponent
     */
    public static UIComponent get(String uri, String modeleName, final UIComponent parent, final StringMap attributes, final Map<String, Map<String, String>> style) throws UIWidgetException {
        UIModele modele = modeles.get(uri);
        if (modele == null) {
            return null;
        }

        final UIFakeComponent c = modele.m.get(modeleName);
        if (c == null) {
            return null;
        }

        final UIComponent[] ptr = new UIComponent[1];
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                public void run() {
                    try {
                        ptr[0] = c.getUIComponent(parent, attributes, style);
                    } catch (Exception e) {
                        System.err.println(e);
                        e.printStackTrace();
                    }
                }
            });
        } catch (Exception e) { }

        return ptr[0];
    }
}
