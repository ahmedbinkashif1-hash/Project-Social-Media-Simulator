#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // LOGIN FIRST
    if (!loginUser())
    {
        close();
        return;
    }

    setWindowTitle("Social Media Account Simulator");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QVBoxLayout *menuLayout = new QVBoxLayout;

    QPushButton *btnCreate = new QPushButton("Create Post");
    QPushButton *btnDelete = new QPushButton("Delete Post");
    QPushButton *btnView = new QPushButton("View All Posts");
    QPushButton *btnLike = new QPushButton("Like a Post");
    QPushButton *btnLogout = new QPushButton("Logout");

    menuLayout->addWidget(btnCreate);
    menuLayout->addWidget(btnDelete);
    menuLayout->addWidget(btnView);
    menuLayout->addWidget(btnLike);
    menuLayout->addStretch();
    menuLayout->addWidget(btnLogout);

    postList = new QListWidget;

    mainLayout->addLayout(menuLayout, 1);
    mainLayout->addWidget(postList, 3);

    central->setLayout(mainLayout);

    connect(btnCreate, &QPushButton::clicked, this, &MainWindow::createPost);
    connect(btnView, &QPushButton::clicked, this, &MainWindow::viewPosts);
    connect(btnLike, &QPushButton::clicked, this, &MainWindow::likePost);
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::deletePost);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::logout);

    loadPosts();
    viewPosts();
}

/* ================= LOGIN ================= */

bool MainWindow::loginUser()
{
    QStringList options = {"Login", "Signup"};
    bool ok;
    QString choice = QInputDialog::getItem(
        this, "Welcome", "Choose an option:", options, 0, false, &ok);

    if (!ok) return false;

    QString user = QInputDialog::getText(this, "Username", "Enter username:");
    QString pass = QInputDialog::getText(this, "Password",
                                         "Enter password:",
                                         QLineEdit::Password);

    if (choice == "Signup")
    {
        signupUser();
        return loginUser();
    }

    if (checkLogin(user, pass))
    {
        currentUser = user;
        return true;
    }

    QMessageBox::warning(this, "Error", "Invalid username or password!");
    return loginUser();
}

void MainWindow::signupUser()
{
    QString user = QInputDialog::getText(this, "Signup", "Choose username:");
    QString pass = QInputDialog::getText(this, "Signup",
                                         "Choose password:",
                                         QLineEdit::Password);

    QFile file("users.txt");
    file.open(QIODevice::Append);
    QTextStream out(&file);
    out << user << " " << pass << "\n";
    file.close();

    QMessageBox::information(this, "Success", "Account created!");
}

bool MainWindow::checkLogin(const QString &user, const QString &pass)
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly)) return false;

    QTextStream in(&file);
    QString u, p;

    while (!in.atEnd())
    {
        in >> u >> p;
        if (u == user && p == pass)
            return true;
    }
    return false;
}

/* ================= POSTS ================= */

void MainWindow::loadPosts()
{
    QFile file("posts.txt");
    if (!file.open(QIODevice::ReadOnly)) return;

    QTextStream in(&file);
    in >> postCount;
    in.readLine();

    for (int i = 0; i < postCount; i++)
    {
        posts[i] = in.readLine();
        in >> likes[i];
        in.readLine();
    }
    file.close();
}

void MainWindow::savePosts()
{
    QFile file("posts.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    out << postCount << "\n";
    for (int i = 0; i < postCount; i++)
    {
        out << posts[i] << "\n";
        out << likes[i] << "\n";
    }
    file.close();
}

void MainWindow::viewPosts()
{
    postList->clear();
    for (int i = 0; i < postCount; i++)
    {
        QString display = posts[i];
        display.replace("<NL>", "\n");
        display.replace("|", ":\n");

        postList->addItem(display +
                          "\nLikes: " +
                          QString::number(likes[i]));

    }
}

void MainWindow::createPost()
{
    if (postCount >= MAX_POSTS)
    {
        QMessageBox::warning(this, "Limit", "Post limit reached!");
        return;
    }

    bool ok;
    QString text = QInputDialog::getMultiLineText(
        this, "Create Post", "Write your post:", "", &ok);

    if (ok && !text.isEmpty())
    {
        QString safeText = text;
        safeText.replace("\n", "<NL>");

        posts[postCount] = currentUser + "|" + safeText;

        likes[postCount] = 0;
        postCount++;
        savePosts();
        viewPosts();
    }
}

void MainWindow::likePost()
{
    int row = postList->currentRow();
    if (row < 0) return;

    likes[row]++;
    savePosts();
    viewPosts();
}

void MainWindow::deletePost()
{
    int row = postList->currentRow();
    if (row < 0) return;

    for (int i = row; i < postCount - 1; i++)
    {
        posts[i] = posts[i + 1];
        likes[i] = likes[i + 1];
    }

    postCount--;
    savePosts();
    viewPosts();
}

void MainWindow::logout()
{
    QMessageBox::information(this, "Logout", "Logged out!");
    close();
}
