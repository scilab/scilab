package com.foo;

import java.util.Random;

public enum Color {

    BLACK("black"), BLUE("blue"), BROWN("brown"), GREEN("green"),
    ORANGE("orange"), RED("red"), WHITE("white"), YELLOW("yellow");

    public String	descr;

    private Color(String s) {
        descr = s;
    }

    public String getDescr() {
        return descr;
    }

    public static Color random() {
        Color[] allColors = values();
        return allColors[new Random().nextInt(values().length)];
    }

}