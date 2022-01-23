//
// Created by IscG on 17/01/2022.
//
#include "ipd.h"
#include "icg.h"

//#define PD

int main() {
    int h = 0;
    int q = 0;
    int r = 0;
    int f = 0;
    int mr = 0;
    int mq = 1000;
    for (h = 10; h <= 10000; h*=10) {
        std::ofstream file;
        std::string filename = "../data/";
        mq = 1000-h;
#ifdef PD
        filename += "ipd/";
#else
        filename += "icg/";
#endif
        filename += std::to_string(h) + "-"
                    + std::to_string(q) + "-"
                    + std::to_string(r) + "-"
                    + std::to_string(f);
#ifdef PD
        filename += "-"+std::to_string(mr)
                +"-"+std::to_string(mq);
#endif
        filename += ".txt";
        file.open(filename);
        for (int i = 0; i < 20; i++) {
#ifdef PD
            std::vector<int> conv = IPD::convergenceTime(h,q,r,f,mr,mq);
#else
            std::vector<int> conv = ICG::convergenceTime(h, q, r, f);
#endif
            for (int j = 0; j < conv.size(); j += 100) {
                file << conv[j] << " ";
            }
            file << "\n";
        }
        file.close();
    }
}
