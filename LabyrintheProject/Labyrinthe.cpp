#include "httplib.hpp"
#include "Lab.h"
#include "Cellule.h"
#include <iostream>
#include "json.hpp"

int main() {
    httplib::Server svr;

    //Labyrinthe global (r�utilisable entre les requ�tes)
    Lab labyrinthe(40, 16);

    //Variables globales pour start et end
    int start_row = -1, start_col = -1;
    int end_row = -1, end_col = -1;

    //Route POST pour g�rer les actions sur le labyrinthe
    svr.Post("/labyrinthe", [&labyrinthe, &start_row, &start_col, &end_row, &end_col](const httplib::Request& req, httplib::Response& res) {
        try {
            //Parse le corps de la requ�te JSON
            auto body = nlohmann::json::parse(req.body);

            if (!body.contains("action")) {
                res.status = 400;
                res.set_content("Requ�te invalide : champ 'action' manquant.", "text/plain");
                return;
            }

            std::string action = body["action"];

            if (action == "generate") {
                if (!body.contains("start") || !body.contains("end")) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent �tre fournis.", "text/plain");
                    return;
                }

                //Extraire les points de d�part et d'arriv�e
                auto start = body["start"];
                auto end = body["end"];

                start_row = start["row"];
                start_col = start["col"];
                end_row = end["row"];
                end_col = end["col"];

                //Initialisation du labyrinthe avec les points de d�part et d'arriv�e
                labyrinthe.generate(start_row, start_col, end_row, end_col);
                res.set_content(labyrinthe.toJson(), "application/json");
            }
            else if (action == "solve_dijkstra") {
                if (start_row == -1 || start_col == -1 || end_row == -1 || end_col == -1) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent �tre d�finis avant de r�soudre.", "text/plain");
                    return;
                }

                //Ex�cuter Dijkstra avec les points de d�part et d'arriv�e
                labyrinthe.reset();
                labyrinthe.dijkstra(start_row, start_col, end_row, end_col);
                res.set_content(labyrinthe.toJson(), "application/json");
                
            }
            else if (action == "solve_bfs") {
                if (start_row == -1 || start_col == -1 || end_row == -1 || end_col == -1) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent �tre d�finis avant de r�soudre.", "text/plain");
                    return;
                }
 
                //Ex�cuter BFS avec les points de d�part et d'arriv�e
                labyrinthe.reset();
                labyrinthe.bfs(start_row, start_col, end_row, end_col);
                res.set_content(labyrinthe.toJson(), "application/json");
            }
            else {
                res.status = 400;
                res.set_content("Action non reconnue : " + action, "text/plain");
            }
        }
        catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Erreur interne du serveur : " + std::string(e.what()), "text/plain");
        }
        });

    //D�marrer le serveur sur le port 8080
    std::cout << "Serveur d�marr� sur http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
