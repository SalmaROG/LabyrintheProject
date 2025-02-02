#include "Cellule.h"
#include<climits>

Cellule::Cellule(int x, int y) : x(x), y(y), type(WALL), dist(INT_MAX), parent(nullptr) {
}

int Cellule::getX()
{
    return x;
}

int Cellule::getY()
{
    return y;
}

void Cellule::setType(CellType t) {
    type = t;
}

CellType Cellule::getType() const {
    return type;
}

int Cellule::getDist() const {
    return dist;
}

void Cellule::setDist(int d) {
    dist = d;
}

Cellule* Cellule::getParent() const {
    return parent;
}

void Cellule::setParent(Cellule* p) {
    parent = p;
}
