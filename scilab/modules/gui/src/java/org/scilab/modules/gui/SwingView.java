/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
package org.scilab.modules.gui;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_AXES__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACKTYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CALLBACK__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CONSOLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_FIGURE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_IMMEDIATE_DRAWING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_LAYOUT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PARENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_POSITION__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_PROGRESSIONBAR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_SIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_STYLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHECKEDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICHILDMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTEXTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UIPARENTMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_BACKGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_CHECKED__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_EDIT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SPINNER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ENABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTANGLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTNAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTSIZE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTUNITS__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FONTWEIGHT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FOREGROUNDCOLOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME_BORDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_FRAME__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_HORIZONTALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_ICON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_IMAGE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LABEL__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LAYER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_LISTBOX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MAX__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_MIN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_POPUPMENU__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_PUSHBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RADIOBUTTON__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_RELIEF__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SCROLLABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SEPARATOR__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_SLIDER__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_STRING__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TABLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TAB__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_TEXT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VERTICALALIGNMENT__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VALID__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_WAITBAR__;
import static org.scilab.modules.gui.utils.Debug.DEBUG;

import java.awt.Component;
import java.awt.Container;
import java.awt.GraphicsEnvironment;
import java.lang.reflect.InvocationTargetException;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import javax.media.opengl.GL;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLCanvas;
import javax.swing.JComponent;
import javax.swing.JSeparator;
import javax.swing.SwingUtilities;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.scilab.modules.graphic_export.Driver;
import org.scilab.modules.graphic_objects.DataLoader;
import org.scilab.modules.graphic_objects.console.Console;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.checkbox.SwingScilabCheckBox;
import org.scilab.modules.gui.bridge.checkboxmenuitem.SwingScilabCheckBoxMenuItem;
import org.scilab.modules.gui.bridge.console.SwingScilabConsole;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.bridge.editbox.SwingScilabEditBox;
import org.scilab.modules.gui.bridge.editbox.SwingScilabSpinner;
import org.scilab.modules.gui.bridge.frame.SwingScilabFrame;
import org.scilab.modules.gui.bridge.frame.SwingScilabLayer;
import org.scilab.modules.gui.bridge.frame.SwingScilabScrollableFrame;
import org.scilab.modules.gui.bridge.label.SwingScilabLabel;
import org.scilab.modules.gui.bridge.listbox.SwingScilabListBox;
import org.scilab.modules.gui.bridge.menu.SwingScilabMenu;
import org.scilab.modules.gui.bridge.menuitem.SwingScilabMenuItem;
import org.scilab.modules.gui.bridge.popupmenu.SwingScilabPopupMenu;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.bridge.radiobutton.SwingScilabRadioButton;
import org.scilab.modules.gui.bridge.slider.SwingScilabScroll;
import org.scilab.modules.gui.bridge.slider.SwingScilabSlider;
import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.bridge.tab.SwingScilabDockablePanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabPanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabStaticPanel;
import org.scilab.modules.gui.bridge.tab.SwingScilabTabGroup;
import org.scilab.modules.gui.bridge.uiimage.SwingScilabUiImage;
import org.scilab.modules.gui.bridge.uitable.SwingScilabUiTable;
import org.scilab.modules.gui.bridge.waitbar.SwingScilabWaitBar;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.ged.Inspector;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.menubar.ScilabMenuBar;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ScilabToolBar;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.BorderConvertor;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.gui.widget.Widget;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

/**
 * @author Bruno JOFRET
 * @author Vincent COUVERT
 */
public final class SwingView implements GraphicView {

    public static final String NULLUUID = new UUID(0L, 0L).toString();
    private static SwingView me;
    private static boolean headless;
    private Map<Integer, TypedObject> allObjects;

    static {
        try {
            System.loadLibrary("gluegen2-rt");
        } catch (Exception e) {
            System.err.println(e);
        }
    }

    /**
     * Constructor
     */
    private SwingView() {
        GraphicController.getController().register(this);
        allObjects = Collections.synchronizedMap(new HashMap<Integer, TypedObject>());

        try {
            GLCanvas tmpCanvas = new GLCanvas(new GLCapabilities(GLProfile.getDefault()));
            tmpCanvas.getContext().makeCurrent();
            GL gl = tmpCanvas.getGL();
            DataLoader.setABGRExt(gl.isExtensionAvailable("GL_EXT_abgr") ? 1 : 0);
            tmpCanvas.getContext().release();
            tmpCanvas = null;

            /*
             * Forces font loading from the main thread. This is done because if
             * getSciFontManager (thus, font loading) is concurrently accessed
             * from 2 different threads (the AWT's and the main one), freezing
             * may occur.
             */
            FontManager.getSciFontManager();
        } catch (Exception e) {
        }
    }

    public static void registerSwingView() {
        DEBUG("SwingView", "calling registerSwingView()");
        if (me == null) {
            me = new SwingView();
        }
    }

    public static void setHeadless(boolean headless) {
        SwingView.headless = headless;
    }

    public static boolean isHeadless() {
        return SwingView.headless;
    }

    public static SwingViewObject getFromId(Integer id) {
        TypedObject typedObject = me.allObjects.get(id);

        if (typedObject == null) {
            return null;
        }

        return typedObject.getValue();
    }

    public static int getSize() {
        return me.allObjects.size();
    }

    private enum UielementType {
        Console, CheckBox, Edit, Spinner, Frame, Figure, Axes, Image, ListBox, PopupMenu, Progressbar, PushButton, RadioButton, Slider, Table, Text, Uimenu, UiParentMenu, UiChildMenu, UiCheckedMenu, UiContextMenu, Waitbar, Tab, Layer
    }

    private class TypedObject {
        private UielementType _type;
        private SwingViewObject _value;
        private Set<Integer> _children;

        public TypedObject(UielementType _type, SwingViewObject _value) {
            this._type = _type;
            this._value = _value;
            this._children = Collections.synchronizedSet(new HashSet<Integer>());
        }

        public UielementType getType() {
            return _type;
        }

        public SwingViewObject getValue() {
            return _value;
        }

        public Set<Integer> getChildren() {
            return _children;
        }

        public void addChild(Integer childUID) {
            _children.add(childUID);
        }

        public void removeChild(Integer childUID) {
            _children.remove(childUID);
        }

        public boolean hasChild(Integer childUID) {
            return _children.contains(childUID);
        }
    };

