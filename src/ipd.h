#ifndef IPD_H
#define IPD_H
#include "utility.h"

namespace IPD {
    class Agent {
    protected:
        int action{};

    public:
        Agent();
        explicit Agent(int initialAction);
        ~Agent();
        int getAction() const;
        virtual void update(float feedback);
    };

    class QAgent : public Agent {
    private:
        float qValues[4][2]{}, alpha{}, gamma{}, epsilon{};
        int state{};

    public:
        QAgent();
        QAgent(float a, float g, float e, int ia, int is);
        ~QAgent();
        void update(float feedback) override;
        void display();
    };

    class MQAgent : public Agent {
    private:
        float qValues[2]{0,0}, alpha{}, gamma{}, epsilon{};
    public:
        MQAgent();
        MQAgent(float a, float g, float e, int ia);
        ~MQAgent();
        void update(float feedback) override;
    };

    class RAgent : public Agent {
    private:
        float alpha{}, beta{}, epsilon{}, rho{0};
        float rValues[4][2]{};
        int state{};

    public:
        RAgent();
        RAgent(float a, float b, float e, int ia, int is);
        ~RAgent();
        void update(float feedback) override;
        void display();
    };

    class MRAgent : public Agent {
    private:
        float rValues[2]{0,0}, alpha{}, beta{}, epsilon{}, rho{0};
    public:
        MRAgent();
        MRAgent(float a, float b, float e, int ia);
        ~MRAgent();
        void update(float feedback) override;
    };

    class HCRAgent : public Agent {
    private:
        std::queue<std::pair<int, float>> memory;
        float actionRewards[2] { 0, 0 };

    public:
        HCRAgent();
        explicit HCRAgent(int memSize, int ia);
        ~HCRAgent();
        void update(float feedback) override;
    };

    class FPSAgent : public Agent {
    public:
        FPSAgent();
        explicit FPSAgent(int ia);
        ~FPSAgent();
        void update(float feedback) override;
        void display();
    };

    std::vector<int> convergenceTime(int numHCR, int numQ, int numR, int numFPS,
                                     int numMR, int numMQ, int steps = 10000,
                                     int k = 1, float alpha = 0.9, float beta = 0.1,
                                     float gamma = 0.4, float epsilon = 0.05, int memory = 1);
}
#endif