#include <iostream>
#include "Chat.h"

void Chat::start() { isChatWork_ = true; }

std::shared_ptr<User> Chat::getUserByLogin(const std::string& login) const
{
	for (auto &user : Users_)
	{
		if (login == user.getUserLogin())
		{
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

std::shared_ptr<User> Chat::getUserByName(const std::string& name) const
{
	for (auto &user : Users_)
	{
		if (name == user.getUserName())
		{
			return std::make_shared<User>(user);
		}
	}
	return nullptr;
}

void Chat::login()
{
	std::string login, password;
	char operation;

	do
	{
		std::cout << "login: ";
		std::cin >> login;
		std::cout << "password: ";
		std::cin >> password;

		currentUser_ = getUserByLogin(login);
		
		if (currentUser_ == nullptr || (password != currentUser_->getUserPassword()))
		{
			currentUser_ = nullptr;

			std::cout << "failed to login" << std::endl;
			std::cout << "0 - exit or any button - repeat:";
			std::cin >> operation;

			if (operation == '0') { break; }
		}
	} while (!currentUser_);
}

void Chat::signUp()
{
	std::string login, password, name;

	std::cout << "login: ";
	std::cin >> login;
	std::cout << "password: ";
	std::cin >> password;
	std::cout << "name: ";
	std::cin >> name;

	if (getUserByLogin(login) || login == "all") { throw UserLoginExp(); }

	User user = User(login, password, name);
	Users_.push_back(user);
	currentUser_ = std::make_shared<User>(user);
}

void Chat::showChat() const
{
	std::string from;
	std::string to;

	std::cout << "---Chat---" << std::endl;

	for (auto &mess : Messages_)
	{
		if (currentUser_->getUserLogin() == mess.getFrom() || currentUser_->getUserLogin() == mess.getTo() || mess.getTo() == "all")
		{
			from = (currentUser_->getUserLogin() == mess.getFrom()) ? "me" : getUserByLogin(mess.getFrom())->getUserName();

			if (mess.getTo() == "all") { to = "(all)"; }
			else { to = (currentUser_->getUserLogin() == mess.getTo()) ? "me" : getUserByLogin(mess.getTo())->getUserName(); }

			std::cout << "Message from" << from << "to" << to << std::endl;
			std::cout << "text" << mess.getText() << std::endl;
		}
	}
	std::cout << "----------" << std::endl;
}

void Chat::showLoginMenu()
{
	currentUser_ = nullptr;
	
	char operation;

	do
	{
		std::cout << "1 - Login" << std::endl;
		std::cout << "2 - SignUp" << std::endl;
		std::cout << "0 - Shutdown" << std::endl;
		std::cin >> operation;

		switch (operation)
		{
		case '1': 
		
			login();
			break; 
		
		case '2':
		
			try
			{
				signUp();
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		
		case '0':

			isChatWork_ = false; 

		default: 

			std::cout << "1 or 2 or 0" << std::endl; break; 
		}
	} while (!currentUser_ && isChatWork_);
}

void Chat::showUserMenu()
{
	char operation;

	std::cout << "Hello, " << currentUser_->getUserName() << std::endl;

	while (currentUser_)
	{
		std::cout << "Menu: 1 - Show chat | 2 - Add message | 3 - Users | 0 - Logout";

		std::cout << std::endl;

		std::cin >> operation;

		switch (operation)
		{
		case'1':

			showChat();
			break; 

		case'2': 

			addMessage();
			break; 

		case'3': 

			showAllUsersName();
			break;

		case'0': 

			currentUser_ = nullptr; 
			break; 

		default: 

			std::cout << "there is no such option" << std::endl;
			break; 
		}
	}
}

void Chat::showAllUsersName() const
{
	std::cout << "---Users---" << std::endl;
	for (auto& user : Users_)
	{
		std::cout << user.getUserName();

		if (currentUser_->getUserLogin() == user.getUserLogin()) { std::cout << "(me)"; }

		std::cout << std::endl;
	}
	std::cout << "----------" << std::endl;
}

void Chat::addMessage()
{
	std::string to, text;

	std::cout << "To (name or all): ";
	std::cin >> to;
	std::cout << "Text: ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(to == "all" || getUserByName(to)))
	{
		std::cout << "can not send a message" << to << std::endl;
		return;
	}

	if (to == "all") { Messages_.push_back(Message{ currentUser_->getUserLogin(), "all", text }); }
	else { Messages_.push_back(Message{ currentUser_->getUserLogin(), getUserByName(to)->getUserLogin(), text }); }
}