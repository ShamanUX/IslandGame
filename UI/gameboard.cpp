#include "gameboard.hh"
#include "iostream"
#include  <random>
#include  <iterator>
#include <player.hh>


namespace Student {

GameBoard::GameBoard()
{
    determine_midpoints();

    //seuraava for-loop alustaa hexPointers-mapin, johon asetetaan addHexin yhteydessä pointereita.
    for ( auto const& it : midpoints_ ) {
        hexPointers_.insert(std::pair<Common::CubeCoordinate, std::shared_ptr<Common::Hex>>(it.first, nullptr) );
    }

}

GameBoard::~GameBoard()
{

}

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}


void GameBoard::determine_midpoints()
{
    //Lisätään aluksi origo, eli ensimmäinen layeri

    QPointF midpoint_marker(0,0);
    Common::CubeCoordinate cube_coords;
    Common::CubeCoordinate(0, 0, 0) = cube_coords;

    midpoints_[cube_coords] = midpoint_marker;


    int layer_counter = 1;


    while ( layer_counter < layerCount_ ) {

        // Siirrytään loopin alussa seuraavaan layeriin, eli ylöspäin, ja aloitetaan piirtäminen siitä.
        layer_counter++;
        midpoint_marker = move_midpoint(midpoint_marker, "N");
        cube_coords.y += 1;
        cube_coords.z -= 1;
        midpoints_[cube_coords] = midpoint_marker;

        // side_hops on hyppyjen (tai askelten) määrä, mitä tarvitaan päästäksemme jonkin layerin "sivun" päästä päähän.
        // Koska layerin numero on sama kuin sivujen määrä, side_hops on yksi vähemmän kuin layerin numero.
        int side_hops = layer_counter - 1;
        int i;

        //Seuraavat for-loopit liikuttavat keskipiste-merkkiä jokaisen sivun läpi, kiertäen koko layerin.
        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "SW");
            cube_coords.x -= 1;
            cube_coords.z += 1;
            midpoints_[cube_coords] = midpoint_marker;
        }

        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "S");
            cube_coords.y -= 1;
            cube_coords.z += 1;
            midpoints_[cube_coords] = midpoint_marker;
        }

        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "SE");
            cube_coords.x += 1;
            cube_coords.y -= 1;
            midpoints_[cube_coords] = midpoint_marker;
        }

        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "NE");
            cube_coords.x += 1;
            cube_coords.z -= 1;
            midpoints_[cube_coords] = midpoint_marker;
        }

        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "N");
            cube_coords.y += 1;
            cube_coords.z -= 1;
            midpoints_[cube_coords] = midpoint_marker;
        }

        for ( i = side_hops; i != 0; i-- ) {
            midpoint_marker = move_midpoint(midpoint_marker, "NW");
            cube_coords.y += 1;
            cube_coords.x -= 1;
            //Tarkistetaan, että olemmeko yhden kuvion päässä layerin ylimmästä hexagonista, josta aloitettiin piirtäminen
            if ( i != 1 ) {
                midpoints_[cube_coords] = midpoint_marker;
            }
        }
    }





}

QPointF GameBoard::move_midpoint(QPointF midpoint_marker, std::string direction)
{
    //Directionit merkataan ilmansuunnilla: North, South, Northwest, jne.
    if (direction == "N") {
        //Tässä miinustetaan, koska y on negatiivinen ylöspäin.
        midpoint_marker.setY( midpoint_marker.y() - HEX_HEIGHT );
    } else if (direction == "S") {
        midpoint_marker.setY( midpoint_marker.y() + HEX_HEIGHT );
    } else if (direction == "SW") {
        midpoint_marker.setX( midpoint_marker.x() - HEX_WIDTH * .75 );
        midpoint_marker.setY( midpoint_marker.y() + HEX_HEIGHT / 2) ;
    } else if (direction == "SE") {
        midpoint_marker.setX( midpoint_marker.x() + HEX_WIDTH * .75 );
        midpoint_marker.setY( midpoint_marker.y() + HEX_HEIGHT / 2) ;
    } else if (direction == "NW") {
        midpoint_marker.setX( midpoint_marker.x() - HEX_WIDTH * .75 );
        midpoint_marker.setY( midpoint_marker.y() - HEX_HEIGHT / 2) ;
    } else if (direction == "NE") {
        midpoint_marker.setX( midpoint_marker.x() + HEX_WIDTH * .75 );
        midpoint_marker.setY( midpoint_marker.y() - HEX_HEIGHT / 2) ;
    }
    return midpoint_marker;
}


QPointF GameBoard::cube_to_square(Common::CubeCoordinate cubecoords)
{
    return midpoints_.at(cubecoords);
}

std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex> > GameBoard::get_hexPointers()
{
    return hexPointers_;
}

Common::CubeCoordinate GameBoard::pick_random_available_neighbour(std::shared_ptr<Common::Hex> full_hex)
{
   Common::CubeCoordinate random_neighbour;

    //kerätään naapurilaatoista ei-täydet laatat, ja valitaan niistä satunnainen ja sen koordinaatit.
    std::vector<Common::CubeCoordinate> available_neighbours;
    for ( Common::CubeCoordinate cubecoords : full_hex->getNeighbourVector() ) {
        if ( hexPointers_.at(cubecoords)->getPawnAmount() < 3 ) {
            available_neighbours.push_back(cubecoords);
           }
    }
    random_neighbour = *select_randomly(available_neighbours.begin(), available_neighbours.end());
    return random_neighbour;
}

