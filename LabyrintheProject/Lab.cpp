#include "Lab.h"
#include <queue>

Lab::Lab(int w, int h) : width(w), height(h)
{
    // Cr�ation de la grille du labyrinthe
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
            // +Initialiser comme mur partout sauf le d�part et l'arriv�e
            grid[i][j].setType(WALL); //Initialiser comme mur
        }
    }

    //D�part et arriv�e
    grid[startX][startY].setType(START);
    grid[endX][endY].setType(END);

    //Cr�er un chemin entre le d�part et l'arriv�e
    srand(time(0));
    int x = startX, y = startY; //Commencer � la position de d�part

    //G�n�rer un chemin jusqu'� la fin pour assurer qu'il y a au moins un chemin entre d�part et l'arriv�
    while (x != endX || y != endY) {
        //D�cider al�atoirement de la direction (bas ou droite)
        if (x < endX && (rand() % 2 == 0 || y == endY)) {
            x++; // Aller vers le bas
        }
        else if (y < endY) {
            y++; //Aller vers la droite
        }

        //Ouvrir le chemin
        grid[x][y].setType(EMPTY); //Ouverture du chemin
    }

    //Ajoutez des chemins al�atoires autour
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            //Ne pas modifier le d�part, l'arriv�e ou les murs d�j� ouverts
            if (grid[i][j].getType() == START || grid[i][j].getType() == END || grid[i][j].getType() == EMPTY) {
                continue;
            }

            //Ouvrir certaines cellules de mani�re al�atoire
            bool isPath = (rand() % 3 != 0); //2/3 de chance de devenir un chemin
            grid[i][j].setType(isPath ? EMPTY : WALL); //Choisir entre chemin ou mur
        }
    }

    //R�affirmer les propri�t�s de d�part et d'arriv�e
    grid[startX][startY].setType(START); // D�part
    grid[endX][endY].setType(END); // Arriv�e
}



const vector<vector<Cellule>>& Lab::getGrid() const
{
    return grid;
}


void Lab::reset() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j].getType() == PATH) {
                grid[i][j].setType(EMPTY); //Efface uniquement les chemins trouv�s
            }
        }
    }
}

void Lab::dijkstra(int startX, int startY, int endX, int endY) {
    //Dimensions de la grille
    const int dx[] = { 1, -1, 0, 0 }; //D�placement vertical
    const int dy[] = { 0, 0, 1, -1 }; //D�placement horizontal

    //Matrice pour stocker les distances (initialis�es � un maximum)
    vector<vector<int>> dist(height, vector<int>(width, numeric_limits<int>::max()));
    vector<vector<Cellule*>> prev(height, vector<Cellule*>(width, nullptr));

    //File de priorit� pour Dijkstra (distance, x, y)
    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> pq;

    //Initialisation du point de d�part
    dist[startX][startY] = 0;
    pq.push({ 0, startX, startY });

    //Parcours de la grille avec Dijkstra
    while (!pq.empty()) {
        auto [currentDist, x, y] = pq.top();
        pq.pop();

        //Si la distance est d�j� d�pass�e, ignorer
        if (currentDist > dist[x][y]) continue;

        //Si l'on atteint le point d'arriv�e, on peut arr�ter
        if (x == endX && y == endY) break;

        //Parcourir les voisins
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];

            //V�rifier les limites et les types de cellules
            if (nx >= 0 && ny >= 0 && nx < height && ny < width &&
                grid[nx][ny].getType() != WALL) {
                int newDist = currentDist + 1;

                //Mettre � jour si une meilleure distance est trouv�e
                if (newDist < dist[nx][ny]) {
                    dist[nx][ny] = newDist;
                    prev[nx][ny] = &grid[x][y]; // M�moriser le chemin
                    pq.push({ newDist, nx, ny });
                }
            }
        }
    }

    //Reconstituer le chemin en remontant depuis l'arriv�e
    Cellule* current = &grid[endX][endY];

    while (current) {
        int cx = current->getX();
        int cy = current->getY();

        // Ne pas marquer les cases de d�part et d'arriv�e
        if (grid[cx][cy].getType() != START && grid[cx][cy].getType() != END) {
            grid[cx][cy].setType(PATH);  //Marquer le chemin trouv�
        }

        current = prev[cx][cy]; //Remonter dans le chemin
    }
}

void Lab::bfs(int startX, int startY, int endX, int endY)
{
    //D�placements possibles : haut, bas, gauche, droite
    const int dx[] = { 1, -1, 0, 0 }; //D�placement vertical
    const int dy[] = { 0, 0, 1, -1 }; //D�placement horizontal

    //Matrice pour garder la trace des cellules visit�es
    vector<vector<bool>> visited(height, vector<bool>(width, false));

    //Matrice pour suivre les parents (pour reconstruire le chemin)
    vector<vector<tuple<int, int>>> parent(height, vector<tuple<int, int>>(width));

    //File pour la recherche en largeur (BFS)
    queue<tuple<int, int>> q;

    //V�rifier si la case d�part et la case arriv�e sont valides
    if (grid[startX][startY].getType() == WALL || grid[endX][endY].getType() == WALL) {
        cerr << "Erreur : d�part ou arriv�e dans un mur!" << endl;
        return;
    }

    visited[startX][startY] = true;
    q.push({ startX, startY });

    //Parcours de la grille avec BFS
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        //Si on atteint la case d'arriv�e, on arr�te la recherche
        if (x == endX && y == endY) {
            cout << "Arriv�e atteinte !" << endl;
            break;
        }

        //Parcourir les voisins
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i], ny = y + dy[i];

            //V�rifier les limites et s'assurer que la cellule n'est pas un mur et n'a pas �t� visit�e
            if (nx >= 0 && ny >= 0 && nx < height && ny < width &&
                grid[nx][ny].getType() != WALL && !visited[nx][ny]) {

                visited[nx][ny] = true;
                parent[nx][ny] = { x, y };  //Sauvegarder le parent pour retracer le chemin
                q.push({ nx, ny });
            }
        }
    }

    //Retracer le plus court chemin depuis l'arriv�e
    int x = endX, y = endY;
    while (x != startX || y != startY) {
        //Marquer la cellule comme faisant partie du chemin
        grid[x][y].setType(PATH);

        //Remonter vers le parent
        auto [px, py] = parent[x][y];
        x = px;
        y = py;
    }

    //Marquer la cellule de d�part comme faisant partie du chemin
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
                json += "\"S\""; //D�part
                break;
            case END:
                json += "\"E\""; //Arriv�e
                break;
            case WALL:
                json += "\"#\""; //Mur
                break;
            case EMPTY:
                json += "\".\""; //Chemin
                break;
            case PATH:
                json += "\"P\""; //Chemin trouv� par Dijkstra
                break;
            default:
                json += "\"?\""; //Pour les types inconnus
            }

            if (j < width - 1) json += ", "; //Ajouter une virgule entre les �l�ments
        }
        json += "]";
        if (i < height - 1) json += ", "; //Ajouter une virgule entre les lignes
    }
    json += "]";
    return json;
}

