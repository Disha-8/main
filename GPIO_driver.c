#include<stdint.h>
#include"GPIO_driver.h"



	
	// handle structure of GPIO pin//
#define ENABLE          1
#define DISABLE         0
#define SET             ENABLE
#define RESET           DISABLE
#define GPIO_PIN_SET    SET
#define GPIO_PIN_RESET  RESET 


 

                                 //  API spported by this driver  //

  /*Perpheral clock setup */
/* @fn           GPIO_PCLKControl
   @brief        enables or disables perpheral clock for given gpio port
   @parameters   base address of gpio perpheral
                 enable or disable macros
   @return       none
   @note         none
*/
void GPIO_PCLKControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
		if(pGPIOx==GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if (pGPIOx==GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if (pGPIOx==GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx==GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx==GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx==GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx==GPIOG)
		{
			GPIOG_PCLK_EN();
		}
	}
	else
	{
		if(pGPIOx==GPIOA)
		{
			GPIOA_PCLK_DIS();
		}else if (pGPIOx==GPIOB)
		{
			GPIOB_PCLK_DIS();
		}else if (pGPIOx==GPIOC)
		{
			GPIOC_PCLK_DIS();
		}else if(pGPIOx==GPIOD)
		{
			GPIOD_PCLK_DIS();
		}else if(pGPIOx==GPIOE)
		{
			GPIOE_PCLK_DIS();
		}else if(pGPIOx==GPIOF)
		{
			GPIOF_PCLK_DIS();
		}else if(pGPIOx==GPIOG)
		{
			GPIOG_PCLK_DIS();
		}
	}
}

  /*Init and DeInit */
