//
// Created by stormand on 04/04/2022.
//

#include "FSM.h"

std::string FSM::get_string_(FSM::state name) {
    switch (name) {

        case emergency:
            return "emergency";
            break;
        case low:
            return "low";
            break;
        case mid_low:
            return "mid_low";
            break;
        case mid:
            return "mid";
            break;
        case mid_high:
            return "mid_high";
            break;
        case high:
            return "high";
            break;
        default:
            return "unkown";
    }
}

std::string FSM::to_string() {
    return get_string_(current_state_);
}

void FSM::update_enabled_(FSM::state name) {

    switch (name) {

        case emergency:
            enabled_modules_[0] = false;
            enabled_modules_[1] = false;
            enabled_modules_[2] = false;
            enabled_modules_[3] = false;
            enabled_modules_[4] = false;
            break;
        case low:
            enabled_modules_[0] = true;
            enabled_modules_[1] = false;
            enabled_modules_[2] = false;
            enabled_modules_[3] = false;
            enabled_modules_[4] = false;
            break;
        case mid_low:
            enabled_modules_[0] = true;
            enabled_modules_[1] = true;
            enabled_modules_[2] = false;
            enabled_modules_[3] = false;
            enabled_modules_[4] = false;
            break;
        case mid:
            enabled_modules_[0] = true;
            enabled_modules_[1] = true;
            enabled_modules_[2] = true;
            enabled_modules_[3] = false;
            enabled_modules_[4] = false;
            break;
        case mid_high:
            enabled_modules_[0] = true;
            enabled_modules_[1] = true;
            enabled_modules_[2] = true;
            enabled_modules_[3] = true;
            enabled_modules_[4] = false;
            break;
        case high:
            enabled_modules_[0] = true;
            enabled_modules_[1] = true;
            enabled_modules_[2] = true;
            enabled_modules_[3] = true;
            enabled_modules_[4] = true;

            break;
    }
}

int FSM::set_override(FSM::modules module, bool is_on) {
    switch (module) {

        case sub1:
            module_override_[0] = is_on;
            break;
        case sub2:
            module_override_[1] = is_on;
            break;
        case sub3:
            module_override_[2] = is_on;
            break;
        case sub4:
            module_override_[3] = is_on;
            break;
        case sub5:
            module_override_[4] = is_on;
            break;
    }
    return 0;
}

#include "mbed.h"

DigitalOut* pState[]={
        new DigitalOut(D2),
        new DigitalOut(D3),
        new DigitalOut(D4),
        new DigitalOut(D5),
        new DigitalOut(D6)
};

void FSM::do_state() {
    for (int i = 0; i < SUBMODULES_NUM; ++i) {
        bool output = enabled_modules_[i] and module_override_[i];
        pState[i]->write(output);
    }



}

void FSM::up_state() {
    switch (current_state_) {

        case emergency:
            current_state_ = low;
            break;
        case low:
            current_state_ = mid_low;
            break;
        case mid_low:
            current_state_ = mid;
            break;
        case mid:
            current_state_ = mid_high;
            break;
        case mid_high:
        case high:
            current_state_ = high;
            break;
    }
    update_enabled_(current_state_);
}

void FSM::down_state() {
    switch (current_state_) {

        case emergency:
        case low:
            current_state_ = emergency;
            break;
        case mid_low:
            current_state_ = low;
            break;
        case mid:
            current_state_ = mid_low;
            break;
        case mid_high:
            current_state_ = mid;
            break;
        case high:
            current_state_ = mid_high;
            break;
    }
    update_enabled_(current_state_);
}

FSM::state FSM::getCurrentState() const {
    return current_state_;
}

void FSM::determine_state(uint16_t voltage) {
    float volt = ((float)voltage / UINT16_MAX) * 3.3f; //TODO: ret til noget andet end 3.3V
    state target_state;

    if (volt > VBAT_MID_HIGH) { //TODO: Add other dependencies
        target_state = high;
    } else if (volt > VBAT_MID) {
        target_state = mid_high;
    } else if (volt > VBAT_MID_LOW) {
        target_state = mid;
    } else if (volt > VBAT_EMERGENCY) {
        target_state = mid_low;
    } else {
        target_state = emergency;
    }
    state_changer_(target_state);
}

void FSM::state_changer_(state target_state){
    if (current_state_ < target_state) {
        up_state();
    } else if (target_state < current_state_) {
        down_state();
    }
}