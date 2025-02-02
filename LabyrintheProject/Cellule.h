#pragma once


enum CellType {
    WALL,   //Mur
    START,  //Départ
    END,    //Arrivée
    PATH,   //Partie du chemin
    EMPTY   //Espace libre
};

class Cellule {

private:
    int x, y;           //Coordonnées de la cellule
    CellType type;      //Type de la cellule (mur, départ, arrivée, chemin, vide)
    int dist;           //Distance du point de départ
    Cellule* parent;    //Cellule parente pour reconstituer le chemin

public:
    //Constructeur
    Cellule(int x, int y);

    //Getter les coordonnées
    int getX();
    int getY();
    
    //Méthode pour définir le type
    void setType(CellType type);  

    //Méthode pour récupérer le type
    CellType getType() const;   

    //Getter et setter pour la distance
    int getDist() const;
    void setDist(int dist);

    //Getter et setter pour le parent
    Cellule* getParent() const;
    void setParent(Cellule* parent);


};


