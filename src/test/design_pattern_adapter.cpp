#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

class Target
{
public:
	virtual ~Target() = default;

	virtual std::string Request() const
	{
		return "Target: The default target's behavior.";
	}
};

class Adaptee
{
public:
	std::string SpecificRequest() const
	{
		return ".eetpadA eht fo roivaheb laicepS";
	}
};

class Adapter : public Target
{
private:
	Adaptee* adaptee_;

public:
	Adapter(Adaptee* adaptee) : adaptee_(adaptee) {}

	std::string Request() const override
	{
		std::string to_reverse = this->adaptee_->SpecificRequest();
		std::reverse(to_reverse.begin(), to_reverse.end());
		return "Adapter: (TRANSLATED) " + to_reverse;
	}
};

void ClientCode(const Target* target)
{
	std::cout << target->Request();
}

BOOST_AUTO_TEST_SUITE(s)

BOOST_AUTO_TEST_CASE(c)
{
	std::cout << "Client: I can work just fine with the Target objects:\n";
	Target* target = new Target;
	ClientCode(target);
	std::cout << "\n\n";
	Adaptee* adaptee = new Adaptee;
	std::cout << "Client: The Adaptee class has a weird interface. See, I don't understand it:\n";
	std::cout << "Adaptee: " << adaptee->SpecificRequest();
	std::cout << "\n\n";
	std::cout << "Client: But I can work with it via the Adapter:\n";
	Adapter* adapter = new Adapter(adaptee);
	ClientCode(adapter);
	std::cout << "\n";

	delete target;
	delete adaptee;
	delete adapter;
}

BOOST_AUTO_TEST_SUITE_END()