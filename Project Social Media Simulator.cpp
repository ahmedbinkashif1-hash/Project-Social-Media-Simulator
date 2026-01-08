#include <iostream>
#include <fstream>
#include <string>
using namespace std;
/* ================== GLOBAL DATA ================== */
const int MAX_POSTS = 10;
string posts[MAX_POSTS];
int likes[MAX_POSTS];
int postCount = 0;
/* ================== LOAD POSTS ================== */
void loadPosts()
{
    ifstream file("posts.txt");
    if (!file)
        return;
    file >> postCount;
    file.ignore();
    for (int i = 0; i < postCount; i++)
    {
        getline(file, posts[i]);
        file >> likes[i];
        file.ignore();
    }

    file.close();
}

/* ================== SAVE POSTS ================== */
void savePosts()
{
    ofstream file("posts.txt");
    file << postCount << endl;

    for (int i = 0; i < postCount; i++)
    {
        file << posts[i] << endl;
        file << likes[i] << endl;
    }

    file.close();
}
/* ================== SIGNUP ================== */
void signup()
{
    string user, pass;
    cout << "\n--- SIGN UP ---\n";
    cout << "Enter username: ";
    cin >> user;
    cout << "Enter password: ";
    cin >> pass;
    ofstream file("users.txt", ios::app);
    file << user << " " << pass << endl;
    file.close();

    cout << "Account created successfully!\n";
}
/* ================== LOGIN ================== */
bool login()
{
    string user, pass, u, p;

    cout << "\n--- LOGIN ---\n";
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    ifstream file("users.txt");
    while (file >> u >> p)
    {
        if (u == user && p == pass)
        {
            file.close();
            cout << "Login successful!\n";
            return true;
        }
    }
    file.close();
    cout << "Invalid username or password!\n";
    return false;
}
/* ================== CREATE POST ================== */
void createPost()
{
    if (postCount >= MAX_POSTS)
    {
        cout << "Post limit reached!\n";
        return;
    }
    cin.ignore();
    cout << "Write your post: ";
    getline(cin, posts[postCount]);
    likes[postCount] = 0;
    postCount++;
    savePosts();
    cout << "Post created successfully!\n";
}

/* ================== VIEW POSTS ================== */
void viewPosts()
{
    if (postCount == 0)
    {
        cout << "No posts available!\n";
        return;
    }
    for (int i = 0; i < postCount; i++)
    {
        cout << "\nPost ID: " << i;
        cout << "\n" << posts[i];
        cout << "\nLikes: " << likes[i] << endl;
    }
}
/* ================== LIKE POST ================== */
void likePost()
{
    int id;
    viewPosts();

    cout << "\nEnter Post ID to like: ";
    cin >> id;

    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input!\n";
        return;
    }
    if (id >= 0 && id < postCount)
    {
        likes[id]++;
        savePosts();
        cout << "Post liked!\n";
    }
    else
    {
        cout << "Invalid Post ID!\n";
    }
}
/* ================== DELETE POST ================== */
void deletePost()
{
    int id;
    viewPosts();
    cout << "\nEnter Post ID to delete: ";
    cin >> id;
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input!\n";
        return;
    }
    if (id >= 0 && id < postCount)
    {
        for (int i = id; i < postCount - 1; i++)
        {
            posts[i] = posts[i + 1];
            likes[i] = likes[i + 1];
        }

        postCount--;
        savePosts();
        cout << "Post deleted!\n";
    }
    else
    {
        cout << "Invalid Post ID!\n";
    }
}
/* ================== DASHBOARD ================== */
void dashboard()
{
    int choice;
    do
    {
        cout << "\n--- META THREADS DASHBOARD ---";
        cout << "\n1. Create Post";
        cout << "\n2. View Posts";
        cout << "\n3. Like Post";
        cout << "\n4. Delete Post";
        cout << "\n5. Logout";
        cout << "\nEnter choice: ";
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice!\n";
            continue;
        }
        switch (choice)
        {
        case 1: createPost(); break;
        case 2: viewPosts(); break;
        case 3: likePost(); break;
        case 4: deletePost(); break;
        case 5: cout << "Logged out!\n"; break;
        default: cout << "Invalid option!\n";
        }
    } while (choice != 5);
}
/* ================== MAIN FUNCTION ================== */
int main()
{
    loadPosts();
    int choice;
    cout << "===== META THREADS (CONSOLE SIMULATOR) =====\n";
    do
    {
        cout << "\n1. Signup";
        cout << "\n2. Login";
        cout << "\n3. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid choice!\n";
            continue;
        }
        switch (choice)
        {
        case 1: signup(); break;
        case 2:
            if (login())
                dashboard();
            break;
        case 3: cout << "Goodbye!\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 3);
    return 0;
}

