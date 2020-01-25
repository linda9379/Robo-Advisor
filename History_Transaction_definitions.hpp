#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
	unsigned int month_date, unsigned year_date, bool buy_sell_trans,
	unsigned int number_shares, double trans_amount) {
	this->symbol = ticker_symbol;
	this->day = day_date;
	this->month = month_date;
	this->year = year_date;

	if (buy_sell_trans) {
		this->trans_type = "Buy";
	} else{
		this->trans_type = "Sell";
	}
	this->shares = number_shares;
	this->amount = trans_amount;

	this->trans_id = assigned_trans_id;
	assigned_trans_id++;

	set_acb(0.0);
	set_acb_per_share(0.0);
	set_share_balance (0);
	set_cgl(0.0);

	this->p_next = nullptr;
}

// Destructor
// TASK
//
Transaction::~Transaction() {
}

// TASK
// Overloaded < operator.
//
bool Transaction::operator<(Transaction const &other) {
	bool isLess = false;

	if (this->get_year() < other.get_year()) {
		isLess = true;
	} else if (this->get_month() < other.get_month()
			&& this->get_year() <= other.get_year()) {
		isLess = true;
	} else if (this->get_month() <= other.get_month()
			&& this->get_year() <= other.get_year()
			&& this->get_day() < other.get_day()) {
		isLess = true;
	}

	return isLess;
}
// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const {
	return symbol;
}
unsigned int Transaction::get_day() const {
	return day;
}
unsigned int Transaction::get_month() const {
	return month;
}
unsigned int Transaction::get_year() const {
	return year;
}
unsigned int Transaction::get_shares() const {
	return shares;
}
double Transaction::get_amount() const {
	return amount;
}
double Transaction::get_acb() const {
	return acb;
}
double Transaction::get_acb_per_share() const {
	return acb_per_share;
}
unsigned int Transaction::get_share_balance() const {
	return share_balance;
}
double Transaction::get_cgl() const {
	return cgl;
}
bool Transaction::get_trans_type() const {
	return (trans_type == "Buy") ? true : false;
}
unsigned int Transaction::get_trans_id() const {
	return trans_id;
}
Transaction* Transaction::get_next() {
	return p_next;
}

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb(double acb_value) {
	acb = acb_value;
}
void Transaction::set_acb_per_share(double acb_share_value) {
	acb_per_share = acb_share_value;
}
void Transaction::set_share_balance(unsigned int bal) {
	share_balance = bal;
}
void Transaction::set_cgl(double value) {
	cgl = value;
}
void Transaction::set_next(Transaction *p_new_next) {
	p_next = p_new_next;
}

// GIVEN
// Print the transaction.
//
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " " << std::setw(4)
			<< get_symbol() << " " << std::setw(4) << get_day() << " "
			<< std::setw(4) << get_month() << " " << std::setw(4) << get_year()
			<< " ";

	if (get_trans_type()) {
		std::cout << "  Buy  ";
	} else {
		std::cout << "  Sell ";
	}

	std::cout << std::setw(4) << get_shares() << " " << std::setw(10)
			<< get_amount() << " " << std::setw(10) << get_acb() << " "
			<< std::setw(4) << get_share_balance() << " " << std::setw(10)
			<< std::setprecision(3) << get_acb_per_share() << " "
			<< std::setw(10) << std::setprecision(3) << get_cgl() << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
History::History(){
	this -> p_head = nullptr;
}

// Destructor
// TASK
//
History::~History() {
	Transaction *p_traverse { p_head };
	while (p_head != nullptr) {
		p_head = p_traverse->get_next();
		delete p_traverse;
		p_traverse = p_head;
	}
}

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
	ece150::open_file();
	ece150::next_trans_entry();
	p_head = new Transaction(ece150::get_trans_symbol(),
			ece150::get_trans_day(), ece150::get_trans_month(),
			ece150::get_trans_year(), ece150::get_trans_type(),
			ece150::get_trans_shares(),ece150::get_trans_amount());
	insert(p_head);

	Transaction *p_current = nullptr;
	while (ece150::next_trans_entry()) {

		 p_current = new Transaction(ece150::get_trans_symbol(),
				ece150::get_trans_day(), ece150::get_trans_month(),
				ece150::get_trans_year(),ece150::get_trans_type(),
				ece150::get_trans_shares(), ece150::get_trans_amount());
		insert(p_current);
	}
	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
	Transaction *p_current_node {get_p_head()};
	if (p_current_node==nullptr){
		p_head = p_new_trans;
	}else{
		while (p_current_node->get_next() != nullptr) {
			p_current_node = p_current_node->get_next();
		}
		p_current_node -> set_next(p_new_trans);
	}
	p_new_trans -> set_next(nullptr);
}

