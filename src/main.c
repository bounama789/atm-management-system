#include "header.h"

void mainMenu(struct User* u)
{
    int op= NULL;

    system("clear");
    printf("userId: %d", u->id);
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");

    scanf("%[^\n]", &op);
    getchar();

    op -=48;



    switch (op)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccount(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        invalidOperation(mainMenu,u);
    }
};

void initMenu(struct User* u)
{

    int r = 0;
    static int opt = NULL;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    scanf("%[^\n]", &opt);
    getchar();
    
    opt = atoi(&opt);
    switch (opt)
    {
    case 1:
        loginMenu(u);
        printf("\ndone\n");
    if (userExist(u->name) == 0){
        printf("\nThis username not exist\n");
        invalidOperation(initMenu,u);
        return;
    } 
    else{
        char* b = getPassword(u);
        
        int n = strcmp(u->password, b);

        if (n != 0)
        {
            printf("\nWrong password!! or User Name\n");
            sleep(1);
            invalidOperation(initMenu,u);
        }
        else
        {
            printf("\n\nsigned-in sucessfuly\n");
        }
        }
        r = 1;
        break;
    case 2:
        registerMenu(u);
        r = 1;
        break;
    case 3:
        exit(1);
        break;
    default:
        opt = NULL;
        invalidOperation(initMenu,u);
    }
    
};

int main()
{
    struct User u;

    initMenu(&u);
    mainMenu(&u);

    return 0;
}
