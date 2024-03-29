#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"
#include "vector"
#include "infobox.hh"

class GameState :  public Common::IGameState
{

public:
    GameState();
    ~GameState();
    virtual Common::GamePhase currentGamePhase() const;
    virtual int currentPlayer() const;
    virtual void changeGamePhase(Common::GamePhase nextPhase);
    virtual void changePlayerTurn(int nextPlayer);

    std::vector<int> get_availablePlayers();
    void addPlayer(int playerID);
    void removePlayer(int playerID);
    bool isAnyoneAlive();

    void set_infobox(infoBox *boxi);
private:
    Common::GamePhase phase_ = Common::GamePhase::MOVEMENT; //current GamePhase
    int player_ ; //Current player's ID
    std::vector<int> availablePlayers_;
    infoBox* infobox_;

};

#endif // GAMESTATE_HH
