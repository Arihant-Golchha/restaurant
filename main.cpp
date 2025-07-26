#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <cctype>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstdlib>

using namespace std;

// -------------------------------
// CLASS: display_menu
// Purpose: Manages menu items (add & display)
// -------------------------------
class display_menu {
private:
    static int nextId;

    // Loads last used item ID from file to maintain ID continuity
    static void loadlast_id() {
        ifstream infile("load_id.txt");
        if (infile) {
            infile >> nextId;
            infile.close();
        }
    }

    // Saves current item ID to file to persist across sessions
    static void save_id() {
        ofstream saveFile("load_id.txt");
        saveFile << nextId;
        saveFile.close();
    }

public:
    int food_id;
    string food_name;
    float price;

    // Adds a new product to Menu.txt with a unique ID
    void add_product(string food, float p) {
        loadlast_id();
        food_id =  nextId++;
        food_name = food;
        price = p;

        ofstream outputFile("Menu.txt", ios::app); // Append to existing file
        if (!outputFile) {
            cout << "Error creating file" << endl;
        } else {
            outputFile << food_id << " " << food << " " << p << endl;
            outputFile.close();
        }
        save_id();
    }

    // Displays all items from the menu to the customer
    void display_all() {
        ifstream inputFile("Menu.txt");
        string name;
        float price;
        int id;

        cout << "\n--- Menu Items ---\n";

        if (!inputFile) {
            cout << "Menu file not found.\n";
        } else {
            while (inputFile >> id >> name >> price) {
                cout << "ID: " << id << " | Item: " << name << " | Price: Rs. " << price << endl;
            }
            inputFile.close();
        }
    }
    bool checkfood_name(const string& name) {
    for (char c : name) {
        if (!isalpha(c) && c != ' ') { // allow spaces in item names
            return false;
        }
    }
    return !name.empty(); // name must not be empty
}
string sanitize_name(string name) {
        for (char& c : name) {
            if (c == ' ') c = '_';
        }
        return name;
    }

};

// Initialize static variable for unique food ID
int display_menu::nextId = 1001;


// -------------------------------
// CLASS: billing
// Purpose: Handle order billing based on item IDs
// -------------------------------
class billing {
public:
    // Updated: Accepts vector of (item_id, quantity)
    void Bill(const vector<pair<int, int>>& item_orders) {
        ifstream open_menu("Menu.txt"); 
        ofstream latest_bill("latest_bill.txt");
        ofstream Bill_to_csv("Bills.csv",ios::app);
        int id;
        string item_name;
        float price;
        float total = 0;

        if (!open_menu) {
            cout << "Menu file not found.\n";
            return;
        }

        // Load menu items into memory
        vector<tuple<int, string, float>> menu_items;
        while (open_menu >> id >> item_name >> price) {
            menu_items.push_back({id, item_name, price});
        }
        open_menu.close();

        cout << "\n===== BILL =====\n";
        // Get current date
auto now = chrono::system_clock::now();
time_t now_time = chrono::system_clock::to_time_t(now);
tm* local_tm = localtime(&now_time);

stringstream date_stream;
date_stream << put_time(local_tm, "%Y-%m-%d"); // Format: YYYY-MM-DD
string current_date = date_stream.str();

        for (auto& order : item_orders) {
            int order_id = order.first;
            int qty = order.second;

            for (auto& item : menu_items) {
                if (get<0>(item) == order_id) {
                    string name = get<1>(item);
                    float unit_price = get<2>(item);
                    float subtotal = unit_price * qty;
                    total += subtotal;
                    //for console printing
                    cout << name << " (x" << qty << ") - Rs. " << subtotal << endl;
                    //for latest bill printing
                    latest_bill << name << " (x" << qty << ") - Rs. " << subtotal << endl;
                    
                    

                }
            }
        }

        cout << "----------------\n";
        cout << "Total: Rs. " << total << endl;
        //for saving bill to excel file
        Bill_to_csv << current_date << "," << total << endl;
        system("notepad /p latest_bill.txt");
        if(Bill_to_csv){
            cout<<"Successfully added to bill";

            return;
        }
    }
   
};

