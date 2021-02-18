#include "bp_debouce.h"

// #define DEBUG
#ifdef DEBUG
    #include <LittleFS.h>
    #include <adri_logger.h>
    #include <adri_tools_v2.h>  
#endif

mBPDc mBPDcArray[] { 
    mBPDc_click_1,
    mBPDc_click_2,
    mBPDc_click_3,
    mBPDc_longCick_stard,
    mBPDc_longCick_end,
    mBPDc_none
};
#ifdef DEBUG
    String mBPDc_toString(mBPDc mod){
        String result = "";
        switch (mod) {
            case mBPDc_click_1:         result = "click_1"; break;
            case mBPDc_click_2:         result = "click_2"; break;
            case mBPDc_click_3:         result = "click_3"; break;
            case mBPDc_longCick_stard:  result = "longCick_stard"; break;
            case mBPDc_longCick_end:    result = "longCick_end"; break;
            default: break;
        }
        return result;
    }
#endif

// region ################################################ BPDEBOUNCE
bpDebounce * bpDebounceArray[10];

bpDebounce::bpDebounce(int pin, boolean pullup, uint8_t mode){
    _pin    = pin;
    _pullup = pullup;
    _input  = mode;
    pinMode(pin, mode);
}

boolean bpDebounce::statuGet() {
    if (_pullup)    return (digitalRead(_pin)==LOW);
    else            return (digitalRead(_pin)!=LOW);
}

void bpDebounce::statuGet(boolean & result) {
    if (_pullup)    result = (digitalRead(_pin)==LOW);
    else            result = (digitalRead(_pin)!=LOW);
}   
void bpDebounce::pinGet(int & result)           {result = _pin;}
void bpDebounce::pullupGet(boolean & result)    {result = _pullup;}
void bpDebounce::inputGet(uint8_t & result)     {result = _input;}
// endregion >>>> BPDEBOUNCE

// region ################################################ BPDEBOUNCE_CALLBACK
bpDebounce_callback::bpDebounce_callback(_BP_callbackFunc * func){
    _func = *func;
}
void bpDebounce_callback::func_change(_BP_callbackFunc * func){
    _func = *func;
}
void bpDebounce_callback::func(){
    if (!_activate) return;
    _func();
}
void bpDebounce_callback::toglle(){
    _activate = !_activate;
}   
void bpDebounce_callback::activate(boolean & ret){
    ret = _activate;
}   
// endregion >>>> BPDEBOUNCE_CALLBACK

// region ################################################ BPDEBOUNCEHANDLE
bpDebounceHandle * bpDebounceHandleArray[10];

bpDebounceHandle::bpDebounceHandle(bpDebounce * ptr){
    _bpDebounce = ptr;
    _raisedTimer = new adri_timer(100, "", false);
}

void bpDebounceHandle::loop(){

    if (_raisedTimer->isActivate()) {_raisedTimer->loop_stop();return;} 

    unsigned long sNow = millis();
    if (_bpDebounce->statuGet()) {
            switch (_statu) {
                case mBPD_inactive:
                    _lastChange         = sNow;
                    _statu              = mBPD_raised;
                    _shortPress_count   = 0;
                break;
                case mBPD_raised:
                    if ( (sNow-_lastChange) > _longPress_delay ) {
                        _statu      = mBPD_detect_long;
                        _longPress  = true;
                    }
                    if (!_longPress) {
                        if (!_raisedTimer->isActivate()) {_raisedTimer->set_duration_max(100);
                            _raisedTimer->activate();}  
                    }
                break;
                case mBPD_detect_short:
                    _lastChange = sNow;
                    _statu      = mBPD_raised;
                break;  
                case mBPD_detect_long:
                    
                break;                                  
                default:
                    break;
            }  
    }
    if (!_bpDebounce->statuGet()) {
            switch (_statu) {
                case mBPD_raised:
                    _shortPress_count++;
                    _lastChange = sNow;
                    _statu      = mBPD_detect_short;
                break;
                case mBPD_detect_short:
                   if ( (sNow - _lastChange) > _shortPress_delay ) {
                       _shortPress  = _shortPress_count;
                       _statu       = mBPD_inactive;
                   }
                break;                  
                case mBPD_detect_long:
                    #ifdef DEBUG
                    ADRI_LOG(-1, 2, 1, "[%-3d][%-3d][%-3d]callback long_press released", _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input); 
                    #endif
                    if (_callback_lp_e != nullptr) _callback_lp_e->func();
                    reset_long();
                break;              
                default:
                    break;
            }           
    }
    if (_statu == mBPD_detect_long) {
        #ifdef DEBUG
        ADRI_LOG(-1, 2, 1, "[%-3d][%-3d][%-3d]callback long_press", _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input);  
        #endif
        if (_callback_lp_l != nullptr) _callback_lp_l->func();
    }
    if (_shortPress == 1) {
        #ifdef DEBUG
        ADRI_LOGV(-1, 2, 1, _shortPress_count, "[%-3d][%-3d][%-3d]", _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input); 
        #endif
        if (_callback_sp_1 != nullptr) _callback_sp_1->func();
        reset_short();
    }
    if (_shortPress == 2) {
        #ifdef DEBUG
        ADRI_LOGV(-1, 2, 1, _shortPress_count, "[%-3d][%-3d][%-3d]", _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input); 
        #endif
        if (_callback_sp_2 != nullptr) _callback_sp_2->func();
        reset_short();
    }
    if (_shortPress == 3) {
        #ifdef DEBUG
        ADRI_LOGV(-1, 2, 1, _shortPress_count, "[%-3d][%-3d][%-3d]", _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input); 
        #endif
        if (_callback_sp_3 != nullptr) _callback_sp_3->func();
        reset_short();
    }
}

