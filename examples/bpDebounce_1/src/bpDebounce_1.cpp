#include <adri_bpDebounce.h>

bpDebounce_management  _bpDebounce_management;

bpDebounce 			* bp_1;
bpDebounceHandle 	* bpH_1;
int _bpChangeFunc = 0;

void _test_p_1() 	{Serial.print(F("\tSP 1\n"));}
void _test_p_1c() 	{Serial.print(F("\tSP 1c\n"));}
void _test_p_21()	{Serial.print(F("\tSP 21\n"));_bpDebounce_management.callback_toglle(_bpChangeFunc, mBPDc_click_1);}
void _test_p_22()	{Serial.print(F("\tSP 22\n"));_bpDebounce_management.callback(_bpChangeFunc, mBPDc_click_1, _test_p_1c);}
void _test_p_23()	{Serial.print(F("\tSP 23\n"));}
void _test_p_2l()	{Serial.print(F("\tSP 2l\n"));}
void _test_p_2le()	{Serial.print(F("\tSP 2le\n"));}
void setup()
{
	Serial.begin(115200);
  	for(unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime > 5000); ) { }
  	Serial.println(F("\n##################################################\n\n"));
	
  	int result;
	_bpDebounce_management.addBp(D1, false, INPUT, result);
	_bpDebounce_management.callback(result, mBPDc_click_1, _test_p_1);
	_bpChangeFunc = result;
	
	// _bpDebounce_management.addBp(D2, true, INPUT_PULLUP, result);
	// _bpDebounce_management.callback(result, mBPDc_click_1, 			_test_p_21);
	// _bpDebounce_management.callback(result, mBPDc_click_2, 			_test_p_22);
	// _bpDebounce_management.callback(result, mBPDc_click_3, 			_test_p_23);
	// _bpDebounce_management.callback_toglle(result, mBPDc_click_3);
	// _bpDebounce_management.callback(result, mBPDc_longCick_stard, 	_test_p_2l);
	// _bpDebounce_management.callback(result, mBPDc_longCick_end, 	_test_p_2le);

	_bpDebounce_management.print();


}
void loop()
{
	_bpDebounce_management.loop();
}