void GameBoard::set_scene(QGraphicsScene *scene)
{
    scene_ = scene;
}






void GameBoard::removeTransport(int id)
{

}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{

}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{

}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate cubecoords = newHex->getCoordinates();
    hexPointers_[cubecoords] = newHex;
}

void GameBoard::removeActor(int actorId)
{

}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{

}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{
    std::shared_ptr<Common::Hex> target_hex = hexPointers_.at(actorCoord);
    // TODO: GRAAFINEN SETTI!

}

void GameBoard::removePawn(int pawnId)
{
    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnId);
    Common::CubeCoordinate pawnCoord = pawn->getCoordinates();
    hexPointers_.at(pawnCoord)->removePawn(pawn);
    pawns_.erase(pawnId);

}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnId);
    std::shared_ptr<Common::Hex> current_hex = hexPointers_.at(pawn->getCoordinates());
    std::shared_ptr<Common::Hex> target_hex = hexPointers_.at(pawnCoord);
    std::vector<Common::CubeCoordinate> neighbour_tiles = current_hex->getNeighbourVector();

    //A few checks about integrity of this movement before moving
    if ( pawn->getCoordinates() == pawnCoord ) {
        std::cout << "You're already on this tile!" << std::endl;

    } else if ( std::find(neighbour_tiles.begin(), neighbour_tiles.end(), pawnCoord) != neighbour_tiles.end()  ) {
        std::cout << "You're not next to this tile!" << std::endl;
    }
    // checks done: can move pawn
    else {
        current_hex->removePawn(pawn);
        target_hex->addPawn(pawn);
    }

}

//HUOM! STILL NEEDS GRAPHICAL IMPLEMENTATION!
// FIX: UNDEFINED REFERENCE TO PLAYER::add_pawn() KUN KUTSUTAAN add_pawn
void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    std::cout << pawnId << std::endl;
    std::shared_ptr<Common::Hex> hexi = hexPointers_.at(coord);
    std::shared_ptr<Common::Pawn> new_pawn;

    if ( hexi->getPawnAmount() < 3 ) {
        new_pawn = std::make_shared<Common::Pawn>(pawnId, playerId, coord);
    } else {
        Common::CubeCoordinate free_tile_coord = pick_random_available_neighbour(hexi);
        new_pawn = std::make_shared<Common::Pawn>(pawnId, playerId, free_tile_coord);
        hexi = hexPointers_.at(free_tile_coord);
        coord = free_tile_coord;
    }
    hexi->addPawn(new_pawn);
    pawns_[pawnId] = new_pawn;
    // playerPawns_.at(playerId).push_back(pawnId); LISÄÄ TÄHÄN FIND()-checkki!!

    QGraphicsEllipseItem* uusi_nappula = new QGraphicsEllipseItem;
    QPointF XYCOORDS = cube_to_square(coord);
    if ( hexi->getPawnAmount() == 0 ) {
        uusi_nappula->setRect(XYCOORDS.x()+4, XYCOORDS.y()-6,8,8);
    } else if (hexi->getPawnAmount() == 1) {
        uusi_nappula->setRect(XYCOORDS.x()-4, XYCOORDS.y()+6,8,8);
    } else if (hexi->getPawnAmount() == 2) {
        uusi_nappula->setRect(XYCOORDS.x()+4, XYCOORDS.y()+4,8,8);
    }
    QBrush brush;

    //TODO: PELAAJAN VÄRIN HAKU

    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::blue);
    uusi_nappula->setBrush(brush);
    scene_->addItem(uusi_nappula);
    //players_.at(playerId)->add_pawn(pawnId);

}

void GameBoard::addPawn(int playerId, int pawnId)
{
    std::shared_ptr<Common::Hex> hexi = hexPointers_.at(Common::CubeCoordinate(0,0,0));
    std::shared_ptr<Common::Pawn> new_pawn;

    if ( hexi->getPawnAmount() < 3 ) {
        new_pawn = std::make_shared<Common::Pawn>(pawnId, playerId, Common::CubeCoordinate(0,0,0));
    } else {
        Common::CubeCoordinate free_tile = pick_random_available_neighbour(hexi);
        new_pawn = std::make_shared<Common::Pawn>(pawnId, playerId, free_tile);

    }
    hexi->addPawn(new_pawn);
    pawns_[pawnId] = new_pawn;
    playerPawns_.at(playerId).push_back(pawnId);

}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate hexCoord) const
{

    if ( hexPointers_.find(hexCoord) == hexPointers_.end()) {
        return nullptr;
    }
    return hexPointers_.at(hexCoord);

}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{
    if ( hexPointers_.at(tileCoord)->getPieceType() == "Water" ) {
        return true;
    } else {
        return false;
    }
}

int GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{
    if ( hexPointers_.find(tileCoord) == hexPointers_.end()) {
        return -1;
    } else {
        return hexPointers_.at(tileCoord)->getPawnAmount();
    }
}

}
