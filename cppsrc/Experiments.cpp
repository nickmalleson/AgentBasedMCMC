//
// Created by daniel on 06/05/2021.
//

#include <vector>
#include "Experiments.h"
#include "agents/CatMouseAgent.h"
#include "ABMProblem.h"
#include "SimplexMCMC.h"
#include "agents/PredPreyAgent.h"

void Experiments::PredPreyExpt() {
    PredPreyAgent::GRIDSIZE = 8;
    int nTimesteps = 4;
    auto startState = ModelState<PredPreyAgent>::randomPoissonState([](const PredPreyAgent &agent) {
//        if(agent.type() == PredPreyAgent::PREDATOR) return 0.02;
//        return 0.04;
        if(agent.type() == PredPreyAgent::PREDATOR) return 0.08;
        return 0.16;
    });
    std::cout << "Start state: " << startState << std::endl;
    auto [observations, realTrajectory] =
            Observation<PredPreyAgent>::generateObservations(startState, nTimesteps, 0.01);
    ABMProblem<PredPreyAgent> abm(nTimesteps, observations);

    std::cout << "Real trajectory: " << glp::SparseVec(realTrajectory) << std::endl;
    std::cout << "Observations: " << observations << std::endl;
//    for(int i=1; i<=abm.nConstraints(); ++i) {
//        std::cout << "Constraint bound " << abm.getRowLb(i) << ", " << abm.getRowUb(i) << std::endl;
//    }
//    for(int j=1; j<=abm.nVars(); ++j) {
//        std::cout << "Constraint bound " << abm.getColLb(j) << ", " << abm.getColUb(j) << std::endl;
//    }
//    std::cout << "Linearised ABM:\n" << abm << std::endl;

//    abm.solutionBasis(realTrajectory);

//    abm.advBasis();
//    abm.warmUp();
//    std::cout << "Adv basis:" << abm.primalSolution() << std::endl;
//    abm.setObjective(glp::SparseVec());
//    abm.setObjDir(glp::Problem::MINIMISE);
//    abm.simplex(); // try a solve
//    std::cout << "Solve status = " << abm.getStatus() << std::endl;
//    std::cout << "Solution: " << abm.primalSolution() << std::endl;


    SimplexMCMC mcmc(abm, abm.logProbFunc());

    // Check initial basis contains no fixed vars and all auxiliaries are in the basis
    for(int k=1; k<=mcmc.nVars(); ++k) {
        if(mcmc.l[k] == mcmc.u[k]) std::cout << "WARNING: Fixed var in SimplexMCMC " << k << std::endl;
        if(mcmc.kSimTokProb[k] > abm.nConstraints() && (mcmc.l[k] != 0.0 || mcmc.u[k] != 1.0 ))
            std::cout << "WARNING: non-binary structural var " << k << std::endl;
    }
    for(int j=1; j<=mcmc.nNonBasic(); ++j) {
        if(mcmc.kSimTokProb[mcmc.head[mcmc.nBasic()+j]] <= abm.nConstraints()) std::cout << "WARNING: Non-basic auxiliary " << j << std::endl;
    }


    std::cout << "Starting with initial sample:" << std::endl;
    std::cout << glp::SparseVec(mcmc.X()) << std::endl;
    mcmc.nextSample();
    std::cout << "Sample: " << glp::SparseVec(mcmc.X()) << std::endl;
    assert(abm.isValidSolution(mcmc.X()));
    mcmc.nextSample();
    std::cout << "Sample: " << glp::SparseVec(mcmc.X()) << std::endl;
    assert(abm.isValidSolution(mcmc.X()));
    return;

//    for(int n=0; n<99; ++n) {
//        mcmc.nextSample();
//        std::cout << "Sample: " << glp::SparseVec(mcmc.X()) << std::endl;
////        std::cout << "number of fractional pivots = " << mcmc.countFractionalPivCols() << " / " << mcmc.nNonBasic() << std::endl;
//        assert(abm.isValidSolution(mcmc.X()));
//    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Experiments::CatMouseExpt() {
    CatMouseAgent leftCat(CatMouseAgent::Type::CAT, CatMouseAgent::Position::LEFT);
    auto observations = std::vector(
            {Observation(
                    1,
                    leftCat,
                    1,
                    0.95)
            });

    ABMProblem<CatMouseAgent> abm(2, observations);

    std::cout << abm << std::endl;

    // calculate initial trajectory
    abm.cpxBasis();
    abm.simplex();
    std::cout << "LP relaxation initial trajectory: " << abm.primalSolution() << std::endl;
    abm.intOpt();
    std::cout << "MIP initial trajectory: " << abm.mipSolution() << std::endl;
    abm.warmUp();

//    Trajectory<CatMouseAgent> initialTrajectory;
//    initialTrajectory.add(Event(0,leftCat, CatMouseAgent::STAYPUT),1.0);
//    std::cout << "Initial trajectory is" << std::endl;
//    std::cout << initialTrajectory << std::endl;
//    abm.stdBasis();
//    abm.warmUp();

    SimplexMCMC mcmc(abm, abm.logProbFunc());
    std::cout << mcmc.X() << std::endl;
    for(int n=0; n<100; ++n) {
        mcmc.nextSample();
//        mcmc.randomWalk();
        std::cout << n << "  " << mcmc.X() << std::endl;
        std::cout << "Valid = " << abm.isValidSolution(mcmc.X()) << std::endl;
        assert(abm.isValidSolution(mcmc.X()));
    }

}


