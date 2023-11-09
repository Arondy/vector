#include "history.h"

namespace Components {
    string GameHistory::getLastMove() const {
        if (!history.empty()){
            return history.back();
        } else {
            return "";
        }
    }

    void GameHistory::addMove(const string& move){
        history.push_back(move);
    }
}