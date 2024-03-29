#include "gameboard.hh"
#include "iostream"
#include  <random>
#include  <iterator>
#include <player.hh>
#include <QObject>
#

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

//seuraavat 2 blokkia alustavat randomgeneraattorin satunnaisen naapurilaatan valitsemiselle


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
    double nollakohta = 0;
    QPointF midpoint_marker(nollakohta,nollakohta);
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
        // Koska layerin numero on sama kuin yhdellä sivulla olevien laattojen määrä, side_hops on yksi vähemmän kuin layerin numero.
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
	/* Keskipisteiden siirto laskettu niin, että ylä- ja alapuolella sijaitseviin keskipisteisiin on 
	 * matkaa kuusikulmion korkeuden (HEX_HEIGHT) verran, ja sivulla sijaitseviin
	 * keskipisteisiin 3/4 kuusikulmion leveydestä (HEX_WIDTH) sivusuuntaan, ja 
	 * puoli korkeutta pystysuoraan suuntaan.
	 */
	
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

std::map<int, std::shared_ptr<Common::Pawn> > GameBoard::get_pawns()
{
    return pawns_;
}

Common::CubeCoordinate GameBoard::pick_random_available_neighbour(std::shared_ptr<Common::Hex> full_hex)
{
   Common::CubeCoordinate random_neighbour;

    //kerätään naapurilaatoista ei-täydet laatat, ja valitaan niistä satunnainen ja sen koordinaatit.
    std::vector<Common::CubeCoordinate> available_neighbours;
    for ( Common::CubeCoordinate cubecoords : full_hex->getNeighbourVector() ) {
        if ( hexPointers_.at(cubecoords)->getPawnAmount() < 3) {
            if ( cubecoords.x == 0 && cubecoords.y == 0 && cubecoords.z == 0 ) {
                // Do nothing
            } else {
                available_neighbours.push_back(cubecoords);
            }
           }
    }
    random_neighbour = *select_randomly(available_neighbours.begin(), available_neighbours.end());
    return random_neighbour;
}





void GameBoard::set_scene(QGraphicsScene *scene)
{
    scene_ = scene;
}

QGraphicsScene *GameBoard::get_scene()
{
    return scene_;
}

void GameBoard::insert_hexItems(Common::CubeCoordinate cubecoords, hexgraphics* hex)
{
    hexItems_[cubecoords] = hex;

    //alustetaan samalla pawnSlots_ map
    std::vector<bool> dummy {false, false, false};
    pawnSlots_[cubecoords] = dummy;
}

std::map<Common::CubeCoordinate, hexgraphics *> GameBoard::get_hexItems()
{
    return hexItems_;
}

void GameBoard::removeTransport(int id)
{
    if ( transports_.find(id) != transports_.end() ) {
        std::shared_ptr<Common::Transport> kulkuneuvo = transports_.at(id);
        std::shared_ptr<Common::Hex> kuusikulmio = kulkuneuvo->getHex();
        kuusikulmio->removeTransport(kulkuneuvo);
        if (testing_ != true) {
            delete transportItems_.at(kulkuneuvo->getId());
        }


        transports_.erase(id);
    }
}

void GameBoard::set_testmode_off()
{
    testing_ = false;
}

