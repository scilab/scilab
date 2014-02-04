package org.scilab.modules.gui.bridge.frame;



import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UI_VALUE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_VISIBLE__;

import java.awt.CardLayout;
import java.awt.Component;
import java.util.ArrayList;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;

public class SwingScilabLayer extends SwingScilabFrame {
    private static final long serialVersionUID = 9038916804095320758L;
    private ArrayList<String> layerName = new ArrayList<String>();
    private CardLayout layout = new CardLayout();

    public SwingScilabLayer() {
        super();
        setLayout(layout);
    }

    public void addMember(SwingScilabFrame member) {
        String name = member.getId().toString();
        member.setName(name);
        layerName.add(name);
        add(member, name);
        revalidate();
        //active the first child
        if (getActiveLayer() == -1) {
            setActiveLayer(1);
        }

    }

    public Integer getActiveLayer() {
        SwingScilabFrame card = null;
        for (Component comp : getComponents()) {
            if (comp.isVisible() == true) {
                card = (SwingScilabFrame) comp;
                break;
            }
        }

        if (card == null) {
            return -1;
        }

        return layerName.indexOf(card.getName()) + 1;
    }

    private SwingScilabFrame getLayerFromIndex(Integer layer) {
        if (layer < 1 || layer > getComponentCount()) {
            return null;
        }

        return (SwingScilabFrame) getComponent(layer - 1);
    }

    private Integer getLayerIndex(SwingScilabFrame layer) {
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


    public Boolean isLayerActive(SwingScilabFrame layer) {
        Integer index = getLayerIndex(layer);
        if (index == -1) {
            return false;
        }

        return isLayerActive(index);
    }


    public void removeMember(SwingScilabFrame member) {
        layerName.remove(member.getId().toString());
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
        if (layerPos < 1 || layerPos > layerName.size()) {
            return false;
        }

        Integer previous = getActiveLayer();
        if (setActiveLayer(layerName.get(layerPos - 1)) == false) {
            return false;
        }

        Integer next = getActiveLayer();
        updateChildrenVisible(previous, next);
        return true;
    }

    private boolean setActiveLayer(String layer) {
        //check existing item
        if (layerName.contains(layer)) {
            layout.show(this, layer);
            return true;
        } else {
            return false;
        }
    }

    private void updateChildrenVisible(Integer previous, Integer next) {
        SwingScilabFrame previousFrame = getLayerFromIndex(previous);
        SwingScilabFrame nextFrame = getLayerFromIndex(next);

        if (previousFrame != null) {
            GraphicController.getController().setProperty(previousFrame.getId(), __GO_VISIBLE__, false);
        }

        if (nextFrame != null) {
            GraphicController.getController().setProperty(nextFrame.getId(), __GO_VISIBLE__, true);
        }

        GraphicController.getController().setProperty(getId(),  __GO_UI_VALUE__, new Double[] {next.doubleValue()});

    }
}
