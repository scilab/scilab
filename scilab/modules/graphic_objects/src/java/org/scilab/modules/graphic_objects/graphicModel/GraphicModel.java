/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
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

package org.scilab.modules.graphic_objects.graphicModel;

import java.util.HashMap;
import java.util.Map;

import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.datatip.Datatip;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.lighting.Light;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.uibar.progressionbar.Progressionbar;
import org.scilab.modules.graphic_objects.uibar.waitbar.Waitbar;
import org.scilab.modules.graphic_objects.uicontextmenu.Uicontextmenu;
import org.scilab.modules.graphic_objects.uicontrol.checkbox.CheckBox;
import org.scilab.modules.graphic_objects.uicontrol.edit.Edit;
import org.scilab.modules.graphic_objects.uicontrol.edit.Spinner;
import org.scilab.modules.graphic_objects.uicontrol.frame.Frame;
import org.scilab.modules.graphic_objects.uicontrol.frame.border.FrameBorder;
import org.scilab.modules.graphic_objects.uicontrol.layer.Layer;
import org.scilab.modules.graphic_objects.uicontrol.listbox.ListBox;
import org.scilab.modules.graphic_objects.uicontrol.popupmenu.PopupMenu;
import org.scilab.modules.graphic_objects.uicontrol.pushbutton.PushButton;
import org.scilab.modules.graphic_objects.uicontrol.radiobutton.RadioButton;
import org.scilab.modules.graphic_objects.uicontrol.slider.Slider;
import org.scilab.modules.graphic_objects.uicontrol.tab.Tab;
import org.scilab.modules.graphic_objects.uicontrol.table.Table;
import org.scilab.modules.graphic_objects.uicontrol.uiimage.UiImage;
import org.scilab.modules.graphic_objects.uicontrol.uitext.UiText;
import org.scilab.modules.graphic_objects.uimenu.Uimenu;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

/**
 * GraphicModel class
 * @author Bruno JOFRET
 */
public final class GraphicModel {

    private static GraphicModel me;
    private static GraphicObject figureModel;
    private static GraphicObject axesModel;

    private Map<Integer, GraphicObject> allObjects = new HashMap<Integer, GraphicObject>();

    /**
     * Default constructor
     */
    private GraphicModel() { }

    /**
     * Returns the model
     * @return the model
     */
    public static GraphicModel getModel() {
        if (me == null) {
            me = new GraphicModel();
        }

        return me;
    }

    public static Figure getFigureModel() {
        return (Figure) figureModel;
    }

    public static Axes getAxesModel() {
        return (Axes) axesModel;
    }

    /**
     * @param id the id of the object to get
     * @return the object
     */
    public GraphicObject getObjectFromId(Integer id) {
        return allObjects.get(id);
    }

    /**
     * Returns a null property
     * @param id the id of the object
     * @param property the property name
     * @return the property
     */
    public Object getNullProperty(Integer id, String property) {
        GraphicObject object = allObjects.get(id);
        return object.getNullProperty(property);
    }

    /**
     * Fast property get
     * @param id object id
     * @param property property name
     * @return property value
     */
    public Object getProperty(Integer id, int property) {
        GraphicObject object = allObjects.get(id);

        if (object != null) {
            Object propertyType = object.getPropertyFromName(property);
            return object.getProperty(propertyType);
        }
        return null;
    }

    /**
     * Fast property set
     * @param id object id
     * @param property name
     * @param value property value
     * @return true if the property has been set, false otherwise
     */
    public UpdateStatus setProperty(Integer id, int property, Object value) {
        GraphicObject object = allObjects.get(id);
        if (object != null) {
            synchronized (object) {
                Object propertyType = object.getPropertyFromName(property);
                return object.setProperty(propertyType, value);
            }
        }
        return UpdateStatus.Fail;
    }

    /**
     * Creates an object
     * @param id object id
     * @param type object type
     * @return the created object's id
     */
    public Integer createObject(Integer id, GraphicObject.Type type) {
        GraphicObject object = createTypedObject(type);

        if (object != null) {
            allObjects.put(id, object);
            object.setIdentifier(id);

            return id;
        } else {
            return 0;
        }
    }

