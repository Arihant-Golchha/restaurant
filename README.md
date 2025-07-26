# 🍽️ Restaurant Billing System (C++ Console App)

A simple and effective C++ application for managing restaurant operations — from menu management to order billing, with file storage and bill export features.

## 🚀 Features

- ✅ Add, edit, and delete menu items (with auto-incremented IDs)
- 🧾 Place orders with item quantity
- 💵 Generate customer bills
- 📄 Save human-readable bill to `latest_bill.txt`
- 📊 Log structured billing data in `bills.csv` (Excel-compatible)
- 📅 Adds current **date** to every bill
- 🔐 Admin and customer modes

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 🛠️ Tech Stack

- **Language:** C++
- **File Handling:** `ifstream`, `ofstream` for `.txt` and `.csv`
- **Date Handling:** `<ctime>`
- **Data Structures:** `vector`, `tuple`, `pair`

------------------------------------------------------------------------------------------------------------------------------------------------------------------

## 📁 File Structure

| File              | Purpose                                 |
|-------------------|-----------------------------------------|
| `main.cpp`        | Main source code                        |
| `Menu.txt`        | Stores menu items                       |
| `load_id.txt`     | Tracks last used item ID                |
| `latest_bill.txt` | Most recent bill (formatted)            |
| `bills.csv`       | All bills with date (Excel format)      |
| `temp.txt`        | Temporary file for safe file operations |

-------------------------------------------------------------------------------------------------------------------------------------------------------------------

📌 Future Improvements
GUI interface using Qt or other frameworks

Admin login authentication

Customer name & contact logging

Tax and discount system

Print bills directly
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
## 🧪 How to Run

### Step 1: Compile the code
```bash
g++ main.cpp -o restaurant
```
------------------------------------------------------------------------------------------------------------------------------------------------------------------
🙋‍♂️ Author
Arihant Golchha
🔗 linkedIn = www.linkedin.com/in/arihant-golchha-086460342
🐙 GitHub = https://github.com/Arihant-Golchha

If you found this project helpful, give it a ⭐️ and consider sharing it with others!
