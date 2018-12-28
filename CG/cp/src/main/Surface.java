package main;
import java.util.Vector;

import static main.Point.*;

public class Surface {
    public Vector<Vector<Point>> surface;

    Surface() {
        surface = new Vector<>();
    }


    void countSurface(Spline border1, Spline border2, Spline border3, Spline border4) {
        if(border1.n != border3.n || border2.n != border4.n) {
            System.exit(1);
        }

        double step1 = border1.step;
        double step2 = border2.step;

        double w = 0;
        double u;

        Vector<Point> a = border1.spline(true); // 1 || 3
        Vector<Point> d = border2.spline(true); // 2 || 4
        Vector<Point> b = border3.spline(true);
        Vector<Point> c = border4.spline(true);

        int n = a.size();
        int m = c.size();

        for (int j = 0; j < n; j++) {
            Vector<Point> tmp = new Vector<>();
            u = j * border1.step;
            w = 0;
            for (int i = 0; i < m; i++) {
                w = i * border2.step;

                Point tmp1 = sum( mul( a.get(j), 1 - w), mul(b.get(j), w) );
                Point tmp2 = sum( mul( c.get(i), 1 - u), mul(d.get(i), u) );

                Point tmp3 = mul( sum( mul(a.get(0), (1-u) * (1-w)) , mul(b.get(0), (1-u) * w) ) , -1);
                Point tmp4 = mul( sum( mul(a.get(n - 1), u * (1-w)) , mul(b.get(n - 1), u * w) ) , -1);
                tmp.add( sum( sum(tmp1, tmp2) , sum(tmp3, tmp4) ));
            }

            surface.add(tmp);
        }
    }
}
