#include "httplib.hpp"
#include "Lab.h"
#include "Cellule.h"
#include <iostream>
#include "json.hpp"

int main() {
    httplib::Server svr;

    //Labyrinthe global (réutilisable entre les requêtes)
    Lab labyrinthe(40, 16);

    //Variables globales pour start et end
    int start_row = -1, start_col = -1;
    int end_row = -1, end_col = -1;

    //Route POST pour gérer les actions sur le labyrinthe
    svr.Post("/labyrinthe", [&labyrinthe, &start_row, &start_col, &end_row, &end_col](const httplib::Request& req, httplib::Response& res) {
        try {
            //Parse le corps de la requête JSON
            auto body = nlohmann::json::parse(req.body);

            if (!body.contains("action")) {
                res.status = 400;
                res.set_content("Requête invalide : champ 'action' manquant.", "text/plain");
                return;
            }

            std::string action = body["action"];

            if (action == "generate") {
                if (!body.contains("start") || !body.contains("end")) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent être fournis.", "text/plain");
                    return;
                }

                //Extraire les points de départ et d'arrivée
                auto start = body["start"];
                auto end = body["end"];

                start_row = start["row"];
                start_col = start["col"];
                end_row = end["row"];
                end_col = end["col"];

                //Initialisation du labyrinthe avec les points de départ et d'arrivée
                labyrinthe.generate(start_row, start_col, end_row, end_col);
                res.set_content(labyrinthe.toJson(), "application/json");
            }
            else if (action == "solve_dijkstra") {
                if (start_row == -1 || start_col == -1 || end_row == -1 || end_col == -1) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent être définis avant de résoudre.", "text/plain");
                    return;
                }

                //Exécuter Dijkstra avec les points de départ et d'arrivée
                labyrinthe.reset();
                labyrinthe.dijkstra(start_row, start_col, end_row, end_col);
                res.set_content(labyrinthe.toJson(), "application/json");
                
            }
            else if (action == "solve_bfs") {
                if (start_row == -1 || start_col == -1 || end_row == -1 || end_col == -1) {
                    res.status = 400;
                    res.set_content("Les points 'start' et 'end' doivent être définis avant de résoudre.", "text/plain");
                    return;
                }
 
                //Exécuter BFS avec les points de départ et d'arrivée
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

    //Démarrer le serveur sur le port 8080
    std::cout << "Serveur démarré sur http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
