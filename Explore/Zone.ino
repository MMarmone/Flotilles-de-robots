class Zone{
  private:
    float x1, y1;   // en bas à gauche
    float x2, y2;   // en haut à droite
    ArrayOfNode nodes;
    
  public:
    Zone(float x, float y, float width, float height){
      this.x1 = x;
      this.y1 = y;
      this.x2 = x + width;
      this.y2 = y + height;
      this.nodes = new ArrayOfNodes(100);
    }

    // Return true if the point (xPos, yPos) is contained in the zone
    bool contains(float xPos, float yPos){
      return x1 <= xPos && xPos <= x2 && y1 <= yPos && yPos <= y2;
    }
    
    // Return true if the node is contained in the zone
    bool contains(Node node){
      return nodes.contains(node);
    }

    // Add a node to the zone
    // Return true if the node added created a cycle
    bool add(Node node){
      Nodes[] myNodes = nodes.getNodes();
      int nElements = nodes.getSize();
      
      // for every node in the zone
      for(int i = 0; i < nElements; i++){
        // if the node is contained within a distance, create a link between them
        if(myNodes[i].isInRadius(20, node)) {
          node.addLinkOut(myNodes[i]);
          myNodes[i].addLinkIn(node);
        }
      }

      // Add the node to the zone
      nodes.add(node);
      
      return node.isThereCycle();
    }
}
