import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;
import java.util.Collection;

public class MapGraphic extends JPanel {

    // Map information
    private Map map;
    private int MAP_HEIGHT;
    private int MAP_WIDTH;

    // Color of the objects on the map
    private final Color color = Color.BLACK;
    private final Color link_color = Color.BLACK;
    private final Color zone_border = Color.RED;
    private final Color zone_fill = Color.PINK;

    // Color of the points depending on the sensor that added it *RAINBOW*
    private final Color[] sensorColors = new Color[]{
            Color.RED,
            Color.ORANGE,
            Color.YELLOW,
            Color.GREEN,
            Color.BLUE,
            Color.MAGENTA
    };

    // Size of a pixel in the map
    private final int PIXEL_SIZE = 3;
    private final int HALF_PIXEL = 1;
    private final int PADDING = 4;

    private double RATIO = 1;

    // Should we draw link between nodes ?
    private boolean drawLink;
    // Should we draw the zones ?
    private boolean drawZones;

    // Constructor
    public MapGraphic(Map map){
        this(map, false);
    }

    public MapGraphic(Map map, boolean drawLink){
        this.drawLink = drawLink;
        this.map = map;
        MAP_HEIGHT = map.getHeight();
        MAP_WIDTH = map.getWidth();

        setPreferredSize(new Dimension(600, 600));
    }

    public void setDrawLink(boolean drawLink){
        this.drawLink = drawLink;
    }

    @Override
    public void paintComponent(Graphics g) {
        super.paintComponent(g);

        // Clear the window
        g.clearRect(0, 0, getWidth(), getHeight());

        // Get the map matrix
        if(this.map.generateMap()) {
            MAP_HEIGHT = this.map.getHeight();
            MAP_WIDTH = this.map.getWidth();


            RATIO = (600.0 - PADDING - PADDING) / Math.max(MAP_WIDTH, MAP_HEIGHT);

            int minX = map.getFloorX();
            int minY = map.getFloorY();
            Collection<Zone> zones = map.getZones().values();

            for(Zone zone : zones){
                int x1 = (int) (Math.round(zone.getX1() - minX) * RATIO) + PADDING;
                int x2 = (int) (Math.round(zone.getX2() - minX) * RATIO) + PADDING;
                int y1 = (int) (Math.round(zone.getY1() - minY) * RATIO) + PADDING;
                int y2 = (int) (Math.round(zone.getY2() - minY) * RATIO) + PADDING;

                // Color of the inside
                g.setColor(zone_fill);
                g.fillRect(x1, y1, x2-x1, y2-y1);

                // Color of the border
                g.setColor(zone_border);

                g.drawLine(x1, y1, x2, y1);
                g.drawLine(x2, y1, x2, y2);
                g.drawLine(x2, y2, x1, y2);
                g.drawLine(x1, y2, x1, y1);
            }

            // Set the color to the color we will draw the nodes and links
            g.setColor(color);

            ArrayList<Node> nodes = new ArrayList<>();
            nodes.addAll(this.map.getNodes());

            for (Node node : nodes) {
                int x = (int) (node.getxIndex() * RATIO) + PADDING;
                int y = (int) (node.getyIndex() * RATIO) + PADDING;
                // Change the color of the point depending on the sensor
                if(node.getFromSensor() >= 0 && node.getFromSensor() < sensorColors.length) g.setColor(sensorColors[node.getFromSensor()]);
                // Draw the point
                g.fillRect(x-HALF_PIXEL, y-HALF_PIXEL, PIXEL_SIZE, PIXEL_SIZE);

                // Add a thin black border to the point
                g.setColor(color);
                g.drawRect(x-HALF_PIXEL, y-HALF_PIXEL, PIXEL_SIZE, PIXEL_SIZE);

                // Draw the link between nodes
                if (drawLink) {
                    g.setColor(link_color);
                    for (Node out : node.getOut()) {
                        int x2 = (int) (out.getxIndex() * RATIO) + PADDING;
                        int y2 = (int) (out.getyIndex() * RATIO) + PADDING;
                        g.drawLine(x, y, x2, y2);
                    }
                }
            }
        }
    }

}