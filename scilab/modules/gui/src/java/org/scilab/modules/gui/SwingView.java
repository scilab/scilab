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

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGERENDERER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.gui.utils.Debug.DEBUG;

import java.awt.Component;
import java.awt.Container;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import javax.swing.ImageIcon;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.view.View;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.imagerenderer.SwingScilabImageRenderer;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.widget.Widget;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class SwingView implements GraphicView {

    private static final String SCIDIR = System.getenv("SCI");

    private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_menubar.xml";
    private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_toolbar.xml";

    private static SwingView me;

    /**
     * Constructor
     */
    private SwingView() {
        GraphicController.getController().register(this);
        allObjects = Collections.synchronizedMap(new HashMap<String, TypedObject>());
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
        CheckBox,
        Figure,
        ImageRenderer,
        PushButton,
        RadioButton,
        Table,
        Text
    } 

    private class TypedObject {
        private UielementType   _type;
        private SwingViewObject _value;
        private Set<String> _children;

        public TypedObject(UielementType _type, SwingViewObject _value) {
            this._type = _type;
            this._value = _value;
            this._children = new HashSet<String>();
        }

        public UielementType getType() {
            return _type;
        }

        public SwingViewObject getValue() {
            return _value;
        }

        public Set<String> getChildren() {
            return _children;
        }

        public void addChild(String childUID) {
            _children.add(childUID);
        }

        public void removeChild(String childUID) {
            _children.remove(childUID);
        }

        public boolean hasChild(String childUID) {
            return _children.contains(childUID);
        }
    };

    private Map<String, TypedObject> allObjects;

    @Override
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
        if (style.equals(__GO_UI_CHECKBOX__)) {
            return UielementType.CheckBox;
        } else if (style.equals(__GO_FIGURE__)) {
            return UielementType.Figure;
        } else if (style.equals(__GO_UI_IMAGERENDERER__)) {
            return UielementType.ImageRenderer;
        } else if (style.equals(__GO_UI_PUSHBUTTON__)) {
            return UielementType.PushButton;
        } else if (style.equals(__GO_UI_RADIOBUTTON__)) {
            return UielementType.RadioButton;
        } else if (style.equals(__GO_UI_TABLE__)) {
            return UielementType.Table;
        } else if (style.equals(__GO_UI_TEXT__)) {
            return UielementType.Text;
        }
        return null;
    }

    private TypedObject CreateObjectFromType(String type, String id) {
        UielementType enumType = StyleToEnum(type);
        return new TypedObject(enumType, CreateObjectFromType(enumType, id));
    }

    private SwingViewObject CreateObjectFromType(UielementType type, String id) {
        switch (type) {
        case CheckBox:
            SwingScilabCheckBox checkBox = new SwingScilabCheckBox();
            checkBox.setId(id);
            setDefaultProperties(checkBox, id);
            return checkBox;
        case Figure:
            Figure figure = (Figure) GraphicController.getController().getObjectFromId(id);
            String figureTitle = figure.getName();
            Integer figureId = figure.getId();
            if ((figureTitle != null) && (figureId != null)) {
                figureTitle = figureTitle.replaceFirst("%d", figureId.toString());
            }

            SwingScilabWindow window = new SwingScilabWindow();

            window.setTitle(figureTitle);
            /* MENUBAR */
            MenuBar menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE, figureId);
            /* TOOLBAR */
            ToolBar toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE, figureId);
            /* INFOBAR */
            TextBox infoBar = ScilabTextBox.createTextBox();

            SwingScilabTab tab = new SwingScilabTab(figureTitle, figureId, figure);
            tab.setId(id);

            tab.setMenuBar(menuBar);
            tab.setToolBar(toolBar);
            tab.setInfoBar(infoBar);
            window.addMenuBar(menuBar);
            window.addToolBar(toolBar);
            window.addInfoBar(infoBar);

            tab.setWindowIcon(new ImageIcon(SCIDIR + "/modules/gui/images/icons/graphic-window.png").getImage());

            tab.setParentWindowId(window.getId());

            DockingManager.dock(tab, window.getDockingPort());
            ActiveDockableTracker.requestDockableActivation(tab);

            window.setVisible(true);
            tab.setVisible(true);
            tab.setName(figureTitle);

            String infoMessage = figure.getInfoMessage();
            if ((infoMessage == null) || (infoMessage.length() == 0)) {
                infoBar.setText("");
            } else {
                infoBar.setText(infoMessage);
            }
            tab.update(__GO_SIZE__, (Integer[]) GraphicController.getController().getProperty(id, __GO_SIZE__));
            tab.update(__GO_POSITION__, (Integer[]) GraphicController.getController().getProperty(id, __GO_POSITION__));
            tab.update(__GO_AXES_SIZE__, (Integer[]) GraphicController.getController().getProperty(id, __GO_AXES_SIZE__));
            // TODO set other default properties
            return tab;
        case ImageRenderer:
            SwingScilabImageRenderer imageRenderer = new SwingScilabImageRenderer();
            imageRenderer.setId(id);
            return imageRenderer;
        case PushButton:
            SwingScilabPushButton pushButton = new SwingScilabPushButton();
            pushButton.setId(id);
            setDefaultProperties(pushButton, id);
            return pushButton;
        case RadioButton:
            SwingScilabRadioButton radioButton = new SwingScilabRadioButton();
            radioButton.setId(id);
            setDefaultProperties(radioButton, id);
            return radioButton;
        case Table:
            SwingScilabUiTable table = new SwingScilabUiTable();
            table.setId(id);
            return table;
        case Text:
            SwingScilabLabel text = new SwingScilabLabel();
            text.setId(id);
            setDefaultProperties(text, id);
            return text;
        default:
            return null;
        }
    }
    
    /**
     * Initialize all poperties according to model
     * @param uicontrolObject the uicontrol
     * @param id the uicontrol id
     */
    private void setDefaultProperties(SwingViewObject uicontrolObject, String id) {
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_BACKGROUNDCOLOR__, 
                (Double[]) GraphicController.getController().getProperty(id, __GO_UI_BACKGROUNDCOLOR__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_ENABLE__, 
                (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_ENABLE__, 
                (Boolean) GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_FONTANGLE__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_FONTANGLE__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_FONTNAME__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_FONTNAME__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_FONTSIZE__, 
                (Double) GraphicController.getController().getProperty(id, __GO_UI_FONTSIZE__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_FONTWEIGHT__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_FONTWEIGHT__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_FOREGROUNDCOLOR__, 
                (Double[]) GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_HORIZONTALALIGNMENT__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_HORIZONTALALIGNMENT__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_RELIEF__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_RELIEF__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_STRING__, 
                (String[]) GraphicController.getController().getProperty(id, __GO_UI_STRING__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_UI_VERTICALALIGNMENT__, 
                (String) GraphicController.getController().getProperty(id, __GO_UI_VERTICALALIGNMENT__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_POSITION__, 
                (Double[]) GraphicController.getController().getProperty(id, __GO_POSITION__));
        SwingScilabWidget.update((Widget) uicontrolObject, __GO_VISIBLE__, 
                (Boolean) GraphicController.getController().getProperty(id, __GO_VISIBLE__));   
    }

    @Override
    public void deleteObject(String id) {
        TypedObject requestedObject = allObjects.get(id);
        if (requestedObject != null) {
            switch (requestedObject.getType()) {
            case Figure:
                SwingScilabTab tab = (SwingScilabTab) requestedObject.getValue();
                DockingManager.close(tab);
                DockingManager.unregisterDockable((Dockable) tab);
                tab.close();
                break;
            default:
                ((Widget) requestedObject.getValue()).destroy();
                break;
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        TypedObject registeredObject = allObjects.get(id);
        DEBUG("SwingView", "Update" + property);

        /* On uicontrol style is set after object creation */
        if (registeredObject == null && property.equals(__GO_STYLE__)) {
            String style = (String) GraphicController.getController().getProperty(id, __GO_STYLE__);
            allObjects.put(id, CreateObjectFromType(style, id));
        }

        /* On uicontrol style is set after object creation */
        if (registeredObject != null && property.equals(__GO_CHILDREN__)) {
            String[] newChildren = (String[]) GraphicController.getController().getProperty(id, __GO_CHILDREN__);
            String type = (String) GraphicController.getController().getProperty(id, __GO_TYPE__);

            if (type.equals(__GO_FIGURE__)) {
                TypedObject updatedObject = allObjects.get(id);
                Container updatedComponent = null;
                boolean needRevalidate = false;
                // Add new children
                for (String childId : newChildren) {
                    String childType = (String) GraphicController.getController().getProperty(childId, __GO_TYPE__);
                    if (childType.equals(__GO_UICONTROL__)) {
                        if (!updatedObject.hasChild(childId)) {
                            // Add the child
                            updatedObject.addChild(childId);
                            updatedComponent = (SwingScilabTab) updatedObject.getValue();
                            Component childComponent = (Component) allObjects.get(childId).getValue();
                            updatedComponent.add(childComponent);
                            needRevalidate = true;
                        }
                    }
                }
                // Remove children which have been deleted
                Set<String> newChildrenSet = new HashSet<String>(Arrays.asList(newChildren));
                for (String childId : updatedObject.getChildren()) {
                    if (!newChildrenSet.contains(childId)) {
                        // Add the child
                        updatedObject.removeChild(childId);
                        updatedComponent = (SwingScilabTab) updatedObject.getValue();
                        Component childComponent = (Component) allObjects.get(childId).getValue();
                        updatedComponent.remove(childComponent);
                        needRevalidate = true;
                    }
                }
                if (needRevalidate && updatedComponent != null) {
                    ((View) updatedComponent).revalidate();
                }
            }
        }

        if (registeredObject != null) {
            SwingViewObject swingObject = registeredObject.getValue();
            if (swingObject != null) {
                swingObject.update(property, GraphicController.getController().getProperty(id, property));
            } else {
                //System.err.println("[DEBUG] swingObject (" + id + ") is null can not update.");   
            }
        } else {
            //System.err.println("[DEBUG] registeredObject (" + id + ") is null.");   
        }
    }
}
