#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pthread.h>

#define PRINT_RED "\x1b[31m" 
#define PRINT_GREEN "\x1b[32m"
#define PRINT_AQUA "\x1b[36m"
#define PRINT_GOLD "\033[1;33m"
#define PRINT_LIGHT_AQUA "\033[1;33m""\x1b[36m"
#define PRINT_COLOR_RESET "\x1b[0m" 

int TOGGLE_CLICK_BUFFER = 1;
float CLICK_BUFFER_RATE = 1;
int CLICK_PER_SECOND = 0;

void getKeybind();
void printMenu();
void changeCPS();
void refreshMenu();

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
void clearBuffer()
{
    int ch;
    while(ch = getchar() != '\n') {}
}
void refreshMenu()
{
    system("cls");
    printMenu();
}
void printMenu()
{
    char line[50] = "=======================================";
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
    printf(PRINT_GOLD"          Click Buffer        \n");
    printf("1. Toggle Click Buffer. Currently %s%s%s\n",
        (TOGGLE_CLICK_BUFFER == 1) ? PRINT_GREEN : PRINT_RED,
        (TOGGLE_CLICK_BUFFER == 1) ? "ON" : "OFF",
        PRINT_COLOR_RESET
    );
    printf(PRINT_GOLD"2. Change Click Buffer Rate Value: Currently %g\n", CLICK_BUFFER_RATE);
    printf("3. About The Program \n");
    printf("4. Exit The Program\n"PRINT_COLOR_RESET);
    printf(PRINT_AQUA"%s\n"PRINT_COLOR_RESET, line);
}
int getUserCPS()
{

}
void printUserPrompt()
{
    printf(PRINT_LIGHT_AQUA"\nEnter Your Choice: "PRINT_COLOR_RESET);
}
void leftClick()
{
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
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
        case 1:  // change CPS
            refreshMenu();
            printUserPrompt();
            TOGGLE_CLICK_BUFFER = !TOGGLE_CLICK_BUFFER;
            refreshMenu();
            printUserPrompt();
            break;
        case 2: // KeyBind
            refreshMenu();
            printf("This Feature Will Be Implemented Soon :3\n");
            printf(PRINT_AQUA"Press Enter to continue..."PRINT_COLOR_RESET);
            while (getchar() != '\n');
            getchar();
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
void changeCPS()
{
    refreshMenu();
    printf(PRINT_LIGHT_AQUA"\nChange CPS: "PRINT_COLOR_RESET);
    scanf("%d", &CLICK_PER_SECOND);
    return;
}
void* clickEvent(void* arg)
{
    const DWORD HOLD_THRESHOLD = 200;
    DWORD current_click_time;
    DWORD last_click_time;
    while(1)
    {
        int is_holding_key = 0;
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && TOGGLE_CLICK_BUFFER)
        {
            last_click_time = GetTickCount();
            while(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
            {
                current_click_time = GetTickCount();
                if(current_click_time-last_click_time>HOLD_THRESHOLD)
                {
                    is_holding_key = 1;
                }
                Sleep(1);
            }
            if(!is_holding_key)
            {
                leftClick();
                Sleep(10);
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
