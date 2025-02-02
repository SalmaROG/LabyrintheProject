#pragma once 
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Cellule.h" 

using namespace std;

class Lab {
private:
    int width;
    int height;                      
    vector<vector<Cellule>> grid;

public:
    //Constructeur
    Lab(int w, int h);

    //Générer labyrinthe
    void generate(int startX, int startY, int endX, int endY);

    //Retourner Gird
    const vector<vector<Cellule>>& getGrid() const;

    //Réinitialiser labyrinthe
    void reset();

    //Convertir le labyrinthe en chaîne JSON
    std::string toJson() const;

    //Algorithme bfs
    void bfs(int startX, int startY, int endX, int endY);

    //Algorithme dijkstra
    void dijkstra(int startX, int startY, int endX, int endY);

};