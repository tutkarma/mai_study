package main;

import java.util.Vector;

public class Point {
    public double x;
    public double y;
    public double z;
    public double w;


    Point(double x, double y, double z, double w) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    Point(Point p) {
        this.x = p.x;
        this.y = p.y;
    }

    public static Point sum(Point v1, Point v2) {
        return new Point(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v2.w);
    }

    public static Point sub(Point v1, Point v2) {
        return new Point(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z, v2.w);
    }

    public static Point mul(Point v1, double m) {
        return new Point(v1.x * m, v1.y * m, v1.z * m, v1.w);
    }


    public Point scale() {
        return new Point(x * w, y * w, z * w, 1);
    }
}
