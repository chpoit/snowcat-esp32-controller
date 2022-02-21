#include <Arduino.h>
#include <Bluepad32.h>
#include <vector>
#include "controls/stickdata.h"

#ifndef UTILS_H
#define UTILS_H

namespace utils {

void blinker(void* params);
void blinkController(GamepadPtr gp);
void calibrateSticks(GamepadPtr gp, StickData* stickData);

template <class T>
class LinkedList {
   public:
    LinkedList<T>() {}
    LinkedList<T>(std::initializer_list<T*> items) {
        Item* first = nullptr;
        Item* prev = nullptr;

        for (auto itemData : items) {
            Item* newItem = new Item(itemData);
            if (first == nullptr) {
                first = newItem;
            }
            if (prev != nullptr) {
                prev->next = newItem;
            }
            prev = newItem;
        }
        prev->next = first;
        current = first;
    }
    T* getCurrent() {
        auto k = current->element;
        return k;
    }
    T* getNext() { return next()->element; }

   private:
    class Item {
       public:
        Item(T* element) { this->element = element; }
        Item* next;
        T* element;
    };

    Item* current;
    Item* next() {
        current = current->next;
        return current;
    }
};

}  // namespace utils

#endif