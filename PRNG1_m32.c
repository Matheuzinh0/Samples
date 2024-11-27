// Adapted from TestU01 manual

#include "math.h"
#include <stdio.h>
#include <stdint.h>

// PRNG 1 com m=32 e l=8 Sem o LFSR

// Sementes aleatórias
static uint32_t x = 1142458949U;
static uint32_t y = 848535601U;
static double nb = pow(2, 24);
static uint64_t seed1 = 0xBEEFCAFEu;// Sementeinicial
static uint64_t seed2 = 0x5A5A5A5Au;// Semente inicial
static int r=0;
static int s=32;

//Estrutura para atualizar os valores das coordenadas
typedef struct{
 uint32_t x;
 uint32_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint32_t x, uint32_t y){
coord_arnold coord; //assinatura para estrutura
unsigned int x_aux;
    x_aux = x;
    coord.x = (2 * x + y); //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y); // % MOD;
    return coord; //retorna os valores com assinados
}
uint32_t prng_m32_l8(void)
{

    uint32_t Bx0, Bx1, Bx2, Bx3;
    uint32_t By0, By1, By2, By3;
    unsigned int x_aux;
    unsigned int alpha0, alpha1; //unsigned int armanzena valores de até 32bits
        
    coord_arnold coord;// Assinatura da estrutura
    coord=arnold((uint32_t)seed1,(uint32_t)seed2);//Entrada da semente no mapa de Arnold discreto
    //Atualizando os valores da estrutura
    seed1=coord.x;
    seed2=coord.y;
    //Atualizando as variáveis x e y para as operações
    x=seed1;
    y=seed2;
    //Divisão em sub-blocos: l=8 então divide-se me l/2 sub-blocos, ou seja, em 4 sub-blocos de 8bits cada.
    // Get x[7:0], y[7:0]
    Bx0 = x & 0xff;
    By0 = y & 0xff;

    // Get x[15:8], y[15:8]
    Bx1 = (x & 0xff00) >> 8;
    By1 = (y & 0xff00) >> 8;

    // Get x[23:16], y[23:16]
    Bx2 = (x & 0xff0000) >> 16;
    By2 = (y & 0xff0000) >> 16;


    // Get x[31:24], y[31:24]
    Bx3 = (x & 0xff000000) >> 24;
    By3 = (y & 0xff000000) >> 24;
    //// { X[47:31] XOR y[31:15] +(OR) X[63:47] XOR Y[15:0]} +(OR) {X[15:0] XOR y[47:31] +(OR) X[31:15] XOR Y[63:47:0]}
    alpha0= ((Bx2 ^ By1)<<8 | (Bx3 ^ By0))<<16;
    alpha1= ((Bx0 ^ By2)<<8 | (Bx1 ^ By3));
   
   return alpha0 | alpha1;// r=m , 32bits
}

int main()
{
	int sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m32_l8();
  		printf("i: %d, z: %u\n", i, sample);
	}
 
    return 0;
}
