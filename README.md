# Text-Based Facebook Simulation

This project is a **text-based social network simulation** implemented in C. It allows users to register, log in, manage friends, create posts, and view friends' posts. The application simulates core functionalities of a social network using linked lists.

## Features

- **User Registration and Login**: 
  - Users can register with a username and password.
  - Login validation ensures secure access.

- **Friend Management**:
  - Add or remove friends.
  - Friends are stored in a sorted linked list.

- **Post Management**:
  - Create and delete posts.
  - Posts are managed using a stack (Last-In-First-Out).

- **Display Features**:
  - View all posts of a user.
  - View a specific number of posts.
  - Display posts of a specific friend.

- **Data Management**:
  - Read user data from a CSV file.
  - Free all dynamically allocated memory before program exit.

## Installation

1. Clone the repository or download the source files.
2. Ensure you have a C compiler installed (e.g., GCC).
3. Create a `user_details.csv` file with the required user data.

## Compilation

Use the following command to compile the program:

```bash
gcc -o text_facebook main.c a2_nodes.c a2_functions.c -std=c11
```

## Usage

Run the program using:

```bash
./text_facebook
```

### Main Menu

1. **Register a New User**: Create a new user account.
2. **Login**: Access an existing account.
3. **Exit**: Quit the application.

### Logged-In Menu

1. **Manage Profile**: Change your password.
2. **Manage Posts**: Add or remove posts.
3. **Manage Friends**: Add or remove friends.
4. **View Friend's Posts**: See posts from a specific friend.
5. **Logout**: Log out of your account.

## File Structure

- `main.c`: Contains the main program logic and menu handling.
- `a2_nodes.h` and `a2_nodes.c`: Define and implement data structures (users, friends, posts).
- `a2_functions.h` and `a2_functions.c`: Core functions for user and post management.

## Data Structures

1. **User**:
   - Username and password.
   - Linked list of friends.
   - Stack of posts.
   - Pointer to the next user.

2. **Friend**:
   - Username.
   - Pointer to the friend's posts.
   - Pointer to the next friend.

3. **Post**:
   - Post content.
   - Pointer to the next post.

## Dependencies

- `stdio.h`
- `stdlib.h`
- `string.h`
- `stdbool.h`
- `assert.h`

## Memory Management

The program uses dynamic memory allocation for users, friends, and posts. All memory is released during the program teardown to prevent memory leaks.

## Example Input File (`user_details.csv`)

```csv
username,password,friend1,friend2,...
user1,password123,user2,user3
user2,password456,user1
```

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request for any improvements or bug fixes.

## License

This project is licensed under the MIT License.