    /**
     * Clone object
     * @param id : id of the object to clone
     * @param newId : id of the clone
     * @return newId
     */
    public Integer cloneObject(Integer id, Integer newId) {
        GraphicObject object = allObjects.get(id);
        GraphicObject objectClone = object.clone();
        objectClone.setIdentifier(newId);

        allObjects.put(newId, objectClone);

        return newId;
    }

    /**
     * Creates a typed object
     * @param type the object type
     * @return the created typed object
     */
    private GraphicObject createTypedObject(Type type) {
        GraphicObject createdObject = null;
        switch (type) {
            case ARC:
                createdObject = new Arc();
                break;
            case AXES:
                createdObject = new Axes();
                break;
            case AXESMODEL:
                createdObject = new Axes();
                createdObject.setValid(false);
                axesModel = createdObject;
                break;
            case AXIS:
                createdObject = new Axis();
                break;
            case CHAMP:
                createdObject = new Champ();
                break;
            case COMPOUND:
                createdObject = new Compound();
                break;
            case FAC3D:
                createdObject = new Fac3d();
                break;
            case FEC:
                createdObject = new Fec();
                break;
            case FIGURE:
                createdObject = new Figure();
                break;
            case FIGUREMODEL:
                createdObject = new Figure();
                createdObject.setValid(false);
                figureModel = createdObject;
                break;
            case GRAYPLOT:
                createdObject = new Grayplot();
                break;
            case LABEL:
                createdObject = new Label();
                break;
            case LEGEND:
                createdObject = new Legend();
                break;
            case MATPLOT:
                createdObject = new Matplot();
                break;
            case PLOT3D:
                createdObject = new Plot3d();
                break;
            case POLYLINE:
                createdObject = new Polyline();
                break;
            case RECTANGLE:
                createdObject = new Rectangle();
                break;
            case SEGS:
                createdObject = new Segs();
                break;
            case TEXT:
                createdObject = new Text();
                break;
                /* UICONTROLS */
            case CHECKBOX:
                createdObject = new CheckBox();
                break;
            case EDIT:
                createdObject = new Edit();
                break;
            case SPINNER:
                createdObject = new Spinner();
                break;
            case FRAME:
                createdObject = new Frame();
                break;
            case IMAGE:
                createdObject = new UiImage();
                break;
            case LISTBOX:
                createdObject = new ListBox();
                break;
            case POPUPMENU:
                createdObject = new PopupMenu();
                break;
            case PUSHBUTTON:
                createdObject = new PushButton();
                break;
            case RADIOBUTTON:
                createdObject = new RadioButton();
                break;
            case SLIDER:
                createdObject = new Slider();
                break;
            case TABLE:
                createdObject = new Table();
                break;
            case UITEXT:
                createdObject = new UiText();
                break;
                /* UIMENU */
            case UIMENU:
                createdObject = new Uimenu();
                break;
            case UIMENUMODEL:
                createdObject = new Uimenu();
                createdObject.setValid(false);
                break;
                /* UICONTEXTMENU */
            case UICONTEXTMENU:
                createdObject = new Uicontextmenu();
                break;
                /* Create Scilab console object */
            case CONSOLE:
                createdObject = Console.getConsole();
                ((Console) createdObject).setScilabMode(Console.ScilabMode.NW);
                break;
            case JAVACONSOLE:
                createdObject = Console.getConsole();
                ((Console) createdObject).setScilabMode(Console.ScilabMode.STD);
                break;
                /* Uibar */
            case PROGRESSIONBAR:
                createdObject = new Progressionbar();
                break;
            case WAITBAR:
                createdObject = new Waitbar();
                break;
            case LIGHT:
                createdObject = new Light();
                break;
            case DATATIP:
                createdObject = new Datatip();
                break;
            case TAB:
                createdObject = new Tab();
                break;
            case LAYER:
                createdObject = new Layer();
                break;
            case BORDER:
                createdObject = new FrameBorder();
                break;
            case FRAME_SCROLLABLE:
                createdObject = new Frame();
                ((Frame)createdObject).setScrollable(true);
                break;
            default:
                createdObject = null;
        }
        return createdObject;
    }

    /**
     * Deletes an object
     * @param id object id
     */
    public void deleteObject(Integer id) {
        GraphicObject object = allObjects.get(id);
        synchronized (object) {
            allObjects.remove(id);
        }
    }

}
