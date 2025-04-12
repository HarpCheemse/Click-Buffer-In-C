#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PRINT_RED "\x1b[31m" 
#define PRINT_GREEN "\x1b[32m"
#define PRINT_AQUA "\x1b[36m"
#define PRINT_GOLD "\033[1;33m"
#define PRINT_LIGHT_AQUA "\033[1;33m""\x1b[36m"
#define PRINT_COLOR_RESET "\x1b[0m" 
#define PRINT_CYAN   "\033[0;36m"
#define PRINT_MAGENTA "\033[0;35m"

#define RATE_COLOR_1 "\033[0;92m" // Bright Green
#define RATE_COLOR_2 "\033[0;32m" // Green
#define RATE_COLOR_3 "\033[0;33m" // Yellow
#define RATE_COLOR_4 "\033[0;91m" // Light red
#define RATE_COLOR_5 "\033[0;31m" // Dark Red

#define INPUT_ERROR -1
#define MAX_SIZE 255

#define DETECT_LEFT_CLICK  (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
#define DETECT_RIGHT_CLICK (GetAsyncKeyState(VK_RBUTTON) & 0x8000) 

char VERSION[20] = "1.1.2";
int TOGGLE_LEFT_CLICK_BUFFER = 1;
int TOGGLE_RIGHT_CLICK_BUFFER = 1;

int LEFT_CLICK_BUFFER_RATE = 1;
int RIGHT_CLICK_BUFFER_RATE = 1;

/*                     Photopyes                    */
void getKeybind();
void printMenu();
void changeCPS();
void refreshMenu();
void printUserPrompt();
void leftClick();
void rightClick();

