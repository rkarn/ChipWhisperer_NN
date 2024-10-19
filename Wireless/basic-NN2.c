/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2015 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define IDLE 0
#define KEY 1
#define PLAIN 2

//#define BUFLEN 64
#define BUFLEN 300
#define MAX_ITEMS 8

uint8_t memory[BUFLEN];
uint8_t tmp[BUFLEN];
char asciibuf[BUFLEN];
uint8_t pt[16];

static void delay_2_ms(void);


void my_puts(char *c)
{
  do {
    putch(*c);

  } while (*++c);
}

static void delay_2_ms()
{
  for (volatile unsigned int i=0; i < 0xfff; i++ ){
    ;
  }
}

void my_read(char *buf, int len)
{
  for(int i = 0; i < len; i++) {
    while (buf[i] = getch(), buf[i] == '\0');

    if (buf[i] == '\n') {
      buf[i] = '\0';
      return;
    }
  }
  buf[len - 1] = '\0';
}


#define INPUT_SIZE 8
#define OUTPUT_SIZE1 20
#define OUTPUT_SIZE2 4

float layer1_weights[OUTPUT_SIZE1][INPUT_SIZE] = {
    {-0.409644f, -0.710711f, -2.085330f, 0.218946f, 0.115179f, 0.599601f, -0.325085f, 0.302378f},
    {-0.022886f, -0.985281f, 0.518062f, 0.158357f, -0.358417f, -0.239548f, -0.147660f, 0.009089f},
    {-0.641172f, -0.120358f, -2.754416f, -0.090740f, -0.095734f, -0.289911f, -0.270638f, 0.176571f},
    {0.209053f, 0.229841f, 0.434392f, 0.543819f, 0.706258f, 1.136261f, 0.206609f, -0.137186f},
    {0.266923f, 0.729215f, 1.233839f, 0.150938f, 0.511337f, 0.246002f, 0.144538f, 0.144622f},
    {0.378910f, 0.147494f, 0.174381f, -0.408773f, -1.136339f, 0.655138f, 0.084789f, -0.066401f},
    {0.666381f, 1.117213f, -0.909594f, -0.476068f, 0.101761f, -0.863234f, 0.115786f, -0.521360f},
    {-0.042350f, -0.598295f, 0.497215f, 0.235099f, -2.180087f, 0.040306f, 0.212547f, -0.298637f},
    {-0.389208f, -0.337861f, -0.405585f, 0.398723f, 0.218717f, 0.909563f, 0.090572f, -0.392000f},
    {-0.032673f, 0.043109f, -0.462349f, -0.060018f, -2.265743f, 0.510537f, -0.008246f, -0.026670f},
    {0.615957f, 0.191575f, 0.228740f, -0.244856f, 0.047003f, -0.097458f, -0.257545f, -0.896734f},
    {0.061064f, -0.132935f, -3.790369f, 0.055466f, -0.285962f, -0.016571f, 0.071944f, -0.183826f},
    {0.576337f, 0.935193f, -0.166005f, -0.327523f, -0.239876f, -0.582660f, 0.084165f, -0.258565f},
    {-0.110991f, -0.145137f, 0.295585f, 0.453241f, -0.275983f, 0.224764f, 0.125485f, -0.035301f},
    {-0.066225f, 0.085836f, 0.315529f, 0.292540f, 2.655580f, 0.218280f, 0.056333f, -0.043594f},
    {0.188047f, 0.655666f, -0.620651f, -0.272982f, 0.681932f, 0.991872f, 0.079081f, -0.591655f},
    {0.221799f, 0.075928f, -0.183586f, -0.096027f, -0.467795f, 1.521871f, -0.053487f, 0.189771f},
    {-0.156017f, 0.918178f, -0.385207f, -0.911459f, -0.421702f, -0.291968f, 0.170244f, -0.170641f},
    {0.236344f, -0.049352f, 0.032033f, -0.238226f, 2.442845f, -0.929640f, 0.011197f, -0.089384f},
    {-0.596474f, -0.029301f, -1.150287f, -0.380027f, -0.273297f, 0.537311f, 0.172596f, 0.031391f},
};

