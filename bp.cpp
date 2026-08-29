#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

// constants

/*calculations*/
const double tax = 0.06;
const double member_discount = 0.20;
const double membership_fee = 100.00;
/*array max value*/
const int hist_max = 100;

// array structures 
struct Customer {
	string cust_id;
	string cust_name;
	bool cust_member;
};

struct Staff {
	string staff_id;
	string staff_name;
	string username;
	string password;
};

struct Service {
	string customer_id;
	string serv_id;
	string serv_name;
	int serv_qty;
	double serv_price;
};

struct Appointment {
	string customer_id;
	string appmt_id;
	string appmt_name;
	int appmt_qty;
	double appmt_price;
};

struct histRecord {
	int bill_id;
	string customer_id;
	int payment_id;
	double payment_amt;
	double payable;
	double change;
	string payment_method;
	string payment_status;
	string payment_type;
};

struct pmtResult {
	int bill_id;
	int pmt_id;
	bool status;
	double change;
};

// global variables
Customer cust_list[100] = {
	{"C01", "John Lee", true},
	{"C02", "Alice Tan", false},
	{"C03", "Bob Sam", true}
};
int cust_count = 3;

Staff staff_list[100] = {
	{"ST01", "Admin", "admin", "1234"},
	{"ST02", "Sarah Lim", "sarah", "5678"},
	{"ST03", "Daniel Tan", "daniel", "9999"}
};
int staff_count = 3;

Service serv_list[100] = {
	{"C01", "S01", "Haircut", 1, 50.00},
	{"C02", "S02", "Wash", 3, 60.00},
	{"C03", "S03", "Makeup", 2, 100.00}
};
int serv_count = 3;

Appointment appmt_list[100] = {
	{"C01", "A01", "Wedding", 1, 50.00},
	{"C02", "A02", "Hair Spa", 3, 60.00},
	{"C03", "A03", "Photograph", 2, 100.00}
};
int appmt_count = 3;

histRecord history[hist_max] = {
	// bill_id, customer_id, payment_id, payment_amt, payable, change, method, status, type

	{10001, "C01", 20001, 50.00, 42.40, 7.60, "Cash", "Success", "Service"},
	{10001, "C01", 20002, 42.40, 42.40, 0.00, "E-Wallet", "Success", "Apponitment"},
	{10002, "C02", 20003, 100.00, 100.00, 0.00, "E-Wallet", "Success", "Membership"},
	{10003, "C03", 20004, 50.00, 50.00, 0.00, "Online Banking", "Success", "Service"},
	{10003, "C03", 20005, 120.00, 100.00, 20.00, "Cash", "Success", "Apponitment"},
	{10004, "C01", 20006, 0.00, 42.40, 0.00, "Cash", "Cancelled", "Service"}
};
int history_count = 6;

//function prototypes


void customer(string search_id, string& found_name, bool& found_member);
void service(string search_id, double& out_total, int& out_qty);
void appointment(string search_id, double& out_total, int& out_qty);


void payment(string cust_id, histRecord history[], int& history_count);

pmtResult pmt_member(string cust_id, int bill_id, histRecord history[], int& history_count);
pmtResult pmt_service(string customer_id, int bill_id, histRecord history[], int& history_count);
pmtResult pmt_appmt(string customer_id, int bill_id, histRecord history[], int& history_count);

pmtResult pmt_process(string cust_id, int bill_id, double pmt_payable,
	histRecord history[], int& history_count, string payment_type);

void receipt(int bill_id, int pmt_id, double change, string cust_id,
	histRecord history[], int& history_count);

void invoice(int bill_id, int pmt_id, double change, string cust_id,
	histRecord history[], int& history_count);

void pmt_history(int bill_id, string customer_id, int pmt_id, double pmt_amt, double payable, 
	double change, string method, bool status, string payment_type,
	histRecord history[], int& history_count);

void view_history(histRecord history[], int history_count, bool is_staff, string cust_id);

void logo();
bool staffLogin();
bool custLogin(string& cust_id);

// helper functions
double calc_subtotal(double price, int qty);
double calc_discount(double total_price, bool is_member);
double calc_tax(double total_after_disc);
double calc_payable(double total_after_disc, double tax_amt);

int generateID();
void clear_input();

