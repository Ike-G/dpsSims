#include "icg.h"

#define LOSS -1
#define GAIN 1

namespace ICG {
Agent::Agent() = default;
Agent::Agent(int initialAction) : action(initialAction) {}
Agent::~Agent() = default;
void Agent::update(float feedback) {}
int Agent::getAction() const {
    return action;
}

// RAgent - Standard for ICG
RAgent::RAgent() = default;
RAgent::~RAgent() = default;
RAgent::RAgent(float a, float b, float e, int ia, int is)
    : Agent(ia), alpha(a), beta(b), epsilon(e), state(is) {
    for (auto& r : rValues) {
        r[0] = 0;
        r[1] = 0;
    }
}
void RAgent::update(float feedback) {
    // Training update
    int sDash;
    float u, uDash;
    sDash = feedback == GAIN ? action : 2+action;
    uDash = rValues[sDash][0] > rValues[sDash][1] ? rValues[sDash][0] : rValues[sDash][1];
    rValues[state][action] = (1-alpha)*rValues[state][action]+alpha*(feedback-rho+uDash);
    // If current strategy agrees with max_a rValues
    if (rValues[state][action] >= rValues[state][!action]) {
        uDash = rValues[sDash][0] > rValues[sDash][1] ? rValues[sDash][0] : rValues[sDash][1];
        u = rValues[state][0] > rValues[state][1] ? rValues[state][0] : rValues[state][1];
        rho = (1-beta)*rho+beta*(feedback+uDash-u);
    }
    state = sDash;

    // Action update
    if (rValues[state][0] == rValues[state][1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = rValues[state][0] > rValues[state][1] ? 0 : 1;
}

// Minimal RAgent
MRAgent::MRAgent() = default;
MRAgent::~MRAgent() = default;
MRAgent::MRAgent(float a, float b, float e, int ia)
    : Agent(ia), alpha(a), beta(b), epsilon(e) {}
void MRAgent::update(float feedback) {
    // Training update
    float uDash;
    uDash = rValues[0] > rValues[1] ? rValues[0] : rValues[1];
    rValues[action] = (1-alpha)*rValues[action]+alpha*(feedback-rho+uDash);
    if (rValues[action] > rValues[!action])
        rho = (1-beta)*rho+beta*feedback;

    // Action update
    if (rValues[0] == rValues[1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = rValues[0] > rValues[1] ? 0 : 1;
}

// QAgent - Standard for ICG
QAgent::QAgent() = default;
QAgent::~QAgent() = default;
QAgent::QAgent(float a, float g, float e, int ia, int is)
    : Agent(ia), alpha(a), gamma(g), epsilon(e), state(is) {
    for (auto& q : qValues) {
        q[0] = 0;
        q[1] = 0;
    }
}
void QAgent::update(float feedback) {
    // Training update
    int sDash = feedback == GAIN ? action : action+2;
    float uDash = qValues[sDash][0] > qValues[sDash][1] ? qValues[sDash][0] : qValues[sDash][1];
    qValues[state][action] = (1-alpha)*qValues[state][action]+alpha*(feedback+gamma*uDash);
    state = sDash;

    // Action update
    if (qValues[state][0] == qValues[state][1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = qValues[state][0] > qValues[state][1] ? 0 : 1;
}

// Minimised QAgent
MQAgent::MQAgent() = default;
MQAgent::~MQAgent() = default;
MQAgent::MQAgent(float a, float g, float e, int ia)
    : Agent(ia), alpha(a), gamma(g), epsilon(e) {
    qValues[0] = 0;
    qValues[1] = 0;
}
void MQAgent::update(float feedback) {
    // Training update
    float uDash = qValues[0] > qValues[1] ? qValues[0] : qValues[1];
    qValues[action] = (1-alpha)*qValues[action]+alpha*(feedback+gamma*uDash);

    // Action update
    if (qValues[0] == qValues[1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = qValues[0] > qValues[1] ? 0 : 1;
}

// HCRAgent
HCRAgent::HCRAgent() = default;
HCRAgent::~HCRAgent() = default;
HCRAgent::HCRAgent(int memSize, int ia) : Agent(ia) {
    for (int i = 0; i < memSize; i++)
        memory.push(std::pair<int, float>(0,0));
}

// Previously had a todo fix but now sorted
void HCRAgent::update(float feedback) {
    if (std::get<0>(memory.front()) > 1) {
        std::cout << "Strategy: " << std::get<0>(memory.front()) << std::endl;
        std::cout << "Memory front: " << std::get<0>(memory.front()) << " " << std::get<1>(memory.front()) << std::endl;
        std::cout << "Memory back: " << std::get<0>(memory.back()) << " " << std::get<1>(memory.back()) << std::endl;
        std::cout << "Memory size: " << memory.size() << std::endl;
    }
    actionRewards[std::get<0>(memory.front())] -= std::get<1>(memory.front());
    actionRewards[action] += feedback;
    memory.pop();
    memory.push(std::pair<int, float>(action, feedback));
    if (actionRewards[0] == actionRewards[1])
        action = randinteger(2);
    else
        action = actionRewards[0] > actionRewards[1] ? 0 : 1;
}

// FPSAgent
FPSAgent::FPSAgent() = default;
FPSAgent::~FPSAgent() = default;
FPSAgent::FPSAgent(int ia) : Agent(ia) {}
void FPSAgent::update(float feedback) {}

std::vector<int> convergenceTime(int numHCR, int numQ, int numR, int numFPS,
                                 int steps, int k, float alpha, float beta,
                                 float gamma, float epsilon, int memory) {
    int n = numHCR+numQ+numFPS+numR;
    if (k == 0 || k > (n-1)/2)
        k = (n-1)/2;
    std::vector<int> history(steps+1);
    std::vector<Agent*> agents(n);
    std::vector<HCRAgent> hcrAgents(numHCR);
    std::vector<QAgent> qAgents(numQ);
    std::vector<RAgent> rAgents(numR);
    std::vector<FPSAgent> fpsAgents(numFPS);
    // Permute list
    std::vector<int> permutation;
    permutation.reserve(n);
    for (int i = 0; i < n; i++) {
        permutation.push_back(i);
    }
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(permutation.begin(), permutation.end(), rng);
    for (int i = 0; i < numQ; i++) {
        qAgents[i] = QAgent(alpha, gamma, epsilon, randinteger(2), randinteger(4));
        agents[permutation[i]] = &qAgents[i];
    }
    for (int i = 0; i < numHCR; i++) {
        hcrAgents[i] = HCRAgent(memory, randinteger(2));
        agents[permutation[i+numQ]] = &hcrAgents[i];
    }
    for (int i = 0; i < numR; i++) {
        rAgents[i] = RAgent(alpha, beta, epsilon, randinteger(2), randinteger(4));
        agents[permutation[i+numQ+numHCR]] = &rAgents[i];
    }
    for (int i = 0; i < numFPS; i++) {
        fpsAgents[i] = FPSAgent(randinteger(2));
        agents[permutation[i+numQ+numHCR+numR]] = &fpsAgents[i];
    }
    int converged = 0;
    for (auto& a : agents) {
        if (a->getAction() == 0)
            converged++;
    }
    history[0] = converged;
    int s1, s2;
    for (int step = 0; step < steps; step++) {
        for (auto& match : randomlyPair(n, k)) {
            Agent* p1 = agents[match.first];
            Agent* p2 = agents[match.second];
            s1 = p1->getAction();
            s2 = p2->getAction();
            if (s1 == s2) {
                p1->update(GAIN);
                p2->update(GAIN);
            } else {
                p1->update(LOSS);
                p2->update(LOSS);
            }
        }
        converged = 0;
        for (auto& a : agents) {
            if (a->getAction() == 0)
                converged++;
        }
        history[step+1] = converged;
    }
    return history;
}
}

// int main() {
//     int t, i;
//     // std::cout << "HCR Convergence with respect to n and k:" << std::endl << std::endl;
//     // for (int n = 10; n < 1300; n*=2) {
//     //     for (int k = 1; k < (int)((float)n/2)+1; k += (int)(((float)(n/2)-1)/3)) {
//     //         t = 0;
//     //         i = 0;
//     //         clock_t start = clock();
//     //         while (i < 100 && float(clock()-start)/CLOCKS_PER_SEC < 600) {
//     //             t += convergenceTime(n, 0, 0, 0.9, k);
//     //             i++;
//     //         }
//     //         std::cout << n << " " << k << " " << (float)t/(float)i << std::endl;
//     //     }
//     // }
//     std::cout << std::endl << "HCR Convergence with respect to FPS presence and k:" << std::endl;
//     for (int nh = 100; nh >= 0; nh-=5) {
//         for (int k = 1; k < 51; k += 12) {
//             t = 0;
//             i = 0;
//             clock_t start = clock();
//             while (i < 100 && float(clock()-start)/CLOCKS_PER_SEC < 600) {
//                 t += convergenceTime(nh, 100-nh, 0, 0.9, k);
//                 i++;
//             }
//             std::cout << nh << " " << k << " " << (float)t/(float)i << std::endl;
//         }
//     }
//     // for (int i = 0; i < 100; i++) {
//     //     std::cout << convergenceTime(500, 500, 200, 0.9, 5*(i+1)) << std::endl;
//     // }
//     // std::cout << "Q Convergence with respect to epsilon: " << std::endl;
//     // for (float i = 0; i < 0.5; i+=0.01) {
//     //     t = 0;
//     //     for (int j = 0; j < 100; j++) {
//     //         t += convergenceTime(0, 1000, 0, 0.95, 0, 0.1, 0.9, i);
//     //     }
//     //     std::cout << i << ": " << (float)t/(float)100 << std::endl;
//     // }
//     return 0;
// }