/*                     Input Validation              */
void clearBuffer()
{
    int ch;
    while(ch = getchar() != '\n')
    {}
}
bool intInputHandler(const char* input)
{
	int index = 0;
	if(input[0] =='.') return false;
	if(input[0] =='-') return false;
    int length = strlen(input);
    for(; index<length; index++)
    {
        if(input[index] <='9' && input[index]>='0') continue;
        else return false;
    }
    return true;
}
int intInput()  //my original input validation, just reuse it :>
{
	char buffer[MAX_SIZE];
    while(1)
    {
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer,"\n")] = '\0';
        if(intInputHandler(buffer)) return atoi(buffer);
        else return INPUT_ERROR; 
    }
}
/*                     Input Function                */
void change_LEFT_CLICK_BUFFER_RATE()
{
    clearBuffer();
    printf(PRINT_LIGHT_AQUA"\nChange "PRINT_COLOR_RESET PRINT_GREEN"LEFT"PRINT_COLOR_RESET PRINT_LIGHT_AQUA" Click Buffer Rate: "PRINT_COLOR_RESET);
    while(1)
    {
        int temp = intInput();
        if(temp <= 0 || temp > 5)
        {
            refreshMenu();
            printf(PRINT_RED"Please Enter Integer Number Between 1 And 5"PRINT_COLOR_RESET);
            printf(PRINT_LIGHT_AQUA"\nChange "PRINT_COLOR_RESET PRINT_GREEN"LEFT"PRINT_COLOR_RESET PRINT_LIGHT_AQUA" Click Buffer Rate: "PRINT_COLOR_RESET);
        }
        else
        {
            LEFT_CLICK_BUFFER_RATE = temp;
            return;
        }
    }
}
void change_RIGHT_CLICK_BUFFER_RATE()
{
    clearBuffer();
    printf(PRINT_LIGHT_AQUA"\nChange "PRINT_COLOR_RESET PRINT_GREEN"RIGHT"PRINT_COLOR_RESET PRINT_LIGHT_AQUA" Click Buffer Rate: "PRINT_COLOR_RESET);
    while(1)
    {
        int temp = intInput();
        if(temp <= 0 || temp > 5)
        {
            refreshMenu();
            printf(PRINT_RED"Please Enter Integer Number Between 1 And 5"PRINT_COLOR_RESET);
            printf(PRINT_LIGHT_AQUA"\nChange "PRINT_COLOR_RESET PRINT_GREEN"RIGHT"PRINT_COLOR_RESET PRINT_LIGHT_AQUA" Click Buffer Rate: "PRINT_COLOR_RESET);
        }
        else
        {
            RIGHT_CLICK_BUFFER_RATE = temp;
            return;
        }
    }
}
/*                     MENU FUNCTION                 */
const char* rateColor(int rate)
{
    const char* rate_color =
    (rate == 1) ? RATE_COLOR_1 :
    (rate == 2) ? RATE_COLOR_2 :
    (rate == 3) ? RATE_COLOR_3 :
    (rate == 4) ? RATE_COLOR_4 :
    RATE_COLOR_5;
    return rate_color;
}
void printAboutProgram()
{
    refreshMenu();
    printf(PRINT_AQUA"\n\n================================= " PRINT_COLOR_RESET PRINT_LIGHT_AQUA "CREDITS " PRINT_COLOR_RESET PRINT_AQUA"=================================\n" PRINT_COLOR_RESET);
    printf(PRINT_GOLD"\nDeveloped by: HarpCheemse");
    printf("\nVersion: %s",VERSION);
    printf("\n- [Any libraries or resources used]");
    printf("\nContact: [harpcheemse@gmail.com]" PRINT_COLOR_RESET);
    printf(PRINT_AQUA"\n===========================================================================\n\n\n"PRINT_COLOR_RESET);
    printf(PRINT_AQUA"\n\n================================= " PRINT_COLOR_RESET PRINT_LIGHT_AQUA "HOW TO USE " PRINT_COLOR_RESET PRINT_AQUA"===============================\n" PRINT_COLOR_RESET);
    printf(PRINT_GOLD"\nUse Number 1 ->6 To Choose Yout Option And Hit Enter. \nThere Are 5 Levels (1 - 5) To Choose, Each Will Greatly Affect Your CPS");
    printf(PRINT_COLOR_RESET PRINT_AQUA"\n===========================================================================\n"PRINT_COLOR_RESET);
    printf(PRINT_AQUA"\n\n================================= " PRINT_COLOR_RESET PRINT_LIGHT_AQUA " LICENSE" PRINT_COLOR_RESET PRINT_AQUA"=================================\n" PRINT_COLOR_RESET);
    printf(PRINT_GOLD"MIT License\n\n");

    printf("Copyright (c) 2025 HarpCheemse\n\n");
    
    printf("Permission is hereby granted, free of charge, to any person obtaining a copy\n");
    printf("of this software and associated documentation files (the \"Software\"), to deal\n");
    printf("in the Software without restriction, including without limitation the rights\n");
    printf("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n");
    printf("copies of the Software, and to permit persons to whom the Software is\n");
    printf("furnished to do so, subject to the following conditions:\n\n");
    
    printf("The above copyright notice and this permission notice shall be included in all\n");
    printf("copies or substantial portions of the Software.\n\n");
    
    printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
    printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n");
    printf("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n");
    printf("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n");
    printf("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n");
    printf("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n");
    printf("SOFTWARE.\n");
    
    printf(PRINT_AQUA"\n===========================================================================\n"PRINT_COLOR_RESET);
}
void printMenu()
{
    char line[255] = "========================================================================";
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
    printf(PRINT_LIGHT_AQUA"                          ----Click Buffer----        \n\n" PRINT_COLOR_RESET PRINT_GOLD);
    printf("1. Toggle " PRINT_COLOR_RESET PRINT_CYAN "LEFT" PRINT_COLOR_RESET PRINT_GOLD " Click Buffer: %s%-7s%s"
        PRINT_GOLD "    2. Toggle " PRINT_COLOR_RESET PRINT_MAGENTA "RIGHT" PRINT_COLOR_RESET PRINT_GOLD " Click Buffer: %s%-7s%s\n\n",
        (TOGGLE_LEFT_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_LEFT_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET,
        (TOGGLE_RIGHT_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_RIGHT_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET);
    printf(PRINT_GOLD "3. Customize " PRINT_COLOR_RESET PRINT_CYAN "LEFT " 
            PRINT_COLOR_RESET PRINT_GOLD "Click Buffer          " PRINT_COLOR_RESET);
     
     printf(PRINT_GOLD "4. Customize " PRINT_COLOR_RESET PRINT_MAGENTA "RIGHT" 
            PRINT_COLOR_RESET PRINT_GOLD " Click Buffer\n" PRINT_COLOR_RESET);
     
     printf(PRINT_GOLD "     ----- LEVEL " PRINT_COLOR_RESET "%s%d" PRINT_COLOR_RESET 
            PRINT_GOLD " -----        ", rateColor(LEFT_CLICK_BUFFER_RATE), LEFT_CLICK_BUFFER_RATE);
     
     printf(PRINT_GOLD "               ----- LEVEL " PRINT_COLOR_RESET "%s%d" PRINT_COLOR_RESET 
            PRINT_GOLD " -----\n\n", rateColor(RIGHT_CLICK_BUFFER_RATE), RIGHT_CLICK_BUFFER_RATE);
     
    if (TOGGLE_LEFT_CLICK_BUFFER) 
    {
        int result_cps = 0;
        switch (LEFT_CLICK_BUFFER_RATE) 
        {
            case 1: result_cps = 14; break;
            case 2: result_cps = 18; break;
            case 3: result_cps = 24; break;
            case 4: result_cps = 30; break;
            case 5: result_cps = 40; break;
        }
        printf(PRINT_GOLD "Average " PRINT_COLOR_RESET RATE_COLOR_1 "7 Cps"
            PRINT_COLOR_RESET PRINT_GOLD " Is Now ~ " PRINT_COLOR_RESET "%s%d CPS        "
            PRINT_COLOR_RESET, rateColor(LEFT_CLICK_BUFFER_RATE), result_cps);
    } 
    else  printf(PRINT_COLOR_RESET PRINT_RED "   LEFT Click Buffer Is Off         " PRINT_COLOR_RESET);

    if (TOGGLE_RIGHT_CLICK_BUFFER) 
    {
        if(!TOGGLE_LEFT_CLICK_BUFFER) printf(" ");
        int result_cps = 0;
        switch (RIGHT_CLICK_BUFFER_RATE) 
        {
            case 1: result_cps = 14; break;
            case 2: result_cps = 18; break;
            case 3: result_cps = 24; break;
            case 4: result_cps = 30; break;
            case 5: result_cps = 40; break;
        }
        printf(PRINT_GOLD "   Average " PRINT_COLOR_RESET RATE_COLOR_1 "7 Cps"
            PRINT_COLOR_RESET PRINT_GOLD " Is Now ~ " PRINT_COLOR_RESET "%s%d CPS\n"
            PRINT_COLOR_RESET, rateColor(RIGHT_CLICK_BUFFER_RATE), result_cps);
    } 
    else if(TOGGLE_LEFT_CLICK_BUFFER && !TOGGLE_RIGHT_CLICK_BUFFER) printf(PRINT_COLOR_RESET PRINT_RED "        RIGHT Click Buffer Is Off\n" PRINT_COLOR_RESET);
    else if(!TOGGLE_LEFT_CLICK_BUFFER && !TOGGLE_RIGHT_CLICK_BUFFER) printf(PRINT_COLOR_RESET PRINT_RED "         RIGHT Click Buffer Is Off\n" PRINT_COLOR_RESET);

    printf(PRINT_GOLD "\n5. About The Program                   ");
    printf(" 6. Exit The Program\n\n"PRINT_COLOR_RESET);
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
}
void refreshMenu()
{
    system("cls");
    printMenu();
}
void printUserPrompt()
{
    printf(PRINT_LIGHT_AQUA"\nEnter Your Choice: "PRINT_COLOR_RESET);
}
void menuHandler()
{
    refreshMenu();
    printUserPrompt();
    while(1)
    {
        int user_selection;
        scanf("%d", &user_selection);
        switch (user_selection)
        {
        case 1:  // Toggle LEFT Click Buffer ON/OFF
            refreshMenu();
            printUserPrompt();
            TOGGLE_LEFT_CLICK_BUFFER = !TOGGLE_LEFT_CLICK_BUFFER;
            refreshMenu();
            printUserPrompt();
            break;
        case 2:  // Toggle RIGHT Click Buffer ON/OFF
            refreshMenu();
            printUserPrompt();
            TOGGLE_RIGHT_CLICK_BUFFER = !TOGGLE_RIGHT_CLICK_BUFFER;
            refreshMenu();
            printUserPrompt();
            break;
        case 3: // Change LEFT Click Buffer Rate
            refreshMenu();
            change_LEFT_CLICK_BUFFER_RATE();
            refreshMenu();
            printUserPrompt();
            break;
        case 4: // Change LEFT Click Buffer Rate
            refreshMenu();
            change_RIGHT_CLICK_BUFFER_RATE();
            refreshMenu();
            printUserPrompt();
            break;
        case 5:  // About The Program
            printAboutProgram();
            printf(PRINT_AQUA"Press Enter to continue..."PRINT_COLOR_RESET);
            while (getchar() != '\n');
            getchar();
            refreshMenu();
            printUserPrompt();
            break;
        case 6:
            refreshMenu();
            printf(PRINT_GREEN"Exit Program Succesfully"PRINT_COLOR_RESET);
            exit(0);
        default:
            clearBuffer();
            refreshMenu();
            printf(PRINT_RED"Invalid Input!"PRINT_COLOR_RESET);
            printUserPrompt();
            break;
        }
    }
}
/*                    Functionality                  */
float randomNumberGenerator() // generate number between 0.9 - 1.1
{
    int min = 90;
    int max = 110;
    int random_number = min + rand() % (max - min + 1);
    return random_number/100.0;
}
int isHoldingLeftClick()
{
    const DWORD HOLD_THRESHOLD = 200;
    DWORD start_timer;
    DWORD current_timer;
    start_timer = GetTickCount();
    while(DETECT_LEFT_CLICK)     //detect left click and start the timer. if timer > threshold => holding key
    {
        current_timer = GetTickCount();
        if( current_timer - start_timer> HOLD_THRESHOLD)
        {
            return 1;  // no break to consume leftover hold LBUTTON
        }
        Sleep(1);
    }
    return 0;
}
int isHoldingRightClick()
{
    const DWORD HOLD_THRESHOLD = 200;
    DWORD start_timer;
    DWORD current_timer;
    start_timer = GetTickCount();
    while(DETECT_RIGHT_CLICK)   //detect right click and start the timer. if timer > threshold => holding key
    {
        current_timer = GetTickCount();
        if( current_timer - start_timer> HOLD_THRESHOLD)
        {
            return 1;  // no break to consume leftover hold LBUTTON
        }
        Sleep(1);
    }
    return 0;
}
int isLeftClickInWaitThreshold(DWORD start_timer) //Wait for another left click
{
    const DWORD WAIT_THRESHOLD = 200;
    DWORD current_timer;
    while(1)
    {
        current_timer = GetTickCount();
        if ((DETECT_LEFT_CLICK && TOGGLE_LEFT_CLICK_BUFFER)) return 1;
        if(current_timer - start_timer > WAIT_THRESHOLD) return 0;
    }
}
int isRightClickInWaitThreshold(DWORD start_timer) //Wait for another left click
{
    const DWORD WAIT_THRESHOLD = 200;
    DWORD current_timer;
    while(1)
    {
        current_timer = GetTickCount();
        if ((DETECT_RIGHT_CLICK && TOGGLE_LEFT_CLICK_BUFFER)) return 1;
        if(current_timer - start_timer > WAIT_THRESHOLD) return 0;
    }
}
void performLeftClick(int LEFT_CLICK_BUFFER_RATE)
{
    switch(LEFT_CLICK_BUFFER_RATE)
    {
        case 1:
            leftClick();
            Sleep((int)(100.0 * randomNumberGenerator()));
            break;
        case 2:
            leftClick();
            Sleep((int)(40.0 * randomNumberGenerator()));
            leftClick();
            

            break;
        case 3:
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(20.0 * randomNumberGenerator()));
            break;
        case 4:
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
        case 5:
            leftClick();
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            leftClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
    }
}
void performRightClick(int RIGHT_CLICK_BUFFER_RATE)
{
    switch(RIGHT_CLICK_BUFFER_RATE)
    {
        case 1:
            rightClick();
            Sleep((int)(100.0 * randomNumberGenerator()));
            break;
        case 2:
            rightClick();
            Sleep((int)(40.0 * randomNumberGenerator()));
            rightClick();
            

            break;
        case 3:
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(20.0 * randomNumberGenerator()));
            break;
        case 4:
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
        case 5:
            rightClick();
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            rightClick();
            Sleep((int)(10.0 * randomNumberGenerator()));
            break;
    }
}
void leftClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
void rightClick()
{
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}
void* leftClickEvent(void* arg)
{
    DWORD start_timer;
    while(1)
    {
        int is_valid_left_click = 1;
        if(DETECT_LEFT_CLICK && TOGGLE_LEFT_CLICK_BUFFER)  //detect the first left click
        {
            if(isHoldingLeftClick()) continue;  //track if it's a click or a hold
            while(is_valid_left_click)
            {
                start_timer = GetTickCount();
                if(!isLeftClickInWaitThreshold(start_timer)) is_valid_left_click = 0;  //detect if user is clicking fast
                if(isHoldingLeftClick())
                {
                    is_valid_left_click = 0; 
                    printf("STOPING ");
                }     //go back to loop one if user is clicking not fast enough / holding left click
                if(is_valid_left_click)
                {
                    performLeftClick(LEFT_CLICK_BUFFER_RATE);
                }
            }
        }    
        Sleep(1); // reduce CPS usage
    }
    return NULL;
}
void* rightClickEvent(void* arg)
{
    DWORD start_timer;
    while(1)
    {
        int is_valid_right_click = 1;
        if(DETECT_RIGHT_CLICK && TOGGLE_RIGHT_CLICK_BUFFER)  //detect the first left click
        {
            if(isHoldingRightClick()) continue;  //track if it's a click or a hold
            while(is_valid_right_click)
            {
                start_timer = GetTickCount();
                if(!isRightClickInWaitThreshold(start_timer)) is_valid_right_click = 0;  //detect if user is clicking fast
                if(isHoldingRightClick()) is_valid_right_click = 0;      //go back to loop one if user is clicking not fast enough / holding left click
                if(is_valid_right_click)
                {
                    performRightClick(RIGHT_CLICK_BUFFER_RATE);
                }
            }
        }    
        Sleep(1); // reduce CPS usage
    }
    return NULL;
}
int main() 
{
    srand(time(NULL));

    pthread_t background_left_click_event;
    pthread_t background_right_click_event;

    pthread_create(&background_left_click_event, NULL, leftClickEvent, NULL);
    pthread_create(&background_right_click_event, NULL, rightClickEvent, NULL);

    pthread_detach(background_left_click_event);             //run both thread at the same time
    pthread_detach(background_right_click_event);            //if run 1 thread only, the code has to wait for left click buffer to finish to move on to right click buffer
    
    menuHandler();
    return 0;
}
