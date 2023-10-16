#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <quickmail.h>
#include "cJSON.h"

typedef struct UsersList UsersList;
typedef struct UsersListNode UsersListNode;
typedef struct Admin Admin;
typedef struct DatabaseList DatabaseList;
typedef struct DatabaseNode DatabaseNode;
typedef struct AllowedUsersList AllowedUsersList;
typedef struct AllowedUsersNode AllowedUsersNode;
typedef struct TableList TableList;
typedef struct TableNode TableNode;
typedef struct Attribute Attribute;
typedef struct Tuple Tuple;

#define MAX_STRING_SIZE 100

// User Structs -------------------------------------------
struct Admin {
    char* username;
    char* password;
};
struct UsersListNode {
    char full_name[MAX_STRING_SIZE];
    char position[MAX_STRING_SIZE];
    char phone_number[MAX_STRING_SIZE];
    char email[MAX_STRING_SIZE];
    char password[MAX_STRING_SIZE];
    UsersListNode *next;
};
struct UsersList {
    UsersListNode *head;
};
struct AllowedUsersNode {
    char username[MAX_STRING_SIZE];
    AllowedUsersNode *next;
};
struct AllowedUsersList {
    AllowedUsersNode *head;
};
// Database Structs ---------------------------------------



enum DataType {
    INT,
    FLOAT,
    STRING,
    BOOL
};

struct Attribute {
    char name[MAX_STRING_SIZE];
    enum DataType type;
};

struct Tuple {
    char values[5][MAX_STRING_SIZE];
    Tuple *next;
};

struct TableNode {
    int table_id;
    char table_name[MAX_STRING_SIZE];
    Attribute Attributes[5];
    int num_Attributes;


    TableNode *left;
    TableNode *right;
};

struct DatabaseNode {
    int database_id;
    char database_name[MAX_STRING_SIZE];
    AllowedUsersList *allowed_users;
    TableNode *table_tree;
    DatabaseNode *next;
};

struct DatabaseList {
    DatabaseNode *head;
} ;


char* input(char* prompt) {
    char* buffer = (char*) malloc(sizeof(char) * MAX_STRING_SIZE); // Allocate new memory for the input buffer
    printf("%s", prompt);
    fgets(buffer, MAX_STRING_SIZE, stdin); // Read input from the user
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character from the input
    return buffer; // Return the input as a string
}

void send_email(char* email, char* password) {
    // Create the email body
    char body[1000];
    sprintf(body, "Your account has been created successfully.\nYour username is: %s\nYour password is: %s\n", email, password);

    // Initialize the quickmail library
    quickmail_initialize();

    // Create the email message
    quickmail* mail = quickmail_create("fgranados3502@gmail.com", "Database User Creation");
    quickmail_add_to(mail, email);
    quickmail_set_subject(mail, "User created successfully");
    quickmail_set_body(mail, body);
    quickmail_set_hostname(mail, "smtp.gmail.com");

    // Send the email
    if (quickmail_send(mail, "smtp.gmail.com", 587, "fgranados3502@gmail.com", "cdkfkaettfccqglf") == NULL) {
        printf("Email sent successfully.\n");
    } else {
        printf("Unable to send email.\n");
    }

    // Cleanup
    quickmail_destroy(mail);
    quickmail_cleanup();
}

void create_database_list() {
    DatabaseList *list = malloc(sizeof(DatabaseList));
    list->head = NULL;
};

void insert_database(DatabaseList *database_list, char* database_name, char** allowed_users, int num_users) {
    DatabaseNode *node = malloc(sizeof(DatabaseNode));

    strcpy(node->database_name, database_name);

    //need to finish

    node->next = database_list->head;
    database_list->head = node;
}


