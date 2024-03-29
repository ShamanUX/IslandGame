#ifndef GAMEBOARD_HH
#define GAMEBOARD_HH

#include <map>
#include <unordered_map>

#include <QDialog>
#include "pawngraphics.hh"
#include "pixmapgraphics.hh"
#include "hexgraphics.hh"
#include "igameboard.hh"
#include "vector"
#include <QPointF>
#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QBrush>
#include "math.h"
#include "player.hh"
#include "pixmapgraphics.hh"
#include "infobox.hh"

namespace Student {

class GameBoard : public QObject, public Common::IGameBoard
{
public:
    GameBoard();
    ~GameBoard();
    //Määrittää karttapalojen keskipisteiden koordinaatit, ja tallentaa ne vektoriin midpoints_
    void determine_midpoints();

    //determine midpoints()-funktion käyttämä funktio, liikuttelee keskipistettä tarpeen mukaan
    QPointF move_midpoint(QPointF midpoint_marker, std::string direction);

    QPointF cube_to_square(Common::CubeCoordinate cubecoords);

    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> get_hexPointers();
    std::map<int, std::shared_ptr<Common::Pawn>> get_pawns();

    // Valitsee satunnaisen naapurihexin, käytetään jos pawneja on liikaa.
    Common::CubeCoordinate pick_random_available_neighbour(std::shared_ptr<Common::Hex> hexi);

    void set_scene(QGraphicsScene* scene);

    QGraphicsScene* get_scene();
    void insert_hexItems(Common::CubeCoordinate cubecoords, hexgraphics *hex);
    std::map<Common::CubeCoordinate, hexgraphics *> get_hexItems();


    virtual int checkTileOccupation(Common::CubeCoordinate tileCoord) const;
    virtual bool isWaterTile(Common::CubeCoordinate tileCoord) const;
    virtual std::shared_ptr<Common::Hex> getHex(Common::CubeCoordinate hexCoord) const;
    virtual void addPawn(int playerId, int pawnId);
    virtual void addPawn(int playerId, int pawnId, Common::CubeCoordinate coord);
    virtual void movePawn(int pawnId, Common::CubeCoordinate pawnCoord);
    virtual void removePawn(int pawnId);
    virtual void addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord);
    virtual void moveActor(int actorId, Common::CubeCoordinate actorCoord);
    virtual void removeActor(int actorId);
    virtual void addHex(std::shared_ptr<Common::Hex> newHex);
    virtual void addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord);
    virtual void moveTransport(int id, Common::CubeCoordinate coord);
    virtual void removeTransport(int id);
    void set_testmode_off();
    bool checkAnimalTypeExists(std::string type); // tarkistaa onko laudalla annettua eläintä
    void add_pawn_to_player(int pawnId, int playerId); // lisää pelaajalle pawnin
    void delete_pawn_from_player(int pawnId, int playerId); // poistaa pawnin pelaajalta
    std::map<std::shared_ptr<Common::Pawn>,bool> pawns_NearOrIn_Transport(int currentPlayer); //check if any of player's pawns in a transport or in a hex that contains one
    std::map<int, std::vector<int>> get_playerPawns(); // palauttaa mapin jossa avaimena pelaaja ja sisältönä pelaajan pawnit vectorissa
    bool addPawnToTransport(int pawnID); // lisää nappulan transportin sisään
    bool removePawnFromTransport(int pawnID); // poistaa nappulan transportista
    bool pawnInTransport(std::shared_ptr<Common::Pawn> pawn);
    bool playerHasPawns(int playerID); // kertoo onko pelaajalla vielä nappuloita
    void calculateEndOfGamePoints(int playerID); // laskee viimeiseksi jäänelle pelaajalle pisteet jäljelle jääneiden pawnien perusteella
    std::map<int,int> get_PointMap(); // palauttaa mapin pelaajien pisteistä, avaimena pelaajan id
    void set_infoBox(infoBox* laatikko); // asettaa osoittimen ui:n infoboxiin tulostuksia varten

private:
    bool testing_ = true; //testmode: skippaa graafiset elementit gameboardin testauksen yhteydessä.
    void doGraphicalAction(std::shared_ptr<Common::Actor> actor); //Toteuttaa actorin doActionin graafisille olioille
    void addPoint(int playerID);


    int layerCount_ = 20;              //Hexagoni-layerien max määrä kartassa.
    std::map<Common::CubeCoordinate, QPointF> midpoints_; //Hexagonien keskipisteet CubeCoordinate ja xy-muodoissa
    std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>> hexPointers_;
    std::map<int, std::shared_ptr<Common::Actor>> actors_;
    std::map<int, std::shared_ptr<Common::Transport>> transports_;
    std::map<int, std::shared_ptr<Common::Pawn>> pawns_; //pawn pointers, searchable by pawnID
    std::map<int, pawngraphics*> pawnItems_; //pawngraphics-items, searchable by pawnID
    std::map<int, pixmapgraphics*> actorItems_; //  actorien graafiset itemit
    std::map<int, pixmapgraphics*> transportItems_; // transporttien graafiset itemit
    std::map<Common::CubeCoordinate, hexgraphics*> hexItems_;
    std::map<Common::CubeCoordinate, std::vector<bool>> pawnSlots_;
    std::map<int,std::vector<int>> playerPawns_; // key=playerID, value= vector of player's pawns.
    std::map<int,int> playerPoints_; // points of each player

    infoBox* infobox_;
    QGraphicsScene* scene_;

};

}
#endif // GAMEBOARD_HH
