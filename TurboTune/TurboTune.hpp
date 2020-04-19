#ifndef TURBOTUNE_HPP_
#define TURBOTUNE_HPP_

#include <IOKit/IOLib.h>
#include <IOKit/IOService.h>

class TurboTune : public IOService
{
    OSDeclareDefaultStructors(TurboTune)

  public:
    virtual bool init(OSDictionary *dict) override;
    IOService* probe(IOService* provider, SInt32* score) override;
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
    virtual void free(void) override;
    
  private:
    // Read parameters from kext property list.
    void getKextProperties(void);
    uint64_t getTurboRatioLimit(void);
    void setTurboRatioLimit(uint64_t value);
    
    // Turbo ratio limit to be set.
    uint64_t turbo_ratio_limit_;
    // Original turbo ratio limit.
    uint64_t original_turbo_ratio_limit_;
    
    // Model-Specific Register for Turbo Boost ratio. See Intel Software
    // Developer's Manual Volume 4: Model-Specific Registers.
    static const uint32_t MSR_TURBO_RATIO_LIMIT = 0x1ad;
};

#endif  // TURBOTUNE_HPP_
