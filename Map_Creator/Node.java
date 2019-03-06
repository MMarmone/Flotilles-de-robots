public class Node {

    // Position of the node in space
    private float x, y;
    // Set of nodes that have a link in (node -> this)
    private ArrayOfNode in;
    // Set of nodes that have a link out (this -> node)
    private ArrayOfNode out;

    // Constructor
    public Node(float x, float y){
        this.x = x;
        this.y = y;
        this.in = new ArrayOfNode(100);
        this.out = new ArrayOfNode(100);
    }

    // GETTERS
    float getX(){ return x; }
    float getY(){ return y; }
    Node[] getOutLink(){ return out.getNodes(); }
    Node[] getInLink(){ return in.getNodes(); }

    /**
     * Return the number of link incoming (node -> this)
     * @return
     */
    public int getInDegree(){
        return in.getSize();
    }

    /**
     * Return the number of link outcoming (this -> node)
     * @return
     */
    public int getOutDegree(){
        return out.getSize();
    }

    /**
     * Add a link between the two nodes this -> node
     * @param node
     */
    public void addLinkOut(Node node){
        if(!isLinkOut(node)) out.add(node);
    }

    /**
     * Add a link between the two nodes node -> this
     * @param node
     */
    public void addLinkIn(Node node){
        if(!isLinkIn(node)) out.add(node);
    }

    /**
     * Return true if there is a link between the two nodes this -> node
     * @param node
     * @return
     */
    public boolean isLinkOut(Node node){
        return out.contains(node);
    }

    /**
     * Return true if there is a link between the two nodes node -> this
     * @param node
     * @return
     */
    public boolean isLinkIn(Node node){
        return in.contains(node);
    }

    /**
     * Return true if the nodes are equals
     * @param node
     * @return
     */
    public boolean isEquals(Node node){
        return this == node;
    }

    /**
     * Return true if the point (xPos, yPos) is within the radius specified
     * @param radius
     * @param xPos
     * @param yPos
     * @return
     */
    public boolean isInRadius(float radius, float xPos, float yPos){
        radius = radius * radius;
        return squareDistance(xPos, yPos) < radius;
    }

    /**
     * Return true if the node is contained within the radius
     * @param radius
     * @param node
     * @return
     */
    public boolean isInRadius(float radius, Node node){
        return isInRadius(radius, node.getX(), node.getY());
    }

    /**
     * Square distance from this node to point (xPos, yPos)
     * @param xPos
     * @param yPos
     * @return
     */
    public float squareDistance(float xPos, float yPos){
        float X = (x - xPos);
        float Y = (y - yPos);
        return X*X + Y*Y;
    }

    /**
     * Square distance from this to node
     * @param node
     * @return
     */
    public float squareDistance(Node node){
        return squareDistance(node.getX(), node.getY());
    }

}
