#ifndef __HBridge__
#define __HBridge__

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

//-----------------------|
#define PIN_OFF 0      //|
#define PIN_ON  1      //|
//-----------------------|
#define PIN_FORWARD 0  //|
#define PIN_BACKWARD 1 //|
//-----------------------|
#define PIN_B 1
#define PIN_A 0



typedef struct{
    int pin;
} Pin;

typedef struct {
    Pin *inuse;
    Pin *idle;
    int freq;
    int direction;
}Bridge;

typedef enum{ A, B } hbridge_pins;



class HBridge {
    private:
    	Bridge bridge_a, bridge_b;
      Bridge *pins[2];
	  public:
        HBridge(int, int, int, int);
		void write(int, int);
        void setDirection(int, int);
		void update(void);
#ifdef __APPLE__
    void print();
#endif
};

#endif