int main()
{
	char action;
	string cust_id;
	bool is_staff = false;

	// just for testing
	char login_choice;
	bool login_success = false;

	do
	{
		cout << "\n========== LOGIN ==========\n";
		cout << "1. Staff Login\n";
		cout << "2. Customer Login\n";
		cout << "3. Exit\n";
		cout << "Choice: ";
		cin >> login_choice;

		switch (login_choice)
		{
		case '1':
			if (staffLogin() == true)
			{
				login_success = true;
				is_staff = true;

				cout << "\nWelcome Staff!\n";

				// Staff menu
				// staffMenu();

			}
			break;

		case '2':
			if (custLogin(cust_id) == true)
			{
				login_success = true;
				is_staff = false;

				cout << "\nWelcome Customer!\n";

				// Customer menu
				// customerMenu(cust_id);

			}
			break;

		case '3':
			cout << "Exiting program...\n";
			return 0;

		default:
			cout << "Invalid choice. Please re-enter.\n";
		}

	} while (login_choice != '3' && login_success == false);
	// /njust for testing

	// billing and payment menu
	do
	{
		cout << "\n=======================================\n";
		cout << "      BILLING AND PAYMENT PROCESS\n";
		cout << "========================================\n";
		cout << "1. Payment Progress\n";
		cout << "2. Payment History\n";
		cout << "3. Exit Module \n";
		cout << "Choice: ";
		cin >> action;

		switch (action)
		{
		case '1':
			payment(cust_id, history, history_count);
			break;
		case '2':
			view_history(history, history_count, is_staff, cust_id);
			break;
		case '3':
			cout << "Exiting Billiing and Payment Process....\n";
			break;
		default:
			cout << "Invalid menu choice. Please re-enter.\n";
			break;
		}
	} while (action != '3');

	return 0;
}

// find customer  (for testing)
void customer(string search_id, string& found_name, bool& found_member)
{
	found_name = "Unknown";
	found_member = false;

	for (int i = 0; i < cust_count; i++)
	{
		if (cust_list[i].cust_id == search_id)
		{
			found_name = cust_list[i].cust_name;
			found_member = cust_list[i].cust_member;
			break;
		}
	}
}

//find service (for testing)
void service(string search_id, double& out_total, int& out_qty)
{
	out_total = 0;
	out_qty = 0;

	for (int i = 0; i < serv_count; i++)
	{
		if (serv_list[i].customer_id == search_id)
		{
			out_total += calc_subtotal(
				serv_list[i].serv_price, serv_list[i].serv_qty
			);

			out_qty += serv_list[i].serv_qty;
		}

	}
}

// find appointment (for testing)
void appointment(string search_id, double& out_total, int& out_qty)
{
	out_total = 0;
	out_qty = 0;

	for (int i = 0; i < appmt_count; i++)
	{
		if (appmt_list[i].customer_id == search_id)
		{
			out_total += calc_subtotal(
				appmt_list[i].appmt_price, appmt_list[i].appmt_qty
			);

			out_qty += appmt_list[i].appmt_qty;
		}
	}
}

// // PAYMENT MENU // //

