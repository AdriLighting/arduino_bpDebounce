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
 * @date      2021 11:32:26
 *
 */

#ifndef BP_DEBOUCE_H
#define BP_DEBOUCE_H  

#include <Arduino.h>
#include <adri_timer.h>

// #define DEBUG
#ifdef DEBUG

static const char debug_shortPress        [] PROGMEM = "shortPress";
static const char debug_longPress         [] PROGMEM = "longPress";
static const char debug_longPressHandle   [] PROGMEM = "longPress callback";
static const char debug_longPressReleased [] PROGMEM = "longPress released";

static const char p_bpd_pin       [] PROGMEM = "pin";
static const char p_bpd_pullup    [] PROGMEM = "pullup";
static const char p_bpd_input     [] PROGMEM = "input";
static const char p_bpd_statu     [] PROGMEM = "statu";

#define fsget(parm_a) String(FPSTR(parm_a))

#endif

/** 
 * @enum    mBPD
 * @brief   mod pour indiquer l"état binaire du bouton
 *          
 * @see     https://github.com/AdriLighting
 * 
 * @author  Adrien Grellard   
 * @date      2021 11:32:26
 */
enum mBPD
{
  mBPD_inactive,      /**<  */
  mBPD_pressed,       /**< pressé */
  mBPD_detect_long,   /**< pressé et maintenu */
  mBPD_detect_short,  /**< pressé rapidement */
  mBPD_long_end,      /**< fin de la pression maintenu  */
};

/** 
 * @enum    mBPDc
 * @brief   mod pour indiquer le type de callback utiliser 
 * @see     https://github.com/AdriLighting
 * 
 * @author  Adrien Grellard   
 * @date      2021 11:32:26
 */
enum mBPDc
{
  mBPDc_click_1,          /**< pressé rapidement 1fois */
  mBPDc_click_2,          /**< pressé rapidement 2fois */
  mBPDc_click_3,          /**< pressé rapidement 3fois */
  mBPDc_longCick_stard,   /**< tant que la pression est maintenu */
  mBPDc_longCick_end,     /**< fin de la pression maintenu */
  mBPDc_none,             /**< default */

};
// region ################################################ BPDEBOUNCE

/**
 * @class   bpDebounce
 * 
 * @brief   class pour l'initialisation du bouton'
 * @see     https://github.com/AdriLighting/
 * 
 * @author  Adrien Grellard 
 * @date      2021 11:32:26
 */
class bpDebounce {
public:
  int       _pin = -1;        /**< \brief pin utiliser */
  boolean   _pullup = true;   /**< \brief Montage Résistance pull down ou Résistance pull up */
  /*
      INPUT         pull up externe.
      INPUT_PULLUP  pull up interne.
  */
  uint8_t   _input;           /**< \brief Utilisation de la Résistance interne/externe */

  bpDebounce(int pin, boolean pullup, uint8_t mode);

  boolean statuGet();
  void  statuGet(boolean & result); 
  void  pinGet(int & result);
  void  pullupGet(boolean & result);
  void  inputGet(uint8_t & result);
};  
// endregion >>>> BPDEBOUNCE


// region ################################################ BPDEBOUNCE_CALLBACK

typedef void (*_BP_callbackFunc)(); 

/**
 * @class   bpDebounce_callback
 * 
 * @brief   class pour la gestion des fonction callback atrribuer a l'état du bouton
 * @see     https://github.com/AdriLighting/
 * 
 * @author  Adrien Grellard 
 * @date      2021 11:32:26
 */
class bpDebounce_callback {
private:
  mBPDc             _mod        = mBPDc_none;   /**< \brief type de pression */
  _BP_callbackFunc  _func       = NULL;         /**< \brief callback */
  boolean           _activate   = true;         /**< \brief activation/desactivation du callback */
public:

  bpDebounce_callback(_BP_callbackFunc * func);
  void activate(boolean & ret);
  void toglle();
  void func();
  void func_change(_BP_callbackFunc * func);
};  
// endregion >>>> BPDEBOUNCE_CALLBACK


// region ################################################ BPDEBOUNCEHANDLE

/**
 * @class   bpDebounceHandle
 * 
 * @brief   class pour la gestion de l'etat du bouton
 * @see     https://github.com/AdriLighting/
 * 
 * @author  Adrien Grellard 
 * @date      2021 11:32:26
 */
class bpDebounceHandle {
private:
  bpDebounce_callback * _callback_sp_1 = nullptr; /**< \brief ptr du callback attribuer pour un simple click */
  bpDebounce_callback * _callback_sp_2 = nullptr; /**< \brief ptr du callback attribuer pour un double click */
  bpDebounce_callback * _callback_sp_3 = nullptr; /**< \brief ptr du callback attribuer pour un triple click */
  bpDebounce_callback * _callback_lp_l = nullptr; /**< \brief ptr du callback attribuer pendant une longue pression */
  bpDebounce_callback * _callback_lp_e = nullptr; /**< \brief ptr du callback attribuer apres une longue pressionk */


  uint32_t  _longPress_delay  = 3000;   /**< \brief durée minmum pour que la detection longue soit valider */
  boolean   _longPress        = false;  /**< \brief valide la pression longue */

  uint8_t   _shortPress_delay   = 255;  /**< \brief durée pendant laquelle l'incremenation du nombre de pression est faite */
  uint8_t   _shortPress_count   = 0;    /**< \brief incrementation entre chaque pression  */
  uint8_t   _shortPress         = 0;    /**< \brief resulta une foit la durée finit */

  // boolean   _bPread;                    /**< \brief  */
  mBPD      _statu = mBPD_inactive;     /**< \brief mod de l'état du bouton */
  uint32_t  _lastChange;                /**< \brief timer diff */

  // boolean     _reset = false;           /**< \brief   */
  adri_timer  * _pressedTimer;          /**< \brief  */

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
