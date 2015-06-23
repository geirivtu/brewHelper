#include <stdint.h>

#include "caplesense.h"


#define NOTOUCH 0
#define TOUCH 1

#define STEPDIV 4 //Decides how much movement will result in 1 step registered

void capSenseChTrigger(void); void capSenseScanComplete(void);

void SLIDER_Init(){
    //CAPSENSE_Init();

	  /* Initialize capsense */
	  CAPLESENSE_Init(true);

	  /* Setup capSense callbacks. */
	  CAPLESENSE_setupCallbacks(&capSenseScanComplete, &capSenseChTrigger);

	  /* Setup LESENSE in high-accuracy sense mode. */
	  CAPLESENSE_setupLESENSE(false);
}


static int32_t startVal=0;

int32_t SLIDER_posDiff(){
    

    static int32_t lastVal=0;
    
    static int32_t state = NOTOUCH;
    
    int32_t slider = CAPLESENSE_getSliderPosition(); //CAPSENSE_getSliderPosition();

    if(state==NOTOUCH && slider==-1){
        return 0;
    }else if(state==NOTOUCH && slider!=-1){
        startVal = slider;
        state = TOUCH;
        return 0;
    }else if(state==TOUCH && slider!=-1){
        lastVal = slider;
        return 0;
    }else if(state==TOUCH && slider==-1){
        state = NOTOUCH;
        return (lastVal-startVal)/STEPDIV;
    }else{
        return 0;
    }
}

int32_t SLIDER_posChange(){
    
    int32_t slider = CAPLESENSE_getSliderPosition(); //CAPSENSE_getSliderPosition();
    if( slider==-1){
        return 0;
    }else{
        return (slider-startVal)/STEPDIV;
    }
}



/**************************************************************************//**
 * @brief  Callback for sensor scan complete.
 *****************************************************************************/
void capSenseScanComplete(void){
  
}

/**************************************************************************//**
 * @brief  Callback for sensor channel triggered.
 *****************************************************************************/
void capSenseChTrigger(void){
  ;
}
