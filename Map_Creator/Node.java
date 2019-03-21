import java.util.ArrayList;

public class Node {

    // Position of the node in space
    private double x, y;
    // Set of nodes that have a link in (node -> this)
    private ArrayList<Node> in;
    // Set of nodes that have a link out (this -> node)
    private ArrayList<Node> out;

    // Constructor
    public Node(double x, double y){
        this.x = x;
        this.y = y;
        this.in = new ArrayList<>();
        this.out = new ArrayList<>();
    }

    // GETTERS
    double getX(){ return x; }
    double getY(){ return y; }

    /**
     * Return the number of link incoming (node -> this)
     * @return
     */
    public int getInDegree(){
        return in.size();
    }

    /**
     * Return the number of link outcoming (this -> node)
     * @return
     */
    public int getOutDegree(){
        return out.size();
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
    public boolean isInRadius(double radius, double xPos, double yPos){
        radius = radius * radius;
        return squareDistance(xPos, yPos) < radius;
    }

    /**
     * Return true if the node is contained within the radius
     * @param radius
     * @param node
     * @return
     */
    public boolean isInRadius(double radius, Node node){
        return isInRadius(radius, node.getX(), node.getY());
    }

    /**
     * Square distance from this node to point (xPos, yPos)
     * @param xPos
     * @param yPos
     * @return
     */
    public double squareDistance(double xPos, double yPos){
        double X = (x - xPos);
        double Y = (y - yPos);
        return X*X + Y*Y;
    }

    /**
     * Square distance from this to node
     * @param node
     * @return
     */
    public double squareDistance(Node node){
        return squareDistance(node.getX(), node.getY());
    }

}
