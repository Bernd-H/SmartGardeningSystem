/*
	This library was originally copyright of Michael at elechouse.com but permision was
    granted by Wilson Shen on 2016-10-23 for me (Simon Monk) to uodate the code for Arduino 1.0+
    and release the code on github under the MIT license.

	Updated to work for STM32F103RB.
		Lukas Zrout
		23.06.2021
		HTBL Hollabrunn

	SPI2
	GDIO0 is PA8
*/
#include <CC1101.h>
#include <Arduino.h>

/****************************************************************/
#define 	WRITE_BURST     	0x40						//write burst
#define 	READ_SINGLE     	0x80						//read single
#define 	READ_BURST      	0xC0						//read burst
#define 	BYTES_IN_RXFIFO     0x7F  						//byte number in RXfifo

/****************************************************************/
byte PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};

/****************************************************************
*FUNCTION NAME:SpiInit
*FUNCTION     :spi communication initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::SpiInit(void)
{
  // initialize the SPI pins
  /*pinMode(SCK_PIN, OUTPUT);
  pinMode(MOSI_PIN, OUTPUT);
  pinMode(MISO_PIN, INPUT);
  pinMode(SS_PIN, OUTPUT);*/
  	RCC->APB2ENR|=0xC;
	GPIOB->CRH&=0x0000FFFF; 
	GPIOB->CRH|=0xB4B30000; //SO, SCKL, CS alt out push pull; SI input floating

  // enable SPI Master, MSB, SPI mode 0, FOSC/4
  //SpiMode(0);
  //Clock f�r SPI2 aktivieren
	RCC->APB1ENR|=0x4000;
	
	// Bit1=0(CPOL, LOW Idle) Bit0=0(CPHA, falling edge)
	SPI2->CR1&=0xC;
	
	//DFF Data Frame Format Bit 11, 0=> 8Bit-Format
	SPI2->CR1&=0x7FF;
	
	//Master Mode enable
	SPI2->CR1|=0x4;
	
	//Enable Software Chip Select (Input von NSS ersetzt durch Wert in SSI Bit)
	SPI2->CR1|=0x200;
	SPI2->CR1&=0xBF; //SSI=0
	
	//Set SSOE On, Multimaster m�glich, kein Ausgang von SPI, braucht man wenn NSS ein Output ist,
	SPI2->CR2|=0x4;
	
	//Baud Rate
	//periph clock /256 -> ...111...
	SPI2->CR1|=0x38;
	
	//LSB first
	SPI2->CR1&=0x7F;
	
	//SPI Enable
	SPI2->CR1|=0x40;
	
	//CS HIGH
	//GPIOB->BSRR|=0x1000;
}


/****************************************************************
*FUNCTION NAME:SpiTransfer
*FUNCTION     :spi transfer
*INPUT        :value: data to send
*OUTPUT       :data to receive
****************************************************************/
byte CC1101::SpiTransfer(byte value)
{
	//Kommentare darüber ansehen!
	while(!((SPI2->SR>>1)&0x1)); //Solange etwas anderes gesendet wird
	SPI2->DR=value;					//Danach, Paramter senden
	while(!((SPI2->SR>>1)&0x1)); //Solange etwas anderes gesendet wird
	while(!(SPI2->SR&0x1)); //Solange bis buffer empfangen voll ist
  	return SPI2->DR;
}

/****************************************************************
*FUNCTION NAME: GDO_Set()
*FUNCTION     : set GDO0,GDO2 pin
*INPUT        : none
*OUTPUT       : none
****************************************************************/
void CC1101::GDO_Set (void)
{
	int temp;
	temp = GPIOB->CRH;  // Konfigurationsbits PB8-PB15 in temp sichern 
	temp &= 0xFFFFFF00;	// PB8 und PB9Konfigurationsbits l�schen	
	temp |= 0x00000088;	// PB8 und PB9 im Moduls Push Pull definieren
	GPIOB->CRH = temp;  // Neue Konfiguration in Konfigurationsregister sichern 
	/*pinMode(GDO0, INPUT);
	pinMode(GDO2, INPUT);*/
}

void delay(int ms){
	//Warteschleife, 1 ms entsprechen etwa 14355 iterierungen einer schleife
	int i,j;
	for(i = 0; i < ms; i++) {
		for(j = 0; j < 14355; j++) {
	   	}
	}
} 

/****************************************************************
*FUNCTION NAME:Reset
*FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Reset (void)
{
	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;
	delay(1);
	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
	delay(1);
	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;
	
	while((GPIOB->IDR>>14)&0x0001);
	//while(digitalRead(MISO_PIN));
	SpiTransfer(CC1101_SRES);
	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);
	GPIOB->BSSR|=0x1000;
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Init(void)
{
	SpiInit();										//spi initialization
	GDO_Set();			
	GPIOB->BSRR|=1000;
	GPIOB->BSRR|=2000; //SPI AKTIV UND GPIO VERÄNDERUNG!!! Möglich?!?
	GPIOB->BRR|=8000;	//SPI AKTIV UND GPIO VERÄNDERUNG!!! Möglich?!?	

	GPIOA->CRH&=0xFFFFFFF0; 
	GPIOA->CRH|=0x00000004; //PA8 general INPUT 

	/*digitalWrite(SS_PIN, HIGH);
	digitalWrite(SCK_PIN, HIGH);
	digitalWrite(MOSI_PIN, LOW);*/
	Reset();										//CC1101 reset
	RegConfigSettings(F_433);						//CC1101 register config
	SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}

