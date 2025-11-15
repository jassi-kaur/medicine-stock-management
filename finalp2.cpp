#include <iostream>
#include <fstream>
#include <iomanip>  // Added for setw and other formatting
using namespace std;

 class Medicine {
 	public:
    int id;
    char name[50];
    int quantity;
    int price;
};

bool login() {
    string username, password;
    int attempts = 0;
    const int MAX_ATTEMPTS = 3;
    const string CORRECT_USERNAME = "admin";
    const string CORRECT_PASSWORD = "pass123";

    while (attempts < MAX_ATTEMPTS) {
        cout << "\nMEDICAL STOCK MANAGEMENT SYSTEM LOGIN\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;

        // Simple string comparison
        bool userMatch = true;
        bool passMatch = true;
        
        // Check username
        if (username.length() != CORRECT_USERNAME.length()) {
            userMatch = false;
        } else {
            for (int i = 0; i < username.length(); i++) {
                if (username[i] != CORRECT_USERNAME[i]) {
                    userMatch = false;
                    break;
                }
            }
        }
        
        // Check password
        if (password.length() != CORRECT_PASSWORD.length()) {
            passMatch = false;
        } else {
            for (int i = 0; i < password.length(); i++) {
                if (password[i] != CORRECT_PASSWORD[i]) {
                    passMatch = false;
                    break;
                }
            }
        }

        if (userMatch && passMatch) {
            cout << "\nLogin successful!\n";
            return true;
        } else {
            attempts++;
            cout << "\nInvalid username or password! Attempts remaining: " 
                 << MAX_ATTEMPTS - attempts << "\n";
        }
    }

    cout << "\nMaximum login attempts reached. Exiting...\n";
    return false;
}
void addMedicine() {
    Medicine med;
    cout << "\nEnter Medicine ID (1-1000): ";
    cin >> med.id;
    while(med.id < 1 || med.id > 1000) {
        cout << "Invalid ID! Enter between 1-1000: ";
        cin >> med.id;
    }

    cout << "Enter Medicine Name: ";
    cin >> med.name;

    cout << "Enter Quantity (1-10000): ";
    cin >> med.quantity;
    while(med.quantity < 1 || med.quantity > 10000) {
        cout << "Invalid Quantity! Enter between 1-10000: ";
        cin >> med.quantity;
    }

    cout << "Enter Price (10-5000): ";
    cin >> med.price;
    while(med.price < 10 || med.price > 5000) {
        cout << "Invalid Price! Enter between 10-5000: ";
        cin >> med.price;
    }

    ofstream outFile("medicines.dat", ios::app | ios::binary);
    outFile.write(reinterpret_cast<char*>(&med), sizeof(Medicine));
    outFile.close();
    cout << "Medicine added successfully!\n";
}

void displayMedicines() {
    ifstream inFile("medicines.dat", ios::binary);
    if(!inFile) {
        cout << "No medicines found!\n";
        return;
    }

    cout << "\nMEDICINE INVENTORY LISTING\n";
    cout << "--------------------------------------------\n";
    cout << left << setw(8) << "ID" << setw(20) << "NAME" 
         << setw(12) << "QUANTITY" << setw(10) << "PRICE" << endl;
    cout << "--------------------------------------------\n";

    Medicine med;
    while(inFile.read(reinterpret_cast<char*>(&med), sizeof(Medicine))) {
        cout << left << setw(8) << med.id 
             << setw(20) << med.name 
             << setw(12) << med.quantity 
              << setw(7) << med.price << endl;
    }
    
    cout << "--------------------------------------------\n";
    inFile.close();
}
void searchMedicine() {
    int searchId;
    cout << "Enter Medicine ID to search: ";
    cin >> searchId;

    ifstream inFile("medicines.dat", ios::binary);
    bool found = false;
    Medicine med;

    while(inFile.read(reinterpret_cast<char*>(&med), sizeof(Medicine))) {
        if(med.id == searchId) {
            cout << "\nID: " << med.id << "\nName: " << med.name 
                 << "\nQuantity: " << med.quantity << "\nPrice: " << med.price << endl;
            found = true;
            break;
        }
    }

    if(!found) cout << "Medicine not found!\n";
    inFile.close();
}

void deleteMedicine() {
    int delId;
    cout << "Enter Medicine ID to delete: ";
    cin >> delId;

    ifstream inFile("medicines.dat", ios::binary);
    ofstream tempFile("temp.dat", ios::binary);
    bool found = false;
    Medicine med;

    while(inFile.read(reinterpret_cast<char*>(&med), sizeof(Medicine))) {
        if(med.id == delId) {
            found = true;
            continue; // Skip writing to temp file (delete)
        }
        tempFile.write(reinterpret_cast<char*>(&med), sizeof(Medicine));
    }

    inFile.close();
    tempFile.close();

    if(found) {
        remove("medicines.dat");
        rename("temp.dat", "medicines.dat");
        cout << "Medicine deleted successfully!\n";
    } else {
        remove("temp.dat");
        cout << "Medicine not found!\n";
    }
}

void modifyMedicine() {
    int modId;
    cout << "Enter Medicine ID to modify: ";
    cin >> modId;

    fstream file("medicines.dat", ios::in | ios::out | ios::binary);
    bool found = false;
    Medicine med;

    while(file.read(reinterpret_cast<char*>(&med), sizeof(Medicine))) {
        if(med.id == modId) {
            found = true;
            cout << "\nCurrent Details:\n";
            cout << "ID: " << med.id << "\nName: " << med.name 
                 << "\nQuantity: " << med.quantity << "\nPrice: " << med.price << endl;

            cout << "\nEnter new details:\n";
            cout << "New Name: ";
            cin >> med.name;

            cout << "New Quantity (1-10000): ";
            cin >> med.quantity;
            while(med.quantity < 1 || med.quantity > 10000) {
                cout << "Invalid! Enter 1-10000: ";
                cin >> med.quantity;
            }

            cout << "New Price (10-5000): ";
            cin >> med.price;
            while(med.price < 10 || med.price > 5000) {
                cout << "Invalid! Enter 10-5000: ";
                cin >> med.price;
            }

            file.seekp(-sizeof(Medicine), ios::cur);
            file.write(reinterpret_cast<char*>(&med), sizeof(Medicine));
            break;
        }
    }

    file.close();
    cout << (found ? "Medicine updated!\n" : "Medicine not found!\n");
}
int main() {
    if (!login()) {
        return 0; // Exit if login fails
    }

    int choice;
    do {
        cout << "\n****--Medicine Stock Management--****\n";
        cout << "1. Add Medicine\n";
        cout << "2. Display All Medicines\n";
        cout << "3. Search Medicine\n";
        cout << "4. Delete Medicine\n";
        cout << "5. Modify Medicine\n";
        cout << "6. Exit\n";
        cout << "Enter choice(1-6): ";
        cin >> choice;

        switch(choice) {
            case 1: addMedicine(); break;
            case 2: displayMedicines(); break;
            case 3: searchMedicine(); break;
            case 4: deleteMedicine(); break;
            case 5: modifyMedicine(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 6);

    return 0;
}
