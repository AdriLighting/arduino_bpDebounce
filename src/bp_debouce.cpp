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
 * @file      bp_debouce.cpp
 *
 * @brief     main file
 * @see       https://github.com/AdriLighting
 * 
 * @author    Adrien Grellard   
 * @date      2021 11:32:26
 *
 */

#include "bp_debouce.h"

// #define DEBUG

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
            case mBPDc_click_1:         result = "click_1";         break;
            case mBPDc_click_2:         result = "click_2";         break;
            case mBPDc_click_3:         result = "click_3";         break;
            case mBPDc_longCick_stard:  result = "longCick_stard";  break;
            case mBPDc_longCick_end:    result = "longCick_end";    break;
            default: break;
        }
        return result;
    }
#endif

// region ################################################ BPDEBOUNCE


bpDebounce * bpDebounceArray[10];


/**
 * @fn         bpDebounce::bpDebounce(int pin, boolean pullup, uint8_t mode) 
 * @brief      constructor instancier par la class bpDebounceHandle
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param[in]  pin     The pin
 * @param[in]  pullup  The pullup
 * @param[in]  mode    The mode
 */
bpDebounce::bpDebounce(int pin, boolean pullup, uint8_t mode){
    _pin    = pin;
    _pullup = pullup;
    _input  = mode;
    pinMode(pin, mode);
}

/**
 * @fn      bpDebounce::statuGet()
 * @brief   GETTER: etat de tension du bouton
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @return  true if > 0v
 */
boolean bpDebounce::statuGet() {
    if (_pullup)    return (digitalRead(_pin)==LOW);
    else            return (digitalRead(_pin)!=LOW);
}
void bpDebounce::statuGet(boolean & result) {
    if (_pullup)    result = (digitalRead(_pin)==LOW);
    else            result = (digitalRead(_pin)!=LOW);
}  

/**
 * @fn      bpDebounce::pinGet(int & result)
 * @brief   GETTER: pin utiliser sur la board
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param   result  = valeur par reference
 */
void bpDebounce::pinGet(int & result) {result = _pin;}

/**
 * @fn      bpDebounce::pullupGet(boolean & result)
 * @brief   GETTER: type de montage Résistance pull down ou Résistance pull up
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param   result  = valeur par reference
 */
void bpDebounce::pullupGet(boolean & result) {result = _pullup;}

/**
 * @fn      bpDebounce::inputGet(uint8_t & result)
 * @brief   GETTER: Utilisation de la Résistance interne/externe
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param   result  = valeur par reference
 */
void bpDebounce::inputGet(uint8_t & result) {result = _input;}
// endregion >>>> BPDEBOUNCE


// region ################################################ BPDEBOUNCE_CALLBACK

/**
 * @fn      bpDebounce_callback::bpDebounce_callback(_BP_callbackFunc * func)
 * @brief   constructor instancier par la class bpDebounceHandle
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param   ptr ver la fonction callback
 */
bpDebounce_callback::bpDebounce_callback(_BP_callbackFunc * func){
    _func = *func;
}

/**
 * @fn      bpDebounce_callback::func_change(_BP_callbackFunc * func)  
 * @brief   changement du ptr de la fonction callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 * 
 * @param   ptr ver la fonction callback
 */
void bpDebounce_callback::func_change(_BP_callbackFunc * func){
    _func = *func;
}

/**
 * @fn      void bpDebounce_callback::func()
 * @brief   appel de la fonction callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 */
void bpDebounce_callback::func(){
    if (!_activate) return;
    _func();
}

/**
 * @fn      void bpDebounce_callback::toglle()
 * @brief   activation/desactivation de l'appel de la fonction callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard 
 */
void bpDebounce_callback::toglle(){
    _activate = !_activate;
}   

/**
 * @fn      void bpDebounce_callback::activate(boolean & ret)
 *
 * @brief   GETTER: etat de kactivation de l'appele du callback
 *
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 *
 * @param   ret     ret  = valeur par reference
 */
void bpDebounce_callback::activate(boolean & ret){
    ret = _activate;
}   
// endregion >>>> BPDEBOUNCE_CALLBACK


// region ################################################ BPDEBOUNCEHANDLE

bpDebounceHandle * bpDebounceHandleArray[10];

/**
 * @fn      bpDebounceHandle::bpDebounceHandle(bpDebounce * ptr)
 * @brief   insatncier via addBp
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param   ptr     ptr de linstance bpDebounce
 */
bpDebounceHandle::bpDebounceHandle(bpDebounce * ptr){
    _bpDebounce = ptr;
    _pressedTimer = new adri_timer(100, false);
}

/**
 * @fn      void bpDebounceHandle::loop()
 * @brief   loop
 *
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 *
 */