/****************************************************************
*FUNCTION NAME:Init
*FUNCTION     :CC1101 initialization
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::Init(byte f)
{
	SpiInit();										//spi initialization
	GDO_Set();										//GDO set
	GPIOB->BSRR|=1000;
	GPIOB->BSRR|=2000; //SPI AKTIV UND GPIO VERÄNDERUNG!!! Möglich?!?
	GPIOB->BRR|=8000;	//SPI AKTIV UND GPIO VERÄNDERUNG!!! Möglich?!?

	GPIOA->CRH&=0xFFFFFFF0; 
	GPIOA->CRH|=0x00000004; //PA8 general INPUT floating

	//digitalWrite(SS_PIN, HIGH);
	//digitalWrite(SCK_PIN, HIGH);
	//digitalWrite(MOSI_PIN, LOW);
	Reset();										//CC1101 reset
	RegConfigSettings(f);							//CC1101 register config
	SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);		//CC1101 PATABLE config
}


/****************************************************************
*FUNCTION NAME:SpiWriteReg
*FUNCTION     :CC1101 write data to register
*INPUT        :addr: register address; value: register value
*OUTPUT       :none
****************************************************************/
void CC1101::SpiWriteReg(byte addr, byte value)
{
	GPIOB->BSRR|=0x1000;
	//digitalWrite(SS_PIN, LOW);

	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);
	SpiTransfer(addr);
	SpiTransfer(value);
	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
}

/****************************************************************
*FUNCTION NAME:SpiWriteBurstReg
*FUNCTION     :CC1101 write burst data to register
*INPUT        :addr: register address; buffer:register value array; num:number to write
*OUTPUT       :none
****************************************************************/
void CC1101::SpiWriteBurstReg(byte addr, byte *buffer, byte num)
{
	byte i, temp;

	temp = addr | WRITE_BURST;
    //digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;

    //while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);

    SpiTransfer(temp);
    for (i = 0; i < num; i++)
 	{
        SpiTransfer(buffer[i]);
    }

    //digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
}

/****************************************************************
*FUNCTION NAME:SpiStrobe
*FUNCTION     :CC1101 Strobe
*INPUT        :strobe: command; //refer define in CC1101.h//
*OUTPUT       :none
****************************************************************/
void CC1101::SpiStrobe(byte strobe)
{
	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;
	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);
	SpiTransfer(strobe);
	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
}

/****************************************************************
*FUNCTION NAME:SpiReadReg
*FUNCTION     :CC1101 read data from register
*INPUT        :addr: register address
*OUTPUT       :register value
****************************************************************/
byte CC1101::SpiReadReg(byte addr) 
{
	byte temp, value;

    temp = addr|READ_SINGLE;

	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;

	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);

	SpiTransfer(temp);
	value=SpiTransfer(0);

	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
	return value;
}

/****************************************************************
*FUNCTION NAME:SpiReadBurstReg
*FUNCTION     :CC1101 read burst data from register
*INPUT        :addr: register address; buffer:array to store register value; num: number to read
*OUTPUT       :none
****************************************************************/
void CC1101::SpiReadBurstReg(byte addr, byte *buffer, byte num)
{
	byte i,temp;

	temp = addr | READ_BURST;
	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;

	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);

	SpiTransfer(temp);
	for(i=0;i<num;i++)
	{
		buffer[i]=SpiTransfer(0);
	}

	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;
}

/****************************************************************
*FUNCTION NAME:SpiReadStatus
*FUNCTION     :CC1101 read status register
*INPUT        :addr: register address
*OUTPUT       :status value
****************************************************************/
byte CC1101::SpiReadStatus(byte addr) 
{
	byte value,temp;

	temp = addr | READ_BURST;

	//digitalWrite(SS_PIN, LOW);
	GPIOB->BSRR|=0x1000;

	//while(digitalRead(MISO_PIN));
	while((GPIOB->IDR>>14)&0x0001);

	SpiTransfer(temp);
	value=SpiTransfer(0);
	
	//digitalWrite(SS_PIN, HIGH);
	GPIOB->BSSR|=0x1000;

	return value;
}

