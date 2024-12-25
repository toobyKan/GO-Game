#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include "../headers/observer.hpp"

#include <vector>

class Subject {
    public:     
        void attachObserver(Observer* observer);
        void detachObserver(Observer* observer);
        void notifyObservers();
    
    private:
        std::vector<Observer*> observers_;
};

#endif
