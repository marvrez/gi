#include "loading_bar.h"

#include "utils.h"

#include <stdio.h>

LoadingBar::LoadingBar(int max_val, const char* start_msg) 
    : cur_val(0), max_val(max_val), t0(TimeNow()), start_msg(start_msg)
{
}

// small utility to get string if its not a nullptr, else return an empty string
static inline const char* GetString(const char* str) { return str != nullptr ? str : ""; }

void LoadingBar::Display(const char* end_msg)
{
    int pct = this->cur_val*100 / this->max_val;
    double elapsed = TimeNow() - this->t0;
    printf("%s%4d / %d (%3d%%) [", GetString(this->start_msg), this->cur_val, this->max_val, pct);
    for(int p = 0; p < 100; p += 3) {
        if(pct > p) printf("=");
        else printf(" ");
    }
    printf("] %.3fs %s    \r", elapsed, GetString(end_msg));
    fflush(stdout);
}

void LoadingBar::Update()
{
    std::lock_guard<std::mutex> guard(this->mtx);
    cur_val++;
    this->Display();
}

void LoadingBar::Done(const char* end_msg)
{
    this->Display(end_msg);
    printf("\n");
}
