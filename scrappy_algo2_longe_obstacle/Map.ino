class Map{
  private:
    float anchorStep;
    float width, height;  // width and height of the room
    int nZones;           // number of zone in the room
    Zone[] zones;         // zones in the room

  public:
    Map(float width, float height, int nZone, float anchor){
      this.width = width;
      this.height = height;
      this.nZones = nZone;
      this.anchorStep = anchor;

      // not finished
    }
}