// TASK
// sort_by_date(): Sort the linked list by trade date.
//

void History::sort_by_date(){
	Transaction *p_current_node {p_head};
    Transaction *new_head{nullptr};
    Transaction*looped_trans{nullptr};
	std::size_t counter {0};
	bool found = false;

	//Check if the current list is empty
    while(p_head!=nullptr){
    	//If the current list is not empty, then set the current node to p_head
    	p_current_node = p_head;

    	//Remove p_head and set it to the next node in the current list
    	p_head = p_head->get_next();

    	//Check if the new sorted list is empty
    	if (new_head==nullptr){
    		//If the sorted list is empty, set the new head to the current node
    		new_head = p_current_node;
    		new_head ->set_next(nullptr);
    	}else{
    		//If the list is not empty, set a looped trans to go through the sorted list
    		looped_trans = new_head;

    		//Check if the removed head from the current list is less than the looped trans
    		if (*p_current_node < *looped_trans){
    			//If it is, then set the removed element to be the new head and the old head to be the next node
    		    p_current_node->set_next(new_head);
    		    new_head = p_current_node;
    		}else{
    			//If not then loop through to find where the removed element goes by running through the list with a boolean flag
    			counter = 0;
    			//Initialize boolean flag for if found the placement of the removed element to be false
    			found = false;
    			//Count the number of elements in the list
    			while (looped_trans!=nullptr){
    				looped_trans = looped_trans->get_next();
    				++counter;
    			}

    			looped_trans = new_head;

    			for(std::size_t i{0}; i < counter-1; ++i){
    				// Check if the next element is less than the removed element
    				if (*(looped_trans->get_next())< *p_current_node){
    					looped_trans = looped_trans->get_next();
    				}else{
    					//If found where the element goes then insert the element into the list
    					p_current_node->set_next(looped_trans->get_next());
    					looped_trans ->set_next(p_current_node);
    					// Set flag to true and exit the loop
    				    found = true;
    				    break;
    				}
    			}
    			// If did not find where the element goes in the list then it must be the last element in the sorted list
    			// so set the removed element to be the last element in the list
    			if (!found){
    				p_current_node->set_next(looped_trans->get_next());
    				looped_trans ->set_next(p_current_node);
    			}
        	}
    	}

    }
    // Set the p_head to be the new head of the sorted list
    p_head=new_head;
}

// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//
void History::update_acb_cgl(){
	Transaction *p_current_node {get_p_head()};
	Transaction *p_previous_node {get_p_head()};
	double abc {0.0};
	unsigned int share{0};
	double abc_per_share{0.0};
	double cgl {0.0};

	while (p_current_node!=nullptr){
		if (p_current_node->get_trans_type() == true){
			abc += p_current_node->get_amount();
			share +=p_current_node->get_shares();
			abc_per_share = abc/share;
			cgl = 0.0;
		}else{
			abc = abc - (p_current_node->get_shares()*p_previous_node->get_acb_per_share());
			share = share - p_current_node->get_shares();
			abc_per_share = abc/share;
			cgl = p_current_node->get_amount()-(p_current_node->get_shares()*p_previous_node->get_acb_per_share());
		}
		p_current_node->set_acb(abc);
		p_current_node ->set_share_balance(share);
		p_current_node ->set_acb_per_share(abc_per_share);
		p_current_node ->set_cgl(cgl);
		p_previous_node = p_current_node;
		p_current_node = p_current_node->get_next();

	}

}
// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year){
	double total_clg {0.0};
	Transaction*p_current {get_p_head()};
	while(p_current != nullptr&& p_current->get_year()!=year){
		p_current = p_current->get_next();
	}
	while (p_current != nullptr && p_current->get_year()==year) {
		total_clg += p_current->get_cgl();
		p_current = p_current->get_next();
	}
	return total_clg;
}

// TASK
// print() Print the transaction history.
//

void History::print() {
	std::cout << "===== BEGIN TRANSACTION HISTORY =====" << std::endl;

	Transaction *p_traverse { get_p_head()};
	while (p_traverse != nullptr) {
		p_traverse->print();
		p_traverse = p_traverse->get_next();
	}
	p_traverse = nullptr;
	std::cout << "===== END TRANSACTION HISTORY =====" << std::endl;

}

// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction* History::get_p_head() {
	return p_head;
}

#endif

