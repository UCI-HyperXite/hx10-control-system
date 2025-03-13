#ifndef HIGHVOLTAGESYSTEM_HPP
#define HIGHVOLTAGESYSTEM_HPP

struct HighVoltageSystem {
    int pin;
    void (*enable)(struct HighVoltageSystem* self);
    void (*disable)(struct HighVoltageSystem* self);
};

void intializeHighVoltageSystem(struct HighVoltageSystem* highVoltageSystem);
void enable(struct HighVoltageSystem* self);
void disable(struct HighVoltageSystem* self);

#endif