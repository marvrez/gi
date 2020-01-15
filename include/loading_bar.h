#ifndef LOADING_BAR_H
#define LOADING_BAR_H

#include <mutex>

class LoadingBar {
private:
    std::mutex mtx;
    int cur_val; // current progress value
    const int max_val; // maximum progress value
    double t0; // start time

    void Display(const char* end_msg=nullptr);
public:
    LoadingBar(int max_val);
    void Update();
    void Done(const char* end_msg=nullptr);
};

#endif