void GameBoard::doGraphicalAction(std::shared_ptr<Common::Actor> actor)
{
    std::string actorType = actor->getActorType();
    Common::CubeCoordinate actorCoords = actor->getHex()->getCoordinates();
    std::shared_ptr<Common::Hex> hexDummy;

    if ( actorType == "vortex") {
        //First delete everything from this tile, then from neighbouring watertiles.
        hexDummy = hexPointers_.at(actorCoords);
        if ( hexDummy->getActors().empty() == false ) {
            for ( auto toimija : hexDummy->getActors() ) {
                    delete actorItems_.at(toimija->getId());
            }
        }
        if ( hexDummy->getPawns().empty() == false ) {
            for ( auto nappula : hexDummy->getPawns() ) {
                //delete pawnItems_.at(nappula->getId());
                removePawn(nappula->getId());
            }
        }
        if (hexDummy->getTransports().empty() == false) {
            for ( auto kulkuneuvo : hexDummy->getTransports() ) {
                delete transportItems_.at(kulkuneuvo->getId());
            }
        }



        for ( Common::CubeCoordinate neighbourCoords : hexPointers_.at(actorCoords)->getNeighbourVector()) {
            hexDummy = hexPointers_.at(neighbourCoords);
                if ( hexDummy->getActors().empty() == false ) {
                    for ( auto toimija : hexDummy->getActors() ) {
                            delete actorItems_.at(toimija->getId());
                    }
                }
                if ( hexDummy->getPawns().empty() == false ) {
                    for ( auto nappula : hexDummy->getPawns() ) {
                        //delete pawnItems_.at(nappula->getId());
                        removePawn(nappula->getId());
                    }
                }
                if (hexDummy->getTransports().empty() == false) {
                    for ( auto kulkuneuvo : hexDummy->getTransports() ) {
                        delete transportItems_.at(kulkuneuvo->getId());
                    }
                }

        }
    } else if ( actorType == "seamunster" ) {
        //Poistetaan kaikki nappulat ja transportit tästä tiilistä. Spawnatessa siis pelkät pawnit.
        hexDummy = hexPointers_.at(actorCoords);
        if ( hexDummy->getPawns().empty() == false ) {
            for ( auto nappula : hexDummy->getPawns() ) {
                //delete pawnItems_.at(nappula->getId());
                removePawn(nappula->getId());
            }
        }
        if (hexDummy->getTransports().empty() == false) {
            for ( auto kulkuneuvo : hexDummy->getTransports() ) {
                delete transportItems_.at(kulkuneuvo->getId());
            }
        }
    } else if ( actorType == "kraken" ) {
        hexDummy = hexPointers_.at(actorCoords);
        if (hexDummy->getTransports().empty() == false) {
            for ( auto kulkuneuvo : hexDummy->getTransports() ) {
                if ( kulkuneuvo->getPawnsInTransport().size() > 0 ) {
                    for ( auto pawn : kulkuneuvo->getPawnsInTransport() ) {
                        removePawnFromTransport(pawn->getId());
                    }
                }
                delete transportItems_.at(kulkuneuvo->getId());
            }
        }
    } else if ( actorType == "shark" ) {
        hexDummy = hexPointers_.at(actorCoords);
        if ( hexDummy->getPawns().empty() == false ) {
            for ( auto nappula : hexDummy->getPawns() ) {
                //delete pawnItems_.at(nappula->getId());
                removePawn(nappula->getId());
            }
        }
    }
}


void GameBoard::addPoint(int playerID)
{
    if (playerPoints_.find(playerID) != playerPoints_.end()) {
        playerPoints_.at(playerID) += 1;
    } else {
        playerPoints_[playerID] = 1;
    }
}

void GameBoard::calculateEndOfGamePoints(int playerID)
{
    for ( auto const& it : pawns_ ) {
        std::shared_ptr<Common::Pawn> pawn = it.second;
        if ( pawn->getPlayerId() == playerID ) {
            addPoint(playerID);
            pawns_.erase(pawn->getId());
        }
    }
}

std::map<int, int> GameBoard::get_PointMap()
{
    return playerPoints_;
}

void GameBoard::set_infoBox(infoBox *laatikko)
{
    infobox_ = laatikko;
}

bool GameBoard::checkAnimalTypeExists(std::string type)
{
    for (auto const& it : actors_ ) {
        if (it.second->getActorType() == type) {
            return true;
        }
    }
    for (auto const& it : transports_) {
        if (it.second->getTransportType() == type) {
            return true;
        }
    }
    return false;
}

