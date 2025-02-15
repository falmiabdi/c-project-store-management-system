#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ANSI color codes
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN_TEXT = "\033[32m"; // Green text color
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";

const int MAX_PRODUCTS = 100; 
int productCount = 0;  

struct Product {
    int id;
    string name, category;
    int quantity;
    double price;
};

Product products[MAX_PRODUCTS];
string categories[] = {"Electronics", "Clothes", "Food", "Drinks"};

void registerUser(const string& filename) {
    string username, password;
    cout << GREEN_TEXT << "Enter username: " << RESET;
    getline(cin >> ws, username);
    cout << GREEN_TEXT << "Enter password: " << RESET;
    getline(cin >> ws, password);
    ofstream file(filename, ios::app);
    file << username << " " << password << endl;
    file.close();
    cout << GREEN_TEXT << "Registration successful!\n" << RESET;
}

bool loginUser(const string& filename, string& loggedInUser) {
    string username, password, u, p;
    cout << GREEN_TEXT << "Enter username: " << RESET;
    getline(cin >> ws, username);
    cout << GREEN_TEXT << "Enter password: " << RESET;
    getline(cin >> ws, password);
    ifstream file(filename);
    
    while (file >> u) {
        file >> ws;
        getline(file, p);
        p.erase(0, p.find_first_not_of(" "));
        if (u == username && p == password) {
            cout << GREEN_TEXT << "Login successful!\n" << RESET;
            loggedInUser = username;
            return true;
        }
    }
    cout << RED << "Invalid credentials!\n" << RESET;
    return false;
}

void displayCategories() {
    cout << CYAN << "Select a category:\n" << RESET;
    for (size_t i = 0; i < sizeof(categories) / sizeof(categories[0]); ++i) {
        cout << i + 1 << ". " << categories[i] << endl;
    }
}

void addProduct() {
    if (productCount >= MAX_PRODUCTS) {
        cout << RED << "Cannot add more products. Maximum limit reached!\n" << RESET;
        return;
    }
    
    Product &p = products[productCount++];
    cout << GREEN_TEXT << "Enter Product ID: " << RESET;
    cin >> p.id;
    cout << GREEN_TEXT << "Enter Product Name: " << RESET;
    cin.ignore();
    getline(cin, p.name);

    int categoryChoice;
    displayCategories();
    cout << GREEN_TEXT << "Enter category choice: " << RESET;
    cin >> categoryChoice;
    if (categoryChoice < 1 || categoryChoice > sizeof(categories) / sizeof(categories[0])) {
        cout << RED << "Invalid category choice!\n" << RESET;
        return;
    }
    p.category = categories[categoryChoice - 1];

    cout << GREEN_TEXT << "Enter Quantity: " << RESET;
    cin >> p.quantity;
    cout << GREEN_TEXT << "Enter Price: " << RESET;
    cin >> p.price;

    ofstream file("products.txt", ios::app);
    file << p.id << " " << p.name << " " << p.category << " " << p.quantity << " " << p.price << endl;
    file.close();
    cout << GREEN_TEXT << "Product added successfully!\n" << RESET;
}

void displayProductsByCategory() {
    int categoryChoice;
    displayCategories();
    cout << GREEN_TEXT << "Enter choice: " << RESET;
    cin >> categoryChoice;
    if (categoryChoice < 1 || categoryChoice > sizeof(categories) / sizeof(categories[0])) {
        cout << RED << "Invalid choice!\n" << RESET;
        return;
    }
    string selectedCategory = categories[categoryChoice - 1];
    Product p;
    
    ifstream file("products.txt");
    cout << CYAN << "\nID\tName\tCategory\tQuantity\tPrice\n" << RESET;
    cout << "---------------------------------------------------------\n";
    while (file >> p.id >> p.name >> p.category >> p.quantity >> p.price) {
        if (p.category == selectedCategory) {
            cout << p.id << "\t" << p.name << "\t" << p.category << "\t          " << p.quantity << "\t          "<<   p.price << "\n";
        }
    }
    cout << "---------------------------------------------------------\n";
    file.close();
}

void buyProduct() {
    int id;
    cout << GREEN_TEXT << "Enter product ID to buy: " << RESET;
    cin >> id;
    bool found = false;
    for (int i = 0; i < productCount; ++i) {
        if (products[i].id == id) {
            if (products[i].quantity > 0) {
                products[i].quantity--;
                found = true;
                cout << GREEN_TEXT << "Product purchased successfully! Remaining quantity: " << products[i].quantity << "\n" << RESET;
            } else {
                cout << RED << "Product is out of stock!\n" << RESET;
            }
            break;
        }
    }
    if (!found) {
        cout << RED << "Product not found!\n" << RESET;
    }

    ofstream outFile("products.txt");
    for (int i = 0; i < productCount; ++i) {
        outFile << products[i].id << " " << products[i].name << " "
                << products[i].category << " " << products[i].quantity << " "
                << products[i].price << endl;
    }
    outFile.close();
}

