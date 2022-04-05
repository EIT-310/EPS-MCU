//
// Created by stormand on 04/04/2022.
//

#include "nv_store.h"

FlashIAPBlockDevice bd;
TDBStore nvStore(&bd);

std::string NVStore::get_key_str(NVStore::DATA_LOCATION name) {
    std::string key;
    switch (name) {
        case sm_state:
            key = "_0000";
            break;
        case adc_read:
            key = "_0001";
            break;
    }
    return key;
}

int NVStore::write(NVStore::DATA_LOCATION key, void *value) {
    int ret = bd.init();
    if (ret) {
        return ret;
    }

    ret = nvStore.init();
    if (ret) {
        return ret;
    }

    string key_str = get_key_str(key);

    ret = nvStore.set(key_str.c_str(), value, sizeof(value), 0);
    if (ret) {
        return ret;
    }

    ret = nvStore.deinit();
    if (ret) {
        return ret;
    }

    ret = bd.deinit();
    if (ret) {
        return ret;
    }

    return 0;
}
int NVStore::read(NVStore::DATA_LOCATION key, void *value) {
    int ret = bd.init();
    if (ret) {
        return ret;
    }

    ret = nvStore.init();
    if (ret) {
        return ret;
    }

    string key_str = get_key_str(key);

    ret = nvStore.get(key_str.c_str(), value, sizeof(value));
    if (ret) {
        return ret;
    }

    ret = nvStore.deinit();
    if (ret) {
        return ret;
    }

    ret = bd.deinit();
    if (ret) {
        return ret;
    }

    return 0;
}
