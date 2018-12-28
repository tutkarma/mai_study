package main;

import java.util.Vector;

import static main.Point.*;

public class Spline {

    double step;
    Point p1;
    Point p2;
    Point p3;

    Vector<Point> points;
    int n;

    Spline(Vector<Point> pts, int n) {

        this.step = 1. / (double) n;
        this.n = n;

        points = new Vector<>();
        points.addAll(pts);
    }

    public Vector<Point> spline(boolean flag) {

        Vector<Point> ret = new Vector<>();

        for (int i = 0; i < points.size() - 1; i++) {

            ret.addAll(count(i, flag));
        }
        ret.add(points.get(points.size() - 1));
        return ret;
    }

    private Vector<Point> count(int k, boolean flag) {
        Point v1 = points.get(k);
        Point v2 = points.get(k + 1);

        Vector<Point> ret = new Vector<>();
        Point p1 = points.get(k);
        Point p2;

        if(k == 0) {
            p2 = new Point(0,0,0,1);
        }
        else {
            p2 = m(points.get(k - 1), points.get(k + 1), 2 * step);
        }

        Point p3 = points.get(k + 1);
        Point p4;
        if(k == points.size() - 2 || k == points.size() - 1) {
            p4 = new Point(0,0,0,1);
        }
        else {
            p4 = m(points.get(k), points.get(k + 2), 2 * step);
        }

        double t = 0;
        ret.add(v1);
        for (int i = 0; i < n - 1; i++) {
            t += step;
            Point tmp1 = mul(p1,  2 * Math.pow(t,3) - 3 * Math.pow(t,2) + 1);
            Point tmp2 = mul(p2, (Math.pow(t,3) - 2 * Math.pow(t,2) + t) * 0.25);
            if(flag) {
                tmp2 = mul(tmp2, v2.x - v1.x);
            } else {
                tmp2 = mul(tmp2, v2.y - v1.y);
            }

            Point tmp3 = mul(p3, -2 * Math.pow(t,3) + 3 * Math.pow(t,2));
            Point tmp4 = mul(p4, (Math.pow(t,3) - Math.pow(t,2)) * 0.25);
            if(flag) {
                tmp4 = mul(tmp4, v2.x - v1.x);
            } else {
                tmp4 = mul(tmp4, v2.y - v1.y);
            }

            ret.add( sum( sum(tmp1, tmp2), sum(tmp3, tmp4)  ));
        }

        return ret;
    }

    private Point m(Point v1, Point v2, double dt) {
        return mul(sub(v1,v2), 1. / dt);
    }
}
