#include <termios.h>
#include "header.h"
#include <stdlib.h>
#include <string.h>


char *USERS = "./data/users.txt";

void loginMenu(struct User *u)
{
    struct termios oflags, nflags;
invalid:
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%[^\n]s", u->name);
    printf("\n---%s$\n",u->name);
    // clearStdinBuffer();
    getchar();
    if(checkUsername(u->name) != 1){
        printf("\ninvalid username\n");
        sleep(2);
        goto invalid;
    }
    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%[^\n]s", u->password);
    getchar();
    printf("\ndone log\n");
    userPassToMd5(u);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};


void registerMenu(struct User *u){
    FILE * uf = fopen(USERS,"a+");

    struct termios oflags, nflags;

    invalid:
    system("clear");
        printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Name:");

        scanf("%[^\n]s", u->name);
        getchar();

        if (checkUsername(u->name) != 1)
        {
            printf("\nInvalid username.Username must:\n\t-Not contain any space\n\t-Only contain alphabetic characters\n");
            sleep(3);
            goto invalid;
        }
        
        if (userExist(u->name) == 1)
        {
            printf("\n✖ User with this name already exists\n\n");
            sleep(1);
            goto invalid;
        }
        // disabling echo
        tcgetattr(fileno(stdin), &oflags);
        nflags = oflags;
        nflags.c_lflag &= ~ECHO;
        nflags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
        {
            perror("tcsetattr");
            return exit(1);
        }

    choosePass:
        printf("\n\n\n\n\n\t\t\t\tChoose a password:");
        scanf("%[^\n]s", u->password);
        getchar();
        if(strcmp(u->password,"\0")==0){
            printf("\nPassword should not be empty\n");
            sleep(1);
            goto choosePass;
        }

        printf("\n\n\n\n\n\t\t\t\tConfirm your password:");

        char tPass[50];
        scanf("%[^\n]s", tPass);
        getchar();

        if (strcmp(u->password,tPass) != 0)
        {
            printf("passwords doesn't match");
            goto choosePass;
        } else {
            u->id = getLastUserIndex(USERS)+1;
            userPassToMd5(u);
            saveUserToFile(uf,u);
            fclose(uf);
        }
        // restore terminal
        if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
        {
            perror("tcsetattr");
            return exit(1);
        }
};


const char *getPassword(struct User *u)
{
    FILE *fp = fopen("./data/users.txt", "r+");
    char * pass;
    struct User userChecker;


    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    while (getUserFromFile(fp,&userChecker))
    {
        if(strcmp(userChecker.name,u->name) ==0){
            fclose(fp);
            u->id = userChecker.id;
            pass = userChecker.password;
        }
    }
 
    return pass;
};

int getLastUserIndex(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return -1;
    }

    char line[100];
    char* second_last_line = NULL;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (second_last_line != NULL) {
            free(second_last_line);
        }
        second_last_line = strdup(line);
    }
    fclose(fp);
    char * n = strtok(second_last_line," ");

    int index = atoi(n);
    free(second_last_line);
    return index;
};

void saveUserToFile(FILE *ptr, struct User *u)
{
    fprintf(ptr, "%d %s %s\n",
	    u->id,
	    &u->name,
        &u->password);
}

int userExist(char * userName)
{
    FILE *ptr = fopen(USERS,"r+");
    char line[100];

    while(fgets(line, sizeof(line),ptr) != NULL) {
        char* token = strtok(line, " ");
   token = strtok(NULL, " ");

        if (token != NULL && strcmp(token, userName) == 0) {
            return 1;
        }
    }
    fclose(ptr);
    return 0;

}

int getUserFromFile(FILE* ptr, struct User* u)
{
    return fscanf(ptr, "%d %s %s",
        &u->id,
        u->name,
        u->password) != EOF;
}

void userPassToMd5( struct User *u) {
    EVP_MD_CTX* mdctx;
    const EVP_MD* md;
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len;

    mdctx = EVP_MD_CTX_new();
    md = EVP_md5();

    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, u->password, strlen(u->password));
    EVP_DigestFinal_ex(mdctx, digest, &digest_len);
    EVP_MD_CTX_free(mdctx);

    char* result = (char*)malloc(2 * digest_len + 1);
    if (result == NULL) {
        return NULL;
    }

    for (int i = 0; i < digest_len; i++) {
        sprintf(&result[i * 2], "%02x", digest[i]);
    }

    strcpy(u->password,result);
}

int md5Verify(char * str,char *md5){
    return strcmp(str,md5);
}
