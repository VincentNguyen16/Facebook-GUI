#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"

// Function to display the main menu, displayed at the start or after a user logs out.
// Program pauses here for user input.
void print_menu() {
    printf("******************************************************************\n");
    printf("                   Welcome to Text-Based Facebook\n");
    printf("******************************************************************\n\n\n");
    printf("******************************************************************\n");
    printf("                            Main Menu\n");
    printf("******************************************************************\n");
    printf("1. Register a new User.\n");
    printf("2. Login with an existing user's information.\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

// Function to display the logged-in menu for a user
void print_logged_in_menu(const char *username) {
    printf("***********************************************\n");
    printf("Welcome %s:\n", username);
    printf("***********************************************\n");
    printf("1. Manage profile (change password)\n");
    printf("2. Manage posts (add/remove)\n");
    printf("3. Manage friends (add/remove)\n");
    printf("4. Display a friend's posts\n");
    printf("5. Logout\n");
    printf("Enter your choice: ");
}

// Handles initializing the user database, the main menu loop, and the flow between states (logged-in vs. logged-out).
int main() {
    FILE *csv_file = fopen("user_details.csv", "r");
    if (!csv_file) {
        perror("Error opening the CSV file");
        return 1;
    }

    user_t *users = read_CSV_and_create_users(csv_file, 50);
    fclose(csv_file);

    user_t *logged_in_user = NULL; // Pointer to the currently logged-in user
    int input;                    // Variable to store user input for the menu

    // Main menu loop
    while (true) {
        if (!logged_in_user) {
            // User is not logged in. Show the main menu.
            print_menu();
            scanf("%d", &input);

            switch (input) {
                case 1: { // Register a new user
                    char username[30], password[15];
                    bool is_valid = false;

                    printf("Enter a username: ");
                    scanf("%s", username);

                    while (!is_valid) {
                        printf("Enter a password (8-15 characters): ");
                        scanf("%s", password);

                        if (strlen(password) < 8) {
                            printf("Error: Password must be at least 8 characters long.\n");
                        } else {
                            is_valid = true; // Valid password, exit the loop
                        }
                    }

                    // Add the user to the database
                    users = add_user(users, username, password);
                    printf("**** User Added! ****\n");
                    break; // Return to the main menu after registration
                }
                case 2: // Login with an existing user's information
                    logged_in_user = login(users); // Attempt to log in the user
                    // If login fails, logged_in_user remains NULL.
                    break;
                case 3: // Exit the application
                    printf("Goodbye!\n");
                    teardown(users); // Free all memory before exiting
                    return 0; // Exit the program
                default:
                    printf("Invalid input. Please try again.\n");
                    break;
            }
        } else {
            // Logged-in menu loop, show logged-in menu and handle inputs.
            while (logged_in_user) {
                print_logged_in_menu(logged_in_user->username); // Display the logged-in menu
                scanf("%d", &input);

                switch (input) {
                    case 1: { // Manage profile (change password)
                        char old_password[15], new_password[15];
                        printf("Enter the old password: ");
                        scanf("%s", old_password);
                        if (strcmp(logged_in_user->password, old_password) == 0) {
                            printf("Enter a new password (8-15 characters): ");
                            scanf("%s", new_password);
                            strcpy(logged_in_user->password, new_password); // Update the user's password
                            printf("**** Password Changed! ****\n");
                        } else {
                            printf("Incorrect password.\n");
                        }
                        break;
                    }
                    case 2: { // Manage posts (create or delete).
                        char post_content[250];
                        int post_input;
                        do {
                            // Provide options for managing posts.
                            printf("1. Add a post\n2. Remove the most recent post\n3. Return to previous menu\n");
                            printf("Enter your input: ");
                            scanf("%d", &post_input);

                            if (post_input == 1) { // Add a new post
                                printf("Enter the post content: ");
                                scanf(" %[^\n]s", post_content); // Read a line of input
                                add_post(logged_in_user, post_content); // Append to the user's posts.
                                printf("**** Post Added! ****\n");
                            } else if (post_input == 2) { // Remove the most recent post
                                if (delete_post(logged_in_user)) {
                                    printf("**** Post Removed! ****\n");
                                } else {
                                    printf("Error: No posts to remove.\n");
                                }
                            } else if (post_input != 3) { // Invalid input
                                printf("Invalid input. Please try again.\n");
                            }
                        } while (post_input != 3); // Continue until the user chooses to return
                        break;
                    }
                    case 3: { // Manage friends
                        char friend_name[30];
                        int friend_input;
                        do {
                            printf("1. Add a friend\n2. Remove a friend\n3. Return to previous menu\n");
                            printf("Enter your input: ");
                            scanf("%d", &friend_input);

                            if (friend_input == 1) { // Add a friend
                                printf("Enter the friend's name: ");
                                scanf("%s", friend_name);
                                add_friend(logged_in_user, friend_name, false); // Interactive call, silent = false
                            } else if (friend_input == 2) { // Remove a friend
                                printf("Enter the friend's name to remove: ");
                                scanf("%s", friend_name);
                                if (delete_friend(logged_in_user, friend_name)) {
                                    printf("**** Friend Removed! ****\n");
                                } else {
                                    printf("Error: Friend not found.\n");
                                }
                            } else if (friend_input != 3) { // Invalid input
                                printf("Invalid input. Please try again.\n");
                            }
                        } while (friend_input != 3); // Continue until the user chooses to return
                        break;
                    }
                    case 4: { // Display a friend's posts
                        char friend_name[30];
                        printf("Enter the friend's name to view their posts: ");
                        scanf("%s", friend_name);
                        display_friend_posts(logged_in_user, friend_name);
                        break;
                    }
                    case 5: // Logout 
                        printf("Logging out...\n");
                        logged_in_user = NULL; // Reset the logged-in user
                        break;
                    default:
                        printf("Invalid input. Please try again.\n");
                        break;
                }
            }
        }
    }

    return 0;
}
