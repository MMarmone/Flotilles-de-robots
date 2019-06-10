import java.util.ArrayList;
import java.util.HashMap;

public class Map {

    /**************************
     * STATIC
     **************************/
    // Used for the method add to determine the nature of angle.
    public static final int RADIAN = 0, DEGREE = 1;

    // Define at which distance a node get absorbed (in mm).
    public static final double ABSORBTION_RADIUS = 4;
    // Define at which distance 2 nodes are linked (in mm).
    public static final double LINK_RADIUS = 8;

    // Define the scale of the map created (default : 1/8).
    private static final double RATIO = 1;


    /**************************
     * OBJECT
     **************************/

    // Dimension of the map
    private int width, height;

    // Zones
    private HashMap<String, Zone> zones;
    // Size of a zone
    private int xZone, yZone;

    // SPECIAL
    private double minX, minY, maxX, maxY;
    private int floorX, floorY;

    // The set of nodes in the map
    private ArrayList<Node> nodes;


    public Map(int wZone, int hZone){
        width = 0;
        height = 0;
        minX = 100000; maxX = -100000;
        minY = 100000; maxY = -100000;
        nodes = new ArrayList<>();
        zones = new HashMap<>();

        xZone = wZone;
        yZone = hZone;
    }

    /**
     * Find the index x of the zone depending on x
     * @param x position of the zone
     * @return
     */
    private int findZoneX(double x){
        int xIndex = (int) (x / xZone);
        if(x < 0) xIndex--;
        return xIndex;
    }

    /**
     * Find the index y of the zone depending on y
     * @param y
     * @return
     */
    private int findZoneY(double y){
        int yIndex = (int) (y / yZone);
        if(y < 0) yIndex--;
        return yIndex;
    }


    /**
     * Create a link between the node node and all the nodes from the zone, depending on the position
     * @param pos
     * @param xIndex
     * @param yIndex
     * @param node
     */
    private void createLink(Position pos, int xIndex, int yIndex, Node node){
        String up = xIndex+":"+(yIndex-1);
        String down = xIndex+":"+(yIndex+1);
        String left = (xIndex-1)+":"+yIndex;
        String right = (xIndex+1)+":"+yIndex;
        String up_left = (xIndex-1)+":"+(yIndex-1);
        String up_right = (xIndex+1)+":"+(yIndex-1);
        String down_left = (xIndex-1)+":"+(yIndex+1);
        String down_right = (xIndex+1)+":"+(yIndex+1);

        switch (pos){
            case UP:
                if(zones.get(up) != null) zones.get(up).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN:
                if(zones.get(down) != null) zones.get(down).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case LEFT:
                if(zones.get(left) != null) zones.get(left).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case RIGHT:
                if(zones.get(right) != null) zones.get(right).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case UP_LEFT:
                if(zones.get(up) != null) zones.get(up).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(left) != null) zones.get(left).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(up_left) != null) zones.get(up_left).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case UP_RIGHT:
                if(zones.get(up) != null) zones.get(up).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(right) != null) zones.get(right).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(up_right) != null) zones.get(up_right).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN_LEFT:
                if(zones.get(down) != null) zones.get(down).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(left) != null) zones.get(left).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(down_left) != null) zones.get(down_left).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN_RIGHT:
                if(zones.get(down) != null) zones.get(down).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(right) != null) zones.get(right).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(zones.get(down_right) != null) zones.get(down_right).createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;
        }
    }

    /**
     * Add a node to the map given its position (xPos, yPos). Create a link between all nodes within the link radius range.
     * @param xPos
     * @param yPos
     */
    public void add(double xPos, double yPos, int fromSensor){
        // Update information of the map if needed
        if(xPos > maxX) maxX = xPos;
        if(xPos < minX) minX = xPos;
        if(yPos > maxY) maxY = yPos;
        if(yPos < minY) minY = yPos;

        // Information on the point we want to add
        int xIndex = findZoneX(xPos);
        int yIndex = findZoneY(yPos);
        String key = xIndex+":"+yIndex;

        // If the zone doesn't exist, we create it
        if(zones.get(key) == null) zones.put(key, new Zone(xIndex * xZone, yIndex * yZone, xZone, yZone));

        // Creating the position and the node
        Position position = zones.get(key).contains(xPos, yPos);
        Node node = new Node(xPos, yPos, fromSensor);

        // We see if the node is added or not
        boolean added = zones.get(key).add(LINK_RADIUS, ABSORBTION_RADIUS, node);

        // Add to the zone next to it, if needed
        if(added && position != Position.CENTER) createLink(position, xIndex, yIndex, node);
        if(added) nodes.add(node);
    }

    public void add(double xPos, double yPos){
        this.add(xPos, yPos, -1);
    }

    /**
     * Generate the map matrix
     */
    public boolean generateMap(){
        floorX = (int) Math.floor(minX);    // xPos of the most far-left node
        floorY = (int) Math.floor(minY);    // yPos of the most far-up node
        int ceilX = (int) Math.ceil(maxX);      // xPos of the most far-right node
        int ceilY = (int) Math.ceil(maxY);      // yPos of the most far-down node

        width = (int) ((ceilX - floorX) / RATIO) + 1;       // width of the map
        height = (int) ((ceilY - floorY) / RATIO) + 1;      // height of the map

        if(width < 0) return false;

        for(Zone zone : zones.values()){
            for(Node node : zone.getNodes()){
                int x = (int) (Math.round(node.getX() - floorX) / RATIO);
                int y = (int) (Math.round(node.getY() - floorY) / RATIO);
                node.setIndex(x, y);
            }
        }
        return true;
    }

    public int getFloorX(){
        return floorX;
    }

    public int getFloorY(){
        return floorY;
    }

    /**
     * Add a node to the map given the position of the robot (xPos, yPos), its angle (robAngle), the angle of
     * detection of the point angle, and the distance of detection distance. Use Map.RADIAN or Map.DEGREE for the method
     * if the angle are given in radian or degree.
     * @param xPos
     * @param yPos
     * @param robAngle
     * @param angle
     * @param distance
     * @param method
     */
    public void add(double xPos, double yPos, double robAngle, double angle, double distance, double sensor_dist, int fromSensor, int method){
        if(method == DEGREE) {
            angle = (angle + robAngle) % 360;
            angle = 3.141592654 * angle / 180;
            robAngle = 3.141592654 * robAngle / 180;
        }
        double x = xPos + Math.cos(angle) * distance + Math.cos(robAngle) * sensor_dist;
        double y = yPos + Math.sin(angle) * distance + Math.sin(robAngle) * sensor_dist;
        add(x, y, fromSensor);
    }

    /**
     * Return the number of node in the map
     * @return
     */
    int numberOfNode(){
        return nodes.size();
    }

    ArrayList<Node> getNodes(){
        return nodes;
    }

    HashMap<String, Zone> getZones(){
        return zones;
    }

    /**
     * Return the width of the map
     * @return
     */
    int getWidth(){
        return width;
    }

    /**
     * Return the height of the map
     * @return
     */
    int getHeight(){
        return height;
    }

}