void GameBoard::add_pawn_to_player(int pawnId, int playerId)
{
    if ( playerPawns_.find(playerId) == playerPawns_.end() ) {
        playerPawns_[playerId] = std::vector<int> {};
    }
    playerPawns_.at(playerId).push_back(pawnId);
}



bool GameBoard::playerHasPawns(int playerID) {
    for ( auto const& it : pawns_ ) {
        if (it.second->getPlayerId() == playerID ) {
            return true; //Player has atleast one pawn.
        }
    }
    return false; //Player has no pawns left.
}

std::map<std::shared_ptr<Common::Pawn>,bool> GameBoard::pawns_NearOrIn_Transport(int currentPlayer)
{
    std::map<std::shared_ptr<Common::Pawn>,bool> transportyPawns;
    for ( auto const& it : pawns_ ) {
        std::shared_ptr<Common::Pawn> pawn = it.second;
        std::shared_ptr<Common::Hex> pawnHex = hexPointers_.at(pawn->getCoordinates());

        // Check if pawn belongs to current player, and if the hex contains a transport
        if ( pawn->getPlayerId() == currentPlayer && pawnHex->getTransports().size() == 1 ) {

            if ( pawnHex->getTransports().at(0)->isPawnInTransport(pawn) ) {
                 transportyPawns[pawn] = true; //Pawn in the transport, adding the ptr and "true" to our map.
            } else {
                //There is a transport in the pawn's hex, but the pawn is not inside it. Adding pawnptr and "false" to the map.
                transportyPawns[pawn] = false;
            }


        }

    }
    return transportyPawns;
}

std::map<int, std::vector<int> > GameBoard::get_playerPawns()
{
    return playerPawns_;
}

bool GameBoard::addPawnToTransport(int pawnID)
{
    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnID);
    std::shared_ptr<Common::Hex> pawnHex = hexPointers_.at(pawn->getCoordinates());
    std::shared_ptr<Common::Transport> transport = pawnHex->getTransports().at(0);

    pawngraphics* pawnItem = pawnItems_.at(pawnID);
    QPointF XYcoords = cube_to_square(pawn->getCoordinates());

    if (transport->getCapacity() > 0) {
        transport->addPawn(pawn);
        if (pawnItem->get_pawnSlot() == 1) {
            pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.6, XYcoords.y()-HEX_SIZE*0.6,PAWN_WIDTH,PAWN_HEIGHT);
        } else if (pawnItem->get_pawnSlot() == 2) {
            pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.2, XYcoords.y()-HEX_SIZE*0.6,PAWN_WIDTH,PAWN_HEIGHT);
        } else if (pawnItem->get_pawnSlot() == 3) {
            pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.6, XYcoords.y()-HEX_SIZE*0.2,PAWN_WIDTH,PAWN_HEIGHT);
        }
        scene_->update();
        return true;

    } else {
        infobox_->printInfo("Transport is full");
        return false;
    }
}