// class = edit_menu
// work = change the details of the menu food
class edit_menu{
    public: 
    // function used to search food from the menu
    void Search_item(int i){
            int id;
            string food_name;
            float price;

            ifstream searchfile("Menu.txt");
            if(!searchfile){
                cout<<"Menu not found please make sure you have item added to Menu"<<endl;
                return ;
            }
            else {
                while(searchfile>>id>>food_name>>price){
                    if(id == i){
                        int check;
                        cout<<id <<" found with name "<< food_name<<endl;
                        return;
                    }
                }
            }
            searchfile.close();

    }
    // function used for editing name of the food item
    void edit_menu_name(int edit_id) {
    ifstream infile("Menu.txt");
    ofstream temp("temp.txt");
    bool found = false;

    if (!infile || !temp) {
        cout << "Error opening files.\n";
        return;
    }

    int id;
    string name;
    float price;

    while (infile >> id >> name >> price) {
        if (id == edit_id) {
            cout << "Current name: " << name << endl;

            string new_name;
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, new_name);

            display_menu helper;
            if (!helper.checkfood_name(new_name)) {
                cout << "Invalid name. Use alphabet only.\n";
                temp << id << " " << name << " " << price << endl; // keep original
            } else {
                new_name = helper.sanitize_name(new_name);
                temp << id << " " << new_name << " " << price << endl;
                cout << "Name updated successfully.\n";
                found = true;
            }
        } else {
            temp << id << " " << name << " " << price << endl;
        }
    }

    infile.close();
    temp.close();

    remove("Menu.txt");
    rename("temp.txt", "Menu.txt");

    if (!found) {
        cout << "Item with ID " << edit_id << " not found.\n";
    }
}
// function used for edit price of food from the menu
        void edit_menu_price(int edit_id) {
    ifstream infile("Menu.txt");
    ofstream temp("temp.txt");
    bool found = false;

    if (!infile || !temp) {
        cout << "Error opening files.\n";
        return;
    }

    int id;
    string name;
    float price;

    while (infile >> id >> name >> price) {
        if (id == edit_id) {
            cout << "Current price: " << price << endl;

            int new_price;
            cout << "Enter new price: ";
            cin>>new_price;

            display_menu helper;
            
                temp << id << " " << name << " " << new_price << endl;
                cout << "price updated successfully.\n";
                found = true;
            
        } else {
            temp << id << " " << name << " " << price << endl;
        }
    }

    infile.close();
    temp.close();

    remove("Menu.txt");
    rename("temp.txt", "Menu.txt");

    if (!found) {
        cout << "Item with ID " << edit_id << " not found.\n";
    }
}
// function used for deleting the food item from menu
void edit_menu_delete(int i) {
    int id;
    string food_name;
    float price;
    bool found = false;

    ifstream infile("Menu.txt");
    ofstream temp("temp.txt");

    if (!infile || !temp) {
        cout << "Error opening files.\n";
        return;
    }

    vector<tuple<int, string, float>> updated_items;

    while (infile >> id >> food_name >> price) {
        if (id == i) {
            found = true; // mark for deletion
            continue;     // skip writing this one
        }
        updated_items.push_back({id, food_name, price});
    }

    infile.close();

    if (!found) {
        cout << "Item with ID " << i << " not found.\n";
        temp.close();
        remove("temp.txt");
        return;
    }

    
    
    for (auto& item : updated_items) {
        if (get<0>(item) >= i) {
            get<0>(item)--; // decrement ID after the deleted item
        }
    }
    

    for (auto& item : updated_items) {
        temp << get<0>(item) << " " << get<1>(item) << " " << get<2>(item) << endl;
    }

    temp.close();

    remove("Menu.txt");
    rename("temp.txt", "Menu.txt");

    // Update nextId in load_id.txt
    int nextId;
    ifstream loadIdFile("load_id.txt");
    if (loadIdFile) {
        loadIdFile >> nextId;
        loadIdFile.close();
    }

    ofstream saveIdFile("load_id.txt");
    saveIdFile << nextId - 1;
    saveIdFile.close();

    cout << "Item deleted successfully.\n";
}

};

