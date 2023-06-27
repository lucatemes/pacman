/* Nokia 5110 LCD AVR Library example
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 * Custom char code by Marcelo Cohen - 2021
 */

/*
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
*/
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "nokia5110.h"
#include <stdlib.h>
void telaVitoria();
void telaDerrota();

uint8_t glyph[] = {0b00010000, 0b00100100, 0b11100000, 0b00100100, 0b00010000};
uint8_t gameMap[5][14] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
						  {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
						  {1, 4, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
						  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
int i = 0;
int j = 0;
uint8_t comida = 0;	  // espaco -> .
uint8_t parede = 1;	  // /
uint8_t fantasma = 2; // $
uint8_t pacman = 3;	  // circulo
uint8_t nada = 4;	  // nadas
double tempo = 0.0;
int itemProx = 0;
int contadorms = 0;
int posX = 2;
int posY = 1;
int pontosM = 0;
int pontos = 0;
int vidas = 3;
int morte = 0;
int temp = 0;
char msg[30];
int timer = 0;
int winStreak = 0;
// BOTOES
int botaoA()
{
	if (PIND & (1 << PD6))
	{
		while (PIND & (1 << PD6))
		{
			_delay_ms(10);
		}
		return 1;
	}
	return 0;
}

int botaoW()
{
	if (PIND & (1 << PD5))
	{
		while (PIND & (1 << PD5))
		{
			_delay_ms(10);
		}
		return 1;
	}
	return 0;
}

int botaoS()
{
	if (PIND & (1 << PD7))
	{
		while (PIND & (1 << PD7))
		{
			_delay_ms(10);
		}
		return 1;
	}
	return 0;
}

int botaoD()//
{
	if (PINB & (1 << PB0))
	{
		while (PINB & (1 << PB0))
		{
			_delay_ms(10);
		}
		return 1;
	}
	return 0;
}
// METODOS
int pontosMax() //
{
	int pontosMax = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			if (gameMap[i][j] == 0)
			{
				pontosMax++;
			}
		}
	}
	return pontosMax * 10;
}

void inserePacman()
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 14; y++)
		{
			if (gameMap[x][y] == 3)
			{
				gameMap[x][y] = 4;
			}
		}
	}
	gameMap[posX][posY] = pacman;
}
void mortePacman()//
{
	nokia_lcd_clear();
	posX = 2;
	posY = 1;
	vidas--;
	inserePacman();
	PORTD |= (1<<PD2);
	_delay_ms(100);
	PORTD &= ~(1<<PD2);
	_delay_ms(100);
	PORTD |= (1<<PD2);
	_delay_ms(100);
	PORTD &= ~(1<<PD2);
	_delay_ms(100);
	PORTD |= (1<<PD2);
	_delay_ms(100);
	PORTD &= ~(1<<PD2);
	_delay_ms(100);
	PORTD |= (1<<PD2);
	_delay_ms(100);
	PORTD &= ~(1<<PD2);
	_delay_ms(100);
	PORTD |= (1<<PD2);
	_delay_ms(100);
	PORTD &= ~(1<<PD2);
	_delay_ms(100);
}

void AtualizaTela()//
{
	nokia_lcd_clear();
	int colTela = 0;
	for (int x = 0; x < 5; x++)
	{
		colTela = x * 9;
		nokia_lcd_set_cursor(0, colTela);
		nokia_lcd_drawrect(0, 0, 83, 43);
		for (int y = 0; y < 14; y++)
		{
			if (gameMap[x][y] == 0)
			{
				nokia_lcd_write_string(".", 1); // pontos
			}
			if (gameMap[x][y] == 1)
			{
				if ((x != 0) || (y != 0))
				{
					nokia_lcd_write_string("/", 1);
				}
			}
			if (gameMap[x][y] == 2)
			{
				nokia_lcd_write_string("$", 1); // fantasma
			}
			if (gameMap[x][y] == 3)
			{
				nokia_lcd_write_string("@", 1); // pacman
			}
			if (gameMap[x][y] == 4)
			{
				nokia_lcd_write_string(" ", 1); // espaço
			}
		}
	}
	nokia_lcd_render();
}