bool GameBoard::removePawnFromTransport(int pawnID)
{
    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnID);
    std::shared_ptr<Common::Hex> pawnHex = hexPointers_.at(pawn->getCoordinates());
    std::shared_ptr<Common::Transport> transport;
    pawngraphics* pawnItem = pawnItems_.at(pawnID);
    QPointF XYcoords = cube_to_square(pawn->getCoordinates());
    if ( pawnHex->getTransports().size() > 0 ){
            transport = pawnHex->getTransports().at(0);
    } else {

        //Tämä tapahtuu, mikäli ollaan liikutettu delfiini "D":llä pois pelaajan alta
        pawnItem = pawnItems_.at(pawnID);
        if (pawnItem->get_pawnSlot() == 1) {
            pawnItem->setRect(XYcoords.x()+HEX_SIZE/5, XYcoords.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
        } else if (pawnItem->get_pawnSlot() == 2) {
            pawnItem->setRect(XYcoords.x()+HEX_SIZE/5, XYcoords.y()-HEX_SIZE*0.3,PAWN_WIDTH,PAWN_HEIGHT);
        } else if (pawnItem->get_pawnSlot() == 3) {
            pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.3, XYcoords.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
        }
        scene_->update();
        return false;
    }


    pawnItem = pawnItems_.at(pawnID);

    if (transport->isPawnInTransport(pawn)) {
    transport->removePawn(pawn);
    if (pawnItem->get_pawnSlot() == 1) {
        pawnItem->setRect(XYcoords.x()+HEX_SIZE/5, XYcoords.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
    } else if (pawnItem->get_pawnSlot() == 2) {
        pawnItem->setRect(XYcoords.x()+HEX_SIZE/5, XYcoords.y()-HEX_SIZE*0.3,PAWN_WIDTH,PAWN_HEIGHT);
    } else if (pawnItem->get_pawnSlot() == 3) {
        pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.3, XYcoords.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
    }
    scene_->update();
    return true;
    } else {
        infobox_->printInfo("Pawn isn't in transport");

        return false;
    }
}

bool GameBoard::pawnInTransport(std::shared_ptr<Common::Pawn> pawn)
{
    std::shared_ptr<Common::Hex> pawnHex = hexPointers_.at(pawn->getCoordinates());
    if (pawnHex->getTransports().size() != 0) {
        return pawnHex->getTransports().at(0)->isPawnInTransport(pawn);
    }
    return false;

}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Transport> transport;
    if (transports_.find(id) != transports_.end()) {
       transport = transports_.at(id);
    } else {
        return;
    }


    std::shared_ptr<Common::Hex> current_hex = transport->getHex();
    std::shared_ptr<Common::Hex> target_hex;

    if ( hexPointers_.find(coord) != hexPointers_.end() ) {
        target_hex = hexPointers_.at(coord);
    } else {
        return;
    }


    //Ylimääräinen check jolla tarkistetaan voiko transport liikkua hexiin.
    if ( testing_ != true ) {
        if ((hexPointers_.at(coord)->getActors()).size() != 0 or
                (hexPointers_.at(coord)->getTransports()).size() != 0) {
            infobox_->printInfo("There is already another actor or transport on this tile");
        } else {
            if ( transport->getPawnsInTransport().size() != 0) {
                for (auto pawni : transport->getPawnsInTransport()) {
                    current_hex->removePawn(pawni);
                    target_hex->addPawn(pawni);
                    pawni->setCoordinates(target_hex->getCoordinates());
                    pawngraphics* pawnItem;
                    QPointF XYcoords;
                    if ( pawnItems_.find(pawni->getId()) != pawnItems_.end() ) {
                         pawnItem = pawnItems_.at((pawni->getId()));
                         XYcoords = cube_to_square(coord);
                    }

                    if (pawnItem->get_pawnSlot() == 1) {
                        pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.6, XYcoords.y()-HEX_SIZE*0.6,PAWN_WIDTH,PAWN_HEIGHT);
                    } else if (pawnItem->get_pawnSlot() == 2) {
                        pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.2, XYcoords.y()-HEX_SIZE*0.6,PAWN_WIDTH,PAWN_HEIGHT);
                    } else if (pawnItem->get_pawnSlot() == 3) {
                        pawnItem->setRect(XYcoords.x()-HEX_SIZE*0.6, XYcoords.y()-HEX_SIZE*0.2,PAWN_WIDTH,PAWN_HEIGHT);
                    }
                }
            }
            transport->addHex(target_hex);
            QPointF XYCOORDS = cube_to_square(coord);
            transportItems_.at(id)->movePicture(XYCOORDS);
            scene_->update();
        }
    } else {
        transport->addHex((target_hex));
    }

}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport, Common::CubeCoordinate coord)
{

    std::shared_ptr<Common::Hex> hexi = hexPointers_.at(coord);
    hexi->addTransport(transport);
    transport->addHex(hexi);

    if (testing_ != true ) {
        pixmapgraphics* uusi_transport = new pixmapgraphics;
        QPointF XYCOORDS = cube_to_square(coord);
        std::string tyyppi = transport->getTransportType();
        uusi_transport->setPicture(tyyppi);
        uusi_transport->movePicture(XYCOORDS);
        scene_->addItem(uusi_transport);
        transportItems_[transport->getId()] = uusi_transport;
        infobox_->printInfo("A " + transport->getTransportType() + " appeared.");
    }


    transports_[transport->getId()] = transport;

}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    Common::CubeCoordinate cubecoords = newHex->getCoordinates();
    hexPointers_[cubecoords] = newHex;

    //Graafinen puoli
    if ( testing_ != true ) {
    QBrush brush;
    std::string type = newHex->getPieceType();
    if (type == "Peak") {

        brush.setColor(Qt::darkGray);
    } else if (type == "Mountain") {
        brush.setColor(Qt::lightGray);
    } else if (type == "Forest") {
        brush.setColor(Qt::green);
    } else if (type == "Beach") {
        brush.setColor(Qt::yellow);
    } else if (type == "Coral") {
        brush.setColor(Qt::magenta);
    } else if (type == "Water") {
        brush.setColor(Qt::cyan);
    } else {
        std::cout << "Unrecognized type!" << std::endl;
    }


        hexgraphics* HexItem = new hexgraphics;
        brush.setStyle(Qt::SolidPattern);

        HexItem->set_hexptr(newHex);
        HexItem->set_coords(cube_to_square(cubecoords));
        HexItem->setBrush(brush);

        scene_->addItem(HexItem);
        insert_hexItems(cubecoords, HexItem);
    }


}

