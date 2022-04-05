//
// Created by stormand on 04/04/2022.
//

#ifndef EPS_MCU_FSM_H
#define EPS_MCU_FSM_H

#include <string>
#include "../../include/project_config.h"

class FSM {
public:
    enum state {
        emergency,
        low,
        mid_low,
        mid,
        mid_high,
        high
    };
    enum modules {
        sub1,
        sub2,
        sub3,
        sub4,
        sub5
    };


    explicit FSM(state start_state) {
        current_state_ = start_state;
        update_enabled_(current_state_);
        for (auto &&module: module_override_) {
            module = true;
        };
    }

    void do_state();

    void up_state();

    void down_state();

    state getCurrentState() const;

    int set_override(modules module, bool is_on);

    std::string to_string();

private:

    bool enabled_modules_[SUBMODULES_NUM];
    bool module_override_[SUBMODULES_NUM];

    void update_enabled_(state name);

    std::string get_string_(state name);

    state current_state_;

};


#endif //EPS_MCU_FSM_H
