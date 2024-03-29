/*__________________________________________________________________________________
|       Disciplina de Sistemas Embarcados - 2022-1
|       Prof. Douglas Renaux
| __________________________________________________________________________________
|
|		Lab 1
|       Alunos: Gustavo Zeni 
|               Lucas Perin
| __________________________________________________________________________________
*/

/**
 * @file     main.cpp
 * @author   Douglas P. B. Renaux
 * @brief    Solution to Lab1 of ELF74/CSW41 - UTFPR. \n 
 *           Tools instalation and validation procedure.\n 
 *           Show messages on terminal using std::cout. \n 
 *           Show current value of some predefined macros (preprocessor symbols).\n 
 *           Read float value from terminal using std::cin.
 * @version  V2 -for 2022-1 semester
 * @date     Feb, 2022
 ******************************************************************************/

/*------------------------------------------------------------------------------
 *
 *      File includes
 *
 *------------------------------------------------------------------------------*/
#include <stdint.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include "template.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "../../TivaWare_C_Series-2.2.0.295/inc/tm4c1294ncpdt.h"


/*------------------------------------------------------------------------------
 *
 *      Typedefs and constants
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Global vars
 *
 *------------------------------------------------------------------------------*/
	volatile float tempo = 0;
	volatile int32_t timer = 0;
        volatile int32_t chave = 0;
        volatile int32_t valor = 0;
        volatile int32_t clocks = 0;
        volatile int32_t on = 0;
        uint32_t max_tick = 15000000;
	uint32_t ui32SysClock; //Clock em Hz

/*------------------------------------------------------------------------------
 *
 *      File scope vars
 *
 *------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 *
 *      Functions and Methods
 *
 *------------------------------------------------------------------------------*/
        
//Declarations
void initGPIO(void);
void initInterrupt(void);
//void GPIOJIntHandler(void);

extern void SysTickIntHandler(void)
{
    timer++;
}

extern void GPIOJIntHandler(void)
{	
    GPIOIntDisable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
    if (on)
    {
      SysTickDisable();
      chave = 1;
    }
    else
      on = 1;
	
   GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
}
/**
 * Main function.
 *
 * @param[in] argc - not used, declared for compatibility
 * @param[in] argv - not used, declared for compatibility
 * @returns int    - not used, declared for compatibility
 */
int main(int argc, char ** argv)
{


  ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                   SYSCTL_OSC_MAIN |
                                   SYSCTL_USE_PLL |
                                   SYSCTL_CFG_VCO_240), 120000000);

  initGPIO();
  initInterrupt();

	
  while (1)
  {
	tempo = 0.0;
        chave = 0;
        timer = 0;
        valor = 0;
        clocks = 0;
        on = 1;
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1); //LED Ligado
        SysTickPeriodSet(max_tick);
        SysTickEnable(); //Contador
	while((timer < 24) && (!chave))
	{
            
	}
	GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0); //LED Desligado
	if (chave)
        {
          // Current iteration 
          valor = SysTickValueGet();
          tempo = max_tick - valor; //Systick conta decrementando
          tempo = tempo/max_tick;
          tempo += timer; //Tempo em quartos (1/4) de segundo
          tempo = tempo * 125; //Tempo em ms
          clocks = timer * max_tick; 
          clocks = clocks + (max_tick - valor); //Numero de clocks
          cout <<"Tempo de resposta: " << clocks << " clocks, "<< tempo << "ms" << "\n";
        }
        else
        {
          cout <<"Tempo m�ximo de 3s atingido\n";
          SysTickDisable();
        }
        on = 0;
        cout <<"Pressione SW1 para recomecar\n\n";
        while (!on){}
  }
}	

void initGPIO(void) {
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); //LED
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); //Switch
  while(!(SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)&&SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ))){}

  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
  GPIODirModeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  

  SysTickIntEnable();
}

void initInterrupt()
{
 
    // Enables the specified GPIO interruputs
   GPIOIntEnable(GPIO_PORTJ_BASE, GPIO_INT_PIN_0);
  
   // Set the specified type in the specified pin
   GPIOIntTypeSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);
   
   // Enable the interrupt for PORTJ.
   IntEnable(INT_GPIOJ);
   
   // Set the priority
   IntPrioritySet(INT_GPIOJ, 0x00);
     
   IntMasterEnable(); // Enable the Master key of interrupt.
    
}


