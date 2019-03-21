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
    private static final double RATIO = 8;


    /**************************
     * OBJECT
     **************************/

    // Dimension of the map
    private double width, height;
    // Number of zone in the map
    private int nZone;

    // Zones
    private Zone[] zones;
    // Number of zone in the x-axis and y-axis
    private int xZone, yZone;

    // Map converted to an array of int
    private int[][] map;

    // The set of nodes in the map
    private ArrayOfNode nodes;


    // Constructor
    public Map(double width, double height, double wZone, double hZone){
        this.width = width;
        this.height = height;

        double a = width / wZone;
        double b = height / hZone;

        xZone = (int) Math.ceil(a);
        yZone = (int) Math.ceil(b);

        nZone = xZone*yZone;
        zones = new Zone[nZone];

        nodes = new ArrayOfNode(1000);

        for(int i = 0; i < xZone; i++){
            for(int j = 0; j < yZone; j++){
                zones[i+j*xZone] = new Zone(i * wZone, j * hZone, wZone, hZone);
            }
        }

        init();
    }


    /**
     * Create a link between the node node and all the nodes from the zone, depending on the position
     * @param pos
     * @param index
     * @param node
     */
    private void createLink(Position pos, int index, Node node){
        int up = index - yZone, down = index + yZone, left = index - 1, right = index + 1;
        int up_right = index - yZone + 1, up_left = index - yZone - 1, down_left = index + yZone - 1, down_right = index + yZone + 1;

        switch (pos){
            case UP:
                if(up >= 0 && up < nZone) zones[up].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN:
                if(down >= 0 && down < nZone) zones[down].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case LEFT:
                if(left >= 0 && left < nZone) zones[left].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case RIGHT:
                if(right >= 0 && right < nZone) zones[right].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case UP_LEFT:
                if(up >= 0 && up < nZone) zones[up].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(left >= 0 && left < nZone) zones[left].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(up_left >= 0 && up_left < nZone) zones[up_left].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case UP_RIGHT:
                if(up >= 0 && up < nZone) zones[up].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(right >= 0 && right < nZone) zones[right].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(up_right >= 0 && up_right < nZone) zones[up_right].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN_LEFT:
                if(down >= 0 && down < nZone) zones[down].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(left >= 0 && left < nZone) zones[left].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(down_left >= 0 && down_left < nZone) zones[down_left].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;

            case DOWN_RIGHT:
                if(down >= 0 && down < nZone) zones[down].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(right >= 0 && right < nZone) zones[right].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                if(down_right >= 0 && down_right < nZone) zones[down_right].createLink(LINK_RADIUS, ABSORBTION_RADIUS, node);
                break;
        }
    }

    /**
     * Add a node to the map given its position (xPos, yPos). Create a link between all nodes within the link radius range.
     * @param xPos
     * @param yPos
     */
    public void add(double xPos, double yPos){
        Position position;
        Node node = null;
        boolean added = false;

        for(int i = 0; i < nZone; i++){
            position = zones[i].contains(xPos, yPos);

            // If the node must be inside the zone i.
            if(position != Position.NONE) {
                node = new Node(xPos, yPos);
                added = zones[i].add(LINK_RADIUS, ABSORBTION_RADIUS, node);

                // Add to the zone next to it, if needed
                if(added && position != Position.CENTER) createLink(position, i, node);
            }
        }

        // If the node is added to the zone, then we add it to the map
        if(added) {
            nodes.add(node);
            addPoint(node);
        }
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
    public void add(double xPos, double yPos, double robAngle, double angle, double distance, int method){
        if(method == DEGREE) {
            angle = (angle + robAngle) % 360;
            angle = 3.141592654 * angle / 180;
        }
        double x = xPos + Math.cos(angle) * distance;
        double y = yPos + Math.sin(angle) * distance;
        add(x, y);
    }

    /**
     * Return the number of node in the map
     * @return
     */
    int numberOfNode(){
        return nodes.getSize();
    }

    /**
     * Return the width of the map
     * @return
     */
    double getWidth(){
        return width;
    }

    /**
     * Return the height of the map
     * @return
     */
    double getHeight(){
        return height;
    }

    /**
     * Initialiaze the map
     */
    private void init(){
        int width = (int) Math.round(getWidth() / RATIO) + 1;
        int height = (int) Math.round(getHeight() / RATIO) + 1;
        map = new int[width][height];
        for (int w = 0; w < width; w++) {
            for (int h = 0; h < height; h++) map[w][h] = 0;
        }
    }

    /**
     * Add a point to the map
     * @param node
     */
    private void addPoint(Node node){
        int xPos = (int) Math.round(node.getX() / RATIO);
        int yPos = (int) Math.round(node.getY() / RATIO);
        map[xPos][yPos] = 1;
    }

    /**
     * Return the map
     * @return
     */
    public int[][] getMap(){
        return map;
    }

}
