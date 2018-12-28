//Довженко А.А. М8О-307Б
//Класс, выполняющий рассчет необходимых точек гиперболоида и его отрисовку
package main;

import org.joml.Matrix3f;
import org.joml.Matrix4f;
import org.joml.Vector3f;
import shader.Shader;

import java.util.Vector;

import static org.lwjgl.opengl.GL11.*;

public class Figure {

    private Vector<Vector<Vector3f>> figure;

    private float a;
    private float b;
    private float c;

    public Figure(float a, float b, float c) {
        figure = new Vector<>();
        this.a = a;
        this.b = b;
        this.c = c;
    }

    private Vector<Vector3f> countCircle(float z, float step) {
        Vector<Vector3f> res = new Vector<>();
        //double r = Math.sqrt(1 - z * z);
        //double r = 0.5 * z;

        double step1 = 0;
        while (step1 < Math.PI * 2) {
            res.add(new Vector3f((float) (a * Math.sqrt(((z*z) / (c*c)) - 1) * Math.cos(step1)), (float) (b * Math.sqrt((z*z) / (c*c) - 1) * Math.sin(step1)), z));
            step1 += 2 * step;
        }
        step1 = 0;
        res.add(new Vector3f((float) (a * Math.sqrt((z*z) / (c*c) - 1) * Math.cos(step1)), (float) ((b * Math.sqrt((z*z) / (c*c) - 1) *  Math.sin(step1))), z));
        return res;
    }


    public void countFigure(float step) {
        float counter = c;
        float step1 = step;

        float begin = c;

        int n = Math.round( (1f - begin) / step);
        step1 = (1f - begin) / (float)n;
        for (int i = 0; i < n; i++) {
            figure.add(countCircle(counter,step1));
            counter += step1;
        }
        figure.add(countCircle(counter,step1));
        //doTriangles();
    }

    public void drawFigure(Matrix4f matrix, Shader shader) {
        for (int i = 0; i < figure.size() - 1; i++) {
            Vector<Vector3f> vec1 = figure.get(i);
            Vector<Vector3f> vec2 = figure.get(i + 1);
            for (int j = 0; j < vec1.size() - 1; j++) {
                //drawTriangle(vec1.get(j), vec1.get(j + 1), vec2.get(j), Color.BLACK);
                drawTriangle(new Shape(new float[]{
                        vec2.get(j + 1).x, vec2.get(j + 1).y, vec2.get(j + 1).z,
                        vec1.get(j + 1).x, vec1.get(j + 1).y, vec1.get(j + 1).z,
                        vec2.get(j).x, vec2.get(j).y, vec2.get(j).z
                }), matrix, shader);

                drawTriangle(new Shape(new float[]{
                        vec1.get(j).x, vec1.get(j).y, vec1.get(j).z,
                        vec1.get(j + 1).x, vec1.get(j + 1).y, vec1.get(j + 1).z,
                        vec2.get(j).x, vec2.get(j).y, vec2.get(j).z
                }), matrix, shader);
            }
        }

        Vector<Vector3f> v = figure.get(figure.size() - 1);
        for (int i = 0; i < v.size() - 1; i++) {

            drawTriangle( new Shape( new float[]{
                    v.get(i).x, v.get(i).y, v.get(i).z,
                    v.get(i + 1).x, v.get(i + 1).y, v.get(i + 1).z,
                    0,0,1
            }), matrix, shader);
        }
    }


    private boolean drawTriangle(Shape kek, Matrix4f matrix, Shader shader) {
        float k = triangleVisible(kek.getVerts(), matrix, shader);
        if(k > 0) {
            //shader.setColor(Math.min(0.5f * k, 1f) , Math.min(0.5f * k, 1f), Math.min(0.5f * k, 1f));
            kek.draw(GL_POLYGON);
            return true;
        }
        return false;
    }


    private float triangleVisible(float[] vertices, Matrix4f matrix, Shader shader) {
        float d;
        Matrix3f m = new Matrix3f();
        m = matrix.get3x3(m);

        Vector3f p1 = new Vector3f(vertices[0], vertices[1], vertices[2]);
        Vector3f p2 = new Vector3f(vertices[3], vertices[4], vertices[5]);
        Vector3f p3 = new Vector3f(vertices[6], vertices[7], vertices[8]);
        Vector3f inner = new Vector3f(0, 0, c + 0.01f);
        m.transform(p1);
        m.transform(p2);
        m.transform(p3);
        m.transform(inner);

        Vector3f v1 = p2.sub(p1);
        Vector3f v2 = p3.sub(p1);
        Vector3f n = v1.cross(v2);//crossProduct(sb1, sb2);
        Vector3f tmp = inner.sub(p1);

        if (n.dot(tmp) > 0) {
            n.negate();
        }
        Vector3f view = new Vector3f(0, 0, 50);
        if (n.dot(view) > 0) {
            d = Math.abs(n.dot(view) / (view.length() * n.length()));
            shader.set("normal",n);
        } else {
            d = -1;
        }

        return d;
    }

    public void setA(float a) {
        this.a = a;
    }

    public void setB(float b) {
        this.b = b;
    }

    public void setC(float c) {
        this.c = c;
    }

}
