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
#define MAX_ITEMS 18

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


#define INPUT_SIZE 18
#define OUTPUT_SIZE1 20
#define OUTPUT_SIZE2 5

float layer1_weights[OUTPUT_SIZE1][INPUT_SIZE] = {
    {0.640286,0.364561,-0.329904,-0.065119,1.087878,0.454389,-0.316937,0.074538,0.621651,0.228543,1.775838,0.600197,-0.163423,0.959136,-1.152500,-0.552064,-1.014748,0.596203},
    {-0.239140,-0.355620,0.330353,-0.289958,0.100617,0.078361,0.654019,-1.270847,-1.397006,0.630215,1.348338,-0.581591,2.042205,2.576444,0.291473,-0.183595,-0.344063,0.625423},
    {-0.228244,0.956424,0.001851,-0.270034,0.580648,0.584439,0.032728,1.076802,-0.769941,0.175436,1.132441,-1.378619,0.616771,-0.855000,-0.447821,-0.323170,0.177466,-0.778372},
    {-0.763472,-0.297093,0.776864,-0.144429,-0.085912,0.100574,-0.847709,0.156434,0.966699,-0.341069,-0.195191,-0.111998,2.939011,-0.506399,1.226526,-0.910817,-0.010495,0.484773},
    {-0.717379,0.418671,0.198062,-0.549570,-0.824166,-0.054046,0.526948,-0.355681,-1.241959,-0.630723,0.739277,-1.502986,-0.457602,0.408108,-0.691184,1.891842,-0.169814,-0.193675},
    {0.406440,-0.535038,-0.001430,0.589236,0.287676,-0.019257,-0.693751,0.829099,0.066181,-0.131999,0.719953,-1.014888,2.311907,-0.998208,-0.731358,-0.062154,-0.880854,-0.331698},
    {-0.621192,-0.585579,1.158247,0.323974,0.081428,0.249668,0.235557,-0.293540,0.246768,-0.358373,-0.091535,0.405167,-0.425957,2.892221,1.204812,0.637760,-1.043659,0.504914},
    {0.028258,0.270526,-0.504354,-0.161050,0.043740,0.165523,0.528492,0.307346,1.260717,0.765237,-0.636780,0.714348,0.662887,-1.384813,-0.011890,-0.713676,-0.549587,0.789306},
    {-0.697331,-0.413687,0.315318,0.266580,0.035945,0.113257,-0.774258,-0.040469,0.764488,0.128245,0.203818,0.089074,-0.743829,-0.258475,-1.877519,-0.381979,-0.312939,-0.097667},
    {1.257334,-0.241919,-1.612483,0.442589,0.272480,-0.243425,0.405167,-0.790703,-0.995991,-0.330424,-0.530487,0.923879,-0.564133,0.604109,-0.245915,1.834432,-1.230466,0.394564},
    {-0.725258,0.356661,-0.256944,-0.936909,-0.240841,0.177821,-0.355648,-0.319277,1.321146,-0.211582,-0.877662,-0.623776,-1.273937,0.674113,1.554140,-1.795989,0.225466,0.074561},
    {1.171936,-0.903125,0.057833,0.325364,0.477040,-0.273193,0.914805,0.659919,-1.531082,-0.044115,-0.307827,1.033239,1.185421,1.463840,1.008132,0.924246,-0.596112,0.577359},
    {-0.257145,0.381782,0.045758,-0.046075,0.101351,-0.109132,-0.149672,0.432733,0.176542,0.241487,-0.616610,0.316437,-0.572102,5.115479,1.093762,-0.515510,-0.612625,-0.148583},
    {-0.144872,-0.073082,0.121386,-0.051785,0.289475,0.460338,0.197185,0.385908,-0.026246,0.269185,-0.802936,-0.300920,-0.532069,-1.652732,2.029258,0.074833,0.465990,-0.246217},
    {-0.031828,-0.025703,-0.120238,-0.485389,-0.273564,0.465662,0.839947,-2.967748,0.258428,0.612440,-0.636720,-0.118353,-0.001134,0.420041,-0.331780,-0.146080,0.350640,0.124692},
    {1.043595,-0.187529,-1.037593,0.150322,0.460689,-0.028573,0.166808,0.747604,-0.144981,-0.309581,-0.882842,0.421519,-0.911577,0.647891,-0.695615,-0.772132,-0.997167,0.406659},
    {-0.428547,0.240364,-0.429953,0.406114,0.514962,0.706728,0.488261,0.163710,0.658353,-0.338077,0.222875,-0.657096,-0.476005,2.597808,-1.971605,-0.758201,0.236549,0.004063},
    {-1.034923,0.548285,-0.192835,-0.678710,-0.146623,0.053242,-0.026287,-0.680716,-0.342535,0.066810,-2.666526,2.067685,0.159382,0.854797,0.217266,-1.033058,-0.139986,0.263575},
    {0.087233,-0.159462,-0.560512,0.290632,0.037146,0.201908,-0.185838,-0.294198,-1.035016,-0.246593,0.354388,-0.456199,-0.941221,1.401548,-1.160782,0.199750,0.225735,0.604585},
    {0.192581,-0.379362,0.642007,-0.004319,-0.060896,-0.399983,1.556043,1.028516,-0.821439,-0.518557,-1.002539,2.102350,-0.365327,0.325063,-0.011844,0.291332,-0.969372,1.081665},
};

