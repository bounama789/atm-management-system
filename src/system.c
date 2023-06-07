#include "header.h"
#include "ctype.h"

const char* RECORDS = "./data/records.txt";

int getAccountFromFile(FILE* ptr, struct Record* r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
        &r->id,
        &r->userId,
        r->name,
        &r->accountNbr,
        &r->deposit.month,
        &r->deposit.day,
        &r->deposit.year,
        r->country,
        &r->phone,
        &r->amount,
        r->accountType) != EOF;
}

void saveAccountToFile(FILE* ptr, struct User u, struct Record r)
{
    char* line = malloc(sizeof(char) * 1000);
    sprintf(line, "\n%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\0",
        getLastRecordIndex(RECORDS) + 1,
        u.id,
        u.name,
        r.accountNbr,
        r.deposit.month,
        r.deposit.day,
        r.deposit.year,
        r.country,
        r.phone,
        r.amount,
        r.accountType);
    fprintf(ptr, line);
}

void stayOrReturn(int notGood, void f(struct User* u), struct User* u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User* u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void fail(struct User* u)
{
    int option;
    int r = 0;
    printf("✖ Failed!\n\n");
    printf("Enter 0 to retry or 1 to go to the main menu and 2 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        updateAccount(u);
    }
    else if (option == 2)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
    }
}

