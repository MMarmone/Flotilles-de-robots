public class ArrayOfNode {

    // Number of node in the array
    private int nElements;
    // Capacity of the array
    private int capacity;
    // The array of nodes
    private Node[] nodes;

    // Constructor
    public ArrayOfNode(int capacity){
        this.nElements = 0;
        this.capacity = capacity;
        this.nodes = new Node[capacity];
    }

    /**
     * Double the capacity of the array
     */
    private void increaseCapacity(){
        Node[] new_nodes = new Node[capacity+capacity];
        for(int i = 0; i < capacity; i++) new_nodes[i] = nodes[i];
        capacity += capacity;
        nodes = new_nodes;
    }

    /**
     * Add a node to the array
     * @param node
     */
    public void add(Node node){
        if(nElements >= capacity) increaseCapacity();
        if(!contains(node)) nodes[nElements++] = node;
    }

    /**
     * Return true if the array contains the node
     * @param node
     * @return
     */
    public boolean contains(Node node){
        for(int i = 0; i < nElements; i++){
            if(nodes[i].isEquals(node)) return true;
        }
        return false;
    }

    /**
     * Return the array of nodes
     * @return
     */
    public Node[] getNodes(){
        return nodes;
    }

    /**
     * Return the number of element in the array
     * @return
     */
    public int getSize(){
        return nElements;
    }
}
