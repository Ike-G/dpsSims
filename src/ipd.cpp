#include "ipd.h"
#define ALPHA 3.0 // 3
#define BETA 1.0 // 1
#define GAMMA -1.0 // -1
#define DELTA -3.0 // -3
//#define DEBUGQ
//#define DEBUGR
namespace IPD {

std::map<float, int> rewardToState {
        {ALPHA, 0},
        {BETA, 1},
        {GAMMA, 2},
        {DELTA, 3}
};

Agent::Agent() = default;
Agent::Agent(int initialAction) : action(initialAction) {}
Agent::~Agent() = default;
void Agent::update(float feedback) {}
int Agent::getAction() const {
    return action;
}

RAgent::RAgent() = default;
RAgent::RAgent(const float a, const float b, const float e, int ia, int is)
        : Agent(ia), alpha(a), beta(b), epsilon(e), state(is) {
    for (auto& rValue : rValues) {
        rValue[0] = 0, rValue[1] = 0;
    }
}

RAgent::~RAgent() = default;
void RAgent::update(float feedback) {
    int sDash;
    float u, uDash;
    sDash = rewardToState[feedback];
    // The maximum expected average-adjusted value of the next state
    uDash = rValues[sDash][0] > rValues[sDash][1] ? rValues[sDash][0] : rValues[sDash][1];
    // R(s,a) <- r-rho+u'
    rValues[state][action] = (1-alpha)*rValues[state][action]+alpha*(feedback-rho+uDash);
    // rho <- r+u'-u
    // u'-u is the value of the current state relative to the previous state - if it is low then r increases
    uDash = rValues[sDash][0] > rValues[sDash][1] ? rValues[sDash][0] : rValues[sDash][1];
    u = rValues[state][0] > rValues[state][1] ? rValues[state][0] : rValues[state][1];
    // If the current strategy is optimal, update rho
    if (rValues[state][action] >= rValues[state][!action])
        rho = (1-beta)*rho+beta*(feedback+uDash-u);
    state = sDash;

    if (rValues[state][0] == rValues[state][1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = rValues[state][0] > rValues[state][1] ? 0 : 1;
}

void RAgent::display() {
    std::cout << "Rho: " << rho << std::endl;
    std::cout << "Current state: ";
    switch (state) {
        case 0:
            std::cout << "0 (DC)" << std::endl;
            break;
        case 1:
            std::cout << "1 (CC)" << std::endl;
            break;
        case 2:
            std::cout << "2 (DD)" << std::endl;
            break;
        case 3:
            std::cout << "3 (CD)" << std::endl;
            break;
    }
    for (int i = 0; i < 4; i++) {
        std::cout << "R(" << i << ", 0) = " << rValues[i][0] << ", R(" << i << ", 1) = " << rValues[i][1] << std::endl;
    }
    std::cout << "Next strategy: ";
    if (action == 0)
        std::cout << "cooperation" << std::endl;
    else
        std::cout << "defection" << std::endl;
}

// Minimised RAgent - Standard for IPD
MRAgent::MRAgent() = default;
MRAgent::~MRAgent() = default;
MRAgent::MRAgent(float a, float b, float e, int ia)
    : Agent(ia), alpha(a), beta(b), epsilon(e) {
    rValues[0] = 0;
    rValues[1] = 0;
}
void MRAgent::update(float feedback) {
    float uDash = rValues[0] > rValues[1] ? rValues[0] : rValues[1];
    rValues[action] = (1-alpha)*rValues[action]+alpha*(feedback-rho+uDash);
    if (rValues[action] > rValues[!action])
        rho = (1-beta)*rho+beta*feedback;

    // Action update
    if (rValues[0] == rValues[1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = rValues[0] > rValues[1] ? 0 : 1;
}

// QAgent
QAgent::QAgent() = default;
QAgent::QAgent(float a, float g, float e, int ia, int is)
        : Agent(ia), alpha(a), gamma(g), epsilon(e), state(is) {
    for (auto & qValue : qValues) {
        qValue[0] = 0, qValue[1] = 0;
    }
}
QAgent::~QAgent() = default;

void QAgent::update(float feedback) {
    int sDash = rewardToState[feedback];
    float maxValue = qValues[sDash][0] > qValues[sDash][1] ? qValues[sDash][0] : qValues[sDash][1];
    qValues[state][action] = (1-alpha)*qValues[state][action]+alpha*(feedback+gamma*maxValue);
    qValues[state][!action] = (1-alpha)*qValues[state][!action]+alpha*gamma*maxValue;
    state = sDash;

    if (qValues[state][1] == qValues[state][0] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = qValues[state][0] > qValues[state][1] ? 0 : 1;

    // #ifdef DEBUG
    // if ((float)rand()/(float)RAND_MAX < 0.00000000001) {
    //     for (int i = 0; i < 4; i++)
    //         std::cout << "Q(" << i << ", 0) = " << qValues[i][0] << ", Q(" << i << ", 1) = " << qValues[i][1] << std::endl;
    // }
    // #endif
}

void QAgent::display() {
    std::cout << "Current state: ";
    switch (action) {
        case 0:
            std::cout << "0 (DC)" << std::endl;
            break;
        case 1:
            std::cout << "1 (CC)" << std::endl;
            break;
        case 2:
            std::cout << "2 (DD)" << std::endl;
            break;
        case 3:
            std::cout << "3 (CD)" << std::endl;
            break;
    }
    for (int i = 0; i < 4; i++) {
        std::cout << "Q(" << i << ", 0) = " << qValues[i][0] << ", Q(" << i << ", 1) = " << qValues[i][1] << std::endl;
    }
    std::cout << "Next strategy: ";
    if (action == 0)
        std::cout << "cooperation" << std::endl;
    else
        std::cout << "defection" << std::endl;
}

// Minimised Q Agent - Standard for IPD
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
    qValues[!action] = (1-alpha)*qValues[!action]+alpha*gamma*uDash;

    // Action update
    if (qValues[0] == qValues[1] || randreal() < epsilon)
        action = randinteger(2);
    else
        action = qValues[0] > qValues[1] ? 0 : 1;
}

// HCR Agent
HCRAgent::HCRAgent() = default;
HCRAgent::~HCRAgent() = default;
HCRAgent::HCRAgent(int memSize, int ia) : Agent(ia) {
    for (int i = 0; i < memSize; i++)
        memory.push(std::pair<int, float>(0, 0));
}
void HCRAgent::update(float feedback) {
    actionRewards[std::get<0>(memory.front())] -= std::get<1>(memory.front());
    actionRewards[action] += feedback;
    memory.pop();
    memory.push(std::pair<int, float>(action, feedback));
    if (actionRewards[0] > actionRewards[1])
        action = 0;
    else if (actionRewards[1] > actionRewards[0])
        action = 1;
    else
        action = randinteger(2);
}

FPSAgent::FPSAgent() = default;
FPSAgent::FPSAgent(int ia) : Agent(ia) {}
FPSAgent::~FPSAgent() = default;
void FPSAgent::update(float feedback) {}
void FPSAgent::display() {
    printf("Strategy: %d", action);
}

std::vector<int> convergenceTime(int numHCR, int numQ, int numR, int numFPS,
                                 int numMR, int numMQ, int steps, int k,
                                 float alpha, float beta, float gamma,
                                 float epsilon, int memory) {
    int n = numHCR + numQ + numR + numFPS + numMR + numMQ;
    if (k == 0 || k > (n - 1) / 2)
        k = (n - 1) / 2;
    std::vector<int> history(steps+1);
    std::vector<Agent *> agents(n);
    std::vector<HCRAgent> hcrAgents(numHCR);
    std::vector<QAgent> qAgents(numQ);
    std::vector<MQAgent> mqAgents(numMQ);
    std::vector<RAgent> rAgents(numR);
    std::vector<MRAgent> mrAgents(numMR);
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
    int converged = 0;
    // All containing state are initialised assuming hostility
    for (int i = 0; i < numQ; i++) {
        qAgents[i] = QAgent(alpha, gamma, epsilon, randinteger(2), 2);
        agents[permutation[i]] = &qAgents[i];
    }
    for (int i = 0; i < numHCR; i++) {
        // Default 1 for IPD
        hcrAgents[i] = HCRAgent(memory, randinteger(2));
        agents[permutation[i + numQ]] = &hcrAgents[i];
    }
    for (int i = 0; i < numR; i++) {
        rAgents[i] = RAgent(alpha, beta, epsilon, randinteger(2), 2);
        agents[permutation[i + numQ + numHCR]] = &rAgents[i];
    }
    for (int i = 0; i < numFPS; i++) {
        fpsAgents[i] = FPSAgent(randinteger(2));
        agents[permutation[i + numQ + numHCR + numR]] = &fpsAgents[i];
    }
    for (int i = 0; i < numMR; i++) {
        mrAgents[i] = MRAgent(alpha, beta, epsilon, randinteger(2));
        agents[permutation[i + numQ + numHCR + numR + numFPS]] = &mrAgents[i];
    }
    for (int i = 0; i < numMQ; i++) {
        mqAgents[i] = MQAgent(alpha, gamma, epsilon, randinteger(2));
        agents[permutation[i + numQ + numHCR + numR + numFPS + numMR]] = &mqAgents[i];
    }
    for (auto& a : agents) {
        if (a->getAction() == 0)
            converged++;
    }
    history[0] = converged;
    int s1, s2;
#ifdef DEBUGQ
    int traceQ = randinteger(numQ);
#endif
#ifdef DEBUGR
    int traceR = randinteger(numR);
#endif
    for (int step = 0; step < steps; step++) {
#ifdef DEBUGQ
        qAgents[traceQ].display();
#endif
#ifdef DEBUGR
        rAgents[traceR].display();
#endif
        for (auto &match: randomlyPair(n, k)) {
            Agent *p1 = agents[match.first];
            Agent *p2 = agents[match.second];
            s1 = p1->getAction();
            s2 = p2->getAction();
            if (s1 == 0) {
                if (s2 == 0) {
                    p1->update(BETA);
                    p2->update(BETA);
                } else {
                    p1->update(DELTA);
                    p2->update(ALPHA);
                }
            } else {
                if (s2 == 0) {
                    p1->update(ALPHA);
                    p2->update(DELTA);
                } else {
                    p1->update(GAMMA);
                    p2->update(GAMMA);
                }
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