    private static final Set<Integer> managedTypes = new HashSet<Integer>(Arrays.asList(
                GraphicObjectProperties.__GO_FIGURE__,
                GraphicObjectProperties.__GO_AXES__,
                GraphicObjectProperties.__GO_UICONTEXTMENU__,
                GraphicObjectProperties.__GO_UIMENU__,
                GraphicObjectProperties.__GO_CONSOLE__,
                GraphicObjectProperties.__GO_PROGRESSIONBAR__,
                GraphicObjectProperties.__GO_WAITBAR__,
                GraphicObjectProperties.__GO_UICONTROL__
            ));

    @Override
    public void createObject(Integer id) {

        int objectType = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);

        if (managedTypes.contains(objectType) == false) {
            return;
        }
        //System.err.println("[SwingWiew] Object Created : " + id + " with type : " + objectType);
        boolean isValid = (Boolean) GraphicController.getController().getProperty(id, __GO_VALID__);
        if (!isValid) {
            return;
        }

        if (!headless && !GraphicsEnvironment.isHeadless()) {
            DEBUG("SwingWiew", "Object Created : " + id + "with type : " + objectType);
            if (objectType != __GO_UICONTROL__) {
                allObjects.put(id, CreateObjectFromType(objectType, id));
                return;
            }

            if (objectType == __GO_UICONTROL__) {
                int style = (Integer) GraphicController.getController().getProperty(id, __GO_STYLE__);
                DEBUG("SwingView", "__GO_STYLE__(" + style + ")");
                allObjects.put(id, CreateObjectFromType(style, id));
            }
        } else {
            if (objectType == __GO_FIGURE__) {
                Driver.setDefaultVisitor(id);
            }
        }
    }

    private UielementType StyleToEnum(int style) {
        DEBUG("SwingView", "StyleToEnum(" + style + ")");
        switch (style) {
            case __GO_FIGURE__:
                return UielementType.Figure;
            case __GO_AXES__ :
                return UielementType.Axes;
            case __GO_CONSOLE__:
                return UielementType.Console;
            case __GO_UI_CHECKBOX__:
                return UielementType.CheckBox;
            case __GO_UI_EDIT__:
                return UielementType.Edit;
            case __GO_UI_SPINNER__:
                return UielementType.Spinner;
            case __GO_UI_FRAME__:
                return UielementType.Frame;
            case __GO_UI_IMAGE__:
                return UielementType.Image;
            case __GO_UI_LISTBOX__:
                return UielementType.ListBox;
            case __GO_UI_POPUPMENU__:
                return UielementType.PopupMenu;
            case __GO_UI_PUSHBUTTON__:
                return UielementType.PushButton;
            case __GO_UI_RADIOBUTTON__:
                return UielementType.RadioButton;
            case __GO_UI_SLIDER__:
                return UielementType.Slider;
            case __GO_UI_TABLE__:
                return UielementType.Table;
            case __GO_UI_TEXT__:
                return UielementType.Text;
            case __GO_UIMENU__:
                return UielementType.UiChildMenu;
            case __GO_UIPARENTMENU__:
                return UielementType.UiParentMenu;
            case __GO_UICHILDMENU__:
                return UielementType.UiChildMenu;
            case __GO_UICHECKEDMENU__:
                return UielementType.UiCheckedMenu;
            case __GO_PROGRESSIONBAR__:
                return UielementType.Progressbar;
            case __GO_WAITBAR__:
                return UielementType.Waitbar;
            case __GO_UICONTEXTMENU__:
                return UielementType.UiContextMenu;
            case __GO_UI_TAB__:
                return UielementType.Tab;
            case __GO_UI_LAYER__:
                return UielementType.Layer;
        }
        return null;
    }

    private TypedObject CreateObjectFromType(final int type, final Integer id) {
        final UielementType enumType = StyleToEnum(type);
        final SwingViewObject newSVObject[] = new SwingViewObject[1];
        if (SwingUtilities.isEventDispatchThread()) {
            newSVObject[0] = CreateObjectFromType(enumType, id);
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {

                    @Override
                    public void run() {
                        newSVObject[0] = CreateObjectFromType(enumType, id);
                    }
                });
            } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
        return new TypedObject(enumType, newSVObject[0]);
    }

    private SwingViewObject CreateObjectFromType(UielementType type, Integer id) {
        switch (type) {
            case CheckBox:
                SwingScilabCheckBox checkBox = new SwingScilabCheckBox();
                checkBox.setId(id);
                setDefaultProperties(checkBox, id);
                return checkBox;
            case Console:
                Console console = (Console) GraphicController.getController().getObjectFromId(id);
                if (console.getScilabMode() == Console.ScilabMode.STD) {
                    WindowsConfigurationManager.restoreUUID(NULLUUID);
                    SwingScilabConsole sciConsole = ((SwingScilabConsole) ScilabConsole.getConsole().getAsSimpleConsole());
                    SwingScilabDockablePanel consoleTab = (SwingScilabDockablePanel) sciConsole.getParent();
                    consoleTab.setId(id);
                    return consoleTab;
                }
                return null;
            case Edit: {
                SwingScilabEditBox edit = new SwingScilabEditBox();
                edit.setId(id);
                setDefaultProperties(edit, id);
                return edit;
            }
            case Spinner : {
                SwingScilabSpinner spin = new SwingScilabSpinner();
                spin.setId(id);
                setDefaultProperties(spin, id);
                return spin;
            }
            case Figure: {
                Figure figure = (Figure) GraphicController.getController().getObjectFromId(id);
                String figureTitle = figure.getName();
                Integer figureId = figure.getId();
                if ((figureTitle != null) && (figureId != null)) {
                    figureTitle = figureTitle.replaceFirst("%d", figureId.toString());
                }

                SwingScilabWindow window = SwingScilabWindow.createWindow(figure.getDockable());

                window.setTitle(figureTitle);
                Integer[] figureSize = figure.getSize();
                window.setDims(new Size(figureSize[0], figureSize[1]));

                SwingScilabPanel tab;
                if (figure.getDockable()) {
                    tab = new SwingScilabDockablePanel(figureTitle, figureId, figure);
                } else {
                    tab = new SwingScilabStaticPanel(figureTitle, figureId, figure);
                }
                tab.setId(id);

                /* MENUBAR */
                MenuBar menuBar = ScilabMenuBar.createMenuBar();
                menuBar.setVisible(false);
                tab.setMenuBar(menuBar);
                window.addMenuBar(menuBar);

                /* TOOLBAR */
                ToolBar toolbar = ScilabToolBar.createToolBar();
                toolbar.setVisible(false);
                tab.setToolBar(toolbar);
                window.addToolBar(toolbar);

                /* INFOBAR */
                TextBox infoBar = ScilabTextBox.createTextBox();
                infoBar.setVisible(false);
                tab.setInfoBar(infoBar);
                window.addInfoBar(infoBar);

                if (figure.getDockable()) {
                    DockingManager.dock((SwingScilabDockablePanel) tab, window.getDockingPort());
                    ActiveDockableTracker.requestDockableActivation((SwingScilabDockablePanel) tab);
                } else {
                    window.addTab(tab);
                }

                tab.setWindowIcon("graphic-window");

                tab.setParentWindowId(window.getId());

                tab.setEventHandler(figure.getEventHandlerString());
                tab.setEventHandlerEnabled(figure.getEventHandlerEnable());
                tab.setVisible(true);
                tab.setName(figureTitle);

                String infoMessage = figure.getInfoMessage();
                if ((infoMessage == null) || (infoMessage.length() == 0)) {
                    infoBar.setText("");
                } else {
                    infoBar.setText(infoMessage);
                }

                tab.update(__GO_POSITION__, GraphicController.getController().getProperty(id, __GO_POSITION__));

                String icon = (String)GraphicController.getController().getProperty(id, __GO_UI_ICON__);
                if (icon != null && icon.equals("") == false) {
                    tab.update(__GO_UI_ICON__, icon);
                }

                // TODO set other default properties
                window.pack();
                window.setVisible(figure.getVisible());
                return tab;
            }
            case Axes:
                SwingScilabAxes axes = new SwingScilabAxes();
                return axes;
            case Frame: {
                Boolean scrollable = (Boolean) GraphicController.getController().getProperty(id, __GO_UI_SCROLLABLE__);
                if (scrollable) {
                    SwingScilabScrollableFrame frame = new SwingScilabScrollableFrame();
                    frame.setId(id);
                    setDefaultProperties(frame, id);
                    Integer borderId = (Integer) GraphicController.getController().getProperty(id, __GO_UI_FRAME_BORDER__);
                    if (borderId != 0) {
                        frame.setBorder(BorderConvertor.getBorder(borderId));
                    }
                    return frame;
                } else {
                    SwingScilabFrame frame = new SwingScilabFrame();
                    frame.setId(id);
                    setDefaultProperties(frame, id);
                    Integer borderId = (Integer) GraphicController.getController().getProperty(id, __GO_UI_FRAME_BORDER__);
                    if (borderId != 0) {
                        frame.setBorder(BorderConvertor.getBorder(borderId));
                    }
                    return frame;
                }
            }
            case Image:
                SwingScilabUiImage image = new SwingScilabUiImage();
                image.setId(id);
                setDefaultProperties(image, id);
                return image;
            case ListBox:
                SwingScilabListBox listBox = new SwingScilabListBox();
                listBox.setId(id);
                setDefaultProperties(listBox, id);
                return listBox;
            case PopupMenu:
                SwingScilabPopupMenu popupMenu = new SwingScilabPopupMenu();
                popupMenu.setId(id);
                setDefaultProperties(popupMenu, id);
                return popupMenu;
            case Progressbar:
                SwingScilabWaitBar progressbar = new SwingScilabWaitBar();
                progressbar.setIndeterminateMode(true);
                progressbar.setId(id);
                return progressbar;
            case PushButton: {
                SwingScilabPushButton pushButton = new SwingScilabPushButton();
                pushButton.setId(id);
                setDefaultProperties(pushButton, id);
                String icon = (String)GraphicController.getController().getProperty(id, __GO_UI_ICON__);
                if (icon != null && icon.equals("") == false) {
                    pushButton.update(__GO_UI_ICON__, icon);
                }

                return pushButton;
            }
            case RadioButton:
                SwingScilabRadioButton radioButton = new SwingScilabRadioButton();
                radioButton.setId(id);
                setDefaultProperties(radioButton, id);
                return radioButton;
            case Slider:
                if (Console.getConsole().getUseDeprecatedLF()) {
                    SwingScilabScroll slider = new SwingScilabScroll();
                    slider.setId(id);
                    setDefaultProperties(slider, id);
                    return slider;
                } else {
                    SwingScilabSlider slider = new SwingScilabSlider();
                    slider.setId(id);
                    setDefaultProperties(slider, id);
                    return slider;
                }
            case Table:
                SwingScilabUiTable table = new SwingScilabUiTable();
                table.setId(id);
                setDefaultProperties(table, id);
                return table;
            case Text: {
                SwingScilabLabel text = new SwingScilabLabel();
                text.setId(id);
                setDefaultProperties(text, id);
                String icon = (String)GraphicController.getController().getProperty(id, __GO_UI_ICON__);
                if (icon != null && icon.equals("") == false) {
                    text.update(__GO_UI_ICON__, icon);
                }
                return text;
            }
            case Uimenu:
                throw new UnsupportedOperationException();
            case UiParentMenu: /* SwingView internal type */
                SwingScilabMenu parentMenu = new SwingScilabMenu();
                parentMenu.setId(id);
                setMenuDefaultProperties(parentMenu, id);
                return parentMenu;
            case UiChildMenu: /* SwingView internal type */
                SwingScilabMenuItem childMenu = new SwingScilabMenuItem();
                childMenu.setId(id);
                setMenuDefaultProperties(childMenu, id);
                return childMenu;
            case UiCheckedMenu: /* SwingView internal type */
                SwingScilabCheckBoxMenuItem checkedMenu = new SwingScilabCheckBoxMenuItem(false);
                checkedMenu.setId(id);
                setMenuDefaultProperties(checkedMenu, id);
                return checkedMenu;
            case UiContextMenu:
                SwingScilabContextMenu contextMenu = new SwingScilabContextMenu();
                contextMenu.setId(id);
                return contextMenu;
            case Waitbar:
                SwingScilabWaitBar waitbar = new SwingScilabWaitBar();
                waitbar.setIndeterminateMode(false);
                waitbar.setId(id);
                return waitbar;
            case Tab:
                SwingScilabTabGroup TabGroup = new SwingScilabTabGroup();
                TabGroup.setId(id);
                //setDefaultProperties(TabGroup, id);
                //TabGroup.update(property, value);
                return TabGroup;
            case Layer:
                SwingScilabLayer layer = new SwingScilabLayer();
                layer.setId(id);
                return layer;
            default:
                System.err.println("[SwingView] Unmanaged Creation type !!!");
                return null;
        }
    }

    /**
     * Initialize all poperties according to model
     * @param uiMenuObject the uimenu
     * @param id the uimenu id
     */
    private void setMenuDefaultProperties(Widget uiMenuObject, Integer id) {
        SwingViewMenu.update(uiMenuObject, __GO_CHILDREN__, GraphicController.getController().getProperty(id, __GO_CHILDREN__));
        SwingViewMenu.update(uiMenuObject, __GO_CALLBACK__, GraphicController.getController().getProperty(id, __GO_CALLBACK__));
        SwingViewMenu.update(uiMenuObject, __GO_CALLBACKTYPE__, GraphicController.getController().getProperty(id, __GO_CALLBACKTYPE__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_CHECKED__, GraphicController.getController().getProperty(id, __GO_UI_CHECKED__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_ENABLE__, GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_FOREGROUNDCOLOR__, GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_LABEL__, GraphicController.getController().getProperty(id, __GO_UI_LABEL__));
        SwingViewMenu.update(uiMenuObject, __GO_UI_ICON__, GraphicController.getController().getProperty(id, __GO_UI_ICON__));
    }

    /**
     * Initialize all poperties according to model
     * @param uiControlObject the uicontrol
     * @param id the uicontrol id
     */
    private void setDefaultProperties(SwingViewObject uiControlObject, Integer id) {

        /*
         * Visible property is set first to avoid to see the object rendered
         * before all its properties to be set (See bug #10346)
         */
        uiControlObject.update(__GO_VISIBLE__, GraphicController.getController().getProperty(id, __GO_VISIBLE__));
        uiControlObject.update(__GO_UI_BACKGROUNDCOLOR__, GraphicController.getController().getProperty(id, __GO_UI_BACKGROUNDCOLOR__));
        uiControlObject.update(__GO_UI_ENABLE__, GraphicController.getController().getProperty(id, __GO_UI_ENABLE__));
        uiControlObject.update(__GO_UI_FONTANGLE__, GraphicController.getController().getProperty(id, __GO_UI_FONTANGLE__));
        uiControlObject.update(__GO_UI_FONTNAME__, GraphicController.getController().getProperty(id, __GO_UI_FONTNAME__));
        uiControlObject.update(__GO_UI_FONTUNITS__, GraphicController.getController().getProperty(id, __GO_UI_FONTUNITS__));
        uiControlObject.update(__GO_UI_FONTSIZE__, GraphicController.getController().getProperty(id, __GO_UI_FONTSIZE__));
        uiControlObject.update(__GO_UI_FONTWEIGHT__, GraphicController.getController().getProperty(id, __GO_UI_FONTWEIGHT__));
        uiControlObject.update(__GO_UI_FOREGROUNDCOLOR__, GraphicController.getController().getProperty(id, __GO_UI_FOREGROUNDCOLOR__));
        uiControlObject.update(__GO_UI_HORIZONTALALIGNMENT__, GraphicController.getController().getProperty(id, __GO_UI_HORIZONTALALIGNMENT__));
        uiControlObject.update(__GO_UI_RELIEF__, GraphicController.getController().getProperty(id, __GO_UI_RELIEF__));
        uiControlObject.update(__GO_UI_STRING__, GraphicController.getController().getProperty(id, __GO_UI_STRING__));
        uiControlObject.update(__GO_UI_VERTICALALIGNMENT__, GraphicController.getController().getProperty(id, __GO_UI_VERTICALALIGNMENT__));
        uiControlObject.update(__GO_POSITION__, GraphicController.getController().getProperty(id, __GO_POSITION__));
        uiControlObject.update(__GO_LAYOUT__, GraphicController.getController().getProperty(id, __GO_LAYOUT__));
        uiControlObject.update(__GO_UI_MIN__, GraphicController.getController().getProperty(id, __GO_UI_MIN__));
        uiControlObject.update(__GO_UI_MAX__, GraphicController.getController().getProperty(id, __GO_UI_MAX__));
    }

    public void deleteObject(Integer id) {
        final TypedObject requestedObject = allObjects.get(id);
        if (requestedObject != null) {
            switch (requestedObject.getType()) {
                case Figure:
                    if (SwingUtilities.isEventDispatchThread()) {
                        final SwingScilabPanel tab = (SwingScilabPanel) requestedObject.getValue();
                        if (requestedObject.getValue() instanceof SwingScilabDockablePanel) {
                            final SwingScilabDockablePanel dockableTab = (SwingScilabDockablePanel) tab;
                            dockableTab.disablePaint();
                            DockingManager.close(dockableTab);
                            DockingManager.unregisterDockable((Dockable) dockableTab);
                            ClosingOperationsManager.unregisterClosingOperation(dockableTab);
                            ClosingOperationsManager.removeDependency(dockableTab);
                            ClosingOperationsManager.checkTabForClosing(dockableTab);
                            tab.close();
                        } else {
                            tab.close();
                        }
                    } else {
                        try {
                            SwingUtilities.invokeAndWait(new Runnable() {

                                @Override
                                public void run() {
                                    final SwingScilabPanel tab = (SwingScilabPanel) requestedObject.getValue();
                                    if (tab instanceof SwingScilabDockablePanel) {
                                        final SwingScilabDockablePanel dockableTab = (SwingScilabDockablePanel) tab;
                                        dockableTab.disablePaint();
                                        DockingManager.close(dockableTab);
                                        DockingManager.unregisterDockable((Dockable) dockableTab);
                                        ClosingOperationsManager.unregisterClosingOperation(dockableTab);
                                        ClosingOperationsManager.removeDependency(dockableTab);
                                        ClosingOperationsManager.checkTabForClosing(dockableTab);
                                        tab.close();
                                    } else {
                                        tab.close();
                                    }
                                }

                            });
                        } catch (InterruptedException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        } catch (InvocationTargetException e) {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                    }

                    //try close the Property List (GED)
                    try {
                        Inspector.getInspector().close();
                    } catch (NullPointerException e) {
                    }
                    break;
                case Progressbar:
                case Waitbar:
                    SwingScilabWaitBar bar = (SwingScilabWaitBar) requestedObject.getValue();
                    bar.close();
                    break;
                case Frame:
                    Boolean scrollable = (Boolean) GraphicController.getController().getProperty(id, __GO_UI_SCROLLABLE__);
                    if (scrollable) {
                        SwingScilabScrollableFrame frame = (SwingScilabScrollableFrame) requestedObject.getValue();
                        frame.destroy();
                    } else {
                        SwingScilabFrame frame = (SwingScilabFrame) requestedObject.getValue();
                        frame.destroy();
                    }
                    break;
                default:
                    // Nothing to do
                    // uicontrol case: the object is destroyed when its parent updates its children
                    break;
            }
            allObjects.remove(id);
        }
    }

    @Override
    public void updateObject(final Integer id, final int property) {
        final TypedObject registeredObject = allObjects.get(id);

        if (property == __GO_SIZE__ && (headless || GraphicsEnvironment.isHeadless())) {
            int objectType = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
            if (objectType == __GO_FIGURE__) {
                Integer[] dims = (Integer[]) GraphicController.getController().getProperty(id, __GO_SIZE__);
                GraphicController.getController().setProperty(id, __GO_AXES_SIZE__, dims);
            }
        }

        if (registeredObject != null && property == __GO_VALID__ && ((Boolean) GraphicController.getController().getProperty(id, __GO_VALID__))) {
            if (registeredObject.getValue() instanceof SwingScilabDockablePanel) {
                final Runnable r = new Runnable() {
                    @Override
                    public void run() {
                        boolean figVis = (Boolean) GraphicController.getController().getProperty(id, __GO_VISIBLE__);
                        ((SwingScilabDockablePanel) registeredObject.getValue()).getParentWindow().setVisible(figVis);
                        ((SwingScilabDockablePanel) registeredObject.getValue()).setVisible(true);
                        Integer[] figureSize = (Integer[]) GraphicController.getController().getProperty(id, __GO_SIZE__);
                        ((SwingScilabDockablePanel) registeredObject.getValue()).getParentWindow().setDims(new Size(figureSize[0], figureSize[1]));
                    }
                };

                if (SwingUtilities.isEventDispatchThread()) {
                    r.run();
                } else {
                    SwingUtilities.invokeLater(r);
                }
            }
        }

        if (property == __GO_IMMEDIATE_DRAWING__) {
            return;
        }

        if (registeredObject == null && property != __GO_STYLE__) {
            return;
        }

        /* On uicontrol style is set after object creation */
        if (registeredObject == null && property == __GO_STYLE__) {
            int style = (Integer) GraphicController.getController().getProperty(id, __GO_STYLE__);
            allObjects.put(id, CreateObjectFromType(style, id));
            return;
        }

        /* Do not update axes on EDT to avoid dead locks between SwingView & DrawerVisitor */
        /* Test which freezes without this condition: bug_1257.tst in graphics */
        int type = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
        if (type == __GO_AXES__) {
            if (property == __GO_PARENT__) {
                Integer parentId = (Integer) GraphicController.getController().getProperty(id, __GO_PARENT__);
                TypedObject registeredParent = allObjects.get(parentId);
                if (registeredParent == null) {
                    allObjects.remove(id);
                } else {
                    ((SwingScilabAxes) allObjects.get(id).getValue()).update(property, parentId);
                }
            }
            return;
        }

        if (SwingUtilities.isEventDispatchThread()) {
            updateObjectOnEDT(registeredObject, id, property);
        } else {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        updateObjectOnEDT(registeredObject, id, property);
                    }
                });
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }
    }

    public void updateObjectOnEDT(TypedObject registeredObject, final Integer id, final int property) {
        /* Removes the swing object if its parent is not display */
        if (registeredObject != null && property == __GO_PARENT__) {
            Integer parentId = (Integer) GraphicController.getController().getProperty(id, __GO_PARENT__);
            TypedObject registeredParent = allObjects.get(parentId);
            if (registeredParent == null) {
                allObjects.remove(id);
            }
        }

        int type = (Integer) GraphicController.getController().getProperty(id, __GO_TYPE__);
        /* Children list update */
        if (registeredObject != null && property == __GO_CHILDREN__) {
            final Integer[] newChildren = (Integer[]) GraphicController.getController().getProperty(id, __GO_CHILDREN__);

            switch (type) {
                    /*
                     * FIGURE CHILDREN UPDATE
                     */
                case __GO_FIGURE__:
                    updateFigureChildren(registeredObject, newChildren);
                    break;
                    /*
                     * CONSOLE CHILDREN UPDATE
                     */
                case __GO_CONSOLE__:
                    updateConsoleChildren(registeredObject, newChildren);
                    break;
                    /*
                     * MENU CHILDREN UPDATE
                     */
                case __GO_UIMENU__:
                    updateMenuChildren(registeredObject, id, newChildren);
                    break;
                    /*
                     * CONTEXTMENU CHILDREN UPDATE
                     */
                case __GO_UICONTEXTMENU__:
                    updateContextMenuChildren(registeredObject, newChildren);
                    break;
                    /*
                     * UICONTROL "FRAME" CHILDREN UPDATE
                     */
                case __GO_UICONTROL__:

                    int style = (Integer) GraphicController.getController().getProperty(id, __GO_STYLE__);
                    switch (style) {
                        case __GO_UI_LAYER__:
                            updateLayerChildren(registeredObject, newChildren);
                            break;
                        case __GO_UI_FRAME__:
                            if (registeredObject.getValue() instanceof SwingScilabFrame) {
                                updateFrameChildren(registeredObject, newChildren);
                            } else if (registeredObject.getValue() instanceof SwingScilabScrollableFrame) {
                                updateScrollableFrameChildren(registeredObject, newChildren);
                            }
                            break;
                        case __GO_UI_TAB__:
                            updateTabGroupChildren(registeredObject, newChildren);
                            break;
                    }
                    break;
            }
        }

        /*
         * When the CHECKED property is updated for a UIMENU, the object is
         * converted to a SwingScilabCheckBoxMenuItem is not already of this
         * type
         */
        if (registeredObject != null && property == __GO_UI_CHECKED__) {
            if (type == __GO_UIMENU__) {
                switch (registeredObject.getType()) {
                    case UiParentMenu: {
                        SwingScilabMenu meAsAMenu = (SwingScilabMenu) registeredObject.getValue();
                        Container parent = meAsAMenu.getParent();
                        if (parent != null) {
                            parent.remove(meAsAMenu);
                        }

                        registeredObject = CreateObjectFromType(__GO_UICHECKEDMENU__, id);
                        allObjects.put(id, registeredObject);
                        SwingScilabCheckBoxMenuItem meAsAMenuItem = (SwingScilabCheckBoxMenuItem) registeredObject.getValue();
                        if (parent != null) {
                            parent.add(meAsAMenuItem);
                        }
                        break;
                    }
                    case UiChildMenu: {
                        SwingScilabMenuItem childMenu = (SwingScilabMenuItem) allObjects.get(id).getValue();
                        JComponent parentMenu = (JComponent) childMenu.getParent();
                        int index = 0;
                        if (parentMenu != null) {
                            index = parentMenu.getComponentZOrder(childMenu);
                            parentMenu.remove(childMenu);
                        }
                        registeredObject = CreateObjectFromType(__GO_UICHECKEDMENU__, id);
                        allObjects.put(id, registeredObject);
                        registeredObject = allObjects.get(id);
                        if (parentMenu != null) {
                            parentMenu.add((SwingScilabCheckBoxMenuItem) registeredObject.getValue(), index);
                        }
                        return;
                    }
                    default:
                        break;
                }
            }
        }

        /*
         * When the SEPARATOR property is updated for a UIMENU, When the
         * property is set to TRUE: A separator is added if not already done
         * When the property is set to FALSE: The previous separator is removed
         * if it exists
         */
        if (registeredObject != null && property == __GO_UI_SEPARATOR__) {
            if (type == __GO_UIMENU__) {
                Integer parentId = (Integer) GraphicController.getController().getProperty(id, __GO_PARENT__);
                int menuPosition = -1;
                Component currentComponent = (Component) registeredObject.getValue();
                Component[] allChildren = ((SwingScilabMenu) allObjects.get(parentId).getValue()).getMenuComponents();
                for (int kChild = 0; kChild < allChildren.length; kChild++) {
                    if (allChildren[kChild] == currentComponent) {
                        menuPosition = kChild;
                        break;
                    }
                }
                if (menuPosition < 0) {
                    return;
                }
                boolean newSeparatorMode = (Boolean) GraphicController.getController().getProperty(id, __GO_UI_SEPARATOR__);
                if (newSeparatorMode) { // Add a separator above the menu
                    ((SwingScilabMenu) allObjects.get(parentId).getValue()).insertSeparator(menuPosition);
                } else { // Remove the separator above the menu (if there is one)
                    if (menuPosition == 0) { // There is no object above the menu
                        return;
                    }
                    if (((SwingScilabMenu) allObjects.get(parentId).getValue()).getComponent(menuPosition - 1) instanceof JSeparator) {
                        ((SwingScilabMenu) allObjects.get(parentId).getValue()).remove(menuPosition - 1);
                    }
                }
            }
        }

        if (registeredObject != null) {
            final SwingViewObject swingObject = registeredObject.getValue();
            if (swingObject != null) {
                swingObject.update(property, GraphicController.getController().getProperty(id, property));
            }
        }
    }

    /**
     * Update the figure children (called by generic updateObject method)
     * @param id the id of the figure
     * @param newChildren the new children IDs list
     */
    private void updateFigureChildren(TypedObject updatedObject, Integer[] newChildren) {
        Container updatedComponent = (Container) updatedObject.getValue();
        boolean needRevalidate = false;
        boolean needRevalidateMenubar = false;

        // Add new children
        for (Integer childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Add an Axes */
                if (childType == __GO_AXES__) {
                    SwingViewObject canvas = allObjects.get(childId).getValue();
                    ((SwingScilabPanel) updatedComponent).addMember(canvas);
                }

                /* Add an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    SwingViewObject uiContol = allObjects.get(childId).getValue();
                    ((SwingScilabPanel) updatedComponent).addMember(uiContol);

                    // Force Position
                    uiContol.update(__GO_POSITION__, GraphicController.getController().getProperty(childId, __GO_POSITION__));
                    needRevalidate = true;
                }

                /* Add an uimenu */
                if (childType == __GO_UIMENU__) {
                    TypedObject childAsTypedObject = allObjects.get(childId);
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                        case UiCheckedMenu:
                            allObjects.remove(childId);
                            allObjects.put(childId, CreateObjectFromType(__GO_UIPARENTMENU__, childId));
                            ((Container) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            ((Container) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidateMenubar = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        Integer[] oldChildrenSet = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (Integer childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                final Object childTypeObject = GraphicController.getController().getProperty(childId, __GO_TYPE__);
                if (childTypeObject == null) {
                    // the child has already been removed from the model
                    continue;
                }
                int childType = (Integer) childTypeObject;

                /* Remove an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    ((SwingScilabPanel) updatedComponent).removeMember(allObjects.get(childId).getValue());
                    needRevalidate = true;
                }

                /* Remove an uimenu */
                if (childType == __GO_UIMENU__) {
                    TypedObject childAsTypedObject = allObjects.get(childId);
                    switch (childAsTypedObject.getType()) {
                        case UiCheckedMenu:
                            ((Container) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).remove((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            ((Container) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).remove((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidateMenubar = true;
                }
            }
        }

        if (needRevalidateMenubar) {
            ((JComponent) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).revalidate();
            ((JComponent) ((SwingScilabPanel) updatedComponent).getMenuBar().getAsSimpleMenuBar()).repaint();
        }

        if (needRevalidate && updatedComponent != null) {
            ((JComponent) updatedComponent).revalidate();
        }
    }

    /**
     * Update the frame children (called by generic updateObject method)
     * @param id the id of the figure
     * @param newChildren the new children IDs list
     */
    private void updateFrameChildren(TypedObject updatedObject, Integer[] newChildren) {
        SwingScilabFrame updatedComponent = (SwingScilabFrame) updatedObject.getValue();
        boolean needRevalidate = false;
        boolean hasOpenGLAxes = false;
        int oldComponentCount = updatedComponent.getComponentCount();

        // Add new children
        for (Integer childId : newChildren) {
            int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                /* Add an uicontrol */
                if (childType == __GO_UICONTROL__ || childType == __GO_AXES__) {
                    updatedComponent.addMember(allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }

            if (childType == __GO_AXES__) {
                hasOpenGLAxes = true;
            }

        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        Integer[] oldChildrenSet = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (Integer childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Remove an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    updatedComponent.remove((Component) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }

        if (!hasOpenGLAxes) {
            updatedComponent.removeAxes();
            needRevalidate = true;
        }

        if (needRevalidate && updatedComponent != null) {
            updatedComponent.revalidate();
        }

        // Force repaint if we removed components
        if (oldComponentCount > updatedComponent.getComponentCount()) {
            updatedComponent.repaint();
        }
    }

    private void updateScrollableFrameChildren(TypedObject updatedObject, Integer[] newChildren) {
        SwingScilabScrollableFrame updatedComponent = (SwingScilabScrollableFrame) updatedObject.getValue();
        boolean needRevalidate = false;
        int oldComponentCount = updatedComponent.getPanel().getComponentCount();

        // Add new children
        for (Integer childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Add an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    updatedComponent.addMember(allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        Integer[] oldChildrenSet = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (Integer childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Remove an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    updatedComponent.getPanel().remove((Component) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.getPanel().revalidate();
        }

        // Force repaint if we removed components
        if (oldComponentCount > updatedComponent.getPanel().getComponentCount()) {
            updatedComponent.repaint();
        }

    }

    /**
     * Update the frame children (called by generic updateObject method)
     * @param id the id of the figure
     * @param newChildren the new children IDs list
     */
    private void updateLayerChildren(TypedObject updatedObject, Integer[] newChildren) {
        SwingScilabLayer updatedComponent = (SwingScilabLayer) updatedObject.getValue();

        // Add new children
        for (Integer childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Add an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    int childStyle = (Integer) GraphicController.getController().getProperty(childId, __GO_STYLE__);
                    if (childStyle == __GO_UI_FRAME__) {
                        SwingViewObject obj = allObjects.get(childId).getValue();
                        if (obj instanceof SwingScilabScrollableFrame) {
                            updatedComponent.addMember((SwingScilabScrollableFrame)obj);
                        } else {
                            updatedComponent.addMember((SwingScilabFrame)obj);
                        }
                    }
                }
            }
        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        Integer[] oldChildrenSet = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (Integer childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Remove an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    int childStyle = (Integer) GraphicController.getController().getProperty(childId, __GO_STYLE__);
                    if (childStyle == __GO_UI_FRAME__) {
                        SwingViewObject obj = allObjects.get(childId).getValue();
                        if (obj instanceof SwingScilabScrollableFrame) {
                            updatedComponent.removeMember((SwingScilabScrollableFrame)obj);
                        } else {
                            updatedComponent.removeMember((SwingScilabFrame)obj);
                        }
                    }
                }
            }
        }
    }

    /**
     * Update the frame children (called by generic updateObject method)
     * @param id the id of the figure
     * @param newChildren the new children IDs list
     */
    private void updateTabGroupChildren(TypedObject updatedObject, Integer[] newChildren) {
        SwingScilabTabGroup updatedComponent = (SwingScilabTabGroup) updatedObject.getValue();
        boolean needRevalidate = false;

        // Add new children
        for (Integer childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Add an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    String str = "Tab";
                    String[] text = (String[]) GraphicController.getController().getProperty(childId, __GO_UI_STRING__);
                    if (text != null && text.length != 0 && text[0] != null) {
                        str = text[0];
                    }

                    updatedComponent.insertTab(str, null, (Component) allObjects.get(childId).getValue(), "", 0);
                    needRevalidate = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        // Clone the children set to avoid concurrent accesses
        Integer[] oldChildrenSet = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (Integer childId : oldChildrenSet) {
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                /* Remove an uicontrol */
                if (childType == __GO_UICONTROL__) {
                    updatedComponent.remove((Component) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.revalidate();
        }
    }

    /**
     * Update the Console menus (called by generic updateObject method)
     * @param id the id of the console object
     * @param newChildren the new children IDs list
     */
    private void updateConsoleChildren(TypedObject updatedObject, Integer[] newChildren) {
        Container updatedComponent = (SwingScilabDockablePanel) updatedObject.getValue();
        boolean needRevalidate = false;

        // Add new children
        for (Integer childId : newChildren) {
            if (!updatedObject.hasChild(childId)) {

                // Add the child
                updatedObject.addChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                if (childType == __GO_UIMENU__) {
                    TypedObject childAsTypedObject = allObjects.get(childId);
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                        case UiCheckedMenu:
                            TypedObject newUiParentMenu = CreateObjectFromType(__GO_UIPARENTMENU__, childId);
                            allObjects.put(childId, newUiParentMenu);
                            ((Container) ((SwingScilabDockablePanel) updatedObject.getValue()).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) newUiParentMenu.getValue());
                            break;
                        default: /* UiParentMenu */
                            ((Container) ((SwingScilabDockablePanel) updatedObject.getValue()).getMenuBar().getAsSimpleMenuBar()).add((SwingScilabMenu) childAsTypedObject.getValue());
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }

        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        Object[] updatedObjectChildren = updatedObject.getChildren().toArray();
        for (int i = 0; i < updatedObjectChildren.length; ++i) {
            Integer childId = (Integer) updatedObjectChildren[i];
            if (!newChildrenSet.contains(childId)) {

                // Remove the child
                updatedObject.removeChild(childId);

                int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);

                if (childType == __GO_UIMENU__) {
                    ((Container) ((SwingScilabDockablePanel) updatedObject.getValue()).getMenuBar().getAsSimpleMenuBar()).remove((SwingScilabMenu) allObjects.get(childId).getValue());
                    needRevalidate = true;
                }
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }

    }

    /**
     * Update the children of a menu menus (called by generic updateObject
     * method)
     * @param id the id of the menu object
     * @param newChildren the new children IDs list
     */
    private void updateMenuChildren(TypedObject updatedObject, Integer id, Integer[] newChildren) {
        Container updatedComponent = null;
        boolean needRevalidate = false;
        int updatedObjectPosition = 0;
        TypedObject newParent = null;

        // Add new children
        for (Integer childId : newChildren) {
            int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);
            if (childType == __GO_UIMENU__) {
                if (!updatedObject.hasChild(childId)) {
                    // Add the child
                    updatedObject.addChild(childId);

                    TypedObject childAsTypedObject = allObjects.get(childId);
                    Object addedChild = allObjects.get(childId).getValue();
                    JComponent parent = null;
                    switch (updatedObject.getType()) {
                        case UiChildMenu:
                            updatedComponent = (SwingScilabMenuItem) updatedObject.getValue();
                            parent = (JComponent) updatedComponent.getParent();
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    /* Replace the item by a parent menu */
                                    if (parent != null) {
                                        updatedObjectPosition = parent.getComponentZOrder((SwingScilabMenuItem) updatedObject.getValue());
                                        parent.remove((SwingScilabMenuItem) updatedObject.getValue());
                                    }
                                    newParent = CreateObjectFromType(__GO_UIPARENTMENU__, id);
                                    allObjects.put(id, newParent);
                                    newParent.addChild(childId);
                                    if (parent != null) {
                                        parent.add((SwingScilabMenu) newParent.getValue(), updatedObjectPosition);
                                    }
                                    /* Update the created menu */
                                    ((SwingScilabMenu) newParent.getValue()).add((SwingScilabMenuItem) addedChild);
                                    break;
                                case UiCheckedMenu:
                                    /* Replace the item by a parent menu */
                                    if (parent != null) {
                                        updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) updatedObject.getValue());
                                        parent.remove((SwingScilabCheckBoxMenuItem) updatedObject.getValue());
                                    }
                                    newParent = CreateObjectFromType(__GO_UIPARENTMENU__, id);
                                    allObjects.put(id, newParent);
                                    newParent.addChild(childId);
                                    if (parent != null) {
                                        parent.add((SwingScilabMenu) newParent.getValue(), updatedObjectPosition);
                                    }
                                    /* Update the created menu */
                                    ((SwingScilabMenu) newParent.getValue()).add((SwingScilabCheckBoxMenuItem) addedChild);
                                    break;
                                default: /* UiParentMenu */
                                    ((SwingScilabMenu) updatedObject.getValue()).add((SwingScilabMenu) addedChild);
                                    break;
                            }
                            break;
                        case UiCheckedMenu:
                            updatedComponent = (SwingScilabCheckBoxMenuItem) updatedObject.getValue();
                            parent = (JComponent) updatedComponent.getParent();
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    /* Replace the item by a parent menu */
                                    if (parent != null) {
                                        updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) updatedObject.getValue());
                                        parent.remove((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    }
                                    newParent = CreateObjectFromType(__GO_UIPARENTMENU__, id);
                                    allObjects.put(id, newParent);
                                    newParent.addChild(childId);
                                    if (parent != null) {
                                        parent.add((SwingScilabMenu) newParent.getValue(), updatedObjectPosition);
                                    }
                                    /* Update the created menu */
                                    ((SwingScilabMenu) newParent.getValue()).add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                                    break;
                                case UiCheckedMenu:
                                    /* Replace the item by a parent menu */
                                    if (parent != null) {
                                        updatedObjectPosition = parent.getComponentZOrder((SwingScilabCheckBoxMenuItem) updatedObject.getValue());
                                        parent.remove((SwingScilabCheckBoxMenuItem) allObjects.get(id).getValue());
                                    }
                                    newParent = CreateObjectFromType(__GO_UIPARENTMENU__, id);
                                    allObjects.put(id, newParent);
                                    newParent.addChild(childId);
                                    if (parent != null) {
                                        parent.add((SwingScilabMenu) newParent.getValue(), updatedObjectPosition);
                                    }
                                    /* Update the created menu */
                                    ((SwingScilabMenu) newParent.getValue()).add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                                    break;
                                default: /* UiParentMenu */
                                    ((SwingScilabMenu) updatedObject.getValue()).add((SwingScilabMenu) allObjects.get(childId).getValue());
                                    break;
                            }
                            break;
                        default: /* UiParentMenu */
                            SwingScilabMenu updatedMenu = (SwingScilabMenu) updatedObject.getValue();
                            updatedComponent = updatedMenu;
                            switch (childAsTypedObject.getType()) {
                                case UiChildMenu:
                                    updatedMenu.add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                                    break;
                                case UiCheckedMenu:
                                    updatedMenu.add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                                    break;
                                default: /* UiParentMenu */
                                    /* Java can not add a JMenu in a JMenu */
                                    /*
                                     * We need to convert the child into a
                                     * MenuItem
                                     */
                                    TypedObject newMenu = CreateObjectFromType(__GO_UIMENU__, childId);
                                    allObjects.put(childId, newMenu);
                                    updatedMenu.add((SwingScilabMenuItem) newMenu.getValue());
                                    break;
                            }
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }
        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        Integer[] oldChildren = updatedObject.getChildren().toArray(new Integer[updatedObject.getChildren().size()]);
        for (int childIndex = 0; childIndex < oldChildren.length; childIndex++) {
            Integer childId = oldChildren[childIndex];
            if (!newChildrenSet.contains(childId)) {
                // Remove the child
                updatedObject.removeChild(childId);
                updatedComponent = (SwingScilabMenu) updatedObject.getValue();

                TypedObject childAsTypedObject = allObjects.get(childId);
                Object removedMenu = childAsTypedObject.getValue();

                switch (childAsTypedObject.getType()) {
                    case UiChildMenu:
                        updatedComponent.remove((SwingScilabMenuItem) removedMenu);
                        break;
                    case UiCheckedMenu:
                        updatedComponent.remove((SwingScilabCheckBoxMenuItem) removedMenu);
                        break;
                    default:
                        updatedComponent.remove((SwingScilabMenu) removedMenu);
                        break;
                }
                needRevalidate = true;
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }
    }

    /**
     * Update the children of a contextmenu (called by generic updateObject
     * method)
     * @param id the id of the contextmenu object
     * @param newChildren the new children IDs list
     */
    private void updateContextMenuChildren(TypedObject updatedObject, Integer[] newChildren) {
        Container updatedComponent = null;
        boolean needRevalidate = false;

        // Add new children
        for (Integer childId : newChildren) {
            int childType = (Integer) GraphicController.getController().getProperty(childId, __GO_TYPE__);
            if (childType == __GO_UIMENU__) {
                if (!updatedObject.hasChild(childId)) {

                    // Add the child
                    updatedObject.addChild(childId);

                    TypedObject childAsTypedObject = allObjects.get(childId);
                    SwingScilabContextMenu updatedMenu = (SwingScilabContextMenu) updatedObject.getValue();
                    updatedComponent = updatedMenu;
                    switch (childAsTypedObject.getType()) {
                        case UiChildMenu:
                            updatedMenu.add((SwingScilabMenuItem) allObjects.get(childId).getValue());
                            break;
                        case UiCheckedMenu:
                            updatedMenu.add((SwingScilabCheckBoxMenuItem) allObjects.get(childId).getValue());
                            break;
                        default: /* UiParentMenu */
                            updatedMenu.add((SwingScilabMenu) allObjects.get(childId).getValue());
                            break;
                    }
                    needRevalidate = true;
                }
            }
        }
        // Remove children which have been deleted
        Set<Integer> newChildrenSet = new HashSet<Integer>(Arrays.asList(newChildren));
        for (Integer childId : updatedObject.getChildren()) {
            if (!newChildrenSet.contains(childId)) {
                // Remove the child
                updatedObject.removeChild(childId);
                updatedComponent = (SwingScilabMenu) updatedObject.getValue();

                TypedObject childAsTypedObject = allObjects.get(childId);
                SwingScilabContextMenu updatedMenu = (SwingScilabContextMenu) updatedObject.getValue();
                Object removedMenu = allObjects.get(childId).getValue();

                switch (childAsTypedObject.getType()) {
                    case UiChildMenu:
                        updatedMenu.remove((SwingScilabMenuItem) removedMenu);
                        break;
                    case UiCheckedMenu:
                        updatedMenu.remove((SwingScilabCheckBoxMenuItem) removedMenu);
                        break;
                    default:
                        updatedMenu.remove((SwingScilabMenu) removedMenu);
                        break;
                }
                needRevalidate = true;
            }
        }
        if (needRevalidate && updatedComponent != null) {
            updatedComponent.validate();
        }
    }
}
