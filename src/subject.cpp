#include "../headers/subject.hpp"

#include <algorithm>

void Subject::attachObserver(Observer* observer){
                observers_.push_back(observer);
            }
void Subject::detachObserver(Observer* observer){
    observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }
void Subject::notifyObservers(){
    for (Observer* observer : observers_) {
        observer->update();
    }
}