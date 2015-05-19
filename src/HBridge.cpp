#import "./HBridge.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Pin *pinWith(int num) {
    // pinMode(num, OUTPUT);
	Pin *p = (Pin *)malloc(sizeof(Pin));
	p->pin = num;
    return p;
}

void swap_pins(Pin *a, Pin *b) {
        Pin temp = *a;
        *a = *b;
        *b = temp;
}

#ifdef __APPLE__
void print_pin(const char * name, Pin *p){
    printf("%s->pin %d\n",name,p->pin);
}
void print_bridge(Bridge *b){
    print_pin("Bridge Use", b->inuse);
    print_pin("Bridge Idle", b->idle);
    printf("Freq:%d\nDir:%d\n",b->freq,b->direction);

}
#endif

void HBridge::turnPinOFF(hbridge_pins pin) {
    pins[pin]->freq = PIN_OFF;
}

HBridge::HBridge(int pinAA, int pinAB, int pinBA, int pinBB) {

	bridge_a.inuse = pinWith(pinAA);
    bridge_a.idle  = pinWith(pinAB);
    bridge_b.inuse = pinWith(pinBA);
    bridge_b.idle  = pinWith(pinBB);

    /*---(DEFAULT DIRECTIONS)---*/
    bridge_a.direction = PIN_FORWARD;
    bridge_b.direction = PIN_FORWARD;
    bridge_a.freq = PIN_OFF;
    bridge_b.freq = PIN_OFF;

    /*---(ARRAY OF POINTERS TO THE TWO PINS FOR EASY ACCESS BY INDEX)---*/
    pins[0] =&bridge_a;
    pins[1] =&bridge_b;
}

void HBridge::write(int pin, int freq) {
    pins[pin]->freq = freq;
}

void HBridge::setDirection(hbridge_pins pin, int dir) {
    if ((pins[pin]->direction == PIN_FORWARD && dir == PIN_FORWARD) || (pins[pin]->direction == PIN_BACKWARD && dir == PIN_BACKWARD)) {

    } else if (pins[pin]->direction == PIN_FORWARD && dir == PIN_BACKWARD) {

        /*----(SWAP POINTERS)----*/
        swap_pins(pins[pin]->inuse, pins[pin]->idle);
        pins[pin]->direction = PIN_BACKWARD;

    } else if (pins[pin]->direction == PIN_BACKWARD && dir == PIN_FORWARD) {

        /*----(SWAP POINTERS)----*/
        swap_pins(pins[pin]->inuse, pins[pin]->idle);
        pins[pin]->direction = PIN_BACKWARD;

    }
}

void HBridge::update(void)/*pointer to an int to save memory*/ {
    /*THE FIRST BRIDGE*/
    analogWrite(pins[0]->inuse->pin, pins[0]->freq);
    analogWrite(pins[0]->idle->pin, pins[0]->PIN_OFF);

    // THE SECOND BRIDGE
    analogWrite(pins[1]->inuse->pin, pins[1]->freq);
    analogWrite(pins[1]->idle->pin, pins[1]->PIN_OFF);
}

#ifdef __APPLE__
void HBridge::print(void){

    printf("---------\nBRIDGE ONE\n");
    print_bridge(pins[0]);

    printf("---------\nBRIDGE TWO\n");
    print_bridge(pins[1]);
}
#endif
/*
int main(int argc, const char*argv[]){
    HBridge b(1,2,3,4);
    b.setPinFrequency(A,200);
    b.setPinDir(A, PIN_BACKWARD);
    b.print();
    b.setPinDir(A, PIN_FORWARD);
    b.print();
}
*/
