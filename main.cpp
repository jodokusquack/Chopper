#include <Arduino.h>
#include <Servo.h>
#include <U8g2lib.h>
 
Servo servo;  // create servo object to control a servo
U8G2_SSD1306_128X64_NONAME_1_HW_I2C screen(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); // The display

// t is the delay interval between each shutter switch in s; Initialize with 5s
int t = 5;

// define the Pins and states
const int start_stop_button = 3;
const int manual_button  = 2;

const int p1_button    = 9;
const int p10_button   = 8;
const int m10_button   = 7;
const int m1_button    = 6;

const int servo_pin    = 11;

// Button states
byte old_p1_state = HIGH;
byte old_p10_state = HIGH;
byte old_m10_state = HIGH;
byte old_m1_state = HIGH;
byte old_manual_state = HIGH;

// a value to keep track of the running state of the program;
bool running = false;
// a value to keep track of the shutter state
bool open_state = false;

// time variables
unsigned long start_time;
unsigned long current_time;
unsigned int diff;
unsigned int number_of_switches;
unsigned int new_number_of_switches;

// line heigt and start height in pixels for the display
#define LINE_HEIGHT 18
#define START_HEIGHT 10

// Function prototypes
void start_stop();
void center_text(char* s, int line, bool underline=false);
void center_int(int i, int line);
void change_shutter_state();

//////////////////
// Setup & Loop //
//////////////////

void setup() {
  // Set input and outputs
  pinMode(start_stop_button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(start_stop_button), start_stop, FALLING);
  attachInterrupt(digitalPinToInterrupt(manual_button), change_shutter_state, FALLING);
  pinMode(manual_button, INPUT_PULLUP);
  pinMode(p1_button, INPUT_PULLUP);
  pinMode(p10_button, INPUT_PULLUP);
  pinMode(m10_button, INPUT_PULLUP);
  pinMode(m1_button, INPUT_PULLUP);

  servo.attach(servo_pin);

  screen.begin();
}
 
void loop() {
  // Switching the shutter
  if (running) {
    // alternate the shutter by calling change_shutter_state()
    current_time = millis();
    diff = (current_time - start_time) / 1000;
    new_number_of_switches = diff / t; // integer division!!
    // number_of_switches describes the amount of times that the chopper should have moved.
    if (new_number_of_switches > number_of_switches) {
      // if t fits in diff one more time:
      change_shutter_state();
      number_of_switches = new_number_of_switches;
    }
  } else {
    // read buttons
    byte current_p1_state = digitalRead(p1_button);
    byte current_p10_state = digitalRead(p10_button);
    byte current_m10_state = digitalRead(m10_button);
    byte current_m1_state = digitalRead(m1_button);
    byte current_manual_state = digitalRead(manual_button);

    // alter t when button is pressed
    // +1
    if (current_p1_state != old_p1_state) {
      old_p1_state = current_p1_state;
      if (current_p1_state == LOW) {
        // means the button was just pushed down
        t = t+1;
      }
    }
    // +10
    if (current_p10_state != old_p10_state) {
      old_p10_state = current_p10_state;
      if (current_p10_state == LOW) {
        // means the button was just pushed down
        t = t+10;
      }
    }
    // -10
    if (current_m10_state != old_m10_state) {
      old_m10_state = current_m10_state;
      if (current_m10_state == LOW) {
        // means the button was just pushed down
        t = t-10;
      }
    }
    // -1
    if (current_m1_state != old_m1_state) {
      old_m1_state = current_m1_state;
      if (current_m1_state == LOW) {
        // means the button was just pushed down
        t = t-1;
      }
    }
    // minimum t is 1s:
    if (t < 1) {
      t = 1;
    }
  }

  // Writing the information to the screen, see U8g2 documentation for more info
  screen.firstPage();
  do {
    screen.setFont(u8g2_font_7x14B_mr);
    if (running) {
      String info = String("Int. = " + String(t) + " s");
      int str_len = info.length() + 1;
      char info_array[str_len];
      info.toCharArray(info_array, str_len);
      center_text(info_array, 0);
      center_text("State:", 1, true);
      if (open_state) {
        center_text("Open", 2);
      } else {
        center_text("Closed", 2);
      }
      // print the timer:
      center_int(diff, 3);
    } else {
      center_text("Not running", 0, true);
      if (open_state) {
        center_text("Status: Open", 1);
      } else {
        center_text("Status: Close", 1);
      }
      center_text("Interval (s):", 2);
      center_int(t, 3);
    }
  } while ( screen.nextPage() );
}

//////////////////////////
// Function definitions //
//////////////////////////

void start_stop() {
  running = !running;
  if (running) {
    start_time = millis();
    number_of_switches = 0;
  }
}

void change_shutter_state() {
  if (open_state) {
    // you might change these values, depending on the orientation of your servo, also I have found that the 100° here are better than only 90°.
    servo.write(180);
  } else {
    servo.write(80);
  }
  open_state = !open_state;
}

// the following functions are helper functions for displaying text in a nice way. 
// (there might be a much better way to do what I am trying to do, but I just dont get Strings in C)
void center_text(char* s, int line, bool underline=false) {
  int w = screen.getStrWidth(s);
  int center_dist = (124 - w)/2;

  if (underline) {
    int str_len = strlen(s);
    char str[str_len];
    for (int i = 0; i < str_len; i++) {
      str[i] = '-';
    }
    screen.drawStr(center_dist, START_HEIGHT + LINE_HEIGHT * line + 8, str)  ;
  }
  screen.drawStr(center_dist, START_HEIGHT + LINE_HEIGHT * line, s);
}
void center_int(int i, int line) {
  // convert the int to a char Array
  char i_array[16]; // max lenght of the i is i think 15 digits in this case
  itoa(i, i_array, 10); // 10 is the base of the number
  int w = screen.getStrWidth(i_array);
  int center_dist = (124 - w) / 2;
  screen.drawStr(center_dist, START_HEIGHT + LINE_HEIGHT * line, i_array);
}
