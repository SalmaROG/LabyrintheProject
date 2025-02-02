#include "Lab.h"
#include <queue>

Lab::Lab(int w, int h) : width(w), height(h)
{
    // Création de la grille du labyrinthe
    for (int i = 0; i < height; i++) {
        vector<Cellule> row;
        for (int j = 0; j < width; j++) {
            row.push_back(Cellule(i, j));
        }
        grid.push_back(row);
    }
}

void Lab::generate(int startX, int startY, int endX, int endY) {
    //Initialiser la grille sans mur autour
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // +Initialiser comme mur partout sauf le départ et l'arrivée
            grid[i][j].setType(WALL); //Initialiser comme mur
        }
    }

    //Départ et arrivée
    grid[startX][startY].setType(START);
    grid[endX][endY].setType(END);

    //Créer un chemin entre le départ et l'arrivée
    srand(time(0));
    int x = startX, y = startY; //Commencer à la position de départ

    //Générer un chemin jusqu'à la fin pour assurer qu'il y a au moins un chemin entre départ et l'arrivé
    while (x != endX || y != endY) {
        //Décider aléatoirement de la direction (bas ou droite)
        if (x < endX && (rand() % 2 == 0 || y == endY)) {
            x++; // Aller vers le bas
        }
        else if (y < endY) {
            y++; //Aller vers la droite
        }

        //Ouvrir le chemin
        grid[x][y].setType(EMPTY); //Ouverture du chemin
    }

    //Ajoutez des chemins aléatoires autour
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            //Ne pas modifier le départ, l'arrivée ou les murs déjà ouverts
            if (grid[i][j].getType() == START || grid[i][j].getType() == END || grid[i][j].getType() == EMPTY) {
                continue;
            }

            //Ouvrir certaines cellules de manière aléatoire
            bool isPath = (rand() % 3 != 0); //2/3 de chance de devenir un chemin
            grid[i][j].setType(isPath ? EMPTY : WALL); //Choisir entre chemin ou mur
        }
    }

    //Réaffirmer les propriétés de départ et d'arrivée
    grid[startX][startY].setType(START); // Départ
    grid[endX][endY].setType(END); // Arrivée
}



const vector<vector<Cellule>>& Lab::getGrid() const
{
    return grid;
}


void Lab::reset() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j].getType() == PATH) {
                grid[i][j].setType(EMPTY); //Efface uniquement les chemins trouvés
            }
        }
    }
}

void Lab::dijkstra(int startX, int startY, int endX, int endY) {
    //Dimensions de la grille
    const int dx[] = { 1, -1, 0, 0 }; //Déplacement vertical
    const int dy[] = { 0, 0, 1, -1 }; //Déplacement horizontal

    //Matrice pour stocker les distances (initialisées à un maximum)
    vector<vector<int>> dist(height, vector<int>(width, numeric_limits<int>::max()));
    vector<vector<Cellule*>> prev(height, vector<Cellule*>(width, nullptr));

    //File de priorité pour Dijkstra (distance, x, y)
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    //Initialisation du point de départ
    dist[startX][startY] = 0;
    pq.push({ 0, startX, startY });

    //Parcours de la grille avec Dijkstra
    while (!pq.empty()) {
        auto [currentDist, x, y] = pq.top();
        pq.pop();

        //Si la distance est déjà dépassée, ignorer
        if (currentDist > dist[x][y]) continue;

        //Si l'on atteint le point d'arrivée, on peut arrêter
        if (x == endX && y == endY) break;

        //Parcourir les voisins
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];

            //Vérifier les limites et les types de cellules
            if (nx >= 0 && ny >= 0 && nx < height && ny < width &&
                grid[nx][ny].getType() != WALL) {
                int newDist = currentDist + 1;

                //Mettre à jour si une meilleure distance est trouvée
                if (newDist < dist[nx][ny]) {
                    dist[nx][ny] = newDist;
                    prev[nx][ny] = &grid[x][y]; // Mémoriser le chemin
                    pq.push({ newDist, nx, ny });
                }
            }
        }
    }

    //Reconstituer le chemin en remontant depuis l'arrivée
    Cellule* current = &grid[endX][endY];

    while (current) {
        int cx = current->getX();
        int cy = current->getY();

        // Ne pas marquer les cases de départ et d'arrivée
        if (grid[cx][cy].getType() != START && grid[cx][cy].getType() != END) {
            grid[cx][cy].setType(PATH);  //Marquer le chemin trouvé
        }

        current = prev[cx][cy]; //Remonter dans le chemin
    }
}

void Lab::bfs(int startX, int startY, int endX, int endY)
{
    //Déplacements possibles : haut, bas, gauche, droite
    const int dx[] = { 1, -1, 0, 0 }; //Déplacement vertical
    const int dy[] = { 0, 0, 1, -1 }; //Déplacement horizontal

    //Matrice pour garder la trace des cellules visitées
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    //Matrice pour suivre les parents (pour reconstruire le chemin)
    vector<vector<tuple<int, int>>> parent(height, vector<tuple<int, int>>(width));

    //File pour la recherche en largeur (BFS)
    queue<tuple<int, int>> q;

    //Vérifier si la case départ et la case arrivée sont valides
    if (grid[startX][startY].getType() == WALL || grid[endX][endY].getType() == WALL) {
        cerr << "Erreur : départ ou arrivée dans un mur!" << endl;
        return;
    }

    visited[startX][startY] = true;
    q.push({ startX, startY });

    //Parcours de la grille avec BFS
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        //Si on atteint la case d'arrivée, on arrête la recherche
        if (x == endX && y == endY) {
            cout << "Arrivée atteinte !" << endl;
            break;
        }

        //Parcourir les voisins
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];

            //Vérifier les limites et s'assurer que la cellule n'est pas un mur et n'a pas été visitée
            if (nx >= 0 && ny >= 0 && nx < height && ny < width &&
                grid[nx][ny].getType() != WALL && !visited[nx][ny]) {

                visited[nx][ny] = true;
                parent[nx][ny] = { x, y };  //Sauvegarder le parent pour retracer le chemin
                q.push({ nx, ny });
            }
        }
    }

    //Retracer le plus court chemin depuis l'arrivée
    int x = endX, y = endY;
    while (x != startX || y != startY) {
        //Marquer la cellule comme faisant partie du chemin
        grid[x][y].setType(PATH);

        //Remonter vers le parent
        auto [px, py] = parent[x][y];
        x = px;
        y = py;
    }

    //Marquer la cellule de départ comme faisant partie du chemin
    grid[startX][startY].setType(PATH);
}






std::string Lab::toJson() const
{
    std::string json = "[";
    for (int i = 0; i < height; ++i) {
        json += "[";
        for (int j = 0; j < width; ++j) {
            const auto& cell = grid[i][j];
            switch (cell.getType()) {
            case START:
                json += "\"S\""; //Départ
                break;
            case END:
                json += "\"E\""; //Arrivée
                break;
            case WALL:
                json += "\"#\""; //Mur
                break;
            case EMPTY:
                json += "\".\""; //Chemin
                break;
            case PATH:
                json += "\"P\""; //Chemin trouvé par Dijkstra
                break;
            default:
                json += "\"?\""; //Pour les types inconnus
            }

            if (j < width - 1) json += ", "; //Ajouter une virgule entre les éléments
        }
        json += "]";
        if (i < height - 1) json += ", "; //Ajouter une virgule entre les lignes
    }
    json += "]";
    return json;
}

