public class Zone {

    private float x1, y1;   // up left
    private float x2, y2;   // down right

    // Nodes contained in the zone
    private ArrayOfNode nodes;

    public Zone(float x, float y, float width, float height) {
        this.x1 = x;
        this.y1 = y;
        this.x2 = x + width;
        this.y2 = y + height;
        this.nodes = new ArrayOfNode(100);
    }

    /**
     * Return the position of the point (xPos, yPos) in the zone.
     * @param xPos
     * @param yPos
     * @return
     */
    public Position contains(float xPos, float yPos) {
        // if the position is contained within the zone
        if(x1 <= xPos && xPos <= x2 && y1 <= yPos && yPos <= y2){

            // compute the position in the zone
            boolean left = x1 >= xPos - Map.LINK_RADIUS;
            boolean up = y1 >= yPos - Map.LINK_RADIUS;
            boolean right = x2 <= xPos + Map.LINK_RADIUS;
            boolean down = y2 >= yPos + Map.LINK_RADIUS;

            // Corner values
            if(up && left) return Position.UP_LEFT;
            if(up && right) return Position.UP_RIGHT;
            if(down && left) return Position.DOWN_LEFT;
            if(down && right) return Position.DOWN_RIGHT;

            // Border values
            if(up) return Position.UP;
            if(down) return Position.DOWN;
            if(left) return Position.LEFT;
            if(right) return Position.RIGHT;

            // Else it's in the center
            return Position.CENTER;
        }
        // It's not in the zone
        return Position.NONE;
    }

    /**
     * Return true if the node is contained in the zone
     * @param node
     * @return
     */
    public boolean contains(Node node) {
        return nodes.contains(node);
    }

    /**
     * Try to add a node to the zone.
     * Return true if the node is added to the zone, false otherwise.
     * @param linkRadius
     * @param absorbtionRadius
     * @param node
     * @return
     */
    public boolean add(float linkRadius, float absorbtionRadius, Node node) {
        boolean created = createLink(linkRadius, absorbtionRadius, node);
        if(created) nodes.add(node);
        return created;
    }

    /**
     * Create a link between node and all the nodes in the zone, if it's possible.
     * Return true if at least one link is created.
     * @param linkRadius
     * @param absorbtionRadius
     * @param node
     * @return
     */
    public boolean createLink(float linkRadius, float absorbtionRadius, Node node){
        if(node == null) return false;
        Node[] myNodes = nodes.getNodes();
        int nElements = nodes.getSize();

        Node[] linked = new Node[nElements];

        linkRadius = linkRadius * linkRadius;
        absorbtionRadius = absorbtionRadius * absorbtionRadius;
        float distance;
        int nLink = 0;
        boolean absorbed = false;

        // for every node in the zone
        for (int i = 0; i < nElements; i++) {
            distance = node.squareDistance(myNodes[i]);
            // if the distance is shorter than the absorbtion radius, then we absorb it
            if(distance < absorbtionRadius){
                absorbed = true;
                break;
                // if the distance is shorter than the link radius, we add it to the array to create a link later
            } else if(distance < linkRadius) linked[nLink++] = myNodes[i];
        }

        if(!absorbed) {
            for (int i = 0; i < nLink; i++) {
                node.addLinkOut(linked[i]);
                linked[i].addLinkIn(node);
            }
        }
        return !absorbed;
    }
}
