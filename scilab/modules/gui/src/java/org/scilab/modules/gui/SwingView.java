/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGERENDERER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.gui.utils.Debug.DEBUG;

import java.util.HashMap;
import java.util.Map;

import org.flexdock.docking.DockingManager;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class SwingView implements GraphicView {

    private static SwingView me;

    /**
     * Constructor
     */
    private SwingView() {
        GraphicController.getController().register(this);
        allObjects = new HashMap<String, TypedObject>();
    }

    public static void registerSwingView() {
        DEBUG("SwingView", "calling registerSwingView()");
        if (me == null) {
            me = new SwingView();
        }
    }

    public static SwingViewObject getFromId(String id) {
        TypedObject typedObject = me.allObjects.get(id);
        
        if (typedObject == null) {
            return null;
        }
        
        return typedObject.getValue();
    }
    
    private enum UielementType {
        Figure,
        ImageRenderer,
        PushButton,
        Table
    } 

    private class TypedObject {
        private UielementType   _type;
        private SwingViewObject _value;

        public TypedObject(UielementType _type, SwingViewObject _value) {
            this._type = _type;
            this._value = _value;
        }

        public UielementType getType() {
            return _type;
        }

        public SwingViewObject getValue() {
            return _value;
        }
         
    }

    private Map<String, TypedObject> allObjects;

    public void createObject(String id) {

        String objectType = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);
        DEBUG("SwingWiew", "Object Created : " + id + "with type : " + objectType);
        if (objectType.equals(__GO_FIGURE__)) {
            allObjects.put(id, CreateObjectFromType(objectType, id));
            return;
        }

        if (objectType.equals(__GO_UICONTROL__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            DEBUG("SwingView", "__GO_STYLE__(" + style + ")");
            if (style != null) {
                allObjects.put(id, CreateObjectFromType(style, id));
            } else {
                allObjects.put(id, null);
            }
        }
    }

    private UielementType StyleToEnum(String style) {
        DEBUG("SwingView", "StyleToEnum(" + style + ")");
        if (style.equals(__GO_FIGURE__)) {
            return UielementType.Figure;
        }
        if (style.equals(__GO_UI_IMAGERENDERER__)) {
            return UielementType.ImageRenderer;
        }
        if (style.equals(__GO_UI_PUSHBUTTON__)) {
            return UielementType.PushButton;
        }
        if (style.equals(__GO_UI_TABLE__)) {
            return UielementType.Table;
        }
        return null;
    }

    private TypedObject CreateObjectFromType(String type, String id) {
        UielementType enumType = StyleToEnum(type);
        return new TypedObject(enumType, CreateObjectFromType(enumType, id));
    }

    private SwingViewObject CreateObjectFromType(UielementType type, String id) {
        switch (type) {
        case Figure:
            SwingScilabWindow win = new SwingScilabWindow();
            SwingScilabTab tab = new SwingScilabTab("");
            DockingManager.dock(tab, win.getDockingPort());
            return tab;
        case ImageRenderer:
        	SwingScilabImageRenderer imageRenderer = new SwingScilabImageRenderer();
        	imageRenderer.setId(id);
        	return imageRenderer;
        case PushButton:
            SwingScilabPushButton button = new SwingScilabPushButton();
            button.setId(id);
            Double[] defaultButtonBackground = {0.6, 0.6, 0.6};
            GraphicController.getController().setProperty(id, __GO_UI_BACKGROUNDCOLOR__, defaultButtonBackground);
            SwingScilabWidget.update(button, __GO_UI_ENABLE__, (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
            SwingScilabWidget.update(button, __GO_UI_FONTANGLE__, (String) GraphicController.getController().getProperty(id, __GO_UI_FONTANGLE__));
            SwingScilabWidget.update(button, __GO_UI_FONTNAME__, (String) GraphicController.getController().getProperty(id, __GO_UI_FONTNAME__));
            SwingScilabWidget.update(button, __GO_UI_FONTSIZE__, (Double) GraphicController.getController().getProperty(id, __GO_UI_FONTSIZE__));
            SwingScilabWidget.update(button, __GO_UI_FONTWEIGHT__, (String) GraphicController.getController().getProperty(id, __GO_UI_FONTWEIGHT__));
            SwingScilabWidget.update(button, __GO_UI_FOREGROUNDCOLOR__, (Double[]) GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
            SwingScilabWidget.update(button, __GO_UI_HORIZONTALALIGNMENT__, (String) GraphicController.getController().getProperty(id, __GO_UI_HORIZONTALALIGNMENT__));
            SwingScilabWidget.update(button, __GO_UI_RELIEF__, (String) GraphicController.getController().getProperty(id, __GO_UI_RELIEF__));
            SwingScilabWidget.update(button, __GO_UI_STRING__, (String[]) GraphicController.getController().getProperty(id, __GO_UI_STRING__));
            SwingScilabWidget.update(button, __GO_UI_VERTICALALIGNMENT__, (String) GraphicController.getController().getProperty(id, __GO_UI_VERTICALALIGNMENT__));
            SwingScilabWidget.update(button, __GO_POSITION__, (Double[]) GraphicController.getController().getProperty(id, __GO_POSITION__));
            SwingScilabWidget.update(button, __GO_VISIBLE__, (Boolean) GraphicController.getController().getProperty(id, __GO_VISIBLE__));
            return button;
        case Table:
        	SwingScilabUiTable table = new SwingScilabUiTable();
        	table.setId(id);
        	return table;
        default:
            return null;
        }
    }

    public void deleteObject(String id) {
        TypedObject requestedObject = allObjects.get(id);


    }

    public void updateObject(String id, String property) {
        TypedObject registeredObject = allObjects.get(id);
        
        /* On uicontrol style is set after object creation */
        if (registeredObject == null && property.equals(__GO_STYLE__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            allObjects.put(id, CreateObjectFromType(style, id));
        }

        if (registeredObject != null) {
            SwingViewObject swingObject = registeredObject.getValue();
            if (swingObject != null) {
                swingObject.update(property, GraphicController.getController().getProperty(id, property));
            } else {
                System.err.println("[DEBUG] swingObject (" + id + ") is null can not update.");   
            }
        } else {
            System.err.println("[DEBUG] registeredObject (" + id + ") is null.");   
        }
    }

}
