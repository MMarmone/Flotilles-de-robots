#include <math.h>
#include "Queue.cpp"
#define FREE 0
#define BLOCKED 1
#define UNEXPLORED 2
#define MAX 100000
#define ROW 10
#define COL 10

typedef struct{
  int first, second;
} Pair;

Pair* PATH;

class Cell{
  public:
    int x, y, movementCost;
    Pair parent;
    double estimatedCost, cost;

    Cell(int x, int y){
      this->x = x;
      this->y = y;
      movementCost = MAX;
      estimatedCost = MAX;
      cost = MAX;
    }

    void setParent(int x, int y){
      parent = {x, y};
    }

    Pair getParent(){
      return parent;
    }

    void setMovementCost(int movementCost){
      this->movementCost = movementCost;
      cost = estimatedCost + movementCost;
    }

    void setEstimatedCost(double estimatedCost){
      this->estimatedCost = estimatedCost;
      cost = estimatedCost + movementCost;
    }

    void setAllCosts(int movement, double estimated){
      estimatedCost = estimated;
      movementCost = movement;
      cost = estimatedCost + movementCost;
    }

    int getMovementCost(){
      return movementCost;
    }

    double getCost(){
      return cost;
    }

    double getEstimatedCost(){
      return estimatedCost;
    }
  
};

bool isBlocked(int grid[][COL], int x, int y){
  if(grid[x][y] == BLOCKED) return true;
  return false;
}

bool isValid(int x, int y){
  return x < ROW && x >= 0 && y < COL && y >= 0;
}

bool isDestination(int x, int y, Pair dest){
  return x == dest.first && y == dest.second;
}

double calculateEstimatedCost(int x, int y, Pair dest){
  return (double)sqrt((x-dest.first)*(x-dest.first) + (y-dest.second)*(y-dest.second));
}

Pair* getPath(Cell* cells[][COL], Pair dest, int SIZE){
  Pair* path = new Pair[SIZE];
  int row = dest.first, col = dest.second;
  Pair parent = cells[row][col]->getParent();
  path[SIZE--] = dest;
  
  while(!(parent.first == row && parent.second == col)){
    path[SIZE--] = parent;
    row = parent.first;
    col = parent.second;
    parent = cells[row][col]->getParent();
  }
  
  return path;
}


void aStarSearch(int grid[][COL], Pair src, Pair dest){
  // Source = destination
  if(src.first == dest.first && src.second == dest.second) return;

  // Destination ou source bloqué
  if(grid[src.first][src.second] == BLOCKED) return;
  if(grid[dest.first][dest.second] == BLOCKED) return;


  // Initialisation des cellules
  Cell* cells[ROW][COL];
  int i, j;
  for(i = 0; i < ROW; i++){
    for(j = 0; j < COL; j++){
      cells[i][j] = new Cell(i, j);
    }
  }

  // Cellules visitées
  bool closedList[ROW][COL];
  // Cellules a visiter
  Queue openList;

  // Initialisation de la file
  i = src.first;
  j = src.second;
  cells[i][j]->setAllCosts(0, 0);
  cells[i][j]->setParent(i, j);
  openList.insert(0, i, j);

  // Tant qu'on a pas fini de visiter
  while(!openList.isEmpty()){
    
    // On récupère le premier élement de la file
    Node* current = openList.pop();
    i = current->x;
    j = current->y;
    closedList[i][j] = true;

    // Nouveau coûts
    double nEstimatedCost, nCost;
    int nMovementCost;

    int neighbors_x[4] = {i-1, i+1, i, i};
    int neighbors_y[4] = {j, j, j-1, j+1};

    // Pour tous les voisins
    for(int n = 0; n < 4; n++){
      // Voisin de coordonnées {nX, nY}
      int nX = neighbors_x[n], nY = neighbors_y[n];

      // Si la case existe
      if(isValid(nX, nY)){
        
        // Si c'est notre objectif
        if(isDestination(nX, nY, dest)){
          cells[nX][nY]->setParent(i, j);
          
          // On calcule le chemin (stocké dans une variable global
          PATH = getPath(cells, dest, cells[i][j]->getMovementCost() + 1);
          return;

        // Si ce n'est pas notre objectif, qu'il n'a pas déjà été visité et qu'il est accessible
        } else if (!closedList[nX][nY] && !isBlocked(grid, nX, nY)){
          // Calcul des nouveaux coûts
          nMovementCost = cells[i][j]->getMovementCost() + 1;
          nEstimatedCost = calculateEstimatedCost(nX, nY, dest);
          nCost = nMovementCost + nEstimatedCost;

          // Si la cellule n'a pas de coût où que le nouveau coût est inférieur
          if(cells[nX][nY]->getCost() == MAX || cells[nX][nY]->getCost() > nCost){
            // On ajoute la cellule dans la liste à visiter
            openList.insert(nCost, nX, nY);

            // On met à jours les distances et le parent
            cells[nX][nY]->setAllCosts(nMovementCost, nEstimatedCost);
            cells[nX][nY]->setParent(i,j);
          }
        }
      }
    }
  }
  // Pas de chemin !
}