void GameBoard::removeActor(int actorId)
{
    if ( actors_.find(actorId) != actors_.end() ){
        std::shared_ptr<Common::Actor> toimija = actors_.at(actorId);
        std::shared_ptr<Common::Hex> kuusikulmio = toimija->getHex();
        kuusikulmio->removeActor(toimija);

        actors_.erase(actorId);

        if ( testing_ != true ) {
            delete actorItems_.at(toimija->getId());
        }
    }
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    std::shared_ptr<Common::Actor> actor = actors_.at(actorId);
    std::shared_ptr<Common::Hex> current_hex = actor->getHex();
    std::shared_ptr<Common::Hex> target_hex;
    if ( hexPointers_.find(actorCoord) != hexPointers_.end() ) {
        target_hex = hexPointers_.at(actorCoord);
    } else {
        return;
    }


    //Ylimääräinen check jolla tarkistetaan voiko actor liikkua hexiin.

    if (testing_ != true) {
        if (actor->getActorType() == "shark") {
            if ((hexPointers_.at(actorCoord)->getActors()).size() != 0 or
                    (hexPointers_.at(actorCoord)->getTransports()).size() != 0) {
                infobox_->printInfo("There is already another actor or transport on this tile");
            } else {
                actor->addHex(target_hex);
                QPointF XYCOORDS = cube_to_square(actorCoord);
                actorItems_.at(actorId)->movePicture(XYCOORDS);
                scene_->update();

                doGraphicalAction(actor);
                actor->doAction();
            }
        } else {
            if ((hexPointers_.at(actorCoord)->getActors()).size() != 0) {
                infobox_->printInfo("There is already another actor on this tile");
            } else {
                actor->addHex(target_hex);
                QPointF XYCOORDS = cube_to_square(actorCoord);
                actorItems_.at(actorId)->movePicture(XYCOORDS);
                scene_->update();

                doGraphicalAction(actor);
                actor->doAction();

            }
        }
    } else {
        actor->addHex(target_hex);
    }

}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor, Common::CubeCoordinate actorCoord)
{

    std::shared_ptr<Common::Hex> hexi = hexPointers_.at(actorCoord);
    actor->addHex(hexi);
    std::cout << "actor id : " << actor->getId() << std::endl;

    if (testing_ != true ) {
        pixmapgraphics* uusi_actor = new pixmapgraphics;
        QPointF XYCOORDS = cube_to_square(actorCoord);
        std::string tyyppi = actor->getActorType();
        uusi_actor->setPicture(tyyppi);
        uusi_actor->movePicture(XYCOORDS);
        scene_->addItem(uusi_actor);
        actorItems_[actor->getId()] = uusi_actor;

        doGraphicalAction(actor);
        if (actor->getActorType() == "vortex") {
            infobox_->printInfo("A " + actor->getActorType() + " appeared and destroyed everything around it.");
        } else {
            infobox_->printInfo("A " + actor->getActorType() + " appeared.");
        }
    }
    actors_[actor->getId()] = actor;
    actor->doAction();



}

