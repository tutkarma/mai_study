package main;

import javafx.scene.paint.Color;


public class Dot {
    public double x;
    public double y;
    public double r;
    Color color;

    public Dot(double x, double y, double r, Color color) {
        this.x = x;
        this.y = y;
        this.r = r;
        this.color = color;
    }

    public boolean inner(double x, double y) {
        //проверка, является ли точка внутренней для данного круга
        double dx = this.x - x;
        double dy = this.y - y;
        return r * r >= dx * dx  +  dy * dy;
    }

}
