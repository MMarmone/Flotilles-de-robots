class Node{
  private:
    float x, y;
    ArrayOfNode in;
    ArrayOfNode out;
  
  public:
    // Constructor
    Node(float x, float y){
      this.x = x;
      this.y = y;
      this.in = new ArrayOfNode(100);
      this.out = new ArrayOfNode(100);
    }
  
    // Add a link between the two nodes this -> node
    void addLinkOut(Node node){
      if(!isLinkOut(node)) out.add(node);
    }

    // Add a link between the two nodes node -> this
    void addLinkIn(Node node){
      if(!isLinkIn(node)) out.add(node);
    }

    // Return true if there is a link between the two nodes this -> node
    bool isLinkOut(Node node){
      return out.contains(node);
    }

    // Return true if there is a link between the two nodes node -> this
    bool isLinkIn(Node node){
      return in.contains(node);
    }

    // Return true if the nodes are equals
    bool isEquals(Node node){
      return x == node.x && y == node.y;
    }

    float getX(){ return x; }
    float getY(){ return y; }

    // Return true if the point (xPos, yPos) is within the radius specified
    bool isInRadius(float radius, float xPos, float yPos){
      radius = radius * radius;
      xPos -= x;
      yPos -= y;
      float distance = xPos*xPos + yPos*yPos;
      return distance < radius;
    }

    // Return true if the node is contained within the radius
    bool isInRadius(float radius, Node node){
      return isInRadius(radius, node.getX(), node.getY());
    }

    // Return the number of link incoming (node -> this)
    int getInLink(){
      return in.getSize();
    }

    // Return the number of link outcoming (this -> node)
    int getOutLink(){
      return out.getSize();
    }

    // Return true if the node is a source
    bool isSource(){
      return in.getSize() == 0 && out.getSize() > 0;
    }

    // Return true if the node is a sink
    bool isSink(){
      return out.getSize() == 0 && in.getSize() > 0;
    }

    // Return true of the node is connected to a sink
    bool isConnectedToSink(){
      Node[] outNodes = out.getNodes();
      int nOut = out.getSize();

      for(int i = 0; i < nOut; i++){
        if(outNodes[i].isSink()) return true;
      }
      return false;
    }

    // Return true if the node is closing a cycle
    bool isThereCycle(){
      return getOutLink() > 1 && isSource() && isConnectedToSink();
    }
    
}
