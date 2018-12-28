//Довженко А.А. М8О-307Б
//Холст

package main;

import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;


import java.io.*;
import java.util.Vector;

import static main.Pencil.*;

public class ResizableCanvas extends Canvas{
    int N;
    public double height,width;

    TransformMatrix matrix;

    Spline spline1;
    Spline spline2;
    Spline spline3;
    Spline spline4;

    Surface surface;

    @Override
    public boolean isResizable() {
        return true;
    }

    public ResizableCanvas(double width, double height) {
        super(width,height);
        this.width = width;
        this.height = height;
        matrix = new TransformMatrix();

        getPoints();
        surface = new Surface();
        surface.countSurface(spline1, spline2, spline3, spline4);
    }

    public void getPoints() {
        BufferedReader file = new BufferedReader(new InputStreamReader(System.in));
        String filename = getLine(file);
        FileReader fr = null;
        try {
            fr = new FileReader(filename);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        BufferedReader in = new BufferedReader(fr);

        String input;
        String[] parsed;
        input = getLine(in);
        N = Integer.parseInt(input);

        int n,m;
        parsed = getLines(in);
        n = Integer.parseInt(parsed[0]);
        m = Integer.parseInt(parsed[1]);

        Vector<Vector<Point>> points = new Vector<>();
        for (int i = 0; i < 2; i++) {
            Vector<Point> v1 = new Vector<>();
            Vector<Point> v2 = new Vector<>();

            parsed = getLines(in);
            for (int j = 0; j < n; j++) {
                v1.add(new Point(
                        Double.parseDouble(parsed[j * 3]),
                        Double.parseDouble(parsed[j * 3 + 1]),
                        Double.parseDouble(parsed[j * 3 + 2]), 1
                ));
            }

            parsed = getLines(in);
            for (int j = 0; j < m; j++) {
                v2.add(new Point(
                        Double.parseDouble(parsed[j * 3]),
                        Double.parseDouble(parsed[j * 3 + 1]),
                        Double.parseDouble(parsed[j * 3 + 2]), 1
                ));
            }
            points.add(v1);
            points.add(v2);
        }

        System.out.println(N + "\n" + n + " " + m);
        for (int i = 0; i < points.size(); i++) {
            Vector<Point> pts = points.get(i);
            for (int j = 0; j < pts.size(); j++) {
                System.out.println(pts.get(j).x + " " + pts.get(j).y + " " + pts.get(j).z);
            }
            System.out.print("\n");
        }

        spline1 = new Spline(points.get(0), N);
        spline2 = new Spline(points.get(1), N);
        spline3 = new Spline(points.get(2), N);
        spline4 = new Spline(points.get(3), N);
    }

    private String getLine(BufferedReader in) {
        String input = null;
        try {
            input = in.readLine();

        } catch (IOException e) {
            e.printStackTrace();
        }
        return input;
    }

    private String[] getLines(BufferedReader in) {
        String[] input = null;
        try {
            input = in.readLine().split("\\s");

        } catch (IOException e) {
            e.printStackTrace();
        }
        return input;
    }


    @Override
    public void resize(double width,double height) {
        //во время изменения размера происходит отрисовка
        setWidth(width);
        setHeight(height);

        GraphicsContext gc = this.getGraphicsContext2D();
        gc.setFill(Color.WHITE);
        gc.fillRect(0,0,getWidth(),getHeight());

        this.height = height;
        this.width = width;

        gc.setFill(Color.rgb( 72, 61,139));
        gc.setFill(Color.GOLD);
        gc.setStroke(Color.DARKTURQUOISE);


        matrix.matrix[3][3] = (int)(Math.min(width,height) / 10);
        drawSurface(surface.surface, matrix,this);

        drawVector(spline1.spline(true), matrix, this, Color.rgb(255, 0,0));
        drawVector(spline2.spline(true), matrix, this, Color.rgb(255, 0,0));
        drawVector(spline3.spline(true), matrix, this, Color.rgb(255, 0,0));
        drawVector(spline4.spline(true), matrix, this, Color.rgb(255, 0,0));
    }
}
