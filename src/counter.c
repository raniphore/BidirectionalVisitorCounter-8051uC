#include<reg51.h>
//------------------Assigning control bits of LCD----------------//
sbit rs=P3^0;
sbit rw=P3^1;
sbit en=P3^2;


int x=0;   //----count-----//

unsigned char n[10]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};  //lookup table for decimal to hex 0x30 means 30 in hex that is 48 in decimal or 0 in ascii//
//-------------------------------------------------------------//
//--assigning up and down pin----//
sbit up=P1^0;
sbit down=P1^1;
//-------------------//
sbit led=P3^3;  //--LED for invalid exit---//
sbit delayled=P3^4;  //---LED indicating delay between two succesive sensing--//
sbit buzz=P1^2;  //--For the buzzer--//

//--------------Prototype declaration--------------------------//
void init_lcd();                           //-----Function to initialize lcd----//
void lcdcmd(unsigned char value1);         //-----Function to send command------//
void lcd_data(unsigned char value2);       //-----Function used to send data-----//
void lcd_msg(unsigned char *ch);           //-----Sending a string by pointer method----//
void delay(unsigned int ms);               //-----Fucntion for 1ms delay------//
void lcd_int(int i);              		  //------Function to send integer as string----// 
	
//------------------------------------------------------------//
//-----------------Main program-------------------------------//

	void main()
	{
		P2=0x00; 		//----initializing port 2 as output port---//
		P1=0xff;
		P3=0x00;
		//P1^1=1;     //-------initializing p1.0 and p1.1 as input port-----//
		
			init_lcd();   //----initialize lcd----//
		
			lcdcmd(0x82);
			lcd_msg("BIDIRECTIONAL");
			delay(10);
			lcdcmd(0xc5);
			delay(10);
			lcd_msg("COUNTER");
			delay(250);
			lcdcmd(0x01);
			delay(10);
			lcdcmd(0x80);
			delay(10);
			lcd_msg("Count=");
			delay(10);
			lcdcmd(0x86);   //-----To set cusor at end of the text "count=" ---------//
			delay(1);     	//---Reducing delay after placing cursor at desired place--//
			lcd_data(n[x]);
			lcdcmd(0x87);   
			delay(1);    
			lcd_data(n[x]);
			lcdcmd(0x88);   
			delay(1);    
			lcd_data(n[x]);
			
			
			while(1) //make it stay forever here
			{
				
				
				if (up==0 && down==1)
				{
					x=x+1;
					lcd_int(x);
					delayled=1;
					delay(100);
					delayled=0;
					while(up==0)
					{
					}//To prevent count increase in case of continuos low signal
				}
				if (down==0 && up==1)
				{
					x=x-1;
					if(x<0)
					{
						x=0;   //----Reinitialize count to zero---//
						lcdcmd(0x01);
						delay(10);
						lcdcmd(0x80);
						delay(10);
						led=1;						//---Activate red led----//
						buzz=0;						//---Activate buzzer----//
						delay(10);
						lcd_msg("INVALID EXIT"); 						//Invalid exit condition
						led=0;
						buzz=1;           //---deactivate buzzer---//
						delay(100);
						lcdcmd(0x01);
						delay(20);
						lcdcmd(0x80);
						delay(20);
						lcd_msg("Count=");
						delay(10);
						lcdcmd(0x86);   //-----To set cusor at end of the text "count=" ---------//
						delay(1);
						lcd_data(n[x]);
						lcdcmd(0x87);   //-----To set cusor at end of the text "count=" ---------//
						delay(1);
						lcd_data(n[x]);
						lcdcmd(0x88);   //-----To set cusor at end of the text "count=" ---------//
						delay(1);
						lcd_data(n[x]);
						
						
					}
					lcd_int(x);
					delayled=1;
					delay(100);
					delayled=0;
					while(down==0)
					{
					}//To prevent count decrease in case of continuos low signal
				}
			}			
		
	}
//------End of main program------//

	void init_lcd()
	{
		lcdcmd(0x38);  //---Initializing 16x2 LCD display---//
		delay(10);
		lcdcmd(0x0E);  //----Setting display on and cursor as blinking--//
		delay(10);
		lcdcmd(0x01);  //--Clearing the lcd display--//
		delay(10);
		lcdcmd(0x06);    //---Auto incrementing cursor---//
		delay(10);
		lcdcmd(0x80);     //---Force cursor on 1st line of lcd----//
		delay(10);
		lcdcmd(0x0c);
		delay(10);
		return;
	}
	
	void lcdcmd(unsigned char value1)
	{
		P2=value1;
		rs=0;   //---RS=0 means command register---//		
		rw=0;   //---RW=0 means write function---//
		//---EN=Enable pin, it is used to latch data into lcd when it has high to low pulse at En pin----//
		en=1;
		delay(1);
		en=0;	
	}
	
	void lcd_data(unsigned char value2)
	{
		P2=value2;
		rs=1;   //---RS=1 means data register---//		
		rw=0;   //---RW=0 means write function---//
		//---EN=Enable pin, it is used to latch data into lcd when it has high to low pulse at En pin----//
		en=1;
		delay(5);  //--increase it to make text come slower
		en=0;	
	}
	
	void lcd_msg(unsigned char *ch)
	{
			while(*ch!=0)
			{
				lcd_data(*ch);
				ch++;
			}
	}
	
	void delay(unsigned int ms)
	{
		unsigned int i,j;
		for(i=0;i<ms;i++)
			for(j=0;j<1275;j++);
	}
		
	void lcd_int(int i)     //Function to send data on LCD as integer but atleast 2 digits
	{  
		unsigned int k=0;
		int count=i;
		
		unsigned int num[3];
		while(i!=0)
		{
			num[k]=i%10;
			i=i/10;
			k++;
		}
		k--;
		
		if(count<10)                //i is the count//
		{
			lcdcmd(0x86);
			delay(5);
			lcd_data(n[0]);
			delay(5);
			lcdcmd(0x87);
			delay(5);
			lcd_data(n[0]);
			delay(5);
			lcdcmd(0x88);
			delay(5);
			lcd_data(n[num[k]]);
		}
		
		
		else if(count<100)
		{
			lcdcmd(0x86);
			delay(5);
			lcd_data(n[0]);
			delay(5);
			lcdcmd(0x87);
			delay(5);
			lcd_data(n[num[k]]);
			delay(5);
			lcdcmd(0x88);
			delay(5);
			lcd_data(n[num[k-1]]);
		}
		else if(count<1000)
		{
			lcdcmd(0x86);
			delay(5);
			lcd_data(n[num[k]]);
			delay(5);
			lcdcmd(0x87);
			delay(5);
			lcd_data(n[num[k-1]]);
			delay(5);
			lcdcmd(0x88);
			delay(5);
			lcd_data(n[num[k-2]]);	
		
		}
		else
		{
			x=0;   //re-initialize count to 0
			lcdcmd(0x01);
			delay(10);
			lcdcmd(0x80);
			delay(10);
			lcd_msg("Count overflow");
		    delay(50);
			lcdcmd(0x01);
			lcdcmd(0x80);
			delay(10);
			lcd_msg("Count=");
			delay(10);
			lcdcmd(0x86);   
			delay(1);
			lcd_data(n[x]);
			lcdcmd(0x87);   
			delay(1);
			lcd_data(n[x]);
			lcdcmd(0x88);   
			delay(1);
			lcd_data(n[x]);
			
		}
		
	}		