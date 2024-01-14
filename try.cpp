#include <iostream>
#include <stack>

using namespace std;

// Your existing functions go here

int main() {
    // Declare variables and initialize stacks
    int options, options2, options3, options4;
    int account_number, account_number1, account_number2;
    stack<int> mainMenuStack;
    stack<int> subMenuStack;

    do {
        // Display main menu
        cout << "---------- ********** ---------- ********** ---------- ********** WELCOME TO THE CROSS-CONTINENTAL TREASURY BANK ********** ---------- ********** ---------- ********** ----------" << endl;

        cout << "Enter your choice: ";
        cin >> options;

        switch (options) 
        {
            case 1:
                break;

            case 2:
                do 
                {
                    cout << "Choose among the options below, what best suits your requirements" << endl;

                    cout << "Enter your choice (0 to go back to the main menu): ";
                    cin >> options2;

                    if (options2 == 0 && !subMenuStack.empty()) 
                    {
                        options2 = subMenuStack.top();
                        subMenuStack.pop();
                    }

                    switch (options2) 
                    {
                        case 1:

                            break;

                        case 2:
                            break;

                        case 3:
                            break;

                        case 4:
                            break;

                        case 5:
                            do {
                                cout << "Choose among the options below, what best suits your requirements" << endl;

                                cout << "Enter your choice (0 to go back to the account options): ";
                                cin >> options3;

                                if (options3 == 0 && !subMenuStack.empty()) 
                                {
                                    options3 = subMenuStack.top();
                                    subMenuStack.pop();
                                }

                                switch (options3) {
                                    case 1:
                                    break;

                                    case 2:
                                    break;

                                } 

                            } while (options3 != 0);

                            break;

                        case 6:
                            break;


                    } // End of sub-menu switch

                    // If the user didn't choose to go back, push the current sub-menu option onto the stack
                    if (options2 != 0) 
                    {
                        subMenuStack.push(options2);
                    }

                } while (options2 != 0);

            break;

            case 3:
            break;


        } // End of main menu switch

        // If the user didn't choose to go back, push the current main menu option onto the stack
        if (options != 0) 
        {
            mainMenuStack.push(options);
        }

    } while (options != 0);

}
