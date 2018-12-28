package Lab7;

import Lab7.Dot;

import java.util.Vector;

public class Curve {
    public double[] x;
    public double[] y;

    public Curve(Dot[] dots) {
        x = new double[6];
        y = new double[6];
        for (int i = 0; i < 6; i++) {
            x[i] = dots[i].x;
            y[i] = dots[i].y;
        }
    }

    Vector<Double> count(double t) {
        //вычисление точки кривой в зависимости от заданного t
        Vector<Double> res = new Vector<>();

        double[] tp = new double[6];
        tp[0] = 1;
        tp[1] = t;

        double[] t_1p = new double[6];
        t_1p[0] = 1;
        t_1p[1] = 1 - t;
        for (int i = 2; i < 6; i++) {
            t_1p[i] = t_1p[i - 1] * t_1p[1];
            tp[i] = tp[i - 1] * tp[1];
        }

        double x = 0;
        double y = 0;
        int[] k = new int[] {1, 5, 10, 10, 5, 1};
        for (int i = 0; i < 6; i++) {
            x += tp[i] * t_1p[5 - i] * this.x[i] * k[i];
            y += tp[i] * t_1p[5 - i] * this.y[i] * k[i];
        }

        res.add(x);
        res.add(y);

        return res;
    }

    Vector<Vector<Double>> countCurve() {
        //рассчет точек кривой
        Vector<Vector<Double>> res = new Vector<>();

        Vector<Double> xs = new Vector<>();
        Vector<Double> ys = new Vector<>();

        double n = Math.pow(10,3);
        double step = 1 / n;

        for (int i = 0; i <= n; i++) {
            Vector<Double> tmp = count(step * i);
            xs.add(tmp.get(0));
            ys.add(tmp.get(1));
        }

        res.add(xs);
        res.add(ys);
        return res;
    }
}
