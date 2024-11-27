#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define poly16l 0xD008
// Example PRNG: PRNG 2 with m=64 and l=16

// random seed
static uint64_t x = 0x7F3B5E9D2A4C7FE;
static uint64_t y = 0x1C6D9A4F8B7E2D3A; 
static uint16_t seed2 = 0x3F4E;//0x4D5E6F;// Semente inicial
static int j=0;
static uint32_t z_sample[4];
uint64_t x_aux;
typedef struct{
uint64_t lambda0;
uint64_t lambda1;
}lambda;
typedef struct{
 uint64_t x;
 uint64_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint64_t x,uint64_t y){
coord_arnold coord; //assinatura para estrutura

    x_aux = x;
    coord.x = (2 * x + y); //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y); // % MOD;
    return coord;
 //retorna os valores com assinados
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
        lfsr->state ^= poly16l;

    return bit; // Devuelve el bit generado
}
lambda prng_m64_l16(int j) {
    uint64_t Bx[4], By[4],lambda0;
    uint32_t w[4], z[4];
    uint64_t beta[4];

struct LFSR lfsr2;

 
     //   printf("Valores iniciais: x = %08X, y = %08X\n", x, y);
      //  printf("\n");
        
coord_arnold coord;
coord=arnold((uint64_t)x,(uint64_t)y);

x=coord.x;
y=coord.y;

     //  printf("Valores finais: x = %08lX, y = %08lX\n", x, y);
      //  printf("\n");
    uint64_t x1 = 0;
    uint64_t y1 = 0;
    

    uint64_t sequence2 = 0; // Inicializar la secuencia como 0 antes de generar la nueva secuencia


    lfsr_init(&lfsr2, seed2);
  
        sequence2 <<= 1;
        sequence2 |= lfsr_next2(&lfsr2);
       

    //Actualizo sus estados
    seed2 = lfsr2.state;

  // printf("\n");
  // printf("Valores atuais: x = %08lX, y = %08lX\n", x, y);

    x1 = x;
    y1 = y ^ (sequence2);
    // Processa os bits em grupos de 4
    for (int i = 0; i < 4; ++i) {
        Bx[i] = (x1 >> (16 * i)) & 0xffff;
        By[i] = (y1 >> (16 * i)) & 0xffff;
       
        w[i] = By[i] | (Bx[i] << 16);
        z[i] = Bx[i] * By[i];
        if(j==1)
        z[i]==z_sample[i];
    }
    beta[3] = (z_sample[0] ^ w[3] ^ w[2]);
    beta[2] = (z_sample[1] ^ w[2] ^ w[1]);
    beta[1] = (z_sample[2] ^ w[1] ^ w[0]);
    beta[0] = (z_sample[3] ^ w[0] ^ w[3]);
    
    lambda l;
    l.lambda0 = ((beta[1] << 32) |(beta[0]));
    l.lambda1 = ((beta[3] << 32) |(beta[2]));
    return l;
 
    
}

int main()
{
	lambda sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m64_l16(j);
                j++;
  		printf("i: %d, z0: %lu, z1: %lu\n", i, sample.lambda0, sample.lambda1);
	}
 
    return 0;
}

