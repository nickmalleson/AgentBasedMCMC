//
// Created by daniel on 26/04/2021.
//

#include "CatMouseAgent.h"

// returns PMF over acts
std::vector<double> CatMouseAgent::timestep(std::multiset<CatMouseAgent> others) {

//        val pCatMove = 0.25
//
//        return if(type == AgentType.CAT) {
//            arrayOf(pCatMove, 1.0-pCatMove)
//        } else {
//            if(others[CatMouseAgent(AgentType.CAT, position)] >= 1) {
//                arrayOf(1.0,0.0)
//            } else {
//                arrayOf(0.0,1.0)
//            }
//        }
    return std::vector<double>(); // placeholder
}

std::vector<CatMouseAgent> CatMouseAgent::consequences(Act act) {
    return std::vector<CatMouseAgent>(); // placeholder
}

std::vector<glpkpp::Constraint> CatMouseAgent::constraints(CatMouseAgent::Act act) {
    return std::vector<glpkpp::Constraint>(); // placeholder
}