void GPIO_Init(GPIO_HANDLE_T *pGPIOHandle)
{
	uint32_t temp=0;	//temp. register foe configuration
	uint32_t var=0;   //variable register for speed
	
	GPIO_PCLKControl(pGPIOHandle->pGPIOx,ENABLE);
	//1 configure mode of gpio pin
if(pGPIOHandle->GPIO_PinConfig.GPIO_PinState==GPIO_INPUTMODE)
{
  if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <=GPIO_PUPD_IN)
	{
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <=GPIO_PIN7)
		{
		temp=( pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<< (4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber+2));
	  pGPIOHandle->pGPIOx->CRL &= ~(0x3<<((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)+2));//clearing
		pGPIOHandle->pGPIOx->CRL |=temp; //setting
			
			//FOR SPEED
		pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed=GPIOIN_RESET;              	
		var = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<(4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	 pGPIOHandle->pGPIOx->CRL&=~(0x3<< (4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));//clearing
  pGPIOHandle->pGPIOx->CRL |=var; //setting	
		}else
		{
			temp=( (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<< ((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-30)));
	  pGPIOHandle->pGPIOx->CRH&=~(((0x3<<(4* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-30)));//clearing
		pGPIOHandle->pGPIOx->CRH |=temp; //setting
			
			//FOR SPEED
					pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed=GPIOIN_RESET;
			var = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-32));
	 pGPIOHandle->pGPIOx->CRH&=~(0x3<<((4* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-32));//clearing
  pGPIOHandle->pGPIOx->CRH |=var; //setting
		}
	
}else
	{
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_IN_FT)
		{
			
			EXTI->FTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR&=~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			
			
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_IN_RT)
		{
			EXTI->RTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR&=~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			
		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_IN_RFT)
		{
			EXTI->FTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR|=(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		//config gpio port selection in AFIO_EXTICR
		uint8_t temp1=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
		uint8_t temp2=pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4;
		uint8_t portcode=GPIO_CODE(pGPIOHandle->pGPIOx);
	  AFIO_PCLK_EN();
		AFIO->EXTICR[temp1]= portcode<<(temp2*4);        
		
		//enable exti interrupt delivery using IMR
		EXTI->IMR|=1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
		
	} 
	temp=0;
	var=0;
}else
	{if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <=GPIO_PIN7)
		{
		temp=( pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<< (4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber+2));
	  pGPIOHandle->pGPIOx->CRL &= ~(0x3<<((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)+2));//clearing
		pGPIOHandle->pGPIOx->CRL |=temp; //setting
			
			//FOR SPEED
			var = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<(4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	 pGPIOHandle->pGPIOx->CRL&=~(0x3<< (4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));//clearing
  pGPIOHandle->pGPIOx->CRL |=var; //setting

		}else
		{
			temp=( (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<< ((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-30)));
	   pGPIOHandle->pGPIOx->CRH&=~(((0x3<<(4* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-30)));//clearing
		pGPIOHandle->pGPIOx->CRH |=temp; //setting
			
			//FOR SPEED
			var = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed<<((4*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-32));
	 pGPIOHandle->pGPIOx->CRH&=~(0x3<<((4* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)-32));//clearing
  pGPIOHandle->pGPIOx->CRH |=var; //setting	
		}
}
	
	temp=0; 
  var=0;

}
		
 
	
	
  //5 configure alt functionality

	
	
void GPIO_DeInit(GPIO_TypeDef *pGPIOx)
{
		if(pGPIOx==GPIOA)
		{
			GPIOA_REG_RESET();
		}else if (pGPIOx==GPIOB)
		{
			GPIOB_REG_RESET();
		}else if (pGPIOx==GPIOC)
		{
			GPIOC_REG_RESET();
		}else if(pGPIOx==GPIOD)
		{
			GPIOD_REG_RESET();
		}else if(pGPIOx==GPIOE)
		{
			GPIOE_REG_RESET();
		}else if(pGPIOx==GPIOF)
		{
			GPIOE_REG_RESET();
		}else if(pGPIOx==GPIOG)
		{
			GPIOE_REG_RESET();
}
	}

 /* data read and write */
uint8_t GPIO_ReadFromIPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;
	
	value=(uint8_t)((pGPIOx->IDR >>PinNumber) & 0x00000001);
	
	return value;
}


uint16_t GPIO_ReadFromIPort(GPIO_TypeDef *pGPIOx)
{
	uint16_t value;
	
	value=(uint16_t)pGPIOx->IDR ;
	
	return value;
}


void GPIO_WriteToOPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if(Value==GPIO_PIN_SET)
	{
		//write 1 to output data register at bit field corresponding to pin number//
		pGPIOx->ODR |=(1<<PinNumber);
		//pGPIOx->ODR =pGPIOx->ODR|(1<<PinNumber);
	}else
	{ 
		//write 0
		pGPIOx->ODR&=~(1<<PinNumber);
		//pGPIOx->ODR=pGPIOx->ODR&~(1<<PinNumber);
	}
}
void GPIO_WriteToOPort(GPIO_TypeDef *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR=Value;
}
void GPIO_ToggleOPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR^=(1<<PinNumber);
}

  /*IRQ Configuration and ISR handling */ 
void GPIO_IRQconfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
		if(IRQNumber<=31)
		{
			//program iser0
			*NVIC_ISER0|=(1<<IRQNumber);
			
		}else if(IRQNumber>31&&IRQNumber<64)
		{
			//iser1
			*NVIC_ISER1|=(1<<(IRQNumber%32));
			
		}else if(IRQNumber>=64&&IRQNumber<96)
		{
			//iser2
			*NVIC_ISER2|=(1<<(IRQNumber%64));
		}
}else
	{
		if(IRQNumber<=31)
		{
			//icer0
		*NVIC_ICER0|=(1<<IRQNumber);
			
		}else if(IRQNumber>31&&IRQNumber<64)
		{
			//icer1
			*NVIC_ICER1|=(1<<(IRQNumber%32));
			
		}else if(IRQNumber>=64&&IRQNumber<96)
		{
			//icer2
			*NVIC_ICER2|=(1<<(IRQNumber%64));
		}
	}
	}

	void GPIO_IRQPriority(int IRQNumber,uint32_t IRQPriority)
	{
		//find IPR register
		int iprx=IRQNumber/4;
		int ipry=IRQNumber%4;
		int shift=(8*ipry)+(8-4);
		
		
		*(NVIC_IPR_BASE+(iprx))|=(IRQPriority<<shift);
		
		
	}

void GPIO_IRQHandling(uint8_t PinNumber)
{
	//clear exti pr register for that pin number
	 if(EXTI->PR&(1<<PinNumber))
	 {
		 //clear
		 EXTI->PR|=(1<<PinNumber);
	 }
	
}


