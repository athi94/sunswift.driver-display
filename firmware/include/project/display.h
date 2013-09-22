/** @mainpage Driver Display function prototypes, macros and defines */

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <project/Lcd.h>
#include <project/LcdConstants.h>
#include <project/Page.h>
#include <project/Line.h>
#include <project/Parameter.h>
#include <project/Slider.h>
#include <project/Coordinate.h>
#include <project/DataSource.h>
#include <arch/uart.h>
#include <project/driver_config.h>
#include <scandal/engine.h>

DataSource speed1; 
DataSource speed2; 
DataSource bv1;
DataSource bv2;  
DataSource cc1; 
DataSource cc2; 
DataSource pch1; 
DataSource pch2; 
DataSource mct1;
DataSource mv1;
DataSource mi1;
DataSource bi1;

/*! Defining parameters to capture - Source 1, Source 2, Init value, Init time, Parameter Name, Parameter Label, Parameter Units **/ 
Parameter speed; 
Parameter bv;
Parameter cc; 
Parameter pch; 
Parameter mct;
Parameter mv;
Parameter mi;
Parameter bi;
Parameter mp; 
Parameter bp; 

/*! Defining the starting coordinate */
Coordinate startCoordinate = {LCD_START_COORDINATE_X, LCD_START_COORDINATE_Y};

/*! Define the Lcd, its pages and also the associated slider chart and lines */
Lcd l;

/*! \brief Defines all channel message interrupt handler. When the data
 * is received, the following channel handlers are called 
 * For each function, we have the following parameters: 
 * \param value - Value received from the CAN 
 * \param src_time - Time on the scandal when the data was received 
 * from CAN
 */
void in_channel_0_handler(int32_t value, uint32_t src_time){
        speed1.value = (unsigned int)value;
        speed1.time  = (unsigned int)src_time;  
        
        updateParameter(&speed);
}
void in_channel_1_handler(int32_t value, uint32_t src_time){
        speed2.value = (unsigned int)value;
        speed2.time  = (unsigned int)src_time;

       updateParameter(&speed);
}
void in_channel_2_handler(int32_t value, uint32_t src_time){
        bv1.value = (unsigned int)value;
        bv1.time  =(unsigned int) src_time;
        updateParameter(&bv);
}
void in_channel_3_handler(int32_t value, uint32_t src_time){
        bv2.value = (unsigned int)value;
        bv2.time  = (unsigned int)src_time;
        updateParameter(&bv);
}
void in_channel_4_handler(int32_t value, uint32_t src_time){
        cc1.value = (unsigned int)value;
        cc1.time  = (unsigned int)src_time;
        updateParameter(&cc);        
}
void in_channel_5_handler(int32_t value, uint32_t src_time){
        cc2.value = (unsigned int)value;
        cc2.time  = (unsigned int)src_time;
        updateParameter(&cc);
}
void in_channel_6_handler(int32_t value, uint32_t src_time){
        pch1.value = (unsigned int)value;
        pch1.time  = (unsigned int)src_time;            
        updateParameter(&pch);
}
void in_channel_7_handler(int32_t value, uint32_t src_time){
        pch2.value = (unsigned int)value;
        pch2.time  = (unsigned int)src_time;
        updateParameter(&pch);
}
void in_channel_8_handler(int32_t value, uint32_t src_time){
        mct1.value = (unsigned int)value;
        mct1.time  = (unsigned int)src_time;
        updateParameter(&mct);
}
void in_channel_9_handler(int32_t value, uint32_t src_time){
        mv1.value = (unsigned int)value;
        mv1.time  = (unsigned int)src_time;

        updateParameter(&mv);
        updateParameter(&mp);
}
void in_channel_10_handler(int32_t value, uint32_t src_time){
        mi1.value = (unsigned int)value;
        mi1.time  = (unsigned int)src_time;
        updateParameter(&mi);
        updateParameter(&mp);
        
}
void in_channel_11_handler(int32_t value, uint32_t src_time){
        bi1.value = (unsigned int)value;
        bi1.time  = (unsigned int)src_time;
        updateParameter(&bi);
        updateParameter(&bp);
}

