#include <stdio.h>
#include <string.h>

void newAccount();
void depositMoney();
void withdrawMoney();
void accBalance();

typedef struct{
    char name[50];
    int accNum;
    float accBalance;
}userAccount;

int main() {
    while(1) {
        int choice;
        printf("\n\n\n-------- BANK MANAGEMENT SYSTEM --------");
        printf("\n1. Create Account");
        printf("\n2. Deposit Money");
        printf("\n3. Withdraw Money");
        printf("\n4. Check Balance");
        printf("\n5. Exit");
        printf("\nEnter what you would like to do: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                newAccount();
                break;

            case 2:
                depositMoney();
                break;

            case 3:
                withdrawMoney();
                break;

            case 4:
                accBalance();
                break;

            case 5:
                printf("Closing your account! Please visit us again.");
                return 0;
                break;

            default:
                printf("\nOption not available. Please try again.");
                break;
        }
    }
}

// DEFINING ALL THE FUNCTIONS USED IN THE PROGRAM 

void newAccount() {
    FILE *file = fopen("bankAccount.dat", "ab");
    if(file == NULL) {
        printf("\nFile does not exist or cannot be opened.");
        return;
    }

    userAccount acc;
    char c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);

    printf("Enter the name of the account holder: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    int ind = strcspn(acc.name, "\n");
    acc.name[ind] = '\0';

    printf("Enter the account number of the account holder: ");
    scanf("%d", &acc.accNum);

    acc.accBalance = 0;

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);

    printf("Your account with name - %s and account number - %d has been created successfully.", acc.name, acc.accNum);
}

void depositMoney() {
    FILE *file = fopen("bankAccount.dat", "rb+");
    if(file == NULL) {
        printf("\nFile does not exist or cannot be opened.");
        return;
    }

    int accNum;
    float money;
    userAccount acc;

    printf("Enter the account number you wish to deposit money into: ");
    scanf("%d", &accNum);

    printf("Enter the amount of money you wish to deposit: ");
    scanf("%f", &money);

    while(fread(&acc, sizeof(acc), 1, file)) {
        if(acc.accNum == accNum) {
            acc.accBalance += money;
            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);
            printf("Your account has been credited with Rs.%.2f. Your new balance is %.2f", money, acc.accBalance);
            return;
        }
    }
    fclose(file);
    printf("Account number %d was not found hence your money was not deposited.", accNum);
}

void withdrawMoney() {
    FILE *file = fopen("bankAccount.dat", "rb+");
    if(file == NULL) {
        printf("\nFile does not exist or cannot be opened.");
        return;
    }

    int accNum;
    float money;
    userAccount acc;

    printf("Enter the account number you wish to withdraw money from: ");
    scanf("%d", &accNum);

    printf("Enter the amount of money you wish to withdraw: ");
    scanf("%f", &money);

    while(fread(&acc, sizeof(acc), 1, file)) {
        if(acc.accNum == accNum) {
            if(acc.accBalance >= money) {
                acc.accBalance -= money;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fclose(file);
                printf("Your account has been debited with Rs.%.2f. Your new balance is %.2f", money, acc.accBalance);
                return;
            } else {
                fclose(file);
                printf("Insufficient balance in your account.");
                return;
            }
        }
    }
    fclose(file);
    printf("Account number %d was not found hence your money was not withdrawn.", accNum);
}

void accBalance() {
    FILE *file = fopen("bankAccount.dat", "rb");
    if(file == NULL) {
        printf("\nFile does not exist or cannot be opened.");
        return;
    }

    int accNum;
    userAccount acc;

    printf("Enter the account number you wish to check balance for: ");
    scanf("%d", &accNum);

    while(fread(&acc, sizeof(acc), 1, file)) {
        if(acc.accNum == accNum) {
            fclose(file);
            printf("The current balance for account number %d is Rs.%.2f", acc.accNum, acc.accBalance);
            return;
        }
    }
    fclose(file);
    printf("Account number %d was not found.", accNum);
}