#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include <stdbool.h>

#define PRINT_RED "\x1b[31m" 
#define PRINT_GREEN "\x1b[32m"
#define PRINT_AQUA "\x1b[36m"
#define PRINT_GOLD "\033[1;33m"
#define PRINT_LIGHT_AQUA "\033[1;33m""\x1b[36m"
#define PRINT_COLOR_RESET "\x1b[0m" 
#define ERROR -1
#define MAX_SIZE 255

int TOGGLE_CLICK_BUFFER = 1;
int CLICK_BUFFER_RATE = 1;
int CLICK_PER_SECOND = 0;
int sleep_for = 1;

void getKeybind();
void printMenu();
void changeCPS();
void refreshMenu();
void printUserPrompt();

/*                     Input Validation              */
void clearBuffer()
{
    int ch;
    while(ch = getchar() != '\n') {}
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
int intInput() 
{
	char buffer[MAX_SIZE];
    while(1)
    {
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer,"\n")] = '\0';
        if(intInputHandler(buffer)) return atoi(buffer);
        else return ERROR; 
    }
}
/*                     Input Function                */
void change_CLICK_BUFFER_RATE()
{
    clearBuffer();
    printf(PRINT_LIGHT_AQUA"\nChange Click Buffer Rate: "PRINT_COLOR_RESET);
    while(1)
    {
        int temp = intInput();
        if(temp == ERROR)
        {
            refreshMenu();
            printf(PRINT_RED"Invalid Input!\n"PRINT_COLOR_RESET);
            printf(PRINT_LIGHT_AQUA"Change Click Buffer Rate: "PRINT_COLOR_RESET);
        }
        else if(temp <= 0 || temp > 10)
        {
            refreshMenu();
            printf(PRINT_RED"Please Enter Integer Number Between 1 And 10"PRINT_COLOR_RESET);
            printf(PRINT_LIGHT_AQUA"\nChange Click Buffer Rate: "PRINT_COLOR_RESET);
        }
        else
        {
            CLICK_BUFFER_RATE = temp;
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
    printf("\nVersion: 1.0");
    printf("\n- [Any libraries or resources used]");
    printf("\nContact: [harpcheemse@gmail.com]" PRINT_COLOR_RESET);
    printf(PRINT_AQUA"\n===========================================================================\n"PRINT_COLOR_RESET);
}
void printMenu()
{
    char line[255] = "============================================================";
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
    printf(PRINT_GOLD"          Click Buffer        \n");
    printf("1. Toggle Click Buffer. Currently %s%s%s\n",
        (TOGGLE_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET
    );
    printf(PRINT_GOLD"2. Change Click Buffer Rate Value(int): Currently "PRINT_GREEN"%d\n"PRINT_COLOR_RESET, CLICK_BUFFER_RATE);
    printf(PRINT_GOLD"3. About The Program \n");
    printf("4. Exit The Program\n"PRINT_COLOR_RESET);
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
        case 1:  // Toggle Click Buffer ON/OFF
            refreshMenu();
            printUserPrompt();
            TOGGLE_CLICK_BUFFER = !TOGGLE_CLICK_BUFFER;
            refreshMenu();
            printUserPrompt();
            break;
        case 2: // Change Click Buffer Rate
            refreshMenu();
            change_CLICK_BUFFER_RATE();
            refreshMenu();
            printUserPrompt();
            break;
        case 3:  // About The Program
            printAboutProgram();
            printf(PRINT_AQUA"Press Enter to continue..."PRINT_COLOR_RESET);
            while (getchar() != '\n');
            getchar();
            refreshMenu();
            printUserPrompt();
            break;
        case 4:
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
void leftClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}
void* clickEvent(void* arg)
{
    const DWORD HOLD_THRESHOLD = 100;
    const DWORD WAIT_THRESHOLD = 200;
    DWORD first_last_click_time;
    DWORD second_last_click_time;
    while(1)
    {
        int is_holding_key = 0;
        int out_of_clicking_window_time = 0;
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && TOGGLE_CLICK_BUFFER)
        {
            first_last_click_time = GetTickCount();
            while(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            {
                DWORD first_current_click_time = GetTickCount();
                if( first_current_click_time - first_last_click_time>HOLD_THRESHOLD)
                {
                    is_holding_key = 1;  // no break to consume leftover
                }
                Sleep(1);
            }
            if(!is_holding_key) //detect the first click
            {
                START_LOOP:
                DWORD start_timer = GetTickCount();
                while(1)
                {
                    DWORD current_timer = GetTickCount();
                    if(current_timer - start_timer >= WAIT_THRESHOLD)  //detect if the second click inside the WAIT_threshold to start click buffer
                    {
                        out_of_clicking_window_time = 1;
                        break;
                    }
                    if((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && TOGGLE_CLICK_BUFFER)
                    {
                        second_last_click_time = GetTickCount();
                        while((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && TOGGLE_CLICK_BUFFER)  //check if the second click is a click or a hold
                        {
                            DWORD second_current_click_time = GetTickCount();
                            if(second_current_click_time - second_last_click_time > HOLD_THRESHOLD)
                            {
                                is_holding_key = 1; // no break to consume leftover
                            }
                            Sleep(1);
                        }
                        break;
                    } 
                    
                }
                if(!is_holding_key && !out_of_clicking_window_time)
                {
                    for(int i = 0; i< CLICK_BUFFER_RATE;i++)
                    {
                        int first_random_sleep = (rand() % (20)) + 2;
                        int second_random_sleep = (rand() % (20)) + 2;
                        Sleep((second_last_click_time-first_last_click_time)/first_random_sleep);
                        leftClick();
                        Sleep((second_last_click_time-first_last_click_time)/second_random_sleep);
                        leftClick();
                        goto START_LOOP;
                    }
                }
            }
        }
    }
    return NULL;
}
int main() 
{
    pthread_t background_button_press_check;
    pthread_create(&background_button_press_check, NULL, clickEvent, NULL);
    pthread_detach(background_button_press_check);
    menuHandler();
    return 0;
}
