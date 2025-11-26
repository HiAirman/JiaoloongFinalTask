//
// Created by HiAir on 2025/11/26.
//

#ifndef JIAOLOONGFINALTASK_PID_H
#define JIAOLOONGFINALTASK_PID_H


class PID {
public:
    PID(void):
        PID(0, 0, 0, 0, 0, 0.001) {}

    PID(float kp, float ki, float kd, float i_max, float out_max, float d_filter_k = 1, float time = 0.001);

    void reset(void);
    float calc(float ref, float fdb);

    float kp_, ki_, kd_, d_filter_k_;
    float i_max_, out_max_;
    float output_;

private:
    float ref_, fdb_;
    float err_, err_sum_ = 0.0f, last_err_ = 0.0f;
    float pout_, last_pout_ = 0.0f, iout_ = 0, dout_ = 0, last_dout_ = 0.0f;
    float time_;
    bool is_last_err_updated_ = false;
};


#endif //JIAOLOONGFINALTASK_PID_H