void telaInicio()
{
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0, 10);
	nokia_lcd_write_string("Inicie o jogo", 1);
	nokia_lcd_set_cursor(0, 20);
	nokia_lcd_write_string("Apertando 'W'", 1);
	nokia_lcd_render();
	posX = 2;
	posY = 1;
	inserePacman();
}

int verificaRadarPac()
{
	if (gameMap[posX][posY - 1] == 2)
	{
		return 1;
	}
	if (gameMap[posX][posY + 1] == 2)
	{
		return 1;
	}
	if (gameMap[posX - 1][posY] == 2)
	{
		return 1;
	}
	if (gameMap[posX + 1][posY] == 2)
	{
		return 1;
	}
	return 0;
}

void moveFantasma()
{
	int mobProxX = 0;
	int mobProxY = 0;
	int itemProx = 4;
	int temp= 0;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 14; y++)
		{
			if (gameMap[x][y] == 2)
			{

				if (verificaRadarPac() == 1)
				{
					itemProx = gameMap[mobProxX][mobProxY];
					if (gameMap[mobProxX][mobProxY] != 1)
					{
						gameMap[mobProxX][mobProxY] = gameMap[x][y];
						gameMap[x][y] = itemProx;
					}
					mortePacman();
					AtualizaTela();
					if (vidas == 0)
					{
						telaDerrota();
					}
				}
				if (posY < 7)
				{
					mobProxX = x;
					mobProxY = y - 1;
					temp=-1;
				}
				else if (posY > 7)
				{
					mobProxX = x;
					mobProxY = y + 1;
					temp= 1;
				}
				else
				{
					mobProxX = x;
					mobProxY = y;
					temp = 0;
				}

				itemProx = gameMap[mobProxX][mobProxY];
				if (gameMap[mobProxX][mobProxY] != 1)
				{
					gameMap[mobProxX][mobProxY] = gameMap[x][y];
					gameMap[x][y] = itemProx;
				}
				
			}
		}
		AtualizaTela();
	}
}

