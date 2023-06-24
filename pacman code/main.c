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
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"

uint8_t glyph[] = {0b00010000, 0b00100100, 0b11100000, 0b00100100, 0b00010000};
uint8_t gameMap[5][10] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
						  {1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
						  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
						  {1, 0, 2, 0, 0, 0, 0, 1, 0, 1},
						  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
int i = 0;
int j = 0;
uint8_t chao = 0;	  // espaço -> .
uint8_t parede = 1;	  // []
uint8_t fantasma = 2; // $
uint8_t pacman = 3;	  // @
uint8_t comida = 4;	  // 0
int posX = 2;
int posY = 0;
int contadorms = 0;
int temp = 0;

int botaoA()
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

int botaoW()
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

int botaoD()
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

void printaTela()
{
	nokia_lcd_clear();
	int colTela = 0;
	for (int x = 0; x < 5; x++)
	{
		colTela = x * 9;
		nokia_lcd_set_cursor(0, colTela);

		for (int y = 0; y < 10; y++)
		{
			if (gameMap[x][y] == 0)
			{
				nokia_lcd_write_string(" ", 1); // chao
			}
			if (gameMap[x][y] == 1)
			{
				nokia_lcd_write_string("/", 1); // parede
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

				nokia_lcd_write_string(",", 1); // comidinha
			}
		}
	}
	nokia_lcd_render();
}

void inserePacman()
{
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			if (gameMap[x][y] == 3)
			{
				gameMap[x][y] = 0;
			}
		}
	}
	gameMap[posX][posY] = pacman;
}

void telaInicio()
{
	nokia_lcd_clear();
	nokia_lcd_set_cursor(13, 15);
	nokia_lcd_write_string("Inicie o jogo Apertando 'W'",1);
	nokia_lcd_render();
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
	telaInicio();
	if(botaoW() == 1){
		nokia_lcd_clear();
		printaTela();	
	}
	//printaTela();
	inserePacman(posX, posY);
	printaTela();
	int morte = 0;

	while (1)
	{

		if (botaoW() == 1)
		{
			if (gameMap[posX][posY - 1] != 1)
			{
				posY--;
				inserePacman();
				printaTela();
			}
		}
		else if (botaoA() == 1)
		{
			if (gameMap[posX - 1][posY] != 1)
			{
				posX--;
				inserePacman();
				printaTela();
			}
		}
		else if (botaoS() == 1)
		{
			if (gameMap[posX][posY + 1] != 1)
			{
				posY++;
				inserePacman();
				printaTela();
			}
		}
		else if (botaoD() == 1)
		{
			if (gameMap[posX + 1][posY] != 1)
			{
				posX++;
				inserePacman();
				printaTela();
			}
			if (morte == 1)
			{
				_delay_ms(10000);
				printaTela();
				PORTD |= (1 << PD2);
				_delay_ms(5000);
				morte = 0;
			}
		}
	}
}

/*
Fazer:
	MovimentaFastasmas()
	Sistema de Morte
	SuperPoderes
	Contador de tempo e de pontos
	Tela Final
	Sistema de Leds  : Morte = Led vermelho acende por 5 s
					   Vitória = Led verde acende por 5 s
					   Amarelo/Azul = ?
					 
	Botão Start
	Arrumar tela de inicio
	Contabilizar tempo com timer   

Colocar:
	Timer
	Circulo e Retângulos na lib do nokia pro jogo ficar + bonito

*/