void Experiments::RandomWalk() {
    using glp::X;
    glp::Problem myProb;

    myProb.addConstraint(1.0*X(1) + 1.0*X(2) + 1.0*X(3) <= 100.0);
    myProb.addConstraint(10.0*X(1) + 4.0*X(2) + 5.0*X(3) <= 600.0);
    myProb.addConstraint(2.0*X(1) + 2.0*X(2) + 6.0*X(3) <= 300.0);
    myProb.addConstraint(0.0 <= 1.0*X(1));
    myProb.addConstraint(0.0 <= 1.0*X(2));
    myProb.addConstraint(0.0 <= 1.0*X(3));
    myProb.setObjective(10.0*X(1) + 6.0*X(2) + 4.0*X(3));
    myProb.stdBasis();
    myProb.warmUp();
    std::cout << myProb;

    SimplexMCMC myMCMC(myProb, nullPMF);

    std::cout << myMCMC << std::endl;

    for(int sample=0; sample < 5; ++sample) {
        myMCMC.randomWalk();
        std::cout << myMCMC << std::endl;
        std::cout << "Sample is: " << myMCMC.X() << std::endl;
    }

//    mySimplex.pivot(3,3);
//    std::cout << mySimplex << std::endl;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void Experiments::GnuplotTest() {
    Gnuplot gp; //(stdout);
    auto state = ModelState<PredPreyAgent>::randomPoissonState([](const PredPreyAgent &agent) {
        if(agent.type() == PredPreyAgent::PREDATOR) return 0.08;
        return 0.16;
    });
    plotHeatMap(gp, state, state);
}

Gnuplot &Experiments::plotHeatMap(Gnuplot &gp, const ModelState<PredPreyAgent> &aggregateState,
                                  const ModelState<PredPreyAgent> &realState) {
    typedef std::tuple<double,double,double,double,double> HeatRecord;
    std::vector<std::vector<HeatRecord>> heatData;
    std::vector<std::tuple<double,double,double>> pointData;

    double maxOccupancy = 0.0;
    for(auto [agent, occupancy] : aggregateState) {
        if(occupancy > maxOccupancy) maxOccupancy = occupancy;
        pointData.emplace_back(agent.xPosition(), agent.yPosition(), agent.type()==PredPreyAgent::PREY?1:2);
    }

    double scale = 255.0/log(maxOccupancy + 1.0);
    for(int x=0; x<PredPreyAgent::GRIDSIZE; ++x) {
        std::vector<HeatRecord> &rabbitRow = heatData.emplace_back();
        for(int y=0; y<PredPreyAgent::GRIDSIZE; ++y) {
            double nRabbits = aggregateState[PredPreyAgent(x,y,PredPreyAgent::PREY)];
            double nFoxes = aggregateState[PredPreyAgent(x,y,PredPreyAgent::PREDATOR)];
            rabbitRow.emplace_back(x,y,log(nRabbits+1.0)*scale,0.0,log(nFoxes+1.0)*scale);
        }
    }

    gp << "set linetype 1 lc 'red'\n";
    gp << "set linetype 2 lc 'blue'\n";
    gp << "plot [-0.5:" << PredPreyAgent::GRIDSIZE-0.5 << "][-0.5:" << PredPreyAgent::GRIDSIZE-0.5 << "] ";
    gp << "'-' with rgbimage notitle, ";
    gp << "'-' with points pointtype 5 pointsize 0.5 lc variable notitle\n";
    gp.send2d(heatData);
    gp.send1d(pointData);
    return gp;
}


Gnuplot &Experiments::plotAgents(Gnuplot &gp, const ModelState<PredPreyAgent> &state) {
    std::vector<std::tuple<double,double,double>> pointData;
    for(auto [agent, occupation]: state) {
        pointData.emplace_back(agent.xPosition(), agent.yPosition(), agent.type()==PredPreyAgent::PREY?1:2);
    }
    gp << "set linetype 1 lc 'red'\n";
    gp << "set linetype 2 lc 'blue'\n";
    gp << "plot [-0.5:" << PredPreyAgent::GRIDSIZE-0.5 << "][-0.5:" << PredPreyAgent::GRIDSIZE-0.5 << "] ";
    gp << "'-' with points pointtype 5 pointsize 0.5 lc variable\n";
    gp.send1d(pointData);
    return gp;
}
