#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

user_t *add_user(user_t *users, const char *username, const char *password) {
    // Create a new user and allocate memory for it
    user_t *new_user = malloc(sizeof(user_t));
    assert(new_user != NULL); // Ensure memory allocation was successful
    strcpy(new_user->username, username); // Set the username
    strcpy(new_user->password, password); // Set the password
    new_user->friends = NULL; // Initialize friends list as empty
    new_user->posts = NULL;   // Initialize posts as empty
    new_user->next = NULL;    // Initialize next pointer as NULL

    // If the list is empty or the new username comes before the head, insert at the front
    if (!users || strcmp(username, users->username) < 0) {
        new_user->next = users; // Link the new user to the rest of the list
        return new_user;        // Return the new head of the list
    }

    // Traverse the list to find the correct position for the new user
    user_t *current = users;
    while (current->next && strcmp(username, current->next->username) < 0) {
        current = current->next;
    }

    //Insert the new user into the list
    new_user->next = current->next;
    current->next = new_user;
    return users; // Return the head of the updated list
}

user_t *find_user(user_t *users, const char *username) {
    // Traverse the list of users
    while (users) {
        // Check if the current user's username matches the search term
        if (strcmp(users->username, username) == 0) {
            return users; // User was found, return the pointer
        }
        users = users->next; // Move to the next user
    }
    return NULL; // User not found, return NULL
}

friend_t *create_friend(const char *username) {
    // Allocate memory for the new friend
    friend_t *n_friend = malloc(sizeof(friend_t));
    assert(n_friend != NULL);
    strcpy(n_friend->username, username); // Set the friend's username using string copy
    n_friend->next = NULL; // Initialize next pointer as NULL
    return n_friend; // Return the new friend node
}

void add_friend(user_t *user, const char *friend_name, _Bool silent) {
    // Check if the friend exists in the database of users
    user_t *friend_user = find_user(user, friend_name);
    if (!friend_user) {
        // If the friend does not exist and silent mode is off, print an error message
        if (!silent) {
            printf("Error: Friend '%s' does not exist.\n", friend_name);
        }
        return; // Exit the function
    }

    // Create a new friend node
    friend_t *new_friend = malloc(sizeof(friend_t));
    assert(new_friend != NULL); // Ensure memory allocation was successful
    strcpy(new_friend->username, friend_name); // Set the friend's username
    new_friend->posts = &(friend_user->posts); // Link to the friend's posts
    new_friend->next = NULL; // Initialize the next pointer to NULL

    // Check if the friend's list is empty or if the new friend should be inserted at the front
    if (!user->friends || strcmp(friend_name, user->friends->username) < 0) {
        new_friend->next = user->friends; // Link the new friend to the current head of the list
        user->friends = new_friend; // Update the head of the list
        if (!silent) {
            printf("Friend '%s' added successfully.\n", friend_name); // Print a success message
        }
        return; // Exit the function
    }

    // Traverse the friends list to find the correct position for insertion in sorted order
    friend_t *current = user->friends;
    while (current->next && strcmp(friend_name, current->next->username) < 0) {
        current = current->next; // Move to the next friend in the list
    }

    // Insert the new friend into the list
    new_friend->next = current->next; // Point the new friend's next to the next node
    current->next = new_friend; // Update the current node to point to the new friend

    // Print a success message if silent mode is off
    if (!silent) {
        printf("Friend '%s' added successfully.\n", friend_name);
    }
}



_Bool delete_friend(user_t *user, char *friend_name) {
    friend_t *currentf = user->friends, *prev = NULL;

    //Traverse the friend list
    while (currentf) {
        // Check if the current friend's name matches
        if (strcmp(currentf->username, friend_name) == 0) {
            if (prev) {
                prev->next = currentf->next; // Skip the current friend
            } else {
                user->friends = currentf->next; // Update the head of the list
            }
            free(currentf); //Free the memory for the deleted friend
            return true;    // Friend successfully deleted
        }
        prev = currentf;        // Move the previous pointer
        currentf = currentf->next; // Move to the next friend
    }

    return false; // Friend not found, deletion unsuccessful
}

post_t *create_post(const char *text) {
    post_t *new_post = malloc(sizeof(post_t));
    assert(new_post != NULL); 
    strcpy(new_post->content, text); // Set the post content
    new_post->next = NULL; // Initialize next pointer as NULL
    return new_post; // Return the new post node
}

