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
#define INPUT_ERROR -1
#define MAX_SIZE 255

#define DETECT_LEFT_CLICK  (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
#define DETECT_RIGHT_CLICK (GetAsyncKeyState(VK_RBUTTON) & 0x8000) 

char VERSION[20] = "1.1";
int TOGGLE_LEFT_CLICK_BUFFER = 1;
int TOGGLE_RIGHT_CLICK_BUFFER = 1;

int LEFT_CLICK_BUFFER_RATE = 1;
int RIGHT_CLICK_BUFFER_RATE = 1;

void getKeybind();
void printMenu();
void changeCPS();
void refreshMenu();
void printUserPrompt();

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
int intInput()  //my original input validation just reuse it :>
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
void printAboutProgram()
{
    refreshMenu();
    printf(PRINT_AQUA"\n\n================================= " PRINT_COLOR_RESET PRINT_LIGHT_AQUA "CREDITS " PRINT_COLOR_RESET PRINT_AQUA"=================================\n" PRINT_COLOR_RESET);
    printf(PRINT_GOLD"\nDeveloped by: HarpCheemse");
    printf("\nVersion: %s",VERSION);
    printf("\n- [Any libraries or resources used]");
    printf("\nContact: [harpcheemse@gmail.com]" PRINT_COLOR_RESET);
    printf(PRINT_AQUA"\n===========================================================================\n"PRINT_COLOR_RESET);
}

void printMenu()
{
    char line[255] = "============================================================";
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
    printf(PRINT_LIGHT_AQUA"                      Click Buffer        \n" PRINT_COLOR_RESET PRINT_GOLD);
    printf("1. Toggle "PRINT_COLOR_RESET PRINT_GREEN" LEFT" PRINT_COLOR_RESET PRINT_GOLD" Click Buffer. Currently %s%s%s\n",
        (TOGGLE_LEFT_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_LEFT_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET
    );
    printf(PRINT_GOLD"2. Toggle "PRINT_COLOR_RESET PRINT_GREEN"RIGHT" PRINT_COLOR_RESET PRINT_GOLD" Click Buffer. Currently %s%s%s\n\n",
        (TOGGLE_RIGHT_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_RIGHT_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET
    );
    printf(PRINT_GOLD"3. Change "PRINT_COLOR_RESET PRINT_GREEN"LEFT " PRINT_COLOR_RESET PRINT_GOLD" Click Buffer Rate: Currently: "PRINT_GREEN"Level %d \n"PRINT_COLOR_RESET, LEFT_CLICK_BUFFER_RATE);
    printf(PRINT_GOLD"4. Change "PRINT_COLOR_RESET PRINT_GREEN"RIGHT" PRINT_COLOR_RESET PRINT_GOLD" Click Buffer Rate: Currently: "PRINT_GREEN"Level %d \n\n"PRINT_COLOR_RESET,RIGHT_CLICK_BUFFER_RATE);
    printf(PRINT_GOLD"5. About The Program \n");
    printf("6. Exit The Program\n\n"PRINT_COLOR_RESET);
    if(TOGGLE_LEFT_CLICK_BUFFER)
    {
        switch(LEFT_CLICK_BUFFER_RATE)
        {
            case 1: 
                printf(PRINT_GOLD"Your LEFT  click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"14 CPS\n"PRINT_COLOR_RESET);
                break;
            case 2:
                printf(PRINT_GOLD"Your LEFT  click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"18 CPS\n"PRINT_COLOR_RESET);
                break;
            case 3:
                printf(PRINT_GOLD"Your LEFT  click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"24 CPS\n"PRINT_COLOR_RESET);
                break; 
            case 4:
                printf(PRINT_GOLD"Your LEFT  click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"30 CPS\n"PRINT_COLOR_RESET);
                break; 
            case 5:
                printf(PRINT_GOLD"Your LEFT  click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"40 CPS\n"PRINT_COLOR_RESET);
                break; 
        }
    }
    else printf(PRINT_RED"LEFT Click Buffer Is Currently Off\n"PRINT_COLOR_RESET);
    if(TOGGLE_RIGHT_CLICK_BUFFER)
    {
        switch(RIGHT_CLICK_BUFFER_RATE)
        {
            case 1: 
                printf(PRINT_GOLD"Your RIGHT click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"14 CPS\n"PRINT_COLOR_RESET);
                break;
            case 2:
                printf(PRINT_GOLD"Your RIGHT click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"18 CPS\n"PRINT_COLOR_RESET);
                break;
            case 3:
                printf(PRINT_GOLD"Your RIGHT click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"24 CPS\n"PRINT_COLOR_RESET);
                break; 
            case 4:
                printf(PRINT_GOLD"Your RIGHT click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"30 CPS\n"PRINT_COLOR_RESET);
                break; 
            case 5:
                printf(PRINT_GOLD"Your RIGHT click Average Of "PRINT_COLOR_RESET PRINT_GREEN"7 Cps" PRINT_COLOR_RESET PRINT_GOLD" Is Now ~ "PRINT_COLOR_RESET PRINT_GREEN"40 CPS\n"PRINT_COLOR_RESET);
                break; 
        }
    }
    else printf(PRINT_RED"RIGHT Click Buffer Is Currently Off\n"PRINT_COLOR_RESET);
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
    return random_number/100;
}
int isHoldingLeftClick()
{
    const DWORD HOLD_THRESHOLD = 100;
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
    const DWORD HOLD_THRESHOLD = 100;
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
    int sleep_for;
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
                if(isHoldingLeftClick()) is_valid_left_click = 0;      //go back to loop one if user is clicking not fast enough / holding left click
                if(is_valid_left_click)
                {
                    for(int i = 0; i < LEFT_CLICK_BUFFER_RATE ; i++)
                    {
                        sleep_for = 400 * randomNumberGenerator() / pow(2,LEFT_CLICK_BUFFER_RATE);
                        leftClick();
                        Sleep(sleep_for);
                    }
                }
            }
        }    
        Sleep(1); // reduce CPS usage
    }
    return NULL;
}
void* rightClickEvent(void* arg)
{
    int sleep_for;
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
                    for(int i = 0; i < RIGHT_CLICK_BUFFER_RATE ; i++)
                    {
                        sleep_for = 400 * randomNumberGenerator() / pow(2,RIGHT_CLICK_BUFFER_RATE);
                        rightClick();
                        Sleep(sleep_for);
                    }
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