float layer1_bias[OUTPUT_SIZE1] = {0.225189f, -0.243888f, 0.172829f, 0.567473f, 1.332988f, -0.195586f, 0.662596f, -0.362901f, -0.434863f, -0.509120f, 0.623812f, 0.655408f, -0.568027f, 0.657743f, 0.503402f, -0.057546f, -0.582498f, -0.157871f, -0.003850f, 0.861265f};



float layer2_weights[OUTPUT_SIZE2][OUTPUT_SIZE1] = {
    {-0.105637f, 0.059999f, -0.507052f, -1.223264f, -0.084382f, 1.133055f, -0.844729f, 0.101294f, -0.732485f, 1.335826f, 0.453923f, -1.468623f, -0.940916f, 0.454373f, 1.095753f, 0.503926f, -1.254940f, 0.991383f, -0.262263f, -0.766846f},
    {0.121398f, 0.144230f, -1.763132f, 1.350430f, 1.030765f, -0.737202f, 1.512898f, -0.894714f, 0.411187f, -4.482953f, 0.938865f, -2.877256f, 0.905744f, 0.321768f, -4.227506f, -1.870481f, -1.571317f, -1.757081f, -5.194714f, 0.998021f},
    {1.057062f, -1.254336f, 1.128546f, -0.596642f, -1.899236f, 0.276292f, 0.381745f, -0.472760f, -0.102404f, 0.546754f, -0.569513f, 1.564337f, 0.037747f, -0.799893f, 0.461157f, 0.332368f, 0.308779f, -0.811610f, 0.158046f, 0.699549f},
    {-1.369804f, 0.570615f, -2.598794f, 0.223637f, -0.889816f, -0.132606f, -0.568944f, 0.928508f, 0.788531f, -0.942358f, -0.177431f, 0.224084f, -1.589802f, -0.385355f, -0.169612f, -1.090144f, 1.572152f, -0.965869f, 0.558365f, -0.836404f},
};

float layer2_bias[OUTPUT_SIZE2] = {-0.108636f, 0.992707f, -0.177296f, -0.725652f};

float relu(float x) {
        return (0.0f > x)?0.0f:x;
}


void softmax(float* layer2_output, float* output) {
    float sum_exp = 0.0;
    int num_neurons = 5;
    // Compute the sum of exp(logits[i])
    for (int i = 0; i < num_neurons; i++) {
        sum_exp += exp(layer2_output[i]);
    }

    // Compute softmax for each neuron
    for (int i = 0; i < num_neurons; i++) {
        output[i] = exp(layer2_output[i]) / sum_exp;
    }
}

void forward(uint8_t* input, float* layer1_output, float* layer2_output, float* output) {
    for (int i = 0; i < 20; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < 8; ++j) {
            sum += (input[j] * layer1_weights[i][j]);
        }
        sum += layer1_bias[i];
        layer1_output[i] = relu(sum);
    }
    for (int i = 0; i < 4; ++i) {
        float sum = 0.0f;
        for (int j = 0; j < 20; ++j) {
            sum += layer1_output[j] * layer2_weights[i][j];
        }
        sum += layer2_bias[i];
        layer2_output[i] = sum;
    }

    softmax(layer2_output, output);
    // float output(5);
    // for (int i = 0; i < 3; ++i) {
    //     output[i] = softmax(layer2_output[i], layer2_output);
    //         }

    //return layer2_output;
}


void reverse(char str[], int length)
{
  int start = 0;
  int end = length - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
    start++;
  }
}

// Implementation of citoa()
char* citoa(uint16_t num, char* str, uint8_t base)
{
  int i = 0;
  bool isNegative = 0;

  /* Handle 0 explicitly, otherwise empty string is
  * printed for 0 */
  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  // In standard itoa(), negative numbers are handled
  // only with base 10. Otherwise numbers are
  // considered unsigned.
  if (num < 0 && base == 10) {
    isNegative = 1;
    num = -num;
  }

  // Process individual digits
  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // If number is negative, append '-'
  if (isNegative)
    str[i++] = '-';

  str[i] = '\0'; // Append string terminator

  // Reverse the string
  reverse(str, i);

  return str;
}


