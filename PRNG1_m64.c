// Adapted from TestU01 manual

#include "math.h"
#include <stdio.h>
#include <stdint.h>
// PRNG 1 com m=64 e l=16

// Sementes aleatórias
static uint64_t x = 1142458949U;
static uint64_t y = 848535601U;
//uint64_t x,y;
static double nb = pow(2, 24);
static uint64_t seed1 = 0xBEEFCAFEu;// Sementeinicial
static uint64_t seed2 = 0x5A5A5A5Au;// Semente inicial
static int r=0;
static int s=32;

//Estrutura para atualizar os valores das coordenadas
typedef struct{
 uint64_t x;
 uint64_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint64_t x, uint64_t y){
coord_arnold coord; //assinatura para estrutura
   uint64_t x_aux;
    x_aux = x;
    coord.x = (2 * x + y); //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y); // % MOD;
    return coord; //retorna os valores com assinados
}
uint64_t prng_m64_l16(void)
{

    uint64_t Bx0, Bx1, Bx2, Bx3;
    uint64_t By0, By1, By2, By3;
    uint64_t alpha0, alpha1;

    coord_arnold coord;// Assinatura da estrutura
    coord=arnold((uint64_t)seed1,(uint64_t)seed2);//Entrada da semente no mapa de Arnold discreto
    //Atualizando os valores da estrutura
    seed1=coord.x;
    seed2=coord.y;
    //Atualizando as variáveis x e y para as operações
    x=seed1;
    y=seed2;
   //Divisão em m/l sub-blocos, 4 sub-blocos com comprimento de 16bits
   //Sub-blocos de maior índice possuem os bits mais significativos  
    // Get x[15:0], y[15:0]
    Bx0 = x & 0xffff;
    By0 = y & 0xffff;
    
    // Get x[31:15], y[31:15]
    Bx1 = (x & 0xffff0000) >> 16;
    By1 = (y & 0xffff0000) >> 16;
    
    // Get x[47:31], y[47:31]
    Bx2 = (x & 0xffff00000000) >> 32;
    By2 = (y & 0xffff00000000) >> 32;

    // Get x[63:47], y[63:47]
    Bx3 = (x & 0xffff000000000000) >> 48;
    By3 = (y & 0xffff000000000000) >> 48;
// { X[47:31] XOR y[31:15] +(OR) X[63:47] XOR Y[15:0]} +(OR) {X[15:0] XOR y[47:31] +(OR) X[31:15] XOR Y[63:47:0]}
    alpha0= ((Bx2 ^ By1)<<16 | (Bx3 ^ By0))<<32;//32bits mais significativo
    alpha1= ((Bx0 ^ By2)<<16 | (Bx1 ^ By3));//32bits menos significativos
    
    return alpha0|alpha1; //64bits r=m
       
}


int main()
{
	uint64_t sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m64_l16();
  		printf("i: %d, z: %lu\n", i, sample);
	}
 
    return 0;
}
