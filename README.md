# TurboTune

TurboTune is a macOS kernel extension for tuning of Turbo Boost settings.

TurboTune enables fine-grained control of Turbo Boost behavior on Intel
processors. Instead of simply turning Turbo Boost on/off, it allows creating a
custom turbo curve to balance performance and energy efficiency.

# Disclaimer

- The kernel extension changes Turbo Boost settings by modifying model-specific
  registers (MSR), which may void your CPU's warranty and cause permanent damage
  to the CPU. Proceed with caution.
- The kernel extension is unsigned. System Integrity Protection (SIP) needs to
  be disabled to load the extension.

# Usage

You need to create a custom turbo curve by modifying the `MSR_TURBO_RATIO_LIMIT`
field in `Info.plist` file before loading the kernel extension.
`MSR_TURBO_RATIO_LIMIT` is a model-specific register on Intel processors that
controls the Turbo Boost settings. The format of the register can vary between
processors models. Please refer to *Intel® 64 and IA-32 Architectures Software
Developer’s Manual Volume 4: Model-Specific Registers* to determine the format
for your processor model.

### Example: custom turbo curve for Core i5-8259U

Core i5-8259U is a quad-core processor with a 2.3 GHz base frequency, and a
default turbo frequency limit between 3.8 GHz (when a single core is active) and
3.5 GHz (when all four cores are active). For this processor,
`MSR_TURBO_RATIO_LIMIT` is a 64-bit register where the N-th lowest byte
represents the turbo frequency limit when N cores are active (frequency = byte
value x 100 MHz). With the default policy, the register has a value of
`0x24252626`.

Suppose we would like to save power but do not want to turn off Turbo Boost
entirely (which would limit all cores to the base frequency of 2.3 GHz), we can
define a custom turbo curve with, for example, a single-core turbo of 3.0 GHz,
and an all-core turbo of 2.7 GHz. With this policy, the value of the
`MSR_TURBO_RATIO_LIMIT` register is `0x1B1C1D1E`. To enable this policy, change
the `MSR_TURBO_RATIO_LIMIT` field `Info.plist` to `454827294` (`0x1B1C1D1E` in
decimal) and load the kernel extension.

Profile  | 1-Core | 2-Core | 3-Core | 4-Core | MSR\_TURBO\_RATIO\_LIMIT
-------- | ------ | ------ | ------ | ------ | ------------------------
Default  | 3800 MHz | 3800 MHz | 3600 MHz | 3500 MHz | 0x24252626
Custom   | 3000 MHz | 2900 MHz | 2800 MHz | 2700 MHz | 0x1B1C1D1E
