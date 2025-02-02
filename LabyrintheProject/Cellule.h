#pragma once


enum CellType {
    WALL,   //Mur
    START,  //D�part
    END,    //Arriv�e
    PATH,   //Partie du chemin
    EMPTY   //Espace libre
};

class Cellule {

private:
    int x, y;           //Coordonn�es de la cellule
    CellType type;      //Type de la cellule (mur, d�part, arriv�e, chemin, vide)
    int dist;           //Distance du point de d�part
    Cellule* parent;    //Cellule parente pour reconstituer le chemin

public:
    //Constructeur
    Cellule(int x, int y);

    //Getter les coordonn�es
    int getX();
    int getY();
    
    //M�thode pour d�finir le type
    void setType(CellType type);  

    //M�thode pour r�cup�rer le type
    CellType getType() const;   

    //Getter et setter pour la distance
    int getDist() const;
    void setDist(int dist);

    //Getter et setter pour le parent
    Cellule* getParent() const;
    void setParent(Cellule* parent);


};