void payment(string cust_id, histRecord history[], int& history_count)
{
	char action;
	char other_payment = 'N';

	// one bill id per payment section
	int bill_id = generateID();

	pmtResult result;
	bool payment_done = false;

	do
	{
		cout << "\n -------------- PAYMENT ---------------\n";
		cout << "1. Member Registration / Renew Payment\n";
		cout << "2. Service Payment\n";
		cout << "3. Appointment Payment\n";
		cout << "4. Exit Module \n";
		cout << "Choice: ";
		cin >> action;

		payment_done = false;

		switch (action)
		{
		case '1':
			result = pmt_member(cust_id, bill_id, history, history_count);

			if (result.status == true)
			{
				payment_done = true;
			}
			break;

		case '2':
			result = pmt_service(cust_id, bill_id, history, history_count);

			if (result.status == true)
			{
				payment_done = true;
			}
			break;

		case '3':
			result = pmt_appmt(cust_id, bill_id, history, history_count);

			if (result.status == true)
			{
				payment_done = true;
			}
			break;

		case '4':
			cout << "Exiting Payment Process....\n";
			break;

		default:
			cout << "Invalid menu choice. Please re-enter.\n";
			break;
		}

		// successful payment
		if (payment_done == true && action != '4')
		{
			cout << "\nDo you want to make another payment? (Y/N): ";
			cin >> other_payment;

			while (tolower(other_payment) != 'y' &&
				tolower(other_payment) != 'n')
			{
				cout << "Invalid choice. Please enter Y or N: ";
				cin >> other_payment;
			}

			// N = finish this payment session
			if (tolower(other_payment) == 'n')
			{
				break;
			}
		}

	} while (action != '4');

	// printing receipt or invoice
	if (payment_done == true)
	{
		char print_choice;

		do
		{
			cout << "\nDo you want to print the Receipt or Invoice?\n";
			cout << "1. Receipt\n";
			cout << "2. Invoice\n";
			cout << "3. No and Exit\n";
			cout << "Choice: ";
			cin >> print_choice;

			switch (print_choice)
			{
			case '1':
				receipt(bill_id, 0, 0, cust_id,
					history, history_count);
				break;

			case '2':
				invoice(bill_id, 0, 0, cust_id,
					history, history_count);
				break;

			case '3':
				cout << "Thank you for Purchasing\n";
				cout << "Exiting the payment module...\n";
				break;

			default:
				cout << "Invalid choice. Please re-enter...\n";
				break;
			}

		} while (print_choice != '1' &&
			print_choice != '2' &&
			print_choice != '3');
	}
}

// 1. Member Registration / Renew
pmtResult pmt_member(string cust_id, int bill_id, histRecord history[], int& history_count)
{
	double payable;
	int duration;
	char retry = 'N';
	pmtResult result;

	cout << "\nThe Member Regitration/Renew fee is RM100.00/MONTH\n";

	do
	{
		// asking for the membership month
		cout << "Number of Month(s): ";
		cin >> duration;

		if (duration <= 0)
		{
			cout << "Invalid duration. Please re-enter...\n";
			clear_input();
			duration = 0;
		}

	} while (duration == 0);

	payable = membership_fee * duration;

	do
	{
		// payment process
		result = pmt_process(cust_id, bill_id, payable, history, history_count, "Membership");

		if (result.status == true)
		{
			// updating member status when success
			for (int i = 0; i < cust_count; i++)
			{
				if (cust_list[i].cust_id == cust_id)
				{
					cust_list[i].cust_member = true;
					break;
				}
			}

			cout << "Payment SUCCEED! \n";
			return result;

		}
		else
		{
			cout << "Payment FAILURE\n"; // payment failure
			cout << "Do you want to redo the payment? (Y/N): ";
			cin >> retry;

			while (tolower(retry) != 'y' && tolower(retry) != 'n')
			{
				cout << "Invalid choice. Please enter Y or N: ";
				cin >> retry;
			}
		}
	} while (tolower(retry) == 'y');

	cout << "Exiting the Progress...\n";
	return result;
}

// 2. Payment for services
pmtResult pmt_service(string customer_id, int bill_id, histRecord history[], int& history_count)
{
	int total_qty = 0;
	double total_price = 0;
	double discount_amt = 0;
	double total_after_disc = 0;
	double tax_amt = 0;
	double payable = 0;

	string customer_name;
	bool is_member = false;

	// get customer's info
	customer(customer_id, customer_name, is_member);

	cout << fixed << setprecision(2);

	cout << "\n";
	cout << "Customer ID  : " << customer_id << endl;
	cout << "Customer Name: " << customer_name << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << left
		<< setw(10) << "ID"
		<< setw(25) << "Service Name"
		<< setw(10) << "Quantity"
		<< setw(15) << "Unit Price(RM)"
		<< setw(15) << "Subtotal(RM)" << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	for (int i = 0; i < serv_count; i++)
	{
		// print and calculation of services
		if (serv_list[i].customer_id == customer_id)
		{
			double subtotal = calc_subtotal(serv_list[i].serv_price, serv_list[i].serv_qty);

			total_qty += serv_list[i].serv_qty;
			total_price += subtotal;

			cout << left
				<< setw(10) << serv_list[i].serv_id
				<< setw(25) << serv_list[i].serv_name
				<< setw(10) << serv_list[i].serv_qty
				<< right
				<< setw(15) << serv_list[i].serv_price
				<< setw(15) << subtotal << endl;
		}
	}

	// payment calculations
	discount_amt = calc_discount(total_price, is_member);
	total_after_disc = total_price - discount_amt;
	tax_amt = calc_tax(total_after_disc);
	payable = calc_payable(total_after_disc, tax_amt);

	// data stored for other function use
	pmtResult result = pmt_process(customer_id, bill_id, payable, history, history_count, "Service");

	// summary
	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Total Quantity:" << total_qty << endl;
	cout << "Total Price   :   RM" << total_price << endl;
	cout << "Discount      : - RM" << discount_amt << endl;
	cout << "Tax           :   RM" << tax_amt << endl;
	cout << "Payable       :   RM" << payable << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Change        :   RM" << result.change << endl;

	cout << setfill('=') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Exiting to the Payment Menu ...";
	return result;

}

