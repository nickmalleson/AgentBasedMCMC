//
// Created by daniel on 13/05/2021.
//

#ifndef GLPKTEST_STATETRAJECTORY_H
#define GLPKTEST_STATETRAJECTORY_H

#include <vector>
#include <map>

#include "glpkpp.h"

template<typename AGENT>
class StateTrajectory: public std::vector<ModelState<AGENT>> {
public:
    StateTrajectory(const glp::SparseVec &actTrajectory) {
        for(int i=1; i <= actTrajectory.sparseSize(); ++i) {
            auto event = Event<AGENT>(actTrajectory.indices[i]);
            if(event.time() >= this->size()) this->resize(event.time()+1);
            (*this)[event.time()][event.agent()] += actTrajectory.values[i];
        }
    }


    StateTrajectory(const std::vector<double> &eventTrajectory) {
        this->resize((eventTrajectory.size() - 1) / (AGENT::domainSize() * AGENT::actDomainSize()));
        for(int eventId=1; eventId < eventTrajectory.size(); ++eventId) {
            if(eventTrajectory[eventId] != 0.0) {
                auto event = Event<AGENT>(eventId);
                (*this)[event.time()][event.agent()] += eventTrajectory[eventId];
            }
        }
    }
};


#endif //GLPKTEST_STATETRAJECTORY_H
