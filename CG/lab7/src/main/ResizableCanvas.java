// Довженко А.А. М8О-307Б
// Холст

package main;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.util.Random;
import java.util.Vector;


public class ResizableCanvas extends Canvas{

    public double height,width;

    public Dot[] dots;
    int chosen;

    @Override
    public boolean isResizable() {
        return true;
    }

    public ResizableCanvas(double width, double height) {

        super(width,height);
        this.width = width;
        this.height = height;

        dots = new Dot[6];
        Random rand = new Random();
        //в начале работы точки расположены по умолчанию
        dots[0] = new Dot(49, 223, 15, Color.rgb(0,0,0));
        dots[1] = new Dot(60, 34, 15,  Color.rgb(0, 0, 0));
        dots[2] = new Dot(90, 34, 15,  Color.rgb(0,0,0));
        dots[3] = new Dot(90, 125, 15, Color.rgb(0, 0,  0));
        dots[4] = new Dot(110, 220, 15,Color.rgb(0,  0, 0 ));
        dots[5] = new Dot(170, 50, 15, Color.rgb( 0, 0,0));

        chosen = -1;

    }

    @Override
    public void resize(double width,double height) {
        //во время изменения размера происходит отрисовка
        setWidth(width);
        setHeight(height);

        GraphicsContext gc = this.getGraphicsContext2D();
        gc.setFill(Color.WHITE);
        gc.fillRect(0,0,getWidth(),getHeight());


        for (int i = 0; i < 6; i++){
            if(i != 5) {
                gc.setStroke(Color.BLACK);
                gc.strokeLine(dots[i].x,dots[i].y, dots[i + 1].x, dots[i + 1].y);
            }

            gc.setFill(dots[i].color);
            gc.fillOval(dots[i].x - dots[i].r / 2, dots[i].y - dots[i].r / 2, dots[i].r, dots[i].r);
        }

        this.height = height;
        this.width = width;

        Vector<Vector<Double>> curve = new Curve(dots).countCurve();

        gc.setStroke(Color.rgb(255, 0, 0));

        Vector<Double> x = curve.get(0);
        Vector<Double> y = curve.get(1);
        for (int i = 1; i < x.size(); i++) {
            gc.strokeLine(x.get(i - 1), y.get(i - 1), x.get(i), y.get(i));
        }


    }
}