// User List Management Functions ------------------------------------------------------------------- ------------------------------------------------------------------------------------------------------------------------------------------------
UsersList *create_linked_list() {
    UsersList *list = malloc(sizeof(UsersList));
    list->head = NULL;
    return list;
}
void insert_users_list_node(UsersList *list, char* email, char* full_name, char* position, char* phone_number,  char* password) {
    UsersListNode *node = malloc(sizeof(UsersListNode));
    strcpy(node->full_name, full_name);
    strcpy(node->position, position);
    strcpy(node->phone_number, phone_number);
    strcpy(node->email, email);
    strcpy(node->password, password);
    node->next = list->head;
    list->head = node;
}
void delete_users_list_node(UsersList *list, char* username) {
    //Checks if the username is admin and prints error and returns if it is
    if (strcmp(username, "admin") == 0) {
        printf("Cannot delete admin account\n");
        return;
    }
    UsersListNode *node = list->head;
    UsersListNode *prev = NULL;
    while (node != NULL) {
        if (strcmp(node->email, username) == 0) {
            if (prev == NULL) {
                list->head = node->next;
            } else {
                prev->next = node->next;
            }
            free(node);
            break;
        }
        prev = node;
        node = node->next;
    }
}
void print_users_list_nodes(UsersList *list) {
    UsersListNode *node = list->head;
    if (list->head == NULL) {
        printf("No users registered\n");
        return;
    }
    int counter = 1;
    printf("Registered Users:\n");
    while (node != NULL) {
        printf("%d. %s\n", counter,node->email);
        counter++;
        node = node->next;
    }
}
// Requirement verifications ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int verify_email_requirements(UsersList *list, char* email) {



    quickmail_initialize();

    quickmail* mail = quickmail_create(email, "Test email");

    if (mail == NULL) {
        quickmail_cleanup();
        return 0;
    } else {
        quickmail_destroy(mail);
        quickmail_cleanup();
        return 1;
    }
}
int verify_password_requirements(char* password) {
    int length = strlen(password);
    if (length != 9) {
        return 0;
    }

    char* numbers[10] = {"0","1","2","3","4","5","6","7","8","9"};
    char* unique_numbers[9];

    int has_lowercase = 0; //Must have no lowercase letters
    int has_special = 0; //Must have at least 1 special character
    int unique_number_count = 0; //Must have at least 3 unique numbers from 0 to 9
    int has_vowel = 0; //Must not have any vowels
    int uppercase_count = 0; //Must have at least 2 uppercase letters
    for (int i = 0; i < length; i++) {
        char c = password[i];
        if (c >= 'a' && c <= 'z') {
            has_lowercase = 1;
        }else if (c >= 'A' && c <= 'Z') {
            uppercase_count++;
        }
        else if (c >= '0' && c <= '9'){
            for (int j = 0; j < 10; j++) {
                if (c == *numbers[j]) {
                    int unique = 1;
                    for (int k = 0; k < unique_number_count; k++) {
                        if (c == *unique_numbers[k]) {
                            unique = 0;
                            break;
                        }
                    }
                    if (unique == 1) {
                        unique_numbers[unique_number_count] = numbers[j];
                        unique_number_count++;
                    }
                }
            }
        }
        else {
            has_special = 1;
        }
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            has_vowel = 1;
        }
    }
    if (has_lowercase == 1 || has_special == 0 || unique_number_count < 3 || has_vowel == 1 || uppercase_count < 2) {
        return 0;
    }
    return 1;
}
// User Management Functions -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void add_user(UsersList *list) {
    char* full_name;
    char* position;
    char* phone_number;
    char* email;
    char* password;

    full_name = input("Full name: ");
    position = input("Position: ");
    phone_number = input("Phone number: ");

    while(1) {
        email = input("Email: ");

        if (verify_email_requirements(list ,email) == 0) {
            printf("Invalid email address\n");
        } else {
            break;
        }
    }
    while(1) {
        password = input("Password: ");

        if (verify_password_requirements(password) == 0) {
            printf("Password does not meet requirements\n");
            printf("Your password must meet the following criteria: be 9 characters long, include numbers, letters,\n");
            printf("and symbols; contain 3 non-repeated numbers; exclude vowels; and include 2 uppercase letters\n");
        } else {
            break;
        }
    }
    insert_users_list_node(list, email, full_name, position, phone_number, password);
    //send_email(email, password);

    free(full_name);
    free(position);
    free(phone_number);
    free(email);
    free(password);
};
void delete_user(UsersList *list, Admin admin) {
    char* username_input;
    char* password_input;
    char* admin_password = admin.password;
    while(1) {
        username_input = input("Enter the username to delete: ");
        if (strcmp(username_input, "admin") == 0) {
            printf("Cannot delete admin account, try again or enter Cancel to cancel the deletion.\n");
        }else if (strcmp(username_input, "Cancel") == 0){
            printf("Deletion cancelled\n");
            return;
        }else {
            UsersListNode *node = list->head;
            while(node != NULL) {
                if (strcmp(node->email, username_input) == 0) {
                    break;
                }
                node = node->next;
            }
            if (node == NULL) {
                printf("Username not found, try again or enter Cancel to cancel the deletion.\n");
            } else {
                break;
            }
        }
    }
    while(1) {
        password_input = input("Enter admin password to confirm deletion: ");
        if (strcmp(password_input, "Cancel") == 0) {
            printf("Deletion cancelled\n");
            return;
        } else if (strcmp(password_input, admin_password) != 0) {
            printf("Incorrect password, try again or enter Cancel to cancel the deletion.\n");
        } else {
            break;
        }
    }
    delete_users_list_node(list, username_input);
    printf("User deleted successfully\n");
}
// Database Management Functions -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void show_databases(DatabaseList *database_list, AllowedUsersList *allowed_users_list){
    DatabaseNode *database_node = database_list->head;
    if (database_list->head == NULL) {
        printf("No databases created\n");
        return;
    }
    int counter = 1;
    printf("Databases:\n");
    while (database_node != NULL) {
        printf("%d. %s\n", counter,database_node->database_name);
        counter++;
        database_node = database_node->next;
    }
}