void bpDebounceHandle::loop(){
    /* chien de garde pour la prise d'etat du bouton */
    if (_pressedTimer->isActivate()) {_pressedTimer->loop_stop();return;} 

    /* timer now */
    unsigned long sNow = millis();

    /* le bouton et pressé */
    if (_bpDebounce->statuGet()) {
      switch (_statu) {

        /* initialisation par default */
        case mBPD_inactive:
            _lastChange         = sNow;             // reset du timerDiff
            _statu              = mBPD_pressed;     // le bouton recoit un impulsion
            _shortPress_count   = 0;                // remise a 0 compteur de click
        break;

        /* 1er impulsion */
        case mBPD_pressed:

            /* si l"inpulsion depasse (3000ms) */
            if ( (sNow-_lastChange) > _longPress_delay ) {
              _statu      = mBPD_detect_long;     // le bouton recoit un impulsion > a 3000ms
              _longPress  = true;
            }

            /* activation de chien de garde a 100ms si la dernierre impulsion < a 3000ms*/
            if (!_longPress) {
              if (!_pressedTimer->isActivate()) {
                _pressedTimer->set_durationMax(100);
                _pressedTimer->activate();
              }  
            }
        break;

        /* on ne remet pas le compteur de click a 0 */
        case mBPD_detect_short:
          _lastChange = sNow;           // reset du timerDiff
          _statu      = mBPD_pressed;   // le bouton recoit un impulsion    
        break;  

        /* nada */
        case mBPD_detect_long:  break;                                  
        default:                break;
      }  
    }

    /* le bouton est relaché */
    if (!_bpDebounce->statuGet()) {
      switch (_statu) {


        case mBPD_pressed:
            _shortPress_count++;              // incrementation du compteur de click 
            _lastChange = sNow;               // reset du timerDiff
            _statu      = mBPD_detect_short;  // derinierre impulsion inferieur a 3000ms
        break;

        /* si pas d'impulison > 3000ms et que la dernierre impulsion > 250ms on valide le nombre d'inpulsion */
        case mBPD_detect_short:
          if ( (sNow - _lastChange) > _shortPress_delay ) {
            _shortPress  = _shortPress_count;
            _statu       = mBPD_inactive;
          }
        break;

        /* si la dernierre impulsion > 3000ms on valide la fin de l'inpulsion longue */                
        case mBPD_detect_long:
            #ifdef DEBUG
              Serial.printf_P(PSTR("-%20s [%-3d][%-3d][%-3d]\n"), debug_longPressReleased, _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input); 
            #endif
            if (_callback_lp_e != nullptr) _callback_lp_e->func();    // apell de la fonction callback LC
            reset_long();                                             // mise a default des parmaetres 
        break;              
        default: break;
      }           
    }

    /* si le bouton n'est pas relacher > 3000ms */
    if (_statu == mBPD_detect_long) {
      #ifdef DEBUG
        Serial.printf_P(PSTR("%-20s [%-3d][%-3d][%-3d]\n"), debug_longPressHandle, _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input);  
      #endif
      if (_callback_lp_l != nullptr) _callback_lp_l->func();
    }

    /* si pas d'impulison > 3000ms et que la dernierre impulsion > 250ms on lance le callback atrribuer au nombre de click compter*/
    if (_shortPress == 1) {
      #ifdef DEBUG
        Serial.printf_P(PSTR("%-20s [%-3d][%-3d][%-3d][%d]\n"), debug_shortPress, _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input, _shortPress_count); 
      #endif
      if (_callback_sp_1 != nullptr) _callback_sp_1->func();  // apell de la fonction callback 1CLICK
      reset_short();                                          // mise a default des parmaetres 
    }
    if (_shortPress == 2) {
      #ifdef DEBUG
        Serial.printf_P(PSTR("%-20s [%-3d][%-3d][%-3d][%d]\n"), debug_shortPress, _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input, _shortPress_count); 
      #endif
      if (_callback_sp_2 != nullptr) _callback_sp_2->func();  // apell de la fonction callback 2CLIC
      reset_short();                                          // mise a default des parmaetres 
    }
    if (_shortPress == 3) {
      #ifdef DEBUG
        Serial.printf_P(PSTR("%-20s [%-3d][%-3d][%-3d][%d]\n"), debug_shortPress, _bpDebounce->_pin, _bpDebounce->_pullup, _bpDebounce->_input, _shortPress_count); 
      #endif
      if (_callback_sp_3 != nullptr) _callback_sp_3->func();  // apell de la fonction callback 3CLIC
      reset_short();                                          // mise a default des parmaetres 
    }
}

/**
 * @fn      void bpDebounceHandle::reset_short()
 * @brief   reset_short
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 */
void bpDebounceHandle::reset_short(){
    _shortPress         = 0;
    _shortPress_count   = 0;
    _longPress          = false;
    _statu              = mBPD_inactive;
    _lastChange         = millis();       // reset timerDiff
    if (!_pressedTimer->isActivate()) {_pressedTimer->set_durationMax(250);_pressedTimer->activate();}    
}
/**
 * @fn      void bpDebounceHandle::reset_short()
 * @brief   reset_long
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 */
void bpDebounceHandle::reset_long(){
    _shortPress         = 0;
    _shortPress_count   = 0;
    _longPress          = false;
    _statu              = mBPD_inactive;
}   

