#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void createPost();
    void viewPosts();
    void likePost();
    void deletePost();
    void logout();

private:
    // POSTS
    const int MAX_POSTS = 10;
    QString posts[10];
    int likes[10];
    int postCount = 0;

    // LOGIN
    QString currentUser;

    QListWidget *postList;

    // FILE FUNCTIONS
    void loadPosts();
    void savePosts();

    // LOGIN FUNCTIONS
    bool loginUser();
    void signupUser();
    bool checkLogin(const QString &user, const QString &pass);
};

#endif
