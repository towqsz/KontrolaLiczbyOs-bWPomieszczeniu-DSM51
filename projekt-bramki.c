#include <at89x51.h>
#include <absacc.h>

#define tab_size 2
#define l_warn upper_limit-5;

int upper_limit=20;

#define LCDWC XBYTE[0xff80] //LCD write control
#define LCDWD XBYTE[0xff81] //LCD write data
#define LCDRC XBYTE[0xff82] //LCD read control (status)
#define LCDRD XBYTE[0xff83] //LCD read data

#define CSKB0 XBYTE[0xff21] //stan kb1 (0-7)
#define CSKB1 XBYTE[0xff22] //stan kb2 (pozostale)

void wait(unsigned int d) {
	unsigned int i,j;
	for (j=0; j<d; j++)
    	for (i=0; i<144; i++);
}


void write_control(unsigned char a){	//zapis rozakazu, zapis danej
	while ( XBYTE[0xff82] & 0x80 ) wait(1);
	XBYTE[0xff80]=a;	
}


void write_data(unsigned char a){
	while ( XBYTE[0xff82] & 0x80 ) wait(1);
	XBYTE[0xff81]=a;	
}


void lcd_clr(void){
	write_control(0x01);
	wait(10);
}

//inicjalizacja wyswietlacza
void lcd_init (void) {
unsigned char i;
	static const unsigned char code init_tab[] ={0x33, 0x32, 0x38, 0x08, 0x0f, 0x06};
	for (i = 0; i < sizeof init_tab; ++i) {
        write_control(init_tab[i]); 
        wait(1);
	}
	lcd_clr();
	write_control(0x80);            // wybierz adres DD RAM
	wait(1);
}

//program obslugi wswietlacza
void lcd(unsigned char c) {
    unsigned char x;
        switch(c){				//znakow
case'\n':						//od nowego wiersza
		x=XBYTE[0xFF82];
		if (x<=0x0f) write_control(0xc0)	;
		if((x>=0x41)&&(x<=0x50)){
		write_control(0x80);
		}
		break;
case'\r':
		XBYTE[0xFF82];
		if((x>=0x00)&(x<=0x0f))write_control(0x80);	//rozkaz dana
		if((x>=0x41)&(x<=0x50))write_control(0xc0);	//
		break;
default:
        if(c>0x1f && c,0x80) write_data(c);
    	wait(1);
        break;
	}
wait(1);
XBYTE[0xFF82];
if(x==0x10)write_control(0xc0);			//rozkaz dana
if(x==0x50)write_control(0x80);			//
}


 void set_buzzer(int time, int *settime)
{
	P1_5=0;
	(*settime)=time;
}

void check_buzzer(int time, int *settime)
{
	if (time > (*settime))
	{
		P1_5=1;
	}
}

void set_light(int l)
{
	if(l==1)
	{
		P1_6 = 1;
		XBYTE[0xF030] = 0x40;
		XBYTE[0xF038] = 0x10;
		P1_6 = 0;
	}
	else if (l==0)
	{
		P1_6 = 1;
		XBYTE[0xF030] = 0;
		XBYTE[0xF038] = 0;
		P1_6 = 0;
	}

	else
	{
	 	P1_6 = 1;
		XBYTE[0xF030] = 0x40;
		XBYTE[0xF038] = 0x20;
		P1_6 = 0;
	}
}

void int2string(unsigned char tab[], int number)
{
	int i,n;
	n=0;
	for (i = 10 ; i > 0 ; i=i/10)
		{
			tab[n]=(number/i)%10;
			n++;
		}

	for (i=0 ; i<tab_size ; i++)
	{
		
		if(tab[i]==0)
			tab[i]='0';
		else if(tab[i]==1)
			tab[i]='1';
		else if(tab[i]==2)
			tab[i]='2';
		else if(tab[i]==3)
			tab[i]='3';
		else if(tab[i]==4)
			tab[i]='4';
		else if(tab[i]==5)
			tab[i]='5';
		else if(tab[i]==6)
			tab[i]='6';
		else if(tab[i]==7)
			tab[i]='7';
		else if(tab[i]==8)
			tab[i]='8';
		else if(tab[i]==9)
			tab[i]='9';
	}
}

