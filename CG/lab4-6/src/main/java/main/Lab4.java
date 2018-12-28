// Довженко А.А. М8О-307Б
// Лабораторная работа 4-5
// Вариант№7: Одна из полостей двуполостного гиперболоида.

package main;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import org.joml.Matrix4f;
import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;
import shader.Shader;

import java.io.File;
import java.io.FileInputStream;
import java.net.URL;
import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

public class Lab4 extends Application{

    // The window handle
    private long window;
    private Matrix4f matrix;
    private double curX, curY;

    public static float a = 1;
    public static float b = 1;
    public static float c = 0.5f;

    public static float red = 0;
    public static float green = 0.2f;
    public static float blue = 0.8f;

    public static float ambient = 1f;
    public static float diffuse = 1f;
    public static float specular = 1f;

    private float step = 0.25f;
    private Figure figure;

    public Lab4() {
        matrix = new Matrix4f().identity();
        curX = 0;
        curY = 0;
    }

    public void run() {
        System.out.println("Hello LWJGL " + Version.getVersion() + "!");

        initGLFW();
        loop();

        // Free the window callbacks and destroy the window
        glfwFreeCallbacks(window);
        glfwDestroyWindow(window);

        // Terminate GLFW and free the error callback
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void initGLFW() {
        //инициализиция всего необходимого перед началом работы

        GLFWErrorCallback.createPrint(System.err).set();
        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints(); // optional, the current window hints are already the default
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // the window will stay hidden after creation
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // the window will be resizable

        window = glfwCreateWindow(500, 500, "ЛР №4-5 Вариант: Гиперболоид", NULL, NULL);
        if (window == NULL) {
            throw new RuntimeException("Failed to create the GLFW window");
        }

        glfwSetKeyCallback(window, (window, key, scancode, action, mods) -> {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
            if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
                matrix.rotateX((float) Math.toRadians(-10));
            }
            if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
                matrix.rotateX((float) Math.toRadians(10));
            }
            if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
                matrix.rotateY((float) Math.toRadians(-10));
            }
            if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
                matrix.rotateY((float) Math.toRadians(10));
            }
        });

        glfwSetFramebufferSizeCallback(window, (window, w, h) -> {
            glViewport(0, 0, w, h);
            glOrtho(0, w, h, 0, -1, 1);
        });

        try (MemoryStack stack = stackPush()) {
            IntBuffer pWidth = stack.mallocInt(1); // int*
            IntBuffer pHeight = stack.mallocInt(1); // int*

            // Get the window size passed to glfwCreateWindow
            glfwGetWindowSize(window, pWidth, pHeight);

            // Get the resolution of the primary monitor
            GLFWVidMode vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

            // Center the window
            glfwSetWindowPos(
                    window,
                    (vidmode.width() - pWidth.get(0)) / 2,
                    (vidmode.height() - pHeight.get(0)) / 2
            );
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwShowWindow(window);
    }

    private void loop() {
        //цикл, в котором происходит отрисовка

        GL.createCapabilities();
        System.out.println(glGetString(GL_VERSION));

        glEnable(GL_TEXTURE_2D);
        glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

        Shape x = new Shape(new float[]{0, 0, 0, 1, 0, 0});
        Shape y = new Shape(new float[]{0, 0, 0, 0, 1, 0});
        Shape z = new Shape(new float[]{0, 0, 0, 0, 0, 2});

        FloatBuffer floatBuffer = BufferUtils.createFloatBuffer(16);
        matrix.scale(0.25f);

        Shader shader = new Shader();
        shader.set("ambientK", ambient);
        shader.set("diffuseK", diffuse);
        shader.set("specularK", specular);

        figure = new Figure(a,b,c);
        figure.countFigure(step);
        double time = 0;

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadMatrixf(matrix.get(floatBuffer));

            shader.set("matrix", matrix);

            if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                DoubleBuffer xBegin = BufferUtils.createDoubleBuffer(1);
                DoubleBuffer yBegin = BufferUtils.createDoubleBuffer(1);
                glfwGetCursorPos(window, xBegin, yBegin);

                double xB = xBegin.get(0);
                double yB = yBegin.get(0);

                int dx = (int) (xB - curX);
                int dy = (int) (yB - curY);

                if (dx > 30 || dx < -30) {
                    dx = 0;
                }

                if (dy > 30 || dy < -30) {
                    dy = 0;
                }
                matrix.rotateX((float) Math.toRadians(dx));
                matrix.rotateY((float) Math.toRadians(dy));
                curX = xB;
                curY = yB;

            }

            shader.bindShader();

            shader.set("axis",1);
            shader.setColor(1.0f, 0.0f, 0.0f);
            x.draw(GL_LINE_STRIP);
            shader.setColor(1.0f, 0.0f, 0.0f);
            y.draw(GL_LINE_STRIP);
            shader.setColor(1.0f, 0.0f, 0.0f);
            z.draw(GL_LINE_STRIP);
            shader.set("axis",0);

            //shader.setColor((float)Math.abs(Math.sin(time)), (float)Math.abs(Math.sin(time * 2)), (float)Math.abs(Math.sin(time / 3)));
            shader.setColor((float)Math.abs(Math.sin(time)), (float)Math.abs(Math.sin(time * 2)), (float)Math.abs(Math.sin(time / 3)));
            figure.drawFigure(matrix, shader);
            glfwSwapBuffers(window);
            glfwPollEvents();

            try {
                Thread.sleep(35);
            } catch (InterruptedException e) {
                //e.printStackTrace();
            }
            time += 0.05;
            if(time >= 360)  {
                time -= 360;
            }
        }
    }


    @Override
    public void start(Stage stage) throws Exception {
        final Parent root = FXMLLoader.load(getClass().getResource("/fxWindow/window.fxml"));
        final Scene scene = new Scene(root);

        stage.setTitle("ЛР №4-5 Вариант №7: Гиперболоид");
        stage.setScene(scene);
        stage.show();
        stage.setMinHeight(scene.getWindow().getHeight());
        stage.setMinWidth(scene.getWindow().getWidth());
    }


    public static void main(String[] args) {
        //при запуска отрывается окно JavaFX, в котором задаются параметры, а после его закрытия открывается окно с фигурой
        Lab4 lab4 = new Lab4();
        launch(args);
        lab4.run();
    }
}