/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char* name, const char* value) {
    size_t valuelen = strlen(value);

    prop_info* pi = (prop_info*)__system_property_find(name);
    if (pi != nullptr) {
        __system_property_update(pi, value, valuelen);
    } else {
        __system_property_add(name, strlen(name), value, valuelen);
    }
}

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
    auto device = GetProperty("ro.product.product.device", "");
    auto rf_version = std::stoi(GetProperty("ro.boot.rf_version", "0"));
    auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));

    switch (prjname) {
        case 21603: // CN
               OverrideProperty("ro.product.product.model", "RMX3361");
            break;
        case 21675: // IN
               OverrideProperty("ro.product.product.model", "RMX3360");
            break;
        case 21676: // EU
               OverrideProperty("ro.product.product.model", "RMX3363");
            break;
        default:
            LOG(ERROR) << "Unexpected project name: " << prjname;
    }
}