void restart()
{
	uint16_t gameAtualizaTelaMap[5][14] =  {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
						  					{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
						  					{1, 4, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
						  					{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						  					{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			gameMap[i][j] = gameAtualizaTelaMap[i][j];
		}
	}

	vidas = 3;
	pontos = 0;
	pontosM = pontos + pontosMax();
	posX = 2;
	posY = 1;
	inserePacman();
	AtualizaTela();
}

void telaVitoria()
{
	cli();
	winStreak++;
	PORTD |= (1 << PD4);
	nokia_lcd_clear();
	nokia_lcd_set_cursor(10, 10);
	nokia_lcd_write_string("Voce Ganhou", 1);
	nokia_lcd_set_cursor(10, 20);
	sprintf(msg, "Score %d", pontos);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_set_cursor(10, 30);
	sprintf(msg, "Time %d", timer);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_set_cursor(10, 40);
	sprintf(msg, "WS %d", winStreak);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_render();
	_delay_ms(4000);
	PORTD &= ~(1 << PD2);
	pontos = pontos + pontosM;
	nokia_lcd_clear();
	restart();
	posX = 2;
	posY = 1;
	inserePacman();
	AtualizaTela();
}

void telaDerrota()
{
	cli();
	PORTD |= (1 << PD2);
	nokia_lcd_clear();
	nokia_lcd_set_cursor(10, 10);
	nokia_lcd_write_string("Voce Perdeu!", 1);
	nokia_lcd_set_cursor(10, 20);
	sprintf(msg, "Score %d", pontos);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_set_cursor(10, 30);
	sprintf(msg, "Time %d", timer);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_set_cursor(10, 40);
	sprintf(msg, "WS %d", winStreak);
	winStreak = 0;
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_render();
	_delay_ms(4000);
	PORTD &= ~(1 << PD2);
	pontos = 0;
	nokia_lcd_clear();
	restart();
	posX = 2;
	posY = 1;
	inserePacman();
	AtualizaTela();
}


int main(void)
{
	DDRD |= (1 << PD2);
	DDRD |= (1 << PD3);
	DDRD |= (1 << PD4); // setam as portas leds como saída

	DDRB &= ~(1 << PB0); // seta pb0 como entrada
	// PORTB &= ~(1 << PB0); //desabilita pull up

	DDRD &= ~(1 << PD7); // seta pd7 como entrada
	// PORTD &= ~(1 << PD7); //desabilita pull up

	DDRD &= ~(1 << PD6); // seta pd6 como entrada
	// PORTD &= ~(1 << PD6); //desabilita pull up

	DDRD &= ~(1 << PD5); // seta pd5 como entrada
	// PORTD &= ~(1 << PD5); //desabilita pull up
	nokia_lcd_init();  // inicia painel
	nokia_lcd_clear(); // clear no painel
	nokia_lcd_custom(1, glyph);

	TCCR1B |= (1 << WGM12);

	TCCR1B |= (1 << CS12);
	OCR1A = 62500; // PRESCALE DE 256

	TIMSK1 |= (1 << OCIE1A);

	contadorms = 0;
	pontosM = pontosMax();
	telaInicio();
	int contItera = 0;
	while (1)
	{
		if (botaoW() && contItera == 0)
		{
			inserePacman();
			AtualizaTela();
			contItera++;
		}
		sei();
		_delay_ms(1);
		contadorms++;
		if (contadorms == 1000)
		{
			moveFantasma();
			contadorms = 0;
		}
		if (botaoA() == 1)
		{
			if (gameMap[posX][posY - 1] != 1 && gameMap[posX][posY - 1] != 2)
			{
				if (gameMap[posX][posY - 1] == 0)
				{
					pontos += 10;
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
					}
				}
				posY--;
				inserePacman();
				AtualizaTela();
			}
			else if (gameMap[posX][posY - 1] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
					timer = 0;
					pontos = 0;
					timer = 0;
				}
			}
		}
		else if (botaoW() == 1)
		{
			if (gameMap[posX - 1][posY] != 1 && gameMap[posX - 1][posY] != 2)
			{
				if (gameMap[posX - 1][posY] == 0)
				{
					pontos += 10;
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
					}
				}
				posX--;
				inserePacman();
				AtualizaTela();
			}
			else if (gameMap[posX - 1][posY] == 2)
			{
				inserePacman();
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
					pontos = 0;
					timer = 0;
				}
			}
		}
		else if (botaoS() == 1)
		{
			if (gameMap[posX][posY + 1] != 1 && gameMap[posX][posY + 1] != 2)
			{
				if (gameMap[posX][posY + 1] == 0)
				{
					pontos += 10;
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
						pontos = 0;
					}
				}
				posY++;
				inserePacman();
				AtualizaTela();
			}
			else if (gameMap[posX][posY + 1] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
					timer = 0;
					pontos = 0;
				}
			}
		}
		else if (botaoD() == 1)
		{
			if (gameMap[posX + 1][posY] != 1 && gameMap[posX + 1][posY] != 2)
			{
				if (gameMap[posX + 1][posY] == 0)
				{
					pontos += 10;
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
						pontos = 0;
					}
				}
				posX++;
				inserePacman();
				AtualizaTela();
			}
			else if (gameMap[posX + 1][posY] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
					timer = 0;
					pontos = 0;
				}
			}
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;
	timer++;
}

/*
Fazer:
	MovimentaFastasmas()
	Sistema de Morte
	Contador de tempo e de pontos
	Tela Final
					   
Colocar:

	X Circulo e Retângulos na lib do nokia pro jogo ficar + bonito

*/