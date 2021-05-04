//
// Created by daniel on 04/05/2021.
//

#ifndef GLPKTEST_TRAJECTORY_H
#define GLPKTEST_TRAJECTORY_H

#include "glpkpp.h"

template<typename AGENT>
class Trajectory: glp::SparseVec {
public:
    Trajectory(int dimension, int capacity=-1): glp::SparseVec(dimension,capacity) { }
    Trajectory(glp::SparseVec &&rvalue): glp::SparseVec(0) {
        moveFrom(rvalue);
    }

    Trajectory(const glp::SparseVec &lvalue): glp::SparseVec(lvalue) { }

    Trajectory &operator =(glp::SparseVec &&rvalue) {
        moveFrom(rvalue);
        return *this;
    }

    Trajectory &operator =(const glp::SparseVec &lvalue) {
        glp::SparseVec::operator=(lvalue);
        return *this;
    }

    // event count
    double operator()(int time, const AGENT &agent, const typename AGENT::Act &act) {
        return (*this)[Event(time,agent,act)];
    }

    // occupation number
    double operator()(int time, const AGENT &agent) {
        int beginIndex = Event(time,agent,0);
        int endIndex = beginIndex + AGENT::Act::domainSize;
        double occupation = 0.0;
        int index;
        for(int i=1; i<=sparseSize(); ++i) {
            index = indices[i];
            if(beginIndex <= index && index < endIndex) occupation += values[i];
        }
        return occupation;
    }

//    val stateTrajectory: List<Multiset<AGENT>> by lazy {
//            val stateTrajectory = ArrayList<Multiset<AGENT>>()
//            for((event, occupation) in events) {
//                while(stateTrajectory.size <= event.time) stateTrajectory.add(Multiset())
//                stateTrajectory[event.time][event.agent] += occupation
//            }
//            stateTrajectory
//    }
//
//    val finalState: Multiset<AGENT> by lazy {
//            val finalState = Multiset<AGENT>()
//            val penultimateTime = stateTrajectory.lastIndex
//            stateTrajectory
//            .lastOrNull()
//            ?.members
//            ?.forEach { agent ->
//                for(act in model.actDomain) {
//                    val occupation = this[penultimateTime, agent, act]
//                    if(occupation > 0) {
//                        finalState += model.consequences(agent, act) * occupation
//                    }
//                }
//            }
//            finalState
//    }
//
//
//    val events: Sequence<AbstractMap.SimpleEntry<ABM.Event<AGENT, ACT>, Int>>
//    get() = eventVector.nonZeroEntries
//            .asSequence()
//            .map { AbstractMap.SimpleEntry(model.eventDomain[it.key], it.value.toDouble().roundToInt()) }
//
//
//    inline operator fun get(time: Int, agent: AGENT, act: ACT): Int {
//        return get(ABM.Event(time, agent, act))
//    }
//
//    inline operator fun get(event: ABM.Event<AGENT, ACT>): Int {
//        return eventVector[event.ordinal].toDouble().roundToInt()
//    }
//
//
//
//    fun stateAt(time: Int): Multiset<AGENT> {
//        if(time < stateTrajectory.size) return stateTrajectory[time]
//        if(time == stateTrajectory.size && time != 0) return finalState
//        return emptyMultiset()
//    }
//
//
//    fun nAgents(time: Int, agent: AGENT): Int {
//        if(time < stateTrajectory.size) return stateTrajectory[time][agent]
//        if(time == stateTrajectory.size && time != 0) return finalState[agent]
//        return 0
//    }
//
//
//    fun logPrior(): Double {
//        var logP = 0.0
//        for((event,occupation) in events) {
//            logP += occupation * ln( event.agent.timestep(stateTrajectory[event.time])[event.act.ordinal] )
////                   - CombinatoricsUtils.factorialLog(occupation) // for non-fermionic
//        }
//// Add this for non-fermionic
////        for(state in stateTrajectory) {
////            for((_, occupation) in state.entries) {
////                logP += CombinatoricsUtils.factorialLog(occupation)
////            }
////        }
//        return logP
//    }

};


#endif //GLPKTEST_TRAJECTORY_H