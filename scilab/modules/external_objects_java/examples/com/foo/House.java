package com.foo;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class House {

    private Color			color;
    private Door			door;
    private List<IWindow>	windows	= new LinkedList<IWindow>();

    public House(Color color, Door door, IWindow... windows) {
        this.color = color;
        this.door = door;
        this.windows.addAll(Arrays.asList(windows));
    }

    public void repaint() {
        swapDoorHouseColor();
    }

    private void swapDoorHouseColor() {
        Color doorColor = door.getColor();
        door.repaint(color);
        color = doorColor;
    }

    public void addWindow(IWindow window) {
        windows.add(window);
    }

    public void replaceDoor(Door newDoor) {
        this.door = newDoor;
    }

    @Override
    public String toString() {
        return String.format(
                   "This is a house painted in %s, has a %s door, and %d windows",
                   color.getDescr(), door.getColor().getDescr(), windows.size());
    }
}