void __init__all__(void) {
    /*! Defining the sources of data */     
    __init__DataSource__(&speed1, SCANDAL_IN_CHANNEL_SPEED1_ADDRESS, SCANDAL_IN_CHANNEL_SPEED1_CHANNEL, 13, 1);      
    __init__DataSource__(&speed2, SCANDAL_IN_CHANNEL_SPEED2_ADDRESS, SCANDAL_IN_CHANNEL_SPEED2_CHANNEL, 0, 0);    
    __init__DataSource__(&bv1, SCANDAL_IN_CHANNEL_BV1_ADDRESS, SCANDAL_IN_CHANNEL_BV1_CHANNEL, -1, 1);
    __init__DataSource__(&bv2, SCANDAL_IN_CHANNEL_BV2_ADDRESS, SCANDAL_IN_CHANNEL_BV2_CHANNEL, 0, 0);    
    __init__DataSource__(&cc1, SCANDAL_IN_CHANNEL_CC1_ADDRESS, SCANDAL_IN_CHANNEL_CC1_CHANNEL, 0, 0);        
    __init__DataSource__(&cc2, SCANDAL_IN_CHANNEL_CC2_ADDRESS, SCANDAL_IN_CHANNEL_CC2_CHANNEL, 0, 0);
    __init__DataSource__(&pch1, SCANDAL_IN_CHANNEL_PCH1_ADDRESS, SCANDAL_IN_CHANNEL_PCH1_CHANNEL, 0, 0);
    __init__DataSource__(&pch2, SCANDAL_IN_CHANNEL_PCH2_ADDRESS, SCANDAL_IN_CHANNEL_PCH2_CHANNEL, 0, 0);                
    __init__DataSource__(&mct1, SCANDAL_IN_CHANNEL_MCT_ADDRESS , SCANDAL_IN_CHANNEL_MCT_CHANNEL, 0, 0);        
    __init__DataSource__(&mv1, SCANDAL_IN_CHANNEL_MV_ADDRESS, SCANDAL_IN_CHANNEL_MV_CHANNEL, 0, 0);
    __init__DataSource__(&mi1, SCANDAL_IN_CHANNEL_MI_ADDRESS, SCANDAL_IN_CHANNEL_MI_CHANNEL, 0, 0);    
    __init__DataSource__(&bi1, SCANDAL_IN_CHANNEL_BI_ADDRESS, SCANDAL_IN_CHANNEL_BI_CHANNEL, 0, 0);    
    
    /*! Add the data source to its assocaiated parameters */ 
    __init__Parameter__(&speed, &speed1, NULL, speed1.value, speed1.time, "Speed", "SP", "km/h", NULL, 0);          
    __init__Parameter__(&bv, &bv1, NULL, bv1.value, bv1.time, "Battery Voltage", "BV", "Volts", NULL, 0);
    __init__Parameter__(&cc, &cc1, &cc2, cc1.value, cc1.time, "Cruise Control", "CC", "", NULL, 0);
    __init__Parameter__(&pch, &pch1, &pch2, pch2.value, pch2.time, "Precharge", "PC", "", NULL, 0);
    __init__Parameter__(&mct, &mct1, NULL, mct1.value, mct1.time, "Precharge", "MCT", "", NULL, 0);    
    __init__Parameter__(&mv, &mv1, NULL, mv1.value, mv1.time, "Motor Voltage", "MV", "", NULL, 0);    
    __init__Parameter__(&mi, &mi1, NULL, mi1.value, mi1.time, "Motor Current", "MI", "", NULL, 0);    
    __init__Parameter__(&bi, &bi1, NULL, bi1.value, bi1.time, "Battery Current", "BI", "", NULL, 0);            
    __init__Parameter__(&mp, &cc1, NULL, cc1.value, cc1.time, "Motor Power", "MP", "Watts", &cc, 1);
    __init__Parameter__(&bp, &bv1, NULL, bv1.value, bv1.time, "Battery Power", "BP", "Watts", &bi, 1);
        
    /*! Initialise LCD display parameters. Pages, sliders and Lines are automatically 
     *  created recursively when LCD struct is created*/
    __init__Lcd__(&l, LCD_NO_OF_PAGES);        
      
    /*! Defining the Page start */    
    setPageStart(&l.pages[0], startCoordinate);    
  
    /* Adding sliders to draw */    
#if LCD_NO_OF_SLIDERS > 0 
    setMax(&l.pages[0].sliders[0], 140);
    setMin(&l.pages[0].sliders[0], 0);
    addParameterToSlider(&l.pages[0].sliders[0], &speed);
    
    setMax(&l.pages[0].sliders[1], 160);
    setMin(&l.pages[0].sliders[1], 0);
    addParameterToSlider(&l.pages[0].sliders[1], &bp);

    /*setMax(&l.pages[1].sliders[0], 7000);
    setMin(&l.pages[1].sliders[0], 0);
    addParameterToSlider(&l.pages[1].sliders[0], &speed);        

    setMax(&l.pages[1].sliders[1], 20);
    setMin(&l.pages[1].sliders[1], 0);
    addParameterToSlider(&l.pages[1].sliders[1], &bv);  */
#endif 
    
    /*! Create lines and add parameters */        
    addParameter(&l.pages[0].lines[0],&speed);
    addParameter(&l.pages[0].lines[1],&cc);
    addParameter(&l.pages[0].lines[2],&pch);
    addParameter(&l.pages[0].lines[3],&mp);
    addParameter(&l.pages[0].lines[4],&bp);    
    //addParameter(&l.pages[0].lines[5],NULL);    
    //addParameter(&l.pages[0].lines[6],NULL);
    
    /*addParameter(&l.pages[1].lines[0],NULL);
    addParameter(&l.pages[1].lines[1],&mct);
    addParameter(&l.pages[1].lines[2],&mv);
    addParameter(&l.pages[1].lines[3],&mi);
    addParameter(&l.pages[1].lines[4],&bi);
    */
    
    /*addParameter(&l.pages[1].lines[1],&mct);
    addParameter(&l.pages[1].lines[2],&mp);
    addParameter(&l.pages[1].lines[3],&bp);
    addParameter(&l.pages[1].lines[4],NULL);*/
        
    /*addParameter(&l.pages[1].lines[0],NULL);
    addParameter(&l.pages[1].lines[1],&speed);
    addParameter(&l.pages[1].lines[2],&bv);
    addParameter(&l.pages[1].lines[3],&cc);
    addParameter(&l.pages[1].lines[4],&pch);*/    

    /*Register all channels that represent the source*/
    scandal_register_in_channel_handler(0, &in_channel_0_handler);
    scandal_register_in_channel_handler(1, &in_channel_1_handler);
    scandal_register_in_channel_handler(2, &in_channel_2_handler);
    scandal_register_in_channel_handler(3, &in_channel_3_handler);
    scandal_register_in_channel_handler(4, &in_channel_4_handler);
    scandal_register_in_channel_handler(5, &in_channel_5_handler);
    scandal_register_in_channel_handler(6, &in_channel_6_handler);
    scandal_register_in_channel_handler(7, &in_channel_7_handler);
    scandal_register_in_channel_handler(8, &in_channel_8_handler);
    scandal_register_in_channel_handler(9, &in_channel_9_handler);
    scandal_register_in_channel_handler(10, &in_channel_10_handler);
    scandal_register_in_channel_handler(11, &in_channel_11_handler);    
}

/*! \brief Analog switch control pin to choose between the LCD and uC 
 */
#define TX_CNTL_PORT 1
#define TX_CNTL_PIN  11

/*! \brief Connect TX Line to the LCD. Since FT311D is not present,
 *  we short lines 2 and 3 of the U4 analog switch
 */
#define CONNECT_TO_LCD_PROGRAMMER_INIT GPIO_SetDir(TX_CNTL_PORT, TX_CNTL_PIN, 1);
#define CONNECT_TO_LCD GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 0); 
	
#define CONNECT_TO_PROGRAMMER GPIO_SetValue(TX_CNTL_PORT, TX_CNTL_PIN, 1);

#endif
