package com.foo;

public class CircularWindow implements IWindow {

    double	radius;

    public CircularWindow(double radius) {
        this.radius = radius;
    }

    public double getWindowArea() {
        return Math.PI * radius * radius;
    }
}