// 3. Payment for appointment
pmtResult pmt_appmt(string customer_id, int bill_id, histRecord history[], int& history_count)
{
	int total_qty = 0;
	double total_price = 0;
	double discount_amt = 0;
	double total_after_disc = 0;
	double tax_amt = 0;
	double payable = 0;

	string customer_name;
	bool is_member = false;

	// get customer's info
	customer(customer_id, customer_name, is_member);

	cout << fixed << setprecision(2);

	cout << "\n";
	cout << "Customer ID  : " << customer_id << endl;
	cout << "Customer Name: " << customer_name << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << left
		<< setw(10) << "ID"
		<< setw(25) << "Service Name"
		<< setw(10) << "Quantity"
		<< setw(15) << "Unit Price(RM)"
		<< setw(15) << "Subtotal(RM)" << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	for (int i = 0; i < appmt_count; i++)
	{
		//print and calculation for appointment
		if (appmt_list[i].customer_id == customer_id)
		{
			double subtotal = calc_subtotal(appmt_list[i].appmt_price, appmt_list[i].appmt_qty);

			total_qty += appmt_list[i].appmt_qty;
			total_price += subtotal;

			cout << left
				<< setw(10) << appmt_list[i].appmt_id
				<< setw(25) << appmt_list[i].appmt_name
				<< setw(10) << appmt_list[i].appmt_qty
				<< right
				<< setw(15) << appmt_list[i].appmt_price
				<< setw(15) << subtotal << endl;
		}
	}

	// payment detail calculation
	discount_amt = calc_discount(total_price, is_member);
	total_after_disc = total_price - discount_amt;
	tax_amt = calc_tax(total_after_disc);
	payable = calc_payable(total_after_disc, tax_amt);

	// data stored for other function use
	pmtResult result = pmt_process(customer_id, bill_id, payable, history, history_count, "Apponitment");

	//summary
	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Total Quantity:" << total_qty << endl;
	cout << "Total Price   :   RM" << total_price << endl;
	cout << "Discount      : - RM" << discount_amt << endl;
	cout << "Tax           :   RM" << tax_amt << endl;
	cout << "Payable       :   RM" << payable << endl;

	cout << setfill('-') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Change        :   RM" << result.change << endl;

	cout << setfill('=') << setw(75) << "-" << endl;
	cout << setfill(' ') << " ";

	cout << "Exiting to the Payment Menu ...";
	return result;
}

