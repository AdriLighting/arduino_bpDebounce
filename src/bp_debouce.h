/*
    MIT License

    Copyright (c) 2021 Adrien Grellard

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
/**
 * @file      bp_debouce.h
 *
 * @brief     main file
 * @see       https://github.com/AdriLighting
 * 
 * @author    Adrien Grellard   
 * @date      jeu. 13 dec. 2021 11:32:26
 *
 */

#ifndef BP_DEBOUCE_H
#define BP_DEBOUCE_H

#include <Arduino.h>
#include <adri_timer.h>

// #define DEBUG
#ifdef DEBUG

static const char debug_shortPress 				[] PROGMEM = "shortPress";
static const char debug_longPress 				[] PROGMEM = "longPress";
static const char debug_longPressHandle 		[] PROGMEM = "longPress callback";
static const char debug_longPressReleased 		[] PROGMEM = "longPress released";

static const char p_bpd_pin 		[] PROGMEM = "pin";
static const char p_bpd_pullup 		[] PROGMEM = "pullup";
static const char p_bpd_input 		[] PROGMEM = "input";
static const char p_bpd_statu 		[] PROGMEM = "statu";

#define fsget(parm_a) String(FPSTR(parm_a))

#endif

enum mBPD
{
	mBPD_inactive,
	mBPD_raised,
	mBPD_detect_long,
	mBPD_detect_short,
	mBPD_long_end,
};
enum mBPDc
{
	mBPDc_click_1,
	mBPDc_click_2,
	mBPDc_click_3,
	mBPDc_longCick_stard,
	mBPDc_longCick_end,
	mBPDc_none,

};
// region ################################################ BPDEBOUNCE
class bpDebounce {
private:


public:
	int 		_pin = -1;
	boolean 	_pullup = true;
	uint8_t 	_input;

	bpDebounce(int pin, boolean pullup, uint8_t mode);

    boolean statuGet();
    void 	statuGet(boolean & result);	
    void 	pinGet(int & result);
    void 	pullupGet(boolean & result);
    void 	inputGet(uint8_t & result);
};	
// endregion >>>> BPDEBOUNCE

// region ################################################ BPDEBOUNCE_CALLBACK
typedef void (*_BP_callbackFunc)();  
class bpDebounce_callback {
private:
	mBPDc 				_mod		= mBPDc_none;
	_BP_callbackFunc	_func 		= NULL;
	boolean 			_activate 	= true;	
public:

	bpDebounce_callback(_BP_callbackFunc * func);
	void activate(boolean & ret);
	void toglle();
	void func();
	void func_change(_BP_callbackFunc * func);
};	
// endregion >>>> BPDEBOUNCE_CALLBACK

// region ################################################ BPDEBOUNCEHANDLE
class bpDebounceHandle {
private:
	bpDebounce_callback * _callback_sp_1 = nullptr;
	bpDebounce_callback * _callback_sp_2 = nullptr;
	bpDebounce_callback * _callback_sp_3 = nullptr;
	bpDebounce_callback * _callback_lp_l = nullptr;
	bpDebounce_callback * _callback_lp_e = nullptr;


	uint32_t 	_longPress_delay 	= 3000;
	boolean 	_longPress 			= false;

	uint8_t 	_shortPress_delay 	= 255;
	uint8_t 	_shortPress_count	= 0;
	uint8_t 	_shortPress			= 0;

	boolean 	_bPread;
	mBPD 		_statu 				= mBPD_inactive;
	uint32_t	_lastChange;

	boolean 	_reset 				= false;
	adri_timer 	* _raisedTimer;

public:
	bpDebounce * _bpDebounce;


	bpDebounceHandle(bpDebounce * ptr) ;
	void loop();
	void reset_short();
	void reset_long();
	void callback(mBPDc mod, _BP_callbackFunc func);
	void callback_toglle(mBPDc mod);
	void callback_isRegister(mBPDc mod, boolean & result);
	void callback_isActivate(mBPDc mod, boolean & result);
	
};	
// endregion >>>> BPDEBOUNCEHANDLE

// region ################################################ BPDEBOUNCE_MANAGEMENT
class bpDebounce_management {
private:
	int _pos = 0;
public:
	bpDebounce_management();
	void addBp(int pin, boolean pullup, uint8_t mode, int & result);
	void loop();
	void callback(int bp, mBPDc mod, _BP_callbackFunc func);
	void callback_toglle(int bp, mBPDc mod);
	void print(int bp);
	void print();
};	
// endregion >>>> BPDEBOUNCE_MANAGEMENT


#endif // BP_DEBOUCE_H
