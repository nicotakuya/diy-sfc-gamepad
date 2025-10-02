// super famicom joystick by takuya matsubara

// SFC connnector
//  ---------+------------+
// |(7)(6)(5)|(4)(3)(2)(1)|
//  ---------+------------+
// (1)VCC : 5V
// (2)CLK : clock (rising edge=change data)
// (3)P/S : latch (low=latch / high=loading)
// (4)DAT : data
// (7)GND : ground

// P/S
// +-+
// | |
// + +---------------------------------------------------

// CLK
// -----+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---
//      |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  
//     

//  DAT(gamepad)
//   +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//   | B  Y SL ST UP DW LT RT  A  X  L  R| 0  0  0  0|
// --+--+--+--+--+--+--+--+--+--+--+--+--+           +---

#define B     2
#define Y     3
#define SEL   4
#define START 5
#define UP    6
#define DOWN  7
#define LEFT  8
#define RIGHT 9
#define A     10
#define X     11
#define L     12
#define R     13

#define PINCLK  A1
#define PINPS   A2
#define PINDAT  A3
  
void setup() {
  int i;
  
  for(i=2; i<=13; i++){
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(PINCLK, INPUT_PULLUP);
  pinMode(PINPS, INPUT_PULLUP);
  pinMode(PINDAT, OUTPUT);
}

void loop() {
  unsigned char bitcnt;
  unsigned int work;
  unsigned int mask;

  while(1){
    work = 0xFFFF;
    mask = 0x8000;
    for(bitcnt=2; bitcnt<=13; bitcnt++){
      if(digitalRead(bitcnt)==LOW){
        work ^= mask;
      }
      mask >>=1;
    }    
    mask = 0x8000;
    while((PINC & (1<<2))!=0);  // while(digitalRead(PINPS)==HIGH);
    while((PINC & (1<<2))==0);  // while(digitalRead(PINPS)==LOW);
    for(bitcnt=0; bitcnt<16; bitcnt++){
      while((PINC & (1<<1))==0);  // while(digitalRead(PINCLK)==LOW);
      if(work & mask){
        PORTC |= (1<<3);  // digitalWrite(PINDAT, HIGH);
      }else{
        PORTC &= ~(1<<3); // digitalWrite(PINDAT, LOW);
      }
      mask >>= 1;
      while((PINC & (1<<1))!=0);  // while(digitalRead(PINCLK)==HIGH);
    }
  }
}