// payment progress
pmtResult pmt_process(string cust_id, int bill_id, double pmt_payable,
	histRecord history[], int& history_count, string payment_type)
{
	pmtResult result;
	result.bill_id = bill_id;
	result.pmt_id = generateID();
	result.status = false;
	result.change = 0;

	char method;
	char pay_comf;
	char retry = 'N';
	string method_used;

	do
	{
		//choicing payment method
		cout << "\n---------- PAYMENT METHOD ----------\n";
		cout << "1. E-Wallet\n";
		cout << "2. Online Banking\n";
		cout << "3. Debit / Credit Card\n";
		cout << "4. Cash\n";
		cout << "Method: ";
		cin >> method;

		switch (method)
		{
		case '1':
			method_used = "E-Wallet";
			break;
		case '2':
			method_used = "Online Banking";
			break;
		case '3':
			method_used = "Debit / Credit Card";
			break;
		case '4':
			method_used = "Cash";
			break;
		default:
			cout << "Input Error. Please re-enter...\n";
			break;
		}
	} while (method < '1' || method > '4');


	// for cash payment
	if (method == '4')
	{
		do
		{
			double pay_amt;

			cout << fixed << setprecision(2);
			cout << "Total Payable      : RM" << pmt_payable << endl;
			cout << "Enter Amount Paying: RM";
			cin >> pay_amt;

			if (pay_amt < 0)
			{
				cout << "Invalid payment amount.\n";
				pay_comf = '1';
			}
			else
			{
				result.change = pay_amt - pmt_payable;

				if (result.change < 0)
				{
					// solution for paid amount shortage
					cout << "Amount Shortage!\n";
					cout << "1. Re-enter Amount\n";
					cout << "2. Cancel Payment\n";
					cout << "Choice: ";
					cin >> pay_comf;

					while (pay_comf != '1' && pay_comf != '2')
					{
						cout << "Invalid choice. Please re-enter...";
						cin >> pay_comf;
					}

					if (pay_comf == '2') // cancelled payment 
					{
						cout << "Cancellation Succeed...\n";

						pmt_history(result.bill_id, cust_id, result.pmt_id, pay_amt, pmt_payable, result.change, 
							method_used, result.status, payment_type, history, history_count);

						return result;
					}
				}
				else
				{
					result.status = true;
					pay_comf = '0';
				}
			}
		} while (pay_comf == '1');
	}

	//for non-cash payment
	else
	{
		do
		{
			cout << fixed << setprecision(2);
			cout << "\nTotal Payable : RM" << pmt_payable << endl;

			// payment confirmation
			cout << "Confirm payment? (Y/N): ";
			cin >> pay_comf;

			while (tolower(pay_comf) != 'y' && tolower(pay_comf) != 'n')
			{
				cout << "Invalid choice. Please enter Y or N: ";
				cin >> pay_comf;
			}

			if (tolower(pay_comf) == 'y')
			{
				result.status = true;
			}
			else
			{
				// solution for failed payment
				result.status = false;

				cout << "Payment failure. \n";
				cout << "Do you want to retry payment (Y?N): ";
				cin >> retry;

				while (tolower(retry) != 'y' && tolower(retry) != 'n')
				{
					cout << "Invalid choice. Please enter Y or N: ";
					cin >> retry;
				}

				if (tolower(retry) == 'y')
				{
					return pmt_process(cust_id, bill_id, pmt_payable, history, history_count, payment_type);
				}
				else
				{
					pmt_history(result.bill_id, cust_id, result.pmt_id, 0, pmt_payable, 0, method_used,
						false, payment_type, history, history_count);
					return result;
				}
			}
		} while (result.status == false);
	}

	// save successed payment
	pmt_history(result.bill_id, cust_id, result.pmt_id, pmt_payable, pmt_payable, result.change,
		method_used, result.status, payment_type, history, history_count);

	return result;

}

void logo() {
	cout << endl << setfill('=') << setw(75) << "=" << endl;
	cout << setfill(' ') << "\t\t\tGG SOLON" << endl;
	cout << "Address: Lot 123, Jalan Butik, 55330 Kuala Lumpur, Malaysia" << endl;
	cout << left << setw(35) << "Phone Number: 09-421 2333";
	cout << setw(35) << "Register Number: 201901000005" << endl;
	cout << setfill('=') << setw(75) << "=" << endl << setfill(' ');
}

