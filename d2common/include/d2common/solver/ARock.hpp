#pragma once

#include "SolverWrapper.hpp"

namespace D2Common {
struct ARockSolverConfig {
    int self_id = 0;
    double rho_frame_T = 0.1;
    double rho_frame_theta = 0.1;
    double rho_landmark = 0.1;
    double eta_k = 0.9;
    int max_steps = 10;
    ceres::Solver::Options ceres_options;
};

class ARockSolver : public SolverWrapper {
protected:
    ARockSolverConfig config;
    double rho_landmark = 0.1;
    double rho_T = 0.1;
    double rho_theta = 0.1;
    int self_id = 0;
    int iteration_count = 0;
    std::map<state_type*, std::map<int, VectorXd>> dual_states_local;
    std::map<state_type*, std::map<int, VectorXd>> dual_states_remote;
    std::map<state_type*, ParamInfo> all_estimating_params;

    virtual void receiveAll() = 0;
    virtual void broadcastData() = 0;
    void addParam(const ParamInfo & param_info);
    void updateDualStates();
    ceres::Solver::Summary solveLocalStep();
    void setDualFactors();
public:
    ARockSolver(D2State * _state, ARockSolverConfig _config):
            SolverWrapper(_state), config(_config), self_id(config.self_id) {
        rho_landmark = config.rho_landmark;
        rho_T = config.rho_frame_T;
        rho_theta = config.rho_frame_theta;
    }
    void reset() override;
    virtual void addResidual(ResidualInfo*residual_info) override;
    ceres::Solver::Summary solve() override;
};
}