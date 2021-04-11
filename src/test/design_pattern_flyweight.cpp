#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <iostream>

struct SharedState
{
	std::string brand_;
	std::string model_;
	std::string color_;

	SharedState(const std::string& brand
		, const std::string& model
		, const std::string& color)
		: brand_(brand), model_(model), color_(color)
	{}

	friend std::ostream& operator<<(std::ostream& os, const SharedState& ss)
	{
		return os << "[ " << ss.brand_ << " ," << ss.model_ << " ," << ss.color_ << " ]";
	}
};

struct UniqueState
{
	std::string owner_;
	std::string plates_;

	UniqueState(const std::string& owner, const std::string& plates)
		: owner_(owner), plates_(plates)
	{}

	friend std::ostream& operator<<(std::ostream& os, const UniqueState& us)
	{
		return os << "[ " << us.owner_ << " ," << us.plates_ << " ]";
	}
};

class Flyweight
{
private:
	SharedState* shared_state_;

public:
	Flyweight(const SharedState* shared_state) : shared_state_(new SharedState(*shared_state))
	{}

	Flyweight(const Flyweight& other) : shared_state_(new SharedState(*other.shared_state_))
	{}

	~Flyweight()
	{
		delete shared_state_;
	}

	SharedState* shared_state() const
	{
		return shared_state_;
	}

	void Operation(const UniqueState& unique_state) const
	{
		std::cout << "Flyweight: Display shared (" << *shared_state_ << ") and unique (" << unique_state << ") state.\n";
	}
};

class FlyweightFactory
{
private:
	std::unordered_map<std::string, Flyweight> flyweights_;

	std::string GetKey(const SharedState& ss) const
	{
		return ss.brand_ + "_" + ss.model_ + "_" + ss.color_;
	}

public:
	FlyweightFactory(std::initializer_list<SharedState> share_states)
	{
		for (const SharedState& ss : share_states)
		{
			this->flyweights_.insert(std::make_pair<std::string, Flyweight>(this->GetKey(ss), Flyweight(&ss)));
		}
	}

	Flyweight GetFlyweight(const SharedState& shared_state)
	{
		std::string key = this->GetKey(shared_state);
		if (this->flyweights_.find(key) == this->flyweights_.end())
		{
			std::cout << "FlyweightFactory: Can't find a flyweight, creating new one.\n";
			this->flyweights_.insert(std::make_pair(key, Flyweight(&shared_state)));
		}
		else
		{
			std::cout << "FlyweightFactory: Reusing existing flyweight.\n";
		}
		return this->flyweights_.at(key);
	}

	void ListFlyweights() const
	{
		size_t count = this->flyweights_.size();
		std::cout << "\nFlyweightFactory: I have " << count << " flyweights:\n";
		for (std::pair<std::string, Flyweight> pair : this->flyweights_)
		{
			std::cout << pair.first << "\n";
		}
	}
};

void AddCarToPoliceDatabase(
	FlyweightFactory& ff, const std::string& plates, const std::string& owner,
	const std::string& brand, const std::string& model, const std::string& color
)
{
	std::cout << "\nClient: Adding a car to database.\n";
	const Flyweight& flyweight = ff.GetFlyweight({ brand, model, color });
	flyweight.Operation({ plates, owner });
}

BOOST_AUTO_TEST_SUITE(s)

BOOST_AUTO_TEST_CASE(c)
{
	FlyweightFactory* factory
		= new FlyweightFactory({
			{"Chevrolet", "Camaro2018", "pink"}
			, {"Mercedes Benz", "C300", "black"}
			, {"Mercedes Benz", "C500", "red"}
			, {"BMW", "M5", "red"}
			, {"BMW", "X6", "white"}});
	factory->ListFlyweights();

	AddCarToPoliceDatabase(*factory
		, "CL234IR"
		, "James Doe"
		, "BMW"
		, "M5"
		, "red");

	AddCarToPoliceDatabase(*factory
		, "CL234IR"
		, "James Doe"
		, "BMW"
		, "X1"
		, "red");

	factory->ListFlyweights();
	
	delete factory;
}

BOOST_AUTO_TEST_SUITE_END()