/**
 * @fn        void bpDebounceHandle::callback(mBPDc mod, _BP_callbackFunc func)
 * @brief     attribution des fonctions callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in] mod   callaback a attribuer suvant limpulsion souhaiter
 * @param[in] func  ptr ver la fonction
 */
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

/**
 * @fn      void bpDebounceHandle::callback_toglle(mBPDc mod)
 * @brief   avtivation/desactivation d_ callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in] mod   type d'impulsion
 */
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

/**
 * @fn        void bpDebounceHandle::callback_isRegister(mBPDc mod, boolean & result)
 * @brief     GETTER: ptr referencer ou pas
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in]  mod     type d'impulsion
 * @param      result  valeure par reference
 */
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

/**
 * @brief   GETTER: callaback avtiver ou pas
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in]  mod     type d'impulsion
 * @param      result  valeure par reference
 */
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

/**
 * @brief      Constructs a new instance (statique instance) 
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 */
bpDebounce_management::bpDebounce_management(){
    #ifdef DEBUG
    #endif  

}

/**
 * @fn      void bpDebounce_management::addBp(int pin, boolean pullup, uint8_t mode, int & result)
 * @brief   Adds a bp.
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in]  pin     The pin
 * @param[in]  pullup  The pullup
 * @param[in]  mode    The mode
 * @param      result  valeure de la postion de larray des instance
 */
void bpDebounce_management::addBp(int pin, boolean pullup, uint8_t mode, int & result){
    bpDebounceArray[_pos]       = new bpDebounce(pin, pullup, mode);
    bpDebounceHandleArray[_pos] = new bpDebounceHandle(bpDebounceArray[_pos]);
    result = _pos;
    _pos++;
}

/**
 * @fn      void bpDebounce_management::loop()
 * @brief   backgroud 
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 */
void bpDebounce_management::loop(){
    for (int i = 0; i < _pos; ++i)
    {
        bpDebounceHandleArray[i]->loop();
    }
}

/**
 * @fn      void bpDebounce_management::callback(int bp, mBPDc mod, _BP_callbackFunc func)
 * @brief   creation des callback
 * 
 * @see     https://github.com/AdriLighting/
 * @author  Adrien Grellard
 * 
 * @param[in]  bp    postion de larray des instance
 * @param[in]  mod   type de callback
 * @param[in]  func  fonction referente
 */
void bpDebounce_management::callback(int bp, mBPDc mod, _BP_callbackFunc func){
    bpDebounceHandleArray[bp]->callback(mod, func);
}

void bpDebounce_management::callback_toglle(int bp, mBPDc mod){
    bpDebounceHandleArray[bp]->callback_toglle(mod);
}  


/* #########################################################################################
 DEBUG
*/ 
#ifdef DEBUG
void padding(String & result, const String & name, const String & value , int len = 25, const char * tdb1 = "\t", const char * sep = " : ",  const char * tdb2 = "",  const char * last = "");
void padding(String & result, const String & name, const String & value , int len, const char * tdb1, const char * sep,  const char * tdb2,  const char * last){
    String s=name;
    int size = s.length();
    while (size<len){
        s+=" ";
        size = s.length();
    }
    result = tdb1+s+sep+tdb2+value+last;
}
#endif
void bpDebounce_management::print(int bp){
#ifdef DEBUG    
    
    uint8_t tempStrArraySize    = 10;
    String * tempStrArray       = new String[tempStrArraySize];
    String paddingStr;

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

    tempStrArray[0] = "BP : " + String(bp) + "\n";
    padding(paddingStr, fsget(p_bpd_pin),       String(pin),    20);  
    tempStrArray[1] =  paddingStr;  
    padding(paddingStr, fsget(p_bpd_pullup),    String(pullup), 20);  
    tempStrArray[2] =  paddingStr;  
    padding(paddingStr, fsget(p_bpd_input),     String(input),  20);  
    tempStrArray[3] =  paddingStr;  
    padding(paddingStr, fsget(p_bpd_statu),     String(statu),  20);  
    tempStrArray[4] =  paddingStr;  

    int cnt = 5; 
    for (int i = mBPDc_click_1; i < mBPDc_none; ++i) {  
        if (callbackRegister[i]) {
            padding(paddingStr, mBPDc_toString(mBPDcArray[i]) , String(callbackActivate[i]), 20); 
            tempStrArray[cnt] =  paddingStr;
        }  else {
            padding(paddingStr, mBPDc_toString(mBPDcArray[i]) , "unregistered", 20);
            tempStrArray[cnt] = paddingStr;
        }
        cnt++;  
    } 
    for (int i = 0; i < tempStrArraySize; ++i){
        Serial.println(tempStrArray[i]);
    }
#endif
}
void bpDebounce_management::print(){
    for (int i = 0; i < _pos; ++i) {print(i);}  
}


 
// endregion >>>> BPDEBOUNCE_MANAGEMENT

