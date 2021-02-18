#ifndef BP_DEBOUCE_H
#define BP_DEBOUCE_H

#include <Arduino.h>
#include <adri_timer.h>

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