void GameBoard::removePawn(int pawnId)
{
    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnId);
    Common::CubeCoordinate pawnCoord = pawn->getCoordinates();
    hexPointers_.at(pawnCoord)->removePawn(pawn);

    pawns_.erase(pawnId);

    //Poistetaan vielä pawnin graafinen puoli.
    if (testing_ != true) {
        if (pawnItems_.find(pawnId) != pawnItems_.end()) {
            delete pawnItems_.at(pawnId);
            pawnItems_.erase(pawnId);
        }

    }
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{

    std::shared_ptr<Common::Pawn> pawn = pawns_.at(pawnId);
    std::shared_ptr<Common::Hex> current_hex = hexPointers_.at(pawn->getCoordinates());
    std::shared_ptr<Common::Hex> target_hex;
    if (hexPointers_.find(pawnCoord) != hexPointers_.end()) {
        target_hex = hexPointers_.at(pawnCoord);
    } else {
        return;
    }
    std::shared_ptr<Common::Transport> transportInCurrent = nullptr;
    if (current_hex->getTransports().size() != 0) {
        transportInCurrent = current_hex->getTransports().at(0);
    }
    bool canMove = true;
    std::vector<Common::CubeCoordinate> neighbour_tiles = current_hex->getNeighbourVector();


    // Ylimääräinen check joka tarkistaa saako transportin sisällä oleva pawn liikkua
    if (transportInCurrent != nullptr) {
        if (transportInCurrent->isPawnInTransport(pawn)) {
            if (transportInCurrent->canMove(pawn->getPlayerId())){
                canMove = true;
            } else {
                if (infobox_ != nullptr) {
                    infobox_->printInfo("Player " + std::to_string(pawn->getPlayerId()) +
                              "can't move this boat. Unboard before moving.");
                }
                canMove = false;
            }
        }
    }

        current_hex->removePawn(pawn);

        if (target_hex->getPieceType() != "Coral") {
            target_hex->addPawn(pawn);
            // jos pawn siirretään maaliruutuun, sen toinnallinen puoli poistetaan pelistä
        } else {
            // Pawnin saattamisesta maaliruutuun saa kaksi pistettä
            addPoint(pawn->getPlayerId());
            addPoint(pawn->getPlayerId());
            infobox_->printInfo("Player " + std::to_string(pawn->getPlayerId()) + "has saved a pawn! Two points added.");
            pawns_.erase(pawn->getId());
        }
        if (testing_ != true ) {
            int pawnslot = pawnItems_.at(pawnId)->get_pawnSlot();
            pawnSlots_.at(pawn->getCoordinates()).at(pawnslot-1) = false;

            pawn->setCoordinates(pawnCoord);

            QPointF XYCOORDS = cube_to_square(pawnCoord);
            if ( pawnItems_.find(pawnId) == pawnItems_.end() ) {
                std::cout << "Tätä viestiä ei pitäisi näkyä" << std::endl;

            } else if (canMove) {

                if ( pawnSlots_.at(pawnCoord).at(0) == false) {
                    pawnItems_.at(pawnId)->setRect(XYCOORDS.x()+HEX_SIZE/5, XYCOORDS.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
                    pawnSlots_.at(pawnCoord).at(0) = true;
                    pawnItems_.at(pawnId)->set_pawnSlot(1);
                } else if ( pawnSlots_.at(pawnCoord).at(1) == false ) {
                    pawnItems_.at(pawnId)->setRect(XYCOORDS.x()+HEX_SIZE/5, XYCOORDS.y()-HEX_SIZE*0.3,PAWN_WIDTH,PAWN_HEIGHT);
                    pawnSlots_.at(pawnCoord).at(1) = true;
                    pawnItems_.at(pawnId)->set_pawnSlot(2);
                } else if ( pawnSlots_.at(pawnCoord).at(2) == false ) {
                    pawnItems_.at(pawnId)->setRect(XYCOORDS.x()-HEX_SIZE*0.3, XYCOORDS.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
                    pawnSlots_.at(pawnCoord).at(2) = true;
                    pawnItems_.at(pawnId)->set_pawnSlot(3);
                }
                scene_->update();
            }
        }
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{

    std::shared_ptr<Common::Hex> hexi;
    if ( coord.x == 0 && coord.y == 0 && coord.z == 0 && testing_ != true ) {
        Common::CubeCoordinate coords_outside_center = pick_random_available_neighbour(hexPointers_.at(Common::CubeCoordinate(0,0,0)));
        hexi = hexPointers_.at(coords_outside_center);
        coord = coords_outside_center;
    } else {
        hexi = hexPointers_.at(coord);
    }

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

    // Creating A graphical pawn.
    if ( testing_ != true ) {
        pawngraphics* uusi_nappula = new pawngraphics;
        //Player player(123);
        QPointF XYCOORDS = cube_to_square(coord);



        if ( pawnSlots_.at(coord).at(0) == false ) {
            uusi_nappula->setRect(XYCOORDS.x()+HEX_SIZE/5, XYCOORDS.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
            pawnSlots_.at(coord).at(0) = true;
            uusi_nappula->set_pawnSlot(1);
        } else if ( pawnSlots_.at(coord).at(1) == false ) {
            uusi_nappula->setRect(XYCOORDS.x()+HEX_SIZE/5, XYCOORDS.y()-HEX_SIZE*0.3,PAWN_WIDTH,PAWN_HEIGHT);
            pawnSlots_.at(coord).at(1) = true;
            uusi_nappula->set_pawnSlot(2);
        } else if ( pawnSlots_.at(coord).at(2) == false ) {
            uusi_nappula->setRect(XYCOORDS.x()-HEX_SIZE*0.3, XYCOORDS.y()+HEX_SIZE/5,PAWN_WIDTH,PAWN_HEIGHT);
            pawnSlots_.at(coord).at(2) = true;
            uusi_nappula->set_pawnSlot(3);
        }
        QBrush brush;

        //PELAAJAN VÄRIN HAKU

        brush.setStyle(Qt::SolidPattern);
        if ( new_pawn->getPlayerId() == 1001 ) {
            brush.setColor(Qt::blue);
        } else if ( new_pawn->getPlayerId() == 1002 ) {
            brush.setColor(Qt::red);
        } else if ( new_pawn->getPlayerId() == 1003 ) {
            brush.setColor(Qt::white);
        } else if ( new_pawn->getPlayerId() == 1004 ) {
            brush.setColor(Qt::darkGreen);
        }

        uusi_nappula->setBrush(brush);
        scene_->addItem(uusi_nappula);
        pawnItems_[pawnId] = uusi_nappula;
        std::cout << "Pawn added: " << pawnId << " Player ID: " << new_pawn->getPlayerId() << std::endl;
    }



}

//Kutsuu kolmen argumentin addPawnia, cubecoordinaattina keskitiili.
void GameBoard::addPawn(int playerId, int pawnId)
{
    GameBoard::addPawn(playerId, pawnId, Common::CubeCoordinate(0,0,0));

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
