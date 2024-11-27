#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define poly8l 0xB8
// Exemplo PRNG: PRNG 2 com m=32 e l=8

// semente aleatória
static unsigned int x = 0x877;//0x1A3FB; --Best
static unsigned int y = 0xd52585;//0xB7C1E2
static double nb = pow(2, 24);
static uint8_t seed2= 0xFF; //seed LFSR 8 bits
static int j=0;
static uint16_t sample[4];
typedef struct{
 uint32_t x;
 uint32_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint32_t x,uint32_t y){
unsigned int x_aux;
coord_arnold coord; //assinatura para estrutura
    x_aux = x;
    coord.x = (2 * x + y); //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y); // % MOD;
    return coord; //retorna os valores com assinados
}
// Estrutura do LFSR
struct LFSR {
    uint8_t state; // Estado atual do LFSR
};


// Função para iniciar o LFSR a partir da semente dada
void lfsr_init(struct LFSR* lfsr, uint8_t seed) {
    lfsr->state = seed;
}

// Función para generar el siguiente valor del segundo LFSR y actualizar el estado
uint8_t lfsr_next2(struct LFSR* lfsr) {
    uint8_t bit = lfsr->state & 1; // Extrae el bit menos significativo
    lfsr->state >>= 1; // Desplaza un bit a la derecha

    if (bit) // Si el bit extraído es 1, aplica retroalimentación
        lfsr->state ^= poly8l;

    return bit; // Devuelve el bit generado
}
uint64_t prng_m32_l8(int j) {
    uint8_t Bx[8], By[8];
    uint16_t w[8], z[8];
    uint64_t beta[4], lambda0;
struct LFSR lfsr2;
 
      //  printf("Valores iniciais: x = %08X, y = %08X\n", x, y);
     //   printf("\n");
        
coord_arnold coord;
coord=arnold((uint32_t)x,(uint32_t)y);

x=coord.x;
y=coord.y;

    //  printf("Valores finais: x = %08X, y = %08X\n", x, y);
      //  printf("\n");
    uint32_t x1 = 0;
    uint32_t y1 = 0;
   
    lfsr_init(&lfsr2, seed2);
    
    uint8_t sequence2 = 0; // Inicializar la secuencia como 0 antes de generar la nueva secuencia
      
        sequence2 <<= 1;
        sequence2 |= lfsr_next2(&lfsr2);
       

    //Actualizo sus estados
    seed2 = lfsr2.state;

   // printf("\n");
  //  printf("Depois: S1 = %X, S2 = %X\n", sequence1, sequence2);

    x1 = x; 
    y1 = y ^ (sequence2);
    // Processa os bits em grupos de 4
    for (int i = 0; i < 4; ++i) {
        Bx[i] = (x1 >> (8 * i)) & 0xff;
        By[i] = (y1 >> (8 * i)) & 0xff;
        
        w[i] = By[i] | (Bx[i] << 8);
        z[i] = Bx[i] * By[i];
        if(j==1)
        z_sample[i]==z[i];
    }

    beta[3] = (z_sample[0] ^ w[3] ^ w[2]);
    beta[2] = (z_sample[1] ^ w[2] ^ w[1]);
    beta[1] = (z_sample[2] ^ w[1] ^ w[0]);
    beta[0] = (z_sample[3] ^ w[0] ^ w[3]);
    
    // Construa a palavra lambda0
  lambda0 = ((beta[3] << 48) | (beta[2] << 32) | (beta[1] << 16) |(beta[0]));
 // printf("%ld bits\n",sizeof((w[3] ^ z[0]) ^ (w[1] ^ z[2]))*8);
          return lambda0;    
}

int main()
{
	uint64_t sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m32_l8(j);
                j++;
  		printf("i: %d, z: %lu\n", i, sample);
	}
    return 0;
}

