# adri_button

Library for momentary push button, allows to detect the state of the button: the number of clicks or if it is held and to assign functions to each state. 

Supports AVR and ESP8266 platform

<br>



## QuickStart

Create the main instance "bpDebounce_management"
```c++
bpDebounce_management  _bpDebounce_management;
```

### Setup

#### adding a buutton
* void addBp(int pin, boolean pullup, uint8_t mode, int & result);
```c++
       int pin          = boardp in of the button
       boolean pullup   = if the buton is wiring as puulup or pulldown
       uint8_t mode     = INPUT or INPUT_PULLUP 
       int & result     = return the postion of the button instance 
        _bpDebounce_management.addBp(D1, false, INPUT, result);
```
#### adding a callback function
* void callback(int bp, mBPDc mod, _BP_callbackFunc func);
```c++
    int bp                  = postion of the buton instance 
    mBPDc mod               = state of the button
    _BP_callbackFunc func   = function
    _bpDebounce_management.callback(result, mBPDc_click_1, _test_p_21);

```
### Loop
* void loop();
```c++
        _bpDebounce_management.loop();
```
### Featues

* addBp(int pin, boolean pullup, uint8_t mode, int & result);
    * initialise a button
* loop();
    * loop of all buttons
* print();
    * print statu of all buttons
* callback(int bp, mBPDc mod, _BP_callbackFunc func);
    * attribue or change a function of the button state
* callback_toglle(int bp, mBPDc mod);
    * ativate/desctivate the function of the button state

### Little example
```c++
#include <adri_bpDebounce.h>

bpDebounce_management  _bpDebounce_management;

void _test_p_1()    {Serial.print(F("\tSP 1\n"));}
void _test_p_21()   {Serial.print(F("\tSP 21\n"));}
void _test_p_22()   {Serial.print(F("\tSP 22\n"));}
void _test_p_23()   {Serial.print(F("\tSP 23\n"));}
void _test_p_2l()   {Serial.print(F("\tSP 2l\n"));}
void _test_p_2le()  {Serial.print(F("\tSP 2le\n"));}
void setup()
{
    int result;
    _bpDebounce_management.addBp(D1, false, INPUT, result);
    _bpDebounce_management.callback(result, mBPDc_click_1, _test_p_1);
    _bpDebounce_management.addBp(D2, true, INPUT_PULLUP, result);
    _bpDebounce_management.callback(result, mBPDc_click_1, _test_p_21);
    _bpDebounce_management.callback(result, mBPDc_click_2, _test_p_22);
    _bpDebounce_management.callback(result, mBPDc_click_3, _test_p_23);
    _bpDebounce_management.callback(result, mBPDc_longCick_stard, _test_p_2l);
    _bpDebounce_management.callback(result, mBPDc_longCick_end, _test_p_2le);

}
void loop()
{
    _bpDebounce_management.loop();
}
```


## Dependecies

<pre>
Librairies
<details>
adri_timer                      = https://github.com/AdriLighting/adri_timer

adri_bpDebounce                 = https://github.com/AdriLighting/adri_bpDebounce

</details>
Dependency Graph
<details>
|-- [adri_bpDebounce] 1.0.0
|   |-- [adri_timer] 1.0.0
|-- [adri_timer] 1.0.0</details>
</pre>
