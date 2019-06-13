class Node{
  public:
    double f;
    int x, y;
    Node* next;

    Node(double f, int x, int y){
     this->f = f;
     this->x = x;
     this->y = y; 
    }
};

class Queue{
  private:
    Node* first;
    Node* last;
    int SIZE = 0;

  public:
    Queue();

    bool isEmpty(){
      return SIZE == 0;
    }

    void insert(double f, int x, int y){
      if(isEmpty()) {
        first = new Node(f, x, y);
        last = first;
        SIZE++;
      } else {
        Node* current = first;
        // On regarde si le node est déjà dans la liste
        for(int i = 0; i < SIZE; i++){
          // S'il y est, on met à jour son f
          if(current->x == x && current->y == y) {
            current->f = f;
            return;
          }
        }
        // Sinon on le rajoute à la fin
        last->next = new Node(f, x, y);
      }
    }

    Node* pop(){
      if(isEmpty()) return first;
      Node* item = first;
      first = first->next;
      return item;
    }
    
};