void deleteProduct() {
    int id;
    cout << GREEN_TEXT << "Enter Product ID to delete: " << RESET;
    cin >> id;
    bool found = false;
    int index = -1;

    for (int i = 0; i < productCount; ++i) {
        if (products[i].id == id) {
            found = true;
            index = i;
            break;
        }
    }

    if (found) {
        for (int i = index; i < productCount - 1; ++i) {
            products[i] = products[i + 1];
        }
        productCount--;
        cout << GREEN_TEXT << "Product deleted successfully!\n" << RESET;
    } else {
        cout << RED << "Product not found!\n" << RESET;
    }

    // Update the products file
    ofstream outFile("products.txt");
    for (int i = 0; i < productCount; ++i) {
        outFile << products[i].id << " " << products[i].name << " "
                << products[i].category << " " << products[i].quantity << " "
                << products[i].price << endl;
    }
    outFile.close();
}

void adminMenu() {
    int choice;

    while (true) {
        cout << "\n3. Add Product\n4. Delete Product\n5. Exit to Main Menu\nEnter choice: ";
        cin >> choice;

        if (choice < 3 || choice > 5) {
            cout << RED << "Invalid choice! Please try again.\n" << RESET;
            continue;
        }

        switch (choice) {
            case 3: 
                addProduct(); 
                break;
            case 4: 
                deleteProduct(); 
                break;
            case 5:
                cout << GREEN_TEXT << "Exiting to main menu...\n" << RESET;
                return;
            default: 
                cout << RED << "Invalid choice!\n" << RESET;
        }
    }
}

void userMenu() {
    bool exitProgram = false;

    while (!exitProgram) {
        cout << "\n1. View Products by Category\n2. Buy Product\n3. Exit to Main Menu\nEnter choice: ";
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: 
                displayProductsByCategory(); 
                break;
            case 2: 
                buyProduct(); 
                break;
            case 3: 
                exitProgram = true; 
                cout << GREEN_TEXT << "Exiting to main menu...\n" << RESET; 
                break;
            default: 
                cout << RED << "Invalid choice!\n" << RESET;
        }
        
        // After viewing products, return to the user menu
        if (choice == 1) {
            cout << GREEN_TEXT << "Returning to the user menu...\n" << RESET;
        }
    }
}

void exitToMainMenu() {
    cout << GREEN_TEXT << "Exiting to main menu...\n" << RESET;
}

int main() {
    int choice;
    string loggedInUser;

    while (true) {
        cout << GREEN_TEXT << "Select user type:\n1. User\n2. Admin\nEnter choice: " << RESET;
        cin >> choice;

        if (choice != 1 && choice != 2) {
            cout << RED << "Invalid choice! Exiting...\n" << RESET;
            return 0;
        }

        if (choice == 1) {
            while (true) {
                cout << "\n1. Register\n2. Login\n3. Reset Password\n4. Exit to Main Menu\nEnter choice: ";
                cin >> choice;

                if (choice < 1 || choice > 4) {
                    cout << RED << "Invalid choice! Exiting...\n" << RESET;
                    return 0;
                }

                switch (choice) {
                    case 1: registerUser("users.txt");
				   break;
                    case 2: 
                        if (loginUser("users.txt", loggedInUser)) {
                            userMenu();
                        }
                        break;
                    case 3: cout << RED << "Reset password functionality pending\n" << RESET; break;
                    case 4: 
                        exitToMainMenu();
                        break;
                    default: cout << RED << "Invalid choice!\n" << RESET;
                }
            }
        } else if (choice == 2) {
            string filename = "admin.txt";
            bool loggedIn = false;

            while (true) {
                cout << "\n1. Login\n2. Exit to Main Menu\nEnter choice: ";
                cin >> choice;

                if (choice < 1 || choice > 2) {
                    cout << RED << "Invalid choice! Please try again.\n" << RESET;
                    continue;
                }

                if (choice == 1) {
                    if (loginUser(filename, loggedInUser)) {
                        adminMenu();
                    }
                } else if (choice == 2) {
                    exitToMainMenu();
                    break;
                }
            }
        }
    }
 cout<<"thank you for your service: "<<endl;
    return 0;
}