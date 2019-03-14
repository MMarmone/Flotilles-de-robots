import javax.swing.*;
import java.awt.*;

public class MapGraphic extends JPanel {

    // Map information
    private Map map;
    private int MAP_HEIGHT;
    private int MAP_WIDTH;

    // Color of the objects on the map
    private final Color color = Color.BLACK;

    // Size of a pixel in the map
    private final int PIXEL_SIZE = 2;
    private final int PADDING = 4;


    // Constructor
    public MapGraphic(Map map){
        this.map = map;
        MAP_HEIGHT = map.getMap().length;
        MAP_WIDTH = map.getMap()[0].length;

        // Set the size of the graphic window to the size of the map
        setPreferredSize(new Dimension(MAP_WIDTH + PADDING + PADDING, MAP_HEIGHT + PADDING + PADDING));
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        // Clear the window
        g.clearRect(0, 0, getWidth(), getHeight());

        // Get the map matrix
        int[][] map = this.map.getMap();

        // Set the color to the color we will draw the objects
        g.setColor(color);

        for (int i = 0; i < MAP_HEIGHT; i++) {
            for (int j = 0; j < MAP_WIDTH; j++) {
                // If there is an object (!= 0), we draw a pixel
                if(map[i][j] != 0) g.fillRect(i+PADDING, j+PADDING, PIXEL_SIZE, PIXEL_SIZE);
            }
        }
    }
}