// generating receipt
void receipt(int bill_id, int pmt_id, double change, string cust_id,
	histRecord history[], int& history_count)
{
	string customer_name;
	bool is_member = false;

	// get customer's info
	customer(cust_id, customer_name, is_member);

	int total_qty = 0;
	double total_price = 0;
	double total_discount = 0;
	double total_tax = 0;
	double total_payable = 0;
	double total_paid = 0;
	double total_change = 0;

	bool found = false;
	bool service_printed = false;
	bool appointment_printed = false;

	logo();

	// print receipt info
	cout << "Receipt Number: " << bill_id << endl;
	cout << "Customer ID   : " << cust_id << endl;
	cout << "Customer Name : " << customer_name << endl;

	cout << setfill('=') << setw(90) << "=" << endl;
	cout << setfill(' ');

	cout << left
		<< setw(10) << "ID"
		<< setw(45) << "Item"
		<< setw(12) << "Quantity"
		<< setw(18) << "Unit Price(RM)"
		<< setw(15) << "Subtotal(RM)"
		<< endl;

	cout << setfill('-') << setw(90) << "-" << endl;
	cout << setfill(' ');

	// receipt purchased item printing
	for (int i = 0; i < history_count; i++)
	{
		if (history[i].customer_id == cust_id &&
			history[i].bill_id == bill_id &&
			history[i].payment_status == "Success")
		{
			found = true;

			// membership
			if (history[i].payment_type == "Membership")
			{
				cout << left
					<< setw(10) << "-"
					<< setw(45) << "Membership registration / renew"
					<< setw(12) << "-"
					<< setw(18) << fixed << setprecision(2)
					<< history[i].payment_amt
					<< setw(15) << history[i].payment_amt
					<< endl;

				total_price += history[i].payment_amt;
				total_payable += history[i].payment_amt;
				total_paid += history[i].payment_amt;
				total_change += history[i].change;
			}

			// service
			else if (history[i].payment_type == "Service" &&
				service_printed == false)
			{
				service_printed = true;

				// listing service purchased
				for (int j = 0; j < serv_count; j++)
				{
					if (serv_list[j].customer_id == cust_id)
					{
						double subtotal = calc_subtotal(
							serv_list[j].serv_price,
							serv_list[j].serv_qty
						);

						cout << left
							<< setw(10) << serv_list[j].serv_id
							<< setw(45) << serv_list[j].serv_name
							<< setw(12) << serv_list[j].serv_qty
							<< setw(18) << fixed << setprecision(2)
							<< serv_list[j].serv_price
							<< setw(15) << subtotal
							<< endl;

						total_qty += serv_list[j].serv_qty;
						total_price += subtotal;
					}
				}

				double service_price = 0;
				int service_qty = 0;

				for (int j = 0; j < serv_count; j++)
				{
					if (serv_list[j].customer_id == cust_id)
					{
						service_price += calc_subtotal(serv_list[j].serv_price, serv_list[j].serv_qty);
						service_qty += serv_list[j].serv_qty;
					}
				}

				// individual item calcalations
				double service_discount = calc_discount(service_price, is_member);
				double service_after_discount = service_price - service_discount;
				double service_tax = calc_tax(service_after_discount);
				double service_payable = calc_payable(service_after_discount, service_tax);

				// summary calculations
				total_discount += service_discount;
				total_tax += service_tax;
				total_payable += service_payable;
				total_paid += history[i].payment_amt;
				total_change += history[i].change;
			}

			// appointment
			else if (history[i].payment_type == "Appointment" &&
				appointment_printed == false)
			{
				appointment_printed = true;

				// listing appointment purchased
				for (int j = 0; j < appmt_count; j++)
				{
					if (appmt_list[j].customer_id == cust_id)
					{
						double subtotal = calc_subtotal(
							appmt_list[j].appmt_price,
							appmt_list[j].appmt_qty
						);

						cout << left
							<< setw(10) << appmt_list[j].appmt_id
							<< setw(45) << appmt_list[j].appmt_name
							<< setw(12) << appmt_list[j].appmt_qty
							<< setw(18) << fixed << setprecision(2)
							<< appmt_list[j].appmt_price
							<< setw(15) << subtotal
							<< endl;

						total_qty += appmt_list[j].appmt_qty;
						total_price += subtotal;
					}
				}

				double appointment_price = 0;
				int appointment_qty = 0;

				for (int j = 0; j < appmt_count; j++)
				{
					if (appmt_list[j].customer_id == cust_id)
					{
						appointment_price += calc_subtotal(
							appmt_list[j].appmt_price,
							appmt_list[j].appmt_qty
						);

						appointment_qty += appmt_list[j].appmt_qty;
					}
				}

				// individual item calculations
				double appointment_discount = calc_discount(appointment_price, is_member);
				double appointment_after_discount = appointment_price - appointment_discount;
				double appointment_tax = calc_tax(appointment_after_discount);
				double appointment_payable = calc_payable(appointment_after_discount, appointment_tax);

				// summary calculations
				total_discount += appointment_discount;
				total_tax += appointment_tax;
				total_payable += appointment_payable;
				total_paid += history[i].payment_amt;
				total_change += history[i].change;
			}
		}
	}

	// total summary
	cout << setfill('-') << setw(90) << "-" << endl;
	cout << setfill(' ');

	cout << fixed << setprecision(2);

	cout << "Total Quantity : " << total_qty << endl;
	cout << "Total Price    : RM" << total_price << endl;
	cout << "Discount       : - RM" << total_discount << endl;
	cout << "Tax            : RM" << total_tax << endl;
	cout << "Payable        : RM" << total_payable << endl;

	cout << setfill('=') << setw(90) << "=" << endl;
	cout << setfill(' ');

	cout << "\nThank You For Purchasing....." << endl;
	cout << "Exiting Payment Process....." << endl;
}

