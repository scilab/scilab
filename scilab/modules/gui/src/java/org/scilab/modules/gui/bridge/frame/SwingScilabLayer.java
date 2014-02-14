package org.scilab.modules.gui.bridge.frame;



import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TAG__;

import java.awt.CardLayout;
import java.awt.Component;
import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.gui.SwingViewObject;

public class SwingScilabLayer extends SwingScilabFrame {
    private static final long serialVersionUID = 9038916804095320758L;
    private ArrayList<Integer> layers = new ArrayList<Integer>();

    public SwingScilabLayer() {
        super();
        setLayout(new CardLayout());
    }

    public void addMember(SwingScilabFrame member) {
        String name = member.getId().toString();
        member.setName(name);
        layers.add(member.getId());
        add(member, name);
        revalidate();
        //active the first child
        if (getActiveLayer() == -1) {
            setActiveLayer(1);
        }
    }

    public void addMember(SwingScilabScrollableFrame member) {
        String name = member.getId().toString();
        member.setName(name);
        layers.add(member.getId());
        add(member, name);
        revalidate();
        //active the first child
        if (getActiveLayer() == -1) {
            setActiveLayer(1);
        }
    }

    public Integer getActiveLayer() {
        Component card = null;
        for (Component comp : getComponents()) {
            if (comp.isVisible() == true) {

                card = comp;
                break;
            }
        }

        if (card == null) {
            return -1;
        }

        return layers.indexOf(Integer.parseInt(card.getName())) + 1;
    }

    private Component getLayerFromIndex(Integer layer) {
        if (layer < 1 || layer > getComponentCount()) {
            return null;
        }

        return getComponent(layer - 1);
    }

    private Integer getLayerIndex(Component layer) {
        for (int i = 0; i < getComponentCount(); i++) {
            if (getComponent(i) == layer) {
                return i + 1;
            }
        }

        return -1;
    }

    public Boolean isLayerActive(Integer layer) {
        return getActiveLayer() == layer;
    }


    public Boolean isLayerActive(Component layer) {
        Integer index = getLayerIndex(layer);
        if (index == -1) {
            return false;
        }

        return isLayerActive(index);
    }


    public void removeMember(SwingScilabScrollableFrame member) {
        layers.remove(member.getId());
        remove(member);
        revalidate();

        //update model with new index
        Integer active = getActiveLayer();
        if (active == -1) {
            if (setActiveLayer(1)) {
                active = 1;
            }
        }

        //update model with new index
        GraphicController.getController().setProperty(getId(),  __GO_UI_VALUE__, new Double[] {active.doubleValue()});
    }

    public void removeMember(SwingScilabFrame member) {
        layers.remove(member.getId());
        remove(member);
        revalidate();

        //update model with new index
        Integer active = getActiveLayer();
        if (active == -1) {
            if (setActiveLayer(1)) {
                active = 1;
            }
        }

        //update model with new index
        GraphicController.getController().setProperty(getId(),  __GO_UI_VALUE__, new Double[] {active.doubleValue()});
    }

    public boolean setActiveLayer(Integer layerPos) {
        if (layerPos < 1 || layerPos > layers.size()) {
            return false;
        }

        Integer previous = getActiveLayer();
        if (setActiveLayer(layers.get(layerPos - 1).toString()) == false) {
            return false;
        }

        Integer next = getActiveLayer();
        updateChildrenVisible(previous, next);
        return true;
    }

    private boolean setActiveLayer(String layer) {
        //check existing item
        ((CardLayout)getLayout()).show(this, layer);
        return true;
    }

    public boolean setActiveLayerFromName(String name) {

        //get layer's name from property "string"
        GraphicController controller = GraphicController.getController();

        for (int i = 0 ; i < layers.size() ; i++) {
            String string = (String)controller.getProperty(layers.get(i), __GO_TAG__);
            if (string.equals(name)) {
                return setActiveLayer(i + 1);
            }
        }

        return false;
    }
    private void updateChildrenVisible(Integer previous, Integer next) {
        SwingViewObject previousFrame = (SwingViewObject)getLayerFromIndex(previous);
        SwingViewObject nextFrame = (SwingViewObject) getLayerFromIndex(next);

        if (previousFrame != null) {
            GraphicController.getController().setProperty(previousFrame.getId(), __GO_VISIBLE__, false);
        }

        if (nextFrame != null) {
            GraphicController.getController().setProperty(nextFrame.getId(), __GO_VISIBLE__, true);
        }

        GraphicController.getController().setProperty(getId(),  __GO_UI_VALUE__, new Double[] {next.doubleValue()});

    }
}