float layer1_bias[OUTPUT_SIZE1] = {-0.806424,0.241646,1.066198,-1.206113,1.332595,0.481697,0.206470,0.386316,0.391264,0.904476,0.239374,0.064110,-0.830701,-0.473098,1.259269,-0.270504,-0.025365,-0.615772,0.872878,0.176087};



float layer2_weights[OUTPUT_SIZE2][OUTPUT_SIZE1] = {
    {0.522647,-3.391322,-2.900689,0.149538,-0.365979,1.073748,0.381242,-0.111516,1.034441,-0.078445,-0.220379,-0.225923,-1.886620,-0.885533,1.348672,-1.723816,1.396838,0.793079,-2.072052,1.256090},
    {-1.083351,0.108866,0.073674,-2.875428,-0.378261,-0.542895,1.319856,0.750559,-2.576042,0.202084,-2.328767,-1.394354,-0.461263,1.570724,-1.065530,0.758267,-0.111214,1.954824,0.922303,-0.103649},
    {0.022463,1.461032,0.342127,-2.487505,0.600504,-1.355927,1.059955,1.544362,-0.269170,-1.140294,1.384341,1.577981,-5.089121,-2.399866,-3.316606,0.652045,0.902637,-3.280838,-1.150342,-2.613814},
    {-2.010102,-0.518362,0.634260,0.618712,-1.596431,-0.818251,-0.736533,-0.315735,0.727578,0.897471,0.605285,0.189894,0.014994,-0.678925,-0.379787,0.431748,-3.875574,0.747175,0.018660,0.233175},
    {1.467692,-0.645867,0.370776,0.371371,1.414608,1.495808,-1.458457,-1.740231,-0.563632,-2.118622,-0.939956,-1.051250,3.671424,0.767655,1.360816,-1.332708,1.282859,-1.813855,1.002632,1.004690},
};

float layer2_bias[OUTPUT_SIZE2] = {-0.879520, 0.647960, -0.549407, 0.468502, -0.106096};

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
        for (int j = 0; j < 18; ++j) {
            sum += (input[j] * layer1_weights[i][j]);
        }
        sum += layer1_bias[i];
        layer1_output[i] = relu(sum);
    }
    for (int i = 0; i < 5; ++i) {
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
    char count_token[5]; 
    uint8_t c = 0;
    float layer1_output[20];
    float layer2_output[5] = {0,0,0,0,0};
    float output[5] = {0,0,0,0,0};
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


