#include "TurboTune.hpp"

#include <i386/proc_reg.h>
#include <libkern/libkern.h>

OSDefineMetaClassAndStructors(TurboTune, IOService)

bool TurboTune::init(OSDictionary *dict) {
    IOLog("TurboTune: Initializing.\n");
    bool status = IOService::init(dict);
    
    if (!status) {
        IOLog("TurboTune: Initialization failed.\n");
        return false;
    }
    
    getKextProperties();
    if (turbo_ratio_limit_ == 0) {
        IOLog("TurboTune: Invalid MSR_TURBO_RATIO_LIMIT from kext.\n");
        return false;
    }
    
    return true;
}

IOService* TurboTune::probe(IOService* provider, SInt32* score) {
    IOService* service = IOService::probe(provider, score);
    return service;
}

bool TurboTune::start(IOService *provider) {
    IOLog("TurboTune: Starting.\n");

    bool status = IOService::start(provider);
    if (!status) {
        IOLog("TurboTune: Start failed.\n");
        return false;
    }

    // Back up original turbo ratio limit.
    original_turbo_ratio_limit_ = getTurboRatioLimit();

    setTurboRatioLimit(turbo_ratio_limit_);

    return true;
}

void TurboTune::stop(IOService *provider) {
    IOLog("TurboTune: Stopping.\n");

    // Restore original turbo ratio limit.
    setTurboRatioLimit(original_turbo_ratio_limit_);
    
    IOService::stop(provider);
}

void TurboTune::free() {
    IOLog("TurboTune: Cleaning up.");
    IOService::free();
}

void TurboTune::getKextProperties() {
    OSNumber* turbo_ratio_limit = OSDynamicCast(OSNumber,
        getProperty("MSR_TURBO_RATIO_LIMIT"));

    turbo_ratio_limit_ = turbo_ratio_limit == nullptr ? 0 :
        turbo_ratio_limit->unsigned64BitValue();
    IOLog("TurboTune: kext MSR_TURBO_RATIO_LIMIT = 0x%llx\n",
          turbo_ratio_limit_);
}

uint64_t TurboTune::getTurboRatioLimit() {
    return rdmsr64(MSR_TURBO_RATIO_LIMIT);
}

void TurboTune::setTurboRatioLimit(uint64_t value) {
    wrmsr64(MSR_TURBO_RATIO_LIMIT, value);
}
