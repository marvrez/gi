#ifndef LOADING_BAR_H
#define LOADING_BAR_H

#include <mutex>

class LoadingBar {
private:
    std::mutex mtx;
    int cur_val; // current progress value
    const int max_val; // maximum progress value
    double t0; // start time
    const char* start_msg; // message to display before the loading bar

    void Display(const char* end_msg=nullptr);
public:
    LoadingBar(int max_val, const char* start_msg=nullptr);
    void Update();
    void Done(const char* end_msg=nullptr);
};

#endif
