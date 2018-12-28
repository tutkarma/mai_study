//Довженко А.А. М8О-307Б
//Класс, предоставляющий интерфейс взаимодействия с шейдером

package shader;

import org.joml.Matrix4f;
import org.joml.Vector3f;
import org.lwjgl.BufferUtils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.nio.FloatBuffer;


import static org.lwjgl.opengl.GL20.*;

public class Shader {
    private final String FOLDER = "shader/";
    private final String VERTEX_SHADER_FILE = "shader.vs";
    private final String FRAGMENT_SHADER_FILE = "shader.fs";

    private int program;
    private int vertexShader;
    private int fragmentShader;

    public Shader() {
        program = glCreateProgram();
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, getSource(FOLDER + VERTEX_SHADER_FILE));
        glCompileShader(vertexShader);
        if (glGetShaderi(vertexShader, GL_COMPILE_STATUS) != 1) {
            System.err.println(glGetShaderInfoLog(vertexShader));
            System.exit(1);
        }

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, getSource(FOLDER + FRAGMENT_SHADER_FILE));
        glCompileShader(fragmentShader);
        if (glGetShaderi(fragmentShader, GL_COMPILE_STATUS) != 1) {
            System.err.println(glGetShaderInfoLog(fragmentShader));
            System.exit(2);
        }

        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glBindAttribLocation(program, 0, "vertex");

        glLinkProgram(program);
        if (glGetProgrami(program, GL_LINK_STATUS) != 1) {
            System.err.println(glGetProgramInfoLog(program));
            System.exit(3);
        }

        glValidateProgram(program);
        if (glGetProgrami(program, GL_VALIDATE_STATUS) != 1) {
            System.err.println(glGetProgramInfoLog(program));
            System.exit(4);
        }
    }


    public void setColor(float red, float green, float blue) {
        set("red", red);
        set("green", green);
        set("blue", blue);
    }

    public void set(String name, Vector3f vector) {
        int pointer = glGetUniformLocation(program, name);
        if (pointer != -1) {
            glUniform3f(pointer, vector.x, vector.y, vector.z);
        }
    }

    public void set(String name, int value) {
        int pointer = glGetUniformLocation(program, name);
        if(pointer != -1) {
            glUniform1i(pointer,value);
        }
    }

    public void set(String name, float value) {
        int pointer = glGetUniformLocation(program,name);
        if( pointer != -1) {
            glUniform1f(pointer, value);
        }
    }

    public void set(String name, Matrix4f matrix) {
        int pointer = glGetUniformLocation(program,name);
        if(pointer != -1) {
            FloatBuffer buffer = BufferUtils.createFloatBuffer(16);
            matrix.get(buffer);
            glUniformMatrix4fv(pointer, false, buffer);
        }
    }

    public void bindShader() {
        glUseProgram(program);
    }

    private String getSource(String filename) {
        //получение кода шейдера из файла
        StringBuilder res = new StringBuilder();
        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(new File(filename)));

            String str;
            while((str = reader.readLine()) != null) {
                res.append(str);
                res.append("\n");
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        //System.out.println(res.toString());
        return res.toString();
    }
}