// -------------------------------
// FUNCTION: main
// Purpose: Main interface for user/admin operations
// -------------------------------
int main() {
    int choice;

    while (true) {
        cout << "\nWelcome to the Restro" << endl;
        cout << "1. Order Food" << endl;
        cout << "2. Admin Login (Add Item)" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter Your Choice: ";
        cin >> choice;

        if (choice == 1) {
            display_menu menu;
            menu.display_all(); // Show menu

            vector<pair<int,int>> item_orders;
            int code;
            int quantity;

            // Take multiple item codes from user
            cout << "\nEnter item IDs to order (0 to finish):" << endl;

            while (true) {
                cout << "ID: ";
                cin >> code;
                if (code == 0) break;
                cout<<"Enter the Quantity for "<<code<<": ";
                cin>>quantity;
                if(quantity<=0){
                    cout<<"invalid quantity please make sure the quantity are valid ";
                    continue;
                }
                item_orders.push_back({code, quantity});

        }
        bool generate_bill = false;
                int Bill_generation;
                cout<<"do you wish to edit bill(press 0 for yes)"<<endl;
                cout<<"do you wish to generate bill(press 1 for yes) : ";
                
                cin>>Bill_generation;
                switch (Bill_generation)
                {
                case 0: 
                    
                    break;
                
                case 1: 
                    generate_bill = true;
                    break;
                default:
                    cout<<"Invalid input please make sure that you have enter the correct key: "<<endl;
                    break;
                }
                if(generate_bill){
                
                billing bill;
            bill.Bill(item_orders);
            }
            // Generate and display bill
            
        }

        else if (choice == 2) {
            int choice_2;
            string food_name;
            float price;
            cout<<"1. add product"<<endl;
            cout<<"2. Edit product "<<endl;
            cout<<"0. exit to main menu"<<endl;
            cout<<"enter your choice: ";
            cin>>choice_2;
            switch (choice_2)
            {
            case 1:
                  {
                    cin.ignore(); // Clears newline character from input buffer
            cout << "Enter the name of the Item: ";
            getline(cin, food_name);
            display_menu checkfood;
            if(!checkfood.checkfood_name(food_name)){
                cout<<"invalid name of the food item! please check the food to continue"<<endl;
                continue;
            }
            else {
                food_name = checkfood.sanitize_name(food_name);
            cout << "Enter the price of item: ";
            cin >> price;
            if(price>0){
            

            display_menu item;
            item.add_product(food_name, price);
            cout << "Item added successfully!\n";
            break;
        }
            else {
                cout<<"please enter a valid price";
                continue;

            }
            } 
        }
           case 2: {
                    int choice;
                    int edit_code;
                    display_menu menu;
                    menu.display_all(); // Show menu

                    cout<<"enter the item_code which you want to edit: ";
                    cin>>edit_code;
                    edit_menu food_search;
                    food_search.Search_item(edit_code);
                    cout<<"1. edit product\n0. go to main menu"<<endl;
                    cout<<"Do you wish to edit this product: ";
                    cin>>choice;
                    switch (choice)
                    {
                    case 1: {
                        int choice;
                        cout<<"what do you want to change"<<endl;
                            cout<<"1. Name\n2. price\n 3. remove from menu\n 0. main menu"<<endl;
                            cout<<"enter your choice: ";
                            cin>>choice;
                            switch (choice)
                            {
                            case 1: {
                                edit_menu name;
                                name.edit_menu_name(edit_code);
                                break;
                            }
                            case 2: {
                                edit_menu price;
                                price.edit_menu_price(edit_code);
                                break;
                            }
                            case 3: {
                                edit_menu edit;
                                edit.edit_menu_delete(edit_code);
                                break;
                            }
                            case 0: {
                                break;
                            }
                            default:
                            cout<<"invalid input please try again";
                                continue;
                            }
                        break;
                    }
                    case 0: {
                        break;
                    }
                    
                    default:
                        break;
                    }

                    
                break;
           }     
            
            default:
                cout<<"invalid input !! please try again."<<endl;
                break;
            }

           
        }

        else if (choice == 3) {
            cout << "Thanks for using the app\n";
            break;
        }

        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
