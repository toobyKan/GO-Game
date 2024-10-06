#ifndef OBSERVER_H
#define OBSERVER_H

class Observer {
public:
    virtual void update() = 0;  // Pure virtual function, must be implemented by derived classes
};

#endif