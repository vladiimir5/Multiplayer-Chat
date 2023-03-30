#pragma once
#include <vector>
#include <exception>
#include <memory>
#include "Message.h"

struct UserLoginExp : public std::exception
{
	const char* what() const noexcept override { return "login is already in use"; }
};

class Chat
{
	bool isChatWork_ = false;
	std::vector<User> Users_;
	std::vector<Message> Messages_;
	std::shared_ptr<User> currentUser_ = nullptr;

	void login();
	void signUp();
	void showChat() const;
	void showAllUsersName() const;
	void addMessage();
	std::vector<User>& getAllUsers() { return Users_; }
	std::vector<Message>& getAllMessages() { return Messages_; }
	std::shared_ptr<User> getUserByLogin(const std::string& login) const;
	std::shared_ptr<User> getUserByName(const std::string& name) const;

public:
	void start();
	bool isChatWork() const { return isChatWork_; }
	std::shared_ptr<User> getCurrentUser() const { return currentUser_; }
	void showLoginMenu();
	void showUserMenu();
};