/****************************************************************
*FUNCTION NAME:RegConfigSettings
*FUNCTION     :CC1101 register config //details refer datasheet of CC1101/CC1100//
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::RegConfigSettings(byte f) 
{
    SpiWriteReg(CC1101_FSCTRL1,  0x08);
    SpiWriteReg(CC1101_FSCTRL0,  0x00);
	
    switch(f)
    {
      case F_868:
      	SpiWriteReg(CC1101_FREQ2,    F2_868);
      	SpiWriteReg(CC1101_FREQ1,    F1_868);
      	SpiWriteReg(CC1101_FREQ0,    F0_868);
        break;
      case F_915:
        SpiWriteReg(CC1101_FREQ2,    F2_915);
        SpiWriteReg(CC1101_FREQ1,    F1_915);
        SpiWriteReg(CC1101_FREQ0,    F0_915);
        break;
	  case F_433:
        SpiWriteReg(CC1101_FREQ2,    F2_433);
        SpiWriteReg(CC1101_FREQ1,    F1_433);
        SpiWriteReg(CC1101_FREQ0,    F0_433);
        break;
	  default: // F must be set
	  	break;
	}
	
    SpiWriteReg(CC1101_MDMCFG4,  0x5B);
    SpiWriteReg(CC1101_MDMCFG3,  0xF8);
    SpiWriteReg(CC1101_MDMCFG2,  0x03);
    SpiWriteReg(CC1101_MDMCFG1,  0x22);
    SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    SpiWriteReg(CC1101_CHANNR,   0x00);
    SpiWriteReg(CC1101_DEVIATN,  0x47);
    SpiWriteReg(CC1101_FREND1,   0xB6);
    SpiWriteReg(CC1101_FREND0,   0x10);
    SpiWriteReg(CC1101_MCSM0 ,   0x18);
    SpiWriteReg(CC1101_FOCCFG,   0x1D);
    SpiWriteReg(CC1101_BSCFG,    0x1C);
    SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
	SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    SpiWriteReg(CC1101_FSCAL3,   0xEA);
	SpiWriteReg(CC1101_FSCAL2,   0x2A);
	SpiWriteReg(CC1101_FSCAL1,   0x00);
    SpiWriteReg(CC1101_FSCAL0,   0x11);
    SpiWriteReg(CC1101_FSTEST,   0x59);
    SpiWriteReg(CC1101_TEST2,    0x81);
    SpiWriteReg(CC1101_TEST1,    0x35);
    SpiWriteReg(CC1101_TEST0,    0x09);
    SpiWriteReg(CC1101_IOCFG2,   0x0B); 	//serial clock.synchronous to the data in synchronous serial mode
    SpiWriteReg(CC1101_IOCFG0,   0x06);  	//asserts when sync word has been sent/received, and de-asserts at the end of the packet 
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);		//two status bytes will be appended to the payload of the packet,including RSSI LQI and CRC OK
											//No address check
    SpiWriteReg(CC1101_PKTCTRL0, 0x05);		//whitening off;CRC Enable��variable length packets, packet length configured by the first byte after sync word
    SpiWriteReg(CC1101_ADDR,     0x00);		//address used for packet filtration.
    SpiWriteReg(CC1101_PKTLEN,   0x3D); 	//61 bytes max length
}

/****************************************************************
*FUNCTION NAME:SendData
*FUNCTION     :use CC1101 send data
*INPUT        :txBuffer: data array to send; size: number of data to send, no more than 61
*OUTPUT       :none
****************************************************************/
void CC1101::SendData(byte *txBuffer,byte size)
{
	SpiWriteReg(CC1101_TXFIFO,size);
	SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);			//write data to send
	SpiStrobe(CC1101_STX);									//start send

	while(!(GPIOA->IDR>>8));
    //while (!digitalRead(GDO0));								// Wait for GDO0 to be set -> sync transmitted  

	while((GPIOA->IDR>>8));
    //while (digitalRead(GDO0));								// Wait for GDO0 to be cleared -> end of packet
	SpiStrobe(CC1101_SFTX);									//flush TXfifo
}

/****************************************************************
*FUNCTION NAME:SetReceive
*FUNCTION     :set CC1101 to receive state
*INPUT        :none
*OUTPUT       :none
****************************************************************/
void CC1101::SetReceive(void)
{
	SpiStrobe(CC1101_SRX);
}

/****************************************************************
*FUNCTION NAME:CheckReceiveFlag
*FUNCTION     :check receive data or not
*INPUT        :none
*OUTPUT       :flag: 0 no data; 1 receive data 
****************************************************************/
byte CC1101::CheckReceiveFlag(void)
{
	if((GPIOA->IDR>>8))			//receive data
	{
		while ((GPIOA->IDR>>8));
		return 1;
	}
	else							// no data
	{
		return 0;
	}
}


/****************************************************************
*FUNCTION NAME:ReceiveData
*FUNCTION     :read data received from RXfifo
*INPUT        :rxBuffer: buffer to store data
*OUTPUT       :size of data received
****************************************************************/
byte CC1101::ReceiveData(byte *rxBuffer)
{
	byte size;
	byte status[2];

	if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)
	{
		size=SpiReadReg(CC1101_RXFIFO);
		SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
		SpiReadBurstReg(CC1101_RXFIFO,status,2);
		SpiStrobe(CC1101_SFRX);
		return size;
	}
	else
	{
		SpiStrobe(CC1101_SFRX);
		return 0;
	}
	
}

CC1101 CC1101;




