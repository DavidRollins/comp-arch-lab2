/*
    Remove all unnecessary lines (including this one) 
    in this comment.
    REFER TO THE SUBMISSION INSTRUCTION FOR DETAILS

    Name 1: Dilya Anvarbekova 
    Name 2: David Rollins
    UTEID 1: da28364
    UTEID 2: der2366
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Instruction Level Simulator                         */
/*                                                             */
/*   EE 460N                                                   */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files: isaprogram   LC-3b machine language program file     */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void process_instruction();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A 
*/

#define WORDS_IN_MEM    0x08000 
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */


typedef struct System_Latches_Struct{

  int PC,		/* program counter */
    N,		/* n condition bit */
    Z,		/* z condition bit */
    P;		/* p condition bit */
  int REGS[LC_3b_REGS]; /* register file. */
} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int INSTRUCTION_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands                    */
/*                                                             */
/***************************************************************/
void help() {                                                    
  printf("----------------LC-3b ISIM Help-----------------------\n");
  printf("go               -  run program to completion         \n");
  printf("run n            -  execute program for n instructions\n");
  printf("mdump low high   -  dump memory from low to high      \n");
  printf("rdump            -  dump the register & bus values    \n");
  printf("?                -  display this help menu            \n");
  printf("quit             -  exit the program                  \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {                                                

  process_instruction();
  CURRENT_LATCHES = NEXT_LATCHES;
  INSTRUCTION_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {                                      
  int i;

  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating for %d cycles...\n\n", num_cycles);
  for (i = 0; i < num_cycles; i++) {
    if (CURRENT_LATCHES.PC == 0x0000) {
	    RUN_BIT = FALSE;
	    printf("Simulator halted\n\n");
	    break;
    }
    cycle();
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed                 */
/*                                                             */
/***************************************************************/
void go() {                                                     
  if (RUN_BIT == FALSE) {
    printf("Can't simulate, Simulator is halted\n\n");
    return;
  }

  printf("Simulating...\n\n");
  while (CURRENT_LATCHES.PC != 0x0000)
    cycle();
  RUN_BIT = FALSE;
  printf("Simulator halted\n\n");
}

/***************************************************************/ 
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {          
  int address; /* this is a byte address */

  printf("\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  printf("-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    printf("  0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  printf("\n");

  /* dump the memory contents into the dumpsim file */
  fprintf(dumpsim_file, "\nMemory content [0x%.4x..0x%.4x] :\n", start, stop);
  fprintf(dumpsim_file, "-------------------------------------\n");
  for (address = (start >> 1); address <= (stop >> 1); address++)
    fprintf(dumpsim_file, " 0x%.4x (%d) : 0x%.2x%.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */   
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {                               
  int k; 

  printf("\nCurrent register/bus values :\n");
  printf("-------------------------------------\n");
  printf("Instruction Count : %d\n", INSTRUCTION_COUNT);
  printf("PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  printf("Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    printf("%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  printf("\n");

  /* dump the state information into the dumpsim file */
  fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
  fprintf(dumpsim_file, "-------------------------------------\n");
  fprintf(dumpsim_file, "Instruction Count : %d\n", INSTRUCTION_COUNT);
  fprintf(dumpsim_file, "PC                : 0x%.4x\n", CURRENT_LATCHES.PC);
  fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
  fprintf(dumpsim_file, "Registers:\n");
  for (k = 0; k < LC_3b_REGS; k++)
    fprintf(dumpsim_file, "%d: 0x%.4x\n", k, CURRENT_LATCHES.REGS[k]);
  fprintf(dumpsim_file, "\n");
  fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */  
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {                         
  char buffer[20];
  int start, stop, cycles;

  printf("LC-3b-SIM> ");

  scanf("%s", buffer);
  printf("\n");

  switch(buffer[0]) {
  case 'G':
  case 'g':
    go();
    break;

  case 'M':
  case 'm':
    scanf("%i %i", &start, &stop);
    mdump(dumpsim_file, start, stop);
    break;

  case '?':
    help();
    break;
  case 'Q':
  case 'q':
    printf("Bye.\n");
    exit(0);

  case 'R':
  case 'r':
    if (buffer[1] == 'd' || buffer[1] == 'D')
	    rdump(dumpsim_file);
    else {
	    scanf("%d", &cycles);
	    run(cycles);
    }
    break;

  default:
    printf("Invalid Command\n");
    break;
  }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {                                           
  int i;

  for (i=0; i < WORDS_IN_MEM; i++) {
    MEMORY[i][0] = 0;
    MEMORY[i][1] = 0;
  }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename) {                   
  FILE * prog;
  int ii, word, program_base;

  /* Open program file. */
  prog = fopen(program_filename, "r");
  if (prog == NULL) {
    printf("Error: Can't open program file %s\n", program_filename);
    exit(-1);
  }

  /* Read in the program. */
  if (fscanf(prog, "%x\n", &word) != EOF)
    program_base = word >> 1;
  else {
    printf("Error: Program file is empty\n");
    exit(-1);
  }

  ii = 0;
  while (fscanf(prog, "%x\n", &word) != EOF) {
    /* Make sure it fits. */
    if (program_base + ii >= WORDS_IN_MEM) {
	    printf("Error: Program file %s is too long to fit in memory. %x\n",
             program_filename, ii);
	    exit(-1);
    }

    /* Write the word to memory array. */
    MEMORY[program_base + ii][0] = word & 0x00FF;
    MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;
    ii++;
  }

  if (CURRENT_LATCHES.PC == 0) CURRENT_LATCHES.PC = (program_base << 1);

  printf("Read %d words from program into memory.\n\n", ii);
}

/************************************************************/
/*                                                          */
/* Procedure : initialize                                   */
/*                                                          */
/* Purpose   : Load machine language program                */ 
/*             and set up initial state of the machine.     */
/*                                                          */
/************************************************************/
void initialize(char *program_filename, int num_prog_files) { 
  int i;

  init_memory();
  for ( i = 0; i < num_prog_files; i++ ) {
    load_program(program_filename);
    while(*program_filename++ != '\0');
  }
  CURRENT_LATCHES.Z = 1;  
  NEXT_LATCHES = CURRENT_LATCHES;
    
  RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {                              
  FILE * dumpsim_file;

  /* Error Checking */
  if (argc < 2) {
    printf("Error: usage: %s <program_file_1> <program_file_2> ...\n",
           argv[0]);
    exit(1);
  }

  printf("LC-3b Simulator\n\n");

  initialize(argv[1], argc - 1);

  if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
    printf("Error: Can't open dumpsim file\n");
    exit(-1);
  }

  while (1)
    get_command(dumpsim_file);
    
}

/***************************************************************/
/* Do not modify the above code.
   You are allowed to use the following global variables in your
   code. These are defined above.

   MEMORY

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */

/***************************************************************/

int sext(int value, int bits)
{
    int sign = (1 << (bits - 1)) & value;
    int mask = ((~0U) >> (bits - 1)) << (bits - 1);
    if (sign != 0)
        value |= mask;
    else
        value &= ~mask;
    return value;
}

void setcc(int reg) {
   int b = reg>>15;
   if (reg == 0) {
      NEXT_LATCHES.P = 0;
      NEXT_LATCHES.Z = 1;
      NEXT_LATCHES.N = 0;
   } else if (b == 1) {
      NEXT_LATCHES.P = 0;
      NEXT_LATCHES.Z = 0;
      NEXT_LATCHES.N = 1;
   } else {
      NEXT_LATCHES.P = 1;
      NEXT_LATCHES.Z = 0;
      NEXT_LATCHES.N = 0;
   }
}

void process_instruction(){
  /*  function: process_instruction
   *  
   *    Process one instruction at a time  
   *       -Fetch one instruction
   *       -Decode 
   *       -Execute
   *       -Update NEXT_LATCHES
   */     
  int instructionRegister = 0;
  
  if(CURRENT_LATCHES.PC/2 < WORDS_IN_MEM){
    instructionRegister = (MEMORY[CURRENT_LATCHES.PC/2][1] << 8) + MEMORY[CURRENT_LATCHES.PC/2][0]; 
    NEXT_LATCHES = CURRENT_LATCHES;
    NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2; // next instruction
  } else{
    exit(1); // idk just feels like we shouldn't go past memory
  }
  
  int opcode = ((instructionRegister) & (0xF000)) >> 12; // get opcode
  
  if (opcode == 0){ // br
  /*
    if ((n AND N) OR (z AND Z) OR (p AND P)) 
    PC = PC + LSHF(SEXT(PCoffset9), 1);

    n is 11th bit, z is 10th bit, p is 9th bit
  */
  int nzp = instructionRegister >> 9; // get the nzp bits
  int n = 0;
  int z = 0;
  int p = 0;
  // 000, 001, 010, 011, 100, 101, 110, 111
  switch (nzp) {
    case 1:
    {
      p = 1;
      break;
    }
    case 2:
    {
      z = 1;
      break;
    }
    case 3:
    {
      z = 1;
      p = 1;
      break;
    }
    case 4:
    {
      n = 1;
      break;
    }
    case 5:
    {
      n = 1;
      p = 1; 
      break;
    }
    case 6:
    {
      n = 1;
      z = 1;
      break;
    }
    case 7:
    {
      n = 1;
      z = 1;
      p = 1;
      break;
    }

  } 
  if (n & CURRENT_LATCHES.N || z & CURRENT_LATCHES.Z || p & CURRENT_LATCHES.P) {
    int pcOffset9 = (instructionRegister & 0x01FF); // get only the offset from the word
    NEXT_LATCHES.PC = Low16bits(NEXT_LATCHES.PC + Low16bits(sext(pcOffset9, 9)));
  }


  // if nzp then registers = 1, 110, 11100
  
  } else if (opcode == 1){ // add
  /*
  if (bit[5] == 0)
    DR = SR1 + SR2;
  else
    DR = SR1 + SEXT(imm5);
  setcc();
  */
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  
  int sourceRegister1 = ((instructionRegister) & 0x01C0)>> 6;
  
  int A = ((instructionRegister) & 0x0020)>> 5;
  if(A == 0 ){
    int sourceRegister2 = ((instructionRegister) & 0x0007); 
    
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] + CURRENT_LATCHES.REGS[sourceRegister2]);
  } else if (A){
    int imm5 = ((instructionRegister) & 0x001F);
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] + sext(imm5, 5));
  }
  setcc(NEXT_LATCHES.REGS[destinationRegister]);
  
  } else if (opcode == 2){ // ldb
/*
DR = SEXT(mem[BaseR + SEXT(boffset6)]); 
setcc();
*/
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int baseRegister = ((instructionRegister) & 0x01C0) >> 6;
  int bOffset6 = ((instructionRegister) & 0x003F); 

  int memAddress = Low16bits(CURRENT_LATCHES.REGS[baseRegister] + sext(bOffset6, 6));
  int whichByte = memAddress%2;
  memAddress /= 2;

  NEXT_LATCHES.REGS[destinationRegister] = sext(MEMORY[memAddress][whichByte], 8); 
  setcc(NEXT_LATCHES.REGS[destinationRegister]);
  
  } else if (opcode == 3){ // stb
/*
mem[BaseR + SEXT(boffset6)] = SR[7:0];
*/
  int sourceRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int baseRegister = ((instructionRegister) & 0x01C0)>> 6;
  int bOffset6 = ((instructionRegister) & 0x003F); 

  int memAddress = Low16bits(CURRENT_LATCHES.REGS[baseRegister] + sext(bOffset6, 6));
  int whichByte = memAddress%2;
  memAddress /= 2;
  MEMORY[memAddress][whichByte] = CURRENT_LATCHES.REGS[sourceRegister] & 0x00FF;


  } else if (opcode == 4){ // jsr(r)

  /*
R7 = PC ;
if (bit[11] == 0)
PC = BaseR; else
PC = PC + LSHF(SEXT(PCoffset11), 1);
  */
    int pc = NEXT_LATCHES.PC;
    int bit11 = ((instructionRegister) & 0x0800); // 0000 1000 0000 0000
    if (bit11 == 1) {
      int pcoffset11 = ((instructionRegister) & 0x07FF);
            NEXT_LATCHES.PC = Low16bits(pc + sext(pcoffset11, 11)<<1);
    } else {
        int baseRegister = ((instructionRegister) & 0x01C0); // 0000 0001 1100 0000
        NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baseRegister];
    }
      NEXT_LATCHES.REGS[7] = pc;

  } else if (opcode == 5){ // and
/*
if (bit[5] == 0)
DR = SR1 AND SR2;
else
DR = SR1 AND SEXT(imm5);
setcc();
*/
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int sourceRegister1 = ((instructionRegister) & 0x01C0)>> 6;
  int A = ((instructionRegister) & 0x0020)>> 5;
  if(A == 0 ){
    int sourceRegister2 = ((instructionRegister) & 0x0007); 
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] & CURRENT_LATCHES.REGS[sourceRegister2]);
  } else {
    int imm5 = ((instructionRegister) & 0x001F);
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] & sext(imm5, 5));
  }
  setcc(NEXT_LATCHES.REGS[destinationRegister]);
  

  } else if (opcode == 6){ // ldw
/*
DR = MEM[BaseR + LSHF(SEXT(offset6), 1)];
setcc();
*/
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int baseRegister = ((instructionRegister) & 0x01C0) >> 6;
  int bOffset6 = ((instructionRegister) & 0x003F); 

  int memAddress = Low16bits(CURRENT_LATCHES.REGS[baseRegister] + (sext(bOffset6, 6) << 1))/2;
  NEXT_LATCHES.REGS[destinationRegister] = MEMORY[memAddress][0] + (MEMORY[memAddress][1] << 8); 
  setcc(NEXT_LATCHES.REGS[destinationRegister]);


  } else if (opcode == 7){ // stw
/*
MEM[BaseR + LSHF(SEXT(offset6), 1)] = SR;
*/
  int sourceRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int baseRegister = ((instructionRegister) & 0x01C0)>> 6;
  int bOffset6 = ((instructionRegister) & 0x003F); 

  int memAddress = Low16bits(CURRENT_LATCHES.REGS[baseRegister] + (sext(bOffset6, 6)<<1))/2;
  MEMORY[memAddress][1] = CURRENT_LATCHES.REGS[sourceRegister] >> 8;
  MEMORY[memAddress][0] = CURRENT_LATCHES.REGS[sourceRegister] & 0x00FF;

  }  // rti --- assuming rti is not in our input file
/*
if (PSR[15] == 1) privilege mode violation PC = MEM[R6]; R6 is the SSP
R6 = R6 + 2;
TEMP = MEM[R6];
R6 = R6+2
PSR = TEMP; the privilege mode and condition codes of the interrupted process are restored
*/
  else if (opcode == 9){ // xor, not
/*

if (bit[5] == 0)
DR = SR1 XOR SR2;
else
DR = SR1 XOR SEXT(imm5);
setcc();
*/
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int sourceRegister1 = ((instructionRegister) & 0x01C0)>> 6;
  int A = ((instructionRegister) & 0x0020)>> 5;

  if(A == 0 ){
    int sourceRegister2 = ((instructionRegister) & 0x0007); 
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] ^ CURRENT_LATCHES.REGS[sourceRegister2]);
  } else {
    int imm5 = ((instructionRegister) & 0x001F);
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1]  ^ sext(imm5, 5));
  }
  setcc(NEXT_LATCHES.REGS[destinationRegister]);
  }  // 10 and 11 not used
  else if (opcode == 12){  // jmp
/*
PC = BaseR;
*/
  int baseRegister = ((instructionRegister) & 0x01C0)>> 6;
  NEXT_LATCHES.PC = CURRENT_LATCHES.REGS[baseRegister];

  } else if (opcode == 13){ // shf
/*
if (bit[4] == 0)
DR = LSHF(SR, amount4);
else
if (bit[5] == 0)
DR = RSHF(SR, amount4, 0); 
else
DR = RSHF(SR, amount4, SR[15]); 
setcc();

Bit [4] determines the direction (left or right) of the shift; bit [5] determines whether a right shift is arithmetic or logical. 
If bit [4] is 0, the source operand in SR is shifted left by the number of bit positions indicated by amount4. 
If bit [4] is 1, the source operand is shifted right by amount4 bits. 
If the operation is a right shift, bit [5] of the instruction determines whether the sign bit of the original source operand is preserved. 
If bit [5] is 1, the right shift is an arithmetic shift; thus the original SR[15] is shifted into the vacated bit positions. If bit[5] is 0, zeroes are shifted into the vacated bit positions. 
The result is stored in DR. The condition codes are set, based on whether the result is negative, zero, or positive.


*/
  int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
  int sourceRegister1 = ((instructionRegister) & 0x01C0)>> 6;
  int bit5 = ((instructionRegister) & 0x0020)>>5;
  int bit4 = ((instructionRegister) & 0x0010)>>4;  

  int amount4 = ((instructionRegister) & 0x000F);

  if(!bit4){
    NEXT_LATCHES.REGS[destinationRegister] = Low16bits(CURRENT_LATCHES.REGS[sourceRegister1] << amount4);
  } else if (bit5==0){
    NEXT_LATCHES.REGS[destinationRegister] = CURRENT_LATCHES.REGS[sourceRegister1] >> amount4;
  }else if (bit5){
    // thus the original SR[15] is shifted into the vacated bit positions. If bit[5] is 0, zeroes are shifted into the vacated bit positions. 
    if(CURRENT_LATCHES.REGS[sourceRegister1] >> 15 == 0){
      NEXT_LATCHES.REGS[destinationRegister] = CURRENT_LATCHES.REGS[sourceRegister1] >> amount4;
   
    }
    else { // otherwise, we need to rotate the numbers
      int temp = CURRENT_LATCHES.REGS[sourceRegister1];
      // shift the bits to the next index
      while(amount4 > 0){
        temp = (temp>>1) + 0x8000; // iteratively add 1's into the register
        amount4--;
      }
      NEXT_LATCHES.REGS[destinationRegister] = temp;
    }
  }

  setcc(destinationRegister);

  } else if (opcode == 14){ // lea

  /*
DR = PC + LSHF(SEXT(PCoffset9),1); 
setcc();
  */
 int destinationRegister = ((instructionRegister) & 0x0E00)>> 9; // want bits 9, 10, 11 X0E00
   int pcOffset9 = (instructionRegister & 0x01FF); // get only the offset from the word
  NEXT_LATCHES.REGS[destinationRegister] = Low16bits(NEXT_LATCHES.PC + Low16bits(sext(pcOffset9, 9) << 1));

  } else if (opcode == 15){ // trap
/*
R7 = PC ;
PC = MEM[LSHF(ZEXT(trapvect8), 1)];
*/ 
  NEXT_LATCHES.REGS[7] = NEXT_LATCHES.PC;
  int trapvect8 = (instructionRegister & 0x00FF);
  NEXT_LATCHES.PC =  ((MEMORY[trapvect8][1] << 8) + MEMORY[trapvect8][0]);

  } 









}