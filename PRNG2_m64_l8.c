// Adapted from TestU01 manual
/*
Semente 1: 0x4F6A1C9B
Semente 2: 0x7E3D5F2A
*/
#include <stdio.h>
#include<math.h>
#include<stdint.h>

#define poly16l 0xD008
// Example PRNG: PRNG 2 with m=64 and l=8

// random seed
static uint64_t x = 0x7F3B5EA4C7FE;
static uint64_t y = 0x1C6D9A4F8B7E2D3A; 
static double nb = pow(2, 24);
static uint16_t seed2 = 0x3F4E;//0x4D5E6F;// Semente inicial
static int j=0;
static uint16_t z_sample0,z_sample1,z_sample2,z_sample3,z_sample4,z_sample5,z_sample6,z_sample7;
uint64_t x_aux;
//Estrutura para 128bits
typedef struct{
uint64_t lambda0;
uint64_t lambda1;
}lambda;
//Estrutura para atualizar os valores das coordenadas
typedef struct{
 uint64_t x;
 uint64_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint64_t x, uint64_t y){
coord_arnold coord; //assinatura para estrutura
    x_aux = x;
    coord.x = (2 * x + y); //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y); // % MOD;
    return coord; //retorna os valores com assinados
}
// Estrutura do LFSR
struct LFSR {
    uint16_t state; // Estado atual do LFSR
};

// Função para iniciar o LFSR a partir da semente dada
void lfsr_init(struct LFSR* lfsr, uint16_t seed) {
    lfsr->state = seed;
}

// Función para generar el siguiente valor del segundo LFSR y actualizar el estado
uint16_t lfsr_next2(struct LFSR* lfsr) {
    uint16_t bit = lfsr->state & 1; // Extrae el bit menos significativo
    lfsr->state >>= 1; // Desplaza un bit a la derecha

    if (bit) // Si el bit extraído es 1, aplica retroalimentación
        lfsr->state ^= poly16l;//POLYNOMIAL2_HIGH << 48 | POLYNOMIAL2_LOW;

    return bit; // Devuelve el bit generado
}


lambda prng_m64_l8 (int j)
{
    uint8_t Bx0, Bx1, Bx2, Bx3,Bx4,Bx5,Bx6,Bx7;
    uint8_t By0, By1, By2, By3,By4,By5,By6,By7;
    uint16_t w0, w1, w2, w3,w4,w5,w6,w7;
    uint16_t z0, z1, z2, z3,z4,z5,z6,z7;
    uint64_t beta0, beta1, beta2, beta3,beta4,beta5,beta6,beta7;
    uint64_t lambda0,lambda1;
     struct LFSR lfsr2;
     lambda l;
coord_arnold coord;
coord=arnold((uint64_t)x,(uint64_t)y);
x=coord.x;
y=coord.y;

    uint64_t x1 = 0;
    uint64_t y1 = 0;


    lfsr_init(&lfsr2, seed2);


    uint32_t sequence2 = 0; // Inicializar la secuencia como 0 antes de generar la nueva secuencia



        sequence2 <<= 1;
        sequence2 |= lfsr_next2(&lfsr2);

    //Actualizo sus estados
    seed2 = lfsr2.state;

    x1 =x;//^sequence1;
    y1 =y^sequence2;
    // Get x[7:0} , y[7:0]
    Bx0 = x1 & 0xff;
    By0 = y1 & 0xff;
    
    w0 = By0 | (Bx0 << 8);
    z0 = Bx0 * By0;
    if(j==1)
    z_sample0==z0;
    // Get x[15:7], y[15:7]
    Bx1 = (x1 & 0xff00) >> 8;
    By1 = (y1 & 0xff00) >> 8;
    
    w1 = By1 | (Bx1 << 8);
    z1 = Bx1 * By1;
    if(j==1)
    z_sample1==z1;
    // Get x[23:15], y[23:15]
    Bx2 = (x1 & 0xff0000) >> 16;
    By2 = (y1 & 0xff0000) >> 16;
    
    w2 = By2 | (Bx2 << 8);
    z2 = Bx2 * By2; 

    if(j==1)
    z_sample2==z2;

    // Get x[31:23], y[31:23]
    Bx3 = (x1 & 0xff000000) >> 24;
    By3 = (y1 & 0xff000000) >> 24;

    w3 = By3 | (Bx3 << 8);
    z3 = Bx3 * By3;

    if(j==1)
    z_sample3==z3;

    // Get x[39:31], y[39:31]
    Bx4 = (x1 & 0xff00000000) >> 32;
    By4 = (y1 & 0xff00000000) >> 32;
    
    w4 = By4 | (Bx4 << 8);
    z4 = Bx4 * By4;

    if(j==1)
    z_sample4==z4;

    
    // Get x[47:39], y[47:39]
    Bx5 = (x1 & 0xff0000000000) >> 40;
    By5 = (y1 & 0xff0000000000) >> 40;
    
    w5 = By5 | (Bx5 << 8);
    z5 = Bx5 * By5;
    if(j==1)
    z_sample5==z5;

     // Get x[54:47], y[55:47]
    Bx6 = (x1 & 0xff000000000000) >> 48;
    By6 = (y1 & 0xff000000000000) >> 48;
    
    w6 = By6 | (Bx6 << 8);
    z6 = Bx6 * By6;

    if(j==1)
    z_sample6==z6;

     // Get x[63:55], y[63:55]
    Bx7 = (x1 & 0xff00000000000000) >> 56;
    By7 = (y1 & 0xff00000000000000) >> 56;
    
    w7 = By7 | (Bx7 << 8);
    z7 = Bx7 * By7;
    
    if(j==1)
    z_sample7==z7;

    
    // Get beta values
beta7 = (z_sample0 ^ w7 ^ w4);
beta6 = (z_sample1 ^ w6 ^ w3);
beta5 = (z_sample2 ^ w5 ^ w2);
beta4 = (z_sample3 ^ w4 ^ w1);
beta3 = (z_sample4 ^ w3 ^ w0);
beta2 = (z_sample5 ^ w2 ^ w7);
beta1 = (z_sample6 ^ w1 ^ w6);
beta0 = (z_sample7 ^ w0 ^ w5);

    
   l.lambda0=(beta7<<48|beta6<<32|beta5<<16|beta4);
    l.lambda1=(beta3<<48|beta2<<32|beta1<<16|beta0);
    return l;
    
}

int main()
{
	lambda sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m64_l8(j);
                j++;
  		printf("i: %d, z0: %lu, z1: %lu\n", i, sample.lambda0, sample.lambda1);
	}
 
    return 0;
}