void bpDebounceHandle::reset_short(){
    _shortPress         = 0;
    _shortPress_count   = 0;
    _longPress          = false;
    _statu              = mBPD_inactive;
    _lastChange         = millis();
    if (!_raisedTimer->isActivate()) {_raisedTimer->set_duration_max(250);_raisedTimer->activate();}    
}
void bpDebounceHandle::reset_long(){
    _shortPress         = 0;
    _shortPress_count   = 0;
    _longPress          = false;
    _statu              = mBPD_inactive;
}   

void bpDebounceHandle::callback(mBPDc mod, _BP_callbackFunc func){
    switch (mod) {
        case mBPDc_click_1:
            if (_callback_sp_1 == nullptr) _callback_sp_1 = new bpDebounce_callback(&func);
            if (_callback_sp_1 != nullptr) _callback_sp_1->func_change(&func);
        break;
        case mBPDc_click_2:
            if (_callback_sp_2 == nullptr) _callback_sp_2 = new bpDebounce_callback(&func);
            if (_callback_sp_2 != nullptr) _callback_sp_2->func_change(&func);
        break;
        case mBPDc_click_3:
            if (_callback_sp_3 == nullptr) _callback_sp_3 = new bpDebounce_callback(&func);
            if (_callback_sp_3 != nullptr) _callback_sp_3->func_change(&func);
        break;
        case mBPDc_longCick_stard:
            if (_callback_lp_l == nullptr) _callback_lp_l = new bpDebounce_callback(&func);
            if (_callback_lp_l != nullptr) _callback_lp_l->func_change(&func);
        break;
        case mBPDc_longCick_end:
            if (_callback_lp_e == nullptr) _callback_lp_e = new bpDebounce_callback(&func);
            if (_callback_lp_e != nullptr) _callback_lp_e->func_change(&func);
        break;                      
        default:
        break;
    }
}   
void bpDebounceHandle::callback_toglle(mBPDc mod){
    switch (mod) {
        case mBPDc_click_1:         if (_callback_sp_1 != nullptr) _callback_sp_1->toglle(); break;
        case mBPDc_click_2:         if (_callback_sp_2 != nullptr) _callback_sp_2->toglle(); break;
        case mBPDc_click_3:         if (_callback_sp_3 != nullptr) _callback_sp_3->toglle(); break;
        case mBPDc_longCick_stard:  if (_callback_lp_l != nullptr) _callback_lp_l->toglle(); break;
        case mBPDc_longCick_end:    if (_callback_lp_e != nullptr) _callback_lp_e->toglle(); break;                     
        default:
        break;
    }
} 
void bpDebounceHandle::callback_isRegister(mBPDc mod, boolean & result){
    result = false;
    switch (mod) {
        case mBPDc_click_1:         if (_callback_sp_1 != nullptr) result = true; break;
        case mBPDc_click_2:         if (_callback_sp_2 != nullptr) result = true; break;
        case mBPDc_click_3:         if (_callback_sp_3 != nullptr) result = true; break;
        case mBPDc_longCick_stard:  if (_callback_lp_l != nullptr) result = true; break;
        case mBPDc_longCick_end:    if (_callback_lp_e != nullptr) result = true; break;                        
        default: result = false; break;
    }
} 
void bpDebounceHandle::callback_isActivate(mBPDc mod, boolean & result){
    result = false;
    switch (mod) {
        case mBPDc_click_1:         if (_callback_sp_1 != nullptr) _callback_sp_1->activate(result); break;
        case mBPDc_click_2:         if (_callback_sp_2 != nullptr) _callback_sp_2->activate(result); break;
        case mBPDc_click_3:         if (_callback_sp_3 != nullptr) _callback_sp_3->activate(result); break;
        case mBPDc_longCick_stard:  if (_callback_lp_l != nullptr) _callback_lp_l->activate(result); break;
        case mBPDc_longCick_end:    if (_callback_lp_e != nullptr) _callback_lp_e->activate(result); break;                     
        default: result = false; break;
    }
}       
// endregion >>>> BPDEBOUNCEHANDLE