// Login ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* login(UsersList *list, Admin admin) {
    char* username;
    char* password;

    printf("\nLogin Menu\n\n");

    username = input("Username: ");
    password = input("Password: ");

    if (strcmp(username, admin.username) == 0 && strcmp(password, admin.password) == 0) {
        return "admin";
    }
    UsersListNode *node = list->head;
    while (node != NULL) {
        if (strcmp(node->email, username) == 0 && strcmp(node->password, password) == 0) {
            return node->email;
        }
        node = node->next;
    }
    return NULL;
}
//Menus -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void show_admin_menu() {
    printf("\nAdmin Menu:\n");
    printf("1. Add user\n");
    printf("2. Delete user\n");
    printf("3. Show users\n");
    printf("4. Show databases\n");
    printf("5. Log out\n");
    printf("6. Exit\n");
}
void show_user_menu() {
    printf("\nUser Menu:\n");
    printf("1. option1\n");
    printf("2. option2\n");
    printf("3. option3\n");
    printf("4. Log out\n");
    printf("5. Exit\n");
}
// Main ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main() {
    Admin admin;
    admin.username = "admin";
    admin.password = "admin";

    DatabaseList *database_list = malloc(sizeof(DatabaseList));
    database_list->head = NULL;
    AllowedUsersList *allowed_users_list = malloc(sizeof(AllowedUsersList));
    allowed_users_list->head = NULL;

    int exit = 0;
    UsersList *list = create_linked_list();
    do { // do while loop for login
        int log_out = 0;
        char* logged_user = login(list, admin);
        if (logged_user == NULL) {
            printf("Invalid username or password\n");
            continue;
        }
        else if (strcmp(logged_user, "admin") == 0) {
            printf("Welcome admin!\n");
            do {
                show_admin_menu();
                char* choice_input = input("Enter your choice: ");
                int choice;
                choice = strtol(choice_input, NULL, 10);
                switch (choice) {
                    case 1: {
                        printf("Add new user...\n");
                        add_user(list);
                        break;
                    } // Add new user
                    case 2: {
                        printf("Delete user...\n");
                        delete_user(list, admin);
                        break; } // Delete user
                    case 3: {
                        printf("Show users...\n");
                        print_users_list_nodes(list);
                        break; } // Show users
                    case 4: {
                        printf("Show databases...\n");
                        show_databases(database_list, allowed_users_list);
                        break; // Show databases
                    }
                    case 5: {
                        printf("Logging out...\n");
                        log_out = 1;
                        break; } // Log out
                    case 6: {
                        printf("Exiting...\n");
                        log_out = 1;
                        exit = 1;
                        break; } // Exit
                    default:
                        printf("Invalid choice\n"); // Invalid choice
                    break;
                }
            } while (log_out == 0);
        }
        else {
            printf("Welcome %s!\n",logged_user);
            do {
                show_user_menu();
                char* choice_input = input("Enter your choice: ");
                int choice;
                choice = strtol(choice_input, NULL, 10);
                switch (choice) {
                    case 1: {
                        printf("option1...\n");
                        break;
                    } // option1
                    case 2: {
                        printf("option2...\n");
                        break;
                    } // option2
                    case 3: {
                        printf("option3...\n");
                        break; } // option3
                    case 4: {
                        printf("Logging out...\n");
                        log_out = 1;
                        break; } // Log out
                    case 5: {
                        printf("Exiting...\n");
                        log_out = 1;
                        exit = 1;
                        break; } // Exit
                    default:
                        printf("Invalid choice\n"); // Invalid choice
                        break;
                }
            } while (log_out == 0);
        }
    } while (exit == 0);
    return 0;
}


