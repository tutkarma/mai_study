package main;


import javafx.fxml.FXML;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.text.Text;

public class Controller {

    ResizableCanvas canvas;

    @FXML
    public AnchorPane canvasHolder;

    @FXML
    public Text text1;

    @FXML
    public Text text2;

    @FXML
    public Text text3;

    @FXML
    public Text text4;

    @FXML
    public Text text5;

    @FXML
    public Text text6;

    @FXML
    void initialize() {
        canvas = new ResizableCanvas(canvasHolder.getWidth(), canvasHolder.getWidth());

        AnchorPane.setTopAnchor(canvas, 0.);
        AnchorPane.setBottomAnchor(canvas, 0.);
        AnchorPane.setLeftAnchor(canvas, 0.);
        AnchorPane.setRightAnchor(canvas, 0.);
        canvasHolder.getChildren().add(canvas);
        printText();
        canvas.resize(canvasHolder.getWidth(), canvasHolder.getHeight());

        canvas.addEventHandler(MouseEvent.MOUSE_PRESSED, event -> {
            for (int i = 0; i < 6; i++) {
                //проверка на расположение мышки во время клика
                //если клик совершается по какой-то точке, она считается активной
                if(canvas.dots[i].inner(event.getX(),event.getY())) {
                    canvas.chosen = i;
                    break;
                }
            }
        });

        canvas.addEventHandler(MouseEvent.MOUSE_DRAGGED, event -> {
                if(canvas.chosen != -1) {
                    //перемещение активной точки
                    double x = Math.min(Math.max(10, event.getX()), canvas.width - 10);
                    double y = Math.min(Math.max(10, event.getY()), canvas.height - 10);
                    canvas.dots[canvas.chosen].x = x;
                    canvas.dots[canvas.chosen].y = y;
                    printText();
                    canvas.resize(canvas.width, canvas.height);

                }
        });

        canvas.addEventHandler(MouseEvent.MOUSE_RELEASED, event -> canvas.chosen = -1);
    }

    private void printText() {
        text1.setText(canvas.dots[0].x + " " + canvas.dots[0].y);
        text2.setText(canvas.dots[1].x + " " + canvas.dots[1].y);
        text3.setText(canvas.dots[2].x + " " + canvas.dots[2].y);
        text4.setText(canvas.dots[3].x + " " + canvas.dots[3].y);
        text5.setText(canvas.dots[4].x + " " + canvas.dots[4].y);
        text6.setText(canvas.dots[5].x + " " + canvas.dots[5].y);
    }

}