// region ################################################ BPDEBOUNCE_MANAGEMENT
bpDebounce_management::bpDebounce_management(){
    #ifdef DEBUG
        LittleFS.begin();
        new adri_toolsV2();
        new adriToolsLogger();      
    #endif  

}

void bpDebounce_management::addBp(int pin, boolean pullup, uint8_t mode, int & result){
    bpDebounceArray[_pos]       = new bpDebounce(pin, pullup, mode);
    bpDebounceHandleArray[_pos] = new bpDebounceHandle(bpDebounceArray[_pos]);
    result = _pos;
    _pos++;
}
void bpDebounce_management::loop(){
    for (int i = 0; i < _pos; ++i)
    {
        bpDebounceHandleArray[i]->loop();
    }
}
#ifdef DEBUG
fs(p_bpd_pin,       "pin");
fs(p_bpd_pullup,    "pullup");
fs(p_bpd_input,     "input");
fs(p_bpd_statu,     "statu");
#endif
void bpDebounce_management::print(int bp){
#ifdef DEBUG    
    
    adri_toolsV2* _tools = adri_toolsv2Ptr_get();
    if (_tools->tempStrArray != nullptr) delete[] _tools->tempStrArray;
    _tools->tempStrArraySize    = 10;
    _tools->tempStrArray        = new String[_tools->tempStrArraySize];

    int         pin;
    boolean     statu;
    boolean     pullup;
    uint8_t     input;
    boolean     callbackRegister[5];
    boolean     callbackActivate[5];

    bpDebounceHandleArray[bp]->_bpDebounce->pinGet(pin);
    bpDebounceHandleArray[bp]->_bpDebounce->pullupGet(pullup);
    bpDebounceHandleArray[bp]->_bpDebounce->inputGet(input);
    bpDebounceHandleArray[bp]->_bpDebounce->statuGet(statu);
    for (int i = mBPDc_click_1; i < mBPDc_none; ++i) {
        bpDebounceHandleArray[bp]->callback_isRegister(mBPDcArray[i], callbackRegister[i]); 
        if (callbackRegister[i]) {
            bpDebounceHandleArray[bp]->callback_isActivate(mBPDcArray[i], callbackActivate[i]); 
        }       
    } 

    _tools->tempStrArray[0] = "BP : " + String(bp) + "\n";
    _tools->tempStrArray[1] = _tools->info_parm(fsget(p_bpd_pin),       String(pin),    20);     
    _tools->tempStrArray[2] = _tools->info_parm(fsget(p_bpd_pullup),    String(pullup), 20);     
    _tools->tempStrArray[3] = _tools->info_parm(fsget(p_bpd_input),     String(input),  20);     
    _tools->tempStrArray[4] = _tools->info_parm(fsget(p_bpd_statu),     String(statu),  20);     
    int cnt = 5; 
    for (int i = mBPDc_click_1; i < mBPDc_none; ++i) {  
        if (callbackRegister[i]) {
            _tools->tempStrArray[cnt] = _tools->info_parm( mBPDc_toString(mBPDcArray[i]) , String(callbackActivate[i]), 20);  
        }  else {
            _tools->tempStrArray[cnt] = _tools->info_parm( mBPDc_toString(mBPDcArray[i]) , "unregistered", 20);  
        }
        cnt++;  
    } 

    _tools->tempStr_print();  
#endif
}
void bpDebounce_management::print(){
    for (int i = 0; i < _pos; ++i) {print(i);}  
}
void bpDebounce_management::callback(int bp, mBPDc mod, _BP_callbackFunc func){
    bpDebounceHandleArray[bp]->callback(mod, func);
}
void bpDebounce_management::callback_toglle(int bp, mBPDc mod){
    bpDebounceHandleArray[bp]->callback_toglle(mod);
}   
// endregion >>>> BPDEBOUNCE_MANAGEMENT

