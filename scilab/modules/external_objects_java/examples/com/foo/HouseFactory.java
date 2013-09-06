package com.foo;

public class HouseFactory {

    public static House basicHouse() {
        Door door = new Door(Color.WHITE);
        IWindow window = new RectangularWindow(2, 0.8);
        return new House(Color.WHITE, door, window);
    }

    public static House flashyHouse() {
        Door door = new Door(Color.random());
        IWindow window = new CircularWindow(1.2);
        return new House(Color.random(), door, window);
    }
}