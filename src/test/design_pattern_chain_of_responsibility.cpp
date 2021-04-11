#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <iostream>

class Handler
{
public:
	virtual Handler* SetNext(Handler* handler) = 0;
	virtual std::string Handle(std::string request) = 0;
};

class AbstractHandler : public Handler
{
private:
	Handler* next_handler_;

public:
	AbstractHandler() : next_handler_(nullptr)
	{}

	Handler* SetNext(Handler* handler) override
	{
		this->next_handler_ = handler;
		return handler;
	}

	std::string Handle(std::string request) override
	{
		if (this->next_handler_)
		{
			return this->next_handler_->Handle(request);
		}

		return {};
	}
};

class MonkeyHandler : public AbstractHandler
{
public:
	std::string Handle(std::string request) override
	{
		if (request == "Banana")
		{
			return "Monkey: I'll eat the " + request + ".\n";
		}
		else
		{
			return AbstractHandler::Handle(request);
		}
	}
};

class SquirrelHandler : public AbstractHandler
{
public:
	std::string Handle(std::string request) override
	{
		if (request == "Nut")
		{
			return "Squirrel: I'll eat the " + request + ".\n";
		}
		else
		{
			return AbstractHandler::Handle(request);
		}
	}
};

class DogHandler : public AbstractHandler
{
public:
	std::string Handle(std::string request) override
	{
		if (request == "MeatBall")
		{
			return "Dog: I'll eat the " + request + ".\n";
		}
		else
		{
			return AbstractHandler::Handle(request);
		}
	}
};

void ClientCode(Handler& handler)
{
	std::vector<std::string> food = { "Nut", "Banana", "Cup of coffee" };
	for (const std::string& f : food)
	{
		std::cout << "Client: Who wants a " << f << "?\n";
		const std::string result = handler.Handle(f);
		if (!result.empty())
		{
			std::cout << " " << result;
		}
		else
		{
			std::cout << " " << f << " was left untouched.\n";
		}
	}
}

BOOST_AUTO_TEST_SUITE(s)

BOOST_AUTO_TEST_CASE(c)
{
	MonkeyHandler* monkey = new MonkeyHandler;
	SquirrelHandler* squirrel = new SquirrelHandler;
	DogHandler* dog = new DogHandler;
	monkey->SetNext(squirrel)->SetNext(dog);

	std::cout << "Chain: Monkey > Squirrel < Dog\n\n";
	ClientCode(*monkey);
	std::cout << "\n";
	std::cout << "Subchain: Squirrel > Dog\n\n";
	ClientCode(*squirrel);

	delete monkey;
	delete squirrel;
	delete dog;
}

BOOST_AUTO_TEST_SUITE_END()