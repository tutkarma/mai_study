//Довженко А.А. М8О-307Б
//Класс-контроллер для JavaFX окна
package main;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Slider;

public class Controller {
    @FXML
    private Slider aSlider;

    @FXML
    private Slider bSlider;

    @FXML
    private Slider cSlider;

    @FXML
    private Slider redSlider;

    @FXML
    private Slider greenSlider;

    @FXML
    private Slider blueSlider;

    @FXML
    private Slider ambSlider;

    @FXML
    private Slider diffSlider;

    @FXML
    private Slider specSlider;

    @FXML
    private Button button;

    @FXML
    private void initialize() {
        button.setOnAction(event -> {
            Lab4.red = (float)redSlider.getValue();
            Lab4.green = (float)greenSlider.getValue();
            Lab4.blue = (float)blueSlider.getValue();

            Lab4.a = (float)aSlider.getValue();
            Lab4.b = (float)bSlider.getValue();
            Lab4.c = (float)cSlider.getValue();

            Lab4.ambient = (float)ambSlider.getValue();
            Lab4.diffuse = (float)diffSlider.getValue();
            Lab4.specular = (float)specSlider.getValue();
        });
    }
}
