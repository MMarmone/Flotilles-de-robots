class Memory{
  private:
    int CAPACITY = 10;
    double RADIUS = 25; // (in mm)
    double MINIMUM_DISTANCE = 100; // (in mm)
    double *mem_x, *mem_y, *mem_d;
    bool CLOSED = false;
    int SIZE = 0;
    int FOLLOWING_SIDE;

  public:
    Memory(int side){
      FOLLOWING_SIDE = side;
      mem_x = new double[CAPACITY];
      mem_y = new double[CAPACITY];
      mem_d = new double[CAPACITY];
    }

    // Ajoute un point à la mémoire
    void addMemoryPoint(double x, double y, double distance){
      // Si on a plus assez de place, on ajoute rien
      if(SIZE >= CAPACITY) return;

      // On ajoute le point
      mem_x[SIZE] = x;
      mem_y[SIZE] = y;
      mem_d[SIZE++] = distance;
    }

    // Est-ce qu'on a déjà visité ce point ?
    bool isVisited(double x, double y, double distance){
      for(int i = 0; i < SIZE; i++){
        // Si on a parcouru assez de distance depuis l'ajout du point
        if(distance - mem_d[i] > MINIMUM_DISTANCE){
          // On regarde s'il se situe dans une certaine zone autour du point
          if(x >= mem_x[i] - RADIUS && x <= mem_x[i] + RADIUS && y >= mem_y[i] - RADIUS && y <= mem_y[i] + RADIUS) {
            CLOSED = true;
            return true;
          }
        }
      }
      return false;
    }
  
    void eraseMemory(){
      SIZE = 0;
    }

    void setFollowingSide(int side){
      FOLLOWING_SIDE = side;
    }

    // Distance à laquelle a été ajoutée le dernier point
    double getLastDistance(){
      if(SIZE > 0) return mem_d[SIZE-1];
      return 0;
    }

    // Distance parcourue depuis l'ajout du dernier point
    double distanceSinceLastPoint(double distance){
      return distance - getLastDistance();
    }

    // Est-ce que le circuit a déjà été fermé ? (=> on a fait le tour, isVisited => true)
    bool isClosed(){
      return CLOSED;
    }

    // Retourne le côté qui a été suivi durant le chemin mémorisé
    int getSide(){
      return FOLLOWING_SIDE;
    }
    
};
