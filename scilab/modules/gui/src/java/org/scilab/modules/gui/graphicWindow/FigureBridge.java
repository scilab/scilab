/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.gui.graphicWindow;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.events.callback.ScilabCloseCallBack;
import org.scilab.modules.gui.menubar.MenuBar;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.toolbar.ToolBar;
import org.scilab.modules.gui.utils.MenuBarBuilder;
import org.scilab.modules.gui.utils.ToolBarBuilder;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import javax.media.opengl.GLCanvas;
import javax.swing.ImageIcon;
import javax.swing.JPanel;
import java.awt.Component;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;
import static org.scilab.modules.gui.utils.Debug.DEBUG;

/**
 * @author Pierre Lando
 */
public class FigureBridge implements GraphicView {

    private static final String SCIDIR = System.getenv("SCI");

    private static final String MENUBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_menubar.xml";
    private static final String TOOLBARXMLFILE = SCIDIR + "/modules/gui/etc/graphics_toolbar.xml";
    private static final String FIGURE_TITLE = "Graphic window number ";

    private final GLCanvas glCanvas = new GLCanvas();
    private final JPanel panel;
    private FigureInteraction figureInteraction;
    private final String id;

    private Window window = null;
    private MenuBar menuBar = null;
    private ToolBar toolBar = null;
    private Tab tab = null;
    private TextBox infoBar = null;

    private final Map<String, Component> uiElementsMap = new HashMap<String, Component>();

    /**
     * Default constructor.
     * @param id the id of the figure represented by this frame.
     * @throws Exception when the given id is not a figure id.
     */
    private FigureBridge(final String id) throws Exception {
        panel = new JPanel(new PanelLayout());
        panel.add(glCanvas, PanelLayout.GL_CANVAS);

        Object type = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if ("Figure".equals(type)) {
            // TODO : use static Figure.type instead of "Figure".
            if (id == null) {
                throw new Exception("null is not an id");
            }
            this.id = id;
            Object object = GraphicController.getController().getObjectFromId(id);

            if (object instanceof Figure) {
                final Figure figure = (Figure) object;

                Canvas canvas = JoGLCanvasFactory.createCanvas(glCanvas);

                canvas.setMainDrawer(new DrawerVisitor(canvas, figure));

                figureInteraction = new FigureInteraction(glCanvas, id);
                figureInteraction.setEnable(true);

                int figureIndex = figure.getId();
                
                window = ScilabWindow.createWindow();

                window.setTitle(FIGURE_TITLE + figureIndex);
                /* MENUBAR */
                menuBar = MenuBarBuilder.buildMenuBar(MENUBARXMLFILE, figureIndex);
                /* TOOLBAR */
                toolBar = ToolBarBuilder.buildToolBar(TOOLBARXMLFILE, figureIndex);
                /* INFOBAR */
                infoBar = ScilabTextBox.createTextBox();
                
                tab = ScilabTab.createTab(FIGURE_TITLE + figureIndex, figureIndex);
                String closingCommand =
                    "if (get_figure_handle(" + figureIndex + ") <> []) then"
                    +      "  if (get(get_figure_handle(" + figureIndex + "), 'event_handler_enable') == 'on') then"
                    +      "    execstr(get(get_figure_handle(" + figureIndex + "), 'event_handler')+'(" + figureIndex + ", -1, -1, -1000)', 'errcatch', 'm');"
                    +      "  end;"
                    +      "  delete(get_figure_handle(" + figureIndex + "));"
                    +      "end;";
                tab.setCallback(ScilabCloseCallBack.create(figureIndex, closingCommand));
                tab.addMenuBar(menuBar);
                tab.addToolBar(toolBar);
                tab.addInfoBar(infoBar);
                ((SwingScilabTab) tab.getAsSimpleTab()).setWindowIcon(new ImageIcon(System.getenv("SCI")
                                                                                           + "/modules/gui/images/icons/graphic-window.png").getImage());
               
                ((SwingScilabTab) tab.getAsSimpleTab()).setContentPane(panel);
                window.addTab(tab);
                GraphicController.getController().register(this);
                window.setVisible(true);
                tab.setVisible(true);
                panel.setVisible(true);
                updateGUI();
            }
        } else {
            throw new Exception("this id is not a figure");
        }
    }

    /**
     * This method update the figure name showed on the frame.
     * For compatibility, it replace the first occurrence of "%d" by the figure id.
     */
    private void updateGUI() {

        Object object = GraphicController.getController().getObjectFromId(id);
        if (object != null && (object instanceof Figure)) {
            Figure figure = (Figure) object;

            String name = figure.getName();
            Integer figureId = figure.getId();
            if ((name != null) && (figureId != null)) {
                name = name.replaceFirst("%d", figureId.toString());
                tab.setName(name);
            }

            String infoMessage = figure.getInfoMessage();
            if ((infoMessage == null) || (infoMessage.length()==0)) {
                infoBar.setText("");
            } else {
                infoBar.setText(infoMessage);
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        DEBUG("FigureBridge", "[UPDATE] "+id+ ((String) GraphicController.getController().getProperty(id, __GO_TYPE__))+" Property = "+property);       
        
        /*
         * Check if someone is not adding me a child
         */
        if ((this.id.equals(id)) && property.equals(__GO_CHILDREN__)) {

            String[] childrenId =  (String []) GraphicController.getController().getProperty(id,__GO_CHILDREN__);

            for (String childId : childrenId) {
                if (!uiElementsMap.containsKey(childId)) {
                    String childType = (String) GraphicController.getController().getProperty(childId,__GO_TYPE__);
                    if (childType.equals(__GO_UICONTROL__)) {
                        DEBUG("FigureBridge", "[!!!!!] I Have a new Uicontrol Child !!!");
                        //SwingScilabPushButton button = new SwingScilabPushButton();
                        //button.setText("Hello...");
                        //button.setVisible(true);
                        //button.setDims(new Size(200, 200));
                        //button.setPosition(new Position(0, 0));

                        //uiElements.put(childrenId[i], button);
                        //((SwingScilabTab) tab.getAsSimpleTab()).addMember(button);
                        //panel.remove(glCanvas);
                        Component component = (Component) SwingView.getFromId(childId);
                        panel.add(component);
                        uiElementsMap.put(childId, component);
                    }
                }
            }

            System.out.println("map : " + uiElementsMap.size() + " / " + childrenId.length);
            if (uiElementsMap.size() > childrenId.length) {
                Set<String> excessKey = new HashSet<String>(uiElementsMap.keySet());
                excessKey.removeAll(Arrays.asList(childrenId));
                for (String childId : excessKey) {
                    panel.remove(uiElementsMap.get(childId));
                }
                uiElementsMap.keySet().removeAll(excessKey);
            }

            panel.validate();
        }
        
        if(uiElementsMap.containsKey(id)) {
            updateGUI();
        }
    }

    @Override
    public void createObject(String id) {
        DEBUG("FigureBridge", "[CREATE] " + id + " "+((String) GraphicController.getController().getProperty(id, __GO_TYPE__)));
    }

    @Override
    public void deleteObject(String id) {
        DEBUG("FigureBridge", "[DELETE] " + id + " " + ((String) GraphicController.getController().getProperty(id, __GO_TYPE__)));
    }

    public static FigureBridge createFigure(String id) {
        try {
            return new FigureBridge(id);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
