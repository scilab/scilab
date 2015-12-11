package org.scilab.modules.gui.bridge.groupmanager;

import java.util.Collection;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.Map;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.ButtonModel;

public class GroupManager {
    private Map<String, ButtonGroup> buttonGroup = new HashMap<String, ButtonGroup>();
    private static GroupManager me;

    private GroupManager() {
    }

    public static GroupManager getGroupManager() {
        if (me == null) {
            me = new GroupManager();
        }

        return me;
    }

    public void addToGroup(String groupName, AbstractButton obj) {

        //first is to remove obj from others group
        removeFromGroup(obj);

        ButtonGroup group = buttonGroup.get(groupName);
        if (group == null) {
            group = new ButtonGroup();
            buttonGroup.put(groupName, group);
        }

        group.add(obj);
    }

    public void removeFromGroup(AbstractButton obj) {
        Collection<ButtonGroup> groupList = buttonGroup.values();
        for (ButtonGroup group : groupList) {
            group.remove(obj);
        }
    }

    public Enumeration<AbstractButton> getGroupElements(String groupname) {
        ButtonGroup group = buttonGroup.get(groupname);
        return group.getElements();
    }

    public void setSelected(ButtonModel model, String groupname, boolean status) {
        ButtonGroup group = buttonGroup.get(groupname);
        group.setSelected(model, status);
    }
}
