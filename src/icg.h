#ifndef ICG_H
#define ICG_H
#include "utility.h"

namespace ICG {
    class Agent {
    protected:
        int action{};

    public:
        Agent();
        explicit Agent(int initialStrategy);
        ~Agent();
        int getAction() const;
        virtual void update(float feedback);
    };

    class RAgent : public Agent {
    private:
        float rValues[4][2]{}, alpha{}, beta{}, epsilon{}, rho{0};
        int state{};
    public:
        RAgent();
        RAgent(float a, float b, float c, int ia, int is);
        ~RAgent();
        void update(float feedback) override;
    };

    class MRAgent : public Agent {
    private:
        float rValues[2]{0, 0}, alpha{}, beta{}, epsilon{}, rho{0};
    public:
        MRAgent();
        MRAgent(float a, float b, float c, int ia);
        ~MRAgent();
        void update(float feedback) override;
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
    };

    class MQAgent : public Agent {
    private:
        float qValues[2]{0, 0}, alpha{}, gamma{}, epsilon{};
    public:
        MQAgent();
        MQAgent(float a, float g, float e, int ia);
        ~MQAgent();
        void update(float feedback) override;
    };

    class HCRAgent : public Agent {
    private:
        std::queue<std::tuple<int, float>> memory;
        float actionRewards[2] {0, 0};

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
    };

    std::vector<int> convergenceTime(int numHCR, int numQ, int numR, int numFPS,
                                     int steps = 10000, int k = 0,
                                     float alpha = 0.7, float beta = 0.5, float gamma = 0.6,
                                     float epsilon = 0.01, int memory = 2);
}
#endif