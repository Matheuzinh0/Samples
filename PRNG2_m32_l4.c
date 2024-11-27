#include <stdio.h>
#include <stdint.h>
#include <math.h>
#define poly8 0xB8

// Exemplo PRNG: PRNG 2 com m=32 e l=4

// semente aleatória
static unsigned int x = 208431566U;//1142458949U;
static unsigned int y = 1301589U;//4853560U;
static double nb = pow(2, 24);
static uint32_t seed1= 0xFF; //seed LFSR 8 bits 0xB8
static int j=0;
static uint16_t z_sample[8];
typedef struct{
 uint32_t x;
 uint32_t y;
}coord_arnold;//assinatura para estrutura
//Função do mapa de Arnold discreto
coord_arnold arnold(uint32_t x,uint32_t y){
unsigned int x_aux;
coord_arnold coord; //assinatura para estrutura
    x_aux = x;
    coord.x = (2 * x + y);//%mod; //% MOD; //coord.x e coord.y armazenam o valor na estrutura via passagem por referência
    coord.y = (x_aux + y);//%mod; // % MOD;
    return coord; //retorna os valores com assinados
}
struct LFSR {
    uint8_t state; // Estado atual do LFSR
};


// Função para iniciar o LFSR a partir da semente dada
void lfsr_init(struct LFSR* lfsr, uint8_t seed) {
    lfsr->state = seed;
}


// Función para generar el siguiente valor del segundo LFSR y actualizar el estado
uint8_t lfsr_next1(struct LFSR* lfsr) {
    uint8_t bit = lfsr->state & 1; // Extrae el bit menos significativo
    lfsr->state >>= 1; // Desplaza un bit a la derecha

    if (bit) // Si el bit extraído es 1, aplica retroalimentación
        lfsr->state ^= poly8;

    return bit; // Devuelve el bit generado
}
uint64_t prng_m32_l4(int j) {
    uint32_t Bx[8], By[8];
    uint32_t w[8], z[8];
    uint64_t beta[8], lambda0;
    int k;
struct LFSR lfsr1;

//k++;
//printf("Valores iniciais%d: x = %08X, y = %08X\n",k, x, y);
      //  printf("\n");
        
coord_arnold coord;
coord=arnold((uint32_t)x,(uint32_t)y);

x=coord.x;
y=coord.y;

    uint32_t x1 = 0;
    uint32_t y1 = 0;
    //printf("\n");
  //  printf("Valores atuais%d: x = %08X, y = %08X\n",k, x, y);
   lfsr_init(&lfsr1, seed1);
    uint8_t sequence1 = 0; // Inicializar la secuencia como 0 antes de generar la nueva secuencia

        sequence1 <<= 1;
        sequence1 |= lfsr_next1(&lfsr1);


    //Actualizo sus estados
    seed1 = lfsr1.state;

    //printf("depois : %lx\n",sequence1);
  //  printf("\n");
   // printf("Valores atuais: x = %08X, y = %08X\n", x, y);

    x1 = x;
    y1 = y ^ (sequence1);
    // Processa os bits em grupos de 4
    
    for (int i = 0; i < 8; ++i) {
   
        Bx[i] = ((x1 >> (4 * i)) & 0xf);
        By[i] = ((y1 >> (4 * i)) & 0xf);
        
        w[i] = By[i] | (Bx[i] << 4);
        z[i] = By[i] * Bx[i];
        if(j==1)
        z_sample[i]==z[i];
    }


    // Calcula os valores beta com novas combinações
  
beta[7] = (z_sample[0] ^ w[7]) ^ w[4];
beta[6] = (z_sample[1] ^ w[6]) ^ w[3];
beta[5] = (z_sample[2] ^ w[5]) ^ w[2];
beta[4] = (z_sample[3] ^ w[4]) ^ w[1];
beta[3] = (z_sample[4] ^ w[3]) ^ w[0];
beta[2] = (z_sample[5] ^ w[2]) ^ w[7];
beta[1] = (z_sample[6] ^ w[1]) ^ w[6];
beta[0] = (z_sample[7] ^ w[0]) ^ w[5];

    lambda0 = (beta[7] << 56) | (beta[6] << 48) | (beta[5] << 40) | (beta[4] << 32) |
              (beta[3] << 24) | (beta[2] << 16) | (beta[1] << 8) | beta[0];
        return lambda0;     
}

int main()
{
 
 	uint64_t sample;

	for (int i = 0; i < 100; i++)
	{
		sample = prng_m32_l4(j);
                j++;
  		printf("i: %d, z: %lu\n", i, sample);
//  		printf("x: %u, y: %u\n", x, y);
	}
 
    return 0;
}
