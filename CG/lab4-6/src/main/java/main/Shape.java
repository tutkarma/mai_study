//Довженко А.А. М8О-307Б
//Класс для отрисовки контуров

package main;

import org.joml.Vector3f;
import org.lwjgl.BufferUtils;

import java.nio.FloatBuffer;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL15.*;

public class Shape {
    private int vertexArrayPtr;
    private int n;

    public float[] getVerts() {
        return verts;
    }
    private float[] verts;

    public Shape(float[] vertexArray) {
        if(vertexArray.length > 7) {
            for (int i = 0; i < 3; i++) {
                if (vertexArray[3 * i] == 0 && vertexArray[3 * i + 1] == 0 && vertexArray[3 * i + 2] == 0) {
                    System.out.println(vertexArray[0] + " " + vertexArray[1] + " " + vertexArray[2]);
                    System.out.println(vertexArray[3] + " " + vertexArray[4] + " " + vertexArray[5]);
                    System.out.println(vertexArray[6] + " " + vertexArray[7] + " " + vertexArray[8]);
                    System.out.println(" ");
                }
            }
        }

        verts = vertexArray;
        n = vertexArray.length;
        FloatBuffer floatBuffer = BufferUtils.createFloatBuffer(n);
        n /= 3;

        floatBuffer.put(vertexArray);
        floatBuffer.flip();

        vertexArrayPtr = glGenBuffers();
        glBindBuffer(GL_ARRAY_BUFFER, vertexArrayPtr);
        glBufferData(GL_ARRAY_BUFFER, floatBuffer, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    public void draw(int mode) {

        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, vertexArrayPtr);

        glVertexPointer(3, GL_FLOAT, 0, 0);
        glDrawArrays(mode, 0, n);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}