void add_post(user_t *user, const char *text) {
    //Create a new post node
    post_t *new_post = create_post(text);
    // Insert the post at the top of the stack
    new_post->next = user->posts;
    user->posts = new_post;
}

_Bool delete_post(user_t *user) {
    // Check if there are no posts to delete
    if (!user->posts) {
        return false; //Stack is empty, nothing to delete
    }

    // Remove the top post from the stack
    post_t *delete = user->posts;
    user->posts = user->posts->next;
    free(delete); // Free the memory for the deleted post
    return true;     // Post successfully deleted
}

void display_all_user_posts(user_t *user) {
    post_t *post = user->posts;
    int index = 1;

    // Check if there are no posts
    if (!post) {
        printf("Uh oh, no posts available for %s.\n", user->username);
        return;
    }

    // Iterate through the stack and display each post
    while (post) {
        printf("%d - %s\n", index++, post->content);
        post = post->next;
    }
}

void display_user_friends(user_t *user) {
    friend_t *cur_friend = user->friends;
    int index = 1;

    // Check if there are no friends
    if (!cur_friend) {
        printf("%s has no friends :(\n", user->username);
        return;
    }

    // Iterate through the friend list and display each friend's name
    while (cur_friend) {
        printf("%d - %s\n", index++, cur_friend->username);
        cur_friend = cur_friend->next;
    }
}

void display_posts_by_n(user_t *user, int n) {
    if (!user || n <= 0) {
        printf("Invalid user or number of posts to display, please try again.\n");
        return;
    }

    post_t *dis_post = user->posts;
    int total_index = 1;  // Keeps track of the global index across iterations

    // If the user has no posts, display a message
    if (!dis_post) {
        printf("%s has no posts.\n", user->username);
        return;
    }

    while (dis_post) {
        // Display up to 'n' posts in the current iteration
        int count = 0;
        for (; dis_post && count < n; count++) {
            printf("Post %d: %s\n", total_index++, dis_post->content);
            dis_post = dis_post->next;
        }

        // If there are no more posts, exit the loop
        if (!dis_post) {
            printf("No more posts to display.\n");
            break;
        }

        // Prompt the user to display more posts
        char input;
        printf("Do you want to display more posts? (y/n): ");
        scanf(" %c", &input);

        if (input == 'n' || input == 'N') {
            printf("Stopped displaying posts.\n");
            break;
        }
    }
}


void teardown(user_t *users) {
    //Iterate through the user list
    while (users) {
        user_t *to_delete = users;
        users = users->next;

        // Free all friends for the current user
        while (to_delete->friends) {
            friend_t *f_to_delete = to_delete->friends;
            to_delete->friends = to_delete->friends->next;
            free(f_to_delete);
        }

        // Free all posts for the current user
        while (to_delete->posts) {
            post_t *p_to_delete = to_delete->posts;
            to_delete->posts = to_delete->posts->next;
            free(p_to_delete);
        }

        free(to_delete); // Free the current user
    }
}

// Function to log in a user by validating credentials
user_t *login(user_t *users) {
    char username[30], password[15];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    user_t *user = find_user(users, username);
    //If user and inputted password are same as stored value, then login
    if (user && strcmp(user->password, password) == 0) {
        printf("Welcome %s! You are now logged in.\n", username);
        return user; // Return the logged-in user
    } else {
        printf("Invalid username or password. Please try again.\n");
        return NULL; // Login failed
    }
}

void display_friend_posts(user_t *user, const char *friend_name) {
    friend_t *friend = user->friends;

    //Traverse the friend list to find the specified friend
    while (friend && strcmp(friend->username, friend_name) != 0) {
        friend = friend->next;
    }

    // If no friends found, print message
    if (!friend) {
        printf("Error: Friend '%s' not found in your friend list.\n", friend_name);
        return;
    }

    //If friends have no post, print message
    if (!friend->posts || !*friend->posts) {
        printf("Friend '%s' has no posts.\n", friend_name);
        return;
    }

    //Display the friend's posts
    post_t *post = *friend->posts;
    int index = 1;
    while (post) {
        printf("%d - %s\n", index++, post->content);
        post = post->next;
    }
}



user_t *read_CSV_and_create_users(FILE *file, int num_users) {
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line

    for (int i = 0; i < num_users; i++) {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);

        char *username = token;
        user_t *current_user = find_user(users, username);

        token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0) {
            if (strcmp(token, " ") != 0) {
                add_friend(current_user, token, true); // Suppress output
            }
            token = strtok(NULL, ",");
        }
    }
    return users;
}