int check_inout(int *liczba_os)
{
	static int flaga[4];
				  
	//sprawdzenie bramek wejsciowych
	if((*liczba_os)<20 && (~XBYTE[0xF021]))
	{
		if ((~XBYTE[0xF021]&0x02) && flaga[0]==1)
			{
				(*liczba_os)++;
				flaga[0]=0;
			}
		else if(!(~XBYTE[0xF021]&0x02))
			flaga[0]=1;
		
	
		if ((~XBYTE[0xF021]&0x04) && flaga[1]==1)
			{
				(*liczba_os)++;
				flaga[1]=0;
			}
		else if(!(~XBYTE[0xF021]&0x04))
			flaga[1]=1;	
	
		
	}

	if((*liczba_os)>0)
	{
		//sprawdzenie bramek wyjsciowych
		if ((~XBYTE[0xF021]&0x10) && flaga[2]==1)
			{
				(*liczba_os)--;
				flaga[2]=0;
			}
		else if(!(~XBYTE[0xF021]&0x10))
			flaga[2]=1;
	
		if ((~XBYTE[0xF021]&0x20) && flaga[3]==1)
			{
				(*liczba_os)--;
				flaga[3]=0;
			}
		else if(!(~XBYTE[0xF021]&0x20))
			flaga[3]=1;
	
	
			
	}
}

void check_string(unsigned char tab[][15], int status)
{
	static int prev_status;
	int i;

	if (prev_status!=status)
	{
	 	prev_status=status;
		lcd_clr();
		for(i=0 ; tab[status][i] ; i++)
			lcd(tab[status][i]);
	
	}
}

void check_status (int *liczba_os, int *status)
{
	if ((*liczba_os) <=14)
		(*status)=0;
	else if	((*liczba_os) > 14 && (*liczba_os)<=19)
	{
		(*status)=1;
		
	}
	else if ((*liczba_os) > 19)
		{
		(*status)=2;
		}
}

void check_warning(int status, int prev_liczba_os, int liczba_os, int *settime)
{
			if(status==0)  
			{
				set_light(1);
			
			}
			if( status==1)
			{
				set_light(0);
			 	
				if(prev_liczba_os<liczba_os)
					set_buzzer(100, settime);
			}
			if(prev_liczba_os<liczba_os && status==2)
			{
				set_light(3);
				set_buzzer(1000, settime);
			}
}

void check_change (unsigned char tab[], unsigned char txt_gora[][15], int *liczba_os)
{
	static int prev_liczba_os,status, time, settime;
	int i;


	if (prev_liczba_os!=*liczba_os)	   					//czy liczba osob sie zmienila?
		{
			time=0;
											  	
			int2string(tab, *liczba_os);			   	//zamien liczbe na string, zeby wyswietlic liczbe osob
			check_status(liczba_os, &status);	//czy status sie zmienil? Zaaktualizuj go
			check_string(txt_gora, status);				//czy trzeba zmienic napis?

			lcd('\n');									//wroc na poczatek wyswietlacza
			lcd('\n');									//przejdz do linii 2

			for(i=0; txt_gora[3][i] ; i++)
			{	
	   			lcd(txt_gora[3][i]);							 //wypisz aktualna liczbe osob
			}

			for(i=0; i<tab_size ; i++)
			{	
	   			lcd(tab[i]);							 //wypisz aktualna liczbe osob
			}

			check_warning(status, prev_liczba_os, (*liczba_os), &settime);
			prev_liczba_os=*liczba_os;			  //zaktualizuj liczbe osob
			
		}
	   	
		check_buzzer(time, &settime);
	  	if(time < 1000000000);
			time++;
}



int main(){

		int i,n, liczba_os=0;
		unsigned char tab[2];
		unsigned char txt_gora[4][15]={"Zapraszamy", "-------", "Czekaj!", "L. osob: "};
		
		set_light(1);
		lcd_init();
		for(i=0 ; txt_gora[0][i] ; i++)
			lcd(txt_gora[0][i]);
			
		lcd('\n');	 
		for(i=0; txt_gora[3][i] ; i++)
			{	
	   			lcd(txt_gora[3][i]);							 
			}   
		int2string(tab, liczba_os);
		for(n=0; n<tab_size ; n++)
	 		lcd(tab[n]);
		
		
	
		while (1)  {
				
			//sprawdzenie klawiszy
			check_inout(&liczba_os);	
			
			//sprawdzenie, czy zaszla zmiana w liczbie osob
			check_change(tab, txt_gora, &liczba_os);
					
	   	
			
		}	 
	
}