// generate invoice
void invoice(int bill_id, int pmt_id, double change, string cust_id,
	histRecord history[], int& history_count)
{
	string customer_name;
	bool is_member = false;

	customer(cust_id, customer_name, is_member);

	double total_payment = 0;
	bool found = false;

	logo();

	cout << "Invoice Number: " << bill_id << endl;
	cout << "Customer ID   : " << cust_id << endl;
	cout << "Customer Name : " << customer_name << endl;

	cout << setfill('-') << setw(80) << "-" << endl;
	cout << setfill(' ');

	cout << left
		<< setw(15) << "Payment ID"
		<< setw(20) << "Payment Type"
		<< setw(15) << "Amount(RM)"
		<< setw(25) << "Payment Method"
		<< setw(12) << "Status"
		<< endl;

	cout << setfill('-') << setw(80) << "-" << endl;
	cout << setfill(' ');

	// Print only successful payments
	for (int i = 0; i < history_count; i++)
	{
		if (history[i].customer_id == cust_id &&
			history[i].bill_id == bill_id &&
			history[i].payment_status == "Success")
		{
			found = true;

			cout << left
				<< setw(15) << history[i].payment_id
				<< setw(20) << history[i].payment_type
				<< setw(15) << fixed << setprecision(2)
				<< history[i].payment_amt
				<< setw(25) << history[i].payment_method
				<< setw(12) << history[i].payment_status
				<< endl;

			total_payment += history[i].payment_amt;
		}
	}

	cout << setfill('-') << setw(80) << "-" << endl;
	cout << setfill(' ');

	if (found == false)
	{
		cout << "No successful payment found.\n";
	}
	else
	{
		cout << fixed << setprecision(2);
		cout << "TOTAL PAID: RM" << total_payment << endl;
	}

	cout << setfill('=') << setw(80) << "=" << endl;
	cout << setfill(' ');

	cout << "Please keep this invoice for your records.\n";
}

// payment history
void pmt_history(int bill_id, string customer_id, int pmt_id, double pmt_amt, double payable, 
	double change, string method, bool status, string payment_type,
	histRecord history[], int& history_count)
{
	if (history_count < hist_max)
	{
		history[history_count].bill_id = bill_id;
		history[history_count].customer_id = customer_id;
		history[history_count].payment_id = pmt_id;

		history[history_count].payment_amt = pmt_amt;
		history[history_count].payable = payable;
		history[history_count].change = change;

		history[history_count].payment_method = method;
		history[history_count].payment_type = payment_type;

		if (status == true)
		{
			history[history_count].payment_status = "Success";
		}
		else
		{
			history[history_count].payment_status = "Cancelled";
		}

		history_count++;

		cout << "[Log] Saved Successfully.\n";
	}
	else
	{
		cout << "Save Error: Storage is FULL!\n";
	}
}

