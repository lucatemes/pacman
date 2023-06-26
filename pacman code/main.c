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

uint8_t glyph[] = {0b00010000, 0b00100100, 0b11100000, 0b00100100, 0b00010000};
uint8_t gameMap[6][14] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
						  {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
						  {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
						  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
						  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
int i = 0;
int j = 0;
uint8_t chao = 0;	  // espaço -> .
uint8_t parede = 1;	  // /
uint8_t fantasma = 2; // $
uint8_t pacman = 3;	  // circulo
uint8_t comida = 4;	  // 0
double tempo = 0.0;
int itemAtual = 0;
int contadorms2 = 0;
int posX = 2;
int posY = 0;
int contadorms = 0;
int pontos = 0;
int vidas = 3;
int morte = 0;
int temp = 0;
<<<<<<< HEAD
char msg[30];
int timer = 0;
=======
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387

void restart()
{
	// pontos = 0;
	// posX = 2;
	// posY = 0;
	// vidas = 3;
	// inserePacman();
<<<<<<< HEAD
	uint16_t gameStartMap[5][14] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
									{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1},
									{0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
									{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
									{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 14; j++)
		{
			gameMap[i][j] = gameStartMap[i][j];
		}
	}

	vidas = 3;
	pontos = 0;
	sei();
	// gameMap = gameStartMap;
=======
	// uint8_t gameMap[6][14] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, // Mapa do jogo
	// 						  {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	// 						  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	// 						  {1, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	// 						  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	// 						  {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};
	vidas = 3;
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
}

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

int pontosMax()
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
	return pontosMax;
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

void start()
{
	nokia_lcd_clear();
	int colTela = 0;
<<<<<<< HEAD
	for (int x = 0; x < 5; x++)
	{
		colTela = x * 9;
		nokia_lcd_set_cursor(0, colTela);
		nokia_lcd_drawrect(0, 0, 83, 43);
=======
	for (int x = 0; x < 6; x++)
	{
		colTela = x * 9;
		nokia_lcd_set_cursor(0, colTela);

>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
		for (int y = 0; y < 14; y++)
		{
			if (gameMap[x][y] == 0)
			{
				nokia_lcd_write_string(".", 1); // pontos
			}
			if (gameMap[x][y] == 1)
			{
<<<<<<< HEAD
				if ((x != 0) || (y != 0))
				{
					nokia_lcd_write_string("/", 1);
				}
=======
				nokia_lcd_write_string("/", 1);
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
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
<<<<<<< HEAD
=======
			if (gameMap[x][y] == 5)
			{
				if ((x == 5) && (y == 0))
				{
					// char score[8] = "score :";
					// strcat(score, pontos);
					nokia_lcd_write_string("score :", 1);
				}
			}
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
		}
	}
	nokia_lcd_render();
}

void telaInicio()
{
	nokia_lcd_set_cursor(13, 15);
	nokia_lcd_write_string("Inicie o jogo Apertando 'W'", 1);
	nokia_lcd_render();
}

void telaDerrota()
{
<<<<<<< HEAD
	cli();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(10, 20);
	nokia_lcd_write_string("Voce Perdeu!", 1);
	nokia_lcd_render();
	_delay_ms(4000);
	nokia_lcd_clear();
	nokia_lcd_set_cursor(20, 20);
	sprintf(msg, "Score %d", pontos);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_render();
	_delay_ms(3000);
	restart();
=======
	nokia_lcd_clear();
	nokia_lcd_write_string("Você Perdeu o Jogo!", 1);
	nokia_lcd_set_cursor(13, 13);
	nokia_lcd_write_string("Pontuação : ", 1);
	nokia_lcd_set_cursor(13, 24);
	nokia_lcd_write_string("Tempo de jogo : ", 1);
	nokia_lcd_render();
	_delay_ms(2000);
	restart();
	telaInicio();;
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
}

void telaVitoria()
{
<<<<<<< HEAD
	cli();
	nokia_lcd_clear();
	nokia_lcd_set_cursor(10, 20);
	nokia_lcd_write_string("Voce Ganhou", 1);
	nokia_lcd_set_cursor(10, 30);	
	sprintf(msg, "Score %d", pontos);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_set_cursor(10, 40);
	sprintf(msg, "Time %d", timer);
	nokia_lcd_write_string(msg, 1);
	nokia_lcd_render();
	_delay_ms(4000);
	nokia_lcd_clear();
	_delay_ms(4000);
	nokia_lcd_set_cursor(20, 10);
	nokia_lcd_render();
	restart();
}

void moveFantasma()
{
	int mobProxX = 0;
	int mobProxY = 0;
	int mobAtualX = 0;
	int mobAtualY = 0;
	int itemAtual = 0;
	
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 14; y++)
			{
				if (gameMap[x][y] == 2)
				{
					mobProxX = x;
					mobProxY = y;
					mobAtualX = x;
					mobAtualY = y;

					if (mobProxX < posX)
					{
						mobProxX++;
					}
					else
					{
						mobProxX--;
					}
					if (mobProxY < posY)
					{
						mobProxY++;
					}
					else
					{
						mobProxY--;
					}
					if (gameMap[mobProxX][mobProxY] != 1)
					{
						itemAtual = gameMap[mobProxX][mobProxY];
						if(itemAtual != 3){
						gameMap[mobAtualX][mobAtualY] = itemAtual;
						}else{
							gameMap[mobAtualX][mobAtualY] = 0;
						}

						gameMap[mobProxX][mobProxY] = 2;
					}
					if (gameMap[mobProxX][mobProxY] == 3)
					{
						vidas--;
						posX = 2;
						posY = 0;
					}
					start();
				}
			
		}
	}
	start();
=======
	nokia_lcd_clear();
	nokia_lcd_write_string("Você Ganhou o Jogo!", 1);
	nokia_lcd_set_cursor(13, 13);
	nokia_lcd_write_string("Pontuação : ", 1);
	nokia_lcd_set_cursor(13, 24);
	nokia_lcd_write_string("Tempo de jogo : ", 1);
	nokia_lcd_render();
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
}

void mortePacman()
{
	nokia_lcd_clear();
	posX = 2;
	posY = 0;
<<<<<<< HEAD
	vidas--;
	start();
}

int verificaRadarPac(){
	if(gameMap[posX][posY - 1] == 2){
		return 1;
	}
	if(gameMap[posX ][posY + 1] == 2){
		return 1;
	}
	if(gameMap[posX - 1][posY] == 2){
		return 1;
	}
	if(gameMap[posX + 1][posY] == 2){
		return 1;
	}
	return 0;
=======
	pontos = 0;
	vidas--;
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
}

int gameOver()
{
	if (vidas == 0)
	{
		// vidas =3;
		return 1;
	}
	return 0;
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
<<<<<<< HEAD

	TCCR1B |= (1 << WGM12);

	TCCR1B |= (1 << CS12);
	OCR1A = 62500; // PRESCALE DE 256

	TIMSK1 |= (1 << OCIE1A);

	sei();

	temp = 0;
	int pontosM;

	start();
	contadorms2 = 0;
	while (1)
	{
		pontosM = pontosMax();
		sei();
		_delay_ms(1);
		contadorms2++;
		if (contadorms2 == 1000)
		{
			moveFantasma();
			contadorms2 = 0;
		}

		if (botaoW() == 1)
		{
			if (gameMap[posX][posY - 1] != 1 && gameMap[posX][posY - 1] != 2)
			{
				if (gameMap[posX][posY - 1] == 0)
				{
					pontos++;
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
					}
				}
				if (contadorms2 == 1000)
				{
					moveFantasma();
					contadorms2 = 0;
=======
	/*telaInicio();
	while(botaoW() != 0){
		nokia_lcd_clear();
		start();
		inserePacman(posX, posY);
		start();
		temp =1;
	}
	*/
	temp = 0;
	start();
	while (1)
	{	
		if (botaoW() == 1)
		{
			if (gameMap[posX][posY - 1] != 1 && gameMap[posX][posY - 1] != 2)
			{ 
				if (gameMap[posX][posY - 1] == 0)
				{
					pontos++;
					if (pontos == pontosMax())
					{ // VITORIA
						telaVitoria();
					}
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
				posY--;
				inserePacman();
				start();
			}
			else if (gameMap[posX][posY - 1] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
<<<<<<< HEAD
					timer=0;
					pontos = 0;
					timer = 0;
=======
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
			}
		}
		else if (botaoA() == 1)
		{
			if (gameMap[posX - 1][posY] != 1 && gameMap[posX - 1][posY] != 2)
			{
				if (gameMap[posX - 1][posY] == 0)
				{
					pontos++;
<<<<<<< HEAD
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
					}
				}
				if (contadorms2 == 1000)
				{
					moveFantasma();
					contadorms2 = 0;
=======
					if (pontos == pontosMax())
					{ // VITORIA
						telaVitoria();
					}
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
				posX--;
				inserePacman();
				start();
			}
			else if (gameMap[posX - 1][posY] == 2)
			{
				inserePacman();
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
<<<<<<< HEAD
					pontos = 0;
					timer = 0;
=======
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
			}
		}
		else if (botaoS() == 1)
		{
			if (gameMap[posX][posY + 1] != 1 && gameMap[posX][posY + 1] != 2)
			{
				if (gameMap[posX][posY + 1] == 0)
				{
					pontos++;
<<<<<<< HEAD
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
						pontos = 0;
					}
				}
				if (contadorms2 == 1000)
				{
					moveFantasma();
					contadorms2 = 0;
=======
					if (pontos == pontosMax())
					{ // VITORIA
						telaVitoria();
					}
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
				posY++;
				inserePacman();
				start();
			}
			else if (gameMap[posX][posY + 1] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
					telaDerrota();
<<<<<<< HEAD
					timer = 0;
					pontos = 0;
=======
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
			}
		}
		else if (botaoD() == 1)
		{
			if (gameMap[posX + 1][posY] != 1 && gameMap[posX + 1][posY] != 2)
			{
				if (gameMap[posX + 1][posY] == 0)
				{
					pontos++;
<<<<<<< HEAD
					if (pontos == pontosM)
					{ // VITORIA
						telaVitoria();
						timer = 0;
						pontos = 0;
					}
				}
				if (contadorms2 == 1000)
				{
					moveFantasma();
					contadorms2 = 0;
=======
					if (pontos == pontosMax())
					{ // VITORIA
						telaVitoria();
					}
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
				}
				posX++;
				inserePacman();
				start();
			}
			else if (gameMap[posX + 1][posY] == 2)
			{
				mortePacman();
				if (vidas == 0)
				{
<<<<<<< HEAD
					telaDerrota();
					timer = 0;
					pontos = 0;
=======
					nokia_lcd_clear();
					telaDerrota();
>>>>>>> efb76dcdbb2fc142e6394b6dd9ae0a3ec5871387
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
	SuperPoderes
	Contador de tempo e de pontos
	Tela Final
	Sistema de Leds  : Morte = Led vermelho acende por 5 s
					   Vitória = Led verde acende por 5 s
					   Amarelo/Azul = ?

	X Botão Start
	Arrumar tela de inicio

Colocar:
	
	X Circulo e Retângulos na lib do nokia pro jogo ficar + bonito

*/