// void predict(uint8_t input[INPUT_SIZE], float output[OUTPUT_SIZE]) {
//     for (int i = 0; i < OUTPUT_SIZE; i++) {
//         output[i] = biases[i];
//         for (int j = 0; j < INPUT_SIZE; j++) {
//             output[i] += input[j] * weights[i][j]/100;
//         }
//     }
// }

void implement_softmax(float * output, char * pred){
  uint8_t i = 0;
  float max_prob = -100;
  uint8_t max_ind = -1;

  for(i=0;i<5;i++){
    if(output[i] > max_prob){
      max_prob = output[i];
      max_ind = i;
    }
  }

  pred[0] = 48+ max_ind;
  //return max_ind;
}


void binaryStringToArray(char* binaryString, uint8_t * binaryArray) {
    // Get the length of the input binary string
    size_t length = strlen(binaryString);

    // Allocate memory for the uint8_t array
    //uint8_t* binaryArray = (uint8_t*)malloc(length * sizeof(uint8_t));

    // Loop through the string and convert each character to uint8_t
    for (size_t i = 0; i < length; i++) {
        if (binaryString[i] == '1') {
            binaryArray[i] = 1;
        } else if (binaryString[i] == '0') {
            binaryArray[i] = 0;
        } 
    }

    //return binaryArray;
}

uint16_t floatStringToArray(char* floatString, uint8_t* floatArray) {
    // Get the length of the input binary string

    //size_t length = strlen(floatString);
        //float float_array[MAX_ITEMS];
    int count = 0;  // Number of parsed float values

    // Tokenize the input string using strtok()
    char* token = strtok(floatString, ",");

    // Parse each token and convert it to float, then store in the array
    while (token != NULL && count < MAX_ITEMS) {
        floatArray[count] = atoi(token);  // Convert string to float
        count++;
        token = strtok(NULL, ",");  // Get next token
    }


return count;

}

//void relay(char* floatString, uint8_t * binaryA)

int main(void)
  {
    platform_init();
  init_uart();
  trigger_setup();

    uint16_t count = 0;
    char test_vector[300];
    uint8_t test_vector_float[MAX_ITEMS];
    //float output[5] = {0,0,0,0,0};
    char pred[1];
    char count_token[4]; 
    uint8_t c = 0;
    float layer1_output[20];
    float layer2_output[4] = {0,0,0,0,};
    float output[4] = {0,0,0,0};
  while(1){

        // my_puts("*****Safe-o-matic 3000 Booting...\n");
        // //Print some fancy-sounding stuff so that attackers
        // //will get scared and leave us alone
        // my_puts("Aligning bits........[DONE]\n");
        // delay_2_ms();
        // my_puts("Checking Cesium RNG..[DONE]\n");
        // delay_2_ms();
        // my_puts("Masquerading flash...[DONE]\n");
        // delay_2_ms();
        // my_puts("Decrypting database..[DONE]\n");
         delay_2_ms();
         delay_2_ms();
         delay_2_ms();
         delay_2_ms();
        
        // my_puts("\n\n");

        //Give them one last warning
//        my_puts("WARNING: UNAUTHORIZED ACCESS WILL BE PUNISHED\n");

        trigger_low();

        //Get password
        //my_puts("Please enter password to continue: ");
        my_read(test_vector, 300);

//        uint8_t passbad = 0;
//         binaryStringToArray(test_vector, test_vector_int);

        count = floatStringToArray(test_vector, test_vector_float);
        delay_2_ms();
        delay_2_ms();
        delay_2_ms();
        delay_2_ms();
//        for(c=0;c<10;c++){
//          delay_2_ms();  
//        }

        //citoa(count,count_token,10);

        //my_puts(count_token);
        //my_puts(test_vector);
        trigger_high();

        //predict(test_vector_float,output);
        //pred = char(48+


        forward(test_vector_float, layer1_output,layer2_output,output);
        implement_softmax(output,pred);


        //my_puts(pred);
        //my_puts(test_vector);
        // delay_2_ms();
        // delay_2_ms();
        // //my_puts(pred);
        // delay_2_ms();
        // delay_2_ms();
        
        for(c=0;c<10;c++){
          delay_2_ms();  
        }
        //my_puts("pred");
        my_puts(pred);
        //All done;
        while(1);
  }

  return 1;
  }


