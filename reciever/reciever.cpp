#include "../rc-switch/RCSwitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>


RCSwitch mySwitch;
#define BUF_LEN 80
typedef enum { F, T } boolean;

void gameGuesser(){
    int num, lastNum, value, triesNum = 0;
    boolean true = F;
    char gameRun;
    srand(time(NULL));
    num = rand() % 100 + 1;

    while(1){
        if(true == T){
          printf("We are honored. New game has started! \n");
          true = F;
          triesNum = 0;
          num = rand() % 100 + 1;
        }

        if (mySwitch.available()){
          value = mySwitch.getReceivedValue();

          if(value == 0) printf("Unknonw encoding!\n");
          else{
            if (num != lastNum){
              printf("Recieved %i\n", value);
              triesNum++;

              if (value == num){
                printf("Congratulations, you have guessed the correct number! Number of tries: %d\n", triesNum);
                write(file_desc, &value, 1);
                true = T;
              } else if (value < num) printf("Too bad, number you are looking for is bigger!\n");
                else printf("Too bad, number you are looking for is smaller!\n");
            }
          }
          lastNum = value;
          fflush(stdout);
          mySwitch.resetAvailable();
        }
          usleep(100);
          printf("Do you want to play again? Y for true, F for false. \n");
          scanf("%c", gameRun);
          if (gameRun == 'F'){
            printf("Thanks for playing! Reciever out!\n");
            return;
          }
    }

}

int main(int argc, char *argv[]) {

     int PIN = 2;
     int file_desc= open("/dev/gpio_driver", O_RDWR); 

    if (file_desc < 0){
      printf("Error, could not open file.");
      return -1;
    }

    if(wiringPiSetup() == -1) {
       printf("wiringPiSetup failed, exiting...");
       return -2;
     }

     int pulseLength = 0;
     if (argv[1] != NULL) pulseLength = atoi(argv[1]);

     mySwitch = RCSwitch();
     if (pulseLength != 0) mySwitch.setPulseLength(pulseLength);
     mySwitch.enableReceive(PIN);
     
     gameGuesser();
     close(file_desc);

     exit(0);
}