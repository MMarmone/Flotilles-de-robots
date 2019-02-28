class ArrayOfNode{
  private:
    int nElements;
    int capacity;
    
    Node[] nodes;

    // Double the capacity of the array
    void increaseCapacity(){
      Node[] new_nodes = new Nodes[capacity+capacity];
      for(int i = 0; i < capacity; i++) new_nodes[i] = nodes[i];
      capacity += capacity;
      nodes = new_nodes;
    }
    
  public:
    // Constructor
    ArrayOfNode(int capacity){
      this.nElements = 0;
      this.capacity = capacity;
      this.nodes = new Nodes[capacity];
    }

    // Add a node to the array
    void add(Node node){
      if(nElements >= capacity) increaseCapacity();
      nodes[nElements++] = node;
    }

    // Return true if the array contains the node
    bool contains(Node node){
      for(int i = 0; i < nElements; i++){
        if(nodes[i].isEquals(node)) return true;
      }
      return false;
    }

    // Return the array of nodes
    Node[] getNodes(){
      return nodes;
    }

    // Return the size of the array
    int getSize(){
      return nElements;
    }
}
