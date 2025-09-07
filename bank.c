#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int accountNumber;
    char name[100];
    float balance;
};

void createAccount();
void deposit();
void withdraw();
void checkBalance();
void displayAccount();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;

    do {
        printf("\n====== Bank Management System ======\n");
        printf("1. Create New Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display Account Details\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: createAccount(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: checkBalance(); break;
            case 5: displayAccount(); break;
            case 6: printf("Exiting the program. Goodbye!\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while(choice != 6);
}

void createAccount() {
    struct Account acc;
    FILE *fp;

    printf("\nEnter Account Number: ");
    scanf("%d", &acc.accountNumber);
    printf("Enter Name: ");
    getchar(); // To consume newline left by previous scanf
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0; // Remove trailing newline
    acc.balance = 0.0;

    // Open file in append mode
    fp = fopen("accounts.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

void deposit() {
    int accNo;
    float amount;
    FILE *fp, *fpTemp;
    struct Account acc;
    int found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);

    fp = fopen("accounts.dat", "rb");
    fpTemp = fopen("temp.dat", "wb");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            acc.balance += amount;
            found = 1;
        }
        fwrite(&acc, sizeof(struct Account), 1, fpTemp);
    }

    fclose(fp);
    fclose(fpTemp);

    if (found) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        printf("Amount deposited successfully!\n");
    } else {
        remove("temp.dat");
        printf("Account not found!\n");
    }
}

void withdraw() {
    int accNo;
    float amount;
    FILE *fp, *fpTemp;
    struct Account acc;
    int found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);

    fp = fopen("accounts.dat", "rb");
    fpTemp = fopen("temp.dat", "wb");
    if (fp == NULL || fpTemp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            if (acc.balance >= amount) {
                acc.balance -= amount;
                found = 1;
            } else {
                printf("Insufficient balance!\n");
                fwrite(&acc, sizeof(struct Account), 1, fpTemp);
                found = -1; // Found but insufficient funds
                continue;
            }
        }
        fwrite(&acc, sizeof(struct Account), 1, fpTemp);
    }

    fclose(fp);
    fclose(fpTemp);

    if (found == 1) {
        remove("accounts.dat");
        rename("temp.dat", "accounts.dat");
        printf("Amount withdrawn successfully!\n");
    } else if (found == -1) {
        remove("temp.dat");
    } else {
        remove("temp.dat");
        printf("Account not found!\n");
    }
}

void checkBalance() {
    int accNo;
    FILE *fp;
    struct Account acc;
    int found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            printf("Account Holder: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}

void displayAccount() {
    int accNo;
    FILE *fp;
    struct Account acc;
    int found = 0;

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    fp = fopen("accounts.dat", "rb");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.accountNumber == accNo) {
            printf("\n=== Account Details ===\n");
            printf("Account Number: %d\n", acc.accountNumber);
            printf("Account Holder: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Account not found!\n");
    }
}
