package main;


import javafx.event.EventHandler;
import javafx.event.EventType;
import javafx.fxml.FXML;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.text.Text;

public class Controller {

    private ResizableCanvas canvas;

    double xStart;
    double yStart;
    double xEnd;
    double yEnd;

    @FXML
    public AnchorPane canvasHolder;

    @FXML
    void initialize() {
        canvas = new ResizableCanvas(canvasHolder.getWidth(), canvasHolder.getWidth());

        AnchorPane.setTopAnchor(canvas, 0.);
        AnchorPane.setBottomAnchor(canvas, 0.);
        AnchorPane.setLeftAnchor(canvas, 0.);
        AnchorPane.setRightAnchor(canvas, 0.);
        canvasHolder.getChildren().add(canvas);

        canvas.resize(canvasHolder.getWidth(), canvasHolder.getHeight());

        canvas.addEventHandler(MouseEvent.MOUSE_PRESSED, event -> {
            xStart = event.getScreenX();
            yStart = event.getScreenY();
            xEnd = event.getScreenX();
            yEnd = event.getScreenY();
        });

        canvas.addEventHandler(MouseEvent.MOUSE_DRAGGED, event -> {

            xEnd = event.getScreenX();
            yEnd = event.getScreenY();

            double dX = xEnd - xStart;
            double dY = yEnd - yStart;

            canvas.matrix.mult(TransformMatrix.rotationY(dX));
            canvas.matrix.mult(TransformMatrix.rotationX(dY));

            xStart = xEnd;
            yStart = yEnd;

            canvas.resize(canvas.width, canvas.height);
        });
    }
}