// view history
void view_history(histRecord history[], int history_count, bool is_staff, string cust_id)
{
	if (history_count == 0)
	{
		cout << "\n No record found...\n";
		cout << "Exiting the progress...\n";
		return;
	}

	char mode;
	int search_bill_id = 0;
	string search_customer_id;

	// for staff
	if (is_staff == true)
	{
		cout << "\n----- VIEWING MODES -----\n";
		cout << "1. All records\n";
		cout << "2. Search by BILL ID\n";
		cout << "3. Search by Customer ID\n";
		cout << "Choice mode: ";
		cin >> mode;

		while (mode < '1' || mode > '3')
		{
			cout << "Invalid mode. Please re-enter: ";
			cin >> mode;
		}

		if (mode == '2')
		{
			cout << "Enter Bill ID:";
			cin >> search_bill_id;
		}
		else if (mode == '3')
		{
			cout << "Enter Customer ID: ";
			cin >> search_customer_id;
		}

		cout << "\n----- [STAFF] HISTORY RECORDS -----\n";
	}

	// for customers
	else
	{
		cout << "\n----- VIEWING MODES -----\n";
		cout << "1. All records\n";
		cout << "2. Search by BILL ID\n";
		cout << "Choice mode: ";
		cin >> mode;

		while (mode < '1' || mode > '2')
		{
			cout << "Invalid mode. Please re-enter： ";
			cin >> mode;
		}

		if (mode == '2')
		{
			cout << "Enter Bill ID: ";
			cin >> search_bill_id;
		}

		cout << "\n----- [CUSTOMER] HISTORY RECORDS -----\n";
	}

	cout << fixed << setprecision(2);
	cout << left
		<< setw(12) << "Bill ID"
		<< setw(15) << "Payment ID"
		<< setw(16) << "Payable(RM)"
		<< setw(22) << "Payment Method"
		<< setw(12) << "Status" << endl;

	cout << setfill('-') << setw(80) << "-" << endl;
	cout << setfill(' ') << " ";

	bool found = false;

	//diplaying records
	for (int i = 0; i < history_count; i++)
	{
		bool display = true;

		if (is_staff == true)
		{
			// staff
			if (mode == '2' && history[i].bill_id != search_bill_id)
			{
				display = false;
			}
			else if (mode == '3' && history[i].customer_id != search_customer_id)
			{
				display = false;
			}
		}
		else
		{
			// customer
			if (history[i].customer_id != cust_id)
			{
				display = false;
			}
			if (mode == '2' && history[i].bill_id != search_bill_id)
			{
				display = false;
			}
		}

		// print records
		if (display == true)
		{
			found = true;

			cout << fixed << setprecision(2);
			cout << left
				<< setw(12) << history[i].bill_id
				<< setw(15) << history[i].payment_id
				<< setw(16) << history[i].payable
				<< setw(22) << history[i].payment_method
				<< setw(12) << history[i].payment_status << endl;
		}
	}

	if (found == false)
	{
		cout << "No Records Found\n";
	}
}

// calculation functions
double calc_subtotal(double price, int quantity)
{
	return price * quantity;
}

double calc_discount(double total_price, bool is_member)
{
	if (is_member == true)
	{
		return total_price * member_discount;
	}
	else
	{
		return 0;
	}
}

double calc_tax(double total_after_disc)
{
	return total_after_disc * tax;
}

double calc_payable(double total_after_disc, double tax_amount)
{
	return total_after_disc + tax_amount;
}

// ID generation
int generateID()
{
	return rand() % 90000 + 10000;
}

// clearing the input
void clear_input()
{
	cin.clear();
	cin.ignore(1000, '\n');
}

// onwords for testing only
// ============================================================
// STAFF LOGIN
// ============================================================
bool staffLogin()
{
	string username;
	string password;

	cout << "\nStaff Username: ";
	cin >> username;

	cout << "Staff Password: ";
	cin >> password;

	for (int i = 0; i < staff_count; i++)
	{
		if (staff_list[i].username == username &&
			staff_list[i].password == password)
		{
			cout << "Staff Login Successful.\n";
			cout << "Welcome, " << staff_list[i].staff_name << "!\n";
			return true;
		}
	}

	cout << "Invalid staff login.\n";
	return false;
}

// ============================================================
// CUSTOMER LOGIN
// ============================================================
bool custLogin(string& cust_id)
{
	string customer_name;
	bool member;

	cout << "\nCustomer ID: ";
	cin >> cust_id;

	customer(cust_id, customer_name, member);

	if (customer_name == "Unknown")
	{
		cout << "Invalid customer ID.\n";
		return false;
	}

	cout << "Customer Login Successful.\n";
	cout << "Welcome, " << customer_name << "!\n";

	return true;
}