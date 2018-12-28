package main;

import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

import java.util.Vector;

public class Pencil {
    Pencil() {
    }

    static void drawVector(Vector<Point> points, TransformMatrix matrix, ResizableCanvas canvas, Color color) {
        Vector<Point> transformed = new Vector<>();
        for (int i = 0; i < points.size(); i++) {
            transformed.add( matrix.transform(points.get(i)).scale() );
        }

        GraphicsContext gc = canvas.getGraphicsContext2D();
        gc.setStroke(color);

        double xCenter = canvas.width / 2;
        double yCenter = canvas.height / 2;

        for (int i = 0; i < transformed.size() - 1; i++) {
            Point p1 = transformed.get(i);
            Point p2 = transformed.get(i + 1);

            gc.strokeLine(xCenter + p1.x, yCenter - p1.y, xCenter + p2.x, yCenter - p2.y);

        }
    }

    static void drawSurface(Vector<Vector<Point>> surface, TransformMatrix matrix, ResizableCanvas canvas) {
        Vector<Vector<Point>> points = new Vector<>();

        for (int i = 0; i < surface.size(); i++) {
            Vector<Point> tmp = new Vector<>();

            Vector<Point> current = surface.get(i);
            for (int j = 0; j < surface.get(i).size(); j++) {
                tmp.add( matrix.transform(current.get(j)).scale() );
            }
            points.add(tmp);
        }

        GraphicsContext gc = canvas.getGraphicsContext2D();
        gc.setStroke(Color.BLACK);

        double xCenter = canvas.width / 2;
        double yCenter = canvas.height / 2;

        for (int i = 0; i < points.size() - 1; i++) {
            Vector<Point> cur = points.get(i);
            Vector<Point> next = points.get(i + 1);
            for (int j = 0; j < cur.size() - 1; j++) {
                drawTriangle(cur.get(j), cur.get(j + 1), next.get(j), gc, xCenter, yCenter);
                drawTriangle(cur.get(j + 1), next.get(j), next.get(j + 1), gc, xCenter, yCenter);

            }
        }
    }

    private static void drawTriangle(Point p1, Point p2, Point p3, GraphicsContext gc, double xCenter, double yCenter) {
        gc.strokeLine(xCenter + p1.x, yCenter - p1.y, xCenter + p2.x, yCenter - p2.y);
        gc.strokeLine(xCenter + p1.x, yCenter - p1.y, xCenter + p3.x, yCenter - p3.y);
        gc.strokeLine(xCenter + p3.x, yCenter - p3.y, xCenter + p2.x, yCenter - p2.y);
    }

}