void createNewAcc(struct User* u)
{
    struct Record r;
    struct Record cr;
    FILE* pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    while (getAccountFromFile(pf, &cr))
    {
        if (strcmp(r.name, u->name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);

    saveAccountToFile(pf, *u, r);
    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User* u)
{
    struct Record r;

    FILE* pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u->name);
    while (getAccountFromFile(pf, &r))
    {
        if (strcmp(r.name, u->name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                r.accountNbr,
                r.deposit.day,
                r.deposit.month,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

int getLastRecordIndex(char* filename) {
    int n = 0;
    FILE* fp = fopen(filename, "r+");
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return -1;
    }

    char line[1000];
    char* second_last_line = NULL;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (second_last_line != NULL) {
            free(second_last_line);
        }
        char* token = strtok(line, " ");

        if (atoi(token) != 0)
        {
            n = atoi(token);
        }
    }
    fclose(fp);
    free(second_last_line);
    return n;
};

void updateAccount(struct User* u) {
    int id, option, notFound = 0;
    struct Record r;
    FILE* pf = fopen(RECORDS, "r+");
    printf("\nEnter the account id you want to update:");
    scanf("%d", &id);
    int i = 0;
    char* updatedInfo = malloc(sizeof(char) * 1000);

    while (getAccountFromFile(pf, &r))
    {
        i += 2;
        if (r.accountNbr == id)
        {
            if (r.userId != u->id)
            {
                printf("This accounct doesn't belong to you\n");
                fail(u);
            }
            int line_pos = ftell(pf);
            notFound = 1;
            printf("\nChoose the field you want to update:\n\t-> 1. Country\n\t-> 2. Phone number\n\n\tEnter your choice:");
            scanf("%d", &option);
            getchar();

            switch (option) {
            case 1:
                printf("\nEnter the new country:");
                scanf("%[^\n]s", r.country);
                getchar();
                break;
            case 2:
                printf("\nEnter the new phone number:");
                scanf("%d", &r.phone);
                break;
            default:
                printf("\nInvalid option!");
                stayOrReturn(1, updateAccount, u);
                return;
                break;

            }
            sprintf(updatedInfo, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\0",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);
            fclose(pf);
            updateLineInFile(RECORDS, i, updatedInfo);
            success(u);

        }
    }
}

void updateLineInFile(char* filename, int lineNumber, char* newLine) {
    FILE* fp = fopen(filename, "r+");
    if (fp == NULL) {
        printf("Failed to open file %s", filename);
        return;
    }

    char line[1000];
    int currentLineNumber = 0;
    int remainingSpace;
    while (fgets(line, sizeof(line), fp) != NULL) {
        currentLineNumber++;
        int line_pos;
        //strncpy(line,newLine,sizeof(line));
        //fprintf(fp, "%s", line);
        if (lineNumber - currentLineNumber == 1) {

            line_pos = ftell(fp);
        }
        if (lineNumber == currentLineNumber) {
            char empty_line[(sizeof line)];
            int line_lenght = strlen(line);


            memset(line, ' ', line_lenght);
            remainingSpace = line_lenght - strlen(newLine);
            fseek(fp, line_pos, SEEK_SET);
            fwrite(line, 1, line_lenght, fp);
            fseek(fp, line_pos, SEEK_SET);

            // fprintf(fp,"%s",empty_line);
            fputs(newLine, fp);
            line_pos = ftell(fp);

          

        }
  if (currentLineNumber == lineNumber+1 && remainingSpace>0){
                printf("\n%d\n",remainingSpace);
                sleep(2);
                fseek(fp,remainingSpace,SEEK_CUR);
                for(int i=0; i< remainingSpace;i++){
                    fprintf(fp,"\b");
                }
                // memset(line, 'a', remainingSpace);
                // fwrite(line, 1, line_lenght, fp);

                //fputs(line, fp);
            }


    }
    fclose(fp);
}

void invalidOperation(void (*menu)(struct User*), struct User* u)
{
    printf("\nInvalid operation! Please try again.\n");
    sleep(1);
    menu(u);
}

void checkAccountDetails(struct User* u)
{
    int id, notFound = 0;
    struct Record r;
    FILE* pf = fopen(RECORDS, "r");
    printf("\nEnter the account id you want to see:");
    scanf("%d", &id);
    while (getAccountFromFile(pf, &r))
    {


        if (r.accountNbr == id)
        {
            if (r.userId != u->id)
            {
                printf("This account doesn't belong to you\n");
                fail(u);
            }

            notFound = 1;
            system("clear");
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                r.accountNbr,
                r.deposit.day,
                r.deposit.month,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);

            double interest;

            if (strcmp(r.accountType, "saving") == 0)
            {
                printf("Interest rate: 0.07%%\n");
                interest = (r.amount * 0.07) * .083;
            }
            else if (strcmp(r.accountType, "fixed01") == 0)
            {
                printf("Interest rate: 0.04%%\n");
                interest = ((r.amount * 0.04) / 100) * 12;
            }
            else if (strcmp(r.accountType, "fixed02") == 0)
            {
                printf("Interest rate: 0.05%%\n");
                interest = ((r.amount * 0.05) / 100) * 24;
            }
            else if (strcmp(r.accountType, "fixed03") == 0)
            {
                printf("Interest rate: 0.08%%\n");
                interest = ((r.amount * 0.08) / 100) * 36;
            }
            else if (strcmp(r.accountType, "current") == 0)
            {
                interest = 0;
            }
            if (interest > 0)
            {
                printf("Calculated interest: $%.2f\n", interest);
            }
            else
            {
                printf("You will not get interests because the account is of type current");
            }
        }
    }
    fclose(pf);
    if (!notFound)
    {
        printf("\nAccount not found!\n");
        stayOrReturn(1, checkAccountDetails, u);
    }
    else
    {
        success(u);
    }
}

void makeTransaction(struct User* u)
{
    int id, notFound = 0, option;
    double amount;
    struct Record r;
    FILE* pf = fopen(RECORDS, "r+");
    printf("\nEnter the account id you want to make a transaction with:");
    scanf("%d", &id);
    int n = 0;
    while (getAccountFromFile(pf, &r))
    {
        n += 2;
        if (r.accountNbr == id)
        {
            if (r.userId != u->id)
            {
                printf("This account doesn't belong to you\n");
                fail(u);
            }
            notFound = 1;
            int opt;
            char* trans;
            printf("\nSelect Transaction\n");
            printf("\n[1]- Deposit\n");
            printf("\n[2]- Withdraw\n");
            printf("\n[3]- Go back\n");

            scanf("%d", &opt);
            getchar();
            switch (opt)
            {
            case 1:
                trans = "deposit";
                break;
            case 2:
                trans = "withdraw";
                break;
            case 3:
                mainMenu(u);
                break;
            default:
                invalidOperation(makeTransaction, u);
                break;
            }
            printf("\nEnter the amount you want to %s: $", trans);
            scanf("%lf", &amount);
            if (opt == 2 && amount > r.amount)
            {
                printf("\nInsufficient balance!\n");
                stayOrReturn(1, makeTransaction, u);
            }
            else
            {
                if (opt == 1)
                {
                    r.amount += amount;
                }
                else
                {
                    r.amount -= amount;
                }
                char* updatedInfo = malloc(sizeof(char) * 1000);
                sprintf(updatedInfo, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                    r.id,
                    r.userId,
                    r.name,
                    r.accountNbr,
                    r.deposit.month,
                    r.deposit.day,
                    r.deposit.year,
                    r.country,
                    r.phone,
                    r.amount,
                    r.accountType);

                updateLineInFile(RECORDS, n, updatedInfo);
                fclose(pf);
                success(u);
            }
        }
    }
    fclose(pf);
    if (!notFound)
    {
        printf("\nAccount not found!\n");
        stayOrReturn(1, makeTransaction, u);
    }
    else
    {
        printf("\nChoose the operation you want to perform:\n\t-> 1. Deposit\n\t-> 2. Withdraw\n\n\tEnter your choice:");
        scanf("%d", &option);
        if (option != 1 && option != 2)
        {
            invalidOperation(makeTransaction, u);
        }
    }
}

void removeAccount(struct User* u)
{
    int id, notFound = 0;
    struct Record r;
    FILE* pf = fopen(RECORDS, "r+");
    printf("\nEnter the account id you want to remove:");
    scanf("%d", &id);
    int n = 0;
    while (getAccountFromFile(pf, &r))
    {
        n += 2;
        if (r.accountNbr == id)
        {
            if (r.userId != u->id)
            {
                printf("This account doesn't belong to you\n");
                fail(u);

            }
            notFound = 1;
            char* updatedInfo = malloc(1000);
            sprintf(updatedInfo, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
                r.id,
                r.userId,
                r.name,
                r.accountNbr,
                r.deposit.month,
                r.deposit.day,
                r.deposit.year,
                r.country,
                r.phone,
                r.amount,
                r.accountType);

            updateLineInFile(RECORDS, n, updatedInfo);
            fclose(pf);
            success(u);
        }
    }
    fclose(pf);
    if (!notFound)
    {
        printf("\nAccount not found!\n");
        stayOrReturn(1, removeAccount, u);
    }
    else
    {
        printf("\nAccount removed successfully!\n");
    }
}

void transferOwnership(struct User* u)
{
    FILE* pf = fopen(RECORDS, "r+");
    FILE* ptr = fopen("./data/users.txt", "r+");

    struct Record r;
    struct User user;
    int acctNumber;
    int n = 0;
    char uname[50];
    char* upInfo = malloc(1000);



    printf("Enter the account number you want to give away:");
    scanf("%d", &acctNumber);

    while (getAccountFromFile(pf, &r))
    {
        n += 2;
        if (u->id == r.userId)
        {
            if (acctNumber == r.accountNbr)
            {
                printf("Enter the user's name you want to give this account: ");
                scanf("%s", uname);


                if (userExist(uname) == 1)
                {
                    while (getUserFromFile(ptr, &user))
                    {
                        if (user.name == uname)
                        {
                            sprintf(upInfo, "%d %d %s %d %d/%d/%d %s %d %lf %s\n",
                                r.id,
                                user.id,
                                user.name,
                                r.accountNbr,
                                r.deposit.year,
                                r.deposit.month,
                                r.deposit.day,
                                r.country,
                                r.phone,
                                r.amount,
                                r.accountType
                            );
                        }
                    }

                    fclose(ptr);

                }
                break;
            }
            printf("Do you really want to transfer ownership of account n°%d to %s\nPress y/yes to confirm or something else to cancel", r.accountNbr, uname);
            int option;
            scanf("%d", &option);

            if (option == 1)
            {
                printf("%d", n);
                updateLineInFile(RECORDS, n, upInfo);
                fclose(pf);
            }
            else
            {
                printf("Cancelled");
                sleep(1);
                mainMenu(u);
            }
        }
        else
        {
            printf("There is no user with this name: %s", uname);
        }
    }
}

void clearStdinBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int checkUsername(char* username) {
    return strcmp(username, "\0") != 0
        && strchr(username, ' ') == NULL
        && atoi(username) == 0
        && isStringAlpha(username) == 1;
}

int isStringAlpha(char* str) {
    while (*str) {
        if (!isalpha(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}
